// Behaviour test for 0x00472841 GFSerialiseMapBinaryOut<CCharString, long>: count, then key/string pairs in key order;
// the traits' default string is built once and destroyed once
#include <cstdio>
#include <cstring>
static int ctorCount = 0, dtorCount = 0;
struct CCharString { const char* text; CCharString() : text("") { ++ctorCount; } CCharString(const char* t) : text(t) {} ~CCharString() { if (text[0] == 0) ++dtorCount; } };
struct Writer {
    char log[256]; int len;
    void WriteSLONG(int v) { len += std::sprintf(log + len, "%d;", v); }
    void WriteNullTerminatedString(const CCharString& s) { len += std::sprintf(log + len, "%s;", s.text); }
};
struct NodeBase { NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { int key; CCharString value; Node(int k, const char* v) : key(k), value(v) {} };
static NodeBase* Increment(NodeBase* x)
{
    if (x->right) { x = x->right; while (x->left) x = x->left; return x; }
    NodeBase* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; return x;
}
struct Map { NodeBase* header; unsigned int count; };
static void SerialiseOut(Writer& stream, const Map& m)
{
    CCharString defaultVal;      // CPersistTraits<CCharString>::DefaultVal
    stream.WriteSLONG((int)m.count);
    for (NodeBase* it = m.header->left; it != m.header; it = Increment(it)) {
        stream.WriteSLONG(((Node*)it)->key);
        stream.WriteNullTerminatedString(((Node*)it)->value);
    }
}
int main()
{
    // header <-> root(20) with children 10 and 30 (STLport layout: header->parent = root, left = min, right = max)
    NodeBase header; Node a(10, "cam_a"), b(20, "cam_b"), c(30, "cam_c");
    header.parent = &b; header.left = &a; header.right = &c;
    b.parent = &header; b.left = &a; b.right = &c;
    a.parent = &b; a.left = a.right = 0; c.parent = &b; c.left = c.right = 0;
    Map m = { &header, 3 };
    Writer w; w.len = 0; w.log[0] = 0;
    SerialiseOut(w, m);
    bool ok = std::strcmp(w.log, "3;10;cam_a;20;cam_b;30;cam_c;") == 0 && ctorCount == 1 && dtorCount == 1;
    if (!ok) { std::printf("00472841_TEST FAIL %s\n", w.log); return 1; }
    std::printf("00472841_TEST PASS\n");
    return 0;
}