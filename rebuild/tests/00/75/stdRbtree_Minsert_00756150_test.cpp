// Behaviour test for 0x00756150 std::_Rb_tree::_M_insert over pair<const CCharString, pair<long,long> > (CTCHeroRememberCrimes): left/right link choice, header root/leftmost/rightmost upkeep, w-hint goes right, x != 0 goes left, node count, key copy
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

namespace model {

struct Value { int first; int second; };
Value make(int i) { Value v; v.first = i; v.second = i * 1000; return v; }
bool check(const Value& v, int i) { return v.first == i && v.second == i * 1000; }

int g_strings = 0;
struct CCharString {                                  // owned copy of the text (CCharString copy constructor)
    char* Text;
    CCharString(const char* t) : Text((char*)std::malloc(std::strlen(t) + 1)) { std::strcpy(Text, t); ++g_strings; }
    CCharString(const CCharString& o) : Text((char*)std::malloc(std::strlen(o.Text) + 1)) { std::strcpy(Text, o.Text); ++g_strings; }
    ~CCharString() { std::free(Text); --g_strings; }
    bool operator<(const CCharString& o) const { return std::strcmp(Text, o.Text) < 0; }
};

struct Node {
    int   Colour;
    Node* Parent;
    Node* Left;
    Node* Right;
    CCharString Key;
    Value Data;
    Node(const CCharString& k, const Value& d) : Colour(0), Parent(0), Left(0), Right(0), Key(k), Data(d) {}
};

struct Tree {
    Node* Header;                                     // Header->Parent = root, Left = leftmost, Right = rightmost
    unsigned int Count;
    int Rebalances;
    Tree() : Header((Node*)std::malloc(sizeof(Node))), Count(0), Rebalances(0)
    { Header->Parent = 0; Header->Left = Header; Header->Right = Header; }
    // STLport 4.6 _M_insert(x, y, v, w): left of y when y is the header, or (no hint and (x != 0 or v < y)), else right.
    Node* insert_at(Node* x, Node* y, const CCharString& k, const Value& v, Node* w)
    {
        Node* z;
        if (y == Header || (w == 0 && (x != 0 || k < y->Key))) {
            z = new (std::malloc(sizeof(Node))) Node(k, v);
            y->Left = z;
            if (y == Header) {
                Header->Parent = z;
                Header->Right = z;
            }
            else if (y == Header->Left)
                Header->Left = z;
        }
        else {
            z = new (std::malloc(sizeof(Node))) Node(k, v);
            y->Right = z;
            if (y == Header->Right)
                Header->Right = z;
        }
        z->Parent = y;
        z->Left = 0;
        z->Right = 0;
        ++Rebalances;                                 // _Rb_global<bool>::_Rebalance(z, root) is a separate callee
        ++Count;
        return z;
    }
    Node* insert_unique(const char* text, const Value& v)    // the caller's descent
    {
        CCharString k(text);
        Node* y = Header;
        Node* x = Header->Parent;
        while (x != 0) { y = x; x = k < x->Key ? x->Left : x->Right; }
        return insert_at(0, y, k, v, 0);
    }
    void walk(Node* n, const char** out, int* i) const
    {
        if (!n) return;
        walk(n->Left, out, i);
        out[(*i)++] = n->Key.Text;
        walk(n->Right, out, i);
    }
    void destroy(Node* n) { if (!n) return; destroy(n->Left); destroy(n->Right); n->~Node(); std::free(n); }
    ~Tree() { destroy(Header->Parent); std::free(Header); }
};

}

int main()
{
    using namespace model;
    int bad = 0;
    {
        Tree t;
        const char* keys[7] = { "MORPH_D", "MORPH_B", "MORPH_F", "MORPH_A", "MORPH_C", "MORPH_G", "MORPH_E" };
        for (int i = 0; i < 7; ++i)
            if (std::strcmp(t.insert_unique(keys[i], make(i))->Key.Text, keys[i]) != 0) bad = 1;
        if (t.Count != 7 || t.Rebalances != 7 || g_strings != 7) bad = 2;
        if (std::strcmp(t.Header->Left->Key.Text, "MORPH_A") != 0 || std::strcmp(t.Header->Right->Key.Text, "MORPH_G") != 0) bad = 3;
        const char* order[7];
        int n = 0;
        t.walk(t.Header->Parent, order, &n);
        for (int i = 1; i < n; ++i)
            if (std::strcmp(order[i - 1], order[i]) >= 0) bad = 4;
        // a hinted insert (w != 0) always goes right of y: after the rightmost it becomes the new rightmost
        Node* r = t.insert_at(0, t.Header->Right, CCharString("MORPH_H"), make(7), t.Header->Right);
        if (t.Header->Right != r || r->Parent->Right != r || !check(r->Data, 7)) bad = 5;
        // x != 0 forces the left link even when v is not less than y
        Node* leftmost = t.Header->Left;
        Node* l = t.insert_at(leftmost, leftmost, CCharString("MORPH_0"), make(8), 0);
        if (t.Header->Left != l || leftmost->Left != l) bad = 6;
        if (!check(t.Header->Parent->Data, 0)) bad = 7;          // the first insert is still the root (no rebalance here)
    }
    if (g_strings != 0) bad = 8;
    if (bad) { std::printf("00756150_TEST FAIL\n"); return 1; }
    std::printf("00756150_TEST PASS\n");
    return 0;
}