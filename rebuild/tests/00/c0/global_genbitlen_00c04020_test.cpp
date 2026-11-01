// Behaviour test for 00c04020: zlib 1.2.1 trees.c gen_bitlen -- code lengths from the dad chain, bl_count histogram,
// opt_len/static_len sums and the overflow fix-up when a length exceeds max_length
#include <cstdio>
enum { HEAP_SIZE = 16, MAX_BITS = 15 };
struct ct_data { unsigned short Freq; unsigned short Dad; unsigned short Len; };
struct static_tree_desc { const ct_data* static_tree; const int* extra_bits; int extra_base; int elems; int max_length; };
struct tree_desc { ct_data* dyn_tree; int max_code; static_tree_desc* stat_desc; };
struct deflate_state { int heap[HEAP_SIZE]; int heap_max; unsigned short bl_count[MAX_BITS + 1]; unsigned long opt_len, static_len; };
static void gen_bitlen(deflate_state* s, tree_desc* desc)
{
    ct_data* tree = desc->dyn_tree; int max_code = desc->max_code;
    const ct_data* stree = desc->stat_desc->static_tree; const int* extra = desc->stat_desc->extra_bits;
    int base = desc->stat_desc->extra_base, max_length = desc->stat_desc->max_length;
    int h, n, m, bits, xbits; unsigned short f; int overflow = 0;
    for (bits = 0; bits <= MAX_BITS; bits++) s->bl_count[bits] = 0;
    tree[s->heap[s->heap_max]].Len = 0;
    for (h = s->heap_max + 1; h < HEAP_SIZE; h++) {
        n = s->heap[h];
        bits = tree[tree[n].Dad].Len + 1;
        if (bits > max_length) bits = max_length, overflow++;
        tree[n].Len = (unsigned short)bits;
        if (n > max_code) continue;
        s->bl_count[bits]++;
        xbits = 0; if (n >= base) xbits = extra[n - base];
        f = tree[n].Freq;
        s->opt_len += (unsigned long)f * (bits + xbits);
        if (stree) s->static_len += (unsigned long)f * (stree[n].Len + xbits);
    }
    if (overflow == 0) return;
    do {
        bits = max_length - 1;
        while (s->bl_count[bits] == 0) bits--;
        s->bl_count[bits]--; s->bl_count[bits + 1] += 2; s->bl_count[max_length]--;
        overflow -= 2;
    } while (overflow > 0);
    for (bits = max_length; bits != 0; bits--) {
        n = s->bl_count[bits];
        while (n != 0) {
            m = s->heap[--h];
            if (m > max_code) continue;
            if (tree[m].Len != (unsigned)bits) { s->opt_len += ((long)bits - (long)tree[m].Len) * (long)tree[m].Freq; tree[m].Len = (unsigned short)bits; }
            n--;
        }
    }
}
int main()
{
    // leaves 0..3 (freq 1,1,2,4) under the skewed tree 6=(5,3) 5=(4,2) 4=(0,1): lengths 3,3,2,1
    ct_data tree[7] = { {1,4,0}, {1,4,0}, {2,5,0}, {4,6,0}, {2,5,0}, {4,6,0}, {8,0,0} };
    const int extra[4] = { 0, 0, 1, 0 };
    static_tree_desc sd = { 0, extra, 0, 4, 15 };
    tree_desc d = { tree, 3, &sd };
    deflate_state s; s.heap_max = HEAP_SIZE - 7; s.opt_len = s.static_len = 0;
    int order[7] = { 6, 5, 4, 3, 2, 1, 0 };
    for (int i = 0; i < 7; ++i) s.heap[s.heap_max + i] = order[i];
    gen_bitlen(&s, &d);
    bool ok = tree[0].Len == 3 && tree[1].Len == 3 && tree[2].Len == 2 && tree[3].Len == 1 && tree[6].Len == 0
           && s.bl_count[1] == 1 && s.bl_count[2] == 1 && s.bl_count[3] == 2 && s.opt_len == 3 + 3 + 2 * 3 + 4 * 1;
    // max_length 2 forces the two length-3 leaves to overflow: all four leaves end at length 2
    sd.max_length = 2; s.opt_len = 0;
    for (int i = 0; i < 7; ++i) s.heap[s.heap_max + i] = order[i];
    gen_bitlen(&s, &d);
    ok = ok && tree[0].Len == 2 && tree[1].Len == 2 && tree[2].Len == 2 && tree[3].Len == 2 && s.bl_count[2] == 4 && s.bl_count[1] == 0
            && s.opt_len == 2 + 2 + 2 * 3 + 4 * 2;
    std::printf(ok ? "00c04020_TEST PASS\n" : "00c04020_TEST FAIL\n");
    return ok ? 0 : 1;
}