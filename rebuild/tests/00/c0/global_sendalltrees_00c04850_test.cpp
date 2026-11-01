// Behaviour test for zlib 1.2.1 trees.c send_all_trees (0x00C04850): the three code counts (lcodes-257 in 5 bits,
// dcodes-1 in 5 bits, blcodes-4 in 4 bits), then blcodes bit-length code lengths in bl_order (3 bits each), then
// send_tree for the literal tree (max code lcodes-1) and the distance tree (dcodes-1).
#include <cstdio>
#include <cstring>
struct ct_data { unsigned short Freq; unsigned short Len; };
static const unsigned char bl_order[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
struct deflate_state { ct_data dyn_ltree[8]; ct_data dyn_dtree[8]; ct_data bl_tree[19]; };
static char g_log[512];
static void Append(const char* s) { std::strcat(g_log, s); }
static void send_bits(deflate_state*, int value, int length)
{ char buf[32]; std::sprintf(buf, "b%d/%d;", value, length); Append(buf); }
static void send_tree(deflate_state* s, ct_data* tree, int max_code)
{ char buf[32]; std::sprintf(buf, "t%c%d;", tree == s->dyn_ltree ? 'L' : 'D', max_code); Append(buf); }
static void send_all_trees(deflate_state* s, int lcodes, int dcodes, int blcodes)
{
    int rank;
    send_bits(s, lcodes - 257, 5);
    send_bits(s, dcodes - 1, 5);
    send_bits(s, blcodes - 4, 4);
    for (rank = 0; rank < blcodes; rank++)
        send_bits(s, s->bl_tree[bl_order[rank]].Len, 3);
    send_tree(s, (ct_data*)s->dyn_ltree, lcodes - 1);
    send_tree(s, (ct_data*)s->dyn_dtree, dcodes - 1);
}
int main()
{
    deflate_state s; std::memset(&s, 0, sizeof(s));
    s.bl_tree[16].Len = 2; s.bl_tree[17].Len = 3; s.bl_tree[18].Len = 0; s.bl_tree[0].Len = 5; s.bl_tree[8].Len = 1;
    g_log[0] = 0;
    send_all_trees(&s, 260, 3, 5);
    const char* want = "b3/5;b2/5;b1/4;b2/3;b3/3;b0/3;b5/3;b1/3;tL259;tD2;";
    if (std::strcmp(g_log, want) != 0) { std::printf("00c04850_TEST FAIL %s\n", g_log); return 1; }
    g_log[0] = 0;
    send_all_trees(&s, 257, 1, 4);
    if (std::strcmp(g_log, "b0/5;b0/5;b0/4;b2/3;b3/3;b0/3;b5/3;tL256;tD0;") != 0) { std::printf("00c04850_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00c04850_TEST PASS\n");
    return 0;
}