#include <cstdio>
#include <string.h>
#include <math.h>

typedef unsigned char md5_byte_t;
typedef unsigned int  md5_word_t;

typedef struct md5_state_s {
    md5_word_t count[2];
    md5_word_t abcd[4];
    md5_byte_t buf[64];
} md5_state_t;

// --- test support: a plain MD5 block transform, md5_init and Deutsch's md5_append ---
static md5_word_t K[64];
static const int R[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};
static void md5_process(md5_state_t* pms, const md5_byte_t* data)
{
    md5_word_t X[16];
    for (int j = 0; j < 16; ++j)
        X[j] = (md5_word_t)data[j * 4] | ((md5_word_t)data[j * 4 + 1] << 8) | ((md5_word_t)data[j * 4 + 2] << 16)
             | ((md5_word_t)data[j * 4 + 3] << 24);
    md5_word_t a = pms->abcd[0], b = pms->abcd[1], c = pms->abcd[2], d = pms->abcd[3];
    for (int i = 0; i < 64; ++i) {
        md5_word_t f;
        int g;
        if (i < 16)      { f = (b & c) | (~b & d); g = i; }
        else if (i < 32) { f = (d & b) | (~d & c); g = (5 * i + 1) & 15; }
        else if (i < 48) { f = b ^ c ^ d;          g = (3 * i + 5) & 15; }
        else             { f = c ^ (b | ~d);       g = (7 * i) & 15; }
        md5_word_t t = d;
        d = c;
        c = b;
        md5_word_t x = (a + f + K[i] + X[g]) & 0xffffffffu;
        b = (b + ((x << R[i]) | (x >> (32 - R[i])))) & 0xffffffffu;
        a = t;
    }
    pms->abcd[0] += a; pms->abcd[1] += b; pms->abcd[2] += c; pms->abcd[3] += d;
}
static void md5_init(md5_state_t* pms)
{
    for (int i = 0; i < 64; ++i)
        K[i] = (md5_word_t)(fabs(sin((double)(i + 1))) * 4294967296.0);
    pms->count[0] = pms->count[1] = 0;
    pms->abcd[0] = 0x67452301;
    pms->abcd[1] = 0xefcdab89;
    pms->abcd[2] = 0x98badcfe;
    pms->abcd[3] = 0x10325476;
}
static void md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes)
{
    const md5_byte_t* p = data;
    int left = nbytes;
    int offset = (pms->count[0] >> 3) & 63;
    md5_word_t nbits = (md5_word_t)(nbytes << 3);
    if (nbytes <= 0)
        return;
    pms->count[1] += nbytes >> 29;
    pms->count[0] += nbits;
    if (pms->count[0] < nbits)
        pms->count[1]++;
    if (offset) {
        int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);
        memcpy(pms->buf + offset, p, copy);
        if (offset + copy < 64)
            return;
        p += copy;
        left -= copy;
        md5_process(pms, pms->buf);
    }
    for (; left >= 64; p += 64, left -= 64)
        md5_process(pms, p);
    if (left)
        memcpy(pms->buf, p, left);
}

// --- the function under test (source body) ---
void md5_finish(md5_state_t* pms, md5_byte_t digest[16])
{
    static const md5_byte_t pad[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    md5_byte_t data[8];
    int i;

    for (i = 0; i < 8; ++i)
        data[i] = (md5_byte_t)(pms->count[i >> 2] >> ((i & 3) << 3));
    md5_append(pms, pad, ((55 - (pms->count[0] >> 3)) & 63) + 1);
    md5_append(pms, data, 8);
    for (i = 0; i < 16; ++i)
        digest[i] = (md5_byte_t)(pms->abcd[i >> 2] >> ((i & 3) << 3));
}

static bool Digest(const char* text, const char* expectHex)
{
    md5_state_t st;
    md5_byte_t d[16];
    char hex[33];
    md5_init(&st);
    md5_append(&st, (const md5_byte_t*)text, (int)strlen(text));
    md5_finish(&st, d);
    for (int i = 0; i < 16; ++i)
        std::sprintf(hex + i * 2, "%02x", d[i]);
    return strcmp(hex, expectHex) == 0;
}

int main()
{
    if (!Digest("", "d41d8cd98f00b204e9800998ecf8427e") || !Digest("abc", "900150983cd24fb0d6963f7d28e17f72")
        || !Digest("The quick brown fox jumps over the lazy dog", "9e107d9d372bb6826bd81d3542a419d6")
        || !Digest("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
                   "57edf4a22be3c955ac49da2e2107b67a"))
        { std::printf("00a3e010_TEST FAIL\n"); return 1; }
    std::printf("00a3e010_TEST PASS\n");
    return 0;
}