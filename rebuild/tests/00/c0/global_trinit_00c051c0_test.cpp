// Behaviour test for zlib 1.2.1 trees.c _tr_init (0x00C051C0): descriptor binding, bit-buffer reset, last_eob_len, init_block.
#include <stdio.h>
#include <string.h>
typedef unsigned short ush; typedef unsigned long ulg; typedef unsigned int uInt;
struct ct_data { ush freq; ush dad; };
struct static_tree_desc { const ct_data* static_tree; const int* extra_bits; int extra_base; int elems; int max_length; };
struct tree_desc { ct_data* dyn_tree; int max_code; static_tree_desc* stat_desc; };
struct deflate_state {
    ct_data dyn_ltree[573]; ct_data dyn_dtree[61]; ct_data bl_tree[39];
    tree_desc l_desc; tree_desc d_desc; tree_desc bl_desc;
    uInt last_lit; ulg opt_len; ulg static_len; uInt matches; int last_eob_len; ush bi_buf; int bi_valid;
};
static static_tree_desc static_l_desc = { 0, 0, 257, 286, 15 };
static static_tree_desc static_d_desc = { 0, 0, 0, 30, 15 };
static static_tree_desc static_bl_desc = { 0, 0, 0, 19, 7 };
static void init_block(deflate_state* s)
{
    int n;
    for (n = 0; n < 286; n++) s->dyn_ltree[n].freq = 0;
    for (n = 0; n < 30; n++) s->dyn_dtree[n].freq = 0;
    for (n = 0; n < 19; n++) s->bl_tree[n].freq = 0;
    s->dyn_ltree[256].freq = 1;
    s->opt_len = s->static_len = 0L;
    s->last_lit = s->matches = 0;
}
static void _tr_init(deflate_state* s)
{
    s->l_desc.dyn_tree = s->dyn_ltree;
    s->l_desc.stat_desc = &static_l_desc;
    s->d_desc.dyn_tree = s->dyn_dtree;
    s->d_desc.stat_desc = &static_d_desc;
    s->bl_desc.dyn_tree = s->bl_tree;
    s->bl_desc.stat_desc = &static_bl_desc;
    s->bi_buf = 0;
    s->bi_valid = 0;
    s->last_eob_len = 8;
    init_block(s);
}
int main()
{
    static deflate_state s; int n; memset(&s, 0x33, sizeof(s));
    _tr_init(&s);
    if (s.l_desc.dyn_tree != s.dyn_ltree || s.l_desc.stat_desc != &static_l_desc || s.d_desc.dyn_tree != s.dyn_dtree
        || s.d_desc.stat_desc != &static_d_desc || s.bl_desc.dyn_tree != s.bl_tree || s.bl_desc.stat_desc != &static_bl_desc)
    { printf("00c051c0_TEST FAIL\n"); return 1; }
    if (s.l_desc.max_code != 0x33333333 || s.bi_buf != 0 || s.bi_valid != 0 || s.last_eob_len != 8)
    { printf("00c051c0_TEST FAIL\n"); return 1; }
    for (n = 0; n < 286; n++) if (s.dyn_ltree[n].freq != (n == 256 ? 1 : 0)) { printf("00c051c0_TEST FAIL\n"); return 1; }
    if (s.dyn_dtree[29].freq != 0 || s.bl_tree[18].freq != 0 || s.opt_len != 0 || s.static_len != 0 || s.last_lit != 0 || s.matches != 0)
    { printf("00c051c0_TEST FAIL\n"); return 1; }
    printf("00c051c0_TEST PASS\n"); return 0;
}