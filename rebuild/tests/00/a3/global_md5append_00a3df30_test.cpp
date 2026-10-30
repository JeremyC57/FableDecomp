// Behaviour test for md5.c md5_append (0x00A3DF30): bit count update and block buffering (the block transform is counted).
#include <cstdio>
#include <cstring>
typedef unsigned char md5_byte_t; typedef unsigned int md5_word_t;
struct md5_state_t { md5_word_t count[2]; md5_word_t abcd[4]; md5_byte_t buf[64]; };
static int g_blocks;
static void md5_process(md5_state_t*, const md5_byte_t*) { g_blocks++; }
static void md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes)
{
    const md5_byte_t* p = data; int left = nbytes; int offset = (pms->count[0] >> 3) & 63; md5_word_t nbits = (md5_word_t)(nbytes << 3);
    if (nbytes <= 0) return;
    pms->count[1] += nbytes >> 29; pms->count[0] += nbits; if (pms->count[0] < nbits) pms->count[1]++;
    if (offset) {
        int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);
        std::memcpy(pms->buf + offset, p, copy);
        if (offset + copy < 64) return;
        p += copy; left -= copy; md5_process(pms, pms->buf);
    }
    for (; left >= 64; p += 64, left -= 64) md5_process(pms, p);
    if (left) std::memcpy(pms->buf, p, left);
}
int main()
{
    static md5_byte_t data[200]; for (int i = 0; i < 200; ++i) data[i] = (md5_byte_t)i;
    md5_state_t s; std::memset(&s, 0, sizeof(s));
    md5_append(&s, data, 10);
    if (g_blocks != 0 || s.count[0] != 80 || s.buf[9] != 9) { std::printf("00a3df30_TEST FAIL\n"); return 1; }
    md5_append(&s, data, 190);    // fills the first block (54), then 2 full blocks, 8 left
    if (g_blocks != 3 || s.count[0] != 1600 || s.buf[0] != 182 || s.buf[7] != 189) { std::printf("00a3df30_TEST FAIL\n"); return 1; }
    s.count[0] = 0xfffffff8u; md5_append(&s, data, 1);
    if (s.count[1] != 1) { std::printf("00a3df30_TEST FAIL\n"); return 1; }
    std::printf("00a3df30_TEST PASS\n"); return 0;
}