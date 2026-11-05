// Behaviour test for inflate_table 0x00C064D0 (zlib 1.2.1 inftrees.c): builds the fixed literal/length
// and distance tables, a CODES table and checks decoding, the incomplete / over-subscribed rejections and
// the sub-table linkage of the 9-bit root over the fixed length code.
#include <cstdio>
#include <cstring>
#define FAR
#define MAXBITS 15
#define ENOUGH 1440
#define MAXD 154
typedef struct { unsigned char op; unsigned char bits; unsigned short val; } code;
typedef enum { CODES, LENS, DISTS } codetype;

static int inflate_table(codetype type, unsigned short FAR *lens, unsigned codes,
                         code FAR * FAR *table, unsigned FAR *bits, unsigned short FAR *work)
{
    unsigned len, sym, min, max, root, curr, drop;
    int left;
    unsigned used, huff, incr, fill, low, mask;
    code here;
    code FAR *next;
    const unsigned short FAR *base;
    const unsigned short FAR *extra;
    int end;
    unsigned short count[MAXBITS+1];
    unsigned short offs[MAXBITS+1];
    static const unsigned short lbase[31] = {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
    static const unsigned short lext[31] = {
        16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18,
        19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 16, 201, 196};
    static const unsigned short dbase[32] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577, 0, 0};
    static const unsigned short dext[32] = {
        16, 16, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22,
        23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
        28, 28, 29, 29, 64, 64};

    for (len = 0; len <= MAXBITS; len++) count[len] = 0;
    for (sym = 0; sym < codes; sym++) count[lens[sym]]++;
    root = *bits;
    for (max = MAXBITS; max >= 1; max--) if (count[max] != 0) break;
    if (root > max) root = max;
    if (max == 0) return -1;
    for (min = 1; min <= MAXBITS; min++) if (count[min] != 0) break;
    if (root < min) root = min;
    left = 1;
    for (len = 1; len <= MAXBITS; len++) {
        left <<= 1;
        left -= count[len];
        if (left < 0) return -1;
    }
    if (left > 0 && (type == CODES || (codes - count[0] != 1))) return -1;
    offs[1] = 0;
    for (len = 1; len < MAXBITS; len++) offs[len + 1] = offs[len] + count[len];
    for (sym = 0; sym < codes; sym++)
        if (lens[sym] != 0) work[offs[lens[sym]]++] = (unsigned short)sym;
    switch (type) {
    case CODES: base = extra = work; end = 19; break;
    case LENS: base = lbase; base -= 257; extra = lext; extra -= 257; end = 256; break;
    default: base = dbase; extra = dext; end = -1;
    }
    huff = 0; sym = 0; len = min; next = *table; curr = root; drop = 0;
    low = (unsigned)(-1); used = 1U << root; mask = used - 1;
    if (type == LENS && used >= ENOUGH - MAXD) return 1;
    for (;;) {
        here.bits = (unsigned char)(len - drop);
        if ((int)(work[sym]) < end) { here.op = (unsigned char)0; here.val = work[sym]; }
        else if ((int)(work[sym]) > end) { here.op = (unsigned char)(extra[work[sym]]); here.val = base[work[sym]]; }
        else { here.op = (unsigned char)(32 + 64); here.val = 0; }
        incr = 1U << (len - drop);
        fill = 1U << curr;
        do { fill -= incr; next[(huff >> drop) + fill] = here; } while (fill != 0);
        incr = 1U << (len - 1);
        while (huff & incr) incr >>= 1;
        if (incr != 0) { huff &= incr - 1; huff += incr; } else huff = 0;
        sym++;
        if (--(count[len]) == 0) { if (len == max) break; len = lens[work[sym]]; }
        if (len > root && (huff & mask) != low) {
            if (drop == 0) drop = root;
            next += 1U << curr;
            curr = len - drop;
            left = (int)(1 << curr);
            while (curr + drop < max) {
                left -= count[curr + drop];
                if (left <= 0) break;
                curr++;
                left <<= 1;
            }
            used += 1U << curr;
            if (type == LENS && used >= ENOUGH - MAXD) return 1;
            low = huff & mask;
            (*table)[low].op = (unsigned char)curr;
            (*table)[low].bits = (unsigned char)root;
            (*table)[low].val = (unsigned short)(next - *table);
        }
    }
    here.op = (unsigned char)64; here.bits = (unsigned char)(len - drop); here.val = (unsigned short)0;
    while (huff != 0) {
        if (drop != 0 && (huff & mask) != low) {
            drop = 0; len = root; next = *table; curr = root; here.bits = (unsigned char)len;
        }
        next[huff >> drop] = here;
        incr = 1U << (len - 1);
        while (huff & incr) incr >>= 1;
        if (incr != 0) { huff &= incr - 1; huff += incr; } else huff = 0;
    }
    *table += used;
    *bits = root;
    return 0;
}

/* reverse the low n bits of v (deflate codes are stored LSB first) */
static unsigned rev(unsigned v, unsigned n)
{
    unsigned r = 0;
    for (unsigned i = 0; i < n; i++) { r = (r << 1) | (v & 1); v >>= 1; }
    return r;
}

static bool fail(const char* what) { std::printf("00c064d0_TEST FAIL (%s)\n", what); return false; }

