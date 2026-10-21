// test for 0059a8f3 deque iterator difference
#include <stdio.h>
#include <stddef.h>
namespace NUISystem { class CComponent; }
namespace std {
inline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> struct _Deque_iterator {
    typedef T** _Map_pointer;
    typedef ptrdiff_t difference_type;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(T)); }
    T*          _M_cur;                                  // +0x00
    T*          _M_first;                                // +0x04
    T*          _M_last;                                 // +0x08
    _Map_pointer _M_node;                                // +0x0c
    difference_type operator-(const _Deque_iterator& x) const
    {
        return difference_type(_S_buffer_size()) * (_M_node - x._M_node - 1) +
            (_M_cur - _M_first) + (x._M_last - x._M_cur);
    }
};
template struct _Deque_iterator<NUISystem::CComponent*>;
}

int main()
{
    typedef std::_Deque_iterator<NUISystem::CComponent*> It;
    NUISystem::CComponent* b0[32]; NUISystem::CComponent* b1[32]; NUISystem::CComponent* b2[32];
    NUISystem::CComponent** map[3] = { b0, b1, b2 };
    It a; a._M_node = map + 0; a._M_first = b0; a._M_last = b0 + 32; a._M_cur = b0 + 5;
    It z; z._M_node = map + 2; z._M_first = b2; z._M_last = b2 + 32; z._M_cur = b2 + 7;
    It c = a; c._M_cur = b0 + 30;
    int ok = (z - a) == 64 + 2 && (a - a) == 0 && (c - a) == 25 && (a - c) == -25;
    printf(ok ? "0059a8f3_TEST PASS\n" : "0059a8f3_TEST FAIL\n");
    return 0;
}