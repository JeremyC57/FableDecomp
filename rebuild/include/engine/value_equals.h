#ifndef FABLE_ENGINE_VALUE_EQUALS_H
#define FABLE_ENGINE_VALUE_EQUALS_H
#include "stlport/fable_containers.h"
// Original FableWin constructor, by-value pair predicate and Ego_r Value member.
template<class Key,class T> class value_equals {
public:
    value_equals(T value) : Value(value) {}
    bool operator()(std::pair<Key,T> v) { return v.second == Value; }
private:
    T Value;
};
#endif
