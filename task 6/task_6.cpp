#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>  // ШОБЫ ДАБЛ КРУТО БЫЛО

// CP
struct Checkpoint {
    std::string name;
    double latitude;  // я забыл кто из них долгота а кто широта, сложные слова ну и не важно
    double longitude;
    bool necessary;       // true = necessary CP
    double penaltyHours;  // для необяз КП
};

// Builder interface (no need for get() method)
class CheckpointBuilder {
public:
    virtual ~CheckpointBuilder() = default;
    virtual void reset() = 0;
    virtual void add(const Checkpoint& cp) = 0;
};

// ConcreteBuilder для печати списка КП
class TextListBuilder : public CheckpointBuilder {
    std::string output;
    int index = 0;

public:
    void reset() override {
        output.clear();
        index = 0;
    }

    void add(const Checkpoint& cp) override {
        ++index;
        std::ostringstream oss;
        oss << index << ". " << cp.name << " ["
            << std::fixed << std::setprecision(6)
            << cp.latitude << ", " << cp.longitude << "] - ";
        if (cp.necessary) {
            oss << "Special Sector failure";
        } else {
            oss << cp.penaltyHours << " h";
        }
        oss << "\n";
        output += oss.str();
    }

    const std::string& getOutput() const {
        return output;
    }
};

// ConcreteBuilder для суммирования штрафоув
class SumPenaltyBuilder : public CheckpointBuilder {
    double totalPenalty = 0.0;

public:
    void reset() override {
        totalPenalty = 0.0;
    }

    void add(const Checkpoint& cp) override {
        if (!cp.necessary) {
            totalPenalty += cp.penaltyHours;
        }
    }

    double getTotalPenalty() const {
        return totalPenalty;
    }
};

// Director который использует билдер поданный
template <typename Builder>
void constructCheckpoints(const std::vector<Checkpoint>& checkpoints, Builder& builder) {
    builder.reset();
    for (const auto& cp : checkpoints) {
        builder.add(cp);
    }
}

int main() {
    std::vector<Checkpoint> route = {
        {"Start",        55.755800, 37.617600, true,  0.0},
        {"Intermediate", 55.758000, 37.620000, false, 0.5},
        {"Finish",       55.760000, 37.630000, true,  0.0}
    };

    TextListBuilder textBuilder;
    constructCheckpoints(route, textBuilder);
    std::cout << "Checkpoint List:\n" << textBuilder.getOutput();

    SumPenaltyBuilder sumBuilder;
    constructCheckpoints(route, sumBuilder);
    std::cout << "Total penalty for optional checkpoints: "
              << sumBuilder.getTotalPenalty() << " h\n";
}
