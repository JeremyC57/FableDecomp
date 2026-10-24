// Behaviour test for 0x006BA3B0 CFaction::CFaction(const CCharString& name).
#include <cstdio>
#include <cstdlib>

static int g_stringCopies = 0;

struct CCharString {
    void* PStringData;
    CCharString(void* p) : PStringData(p) { }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_stringCopies; }
};

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { CCharString value; };

struct CCharStringSet {
    NodeBase*    header;
    unsigned int count;
    unsigned char comp;
    CCharStringSet() : header(0)
    {
        header = (NodeBase*)std::malloc(sizeof(Node));
        count = 0;
        header->color = false;
        header->parent = 0;
        header->left = header;
        header->right = header;
    }
};

struct CFaction {
    CCharString    Name;
    CCharStringSet AlliedFactions;
    CCharStringSet EnemyFactions;
    CFaction(const CCharString& name) : Name(name) { }
};

int main()
{
    int text = 0;
    CCharString name(&text);
    CFaction f(name);
    int ok = f.Name.PStringData == &text && g_stringCopies == 1;
    CCharStringSet* sets[2] = { &f.AlliedFactions, &f.EnemyFactions };
    for (int i = 0; ok && i < 2; ++i) {
        NodeBase* h = sets[i]->header;
        if (!h || sets[i]->count != 0 || h->color != false || h->parent != 0 || h->left != h || h->right != h) ok = 0;
    }
    if (f.AlliedFactions.header == f.EnemyFactions.header) ok = 0;
    std::free(f.AlliedFactions.header); std::free(f.EnemyFactions.header);
    if (!ok) { std::printf("006ba3b0_TEST FAIL\n"); return 1; }
    std::printf("006ba3b0_TEST PASS\n");
    return 0;
}