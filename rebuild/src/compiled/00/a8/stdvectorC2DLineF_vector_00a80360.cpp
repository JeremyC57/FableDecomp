// std::vector<C2DLineF>::vector(unsigned int n) -- STLport sized constructor: malloc n lines, copy a default-constructed line into each slot
// Leaf: ??0?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAE@I@Z
// Retail: Fable.exe @ 0x00A80360, 122 bytes, `ret 4`; the one caller is UpdateLines 0x00A781A0. Catalog label
// Vector_ConstructInitialized_C2DLineF is descriptive.
// Body: STLport 4.6 explicit vector(size_type n): _Vector_base(n, allocator()) zeroes the three pointers, then
// _M_start = _M_finish = allocate(n) (n ? malloc(n * 16) : 0, malloc 0x00BFEA0E), _M_end_of_storage = _M_start + n; then
// _M_finish = uninitialized_fill_n(_M_start, n, C2DLineF()) -- the value is an uninitialised 16-byte frame temp (C2DLineF's
// default ctor is empty), copied with null-tested placement new.
// Layout: Ego_r.pdb C2DLineF (16 bytes: C2DVector Point1, Point2); STLport vector {start, finish, end_of_storage}.
// Shape: /O2 (16-aligned entry). Two things matter. C2DVector's user copy ctor keeps the four value loads inside the copy
// loop (without it they are hoisted into ebx/ebp/edi, 131v122). The fill loop has to be inlined, which VC7.1 does here only
// with __forceinline (plain inline or the STLport __uninitialized_fill_n dispatch stays out of line, 82v122 / 87v122).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
    C2DLineF() {}
};
namespace std {
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
};
template <class T> __forceinline T* uninitialized_fill_n(T* first, unsigned int n, const T& x)
{
    T* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};
template class vector<C2DLineF>;
}