// crc32_little(unsigned long crc, const unsigned char* buf, unsigned len) -- zlib 1.2.1 crc32.c little-endian BYFOUR CRC-32 (byte loop to alignment, 32-byte DOLIT32 loop, DOLIT4 loop, byte tail)
// Vendor-proof: zlib-1.2.1(crc32.c) _crc32_little -- 722 bytes verified against Fable.exe 0x00C059D0
// Leaf: _crc32_little
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C059D0, 722 bytes. zlib 1.2.1 crc32.c (Z_PREFIX build, BYFOUR), C linkage.
// Identity: the sole caller z_crc32 0x00C05FD0 (landed) tail-jumps here after the folded endian test (the little-endian arm); the body reads crc_table[0..3] (0x012AF780..0x012B0380) only.
// Shape: the whole crc32.c TU as landed for z_crc32 0x00C05FD0 (same source): the crc32.c statics inside
//   `extern "C" { static __declspec(noinline) }` get VC7.1's private convention (crc eax, buf ecx, len edx; caller pops
//   nothing, plain ret). crc_table is crc32.h's 8x256 table (retail .rdata 0x012AF780, declared extern here).
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