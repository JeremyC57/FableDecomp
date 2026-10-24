// CTBaseSingleton<CEngineManager>::Get() -- create the engine manager singleton (empty engine list, empty changed-area vector) on first use, return it
// Leaf: ?Get@?$CTBaseSingleton@VCEngineManager@@@@SIAAVCEngineManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00436FB0, length 112 bytes (105 + int3 padding), `ret` (static __fastcall, no arguments).
// Name: the CTBaseSingleton<T>::Get template of the landed CTBaseSingleton<CConsole>::Get 0x00414C90; the object built (0x14 bytes)
// stores vtable 0x01231584 = CEngineManager (rtti-vtables.tsv); many callers (0x004353A0, 0x004354AF, ... 0x0050233F). The
// catalog's "Get" is right.
// Callees: operator new @ 0x00BFEA1A (0x14), malloc @ 0x00BFEA0E (the list's 12-byte sentinel node -- Fable's STLport node
// allocator), CCountedPointer<CEngineManager>::Reset @ 0x004366B0 (landed generic Reset) on the static Instance at 0x013BA854.
// Shape: /O2 (16-aligned); as 0x00414C90 but CEngineManager's implicit constructor is inlined: the vtable store, then STLport
// list<CIEngine*>'s constructor (`_M_node` = 0, allocate the sentinel, next = prev = itself, store it), then
// vector<C2DBoxI>'s three zero pointers (the last one scheduled after the Reset argument push); `edi` holds the zero for the
// null-tested new; Reset + `return *Instance.Data` are tail-duplicated into both arms. Both containers need their real STLport
// structure (the wave-25 "24i" lesson): the list's node pointer and the vector's end-of-storage pointer live in an
// _STLP_alloc_proxy built from an `allocator_type()` default-argument temporary; with plain members VC schedules all three
// vector zeros after `push esi ; mov ecx` and the node store after `add esp,4` (graded DIFFER 105v105, scheduling only).
// Layout: Ego_r.pdb CEngineManager (32 bytes with a 12-byte list and a 16-byte vector) -> retail 0x14: vptr (CTBaseSingleton),
// Engines +0x04 (STLport list: one node pointer), ChangedAreas +0x08 (STLport vector: 12 bytes). No engine header exists.
// Model: local -- engine/CIEngine.h exists, but only a pointer to it is stored (list<CIEngine*>), so a forward-declared local is
// enough.
#include <stdlib.h>
#include <new>
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* data);                                 // 0x004366B0 for CEngineManager
};
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}
    static CCountedPointer<T> Instance;                  // 0x013BA854 for CEngineManager
    static T& __fastcall Get()
    {
        if (!Instance.Data)
            Instance.Reset(new T);
        return *Instance.Data;
    }
};
namespace std {
template <class T> class allocator {                     // STLport 4.x: empty, malloc-backed in Fable
public:
    allocator() {}
    allocator(const allocator&) {}
    template <class U> allocator(const allocator<U>&) {}
    T* allocate(size_t n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
    Tp* allocate(size_t n) { return MaybeReboundAlloc::allocate(n); }
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T, class Alloc> class _List_base {
public:
    typedef _List_node<T> _Node;
    _List_base(const Alloc& a) : _M_node(allocator<_Node>(a), (_Node*)0)
    {
        _Node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00
};
template <class T, class Alloc = allocator<T> > class list : public _List_base<T, Alloc> {
public:
    typedef Alloc allocator_type;
    explicit list(const allocator_type& a = allocator_type()) : _List_base<T, Alloc>(a) {}
};
template <class T, class Alloc> class _Vector_base {
public:
    _Vector_base(const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) {}
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;   // +0x08
};
template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    typedef Alloc allocator_type;
    explicit vector(const allocator_type& a = allocator_type()) : _Vector_base<T, Alloc>(a) {}
};
}
class CIEngine;
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
};
class CEngineManager : public CTBaseSingleton<CEngineManager> {   // vtable 0x01231584
public:
    std::list<CIEngine*>   Engines;                      // +0x04
    std::vector<C2DBoxI>   ChangedAreas;                 // +0x08
};
template CEngineManager& __fastcall CTBaseSingleton<CEngineManager>::Get();