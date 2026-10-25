// std::vector<unsigned int>::resize(unsigned int new_size, const unsigned int& x) -- STLport resize: erase the tail, or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@IV?$allocator@I@std@@@std@@QAEXIABI@Z
// Retail: Fable.exe @ 0x00442770, 87 bytes, `ret 8`; 11 callers (0x0043A43C ... 0x00BCD702) -- the body is shared
// (COMDAT-folded) by every vector of a 4-byte trivially assignable element; unsigned int per the catalog label.
// Body: if (new_size < size()) erase(begin() + new_size, end()) inline, its copy through STLport __copy_trivial
// (`last == first` -> result, else memmove 0x00BFEAE6 + byte count); else the out-of-line fill insert 0x00441020 at end()
// with new_size - size() copies of x (by const reference: the pointer is passed straight through).
// Fixed (rename1, 2026-10-24): x was modelled by value; STLport's resize(size_type, const T&) and _M_fill_insert(pos, n,
//   const T&) take it by reference -- the caller 0x0049BA82 pushes the address of its local (`lea eax,[esp+8] ; push eax`)
//   and this body forwards that word unchanged to 0x00441020. Bytes unchanged; the graded symbol is now ...QAEXIABI@Z.
// Shape: /O2 (16-aligned entry).
#include <string.h>
namespace std {
template <class T> class allocator {};
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
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
        iterator i = (iterator)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(iterator pos, unsigned int n, const T& x);   // 0x00441020
    void insert(iterator pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<unsigned int>::resize(unsigned int, const unsigned int&);
}