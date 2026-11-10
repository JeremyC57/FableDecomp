#ifndef FABLE_ENGINE_CCOMPAREDEFPOINTER_H
#define FABLE_ENGINE_CCOMPAREDEFPOINTER_H
#include "engine/CDefPointer.h"
// Both PDBs name this one-byte class as the armour multimap comparator.
// Native00868f46 compares the pair's pointee addresses with unsigned ordering.
template<class T,class V> class CCompareDefPointer {
public: bool operator()(const CDefPointer<T>& a,const CDefPointer<T>& b) const {return a.Get()<b.Get();}
};
#endif
