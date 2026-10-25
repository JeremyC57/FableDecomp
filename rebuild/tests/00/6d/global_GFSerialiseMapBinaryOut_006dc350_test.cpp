// Behaviour test for 0x006DC350 GFSerialiseMapBinaryOut of map<long, CComboMultiplierData> (model copy): the count, then
// each key and value in key order, with the value traits' DefaultVal map built first and destroyed last.
#include <cstdio>
#include <cstdlib>

struct Node {
    bool color; Node* parent; Node* left; Node* right;
    long key; int value;
};

static Node* increment(Node* x)
{
    if (x->right != 0) {
        x = x->right;
        while (x->left != 0) x = x->left;
    } else {
        Node* y = x->parent;
        while (x == y->right) { x = y; y = y->parent; }
        if (x->right != y) x = y;
    }
    return x;
}

struct Map { Node* header; unsigned long count; };

static int g_traitsLive = 0;
static int g_traitsBuilt = 0;
struct Traits {
    Map DefaultVal;
    Traits() { DefaultVal.header = (Node*)std::malloc(sizeof(Node)); DefaultVal.count = 0;
               DefaultVal.header->color = false; DefaultVal.header->parent = 0;
               DefaultVal.header->left = DefaultVal.header; DefaultVal.header->right = DefaultVal.header;
               ++g_traitsLive; ++g_traitsBuilt; }
    ~Traits() { if (DefaultVal.header != 0) std::free(DefaultVal.header); --g_traitsLive; }
};

struct Stream {
    long out[32]; int n; int liveAtWrite;
    void WriteSLONG(long v) { liveAtWrite += g_traitsLive; out[n++] = v; }
    void WriteValue(int v) { liveAtWrite += g_traitsLive; out[n++] = 1000 + v; }
};

static void SerialiseOut(Stream& s, const Map& m)
{
    Traits valueTraits;
    s.WriteSLONG((long)m.count);
    for (Node* it = m.header->left; it != m.header; it = increment(it)) {
        s.WriteSLONG(it->key);
        s.WriteValue(it->value);
    }
}

int main()
{
    // keys 10 < 20 < 30 < 40: root 20, left 10, right 30 (right child 40)
    Node h, n10, n20, n30, n40;
    h.color = false; h.parent = &n20; h.left = &n10; h.right = &n40;
    n20.color = true; n20.parent = &h; n20.left = &n10; n20.right = &n30; n20.key = 20; n20.value = 2;
    n10.color = true; n10.parent = &n20; n10.left = 0; n10.right = 0; n10.key = 10; n10.value = 1;
    n30.color = true; n30.parent = &n20; n30.left = 0; n30.right = &n40; n30.key = 30; n30.value = 3;
    n40.color = false; n40.parent = &n30; n40.left = 0; n40.right = 0; n40.key = 40; n40.value = 4;
    Map m; m.header = &h; m.count = 4;

    Stream s; s.n = 0; s.liveAtWrite = 0;
    SerialiseOut(s, m);
    long want[9] = { 4, 10, 1001, 20, 1002, 30, 1003, 40, 1004 };
    int ok = s.n == 9 && s.liveAtWrite == 9 && g_traitsLive == 0 && g_traitsBuilt == 1;
    for (int i = 0; ok && i < 9; ++i) if (s.out[i] != want[i]) ok = 0;

    Node e; e.color = false; e.parent = 0; e.left = &e; e.right = &e;
    Map empty; empty.header = &e; empty.count = 0;
    Stream t; t.n = 0; t.liveAtWrite = 0;
    SerialiseOut(t, empty);
    ok = ok && t.n == 1 && t.out[0] == 0 && g_traitsLive == 0 && g_traitsBuilt == 2;

    if (!ok) { std::printf("006dc350_TEST FAIL\n"); return 1; }
    std::printf("006dc350_TEST PASS\n");
    return 0;
}