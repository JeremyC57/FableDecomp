// Behaviour test for CDefStringTable::~CDefStringTable (0x009D4E90): clear map, members in reverse order, map clear + header free.
#include <cstdio>
#include <cstdlib>

static char g_log[16];
static int g_logLength = 0;
static int g_erased = 0;
static void Log(char c) { if (g_logLength < 15) g_log[g_logLength++] = c; g_log[g_logLength] = 0; }

struct Node { bool colour; Node* parent; Node* left; Node* right; };

struct Tree {
    Node* header;
    unsigned int count;
    void Erase(Node* x) { while (x) { Erase(x->right); Node* y = x->left; std::free(x); ++g_erased; x = y; } }
    void Clear()
    {
        Log('c');
        if (count != 0) {
            Erase(header->parent);
            header->left = header;
            header->parent = 0;
            header->right = header;
            count = 0;
        }
    }
    ~Tree() { Clear(); if (header) { std::free(header); Log('h'); } }
};

struct Stream { ~Stream() { Log('s'); } };
struct WideString { ~WideString() { Log('w'); } };

struct DefStringTable {
    Tree StringMap;
    Stream StringStream;
    WideString TableFilename;
    ~DefStringTable() { StringMap.Clear(); }
};

static Node* NewNode(Node* parent)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->colour = false; n->parent = parent; n->left = 0; n->right = 0;
    return n;
}

int main()
{
    {
        DefStringTable table;
        table.StringMap.header = NewNode(0);
        Node* root = NewNode(table.StringMap.header);
        root->left = NewNode(root);
        root->right = NewNode(root);
        table.StringMap.header->parent = root;
        table.StringMap.count = 3;
    }
    if (g_erased != 3) { std::printf("009d4e90_TEST FAIL\n"); return 1; }
    const char* expected = "cwsch";
    for (int i = 0; ; ++i) {
        if (g_log[i] != expected[i]) { std::printf("009d4e90_TEST FAIL\n"); return 1; }
        if (!expected[i]) break;
    }
    std::printf("009d4e90_TEST PASS\n");
    return 0;
}