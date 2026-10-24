// Behaviour test for 0x00ADC4E0 list<CVertexGroupBlock>::erase(first, last): unlink + destroy each node, return last.
#include <cstdio>
#include <cstdlib>

static int g_destroyed = 0, g_nodesFreed = 0;
struct SBlock { int Id; void Destroy() { ++g_destroyed; } };
struct SNode { SNode* Next; SNode* Prev; SBlock Data; };
// reconstructed body
static SNode* EraseRange(SNode* first, SNode* last)
{
    while (first != last) {
        SNode* pos = first; first = first->Next;           // erase(first++)
        SNode* next = pos->Next; SNode* prev = pos->Prev;
        prev->Next = next; next->Prev = prev;
        pos->Data.Destroy(); ++g_nodesFreed; std::free(pos);
    }
    return last;
}

int main()
{
    SNode sentinel; sentinel.Next = sentinel.Prev = &sentinel;
    SNode* nodes[5];
    for (int i = 0; i < 5; ++i) {
        SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->Data.Id = i;
        n->Next = &sentinel; n->Prev = sentinel.Prev; sentinel.Prev->Next = n; sentinel.Prev = n; nodes[i] = n;
    }
    SNode* r = EraseRange(nodes[1], nodes[4]);              // erase ids 1..3
    if (r != nodes[4] || g_destroyed != 3 || g_nodesFreed != 3) { std::printf("00adc4e0_TEST FAIL\n"); return 1; }
    if (nodes[0]->Next != nodes[4] || nodes[4]->Prev != nodes[0] || sentinel.Next != nodes[0] || sentinel.Prev != nodes[4]) { std::printf("00adc4e0_TEST FAIL\n"); return 1; }
    if (EraseRange(nodes[4], nodes[4]) != nodes[4] || g_destroyed != 3) { std::printf("00adc4e0_TEST FAIL\n"); return 1; }
    r = EraseRange(sentinel.Next, &sentinel);               // erase everything
    if (r != &sentinel || g_destroyed != 5 || sentinel.Next != &sentinel || sentinel.Prev != &sentinel) { std::printf("00adc4e0_TEST FAIL\n"); return 1; }
    std::printf("00adc4e0_TEST PASS\n");
    return 0;
}