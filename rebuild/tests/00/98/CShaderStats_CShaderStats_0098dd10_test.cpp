// Behaviour test for 0x0098DD10 CShaderStats::CShaderStats(): both shader names copy NoneStr, both maps get a fresh
// red header node whose root is null and whose leftmost/rightmost point at itself, node counts zero.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CCharString {
    char text[8];
    CCharString(const char* s) { std::strncpy(text, s, 7); text[7] = 0; }
    CCharString(const CCharString& o) { std::memcpy(text, o.text, 8); }
};
static int g_baseCtors = 0;
struct CBaseClass { CBaseClass() { ++g_baseCtors; } };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Tree {
    NodeBase* header; unsigned int count; char compare;
    Tree() : header(0), count(0)
    {
        header = (NodeBase*)malloc(sizeof(NodeBase) + 12);
        header->color = false; header->parent = 0; header->left = header; header->right = header;
    }
};
static const CCharString NoneStr("NONE");
struct CShaderStats : CBaseClass {
    CCharString CurrentPixelShaderName; Tree PixelShaderChanges;
    CCharString CurrentVertexShaderName; Tree VertexShaderChanges;
    CShaderStats() : CurrentPixelShaderName(NoneStr), CurrentVertexShaderName(NoneStr) { }
};
int main()
{
    int fails = 0;
    CShaderStats s;
    if (g_baseCtors != 1) ++fails;
    if (std::strcmp(s.CurrentPixelShaderName.text, "NONE") || std::strcmp(s.CurrentVertexShaderName.text, "NONE")) ++fails;
    Tree* trees[2] = { &s.PixelShaderChanges, &s.VertexShaderChanges };
    for (int i = 0; i < 2; ++i) {
        NodeBase* h = trees[i]->header;
        if (!h || h->color || h->parent || h->left != h || h->right != h || trees[i]->count != 0) ++fails;
    }
    if (s.PixelShaderChanges.header == s.VertexShaderChanges.header) ++fails;
    free(s.PixelShaderChanges.header); free(s.VertexShaderChanges.header);
    std::printf(fails ? "0098dd10_TEST FAIL\n" : "0098dd10_TEST PASS\n");
    return fails ? 1 : 0;
}