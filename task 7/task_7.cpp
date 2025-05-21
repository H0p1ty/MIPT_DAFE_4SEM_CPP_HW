#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>

#define DEBUGPRINT

// Базовый интерфейс для реализации множества
// Определяет основные операции работы с множеством
class SetImpl {
public:
    virtual ~SetImpl() = default;
    
    // Добавляет элемент в множество (если его там нет)
    virtual void add(int value) = 0;
    
    // Удаляет элемент из множества
    virtual void remove(int value) = 0;
    
    // Проверяет наличие элемента в множестве
    virtual bool contains(int value) const = 0;
    
    // Возвращает все элементы множества в виде вектора
    virtual std::vector<int> elements() const = 0;
};

// Реализация множества на основе вектора
// Оптимальна для небольших множеств (до 10 элементов)
class VectorSetImpl : public SetImpl {
    std::vector<int> data;  // Хранение элементов в векторе
    
public:
    void add(int value) override {
        if (!contains(value)) {
            data.push_back(value);  // Добавляем только уникальные элементы
        }
    }
    
    void remove(int value) override {
        // Удаляем все вхождения элемента (хотя в множестве их должно быть не более одного)
        data.erase(std::remove(data.begin(), data.end(), value), data.end());
    }
    
    bool contains(int value) const override {
        return std::find(data.begin(), data.end(), value) != data.end();
    }
    
    std::vector<int> elements() const override {
        return data;  // Возвращаем копию вектора
    }
};

// Реализация множества на основе хеш-таблицы
// Оптимальна для больших множеств (более 10 элементов)
class HashSetImpl : public SetImpl {
    std::unordered_set<int> data;  // Хранение элементов в хеш-таблице
    
public:
    void add(int value) override {
        data.insert(value);  // insert автоматически проверяет уникальность
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


// Абстракция множества с автоматическим переключением реализаций
// Переключается между векторной и хеш-табличной реализацией
// при достижении порогового размера (kThreshold)
class Set {
    std::unique_ptr<SetImpl> impl;  // Текущая реализация
    
    // Пороговое значение для переключения реализаций
    static constexpr size_t kThreshold = 10;
    
    // Переключает реализацию при необходимости
    // На основе текущего размера множества
    void SwitchImpl() {
        size_t sz = impl->elements().size();
        bool usingHashNow = dynamic_cast<HashSetImpl*>(impl.get()) != nullptr;

#ifdef DEBUGPRINT
        std::string to = usingHashNow ? "vector" : "hash";
        std::cout << "SWITCHING TO " << to << " implementation with size: " << sz << std::endl;
#endif

        // Переключаемся на хеш-таблицу, если размер превысил порог и сейчас вектор
        if (!usingHashNow && sz > kThreshold) {
            auto elems = impl->elements();
            impl = std::make_unique<HashSetImpl>();
            for (int v : elems) impl->add(v);
        } 
        // Возвращаемся к вектору, если размер уменьшился до порога и сейчас хеш-таблица
        else if (usingHashNow && sz <= kThreshold) {
            auto elems = impl->elements();
            impl = std::make_unique<VectorSetImpl>();
            for (int v : elems) impl->add(v);
        }
    }

public:
    // По умолчанию используем векторную реализацию
    Set() : impl(std::make_unique<VectorSetImpl>()) {}

    void add(int value) {
        impl->add(value);
        // Проверяем необходимость переключения после добавления
        if (impl->elements().size() == kThreshold + 1) {
            SwitchImpl();
        }
    }
    
    void remove(int value) {
        impl->remove(value);
        // Проверяем необходимость переключения после удаления
        if (impl->elements().size() == kThreshold) {
            SwitchImpl();
        }
    }
    
    bool contains(int value) const {
        return impl->contains(value);
    }
    
    // Возвращает объединение двух множеств
    // Создает новое множество, содержащее все элементы из обоих множеств
    Set setUnion(const Set& other) const {
        Set result;
        for (int v : impl->elements()) result.add(v);
        for (int v : other.impl->elements()) result.add(v);
        return result;
    }
    
    // Возвращает пересечение двух множеств
    // Создает новое множество, содержащее только общие элементы
    Set setIntersection(const Set& other) const {
        Set result;
        for (int v : impl->elements()) {
            if (other.contains(v)) {
                result.add(v);
            }
        }
        return result;
    }
    
    // Выводит содержимое множества в удобочитаемом формате
    void print() const {
        auto elems = impl->elements();
        std::cout << "{";
        for (size_t i = 0; i < elems.size(); ++i) {
            std::cout << elems[i] << (i+1 < elems.size() ? ", " : "");
        }
        std::cout << "}\n";
    }
};

int main() {
    Set a;
    // Добавляем элементы, чтобы превысить порог переключения
    for (int i = 1; i < 12; ++i) a.add(i);

    a.print();  // Должна использоваться хеш-табличная реализация
    a.remove(5);
    a.print();  // Должна переключиться на векторную реализацию (size <= 10)

    Set b;
    b.add(2); b.add(3); b.add(100);

    // Операции с множествами
    auto u = a.setUnion(b);
    auto inter = a.setIntersection(b);

    std::cout << "Union: "; u.print();
    std::cout << "Intersection: "; inter.print();
}