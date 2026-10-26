// Behaviour test for std::__destroy_aux over deque<CSubtitleRenderer::CSetTextInfo> iterators (0x00440B40): every element from
// first up to (not including) last is destroyed -- its four strings in reverse member order -- and the walk hops to the next
// 4-element block when the cursor reaches a block end.
#include <cstdio>
static int g_log[64];
static int g_count = 0;
class CWideString { public:
    int tag;
    ~CWideString() { if (g_count < 64) g_log[g_count] = tag; ++g_count; }
};
struct CSetTextInfo {
    CWideString Text, Button1Text, Button2Text, Button3Text;
    unsigned long DisplayFlags;
    float DisplayTime;
    bool UpdateWhilePaused, ChangePlayerMode;
};
struct Iter {
    CSetTextInfo* _M_cur; CSetTextInfo* _M_first; CSetTextInfo* _M_last; CSetTextInfo** _M_node;
    enum { BUF = 4 };                              // retail: 128 / sizeof(CSetTextInfo) = 128 / 28
    void _M_set_node(CSetTextInfo** n) { _M_last = (_M_first = *(_M_node = n)) + (int)BUF; }
    Iter& operator++() { if (++_M_cur == _M_last) { _M_set_node(_M_node + 1); _M_cur = _M_first; } return *this; }
    bool operator!=(const Iter& o) const { return _M_cur != o._M_cur; }
};
static void destroy_aux_model(Iter first, Iter last)
{
    for (; first != last; ++first)
        first._M_cur->~CSetTextInfo();
}
static unsigned char g_blocks[3][4 * sizeof(CSetTextInfo)];
int main()
{
    int bad = 0;
    CSetTextInfo* blk[3];
    for (int b = 0; b < 3; ++b) {
        blk[b] = reinterpret_cast<CSetTextInfo*>(g_blocks[b]);
        for (int e = 0; e < 4; ++e) {
            CSetTextInfo& t = blk[b][e];
            int id = (b * 4 + e) * 10;
            t.Text.tag = id + 0; t.Button1Text.tag = id + 1; t.Button2Text.tag = id + 2; t.Button3Text.tag = id + 3;
        }
    }
    // first = block 0 element 2, last = block 2 element 1: elements 2,3 | 4..7 | 8 -> 7 elements, 28 string dtors
    Iter first; first._M_set_node(blk); first._M_cur = blk[0] + 2;
    Iter last;  last._M_set_node(blk + 2); last._M_cur = blk[2] + 1;
    destroy_aux_model(first, last);
    if (g_count != 28) bad = 1;
    static const int elems[7] = { 2, 3, 4, 5, 6, 7, 8 };
    for (int i = 0; i < 7 && !bad; ++i)
        for (int m = 0; m < 4; ++m)
            if (g_log[i * 4 + m] != elems[i] * 10 + (3 - m)) bad = 1;
    g_count = 0;
    destroy_aux_model(last, last);                  // empty range
    if (g_count != 0) bad = 1;
    if (bad) { std::printf("00440b40_TEST FAIL\n"); return 1; }
    std::printf("00440b40_TEST PASS\n");
    return 0;
}