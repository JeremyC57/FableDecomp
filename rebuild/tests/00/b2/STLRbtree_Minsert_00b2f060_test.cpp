#include <cstdio>
#include <cstdlib>
#include <new>

// Model of the STLport _Rb_tree::_M_insert over map<pair<long,long>, CEngineTextureMorph> (retail 0x00B2F060): create the
// node (key + morph copy), link it left/right of y, keep leftmost/rightmost, rebalance, count.
struct Key { long first; long second; };
static bool Less(const Key& x, const Key& y) { return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }

struct BlendVector {
    int* start; int* finish; int* eos;
    BlendVector() : start(0), finish(0), eos(0) { }
    BlendVector(const BlendVector& o) : start(o.start), finish(o.finish), eos(o.eos) { ++s_copies; }
    static int s_copies;
};
int BlendVector::s_copies = 0;

struct Morph { long OriginalTextureIndex; long MorphID; BlendVector Blends; };
struct Value { Key key; Morph morph; };

struct Node {
    bool  color;
    Node* parent;
    Node* left;
    Node* right;
    Value value;
};

static int g_rebalance = 0;
static Node* g_rebalanceNode = 0;
static void Rebalance(Node* x, Node*& root) { ++g_rebalance; g_rebalanceNode = x; (void)root; }

struct Tree {
    Node* header;
    unsigned int count;
    Node* create_node(const Value& v)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        new (&tmp->value) Value(v);
        return tmp;
    }
    Node* insert(Node* x, Node* y, const Value& v, Node* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || Less(v.key, y->value.key)))) {
            z = create_node(v);
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        }
        else {
            z = create_node(v);
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y;
        z->left = 0;
        z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};

int main()
{
    Node header; header.color = false; header.parent = 0; header.left = &header; header.right = &header;
    Tree t; t.header = &header; t.count = 0;
    Value v; v.key.first = 3; v.key.second = 3; v.morph.OriginalTextureIndex = 11; v.morph.MorphID = 22;
    Node* a = t.insert(0, &header, v, 0);
    if (header.parent != a || header.left != a || header.right != a || a->parent != &header || a->value.morph.MorphID != 22 || BlendVector::s_copies != 1) { std::printf("00b2f060_TEST FAIL\n"); return 1; }
    v.key.first = 1;
    Node* b = t.insert(0, a, v, 0);           // 1 < 3 -> left, new leftmost
    if (a->left != b || header.left != b || header.right != a) { std::printf("00b2f060_TEST FAIL\n"); return 1; }
    v.key.first = 7;
    Node* c = t.insert(0, a, v, 0);           // 7 > 3 -> right, new rightmost
    if (a->right != c || header.right != c) { std::printf("00b2f060_TEST FAIL\n"); return 1; }
    v.key.first = 0;
    Node* d = t.insert(0, c, v, c);           // w != 0 forces the right arm
    if (c->right != d || header.right != d) { std::printf("00b2f060_TEST FAIL\n"); return 1; }
    v.key.first = 9;
    Node* e = t.insert(d, d, v, 0);           // x != 0 forces the left arm
    if (d->left != e || e->left != 0 || e->right != 0 || e->parent != d) { std::printf("00b2f060_TEST FAIL\n"); return 1; }
    if (t.count != 5 || g_rebalance != 5 || g_rebalanceNode != e || BlendVector::s_copies != 5) { std::printf("00b2f060_TEST FAIL\n"); return 1; }
    std::free(a); std::free(b); std::free(c); std::free(d); std::free(e);
    std::printf("00b2f060_TEST PASS\n");
    return 0;
}