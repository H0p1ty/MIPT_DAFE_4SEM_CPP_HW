#include <iostream>
#include <string>
#include <map>
#include <memory>

// интерфейс выражения
class Expression {
public:
    virtual ~Expression() = default;
    virtual void print(std::ostream& os) const = 0;
    virtual int evaluate(const std::map<std::string,int>& vars) const = 0;
};
using ExprPtr = std::shared_ptr<Expression>;

//------------------------------
// листья дерева
//------------------------------
class Constant : public Expression {
    int value;
public:
    explicit Constant(int v) : value(v) {}
    void print(std::ostream& os) const override { os << value; }
    int evaluate(const std::map<std::string,int>&) const override { return value; }
};

class Variable : public Expression {
    std::string name;
public:
    explicit Variable(std::string n) : name(std::move(n)) {}
    void print(std::ostream& os) const override { os << name; }
    int evaluate(const std::map<std::string,int>& vars) const override {
        if (!vars.contains(name)) {
            throw std::logic_error("Variable " + name + " does not exist!");
        }
        auto it = vars.find(name);
        return it->second;
    }
};

//------------------------------
// составные части (узлы)
//------------------------------
class BinaryOp : public Expression {
protected:
    ExprPtr left, right;
    std::string op;
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

class Add : public BinaryOp {
public:
    Add(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "+") {}
    int evaluate(const std::map<std::string, int>& vars) const override {
        return left->evaluate(vars) + right->evaluate(vars);
    }
};

class Subtract : public BinaryOp {
public:
    Subtract(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "-") {}
    int evaluate(const std::map<std::string, int>& vars) const override {
        return left->evaluate(vars) - right->evaluate(vars);
    }
};

class IntegerDivide : public BinaryOp {
public:
    IntegerDivide(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "+") {}
    int evaluate(const std::map<std::string, int>& vars) const override {
        return left->evaluate(vars) / right->evaluate(vars);
    }
};

class Multiply : public BinaryOp {
public:
    Multiply(ExprPtr l, ExprPtr r) : BinaryOp(l, r, "*") {}
    int evaluate(const std::map<std::string,int>& vars) const override {
        return left->evaluate(vars) * right->evaluate(vars);
    }
};

//------------------------------
// fabrika singleton
//------------------------------
class ExpressionFactory {
    std::map<int, std::weak_ptr<Constant>> constPool;
    std::map<std::string, std::weak_ptr<Variable>> varPool;

    ExpressionFactory() = default;
public:
    static ExpressionFactory& instance() {
        static ExpressionFactory factory;
        return factory;
    }

    ExprPtr getConstant(int v) {
        auto& wp = constPool[v]; // вик птр чтоб если что - удалился сам без проблем
        if (auto sp = wp.lock()) // если объект умер - nullptr
            return sp;

        // создаем новый
        auto sp = std::make_shared<Constant>(v);
        wp = sp; // перезапишем на новую ссылку
        return sp;
    }

    ExprPtr getVariable(const std::string& name) {
        auto& wp = varPool[name];
        if (auto sp = wp.lock()) return sp;
        auto sp = std::make_shared<Variable>(name);
        wp = sp;
        return sp;
    }
};

//------------------------------

int main() {
    auto& factory = ExpressionFactory::instance();

    // считаем выражение: (2 + x) * 5, где x=3
    auto addition = std::make_shared<Add>(
        factory.getConstant(2),
        factory.getVariable("x")
    );
    auto expr = std::make_shared<Multiply>(
        addition,
        factory.getConstant(5)
    );

    std::map<std::string,int> context{{"x", 3}};
    expr->print(std::cout);
    std::cout << " = " << expr->evaluate(context) << '\n';
}