int main()
{
    static unsigned short lens[320];
    static unsigned short work[288];
    static code codes[ENOUGH];
    code* next; unsigned bits; unsigned sym;

    /* fixed literal/length code (RFC 1951 3.2.6) */
    sym = 0;
    while (sym < 144) lens[sym++] = 8;
    while (sym < 256) lens[sym++] = 9;
    while (sym < 280) lens[sym++] = 7;
    while (sym < 288) lens[sym++] = 8;
    next = codes; bits = 9;
    std::memset(codes, 0xAA, sizeof(codes));
    if (inflate_table(LENS, lens, 288, &next, &bits, work) != 0) return fail("fixed lens"), 1;
    if (bits != 9 || next - codes != 512) return fail("fixed lens size"), 1;
    /* literal 0 is 8-bit code 00110000 (0x30) */
    { code c = codes[rev(0x30, 8)]; if (c.op != 0 || c.bits != 8 || c.val != 0) return fail("lit 0"), 1; }
    /* literal 'A' (65) = 0x30 + 65 = 0x71 */
    { code c = codes[rev(0x71, 8)]; if (c.op != 0 || c.bits != 8 || c.val != 65) return fail("lit A"), 1; }
    /* end of block 256 is 7-bit code 0000000 */
    { code c = codes[0]; if (c.op != 96 || c.bits != 7 || c.val != 0) return fail("eob"), 1; }
    /* length symbol 257 (code 0000001, 7 bits): base 3, 0 extra */
    { code c = codes[rev(1, 7)]; if (c.op != 16 || c.bits != 7 || c.val != 3) return fail("len 257"), 1; }
    /* length symbol 265: 9-bit? no: 265 is 7-bit code 0001001; base 11, 1 extra */
    { code c = codes[rev(9, 7)]; if (c.op != 17 || c.bits != 7 || c.val != 11) return fail("len 265"), 1; }
    /* literal 255 = 9-bit code 111111111 */
    { code c = codes[rev(0x1FF, 9)]; if (c.op != 0 || c.bits != 9 || c.val != 255) return fail("lit 255"), 1; }
    /* 285 = 8-bit 11000101: base 258 */
    { code c = codes[rev(0xC5, 8)]; if (c.op != 16 || c.bits != 8 || c.val != 258) return fail("len 285"), 1; }

    /* fixed distance code: 30 codes of 5 bits */
    for (sym = 0; sym < 32; sym++) lens[sym] = 5;
    next = codes; bits = 5;
    if (inflate_table(DISTS, lens, 32, &next, &bits, work) != 0) return fail("fixed dists"), 1;
    if (bits != 5 || next - codes != 32) return fail("fixed dists size"), 1;
    { code c = codes[rev(4, 5)]; if (c.op != 17 || c.bits != 5 || c.val != 5) return fail("dist 4"), 1; }
    { code c = codes[rev(29, 5)]; if (c.op != 29 || c.bits != 5 || c.val != 24577) return fail("dist 29"), 1; }
    { code c = codes[rev(30, 5)]; if (c.op != 64 || c.bits != 5) return fail("dist 30 invalid"), 1; }

    /* a code-length code with a sub-table: lengths 1,2,3,3 over 4 symbols, root forced to 2 */
    lens[0] = 1; lens[1] = 2; lens[2] = 3; lens[3] = 3;
    next = codes; bits = 2;
    if (inflate_table(CODES, lens, 4, &next, &bits, work) != 0) return fail("codes"), 1;
    if (bits != 2 || next - codes != 4 + 2) return fail("codes size"), 1;
    /* symbol 0 = '0'; index 00 and 10 */
    if (codes[0].op != 0 || codes[0].val != 0 || codes[0].bits != 1) return fail("codes sym0"), 1;
    if (codes[2].op != 0 || codes[2].val != 0) return fail("codes sym0 dup"), 1;
    /* symbol 1 = '10' (reversed: index 01) */
    if (codes[1].op != 0 || codes[1].val != 1 || codes[1].bits != 2) return fail("codes sym1"), 1;
    /* index 11 links to the 1-bit sub-table at 4 */
    if (codes[3].op != 1 || codes[3].bits != 2 || codes[3].val != 4) return fail("codes link"), 1;
    if (codes[4].val != 2 || codes[4].bits != 1 || codes[5].val != 3 || codes[5].bits != 1) return fail("codes sub"), 1;

    /* incomplete set of lengths -> -1 (except a single code) */
    lens[0] = 1; lens[1] = 2; lens[2] = 0; lens[3] = 0;
    next = codes; bits = 7;
    if (inflate_table(CODES, lens, 4, &next, &bits, work) != -1) return fail("incomplete"), 1;
    /* over-subscribed -> -1 */
    lens[0] = 1; lens[1] = 1; lens[2] = 1; lens[3] = 0;
    next = codes; bits = 7;
    if (inflate_table(DISTS, lens, 4, &next, &bits, work) != -1) return fail("oversubscribed"), 1;
    /* no codes -> -1 */
    lens[0] = 0; lens[1] = 0;
    next = codes; bits = 7;
    if (inflate_table(DISTS, lens, 2, &next, &bits, work) != -1) return fail("no codes"), 1;
    /* a single distance code of length 1 is accepted (incomplete, but allowed for non-CODES) */
    lens[0] = 1; lens[1] = 0;
    next = codes; bits = 7;
    if (inflate_table(DISTS, lens, 2, &next, &bits, work) != 0 || bits != 1) return fail("single code"), 1;
    if (codes[1].op != 64) return fail("single code invalid marker"), 1;

    std::printf("00c064d0_TEST PASS\n");
    return 0;
}