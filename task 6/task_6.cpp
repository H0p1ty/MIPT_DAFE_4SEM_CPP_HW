#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Структура, представляющая контрольную точку (Checkpoint) в маршруте
// Содержит информацию о местоположении и правилах прохождения
struct Checkpoint {
    std::string name;       // Название контрольной точки
    double latitude;        // Географическая широта
    double longitude;       // Географическая долгота
    bool necessary;         // Флаг обязательности точки (true = обязательная)
    double penaltyHours;    // Штрафное время в часах (для необязательных точек)
};

// Базовый интерфейс строителя контрольных точек
// Определяет основные операции для построения различных представлений данных
class CheckpointBuilder {
public:
    virtual ~CheckpointBuilder() = default;
    
    // Сбрасывает состояние строителя перед началом новой сборки
    virtual void reset() = 0;
    
    // Добавляет новую контрольную точку в построение
    virtual void add(const Checkpoint& cp) = 0;
};

// Конкретный строитель для создания текстового списка контрольных точек
// Форматирует данные в удобочитаемый текстовый вид
class TextListBuilder : public CheckpointBuilder {
    std::string output;  // Результирующая строка с форматированным списком
    int index = 0;       // Счетчик номеров точек

public:
    void reset() override {
        output.clear();
        index = 0;
    }

    void add(const Checkpoint& cp) override {
        ++index;
        std::ostringstream oss;
        
        // Форматируем строку с информацией о точке:
        oss << index << ". " << cp.name << " ["
            << std::fixed << std::setprecision(6)  // Фиксируем 6 знаков после запятой
            << cp.latitude << ", " << cp.longitude << "] - ";
            
        if (cp.necessary) {
            oss << "Special Sector failure";  // Для обязательных точек
        } else {
            oss << cp.penaltyHours << " h";   // Для необязательных - штрафное время
        }
        oss << "\n";
        
        output += oss.str();  // Добавляем сформированную строку в результат
    }

    // Возвращает сформированный текстовый список
    const std::string& getOutput() const {
        return output;
    }
};

// Конкретный строитель для расчета суммарного штрафного времени
// Вычисляет общее штрафное время за пропуск необязательных точек
class SumPenaltyBuilder : public CheckpointBuilder {
    double totalPenalty = 0.0;  // Накопитель суммарного штрафа

public:
    void reset() override {
        totalPenalty = 0.0;
    }

    void add(const Checkpoint& cp) override {
        // Учитываем только необязательные точки
        if (!cp.necessary) {
            totalPenalty += cp.penaltyHours;
        }
    }

    // Возвращает общее штрафное время
    double getTotalPenalty() const {
        return totalPenalty;
    }
};

// Директор, который управляет процессом построения
// Шаблонный параметр позволяет использовать с любым типом строителя
// checkpoints Вектор контрольных точек для обработки
// Ссылка на строитель, который будет обрабатывать точки
template <typename Builder>
void constructCheckpoints(const std::vector<Checkpoint>& checkpoints, Builder& builder) {
    builder.reset();  // Подготавливаем строитель к новой сборке
    
    // Последовательно добавляем все точки в строитель
    for (const auto& cp : checkpoints) {
        builder.add(cp);
    }
}

int main() {
    // Тестовый маршрут с контрольными точками
    std::vector<Checkpoint> route = {
        {"Start",        55.755800, 37.617600, true,  0.0},    // Обязательная точка
        {"Intermediate", 55.758000, 37.620000, false, 0.5},    // Необязательная (штраф 0.5 ч)
        {"Finish",       55.760000, 37.630000, true,  0.0}     // Обязательная точка
    };

    // Пример использования TextListBuilder
    TextListBuilder textBuilder;
    constructCheckpoints(route, textBuilder);
    std::cout << "Checkpoint List:\n" << textBuilder.getOutput();

    // Пример использования SumPenaltyBuilder
    SumPenaltyBuilder sumBuilder;
    constructCheckpoints(route, sumBuilder);
    std::cout << "Total penalty for optional checkpoints: "
              << sumBuilder.getTotalPenalty() << " h\n";
}