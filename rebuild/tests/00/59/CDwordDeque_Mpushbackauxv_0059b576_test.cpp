#include <cstdio>
#include <cstdlib>
#include <new>
struct SDequeIterator {
    unsigned long* _M_cur; unsigned long* _M_first; unsigned long* _M_last; unsigned long** _M_node;
    void _M_set_node(unsigned long** n) { _M_node = n; _M_first = *n; _M_last = _M_first + 32; }
};
static int g_reserves = 0;
struct CDwordDeque {
    SDequeIterator _M_start, _M_finish; unsigned long** _M_map; unsigned long _M_map_size;
    void _M_reserve_map_at_back(unsigned long n) { g_reserves += (int)n; }
    void _M_push_back_aux_v(const unsigned long& value)
    {
        unsigned long copy = value;
        _M_reserve_map_at_back(1);
        *(_M_finish._M_node + 1) = (unsigned long*)std::malloc(32 * sizeof(unsigned long));
        new (_M_finish._M_cur) unsigned long(copy);
        _M_finish._M_set_node(_M_finish._M_node + 1);
        _M_finish._M_cur = _M_finish._M_first;
    }
};
int main()
{
    unsigned long buf[32]; unsigned long* map[2] = { buf, 0 };
    CDwordDeque d; d._M_map = map; d._M_map_size = 2;
    d._M_finish._M_node = &map[0]; d._M_finish._M_first = buf; d._M_finish._M_last = buf + 32; d._M_finish._M_cur = buf + 31;
    unsigned long v = 0xC0FFEE;
    d._M_push_back_aux_v(v);
    if (buf[31] != 0xC0FFEE || map[1] == 0 || d._M_finish._M_node != &map[1] || d._M_finish._M_first != map[1]
        || d._M_finish._M_cur != map[1] || d._M_finish._M_last != map[1] + 32 || g_reserves != 1) { std::printf("0059b576_TEST FAIL\n"); return 1; }
    std::free(map[1]);
    std::printf("0059b576_TEST PASS\n");
    return 0;
}