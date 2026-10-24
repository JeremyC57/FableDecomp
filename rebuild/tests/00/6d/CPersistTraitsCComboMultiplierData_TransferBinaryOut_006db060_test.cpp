// Behaviour test for 0x006DB060 CPersistTraits<CComboMultiplierData>::TransferBinaryOut: the two attenuation floats, the map size, then each
// (key, float) entry in in-order (sorted) sequence via the STLport successor walk.
#include <cstdio>

struct Stream {
    long longs[16]; int nLongs; float floats[16]; int nFloats;
    void WriteSLONG(long v) { longs[nLongs++] = v; }
    void WriteFloat(float v) { floats[nFloats++] = v; }
};
struct NodeBase { unsigned char color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { long first; float second; };

static NodeBase* Increment(NodeBase* x)
{
    if (x->right) { x = x->right; while (x->left) x = x->left; }
    else { NodeBase* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
struct Map { NodeBase* header; unsigned long count; unsigned char cmp[4]; float seconds; float decay; };

static void TransferBinaryOut(Stream& s, const Map& m)
{
    s.WriteFloat(m.seconds);
    s.WriteFloat(m.decay);
    s.WriteSLONG(m.count);
    for (NodeBase* it = m.header->left; it != m.header; it = Increment(it)) { s.WriteSLONG(((Node*)it)->first); s.WriteFloat(((Node*)it)->second); }
}

int main()
{
    Node header, a, b, c;                         // b root, a left, c right
    header.color = 0; a.first = 1; a.second = 0.5f; b.first = 2; b.second = 1.5f; c.first = 3; c.second = 2.5f;
    header.parent = &b; header.left = &a; header.right = &c;
    b.parent = &header; b.left = &a; b.right = &c;
    a.parent = &b; a.left = a.right = 0; c.parent = &b; c.left = c.right = 0;
    Map m; m.header = &header; m.count = 3; m.seconds = 4.0f; m.decay = 0.25f;
    Stream s; s.nLongs = s.nFloats = 0;
    TransferBinaryOut(s, m);
    if (s.nLongs != 4 || s.longs[0] != 3 || s.longs[1] != 1 || s.longs[2] != 2 || s.longs[3] != 3) { std::printf("006db060_TEST FAIL\n"); return 1; }
    if (s.nFloats != 5 || s.floats[0] != 4.0f || s.floats[1] != 0.25f || s.floats[2] != 0.5f || s.floats[4] != 2.5f) { std::printf("006db060_TEST FAIL\n"); return 1; }
    std::printf("006db060_TEST PASS\n");
    return 0;
}