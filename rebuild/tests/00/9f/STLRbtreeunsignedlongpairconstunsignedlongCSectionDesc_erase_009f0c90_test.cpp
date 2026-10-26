// Behaviour test for 0x009F0C90 _Rb_tree<unsigned long, pair<const unsigned long, CSectionDesc>>::erase(iterator): the
// rebalance gets the node and the header's root/leftmost/rightmost, the value's two strings are destroyed (LongName
// first), the node is freed and the count drops by one.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static char gLog[64];

class CCharString { public: const char* Name; ~CCharString() { std::strcat(gLog, Name); std::strcat(gLog, ";"); } };
struct CSectionDesc { CCharString ShortName; CCharString LongName; bool Loaded; long Size; };

struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Node : NodeBase { unsigned long Key; CSectionDesc Value; };

static NodeBase* gZ; static NodeBase** gRoot; static NodeBase** gLeft; static NodeBase** gRight;
static NodeBase* RebalanceForErase(NodeBase* z, NodeBase*& root, NodeBase*& leftmost, NodeBase*& rightmost)
{
    gZ = z; gRoot = &root; gLeft = &leftmost; gRight = &rightmost;
    root = 0; leftmost = rightmost = 0;            // the only node goes away
    return z;
}

struct Tree
{
    NodeBase* Header;
    unsigned int Count;
    void erase(NodeBase* position)
    {
        Node* y = (Node*)RebalanceForErase(position, Header->Parent, Header->Left, Header->Right);
        y->Value.~CSectionDesc();
        if (y) std::free(y);
        --Count;
    }
};

int main()
{
    NodeBase header;
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->Key = 7; n->Value.ShortName.Name = "short"; n->Value.LongName.Name = "long";
    header.Parent = header.Left = header.Right = n;
    Tree t; t.Header = &header; t.Count = 1;
    gLog[0] = 0;
    t.erase(n);
    bool ok = gZ == n && gRoot == &header.Parent && gLeft == &header.Left && gRight == &header.Right
           && t.Count == 0 && std::strcmp(gLog, "long;short;") == 0 && header.Parent == 0;
    if (!ok) { std::printf("009f0c90_TEST FAIL\n"); return 1; }
    std::printf("009f0c90_TEST PASS\n");
    return 0;
}