// Behaviour test for 0x0047CB50 map<ECategory, vector<CHeroLogBookEntry> >::operator[] (model over std::map semantics).
#include <cstdio>
#include <cstdlib>

struct Node { Node* left; Node* right; long key; int value; };
static int g_inserts = 0;

struct MapModel {
    Node* root;
    Node* lower_bound(long k, Node*& parent, bool& goLeft)
    {
        Node* y = 0; Node* x = root; parent = 0; goLeft = true;
        while (x != 0) {
            parent = x;
            if (!(x->key < k)) { y = x; x = x->left; goLeft = true; }
            else { x = x->right; goLeft = false; }
        }
        return y;
    }
    int& operator[](long k)
    {
        Node* parent; bool goLeft;
        Node* i = lower_bound(k, parent, goLeft);
        if (i == 0 || k < i->key) {
            Node* z = (Node*)std::malloc(sizeof(Node));
            z->left = z->right = 0; z->key = k; z->value = 0;   // value_type(k, T()): empty mapped value
            if (parent == 0) root = z; else if (goLeft) parent->left = z; else parent->right = z;
            ++g_inserts;
            i = z;
        }
        return i->value;
    }
};

static void FreeAll(Node* n) { if (n) { FreeAll(n->left); FreeAll(n->right); std::free(n); } }

int main()
{
    MapModel m; m.root = 0;
    m[3] = 30;
    m[1] = 10;
    m[3] += 1;                       // existing key: no insert
    int fresh = m[2];                // missing key: default-constructed value
    int ok = m[3] == 31 && m[1] == 10 && fresh == 0 && g_inserts == 3;
    FreeAll(m.root);
    if (!ok) { std::printf("0047cb50_TEST FAIL\n"); return 1; }
    std::printf("0047cb50_TEST PASS\n");
    return 0;
}