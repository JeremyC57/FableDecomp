// Behaviour test: CScriptedMapBrush::CScriptedMapBrush() -- no cells, three empty thing trees, empty name
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct NodeBase {
    bool      color;
    NodeBase* parent;
    NodeBase* left;
    NodeBase* right;
};
struct Node : NodeBase {
    u64 key;                                            // the set value / the map key (a map node carries more after it)
};
static int g_frees = 0;
struct CTree {                                          // STLport _Rb_tree: header, count, (empty) compare
    NodeBase*    header;
    unsigned int count;
    CTree(unsigned int nodeSize)                        // inline _Rb_tree ctor: header malloc'd, red, null root, self-linked
    {
        header = (NodeBase*)std::malloc(nodeSize);
        count = 0;
        header->color = false;
        header->parent = 0;
        header->left = header;
        header->right = header;
    }
    NodeBase* end() { return header; }
    NodeBase* find(const u64& k)                        // stand-in for _M_find 0x0082EEB0 / 0x0082EF20 (unbalanced walk)
    {
        NodeBase* y = header;
        NodeBase* x = header->parent;
        while (x)
            if (!(((Node*)x)->key < k)) { y = x; x = x->left; }
            else x = x->right;
        if (y == header || k < ((Node*)y)->key)
            y = header;
        return y;
    }
    void Link(u64 k)                                    // test helper: unbalanced BST insert
    {
        Node* z = (Node*)std::malloc(sizeof(Node));
        z->key = k; z->color = true; z->left = z->right = 0;
        NodeBase** slot = &header->parent;
        NodeBase* parent = header;
        while (*slot) {
            parent = *slot;
            slot = (k < ((Node*)*slot)->key) ? &(*slot)->left : &(*slot)->right;
        }
        *slot = z;
        z->parent = parent;
        ++count;
    }
    static NodeBase* Rebalance_for_erase(NodeBase* z, NodeBase*& root)   // stand-in for 0x0042A34E (unbalanced unlink)
    {
        NodeBase* replacement;
        if (!z->left) replacement = z->right;
        else if (!z->right) replacement = z->left;
        else {
            NodeBase* s = z->right;
            while (s->left) s = s->left;
            if (s->parent != z) {
                s->parent->left = s->right;
                if (s->right) s->right->parent = s->parent;
                s->right = z->right;
                z->right->parent = s;
            }
            s->left = z->left;
            z->left->parent = s;
            replacement = s;
        }
        if (replacement) replacement->parent = z->parent;
        if (root == z) root = replacement;
        else if (z->parent->left == z) z->parent->left = replacement;
        else z->parent->right = replacement;
        return z;
    }
    void erase(NodeBase* position)
    {
        NodeBase* y = Rebalance_for_erase(position, header->parent);
        if (y) { std::free(y); ++g_frees; }
        --count;
    }
};
class C2DBoxI { public: int TLX, TLY, BRX, BRY; };
class CCharString { public:
    const char* text;
    CCharString() : text("") { }
    CCharString(const CCharString& o) : text(o.text) { }
};
struct CScriptedMapCell { float Height; unsigned int Theme[3]; unsigned char ThemeBlend[2]; };
struct CCellVector {
    CScriptedMapCell* start;
    CScriptedMapCell* finish;
    CScriptedMapCell* eos;
    CCellVector() : start(0), finish(0), eos(0) { }
    explicit CCellVector(unsigned int n)                // vector(n) 0x0082F630: n zeroed cells
    {
        start = n ? (CScriptedMapCell*)std::malloc(n * sizeof(CScriptedMapCell)) : 0;
        eos = start + n;
        CScriptedMapCell zero;
        std::memset(&zero, 0, sizeof(zero));
        finish = start;
        for (unsigned int i = 0; i < n; ++i)
            *finish++ = zero;
    }
};
class CScriptedMapBrushInit { public:
    C2DBoxI     GameArea;
    CCharString Name;
    float       CreationTimeSeconds;
    int         CreationPattern;
};
class CScriptedMapBrush { public:
    CCellVector Cells;
    CTree       ThingsToDelete;
    CTree       ThingsToMove;
    CTree       ThingsToAdd;
    C2DBoxI     GameArea;
    CCharString BrushName;
    float       CreationTimeSeconds;
    int         CreationPattern;
    CScriptedMapBrush() : ThingsToDelete(sizeof(Node)), ThingsToMove(sizeof(Node) + 24), ThingsToAdd(sizeof(Node)) { }
    CScriptedMapBrush(const CScriptedMapBrushInit& init)
        : Cells((init.GameArea.BRX - init.GameArea.TLX) * (init.GameArea.BRY - init.GameArea.TLY)),
          ThingsToDelete(sizeof(Node)), ThingsToMove(sizeof(Node) + 24), ThingsToAdd(sizeof(Node)), GameArea(init.GameArea), BrushName(init.Name),
          CreationTimeSeconds(init.CreationTimeSeconds), CreationPattern(init.CreationPattern) { }
};
static bool EmptyTree(const CTree& t)
{
    return t.count == 0 && t.header->parent == 0 && t.header->left == t.header && t.header->right == t.header && !t.header->color;
}

int main()
{
    CScriptedMapBrush b;
    if (b.Cells.start != 0 || b.Cells.finish != 0 || b.Cells.eos != 0)
        { std::printf("0082e6e0_TEST FAIL\n"); return 1; }
    if (!EmptyTree(b.ThingsToDelete) || !EmptyTree(b.ThingsToMove) || !EmptyTree(b.ThingsToAdd))
        { std::printf("0082e6e0_TEST FAIL\n"); return 1; }
    if (b.ThingsToDelete.header == b.ThingsToAdd.header || std::strcmp(b.BrushName.text, "") != 0)
        { std::printf("0082e6e0_TEST FAIL\n"); return 1; }
    std::printf("0082e6e0_TEST PASS\n");
    return 0;
}