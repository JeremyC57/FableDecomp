// behaviour test for 0x00913990: pop_front steps the cursor or frees the exhausted node
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct It { long* cur; long* first; long* last; long** node; };
static int g_frees = 0;
static void PopFront(It& s) { if (s.cur != s.last - 1) ++s.cur; else { std::free(s.first); ++g_frees; ++s.node; s.first = *s.node; s.last = s.first + 4; s.cur = s.first; } }
int main()
{
    long* nodes[2]; nodes[0] = (long*)std::malloc(16); nodes[1] = (long*)std::malloc(16);
    It s; s.node = nodes; s.first = nodes[0]; s.last = s.first + 4; s.cur = s.first + 2;
    PopFront(s); int ok = s.cur == nodes[0] + 3 && g_frees == 0;
    PopFront(s); ok = ok && s.cur == nodes[1] && s.node == nodes + 1 && g_frees == 1;
    std::free(nodes[1]);
    if (!ok) { std::printf("00913990_TEST FAIL\n"); return 1; }
    std::printf("00913990_TEST PASS\n");
    return 0;
}