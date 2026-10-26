// Behaviour test: CScriptedMapBrush::CScriptedMapBrush(const CScriptedMapBrushInit&) -- one zeroed cell per game-area square, empty thing trees, area/name/time/pattern copied
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
    CScriptedMapBrushInit init;
    init.GameArea.TLX = 2; init.GameArea.TLY = 3; init.GameArea.BRX = 6; init.GameArea.BRY = 8;   // 4 x 5
    init.Name.text = "TwinbladesCamp";
    init.CreationTimeSeconds = 12.5f;
    init.CreationPattern = 2;
    CScriptedMapBrush b(init);
    if (b.Cells.finish - b.Cells.start != 20 || b.Cells.eos != b.Cells.finish || b.Cells.start[19].Height != 0.0f || b.Cells.start[7].Theme[2] != 0)
        { std::printf("0082e600_TEST FAIL\n"); return 1; }
    if (!EmptyTree(b.ThingsToDelete) || !EmptyTree(b.ThingsToMove) || !EmptyTree(b.ThingsToAdd))
        { std::printf("0082e600_TEST FAIL\n"); return 1; }
    if (b.GameArea.BRY != 8 || b.GameArea.TLX != 2 || std::strcmp(b.BrushName.text, "TwinbladesCamp") != 0 || b.CreationTimeSeconds != 12.5f || b.CreationPattern != 2)
        { std::printf("0082e600_TEST FAIL\n"); return 1; }
    std::printf("0082e600_TEST PASS\n");
    return 0;
}