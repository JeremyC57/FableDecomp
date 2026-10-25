// _STL::vector<long>::vector(unsigned int n) -- STLport sized constructor of a 4-byte POD vector: allocate n elements and zero them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38F30, 58 bytes, `__fastcall`, one stack argument (`ret 4`), returns this.
//   Only caller: CQ_FireHeartScript::RunVillagerComments @ 0x00D38D48 (`vector<long> v(NumberOfVillagers)`, later
//   filled with villager indices and read as `ShouldVillagerSpeak[v[rand() % count]] = true`).
// Label correction: the catalog "std_vector_AllocatePointers" is descriptive; 0x0050FA40 ("std_vector_allocate_ptrs")
//   is the shared _Vector_base<4-byte>(n, allocator) constructor.
// Callees: _Vector_base<long>::_Vector_base(unsigned int n, const allocator<long>&) @ 0x0050FA40.
// Shape: speed TU (no pragma). STLport 4.6 `explicit vector(size_type n) : _Vector_base(n, allocator_type())
//   { _M_finish = uninitialized_fill_n(_M_start, n, _Tp()); }`; the POD fill is `fill_n`'s
//   `for (; n > 0; --n, ++first) *first = value;`, which VC7.1 turns into `rep stosd` (unsigned n: `test ; jbe`) and
//   the end pointer `lea eax,[edx+ebx*4]`. The allocator temporary lives in the dead argument slot.
namespace _STL {
struct __true_type { };
template <class T> class allocator {
public:
    allocator() { }
};
template <class T> class _Vector_base {
public:
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8 (alloc proxy over the empty allocator)
    _Vector_base(unsigned int n, const allocator<T>& a);    // 0x0050FA40
};
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}
template <class T> class vector : public _Vector_base<T> {
public:
    explicit vector(unsigned int n) : _Vector_base<T>(n, allocator<T>())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};
template class vector<long>;
} // namespace _STL