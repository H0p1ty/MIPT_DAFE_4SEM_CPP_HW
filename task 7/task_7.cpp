#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>

#define DEBUGPRINT


// интерфейс имплементации
class SetImpl {
public:
    virtual ~SetImpl() = default;
    virtual void add(int value) = 0;
    virtual void remove(int value) = 0;
    virtual bool contains(int value) const = 0;
    virtual std::vector<int> elements() const = 0;
};

// имплементация через вектор
class VectorSetImpl : public SetImpl {
    std::vector<int> data;
public:
    void add(int value) override {
        if (!contains(value)) data.push_back(value);
    }
    void remove(int value) override {
        data.erase(std::remove(data.begin(), data.end(), value), data.end());
    }
    bool contains(int value) const override {
        return std::find(data.begin(), data.end(), value) != data.end();
    }
    std::vector<int> elements() const override {
        return data;
    }
};

// имплементация через хеш таблици
class HashSetImpl : public SetImpl {
    std::unordered_set<int> data;
public:
    void add(int value) override {
        data.insert(value);
    }
    void remove(int value) override {
        data.erase(value);
    }
    bool contains(int value) const override {
        return data.contains(value);
    }
    std::vector<int> elements() const override {
        return std::vector<int>(data.begin(), data.end());
    }
};

// абстракция
class Set {
    std::unique_ptr<SetImpl> impl;

    static constexpr size_t kThreshold = 10;
    void SwitchImpl() {
        size_t sz = impl->elements().size();

        bool usingHashNow = dynamic_cast<HashSetImpl*>(impl.get()) != nullptr;

#ifdef DEBUGPRINT
        std::string to = usingHashNow ? "vector" : "hash";
        std::cout << "SWITCHIIIIN TO " << to << " with size: " << sz << std::endl;
#endif

        // проверки тут лишние, но мне лень тестить потом
        if (!usingHashNow && sz > kThreshold) {
            // уходим на хештабло
            auto elems = impl->elements();
            impl = std::make_unique<HashSetImpl>();

            for (int v : elems) impl->add(v);

        } else if (usingHashNow && sz <= kThreshold) {
            // или в вектор
            auto elems = impl->elements();
            impl = std::make_unique<VectorSetImpl>();

            for (int v : elems) impl->add(v);
        }
    }

public:
    Set() : impl(std::make_unique<VectorSetImpl>()) {}

    void add(int value) {
        impl->add(value);
        if (impl->elements().size() == kThreshold + 1) SwitchImpl();
    }
    void remove(int value) {
        impl->remove(value);
        if (impl->elements().size() == kThreshold) SwitchImpl();
    }
    bool contains(int value) const {
        return impl->contains(value);
    }
    Set setUnion(const Set& other) const {
        Set result;
        for (int v : impl->elements()) result.add(v);
        for (int v : other.impl->elements()) result.add(v);
        return result;
    }
    Set setIntersection(const Set& other) const {
        Set result;
        for (int v : impl->elements()) {
            if (other.contains(v)) result.add(v);
        }
        return result;
    }
    void print() const {
        auto elems = impl->elements();
        std::cout << "{";
        for (size_t i = 0; i < elems.size(); ++i) {
            std::cout << elems[i] << (i+1<elems.size()? ", " : "");
        }
        std::cout << "}\n";
    }
};

int main() {
    Set a;
    for (int i = 1; i < 12; ++i) a.add(i);

    a.print();  // тут хештаблица
    a.remove(5);
    a.print();  // переключится на векторную реализацию если size<=10

    Set b;
    b.add(2); b.add(3); b.add(100);

    auto u = a.setUnion(b);  // вот где-то тут напишет про переключение тк создаем новый юнион
    auto inter = a.setIntersection(b);

    std::cout << "Union: "; u.print();
    std::cout << "Intersection: "; inter.print();
}
