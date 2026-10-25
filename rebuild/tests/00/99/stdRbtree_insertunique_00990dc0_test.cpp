// Behaviour test for 0x00990DC0 _Rb_tree<CPendingObjectSortKey, ..>::insert_unique(position, v) (STLport 4.6 hinted unique insert).
#include <cstdio>
#include <cstdlib>

struct CThreadedFile { unsigned int PhysicalSortKey; };
struct CFileObject { unsigned int FilePos; CThreadedFile* PFile; };
struct Key {
    CFileObject* Object;
    bool operator<(const Key& o) const
    {
        unsigned int a = Object->PFile->PhysicalSortKey, b = o.Object->PFile->PhysicalSortKey;
        if (a < b) return true;
        if (a > b) return false;
        if (Object->FilePos < o.Object->FilePos) return true;
        if (Object->FilePos > o.Object->FilePos) return false;
        return Object < o.Object;
    }
};
struct Node { bool color; Node* parent; Node* left; Node* right; Key key; CFileObject* value; };

struct Tree {
    Node* header; unsigned int count;
    Tree() { header = new Node(); header->parent = 0; header->left = header; header->right = header; count = 0; }
    Node* Insert(Node* x, Node* y, const Key& k)
    {
        Node* z = new Node(); z->key = k; z->value = k.Object;
        if (y == header || x != 0 || k < y->key) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0; ++count;
        return z;
    }
    Node* Increment(Node* n)
    {
        if (n->right) { n = n->right; while (n->left) n = n->left; return n; }
        Node* y = n->parent; while (n == y->right) { n = y; y = y->parent; }
        if (n->right != y) n = y;
        return n;
    }
    // the retail body: begin / end / interior hint cases
    Node* InsertHint(Node* pos, const Key& v)
    {
        bool ins;
        if (pos == header->left) {
            if (count <= 0) return InsertUnique(v, ins);
            if (v < pos->key) return Insert(pos, pos, v);
            bool cpv = pos->key < v;
            if (!cpv) return pos;
            Node* after = Increment(pos);
            if (after == header) return Insert(0, pos, v);
            if (v < after->key) return pos->right == 0 ? Insert(0, pos, v) : Insert(after, after, v);
            return InsertUnique(v, ins);
        } else if (pos == header) {
            if (header->right->key < v) return Insert(0, header->right, v);
            return InsertUnique(v, ins);
        } else {
            Node* before = Decrement(pos, header);
            bool cvp = v < pos->key;
            if (cvp && before->key < v) return before->right == 0 ? Insert(0, before, v) : Insert(pos, pos, v);
            Node* after = Increment(pos);
            bool cpv = !cvp;
            if (!cvp) cpv = pos->key < v;
            if (!cvp && cpv && (after == header || v < after->key))
                return pos->right == 0 ? Insert(0, pos, v) : Insert(after, after, v);
            if (cvp == cpv) return pos;
            return InsertUnique(v, ins);
        }
    }
    static Node* Decrement(Node* n, Node* header)
    {
        if (n == header) return n->right;
        if (n->left) { Node* y = n->left; while (y->right) y = y->right; return y; }
        Node* y = n->parent; while (n == y->left) { n = y; y = y->parent; } return y;
    }
    // the retail body: descend, step back once unless at begin(), insert when new
    Node* InsertUnique(const Key& v, bool& inserted)
    {
        Node* y = header; Node* x = header->parent; bool comp = true;
        while (x != 0) { y = x; comp = v < x->key; x = comp ? x->left : x->right; }
        Node* j = y;
        if (comp) {
            if (j == header->left) { inserted = true; return Insert(y, y, v); }
            j = Decrement(j, header);
        }
        if (j->key < v) { inserted = true; return Insert(x, y, v); }
        inserted = false; return j;
    }
};

static bool InOrder(Tree& t, CFileObject** want, int n)
{
    if (t.count != (unsigned int)n) return false;
    Node* cur = t.header->left;
    for (int i = 0; i < n; ++i) { if (cur == t.header || cur->key.Object != want[i]) return false; cur = t.Increment(cur); }
    return cur == t.header;
}

int main()
{
    CThreadedFile f = { 1 };
    CFileObject o[6];
    for (int i = 0; i < 6; ++i) { o[i].FilePos = (unsigned int)(10 * (i + 1)); o[i].PFile = &f; }
    Key k[6];
    for (int i = 0; i < 6; ++i) k[i].Object = &o[i];
    Tree t;
    // empty tree: begin() == end() == header -> falls back to insert_unique
    if (t.InsertHint(t.header->left, k[2])->key.Object != &o[2]) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    // hint end(): appended after the rightmost
    if (t.InsertHint(t.header, k[4])->key.Object != &o[4]) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    // hint begin(): smaller key goes in front
    if (t.InsertHint(t.header->left, k[0])->key.Object != &o[0]) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    // interior hint (pos = node of o[2]): o[1] lies between before (o[0]) and pos
    Node* p2 = t.Increment(t.header->left);
    if (t.InsertHint(p2, k[1])->key.Object != &o[1]) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    // interior hint after pos: o[3] between o[2] and o[4]
    if (t.InsertHint(p2, k[3])->key.Object != &o[3]) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    // duplicate with the matching hint returns pos and inserts nothing; a bad hint still inserts correctly
    if (t.InsertHint(p2, k[2]) != p2) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    if (t.InsertHint(t.header->left, k[5])->key.Object != &o[5]) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    CFileObject* want[6] = { &o[0], &o[1], &o[2], &o[3], &o[4], &o[5] };
    if (!InOrder(t, want, 6)) { std::printf("00990dc0_TEST FAIL\n"); return 1; }
    std::printf("00990dc0_TEST PASS\n");
    return 0;
}