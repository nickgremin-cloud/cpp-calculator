#pragma once

#include "calculator.h"

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QString>

enum class Operation {
    NO_OPERATION,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    POWER
};

inline QString OpToString(Operation op) {
    switch(op) {
    case Operation::NO_OPERATION: return "";
    case Operation::ADDITION: return "+";
    case Operation::DIVISION: return "÷";
    case Operation::MULTIPLICATION: return "×";
    case Operation::SUBTRACTION: return "−";
    case Operation::POWER: return "^";
    default: return "";
    }
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void OnDotClicked();
    void OnPlusMinusClicked();
    void OnBackspaceClicked();
    void OnEqualsClicked();
    void OnClearClicked();
    void OnMemoryStoreClicked();
    void OnMemoryRecallClicked();
    void OnMemoryClearClicked();

private:
    void SetText(const QString& text);
    void AddText(const QString& suffix);
    void SetOperation(Operation op);

    Calculator calculator_;
    QString input_number_;
    Number active_number_;
    Operation current_operation_ = Operation::NO_OPERATION;

    Number memory_value_ = 0.0;
    bool has_memory_ = false;
    bool need_fresh_input_ = false;

    QLabel* l_memory;
    QLabel* l_result;
    QLabel* l_formula;
};
