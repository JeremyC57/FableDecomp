// CClassFactory::`scalar deleting destructor'(unsigned int flags) -- destroy the class list and the class-type table, run the CBaseClassNonCopyable destructor, free when flags & 1
// Leaf: ??_GCClassFactory@@
// Retail: Fable.exe @ 0x00A38080, 78 bytes (+ int3 padding), thiscall `ret 4`. Slot 0 of the CClassFactory vtable
//   0x0129CF70 (rtti-vtables.tsv; the only reference to the function).
// Callees: list<CClass*>::~list 0x00A376A0, __destroy_aux<CClassTypeInfo*> 0x00A37AF0 (landed separately; given its real
//   body here as a same-TU noinline function so the empty __false_type temporary shares the LIVE flags slot, as
//   retail's `lea eax,[esp+8] ; push eax`), free 0x00BFEA14, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300,
//   operator delete 0x00BFE9BC.
// Label: catalog "CGuiControlTreePane_Destructor" is a fold label.
// Layout: as the landed constructor 0x00A373B0 (FableWin.pdb names at the retail offsets): ClassTypeInfo +0x04 (12-byte
//   STLport vector), MaxClassTypeID +0x10, NoAllocatedClasses +0x14, ClassList +0x18 (4-byte STLport list).
// Shape: a user-declared empty virtual destructor DEFINED OUT OF LINE in this TU re-stores the vtable first and is
//   inlined into the vtable's `??_G` (an in-class `{ }` definition left every member destructor out of line there); members die in reverse order
//   (ClassList through its out-of-line destructor, then the vector: _Destroy over [start, finish) and the
//   null-tested free of the block), then the base destructor, then the flags test and operator delete.

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

namespace _STL {
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
    _STL::list<CClass*> TypeList;                           // +0x00
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

void __cdecl operator delete(void* block);                   // MSVCR71 import thunk 0x00BFE9BC

class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};

namespace _STL {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
// 0x00A37AF0 (separate row; same-TU body so the empty tag can share the live flags slot)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    void resize(unsigned int __new_size);                   // 0x00A38150
};
}

class CClassFactory : public CBaseClassNonCopyable {        // vtable 0x0129CF70
protected:
    CClassFactory(unsigned char maxClassTypeID);            // 0x00A373B0 (defined below: emits the vtable and ??_G)
    virtual ~CClassFactory();
    _STL::vector<CClassTypeInfo> ClassTypeInfo;             // +0x04
    unsigned long                MaxClassTypeID;            // +0x10
    unsigned long                NoAllocatedClasses;        // +0x14
    _STL::list<CClass*>          ClassList;                 // +0x18
};

CClassFactory::~CClassFactory()
{
}

CClassFactory::CClassFactory(unsigned char maxClassTypeID)
    : MaxClassTypeID(maxClassTypeID)
{
    NoAllocatedClasses = 0;
    ClassTypeInfo.resize(MaxClassTypeID);
}