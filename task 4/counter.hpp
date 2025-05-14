#ifndef COUNTER_H
#define COUNTER_H

namespace task_4 {
    template<class T>
    unsigned int count_val_ = 0;
}


template<class D>
struct counter {
    counter() {
        ++task_4::count_val_<D>;
    }

    ~counter() {
        --task_4::count_val_<D>;
    }

    static unsigned int count() {
        return task_4::count_val_<D>;
    }
};

#endif //COUNTER_H
