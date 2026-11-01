// Behaviour test for CMemoryDataOutputStream::~CMemoryDataOutputStream (0x00996B30): the Data buffer is freed when allocated, then
// the NewLine string member of CDataOutputStream, then the CDataStream base.
#include <cstdio>
#include <cstdlib>
static int g_log[4];
static int g_n = 0;
static int g_freed = 0;
class CCharString { public: ~CCharString() { g_log[g_n++] = 2; } void* PStringData; };
class CDataStream { public: virtual ~CDataStream() { g_log[g_n++] = 3; } };
class CDataOutputStream : public CDataStream { protected: CCharString NewLine; };
class CByteArray {
public:
    CByteArray() : Start(0), Finish(0), End(0) { }
    ~CByteArray() { if (Start != 0) { std::free(Start); ++g_freed; g_log[g_n++] = 1; } }
    char* Start; char* Finish; char* End;
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    virtual ~CMemoryDataOutputStream() { }
    CByteArray Data;
    unsigned long CurrentPosition;
};
int main()
{
    {
        CMemoryDataOutputStream s;
        s.Data.Start = (char*)std::malloc(16);
    }
    int ok = g_n == 3 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_freed == 1;
    g_n = 0;
    { CMemoryDataOutputStream empty; }
    ok = ok && g_n == 2 && g_log[0] == 2 && g_log[1] == 3 && g_freed == 1;
    if (!ok) { std::printf("00996b30_TEST FAIL\n"); return 1; }
    std::printf("00996b30_TEST PASS\n");
    return 0;
}