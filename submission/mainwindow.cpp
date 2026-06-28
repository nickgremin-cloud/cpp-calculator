#include "mainwindow.h"


#include <QGridLayout>
#include <QVBoxLayout>
#include <QFont>

static QString RemoveTrailingZeroes(const QString &text) {
    for (qsizetype i = 0; i < text.size(); ++i) {
        if (text[i] != '0') {
            return text.mid(i);
        }
    }
    return "";
}

static QString NormalizeNumber(const QString &text) {
    if (text.isEmpty()) {
        return "0";
    }
    if (text.startsWith('.')) {
        return NormalizeNumber("0" + text);
    }
    if (text.startsWith('-')) {
        QString normalized = NormalizeNumber(text.mid(1));
        return normalized == "0" ? "0" : "-" + normalized;
    }
    if (text.startsWith('0') && !text.startsWith("0.")) {
        return NormalizeNumber(RemoveTrailingZeroes(text));
    }
    return text;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {

    setWindowTitle("Calculator");

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* main_layout = new QVBoxLayout(central);
    main_layout->setSpacing(4);
    main_layout->setContentsMargins(4, 4, 4, 4);

    // Labels
    l_memory = new QLabel("", this);
    l_memory->setObjectName("l_memory");
    l_result = new QLabel("0", this);
    l_result->setObjectName("l_result");
    l_result->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    l_formula = new QLabel("", this);
    l_formula->setObjectName("l_formula");
    l_formula->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QFont result_font = l_result->font();
    result_font.setPointSize(result_font.pointSize() + 8);
    l_result->setFont(result_font);

    main_layout->addWidget(l_memory);
    main_layout->addWidget(l_result);
    main_layout->addWidget(l_formula);

    // Grid layout for buttons
    auto* grid = new QGridLayout();
    grid->setSpacing(4);
    main_layout->addLayout(grid);

    auto make_button = [grid](const QString& text, int row, int col) {
        auto* btn = new QPushButton(text);
        btn->setMinimumHeight(40);
        grid->addWidget(btn, row, col);
        return btn;
    };

    // Row 0: MC, MR, MS, C, ±, ⌫
    auto* btn_mc = make_button("MC", 0, 0);
    auto* btn_mr = make_button("MR", 0, 1);
    auto* btn_ms = make_button("MS", 0, 2);
    auto* btn_c  = make_button("C", 0, 3);
    auto* btn_pm = make_button("\xC2\xB1", 0, 4);
    auto* btn_bs = make_button("\xE2\x8C\xAB", 0, 5);

    // Row 1: 7, 8, 9, ÷, xʸ, =
    auto* btn_7 = make_button("7", 1, 0);
    auto* btn_8 = make_button("8", 1, 1);
    auto* btn_9 = make_button("9", 1, 2);
    auto* btn_div = make_button("\xC3\xB7", 1, 3);
    auto* btn_pow = make_button("x\xCA\xB8", 1, 4);
    auto* btn_eq  = make_button("=", 1, 5);

    // Row 2: 4, 5, 6, ×
    auto* btn_4 = make_button("4", 2, 0);
    auto* btn_5 = make_button("5", 2, 1);
    auto* btn_6 = make_button("6", 2, 2);
    auto* btn_mul = make_button("\xC3\x97", 2, 3);

    // Row 3: 1, 2, 3, −
    auto* btn_1 = make_button("1", 3, 0);
    auto* btn_2 = make_button("2", 3, 1);
    auto* btn_3 = make_button("3", 3, 2);
    auto* btn_sub = make_button("\xE2\x88\x92", 3, 3);

    // Row 4: 0, ., +
    auto* btn_0 = make_button("0", 4, 0);
    auto* btn_dot = make_button(".", 4, 2);
    auto* btn_add = make_button("+", 4, 3);

    // Make = span rows 1-4
    grid->addWidget(btn_eq, 1, 5, 4, 1);

    // Set column and row stretches
    for (int i = 0; i < 6; ++i) {
        grid->setColumnStretch(i, 1);
    }
    for (int i = 0; i < 5; ++i) {
        grid->setRowStretch(i, 1);
    }

    // Connect digit buttons
    auto connect_digit = [this](QPushButton* btn, const QString& digit) {
        connect(btn, &QPushButton::clicked, this, [this, digit]() {
            if (need_fresh_input_) {
                input_number_ = "";
                active_number_ = 0;
                need_fresh_input_ = false;
            }
            if (current_operation_ == Operation::NO_OPERATION) {
                l_formula->setText("");
            }
            if (input_number_.isEmpty()) {
                input_number_ = QString::number(active_number_);
            }
            AddText(digit);
        });
    };

    connect_digit(btn_0, "0");
    connect_digit(btn_1, "1");
    connect_digit(btn_2, "2");
    connect_digit(btn_3, "3");
    connect_digit(btn_4, "4");
    connect_digit(btn_5, "5");
    connect_digit(btn_6, "6");
    connect_digit(btn_7, "7");
    connect_digit(btn_8, "8");
    connect_digit(btn_9, "9");

    // Connect dot
    connect(btn_dot, &QPushButton::clicked, this, &MainWindow::OnDotClicked);

    // Connect ±
    connect(btn_pm, &QPushButton::clicked, this, &MainWindow::OnPlusMinusClicked);

    // Connect ⌫
    connect(btn_bs, &QPushButton::clicked, this, &MainWindow::OnBackspaceClicked);

    // Connect operations
    auto connect_op = [this](QPushButton* btn, Operation op) {
        connect(btn, &QPushButton::clicked, this, [this, op]() {
            SetOperation(op);
        });
    };

    connect_op(btn_add, Operation::ADDITION);
    connect_op(btn_sub, Operation::SUBTRACTION);
    connect_op(btn_mul, Operation::MULTIPLICATION);
    connect_op(btn_div, Operation::DIVISION);
    connect_op(btn_pow, Operation::POWER);

    // Connect =
    connect(btn_eq, &QPushButton::clicked, this, &MainWindow::OnEqualsClicked);

    // Connect C
    connect(btn_c, &QPushButton::clicked, this, &MainWindow::OnClearClicked);

    // Connect memory buttons
    connect(btn_ms, &QPushButton::clicked, this, &MainWindow::OnMemoryStoreClicked);
    connect(btn_mr, &QPushButton::clicked, this, &MainWindow::OnMemoryRecallClicked);
    connect(btn_mc, &QPushButton::clicked, this, &MainWindow::OnMemoryClearClicked);

    SetText("0");
}

MainWindow::~MainWindow() {}

void MainWindow::SetText(const QString& text) {
    input_number_ = NormalizeNumber(text);
    active_number_ = input_number_.toDouble();
    l_result->setText(input_number_);
}

void MainWindow::AddText(const QString& suffix) {
    if (input_number_.isEmpty()) {
        input_number_ = QString::number(active_number_);
    }
    SetText(input_number_ + suffix);
}

void MainWindow::SetOperation(Operation op) {
    if (current_operation_ == Operation::NO_OPERATION) {
        calculator_.Set(active_number_);
    } else if (!need_fresh_input_) {
        switch (current_operation_) {
        case Operation::ADDITION:       calculator_.Add(active_number_); break;
        case Operation::SUBTRACTION:    calculator_.Sub(active_number_); break;
        case Operation::MULTIPLICATION: calculator_.Mul(active_number_); break;
        case Operation::DIVISION:       calculator_.Div(active_number_); break;
        case Operation::POWER:          calculator_.Pow(active_number_); break;
        default: break;
        }
    }
    current_operation_ = op;
    l_formula->setText(QString::number(calculator_.GetNumber()) + " " + OpToString(op));
    input_number_ = "";
    need_fresh_input_ = true;
}

void MainWindow::OnDotClicked() {
    if (need_fresh_input_) {
        input_number_ = "";
        active_number_ = 0;
        need_fresh_input_ = false;
    }
    if (current_operation_ == Operation::NO_OPERATION) {
        l_formula->setText("");
    }
    if (input_number_.isEmpty()) {
        input_number_ = QString::number(active_number_);
    }
    if (!input_number_.contains('.')) {
        AddText(".");
    }
}

void MainWindow::OnPlusMinusClicked() {
    if (input_number_.isEmpty()) return;
    if (input_number_.startsWith("-")) {
        SetText(input_number_.mid(1));
    } else {
        SetText("-" + input_number_);
    }
}

void MainWindow::OnBackspaceClicked() {
    if (!input_number_.isEmpty()) {
        input_number_.chop(1);
        SetText(input_number_);
    }
}

void MainWindow::OnEqualsClicked() {
    if (current_operation_ == Operation::NO_OPERATION) return;

    l_formula->setText(QString::number(calculator_.GetNumber()) + " " +
                       OpToString(current_operation_) + " " +
                       QString::number(active_number_) + " =");

    switch (current_operation_) {
    case Operation::ADDITION:       calculator_.Add(active_number_); break;
    case Operation::SUBTRACTION:    calculator_.Sub(active_number_); break;
    case Operation::MULTIPLICATION: calculator_.Mul(active_number_); break;
    case Operation::DIVISION:       calculator_.Div(active_number_); break;
    case Operation::POWER:          calculator_.Pow(active_number_); break;
    default: break;
    }

    active_number_ = calculator_.GetNumber();
    l_result->setText(QString::number(active_number_));
    input_number_ = "";
    current_operation_ = Operation::NO_OPERATION;
    need_fresh_input_ = true;
}

void MainWindow::OnClearClicked() {
    current_operation_ = Operation::NO_OPERATION;
    l_formula->setText("");
    SetText("0");
}

void MainWindow::OnMemoryStoreClicked() {
    memory_value_ = active_number_;
    has_memory_ = true;
    l_memory->setText("M");
}

void MainWindow::OnMemoryRecallClicked() {
    if (!has_memory_) return;
    if (current_operation_ == Operation::NO_OPERATION) {
        l_formula->setText("");
    }
    active_number_ = memory_value_;
    l_result->setText(QString::number(active_number_));
    input_number_ = "";
}

void MainWindow::OnMemoryClearClicked() {
    has_memory_ = false;
    l_memory->setText("");
}
