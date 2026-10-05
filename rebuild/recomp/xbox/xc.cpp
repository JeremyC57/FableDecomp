// Xc* crypto exports: SHA-1, HMAC-SHA1 and RC4 for real (save-game signatures use them);
// the DES/RSA calls (Xbox Live, XBE signatures) are stand-ins.
#include "xhost.hpp"

namespace xb {

namespace {
// SHA-1 state kept in the guest's context buffer (A_SHA_CTX is 116 bytes; this uses 96).
struct Sha1 {
    uint32_t h[5];
    uint64_t len;
    uint8_t buf[64];
    uint32_t n;
};
static_assert(sizeof(Sha1) <= 116, "SHA-1 state must fit the guest context");

uint32_t rol(uint32_t v, int s) { return (v << s) | (v >> (32 - s)); }

void shaBlock(Sha1& s, const uint8_t* p) {
    uint32_t w[80];
    for (int i = 0; i < 16; ++i) w[i] = (uint32_t)p[4 * i] << 24 | (uint32_t)p[4 * i + 1] << 16 | (uint32_t)p[4 * i + 2] << 8 | p[4 * i + 3];
    for (int i = 16; i < 80; ++i) w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    uint32_t a = s.h[0], b = s.h[1], c = s.h[2], d = s.h[3], e = s.h[4];
    for (int i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
        else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
        else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
        else { f = b ^ c ^ d; k = 0xCA62C1D6; }
        const uint32_t t = rol(a, 5) + f + e + k + w[i];
        e = d; d = c; c = rol(b, 30); b = a; a = t;
    }
    s.h[0] += a; s.h[1] += b; s.h[2] += c; s.h[3] += d; s.h[4] += e;
}
void shaInit(Sha1& s) {
    s = Sha1{{0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0}, 0, {}, 0};
}
void shaUpdate(Sha1& s, const uint8_t* p, size_t n) {
    s.len += n;
    while (n) {
        const size_t take = std::min<size_t>(64 - s.n, n);
        std::memcpy(s.buf + s.n, p, take);
        s.n += static_cast<uint32_t>(take);
        p += take;
        n -= take;
        if (s.n == 64) {
            shaBlock(s, s.buf);
            s.n = 0;
        }
    }
}
void shaFinal(Sha1& s, uint8_t out[20]) {
    const uint64_t bits = s.len * 8;
    const uint8_t pad = 0x80;
    shaUpdate(s, &pad, 1);
    const uint8_t zero = 0;
    while (s.n != 56) shaUpdate(s, &zero, 1);
    uint8_t lenb[8];
    for (int i = 0; i < 8; ++i) lenb[i] = static_cast<uint8_t>(bits >> (56 - 8 * i));
    shaUpdate(s, lenb, 8);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 4; ++j) out[4 * i + j] = static_cast<uint8_t>(s.h[i] >> (24 - 8 * j));
}
}  // namespace

KFUNC(XcSHAInit, 1) {
    Sha1 s;
    shaInit(s);
    std::memcpy(gp(ARG(c, 0)), &s, sizeof s);
    return 0;
}
KFUNC(XcSHAUpdate, 3) {
    Sha1 s;
    std::memcpy(&s, gp(ARG(c, 0)), sizeof s);
    shaUpdate(s, gp(ARG(c, 1)), ARG(c, 2));
    std::memcpy(gp(ARG(c, 0)), &s, sizeof s);
    return 0;
}
KFUNC(XcSHAFinal, 2) {
    Sha1 s;
    std::memcpy(&s, gp(ARG(c, 0)), sizeof s);
    shaFinal(s, gp(ARG(c, 1)));
    return 0;
}
// XcHMAC(Key, KeyLen, Data, DataLen, Data2, Data2Len, Digest)
KFUNC(XcHMAC, 7) {
    uint8_t key[64] = {};
    const uint32_t klen = ARG(c, 1);
    if (klen > 64) {
        Sha1 s;
        shaInit(s);
        shaUpdate(s, gp(ARG(c, 0)), klen);
        shaFinal(s, key);
    } else {
        std::memcpy(key, gp(ARG(c, 0)), klen);
    }
    uint8_t ipad[64], opad[64], inner[20];
    for (int i = 0; i < 64; ++i) { ipad[i] = key[i] ^ 0x36; opad[i] = key[i] ^ 0x5C; }
    Sha1 s;
    shaInit(s);
    shaUpdate(s, ipad, 64);
    if (ARG(c, 2)) shaUpdate(s, gp(ARG(c, 2)), ARG(c, 3));
    if (ARG(c, 4)) shaUpdate(s, gp(ARG(c, 4)), ARG(c, 5));
    shaFinal(s, inner);
    shaInit(s);
    shaUpdate(s, opad, 64);
    shaUpdate(s, inner, 20);
    shaFinal(s, gp(ARG(c, 6)));
    return 0;
}
// RC4 key structure: S[256], i, j
KFUNC(XcRC4Key, 3) {
    uint8_t* st = gp(ARG(c, 0));
    const uint32_t klen = ARG(c, 1);
    const uint8_t* k = gp(ARG(c, 2));
    for (int i = 0; i < 256; ++i) st[i] = static_cast<uint8_t>(i);
    uint8_t j = 0;
    for (int i = 0; i < 256; ++i) {
        j = static_cast<uint8_t>(j + st[i] + k[i % klen]);
        std::swap(st[i], st[j]);
    }
    st[256] = st[257] = 0;
    return 0;
}
KFUNC(XcRC4Crypt, 3) {
    uint8_t* st = gp(ARG(c, 0));
    uint8_t* p = gp(ARG(c, 2));
    uint8_t i = st[256], j = st[257];
    for (uint32_t n = 0; n < ARG(c, 1); ++n) {
        i = static_cast<uint8_t>(i + 1);
        j = static_cast<uint8_t>(j + st[i]);
        std::swap(st[i], st[j]);
        p[n] ^= st[static_cast<uint8_t>(st[i] + st[j])];
    }
    st[256] = i;
    st[257] = j;
    return 0;
}
KFUNC(XcBlockCryptCBC, 7) {
    if (ARG(c, 2) != ARG(c, 3)) std::memmove(gp(ARG(c, 2)), gp(ARG(c, 3)), ARG(c, 1));
    return 0;
}
KFUNC(XcKeyTable, 3) { return 0; }
KFUNC(XcDESKeyParity, 2) { return 0; }
KFUNC(XcModExp, 5) { return 0; }
KFUNC(XcVerifyPKCS1Signature, 3) { return 1; }

} // namespace xb
