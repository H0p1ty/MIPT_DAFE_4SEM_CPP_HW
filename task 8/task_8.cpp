#include <iostream>
#include <string>
#include <map>
#include <memory>


// Базовый интерфейс для всех выражений в AST (Abstract Syntax Tree)
// Определяет основные операции, которые должны поддерживать все выражения
class Expression {
public:
    virtual ~Expression() = default;
    
    // Выводит текстовое представление выражения в выходной поток
    virtual void print(std::ostream& os) const = 0;
    
    // Вычисляет значение выражения с использованием переданных переменных
    virtual int evaluate(const std::map<std::string,int>& vars) const = 0;
};

using ExprPtr = std::shared_ptr<Expression>; // Удобный псевдоним для shared_ptr

//------------------------------
// Листовые узлы AST (терминальные выражения)
//------------------------------

// Константное целочисленное значение
class Constant : public Expression {
    int value;
public:
    explicit Constant(int v) : value(v) {}
    
    void print(std::ostream& os) const override { os << value; }
    
    int evaluate(const std::map<std::string,int>&) const override { 
        return value; 
    }
};

// Переменная, значение которой берется из контекста
class Variable : public Expression {
    std::string name;
public:
    explicit Variable(std::string n) : name(std::move(n)) {}
    
    void print(std::ostream& os) const override { os << name; }
    
    int evaluate(const std::map<std::string,int>& vars) const override {
        if (!vars.contains(name)) {
            throw std::logic_error("Variable " + name + " does not exist!");
        }
        return vars.at(name);
    }
};

//------------------------------
// Составные узлы AST (нетерминальные выражения)
//------------------------------

// Базовый класс для всех бинарных операций
class BinaryOp : public Expression {
protected:
    ExprPtr left;   // Левый операнд
    ExprPtr right;  // Правый операнд
    std::string op; // Символ операции (для вывода)
    
public:
    BinaryOp(ExprPtr l, ExprPtr r, std::string operation)
        : left(std::move(l)), right(std::move(r)), op(std::move(operation)) {}
        
    void print(std::ostream& os) const override {
        os << "(";
        left->print(os);
        os << ' ' << op << ' ';
        right->print(os);
        os << ")";
    }
};

// Операция сложения (+)
class Add : public BinaryOp {
public:
    Add(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "+") {}
    
    int evaluate(const std::map<std::string, int>& vars) const override {
        return left->evaluate(vars) + right->evaluate(vars);
    }
};

// Операция вычитания (-)
class Subtract : public BinaryOp {
public:
    Subtract(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "-") {}
    
    int evaluate(const std::map<std::string, int>& vars) const override {
        return left->evaluate(vars) - right->evaluate(vars);
    }
};

// Операция целочисленного деления (//)
class IntegerDivide : public BinaryOp {
public:
    IntegerDivide(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "//") {}
    
    int evaluate(const std::map<std::string, int>& vars) const override {
        int divisor = right->evaluate(vars);
        if (divisor == 0) {
            throw std::logic_error("Division by zero!");
        }
        return left->evaluate(vars) / divisor;
    }
};

// Операция умножения (*)

class Multiply : public BinaryOp {
public:
    Multiply(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "*") {}
    
    int evaluate(const std::map<std::string,int>& vars) const override {
        return left->evaluate(vars) * right->evaluate(vars);
    }
};

//------------------------------
// Фабрика выражений (реализована как Singleton)
// Использует пулы объектов для хранения констант и переменных
//------------------------------
class ExpressionFactory {
    std::map<int, std::weak_ptr<Constant>> constPool;     // Пул констант
    std::map<std::string, std::weak_ptr<Variable>> varPool; // Пул переменных

    // Приватный конструктор для Singleton
    ExpressionFactory() = default;
    
public:
    // Получение экземпляра фабрики
    static ExpressionFactory& instance() {
        static ExpressionFactory factory;
        return factory;
    }


    // Получение константы (с использованием пула)
    ExprPtr getConstant(int v) {
        prune(constPool);
        auto& wp = constPool[v]; // weak_ptr для данного значения
        
        // Пытаемся преобразовать weak_ptr в shared_ptr
        if (auto sp = wp.lock()) {
            return sp; // Возвращаем существующий объект
        }

        // Создаем новый объект, если в пуле нет живого shared_ptr
        auto sp = std::make_shared<Constant>(v);
        wp = sp; // Обновляем weak_ptr в пуле
        return sp;
    }

    // Получение переменной (с использованием пула)
    ExprPtr getVariable(const std::string& name) {
        prune(varPool);
        auto& wp = varPool[name];
        if (auto sp = wp.lock()) return sp;
        
        auto sp = std::make_shared<Variable>(name);
        wp = sp;
        return sp;
    }

    // Очистка пула от "мертвых" weak_ptr
    template<typename K, typename WP>
    void prune(std::map<K, WP>& pool) {
        for (auto it = pool.begin(); it != pool.end(); ) {
            if (it->second.expired()) {
                it = pool.erase(it); // Удаляем записи с истекшими weak_ptr
            } else {
                ++it;
            }
        }
    }
};

//------------------------------
// Пример использования
//------------------------------
int main() {
    auto& factory = ExpressionFactory::instance();

    {
        // Создаем выражение: (2 + x) * 5, где x=3
        auto addition = std::make_shared<Add>(
            factory.getConstant(2),
            factory.getVariable("x")
        );
        auto expr = std::make_shared<Multiply>(
            addition,
            factory.getConstant(5)
        );

        // Устанавливаем контекст переменных
        std::map<std::string,int> context{{"x", 3}};
        
        // Выводим и вычисляем выражение
        expr->print(std::cout);
        std::cout << " = " << expr->evaluate(context) << '\n';
    }

    // Другой пример выражения (не вычисляется в этом примере)
    auto anotherExpr = std::make_shared<Add>(
        factory.getConstant(3),
        factory.getVariable("xx")
    );
}