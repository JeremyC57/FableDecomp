// Behaviour test for 0x0059AC41 std::_Deque_iterator<CComponent*>::_M_decrement (model copy; 4-byte elements, 128-byte blocks)
#include <cstdio>
#include <stddef.h>
inline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> struct DequeIter {
    typedef T** _Map_pointer;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(T)); }
    T* _M_cur; T* _M_first; T* _M_last; _Map_pointer _M_node;
    void _M_set_node(_Map_pointer new_node) { _M_last = (_M_first = *(_M_node = new_node)) + ptrdiff_t(_S_buffer_size()); }
    void _M_decrement() { if (_M_cur == _M_first) { _M_set_node(_M_node - 1); _M_cur = _M_last; } --_M_cur; }
};
int main()
{
    unsigned int blockA[32], blockB[32];
    unsigned int* map[2] = { blockA, blockB };
    DequeIter<unsigned int> it;
    it._M_node = &map[1]; it._M_first = blockB; it._M_last = blockB + 32; it._M_cur = blockB + 5;
    it._M_decrement();
    if (it._M_cur != blockB + 4 || it._M_node != &map[1] || it._M_first != blockB) { std::printf("0059ac41_TEST FAIL\n"); return 1; }
    it._M_cur = blockB;
    it._M_decrement();
    if (it._M_node != &map[0] || it._M_first != blockA || it._M_last != blockA + 32 || it._M_cur != blockA + 31) { std::printf("0059ac41_TEST FAIL\n"); return 1; }
    std::printf("0059ac41_TEST PASS\n");
    return 0;
}