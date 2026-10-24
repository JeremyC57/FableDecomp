// Behaviour test for 0x00469309 GFSerialiseMapBinaryOut<long, CBlendedParticleEffectSet>: count, then key + set per node in order
#include <cstdio>
struct Writer {
    int log[32]; int n;
    void WriteLong(int v) { log[n++] = v; }
};
struct CBlendedParticleEffectSet {
    int Particle1Index; float Particle1Value; int Particle2Index; float Particle2Value;
    void TransferBinaryOut(Writer& w) const { w.WriteLong(Particle1Index); w.WriteLong(Particle2Index); w.WriteLong(1000); w.WriteLong(1001); }
};
struct Node { unsigned char colour; Node* parent; Node* left; Node* right; int key; CBlendedParticleEffectSet value; };
static Node* Increment(Node* x)                      // STLport _Rb_global::_M_increment
{
    if (x->right != 0) { x = x->right; while (x->left != 0) x = x->left; }
    else { Node* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
struct Map { Node* header; unsigned int count; };
static void SerialiseOut(Writer& stream, const Map& m)
{
    stream.WriteLong((int)m.count);
    for (Node* it = m.header->left; it != m.header; it = Increment(it)) {
        stream.WriteLong(it->key);
        it->value.TransferBinaryOut(stream);
    }
}
int main()
{
    Node header, a, b, c;                            // b is the root, a its left child, c its right child
    header.colour = 0; header.parent = &b; header.left = &a; header.right = &c;
    b.colour = 1; b.parent = &header; b.left = &a; b.right = &c; b.key = 20;
    a.colour = 0; a.parent = &b; a.left = 0; a.right = 0; a.key = 10;
    c.colour = 0; c.parent = &b; c.left = 0; c.right = 0; c.key = 30;
    a.value.Particle1Index = 1; a.value.Particle2Index = 2;
    b.value.Particle1Index = 3; b.value.Particle2Index = 4;
    c.value.Particle1Index = 5; c.value.Particle2Index = 6;
    Map m = { &header, 3 };
    Writer w; w.n = 0;
    SerialiseOut(w, m);
    const int want[] = { 3, 10, 1, 2, 1000, 1001, 20, 3, 4, 1000, 1001, 30, 5, 6, 1000, 1001 };
    if (w.n != 16) { std::printf("00469309_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 16; ++i) if (w.log[i] != want[i]) { std::printf("00469309_TEST FAIL\n"); return 1; }
    Node empty; empty.parent = 0; empty.left = &empty; empty.right = &empty;
    Map e = { &empty, 0 }; w.n = 0;
    SerialiseOut(w, e);
    if (w.n != 1 || w.log[0] != 0) { std::printf("00469309_TEST FAIL\n"); return 1; }
    std::printf("00469309_TEST PASS\n");
    return 0;
}