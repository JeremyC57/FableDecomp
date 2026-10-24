// CClassFactory::RegisterAllocatedClass(CClass& cls, long type, bool atBack) -- link a newly allocated class into the factory-wide class list and its type's list (both at the back or both at the front), remember both list positions in the class, bump both counts
// Leaf: RegisterAllocatedClass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A371F0, 219 bytes (+ int3 padding), `ret 0Ch`.
// Identity: Ego_r.pdb `?RegisterAllocatedClass@CClassFactory@@IAEXAAVCClass@@J_N@Z`; called by the landed
//   ReregisterAllocatedClass 0x00A37300 (which names it). The catalog's "CClass_AddAllocation" is descriptive.
// Callees: malloc @ 0x00BFEA0E (STLport list nodes of 12 bytes: next, prev, CClass*).
// Layout (retail, as the landed 0x00A37300 / 0x00A37880): CClassFactory {vptr, vector<CClassTypeInfo> +4 (first/last/end),
//   MaxClassTypeID +0x10, NoAllocatedClasses +0x14, list<CClass*> ClassList +0x18}; CClassTypeInfo (20 bytes) {TypeList +0,
//   NoAllocated +4, Used +8, Local +9, AllocFunc +0xc, Name +0x10}; CClass {vptr, PData +4, ClassListPos +8, TypeListPos +0xc,
//   Type +0x10}. The engine headers carry the Ego_r layouts (12-byte lists), so the retail classes are modelled here.
// Shape: STLport 4.6 list::insert inlined (`_M_create_node` = malloc + null-checked placement copy of the pointer; the
//   position's prev read into a local before `tmp->next = pos`); push_back/push_front = insert(end()/begin(), x), the
//   position evaluated before the allocation; `--end()` / `begin()` stored in the class; the two `malloc(12)` pushes of the
//   first insert are hoisted above the branch and the TypeListPos store is tail-merged.
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }

namespace _STL {

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};

template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node<T>* n) : _M_node(n) { }
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};

template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;                                 // the sentinel only (retail lists carry no size)

    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* p = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node<T>* n = position._M_node;
        _List_node<T>* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_front(const T& x) { insert(begin(), x); }
    void push_back(const T& x) { insert(end(), x); }
};

}

class CClass;
typedef _STL::list<CClass*> CClassList;

class CClass {
public:
    virtual void OnCreated();
    void*                PData;                         // +0x04
    CClassList::iterator ClassListPos;                  // +0x08
    CClassList::iterator TypeListPos;                   // +0x0c
    unsigned char        Type;                          // +0x10
};

class CClassTypeInfo {                                  // 20 bytes
public:
    CClassList    TypeList;                             // +0x00
    unsigned long NoAllocated;                          // +0x04
    bool          Used;                                 // +0x08
    bool          Local;                                // +0x09
    void*         AllocFunc;                            // +0x0c
    void*         Name;                                 // +0x10 CCharString
};

class CClassFactory {
public:
    virtual void v00();
    CClassTypeInfo* ClassTypeInfo_first;                // +0x04 vector<CClassTypeInfo>
    CClassTypeInfo* ClassTypeInfo_last;                 // +0x08
    CClassTypeInfo* ClassTypeInfo_end;                  // +0x0c
    unsigned long   MaxClassTypeID;                     // +0x10
    unsigned long   NoAllocatedClasses;                 // +0x14
    CClassList      ClassList;                          // +0x18
    void RegisterAllocatedClass(CClass& cls, long type, bool atBack);
};

void CClassFactory::RegisterAllocatedClass(CClass& cls, long type, bool atBack)
{
    CClassTypeInfo& info = ClassTypeInfo_first[type];
    if (atBack) {
        ClassList.push_back(&cls);
        info.TypeList.push_back(&cls);
        cls.ClassListPos = --ClassList.end();
        cls.TypeListPos = --info.TypeList.end();
    }
    else {
        ClassList.push_front(&cls);
        info.TypeList.push_front(&cls);
        cls.ClassListPos = ClassList.begin();
        cls.TypeListPos = info.TypeList.begin();
    }
    ++NoAllocatedClasses;
    ++info.NoAllocated;
}