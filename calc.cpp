#include "calcClass.h"
#include "ui_calcGui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("logo.ico"));

    QSignalMapper *signalMapper = new QSignalMapper(this);

    signalMapper->setMapping(ui->btn_0, QString("0")); //映射输入的字符
    signalMapper->setMapping(ui->btn_1, QString("1"));
    signalMapper->setMapping(ui->btn_2, QString("2"));
    signalMapper->setMapping(ui->btn_3, QString("3"));
    signalMapper->setMapping(ui->btn_4, QString("4"));
    signalMapper->setMapping(ui->btn_5, QString("5"));
    signalMapper->setMapping(ui->btn_6, QString("6"));
    signalMapper->setMapping(ui->btn_7, QString("7"));
    signalMapper->setMapping(ui->btn_8, QString("8"));
    signalMapper->setMapping(ui->btn_9, QString("9"));
    signalMapper->setMapping(ui->btn_plus, QString("+"));
    signalMapper->setMapping(ui->btn_minus, QString("-"));
    signalMapper->setMapping(ui->btn_muti, QString("*"));
    signalMapper->setMapping(ui->btn_divide, QString("/"));
    signalMapper->setMapping(ui->btn_left_brackets, QString("("));
    signalMapper->setMapping(ui->btn_right_brackets, QString(")"));
    signalMapper->setMapping(ui->btn_dot, QString("."));

    connect(ui->btn_0, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_1, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_2, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_3, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_4, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_5, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_6, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_7, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_8, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_9, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_plus, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_minus, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_muti, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_divide, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_left_brackets, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_right_brackets, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->btn_dot, SIGNAL(clicked()), signalMapper, SLOT(map()));

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(pushCharToScreen(QString)));
    connect(ui->btn_back, SIGNAL(clicked()), this, SLOT(deleteCharInScreen()));  //连接退格
    connect(ui->btn_clear, SIGNAL(clicked()), this, SLOT(clearScreen()));  //连接清除
    connect(ui->btn_equel, SIGNAL(clicked()), this, SLOT(calcTheQuery()));  //连接计算
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushCharToScreen(QString chr){
    if(isLastInvaild) {
        clearScreen(); //如果上次表达式无效，自动清空屏幕
        isLastInvaild = false;
    }
    QString query = ui->screen->toPlainText();
    query.push_back(chr.back());
    ui->screen->setText(query);
}

void MainWindow::calcTheQuery(){
    QString query = ui->screen->toPlainText();
    try{
        std::string result = std::to_string(calc.calc(query.toStdString()));
        std::string::size_type dotIndex = result.rfind('.');
        if(dotIndex!=std::string::npos && result.back()=='0') {
            while (result.size() >= dotIndex + 1  && (result.back()=='0' || result.back()=='.')) { //删掉多余的0和.
                result.pop_back();
            }
        }
        ui->screen->setText(QString::fromStdString(result));
    }
    catch (std::runtime_error) {
        isLastInvaild = true;
        ui->screen->setText("Invaild expression");
    }

}

void MainWindow::deleteCharInScreen(){
    QString query = ui->screen->toPlainText();
    if(query.length() > 0){
        query=query.left(query.length()-1);
        ui->screen->setText(query);
    }
}

void MainWindow::clearScreen(){
    ui->screen->setText(QString(""));
}

void calculator::pushNums() {
    bool hasDot = false;
    double num = 0;
    int digits = 1; //小数点后的位数

    while (true)
    {
        if (curritr != end && *curritr == '.') {
            if (hasDot == true) {
                throw std::runtime_error("Unexcepted dot in number.");
            }
            hasDot = true;
            ++curritr;
            continue;
        }

        if (curritr != end && *curritr <= 57 && *curritr >= 48) {
            if (hasDot) {
                num += (*curritr - 48) / pow(10, digits);
                ++digits;
                ++curritr;
            }
            else {
                num *= 10;
                num += *curritr - 48;
                ++curritr;
            }
            continue;
        }

        nums.push(num); //不是数字或'.'就将数字存入栈中，并退出循环和函数
        return;
    }
}

void calculator::pushSym() {
    symbols.push(*curritr);
    ++curritr;
}

void calculator::calcTheTop() {
    char oper = symbols.pop();
    double num1 = nums.pop();

    if (oper == '$') {
        nums.push(-num1);
        return;
    }

    double num2 = nums.pop();

    if (oper == '+') {
        nums.push(num2 + num1);
        return;
    }
    if (oper == '-') {
        nums.push(num2 - num1);
        return;
    }
    if (oper == '*') {
        nums.push(num2 * num1);
        return;
    }
    if (oper == '/') {
        nums.push(num2 / num1);
        return;
    }
}

double calculator::calc(std::string expression) {
    symbols.resize(expression.size());
    nums.resize(expression.size()); //用表达式的长度构造栈，保证不会溢出

    beg = expression.begin();
    curritr = expression.begin();
    end = expression.end();

    while (curritr != end) //这里想了下不是if else嵌套地狱就是continue地狱，我选择continue地狱，看起来舒服点233
    {
        if (*curritr <= 57 && *curritr >= 48) {
            pushNums();
            continue;
        }

        if (*curritr == '+' || *curritr == '-' || *curritr == '*' || *curritr == '/' || *curritr == '(' || *curritr == ')') {
            if (*curritr == '-') { //如果是-号且前面紧跟着一个运算符，代表负号
                if (curritr == beg || *(curritr - 1) == '+' || *(curritr - 1) == '-' || *(curritr - 1) == '*' || *(curritr - 1) == '/' || *(curritr - 1) == '(') {
                    symbols.push('$'); //以$储存
                    curritr++;
                    continue;
                }
            }

            if (*curritr == ')') {
                while (symbols.top() != '(') {
                    calcTheTop();
                }
                symbols.pop(); //pop掉(
                curritr++; //这里因为不是调用pushSym()，需要手动++
                continue;
            }

            if (symbols.isEmpty() || symbols.top() == '(') {
                pushSym();
                continue;
            }

            if (symbols.top() == '*' || symbols.top() == '/') {
                if (*curritr == '*' || *curritr == '/' || *curritr == '+' || *curritr == '-') {
                    calcTheTop();
                    continue;
                }
                pushSym();
                continue;
            }

            if (symbols.top() == '+' || symbols.top() == '-') {
                if (*curritr == '+' || *curritr == '-') {
                    calcTheTop();
                    continue;
                }
                pushSym();
                continue;
            }

            if (symbols.top() == '$') {
                calcTheTop();
                continue;
            }
        }

        throw std::runtime_error("Unexcepted symbol.");
    }

    while (!symbols.isEmpty()) {
        calcTheTop();
    }

    return nums.top();
}
