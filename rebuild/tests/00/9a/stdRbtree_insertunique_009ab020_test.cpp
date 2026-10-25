// behaviour test for 0x009AB020 _Rb_tree<CCharString, ...>::insert_unique(v) (map<CCharString, NBankFileManager::CContainedBank>): new keys insert (true) at the walk's parent, equal keys return the existing node (false), a key below the leftmost takes the begin() branch; the inline CCharString order puts a null string first
#include <cstdio>
#include <cstring>
struct Node { bool color; Node* parent; Node* left; Node* right; const char* key; };
struct Result { Node* it; bool inserted; };
static bool Less(const char* a, const char* b)          // CCharString::operator< (data pointers)
{
    if (b == a) return false;
    if (a == 0) return true;
    if (b == 0) return false;
    return std::strcmp(a, b) < 0;
}
static Node gNodes[32]; static int gUsed = 0;
struct Tree {
    Node* header; unsigned int count; int begins;
    Node* Insert(Node* x, Node* y, const char* v)
    {
        Node* z = &gNodes[gUsed++];
        z->key = v; z->left = 0; z->right = 0; z->parent = y; z->color = false;
        if (y == header || x != 0 || Less(v, y->key)) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        ++count;
        return z;
    }
    static Node* Decrement(Node* n)
    {
        if (n->left != 0) { Node* y = n->left; while (y->right) y = y->right; return y; }
        Node* y = n->parent;
        while (n == y->left) { n = y; y = y->parent; }
        return y;
    }
    Result InsertUnique(const char* v)
    {
        Node* y = header; Node* x = header->parent; bool comp = true;
        while (x != 0) { y = x; comp = Less(v, x->key); x = comp ? x->left : x->right; }
        Node* j = y;
        Result r;
        if (comp) {
            if (j == header->left) { ++begins; r.it = Insert(y, y, v); r.inserted = true; return r; }
            j = Decrement(j);
        }
        if (Less(j->key, v)) { r.it = Insert(x, y, v); r.inserted = true; return r; }
        r.it = j; r.inserted = false; return r;
    }
};
int main()
{
    int fails = 0;
    Node header; header.color = true; header.parent = 0; header.left = &header; header.right = &header; header.key = 0;
    Tree t; t.header = &header; t.count = 0; t.begins = 0;
    const char* keys[] = { "mango", "apple", "zebra", "kiwi", "banana", "apple", "zebra", "aardvark", 0, "kiwi" };
    Result r[10];
    for (int i = 0; i < 10; ++i) r[i] = t.InsertUnique(keys[i]);
    if (!r[0].inserted || !r[1].inserted || !r[2].inserted || !r[3].inserted || !r[4].inserted) fails |= 1;
    if (r[5].inserted || r[5].it != r[1].it || r[6].inserted || r[6].it != r[2].it || r[9].inserted || r[9].it != r[3].it) fails |= 2;
    if (!r[7].inserted || !r[8].inserted || header.left != r[8].it) fails |= 4;        // null key sorts first
    if (t.count != 7) fails |= 8;
    if (t.begins < 3) fails |= 16;
    if (header.right != r[2].it) fails |= 32;
    if (fails) { std::printf("009ab020_TEST FAIL %d\n", fails); return 1; }
    std::printf("009ab020_TEST PASS\n");
    return 0;
}