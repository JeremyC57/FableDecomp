// std::vector::resize(unsigned int new_size) -- STLport vector<CClassTypeInfo>::resize(n): resize(n, CClassTypeInfo()) with a default class-type record built on the stack and destroyed after
// Leaf: ?resize@?$vector@VCClassTypeInfo@@V?$allocator@VCClassTypeInfo@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe @ 0x00A38150, 100 bytes, thiscall `ret 4`.
// Callers: the CClassFactory constructors 0x00A37360 / 0x00A373B0 (`ClassTypeInfo.resize(MaxClassTypeID)`).
// Callees: malloc 0x00BFEA0E (the TypeList sentinel), CCharString() 0x0099E4B0, vector<CClassTypeInfo>::resize(n, x)
//   0x00A380E0, ~CCharString 0x0099EAE0, list<CClass*>::~list 0x00A376A0.
// Label: catalog "CTCInventoryMap_InitMiniMapMarkers" is a fold label; the landed CClassFactory constructor 0x00A373B0
//   already names this callee vector<CClassTypeInfo>::resize(unsigned long).
// Shape: STLport 4.6 `void resize(size_type n) { resize(n, _Tp()); }` -- the CClassTypeInfo default constructor inline
//   (list proxy nulled, sentinel malloc'd and self-linked, Name constructed, then Used / NoAllocated / Local / AllocFunc
//   zeroed in body order from one ebx zero), the temporary's implicit destructor (Name, then TypeList) after the call.

extern "C" void* __cdecl malloc(unsigned int size);         // MSVCR71 import thunk 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // MSVCR71 import thunk 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CClass;

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x00A380E0
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}

template void std::vector<CClassTypeInfo>::resize(unsigned int);