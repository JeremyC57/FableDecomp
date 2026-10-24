// Behaviour test for 0x00ADC250 _List_base<CVertexGroupBlock>::clear (destroy each block, free each node, relink sentinel).
#include <cstdio>
#include <cstdlib>

static int g_freed = 0, g_blocksDestroyed = 0;
static void Free(void* p) { if (p) { ++g_freed; std::free(p); } }
struct SBlock {
    unsigned long* Start; unsigned long* Finish; unsigned long* End; void* MapHeader; unsigned int MapCount; int Pad;
    void Destroy() { ++g_blocksDestroyed; Free(MapHeader); if (Start) Free(Start); }   // map (empty here) then vector
};
struct SListNode { SListNode* Next; SListNode* Prev; SBlock Data; };
struct SList {
    SListNode* Node;
    // reconstructed body
    void clear()
    {
        SListNode* cur = Node->Next;
        while (cur != Node) { SListNode* tmp = cur; cur = cur->Next; tmp->Data.Destroy(); Free(tmp); }
        Node->Next = Node; Node->Prev = Node;
    }
};

int main()
{
    SListNode* sentinel = (SListNode*)std::malloc(sizeof(SListNode));
    SList l; l.Node = sentinel; sentinel->Next = sentinel->Prev = sentinel;
    for (int i = 0; i < 3; ++i) {
        SListNode* n = (SListNode*)std::malloc(sizeof(SListNode));
        n->Data.Start = i == 1 ? 0 : (unsigned long*)std::malloc(8); n->Data.MapHeader = std::malloc(0x14); n->Data.MapCount = 0;
        n->Next = sentinel; n->Prev = sentinel->Prev; sentinel->Prev->Next = n; sentinel->Prev = n;
    }
    l.clear();
    if (g_blocksDestroyed != 3 || g_freed != 3 + 3 + 2 || sentinel->Next != sentinel || sentinel->Prev != sentinel) { std::printf("00adc250_TEST FAIL\n"); return 1; }
    l.clear();
    if (g_blocksDestroyed != 3 || g_freed != 8) { std::printf("00adc250_TEST FAIL\n"); return 1; }
    std::free(sentinel);
    std::printf("00adc250_TEST PASS\n");
    return 0;
}