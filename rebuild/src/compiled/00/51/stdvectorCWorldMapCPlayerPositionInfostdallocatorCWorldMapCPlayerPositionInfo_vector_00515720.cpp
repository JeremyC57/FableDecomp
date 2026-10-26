// std::vector<CWorldMap::CPlayerPositionInfo,std::allocator<CWorldMap::CPlayerPositionInfo> >::vector(unsigned int n) -- STLport vector(n) of the world map's 13-byte player position records: allocate n records and copy-construct each from a default record (Set = false)
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515720, 105 bytes, `ret 4` (thiscall: n). No direct caller (an inlined use
//   elsewhere; the body sits in the CWorldMap module next to its other vector instantiations).
// Element: Ego_r.pdb CWorldMap::CPlayerPositionInfo (nested class), 13 bytes packed {bool Set +0; C3DVector Pos +1} -- the element of
//   CWorldMap::PlayerPositionInfo (retail +0x78, landed CWorldMap::SetPlayerPos 0x005063E0). The default record only
//   clears the Set byte (`mov byte ptr [esp+13h],0`): a user default constructor `Set(false)` over a C3DVector whose
//   default constructor leaves the floats unset.
// Callees: malloc @ 0x00BFEA0E; __uninitialized_fill_n<CWorldMap::CPlayerPositionInfo*, unsigned int, CWorldMap::CPlayerPositionInfo>(first,
//   n, x, const __false_type&) @ 0x00513A00 (`__fastcall`, ret 8; manifest "GFExplicitNew<N3DPrimitive::CAnimatedBlock>"
//   is propagated).
// Shape: STLport `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }`: the three-argument front end inlines to the out-of-line
//   `__uninitialized_fill_n(.., __false_type())`; the empty tag temporary is homed in the dead `n` argument slot
//   (`lea edx,[esp+1Ch]`), the 13-byte default record at the top of the frame. Base null initialisers as immediates.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

class CWorldMap {                                   // only the nested element class is needed here
public:
#pragma pack(push, 1)
    class CPlayerPositionInfo {                     // Ego_r.pdb, 13 bytes
    public:
        bool      Set;                              // +0x00
        C3DVector Pos;                              // +0x01
        CPlayerPositionInfo() : Set(false) { }
    };
#pragma pack(pop)
};

namespace std {

struct __false_type { __false_type() { } };

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&);   // 0x00513A00

template <class ForwardIter, class Size, class T>
inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};

template class vector<CWorldMap::CPlayerPositionInfo, allocator<CWorldMap::CPlayerPositionInfo> >;

}