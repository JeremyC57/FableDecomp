// Behaviour test for 0x00A60C30 CChecksumCacheFile::Close()
#include <cstdio>
#include <cstdlib>

static int g_saves = 0;
static int g_erased = 0;

class CWideString {
public:
    const wchar_t* PStringData;
    long GetLength() const                                  // 0x0099B220 stand-in
    {
        long n = 0;
        if (PStringData) while (PStringData[n] != 0) ++n;
        return n;
    }
    const CWideString& operator=(const wchar_t* text) { PStringData = text; return *this; }   // 0x0099B800 stand-in
};
struct SEntryNode {
    bool        Colour;
    SEntryNode* Parent;
    SEntryNode* Left;
    SEntryNode* Right;
    unsigned char Value[0x24];
};
class CRetailEntryMap {
public:
    SEntryNode*   Header;
    long          NodeCount;
    unsigned char Compare[4];
    void _M_erase(SEntryNode* node)                         // 0x00A60A00 stand-in: free the subtree
    {
        while (node != 0) {
            _M_erase(node->Right);
            SEntryNode* left = node->Left;
            std::free(node);
            ++g_erased;
            node = left;
        }
    }
    void clear()
    {
        if (NodeCount != 0) {
            _M_erase(Header->Parent);
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            NodeCount = 0;
        }
    }
};
class CChecksumCacheFile {
public:
    CWideString     Filename;
    CRetailEntryMap Entries;
    long            BufferSize;
    bool            NeedsSaving;
    bool IsOpen() const { return Filename.GetLength() != 0; }
    void Save() { ++g_saves; }                              // 0x00A60AA0 stand-in
    void Close()
    {
        if (IsOpen()) {
            Save();
            Entries.clear();
            Filename = L"";
        }
    }
};

static SEntryNode* NewNode(SEntryNode* parent)
{
    SEntryNode* n = (SEntryNode*)std::malloc(sizeof(SEntryNode));
    n->Parent = parent;
    n->Left = n->Right = 0;
    return n;
}

int main()
{
    SEntryNode header;
    CChecksumCacheFile f;
    f.Filename = L"checksums.bin";
    f.Entries.Header = &header;
    SEntryNode* root = NewNode(&header);                    // root with two children
    root->Left = NewNode(root);
    root->Right = NewNode(root);
    header.Parent = root;
    header.Left = root->Left;
    header.Right = root->Right;
    f.Entries.NodeCount = 3;
    f.Close();
    bool ok = g_saves == 1 && g_erased == 3 && f.Entries.NodeCount == 0 && header.Parent == 0 &&
              header.Left == &header && header.Right == &header && f.Filename.GetLength() == 0;
    f.Close();                                              // already closed: nothing happens
    ok = ok && g_saves == 1 && g_erased == 3;
    if (!ok) { std::printf("00a60c30_TEST FAIL\n"); return 1; }
    std::printf("00a60c30_TEST PASS\n");
    return 0;
}