// z_crc32(uLong crc, const Bytef* buf, uInt len) -- zlib 1.2.1 crc32.c crc32: return 0 for a NULL buffer, else tail-call the little-endian BYFOUR worker crc32_little
// Leaf: _z_crc32
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C05FD0, 24 bytes, __cdecl; tail-jumps to crc32.c's static crc32_little 0x00C059D0 with
//   crc in eax, buf in ecx and len in edx (the endian test folded away). zlib 1.2.1 Z_PREFIX build (crc32 -> z_crc32), BYFOUR.
// Shape (retry1): parked as "LTCG custom register convention"; the crc32.c statics crc32_little / crc32_big written with their
//   real bodies inside `extern "C" { static __declspec(noinline) }` get VC7.1's private convention (crc eax, buf ecx, len edx)
//   and the tail jump matches. crc_table is crc32.h's table (retail .rdata, declared here).
typedef unsigned char  Bytef;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef unsigned int   u4;
typedef int            ptrdiff_t;
#define local static
#define FAR
#define Z_NULL 0
#define TBLS 8
extern "C" {
extern const unsigned long FAR crc_table[TBLS][256];                       // crc32.h (retail .rdata)

#define DO1 crc = crc_table[0][((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8)
#define DO8 DO1; DO1; DO1; DO1; DO1; DO1; DO1; DO1

local __declspec(noinline) unsigned long crc32_little(unsigned long crc, const unsigned char FAR* buf, unsigned len);
local __declspec(noinline) unsigned long crc32_big(unsigned long crc, const unsigned char FAR* buf, unsigned len);

unsigned long __cdecl z_crc32(unsigned long crc, const unsigned char FAR* buf, unsigned len)
{
    if (buf == Z_NULL) return 0UL;

    if (sizeof(void *) == sizeof(ptrdiff_t)) {
        u4 endian;

        endian = 1;
        if (*((unsigned char *)(&endian)))
            return crc32_little(crc, buf, len);
        else
            return crc32_big(crc, buf, len);
    }
    crc = crc ^ 0xffffffffUL;
    while (len >= 8) {
        DO8;
        len -= 8;
    }
    if (len) do {
        DO1;
    } while (--len);
    return crc ^ 0xffffffffUL;
}

#define DOLIT4 c ^= *buf4++; \
        c = crc_table[3][c & 0xff] ^ crc_table[2][(c >> 8) & 0xff] ^ \
            crc_table[1][(c >> 16) & 0xff] ^ crc_table[0][c >> 24]
#define DOLIT32 DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4; DOLIT4

local unsigned long crc32_little(unsigned long crc, const unsigned char FAR* buf, unsigned len)       // 0x00C059D0
{
    register u4 c;
    register const u4 FAR *buf4;

    c = (u4)crc;
    c = ~c;
    while (len && ((ptrdiff_t)buf & 3)) {
        c = crc_table[0][(c ^ *buf++) & 0xff] ^ (c >> 8);
        len--;
    }

    buf4 = (const u4 FAR *)buf;
    while (len >= 32) {
        DOLIT32;
        len -= 32;
    }
    while (len >= 4) {
        DOLIT4;
        len -= 4;
    }
    buf = (const unsigned char FAR *)buf4;

    if (len) do {
        c = crc_table[0][(c ^ *buf++) & 0xff] ^ (c >> 8);
    } while (--len);
    c = ~c;
    return (unsigned long)c;
}

#define REV(w) (((w)>>24)+(((w)>>8)&0xff00)+ \
                (((w)&0xff00)<<8)+(((w)&0xff)<<24))
#define DOBIG4 c ^= *++buf4; \
        c = crc_table[4][c & 0xff] ^ crc_table[5][(c >> 8) & 0xff] ^ \
            crc_table[6][(c >> 16) & 0xff] ^ crc_table[7][c >> 24]
#define DOBIG32 DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4; DOBIG4

local unsigned long crc32_big(unsigned long crc, const unsigned char FAR* buf, unsigned len)
{
    register u4 c;
    register const u4 FAR *buf4;

    c = REV((u4)crc);
    c = ~c;
    while (len && ((ptrdiff_t)buf & 3)) {
        c = crc_table[4][(c >> 24) ^ *buf++] ^ (c << 8);
        len--;
    }

    buf4 = (const u4 FAR *)buf;
    buf4--;
    while (len >= 32) {
        DOBIG32;
        len -= 32;
    }
    while (len >= 4) {
        DOBIG4;
        len -= 4;
    }
    buf4++;
    buf = (const unsigned char FAR *)buf4;

    if (len) do {
        c = crc_table[4][(c >> 24) ^ *buf++] ^ (c << 8);
    } while (--len);
    c = ~c;
    return (unsigned long)(REV(c));
}
}