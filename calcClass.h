#ifndef CALCCLASS_H
#define CALCCLASS_H

#include <QSignalMapper>
#include <QMainWindow>
#include <stdexcept>
#include <cmath>

template<typename type>
class stack {
private:
    type* data;

    size_t length;
public:
    size_t count;

    stack(size_t length) : length(length), count(0) {
        data = new type[length];
        //memset(data, 0, length * sizeof(type));
    }

    ~stack() { delete[] data; }

    void resize(size_t length) {
        delete[] data;
        data = new type[length];
        this->length = length;
        count = 0;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == length;
    }

    type top() {
        if (count <= 0) {
            throw std::runtime_error("Stack is empty");
        }
        return data[count - 1];
    }

    void push(type val) {
        if (count == length) {
            throw std::runtime_error("Stack is full");
        }

        data[count++] = val;
    }

    type pop() {
        if (count <= 0) {
            throw std::runtime_error("Stack is empty");
        }
        type val = data[--count];
        return val;
    }
};


class calculator {
private:
    stack<char> symbols;
    stack<double> nums;

    std::string::iterator beg;
    std::string::iterator curritr;
    std::string::iterator end;

    void pushNums();
    void pushSym();
    void calcTheTop();
public:
    calculator() : symbols(1), nums(1) {};
    double calc(std::string expression);
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    calculator calc;
    Ui::MainWindow *ui;
    bool isLastInvaild = false;

private slots:
    void pushCharToScreen(QString);
    void calcTheQuery();
    void deleteCharInScreen();
    void clearScreen();
};

#endif // CALCCLASS_H
