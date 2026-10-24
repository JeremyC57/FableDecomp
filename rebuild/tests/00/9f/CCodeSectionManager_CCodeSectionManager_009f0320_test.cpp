// Behaviour test for 0x009F0320 CCodeSectionManager::CCodeSectionManager: each map gets a fresh header node that is
// red, has no root and points at itself as leftmost and rightmost; the node count is 0 and section loading is off.
#include <cstdio>
#include <cstdlib>

struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };

template <unsigned int ValueSize> struct Tree           // node = 0x10-byte base (32-bit) + the value
{
    NodeBase* Header;
    unsigned int Count;
    Tree() : Header(0)
    {
        Header = (NodeBase*)std::malloc(sizeof(NodeBase) + ValueSize);
        Count = 0;
        Header->Color = false;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
    ~Tree() { std::free(Header); }
    bool Empty() const { return Count == 0 && !Header->Color && Header->Parent == 0 && Header->Left == Header && Header->Right == Header; }
};

struct CCodeSectionManager
{
    Tree<0x08> ShortNameToHandle;
    Tree<0x08> LongNameToHandle;
    Tree<0x14> SectionDesc;
    bool SectionLoadingEnabled;
    CCodeSectionManager() : SectionLoadingEnabled(false) {}
};

int main()
{
    CCodeSectionManager m;
    bool ok = m.ShortNameToHandle.Empty() && m.LongNameToHandle.Empty() && m.SectionDesc.Empty()
           && !m.SectionLoadingEnabled
           && m.ShortNameToHandle.Header != m.LongNameToHandle.Header && m.LongNameToHandle.Header != m.SectionDesc.Header;
    if (!ok) { std::printf("009f0320_TEST FAIL\n"); return 1; }
    std::printf("009f0320_TEST PASS\n");
    return 0;
}