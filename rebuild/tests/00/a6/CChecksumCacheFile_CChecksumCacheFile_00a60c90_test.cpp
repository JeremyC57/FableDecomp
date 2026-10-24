// Behaviour test for 0x00A60C90 CChecksumCacheFile::CChecksumCacheFile(long bufferSize)
#include <cstdio>
#include <cstdlib>

class CWideString {
public:
    void* PStringData;
    CWideString() : PStringData(0) { }                      // 0x0099AED0 stand-in
};
struct SEntryNode {
    bool        Colour;
    SEntryNode* Parent;
    SEntryNode* Left;
    SEntryNode* Right;
    unsigned char Value[0x24];
};
struct CRetailEntryMap {
    SEntryNode*   Header;
    long          NodeCount;
    unsigned char Compare[4];
    CRetailEntryMap() : Header(0)
    {
        Header = static_cast<SEntryNode*>(std::malloc(sizeof(SEntryNode)));
        NodeCount = 0;
        Header->Colour = false;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
};
class CChecksumCacheFile {
public:
    CWideString     Filename;
    CRetailEntryMap Entries;
    long            BufferSize;
    bool            NeedsSaving;
    CChecksumCacheFile(long bufferSize)
        : Filename(), Entries(), BufferSize((bufferSize + 8) & ~8), NeedsSaving(false)
    {
    }
};

static bool Fresh(const CChecksumCacheFile& f)
{
    const SEntryNode* h = f.Entries.Header;
    return f.Filename.PStringData == 0 && h != 0 && !h->Colour && h->Parent == 0 && h->Left == h && h->Right == h &&
           f.Entries.NodeCount == 0 && !f.NeedsSaving;
}

int main()
{
    CChecksumCacheFile a(100), b(3), c(8), d(0x1000);
    bool ok = Fresh(a) && Fresh(b) && Fresh(c) && Fresh(d) &&
              a.BufferSize == 100 && b.BufferSize == 3 && c.BufferSize == 16 && d.BufferSize == 0x1000;
    std::free(a.Entries.Header); std::free(b.Entries.Header); std::free(c.Entries.Header); std::free(d.Entries.Header);
    if (!ok) { std::printf("00a60c90_TEST FAIL\n"); return 1; }
    std::printf("00a60c90_TEST PASS\n");
    return 0;
}