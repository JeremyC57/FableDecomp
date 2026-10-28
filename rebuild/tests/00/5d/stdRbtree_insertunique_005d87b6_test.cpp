// Behaviour test for 0x005D87B6 _Rb_tree<unsigned long, pair<const unsigned long,long>,...>::insert_unique(iterator hint, const value_type&)
#include <cstdio>

struct Node { bool color; Node* parent; Node* left; Node* right; unsigned long key; long value; };
struct Value { unsigned long first; long second; };
static Node g_pool[64]; static int g_used = 0;
static int g_hintedInserts = 0, g_fallbacks = 0;

static Node* Increment(Node* x)
{
    if (x->right != 0) { x = x->right; while (x->left != 0) x = x->left; }
    else { Node* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
static Node* Decrement(Node* x)
{
    if (x->color == false && x->parent->parent == x) x = x->right;
    else if (x->left != 0) { Node* y = x->left; while (y->right != 0) y = y->right; x = y; }
    else { Node* y = x->parent; while (x == y->left) { x = y; y = y->parent; } x = y; }
    return x;
}
struct Tree {
    Node* header; unsigned int count;
    void Init() { header = &g_pool[g_used++]; header->color = false; header->parent = 0; header->left = header->right = header; count = 0; }
    // unbalanced STLport _M_insert: x != 0 or y == header or v < key(y) links left
    Node* Insert(Node* x, Node* y, const Value& v, Node* w)
    {
        Node* z = &g_pool[g_used++]; z->color = true; z->key = v.first; z->value = v.second; z->left = z->right = 0;
        if (y == header || (w == 0 && (x != 0 || v.first < y->key))) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; ++count;
        return z;
    }
    Node* HintedInsert(Node* x, Node* y, const Value& v, Node* w = 0) { ++g_hintedInserts; return Insert(x, y, v, w); }
    Node* InsertUnique(const Value& v)
    {
        ++g_fallbacks;
        Node* y = header; Node* x = header->parent; bool comp = true;
        while (x != 0) { y = x; comp = v.first < x->key; x = comp ? x->left : x->right; }
        Node* j = y;
        if (comp) { if (j == header->left) return Insert(x, y, v, 0); j = Decrement(j); }
        if (j->key < v.first) return Insert(x, y, v, 0);
        return j;
    }
    Node* InsertUnique(Node* position, const Value& v)
    {
        if (position == header->left) {
            if (count <= 0) return InsertUnique(v);
            if (v.first < position->key) return HintedInsert(position, position, v);
            bool comp_pos_v = position->key < v.first;
            if (comp_pos_v == false) return position;
            Node* after = Increment(position);
            if (after == header) return HintedInsert(0, position, v, position);
            if (v.first < after->key) {
                if (position->right == 0) return HintedInsert(0, position, v, position);
                return HintedInsert(after, after, v);
            }
            return InsertUnique(v);
        } else if (position == header) {
            if (header->right->key < v.first) return HintedInsert(0, header->right, v, position);
            return InsertUnique(v);
        } else {
            Node* before = Decrement(position);
            bool comp_v_pos = v.first < position->key;
            if (comp_v_pos && before->key < v.first) {
                if (before->right == 0) return HintedInsert(0, before, v, before);
                return HintedInsert(position, position, v);
            }
            Node* after = Increment(position);
            bool comp_pos_v = !comp_v_pos;
            if (!comp_v_pos) comp_pos_v = position->key < v.first;
            if (!comp_v_pos && comp_pos_v && (after == header || v.first < after->key)) {
                if (position->right == 0) return HintedInsert(0, position, v, position);
                return HintedInsert(after, after, v);
            }
            if (comp_v_pos == comp_pos_v) return position;
            return InsertUnique(v);
        }
    }
};
static bool Sorted(Tree& t, const unsigned long* keys, int n)
{
    Node* x = t.header->left; int i = 0;
    for (; x != t.header; x = Increment(x), ++i) if (i >= n || x->key != keys[i]) return false;
    return i == n && (int)t.count == n;
}

int main()
{
    Tree t; t.Init();
    Value v10 = { 10, 1 }, v20 = { 20, 2 }, v5 = { 5, 3 }, v15 = { 15, 4 }, v30 = { 30, 5 }, v12 = { 12, 6 }, v25 = { 25, 7 };
    Node* n10 = t.InsertUnique(t.header->left, v10);                      // empty: fallback
    if (g_fallbacks != 1 || n10->key != 10) { std::printf("005d87b6_TEST FAIL\n"); return 1; }
    t.InsertUnique(t.header, v20);                                         // end hint, greater than rightmost
    t.InsertUnique(t.header->left, v5);                                    // begin hint, less than begin
    Node* n20 = t.header->right;
    t.InsertUnique(n20, v15);                                              // interior hint, between before and pos
    t.InsertUnique(n20, v25);                                              // hint just before the insertion point
    if (g_hintedInserts != 4 || g_fallbacks != 1) { std::printf("005d87b6_TEST FAIL\n"); return 1; }
    Node* dup = t.InsertUnique(n10, v10);                                  // equivalent to hint: returns it
    if (dup != n10 || t.count != 5) { std::printf("005d87b6_TEST FAIL\n"); return 1; }
    Node* n12 = t.InsertUnique(t.header->left, v12);                       // bad begin hint: falls back
    if (g_fallbacks != 2 || n12->key != 12) { std::printf("005d87b6_TEST FAIL\n"); return 1; }
    t.InsertUnique(t.header, v30);
    Value v30b = { 30, 9 };
    Node* again = t.InsertUnique(t.header, v30b);                          // end hint, already present: fallback finds it
    if (again->key != 30 || again->value != 5) { std::printf("005d87b6_TEST FAIL\n"); return 1; }
    const unsigned long keys[] = { 5, 10, 12, 15, 20, 25, 30 };
    if (!Sorted(t, keys, 7)) { std::printf("005d87b6_TEST FAIL\n"); return 1; }
    std::printf("005d87b6_TEST PASS\n");
    return 0;
}