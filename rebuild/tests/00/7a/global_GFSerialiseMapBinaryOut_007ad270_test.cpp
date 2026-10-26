// Behaviour test for 0x007AD270 GFSerialiseMapBinaryOut<bool, CCharString>: count, then key/value pairs in order.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* p; };
struct Stream {
    char buf[256]; int len;
    void WriteSLONG(long v) { int x = (int)v; std::memcpy(buf + len, &x, 4); len += 4; }
    void WriteBool(bool v) { buf[len++] = v ? 1 : 0; }
    void WriteNullTerminatedString(const CCharString& s) { int n = (int)std::strlen(s.p) + 1; std::memcpy(buf + len, s.p, n); len += n; }
};
struct Node { Node* parent; Node* left; Node* right; CCharString key; bool value; };
static Node* Increment(Node* node)
{
    if (node->right != 0) { node = node->right; while (node->left != 0) node = node->left; }
    else { Node* y = node->parent; while (node == y->right) { node = y; y = y->parent; } if (node->right != y) node = y; }
    return node;
}
struct Map { Node* header; unsigned int count; };
static void SerialiseOut(Stream& stream, const Map& m)
{
    stream.WriteSLONG(m.count);
    for (Node* it = m.header->left; it != m.header; it = Increment(it)) {
        stream.WriteNullTerminatedString(it->key);
        stream.WriteBool(it->value);
    }
}

int main()
{
    Node H, A, B, C;
    H.parent = &B; H.left = &A; H.right = &C;
    B.parent = &H; B.left = &A; B.right = &C; B.key.p = "b"; B.value = false;
    A.parent = &B; A.left = 0; A.right = 0; A.key.p = "a"; A.value = true;
    C.parent = &B; C.left = 0; C.right = 0; C.key.p = "c"; C.value = true;
    Map m; m.header = &H; m.count = 3;
    Stream s; s.len = 0;
    SerialiseOut(s, m);
    const char expect[] = { 3, 0, 0, 0, 'a', 0, 1, 'b', 0, 0, 'c', 0, 1 };
    if (s.len != 13 || std::memcmp(s.buf, expect, 13) != 0) { std::printf("007ad270_TEST FAIL\n"); return 1; }
    Map e; Node E; E.left = &E; E.right = &E; E.parent = 0; e.header = &E; e.count = 0;
    s.len = 0; SerialiseOut(s, e);
    if (s.len != 4) { std::printf("007ad270_TEST FAIL\n"); return 1; }
    std::printf("007ad270_TEST PASS\n");
    return 0;
}