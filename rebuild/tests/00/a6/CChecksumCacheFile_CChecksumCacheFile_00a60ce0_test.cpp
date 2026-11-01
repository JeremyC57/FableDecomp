// Test for 0x00A60CE0 CChecksumCacheFile::~CChecksumCacheFile: Close() first, then the Entries map is cleared (erase from the root,
// header reset) and its header freed, then the Filename string is destroyed.
#include <cstdio>
#include <cstdlib>
static int g_seq[8], g_n = 0, g_erased = 0;
static void Log(int v) { if (g_n < 8) g_seq[g_n++] = v; }
struct SNode { int Colour; SNode* Parent; SNode* Left; SNode* Right; };
static void Erase(SNode* x) { while (x) { Erase(x->Right); SNode* y = x->Left; ++g_erased; std::free(x); x = y; } }
struct SMap {                                               // models the STLport map: clear() then free the header
    SNode* Header; unsigned int NodeCount;
    ~SMap()
    {
        if (NodeCount != 0) {
            Erase(Header->Parent); Log(2);
            Header->Left = Header; Header->Parent = 0; Header->Right = Header; NodeCount = 0;
        }
        if (Header) { std::free(Header); Log(3); }
    }
};
class CWideString { public: void* PStringData; ~CWideString() { Log(4); } };
class CChecksumCacheFile { public:
    ~CChecksumCacheFile();
    void Close() { Log(1); }
    CWideString Filename;
    SMap Entries;
    int BufferSize;
    bool NeedsSaving;
};
CChecksumCacheFile::~CChecksumCacheFile()
{
    Close();
}
static SNode* NewNode() { SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->Colour = 0; n->Parent = n->Left = n->Right = 0; return n; }
int main()
{
    {
        CChecksumCacheFile f;
        f.Entries.Header = NewNode();
        SNode* root = NewNode(); root->Left = NewNode(); root->Right = NewNode();
        f.Entries.Header->Parent = root; f.Entries.NodeCount = 3;
    }
    if (g_erased != 3 || g_n != 4 || g_seq[0] != 1 || g_seq[1] != 2 || g_seq[2] != 3 || g_seq[3] != 4) { std::printf("00a60ce0_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CChecksumCacheFile f; f.Entries.Header = NewNode(); f.Entries.NodeCount = 0; }
    if (g_erased != 3 || g_n != 3 || g_seq[0] != 1 || g_seq[1] != 3 || g_seq[2] != 4) { std::printf("00a60ce0_TEST FAIL\n"); return 1; }
    std::printf("00a60ce0_TEST PASS\n");
    return 0;
}