#include <stdio.h>
#include <string.h>

// Self-contained /Od behavior harness for COpinionTransientOffset::TransferBinaryIn
// (retail 0x004793f8). Links the row's object; drives the fast path (window has >= 0x18 bytes), the slow path
// (window too small) and the position guard, and checks the observable effects.
// NF104 (q6539, 2026-11-19): the stream is the shared engine/CMemoryDataInputStream.h, passed by reference (Ego_r.pdb
//   ?TransferBinaryIn@COpinionTransientOffset@@QAEXAAVCMemoryDataInputStream@@@Z); the harness builds it in raw storage (no constructor
//   runs, so nothing else links) and sets the CDataInputStream fields by their retail offsets: StreamPos +0x04, CurrentSourcePtr +0x0C,
//   SourceChunkBytesRemaining +0x14.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CMemoryDataInputStream.h"

class COpinionTransientOffset
{ public:
    unsigned char Bytes[0x18];
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

// Slow-path stub so the link resolves; record that it was hit + its args.
static int            g_slowHit = 0;
static unsigned char* g_slowDst = 0;
static long           g_slowSize = 0;

void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* destination, long size)
{
    g_slowHit = 1;
    g_slowDst = destination;
    g_slowSize = size;
}

static long& Position(unsigned char* raw)               { return *(long*)(raw + 0x04); }
static unsigned char*& Cursor(unsigned char* raw)       { return *(unsigned char**)(raw + 0x0C); }
static long& Remaining(unsigned char* raw)              { return *(long*)(raw + 0x14); }
static CMemoryDataInputStream& Stream(unsigned char* raw) { return *(CMemoryDataInputStream*)raw; }

int main(void)
{
    int ok = 1;

    // ---- fast path: window holds >= 0x18 bytes ----
    unsigned char srcbuf[0x40];
    for (int i = 0; i < 0x40; ++i) srcbuf[i] = (unsigned char)(i + 1);

    unsigned long s[sizeof(CMemoryDataInputStream) / 4 + 1];
    unsigned char* raw = (unsigned char*)s;
    memset(s, 0, sizeof(s));
    Position(raw)  = 100;
    Cursor(raw)    = srcbuf + 8;
    Remaining(raw) = 0x40;

    COpinionTransientOffset dst;
    memset(dst.Bytes, 0xAA, sizeof(dst.Bytes));

    g_slowHit = 0;
    dst.TransferBinaryIn(Stream(raw));

    if (g_slowHit)                                    { printf("FAIL slow on fast\n"); ok = 0; }
    if (memcmp(dst.Bytes, srcbuf + 8, 0x18) != 0)     { printf("FAIL bytes\n"); ok = 0; }
    if (Position(raw) != 100 + 0x18)                  { printf("FAIL pos %ld\n", Position(raw)); ok = 0; }
    if (Remaining(raw) != 0x40 - 0x18)                { printf("FAIL remain %ld\n", Remaining(raw)); ok = 0; }
    if (Cursor(raw) != srcbuf + 8 + 0x18)             { printf("FAIL cursor\n"); ok = 0; }

    // ---- slow path: window too small (< 0x18), guard still passes ----
    unsigned long s2[sizeof(CMemoryDataInputStream) / 4 + 1];
    unsigned char* raw2 = (unsigned char*)s2;
    memset(s2, 0, sizeof(s2));
    Position(raw2)  = 0;
    Cursor(raw2)    = srcbuf;
    Remaining(raw2) = 0x10;   // < 0x18

    COpinionTransientOffset dst2;
    memset(dst2.Bytes, 0xBB, sizeof(dst2.Bytes));

    g_slowHit = 0; g_slowDst = 0; g_slowSize = 0;
    dst2.TransferBinaryIn(Stream(raw2));

    if (!g_slowHit)                 { printf("FAIL slow not hit\n"); ok = 0; }
    if (g_slowDst != dst2.Bytes)    { printf("FAIL slow dst\n"); ok = 0; }
    if (g_slowSize != 0x18)         { printf("FAIL slow size %ld\n", g_slowSize); ok = 0; }

    // ---- guard-fail path: position would exceed 0x7FFFFFFF, nothing happens ----
    unsigned long s3[sizeof(CMemoryDataInputStream) / 4 + 1];
    unsigned char* raw3 = (unsigned char*)s3;
    memset(s3, 0, sizeof(s3));
    Position(raw3)  = 0x7FFFFFF0;  // + 0x18 overflows the <= 0x7FFFFFFF bound
    Cursor(raw3)    = srcbuf;
    Remaining(raw3) = 0x40;

    COpinionTransientOffset dst3;
    memset(dst3.Bytes, 0xCC, sizeof(dst3.Bytes));

    g_slowHit = 0;
    dst3.TransferBinaryIn(Stream(raw3));

    if (g_slowHit)                          { printf("FAIL guard slow\n"); ok = 0; }
    if (Position(raw3) != 0x7FFFFFF0)       { printf("FAIL guard pos moved\n"); ok = 0; }
    if (dst3.Bytes[0] != 0xCC)              { printf("FAIL guard dst touched\n"); ok = 0; }

    if (ok) printf("OPINION_TRANSFER_BINARY_IN_004793F8_TEST PASS\n");
    else    printf("OPINION_TRANSFER_BINARY_IN_004793F8_TEST FAIL\n");
    return ok ? 0 : 1;
}
