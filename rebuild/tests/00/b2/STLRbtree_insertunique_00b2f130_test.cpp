#include <cstdio>

// Model of the STLport _Rb_tree::insert_unique over map<pair<long,long>, CEngineTextureMorph> (retail 0x00B2F130),
// driven by a test _M_insert (link without rebalancing) and the standard _M_decrement.
struct Key { long first; long second; };
static bool Less(const Key& x, const Key& y) { return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }

struct Node {
    bool  color;      // false = red
    Node* parent;
    Node* left;
    Node* right;
    Key   key;
};

struct Iterator {
    Node* node;
    void decrement()
    {
        if (node->color == false && node->parent->parent == node)
            node = node->right;
        else if (node->left != 0) {
            Node* y = node->left;
            while (y->right != 0)
                y = y->right;
            node = y;
        }
        else {
            Node* y = node->parent;
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

struct Result { Iterator first; bool second; };

static Node g_pool[16];
static int g_used = 0;

struct Tree {
    Node* header;
    unsigned int count;
    int insertCalls;
    Iterator* insert_node(Iterator* result, Node* x, Node* y, const Key& v, Node* w)
    {
        ++insertCalls;
        Node* z = &g_pool[g_used++];
        z->key = v; z->left = 0; z->right = 0; z->color = true; z->parent = y;
        if (y == header || (w == 0 && (x != 0 || Less(v, y->key)))) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        }
        else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        ++count;
        result->node = z;
        return result;
    }
    Result insert_unique(const Key& v)
    {
        Node* y = header;
        Node* x = header->parent;
        bool comp = true;
        while (x != 0) {
            y = x;
            comp = Less(v, x->key);
            x = comp ? x->left : x->right;
        }
        Iterator j; j.node = y;
        Result r;
        if (comp) {
            if (j.node == header->left) {
                r.first = *insert_node(&j, y, y, v, 0); r.second = true; return r;
            }
            j.decrement();
        }
        if (Less(j.node->key, v)) {
            r.first = *insert_node(&j, x, y, v, 0); r.second = true; return r;
        }
        r.first = j; r.second = false;
        return r;
    }
};

int main()
{
    Node header; header.color = false; header.parent = 0; header.left = &header; header.right = &header;
    Tree t; t.header = &header; t.count = 0; t.insertCalls = 0;
    Key k;
    k.first = 5; k.second = 0; Result r = t.insert_unique(k);
    if (!r.second || r.first.node->key.first != 5 || header.parent != r.first.node || header.left != r.first.node) { std::printf("00b2f130_TEST FAIL\n"); return 1; }
    Node* five = r.first.node;
    k.first = 2; k.second = 3; r = t.insert_unique(k);
    if (!r.second || five->left != r.first.node || header.left != r.first.node) { std::printf("00b2f130_TEST FAIL\n"); return 1; }
    k.first = 9; k.second = 1; r = t.insert_unique(k);
    if (!r.second || five->right != r.first.node || header.right != r.first.node) { std::printf("00b2f130_TEST FAIL\n"); return 1; }
    k.first = 5; k.second = 0; r = t.insert_unique(k);
    if (r.second || r.first.node != five) { std::printf("00b2f130_TEST FAIL\n"); return 1; }
    k.first = 5; k.second = 1; r = t.insert_unique(k);
    if (!r.second) { std::printf("00b2f130_TEST FAIL\n"); return 1; }
    k.first = 2; k.second = 3; r = t.insert_unique(k);
    if (r.second || r.first.node->key.second != 3) { std::printf("00b2f130_TEST FAIL\n"); return 1; }
    if (t.count != 4 || t.insertCalls != 4) { std::printf("00b2f130_TEST FAIL\n"); return 1; }
    std::printf("00b2f130_TEST PASS\n");
    return 0;
}