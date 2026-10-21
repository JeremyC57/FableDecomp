// test for 0059aaf9 deque iterator _M_increment
#include <stdio.h>
#include <stddef.h>
namespace NUISystem { class CComponent; }
namespace std {
inline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> struct _Deque_iterator {
    typedef T** _Map_pointer;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(T)); }
    T*          _M_cur;                                  // +0x00
    T*          _M_first;                                // +0x04
    T*          _M_last;                                 // +0x08
    _Map_pointer _M_node;                                // +0x0c
    void _M_set_node(_Map_pointer new_node)
    {
        _M_last = (_M_first = *(_M_node = new_node)) + ptrdiff_t(_S_buffer_size());
    }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
};
template struct _Deque_iterator<NUISystem::CComponent*>;
}

int main()
{
    typedef NUISystem::CComponent* P;
    static P b0[32], b1[32]; static P* map[2] = { b0, b1 };
    std::_Deque_iterator<P> it; it._M_node = map; it._M_first = b0; it._M_last = b0 + 32; it._M_cur = b0 + 30;
    it._M_increment();
    int ok = it._M_cur == b0 + 31 && it._M_node == map;
    it._M_increment();
    ok &= it._M_cur == b1 && it._M_first == b1 && it._M_last == b1 + 32 && it._M_node == map + 1;
    printf(ok ? "0059aaf9_TEST PASS\n" : "0059aaf9_TEST FAIL\n");
    return ok ? 0 : 1;
}