// std::vector<ENavigatorType>::resize(unsigned int new_size, ENavigatorType x) -- STLport resize: erase the tail, or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@W4ENavigatorType@@V?$allocator@W4ENavigatorType@@@std@@@std@@QAEXIW4ENavigatorType@@@Z
// Retail: Fable.exe @ 0x00412A50, 76 bytes, `ret 8`; callers 0x00412AF2 / 0x00412C65 / 0x00412D87 (the one-argument
// resize(n) wrappers, x = 0). The body is shared (COMDAT-folded) by every 4-byte enum vector; the element type is taken
// from the fill-insert callee's tag (Std_Vector_Insert_Range_4Bytes, ENavigatorType). Catalog label vector<unsigned int> is
// wrong: unsigned int would copy with memmove (STLport trivial-assignment traits); an enum takes the element-wise loop.
// Body: if (new_size < size()) erase(begin() + new_size, end()) inline (STLport __copy random-access loop of
// end() - end() elements, then _M_finish = the copy's end); else the out-of-line fill insert 0x00411F10 at end() with
// new_size - size() copies of x (x passed by value).
// Shape: /O2 (16-aligned entry).
enum ENavigatorType { NAVIGATOR_TYPE_NONE };
namespace std {
template <class T> class allocator {};
template <class InputIter, class OutputIter>
inline OutputIter __copy(InputIter first, InputIter last, OutputIter result)
{
    for (long n = last - first; n > 0; --n)
    {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    iterator erase(iterator first, iterator last)
    {
        iterator i = __copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(iterator pos, unsigned int n, T x);   // 0x00411F10
    void insert(iterator pos, unsigned int n, T x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, T x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<ENavigatorType>::resize(unsigned int, ENavigatorType);
}