#include <cstdio>
// Behaviour copy of GFSerialiseMapBinaryOut<CWeaponTrailGraphicSet, EObjectAugmentationType>: count, then key/Attack/Knockdown
// per node in in-order (key) order, walking an STLport-shaped tree (header->left = leftmost, successor step).
struct CWeaponTrailGraphicSet { long Attack; long Knockdown; };
struct SNodeBase { unsigned char color; SNodeBase* parent; SNodeBase* left; SNodeBase* right; };
struct SNode : SNodeBase { long key; CWeaponTrailGraphicSet value; };
struct SMap { SNodeBase* header; unsigned long count; };
struct SStream {
    long out[64]; int n;
    SStream() : n(0) { }
    void WriteSLONG(long v) { out[n++] = v; }
};
static SNodeBase* Increment(SNodeBase* x)
{
    if (x->right != 0) {
        x = x->right;
        while (x->left != 0) x = x->left;
    } else {
        SNodeBase* y = x->parent;
        while (x == y->right) { x = y; y = y->parent; }
        if (x->right != y) x = y;
    }
    return x;
}
static void SerialiseOut(SStream& stream, const SMap& m)
{
    stream.WriteSLONG((long)m.count);
    for (SNodeBase* it = m.header->left; it != m.header; it = Increment(it)) {
        const SNode* node = static_cast<const SNode*>(it);
        stream.WriteSLONG(node->key);
        stream.WriteSLONG(node->value.Attack);
        stream.WriteSLONG(node->value.Knockdown);
    }
}
int main()
{
    bool ok = true;
    // tree: root 5, left 2, right 9; header parent = root, left = leftmost (2), right = rightmost (9)
    SNodeBase header; SNode a, b, c;
    header.color = 0; header.parent = &b; header.left = &a; header.right = &c;
    b.color = 1; b.parent = &header; b.left = &a; b.right = &c; b.key = 5; b.value.Attack = 50; b.value.Knockdown = 51;
    a.color = 0; a.parent = &b; a.left = 0; a.right = 0; a.key = 2; a.value.Attack = 20; a.value.Knockdown = 21;
    c.color = 0; c.parent = &b; c.left = 0; c.right = 0; c.key = 9; c.value.Attack = -1; c.value.Knockdown = 91;
    SMap m; m.header = &header; m.count = 3;
    SStream s; SerialiseOut(s, m);
    const long expect[10] = { 3, 2, 20, 21, 5, 50, 51, 9, -1, 91 };
    if (s.n != 10) ok = false;
    for (int i = 0; ok && i < 10; ++i) if (s.out[i] != expect[i]) ok = false;
    // empty map: only the count
    SNodeBase h2; h2.color = 0; h2.parent = 0; h2.left = &h2; h2.right = &h2;
    SMap e; e.header = &h2; e.count = 0;
    SStream s2; SerialiseOut(s2, e);
    if (s2.n != 1 || s2.out[0] != 0) ok = false;
    if (!ok) { std::printf("004e910f_TEST FAIL\n"); return 1; }
    std::printf("004e910f_TEST PASS\n");
    return 0;
}