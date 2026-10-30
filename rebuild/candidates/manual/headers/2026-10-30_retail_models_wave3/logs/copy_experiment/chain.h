// experiment: fully implicit def chain with typed PDCB members
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_DEF_IMPLICIT_COPY
#pragma pack(push, 8)
class CDefString { public: long TablePos; };
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector<T>& operator=(const vector<T>& x);
};
}
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;
    bool Dirty;
};
#pragma pack(pop)
#include "engine/CParentDefClassBase.h"
