#include <cstdio>
// Behaviour test for CFileDataInputStream scalar deleting destructor (0x009971A0): the derived Close runs
// (statically bound) before the CDataStream and CBase layers.
static int g_order[8]; static int g_n = 0;
struct CBase { virtual ~CBase() { g_order[g_n++] = 1; } };
struct CDataStream : CBase { virtual void Close() { g_order[g_n++] = 7; } virtual ~CDataStream() { g_order[g_n++] = 2; } };
struct CFileDataInputStream : CDataStream {
    void* File; unsigned char* Buffer; long BufferSize;
    virtual void Close() { g_order[g_n++] = 3; }
    virtual ~CFileDataInputStream() { Close(); }
};
int main()
{
    CBase* p = new CFileDataInputStream;
    delete p;
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) { std::printf("009971a0_TEST FAIL\n"); return 1; }
    std::printf("009971a0_TEST PASS\n");
    return 0;
}