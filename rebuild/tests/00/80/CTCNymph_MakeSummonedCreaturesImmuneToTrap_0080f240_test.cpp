// Behaviour test for 0x0080F240 CTCNymph::MakeSummonedCreaturesImmuneToTrap (model copy over a hand-built STLport list).
#include <cstdio>
class CThing { public: int id; int immuneCount; };
struct CIntelligentPointer { int vptr_slot; CThing* PData; CThing* GetPItem() const { return PData; } };
struct Node { Node* next; Node* prev; CIntelligentPointer data; };
struct List { Node* node; };
static List g_list;
static List& GetAllSummonedCreatures() { return g_list; }
class CTCTrapBase { public: int calls; CThing* last; void SetThingAsImmune(CThing& t) { ++calls; last = &t; ++t.immuneCount; } };
static void make_immune_model(CTCTrapBase* trap)
{
    List& creatures = GetAllSummonedCreatures();
    for (Node* it = creatures.node->next; it != creatures.node; it = it->next)
        trap->SetThingAsImmune(*it->data.GetPItem());
}
int main()
{
    Node sentinel; Node n[3]; CThing t[3];
    int i, ok = 1;
    sentinel.next = &sentinel; sentinel.prev = &sentinel; g_list.node = &sentinel;
    CTCTrapBase trap; trap.calls = 0; trap.last = 0;
    make_immune_model(&trap);                                // empty list
    if (trap.calls != 0) ok = 0;
    for (i = 0; i < 3; ++i) { t[i].id = i; t[i].immuneCount = 0; n[i].data.vptr_slot = 0; n[i].data.PData = &t[i]; }
    sentinel.next = &n[0]; n[0].next = &n[1]; n[1].next = &n[2]; n[2].next = &sentinel;
    sentinel.prev = &n[2]; n[2].prev = &n[1]; n[1].prev = &n[0]; n[0].prev = &sentinel;
    make_immune_model(&trap);
    if (trap.calls != 3 || trap.last != &t[2]) ok = 0;
    for (i = 0; i < 3; ++i) if (t[i].immuneCount != 1) ok = 0;
    if (!ok) { std::printf("0080f240_TEST FAIL\n"); return 1; }
    std::printf("0080f240_TEST PASS\n");
    return 0;
}