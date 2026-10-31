// Behaviour test for zlib 1.2.1 trees.c init_block (0x00C03EE0, static).
#include <stdio.h>
#include <string.h>
typedef unsigned short ush; typedef unsigned long ulg; typedef unsigned int uInt;
struct ct_data { ush freq; ush dad; };
struct deflate_state { ct_data dyn_ltree[573]; ct_data dyn_dtree[61]; ct_data bl_tree[39]; uInt last_lit; ulg opt_len; ulg static_len; uInt matches; };
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
int main()
{
    static deflate_state s; int n; memset(&s, 0x33, sizeof(s));
    init_block(&s);
    for (n = 0; n < 286; n++) if (s.dyn_ltree[n].freq != (n == 256 ? 1 : 0) || s.dyn_ltree[n].dad != 0x3333) { printf("00c03ee0_TEST FAIL\n"); return 1; }
    if (s.dyn_ltree[286].freq != 0x3333 || s.dyn_dtree[29].freq != 0 || s.dyn_dtree[30].freq != 0x3333 || s.bl_tree[18].freq != 0 || s.bl_tree[19].freq != 0x3333)
    { printf("00c03ee0_TEST FAIL\n"); return 1; }
    if (s.opt_len != 0 || s.static_len != 0 || s.last_lit != 0 || s.matches != 0) { printf("00c03ee0_TEST FAIL\n"); return 1; }
    printf("00c03ee0_TEST PASS\n"); return 0;
}