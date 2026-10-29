#include <cstdio>
#include <cstddef>
// Behaviour test for CChunkedFileRawDataChunk scalar deleting destructor (0x00AA8D80): the raw data block is
// released with delete[] (null is fine), then the CChunkedFileChunk and CBase layers.
static int g_order[8]; static int g_n = 0;
class CBaseClassNonCopyable {
public:  virtual ~CBaseClassNonCopyable() { g_order[g_n++] = 1; } };
class CChunkedFileChunk : public CBaseClassNonCopyable { public: virtual ~CChunkedFileChunk() { g_order[g_n++] = 2; } };
struct CChunkedFileRawDataChunk : CChunkedFileChunk {
    unsigned char* Data; unsigned long ByteSize;
    CChunkedFileRawDataChunk() : Data(0), ByteSize(0) { }
    virtual ~CChunkedFileRawDataChunk() { delete[] Data; g_order[g_n++] = 3; }
};
int main()
{
    CChunkedFileRawDataChunk* c = new CChunkedFileRawDataChunk;
    c->Data = new unsigned char[16]; c->ByteSize = 16;
    CBaseClassNonCopyable* p = c;
    delete p;
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) { std::printf("00aa8d80_TEST FAIL\n"); return 1; }
    g_n = 0;
    p = new CChunkedFileRawDataChunk;     // null data block
    delete p;
    if (g_n != 3) { std::printf("00aa8d80_TEST FAIL\n"); return 1; }
    std::printf("00aa8d80_TEST PASS\n");
    return 0;
}