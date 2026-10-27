// Behaviour test for 00527680 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <cstdlib>
struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Tree { NodeBase* Header; unsigned int Count;
    void EmptyInitialize() { Header->Color = false; Header->Parent = 0; Header->Left = Header; Header->Right = Header; } };
int main() {
    Tree t; t.Header = (NodeBase*)std::malloc(sizeof(NodeBase)); t.Count = 0;
    t.Header->Color = true; t.Header->Parent = (NodeBase*)&t;
    t.EmptyInitialize();
    if (t.Header->Color || t.Header->Parent || t.Header->Left != t.Header || t.Header->Right != t.Header) { std::printf("00527680_TEST FAIL\n"); return 1; }
    std::free(t.Header);
    { std::printf("00527680_TEST PASS\n"); return 0; }
}