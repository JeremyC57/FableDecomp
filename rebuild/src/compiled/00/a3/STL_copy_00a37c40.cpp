// std::__copy(CClassTypeInfo* first, CClassTypeInfo* last, CClassTypeInfo* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of class-type records: *result++ = *first++, (last - first) times; returns the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe @ 0x00A37C40, 103 bytes, `ret 0Ch`. /Gr TU written as explicit __fastcall: first in ecx, last in
//   edx, result + the tag reference + the (int*)0 distance pointer on the stack.
// Callers: 0x00A37D0C / 0x00A37DF8 / 0x00A38116 -- the vector<CClassTypeInfo> erase/insert helpers of CClassFactory
//   (0x00A38116 is inside vector<CClassTypeInfo>::resize(n, x) 0x00A380E0).
// Callees: list<CClass*>::operator= 0x00A37940, CCharString::operator= 0x0099EFB0.
// Label: catalog "_Move<CLipSyncPhoneme*,CLipSyncPhoneme*>" is a propagated Ego_r (Dinkumware) label; the element is
//   CClassTypeInfo (CClassFactory::ClassTypeInfo's 20-byte record: TypeList, NoAllocated, Used, Local, AllocFunc, Name).
// Shape: STLport 4.6 _algobase.h `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`
//   -- count (last - first) / 20 by the 66666667h reciprocal; the implicit CClassTypeInfo assignment calls the list
//   assignment, copies the dword / two bools / dword, then the CCharString assignment; the calls keep VC7.1 from
//   unrolling the loop.

extern "C" void* __cdecl malloc(unsigned int size);         // MSVCR71 import thunk 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // MSVCR71 import thunk 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CClass;

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;    // retail list = the sentinel pointer only
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = (_Node*)malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base();                                          // 0x00A376A0 (out of line: clear + free the sentinel)
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    list() : _List_base<_Tp, _Alloc>(_Alloc()) { }
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00A37940
};
}

typedef CClass* (__cdecl* ClassAllocFunc)();

// Retail layout (20 bytes; Ego_r's is 28 because its std::list is 12 bytes), as the landed constructor 0x00A37880.
class CClassTypeInfo {
public:
    std::list<CClass*> TypeList;                           // +0x00
    unsigned long       NoAllocated;                        // +0x04
    bool                Used;                               // +0x08
    bool                Local;                              // +0x09
    ClassAllocFunc      AllocFunc;                          // +0x0C
    CCharString         Name;                               // +0x10
    CClassTypeInfo()
        : TypeList()
        , Name()
    {
        Used = false;
        NoAllocated = 0;
        Local = false;
        AllocFunc = 0;
    }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

template CClassTypeInfo* __fastcall std::__copy<CClassTypeInfo*, CClassTypeInfo*, int>(CClassTypeInfo*, CClassTypeInfo*,
    CClassTypeInfo*, const std::random_access_iterator_tag&, int*);