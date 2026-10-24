#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

struct CChunkedFileRawDataChunk {
    unsigned char* Data;
    unsigned long  ByteSize;
    // the row: replace the payload with a fresh copy of `size` bytes
    void SetData(const void* data, unsigned long size)
    {
        delete[] Data;
        Data = new unsigned char[size];
        ByteSize = size;
        memcpy(Data, data, size);
    }
};
int main()
{
    CChunkedFileRawDataChunk c;
    c.Data = 0;
    c.ByteSize = 0;
    static const char text[] = "chunk payload";
    c.SetData(text, sizeof(text));
    Check(c.ByteSize == sizeof(text) && c.Data != 0 && memcmp(c.Data, text, sizeof(text)) == 0);
    Check((const void*)c.Data != (const void*)text);
    unsigned char bytes[3] = { 1, 2, 250 };
    c.SetData(bytes, 3);
    Check(c.ByteSize == 3 && c.Data[0] == 1 && c.Data[2] == 250);
    c.SetData(bytes, 0);
    Check(c.ByteSize == 0 && c.Data != 0);
    delete[] c.Data;

    if (g_fail) {
        std::printf("00aa7760_TEST FAIL\n");
        return 1;
    }
    std::printf("00aa7760_TEST PASS\n");
    return 0;
}