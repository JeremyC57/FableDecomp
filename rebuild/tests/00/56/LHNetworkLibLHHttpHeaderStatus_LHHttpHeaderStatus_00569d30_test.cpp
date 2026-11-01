// Test for 0x00569D30 LHNetworkLib::LHHttpHeaderStatus::~LHHttpHeaderStatus: Reset() first, then hsURI / hsHost / hsLocation
// freed, then the document header's Reset(), its header string freed and its header-line vector destroyed.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0, g_statusResets = 0, g_headerResets = 0, g_vectorDtors = 0, g_seq[8], g_n = 0;
static void Log(int v) { if (g_n < 8) g_seq[g_n++] = v; }
struct SBlockString {                                       // models basic_string<char>: frees its block when non-null
    char* Start; int Tag;
    SBlockString() : Start(0), Tag(0) { }
    ~SBlockString() { if (Start) { std::free(Start); ++g_frees; Log(Tag); } }
};
struct SStringVector {                                      // models vector<string>: out-of-line destructor
    SBlockString* Start;
    ~SStringVector() { ++g_vectorDtors; Log(9); delete[] Start; }
};
namespace LHNetworkLib {
class LHHttpHeaders { public:
    ~LHHttpHeaders() { Reset(); }
    void Reset() { ++g_headerResets; Log(5); }
    SStringVector HdrDocumentHeader;
    SBlockString HeaderString;
};
struct LHHttpHeaderStatus {
    ~LHHttpHeaderStatus();
    void Reset() { ++g_statusResets; Log(0); }
    int hsServerCode;
    LHHttpHeaders hsDocumentHeader;
    SBlockString hsLocation;
    long hsDocumentSize;
    SBlockString hsHost;
    SBlockString hsURI;
    unsigned short hsPort;
};
LHHttpHeaderStatus::~LHHttpHeaderStatus()
{
    Reset();
}
}
int main()
{
    {
        LHNetworkLib::LHHttpHeaderStatus s;
        s.hsLocation.Start = (char*)std::malloc(4); s.hsLocation.Tag = 1;
        s.hsHost.Start = (char*)std::malloc(4); s.hsHost.Tag = 2;
        s.hsURI.Start = (char*)std::malloc(4); s.hsURI.Tag = 3;
        s.hsDocumentHeader.HeaderString.Start = (char*)std::malloc(4); s.hsDocumentHeader.HeaderString.Tag = 4;
        s.hsDocumentHeader.HdrDocumentHeader.Start = 0;
    }
    static const int expect[7] = { 0, 3, 2, 1, 5, 4, 9 };
    if (g_n != 7 || g_frees != 4 || g_statusResets != 1 || g_headerResets != 1 || g_vectorDtors != 1) { std::printf("00569d30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (g_seq[i] != expect[i]) { std::printf("00569d30_TEST FAIL\n"); return 1; }
    std::printf("00569d30_TEST PASS\n");
    return 0;
}