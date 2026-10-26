// Behaviour test for 0x009F0DE0 _Rb_tree<unsigned long, pair<const unsigned long, CSectionDesc>>::_M_create_node: a
// fresh node holds a copy of the value (key, both strings, Loaded, Size).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gCopies = 0;
class CCharString
{ public:
    const char* Text;
    CCharString(const char* t) : Text(t) {}
    CCharString(const CCharString& o) : Text(o.Text) { ++gCopies; }
};
struct CSectionDesc { CCharString ShortName; CCharString LongName; bool Loaded; long Size; };
struct Value { unsigned long first; CSectionDesc second; };
struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Node : NodeBase { Value Field; };

static Node* CreateNode(const Value& x)
{
    Node* tmp = (Node*)std::malloc(sizeof(Node));
    new (&tmp->Field) Value(x);
    return tmp;
}

int main()
{
    Value v = { 42, { CCharString("intro"), CCharString("data\\intro.sec"), true, 1234 } };
    gCopies = 0;
    Node* n = CreateNode(v);
    bool ok = n->Field.first == 42 && std::strcmp(n->Field.second.ShortName.Text, "intro") == 0
           && std::strcmp(n->Field.second.LongName.Text, "data\\intro.sec") == 0 && n->Field.second.Loaded
           && n->Field.second.Size == 1234 && gCopies == 2;
    std::free(n);
    if (!ok) { std::printf("009f0de0_TEST FAIL\n"); return 1; }
    std::printf("009f0de0_TEST PASS\n");
    return 0;
}