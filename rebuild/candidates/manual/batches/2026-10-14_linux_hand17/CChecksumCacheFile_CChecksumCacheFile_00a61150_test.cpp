#include <cstdio>
#include <cstdlib>
static int g_nameCtor = 0; static const void* g_opened = 0;
class CWideString { public: void* PStringData; CWideString() : PStringData(0) { ++g_nameCtor; } };
struct SEntryNode { bool Colour; SEntryNode* Parent; SEntryNode* Left; SEntryNode* Right; unsigned char Value[0x24]; };
struct CRetailEntryMap {
    SEntryNode* Header; long NodeCount; unsigned char Compare[4];
    CRetailEntryMap() : Header(0) { Header = static_cast<SEntryNode*>(std::malloc(sizeof(SEntryNode))); NodeCount = 0; Header->Colour = false; Header->Parent = 0; Header->Left = Header; Header->Right = Header; }
};
struct CChecksumCacheFile {
    CWideString Filename; CRetailEntryMap Entries; long BufferSize; bool NeedsSaving;
    CChecksumCacheFile(const CWideString& filename, long bufferSize) : Filename(), Entries(), BufferSize((bufferSize + 8) & ~8), NeedsSaving(false) { Open(filename); }
    void Open(const CWideString& filename) { g_opened = &filename; }
};
int main()
{
    CWideString name; g_nameCtor = 0;
    CChecksumCacheFile f(name, 0x100);
    SEntryNode* h = f.Entries.Header;
    if (g_nameCtor != 1 || h == 0 || h->Colour != false || h->Parent != 0 || h->Left != h || h->Right != h || f.Entries.NodeCount != 0) { std::printf("00a61150_TEST FAIL\n"); return 1; }
    if (f.BufferSize != 0x100 || f.NeedsSaving != false || g_opened != &name) { std::printf("00a61150_TEST FAIL\n"); return 1; }
    CChecksumCacheFile g(name, 0x108);
    if (g.BufferSize != 0x110) { std::printf("00a61150_TEST FAIL\n"); return 1; }
    if (sizeof(SEntryNode) != 0x34) { std::printf("00a61150_TEST FAIL\n"); return 1; }
    std::printf("00a61150_TEST PASS\n"); return 0;
}
