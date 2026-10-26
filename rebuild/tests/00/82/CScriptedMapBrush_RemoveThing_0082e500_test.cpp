// Behaviour test: CScriptedMapBrush::RemoveThing -- the uid leaves the move map, the delete set and the add set; absent uids are ignored
#include <cstdio>
#include <cstdlib>

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

class CScriptedMapBrush { public:
    CTree ThingsToDelete;
    CTree ThingsToMove;
    CTree ThingsToAdd;
    CScriptedMapBrush() : ThingsToDelete(sizeof(Node)), ThingsToMove(sizeof(Node) + 24), ThingsToAdd(sizeof(Node)) { }
    void RemoveThing(u64 uid)
    {
        NodeBase* moved = ThingsToMove.find(uid);
        if (moved != ThingsToMove.end())
            ThingsToMove.erase(moved);
        NodeBase* deleted = ThingsToDelete.find(uid);
        if (deleted != ThingsToDelete.end())
            ThingsToDelete.erase(deleted);
        NodeBase* added = ThingsToAdd.find(uid);
        if (added != ThingsToAdd.end())
            ThingsToAdd.erase(added);
    }
};

int main()
{
    CScriptedMapBrush b;
    u64 big = ((u64)7 << 32) | 5;                        // the high dword matters to the compare
    b.ThingsToMove.Link(10); b.ThingsToMove.Link(big); b.ThingsToMove.Link(3);
    b.ThingsToDelete.Link(big); b.ThingsToDelete.Link(5);
    b.ThingsToAdd.Link(4);
    b.RemoveThing(big);
    if (b.ThingsToMove.count != 2 || b.ThingsToDelete.count != 1 || b.ThingsToAdd.count != 1 || g_frees != 2
        || b.ThingsToMove.find(big) != b.ThingsToMove.end() || b.ThingsToMove.find(10) == b.ThingsToMove.end()
        || b.ThingsToDelete.find(5) == b.ThingsToDelete.end())
        { std::printf("0082e500_TEST FAIL\n"); return 1; }
    b.RemoveThing(5);                                    // only in the delete set (the low dword alone differs from `big`)
    b.RemoveThing(99);                                   // nowhere
    if (b.ThingsToMove.count != 2 || b.ThingsToDelete.count != 0 || b.ThingsToAdd.count != 1 || g_frees != 3)
        { std::printf("0082e500_TEST FAIL\n"); return 1; }
    std::printf("0082e500_TEST PASS\n");
    return 0;
}