// CNavigationAreaUpdate::CSwitchableBlockage::CSwitchableBlockage() -- default constructor: UniqueID left unset, the STLport LineList built with its self-linked sentinel node
// Leaf: ??0CSwitchableBlockage@CNavigationAreaUpdate@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005155F0, 32 bytes, `ret` (thiscall, returns this). No direct caller in the image.
// Identity: a constructor (`mov eax,esi` return) that leaves +0..+7 untouched and builds an STLport list at +8 whose
//   node is 0x18 bytes (8-byte link + a 16-byte element). Ego_r.pdb CNavigationAreaUpdate::CSwitchableBlockage is
//   {unsigned __int64 UniqueID +0; list<C2DLineF> LineList +8} with C2DLineF 16 bytes (two C2DVectors) -- the list of
//   16-byte line records handled by the neighbouring STLport list bodies 0x005152C0 / 0x00515420. The catalog's
//   "StdList_ConstructMember" is descriptive. (A pair<unsigned __int64, list<..> > default constructor would zero
//   `first`, so the owner is the blockage record with an implicit/empty constructor.)
// Retail layout: the STLport list is the bare 4-byte sentinel pointer (`_STLP_alloc_proxy<_Node*>`), so the retail
//   record is 0x10 bytes (PDB 0x18 with the Dinkumware 12-byte list).
// Callee: malloc @ 0x00BFEA0E (`__cdecl`, result kept: `add esp,4`).
// Shape: STLport `_List_base(const allocator_type&) : _M_node(a, 0) { _Node* n = _M_node.allocate(1); n->_M_next = n;
//   n->_M_prev = n; _M_node._M_data = n; }` -- the null initialiser stored before the call, allocate(1) folded to a bare
//   malloc(0x18).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class C2DVector {
public:
    float X, Y;
};

class C2DLineF {                                    // Ego_r.pdb, 16 bytes
public:
    C2DVector Point1;                               // +0x00
    C2DVector Point2;                               // +0x08
};

namespace std {

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                       // +0
    _List_node_base* _M_prev;                       // +4
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                      // +8
};

template <class T, class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _List_base(const A&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;    // +0
};

template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
public:
    explicit list(const A& a = A()) : _List_base<T, A>(a) { }
};

}

class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {                     // Ego_r.pdb (retail 0x10 bytes)
    public:
        unsigned __int64     UniqueID;              // +0x00
        std::list<C2DLineF>  LineList;              // +0x08
        CSwitchableBlockage();
    };
};

CNavigationAreaUpdate::CSwitchableBlockage::CSwitchableBlockage()
{
}