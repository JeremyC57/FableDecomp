// Behaviour test: NScript::CQ_AwakeningTheOracleScript::~CQ_AwakeningTheOracleScript (0x00D005E0) -- the STLport tree
// clear() run by each map's dtor (erase from the root when non-empty, reset the header links and count), the virtual
// dtor of every DiggingPoints element, then the script base.
#include <cstdio>
#include <new>
struct Node { bool color; Node* parent; Node* left; Node* right; };
static int g_erased = 0, g_freed = 0, g_things = 0, g_base = 0;
struct Tree {
    Node* header; unsigned count;
    Tree() : header(new Node) { header->parent = 0; header->left = header->right = header; count = 0; }
    void _M_erase(Node* x) { if (x) ++g_erased; }
    void clear()
    {
        if (count != 0) {
            _M_erase(header->parent);
            header->left = header; header->parent = 0; header->right = header; count = 0;
        }
    }
    ~Tree() { clear(); delete header; ++g_freed; }
};
class CScriptThing { public: virtual ~CScriptThing() { ++g_things; } int pad; };
class CScriptBase { public: virtual ~CScriptBase() { ++g_base; } };
struct CQ_AwakeningTheOracleScript : CScriptBase {
    CScriptThing* DiggingStart; CScriptThing* DiggingFinish;
    Tree cs_args; Tree cs_chars;
    CQ_AwakeningTheOracleScript() : DiggingStart(0), DiggingFinish(0) { }
    ~CQ_AwakeningTheOracleScript() { for (CScriptThing* p = DiggingStart; p != DiggingFinish; ++p) p->~CScriptThing(); }
};
int main()
{
    static Node root;
    char raw[3 * sizeof(CScriptThing)];
    CScriptThing* things = reinterpret_cast<CScriptThing*>(raw);
    for (int i = 0; i < 3; ++i) new (static_cast<void*>(&things[i])) CScriptThing();
    {
        CQ_AwakeningTheOracleScript s;
        s.DiggingStart = things; s.DiggingFinish = things + 3;
        s.cs_chars.header->parent = &root; s.cs_chars.count = 1;
        if (s.cs_args.header->left != s.cs_args.header) { std::printf("00d005e0_TEST FAIL\n"); return 1; }
    }
    if (g_erased != 1 || g_freed != 2 || g_things != 3 || g_base != 1) { std::printf("00d005e0_TEST FAIL\n"); return 1; }
    std::printf("00d005e0_TEST PASS\n");
    return 0;
}