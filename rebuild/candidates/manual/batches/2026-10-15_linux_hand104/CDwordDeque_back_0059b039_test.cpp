#include <cstdio>
struct SDequeIterator {
    unsigned long* _M_cur; unsigned long* _M_first; unsigned long* _M_last; unsigned long** _M_node;
    void _M_decrement()
    {
        if (_M_cur == _M_first) { --_M_node; _M_first = *_M_node; _M_last = _M_first + 32; _M_cur = _M_last; }
        --_M_cur;
    }
    SDequeIterator& operator--() { _M_decrement(); return *this; }
    unsigned long& operator*() const { return *_M_cur; }
};
struct CDwordDeque {
    SDequeIterator _M_start, _M_finish; unsigned long** _M_map; unsigned long _M_map_size;
    unsigned long& back() { SDequeIterator tmp = _M_finish; --tmp; return *tmp; }
};
int main()
{
    unsigned long bufA[32], bufB[32]; unsigned long* map[2] = { bufA, bufB };
    for (int i = 0; i < 32; ++i) { bufA[i] = 100 + i; bufB[i] = 200 + i; }
    CDwordDeque d;
    d._M_finish._M_node = &map[1]; d._M_finish._M_first = bufB; d._M_finish._M_last = bufB + 32; d._M_finish._M_cur = bufB + 5;
    if (&d.back() != &bufB[4] || d.back() != 204) { std::printf("0059b039_TEST FAIL\n"); return 1; }
    d._M_finish._M_cur = bufB;                         // at a node start: back() is the last slot of the previous node
    if (&d.back() != &bufA[31] || d._M_finish._M_cur != bufB || d._M_finish._M_node != &map[1]) { std::printf("0059b039_TEST FAIL\n"); return 1; }
    std::printf("0059b039_TEST PASS\n");
    return 0;
}
