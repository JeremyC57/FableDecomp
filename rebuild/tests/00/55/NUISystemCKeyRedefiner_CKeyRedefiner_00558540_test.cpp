#include <cstdio>
#include <cstdlib>
// Behaviour model of NUISystem::CKeyRedefiner::CKeyRedefiner (0x00558540): no action/key text, an empty self-linked action list,
// sub type 0 and every field of the input definition cleared.
struct Node { Node* next; Node* prev; long value; };
struct Definition { long action, controller, key, xbox, mouse; float dx, dy; };
struct KeyRedefiner { void* actionText; void* keyText; Node* actions; long subType; Definition def; };
static void Construct(KeyRedefiner& k)
{
    k.actionText = 0; k.keyText = 0;
    k.actions = 0;
    Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; k.actions = s;
    k.subType = 0;
    Definition d = { 0, 0, 0, 0, 0, 0.0f, 0.0f }; k.def = d;
}
int main()
{
    KeyRedefiner k; k.subType = 5; k.def.key = 7;
    Construct(k);
    bool ok = !k.actionText && !k.keyText && k.actions->next == k.actions && k.subType == 0 && k.def.key == 0 && k.def.dy == 0.0f;
    std::free(k.actions);
    if (!ok) { std::printf("00558540_TEST FAIL\n"); return 1; }
    std::printf("00558540_TEST PASS\n");
    return 0;
}