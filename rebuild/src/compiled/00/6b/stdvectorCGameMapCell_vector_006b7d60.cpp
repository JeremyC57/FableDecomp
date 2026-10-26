// std::vector<CGameMapCell>::vector(unsigned int n) -- STLport sized constructor of the game-map cell vector: allocate n cells and fill them with an (uninitialised) CGameMapCell()
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7D60, 81 bytes, `ret 4` (thiscall: n; returns this).
// Callers: one site, 0x006B75EB (CGameMap::Clear 0x006B7510 building a fresh cell vector to swap in).
// Callees: malloc @ 0x00BFEA0E (the STLport malloc allocator), uninitialized_fill_n<CGameMapCell*, unsigned, CGameMapCell>
//   @ 0x006B7CC0 (out-of-line `ret 4` front end).
// Shape: STLport 4.x `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }` with `_Vector_base(n, a)` inlined (three zero stores, `n ? malloc(n * 7)
//   : 0`, finish = start, end = start + n). The two empty temporaries share the two-dword frame: the allocator at byte 0,
//   the 7-byte `CGameMapCell()` at byte 1 (`lea edx,[esp+9]`), never written because the cell's user constructor is
//   empty (landed CGameMap ctor 0x006B7760).
// Label correction: the catalog `Vector_AllocateAndInit` is descriptive.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class CGameMapCell { public:                                   // 7 bytes (Ego_r.pdb)
    unsigned char AtmosThemes[3];                       // +0
    unsigned char AtmosBlends[2];                       // +3
    signed char   SoundTheme;                           // +5
    unsigned char Music;                                // +6
    CGameMapCell() { }
};

namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x);   // 0x006B7CC0

template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
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
    typedef A allocator_type;
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};

template class vector<CGameMapCell, allocator<CGameMapCell> >;
}