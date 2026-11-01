// Test for _tr_align (0x00C055D0, zlib 1.2.1 trees.c): an empty static block (block type STATIC_TREES<<1 in 3 bits, then
// END_BLOCK = code 0 / length 7 from static_ltree) is appended LSB-first to the bit buffer, the buffer is flushed to whole
// bytes, and when 1 + last_eob_len + 10 - bi_valid < 9 a second empty block follows; last_eob_len ends as 7.
#include <cstdio>
struct State { unsigned char buf[16]; int pending; unsigned short bi_buf; int bi_valid; int last_eob_len; };
#define put_byte(s, c) { s->buf[s->pending++] = (unsigned char)(c); }
static void send_bits(State* s, int value, int length)
{
    if (s->bi_valid > 16 - length) {
        s->bi_buf |= (unsigned short)(value << s->bi_valid);
        put_byte(s, s->bi_buf & 0xff); put_byte(s, (unsigned short)s->bi_buf >> 8);
        s->bi_buf = (unsigned short)((unsigned short)value >> (16 - s->bi_valid));
        s->bi_valid += length - 16;
    } else {
        s->bi_buf |= (unsigned short)(value << s->bi_valid);
        s->bi_valid += length;
    }
}
static void bi_flush(State* s)
{
    if (s->bi_valid == 16) { put_byte(s, s->bi_buf & 0xff); put_byte(s, s->bi_buf >> 8); s->bi_buf = 0; s->bi_valid = 0; }
    else if (s->bi_valid >= 8) { put_byte(s, (unsigned char)s->bi_buf); s->bi_buf >>= 8; s->bi_valid -= 8; }
}
static void tr_align(State* s)
{
    send_bits(s, 1 << 1, 3); send_bits(s, 0, 7); bi_flush(s);
    if (1 + s->last_eob_len + 10 - s->bi_valid < 9) { send_bits(s, 1 << 1, 3); send_bits(s, 0, 7); bi_flush(s); }
    s->last_eob_len = 7;
}
static State Make(int bv, unsigned short bb, int eob) { State s; s.pending = 0; s.bi_buf = bb; s.bi_valid = bv; s.last_eob_len = eob; return s; }
int main()
{
    State s = Make(0, 0, 8); tr_align(&s);        // 10 bits: one byte out, 2 left, no second block
    if (s.pending != 1 || s.buf[0] != 0x02 || s.bi_valid != 2 || s.bi_buf != 0 || s.last_eob_len != 7) { std::printf("00c055d0_TEST FAIL\n"); return 1; }
    s = Make(6, 0x15, 8); tr_align(&s);           // exactly 16 bits: one short out, empty buffer
    if (s.pending != 2 || s.buf[0] != 0x95 || s.buf[1] != 0x00 || s.bi_valid != 0 || s.last_eob_len != 7) { std::printf("00c055d0_TEST FAIL\n"); return 1; }
    s = Make(12, 0x0abc, 3); tr_align(&s);        // 6 bits left after the first block: 1+3+10-6 < 9 sends a second one
    if (s.pending != 4 || s.buf[0] != 0xbc || s.buf[1] != 0x2a || s.buf[2] != 0x80 || s.buf[3] != 0x00 || s.bi_valid != 0 || s.last_eob_len != 7) { std::printf("00c055d0_TEST FAIL\n"); return 1; }
    s = Make(12, 0x0abc, 4); tr_align(&s);        // 1+4+10-6 == 9: no second block
    if (s.pending != 2 || s.bi_valid != 6 || s.last_eob_len != 7) { std::printf("00c055d0_TEST FAIL\n"); return 1; }
    std::printf("00c055d0_TEST PASS\n");
    return 0;
}