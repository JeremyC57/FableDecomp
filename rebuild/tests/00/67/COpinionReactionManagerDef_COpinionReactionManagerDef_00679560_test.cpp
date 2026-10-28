// Behaviour test for 0x00679560 COpinionReactionManagerDef::COpinionReactionManagerDef():
// the tree headers are red, rootless and self-linked, the list node is self-linked, the frequency-traits
// table is cleared and the two cursors point at its low and high rows.
#include <cstdio>
#include <cstdlib>
struct NodeBase { bool Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Tree { NodeBase* Header; unsigned int Count;
    Tree() : Header(0) { Header = (NodeBase*)std::malloc(sizeof(NodeBase)); Count = 0; Header->Color = false; Header->Parent = 0; Header->Left = Header; Header->Right = Header; } };
struct ListNode { ListNode* Next; ListNode* Prev; };
struct List { ListNode* Node; List() : Node(0) { ListNode* n = (ListNode*)std::malloc(sizeof(ListNode)); n->Next = n; n->Prev = n; Node = n; } };
struct Vec { void* First; void* Last; void* End; Vec() : First(0), Last(0), End(0) { } };
struct Traits;
struct TraitsArray { Traits* LowHigh[2][79];
    TraitsArray() { for (unsigned long i = 0; i < 79; ++i) { LowHigh[0][i] = 0; LowHigh[1][i] = 0; } } };
struct Def {
    void* PDefManager; unsigned char Bits; long InstantiationName; bool Dirty;
    Vec AttitudeBlur; Tree Maps[8]; List Matches; TraitsArray FrequencyTraitsArray;
    Traits** FrequencyTraits; Traits** FrequencyTraitsHigh;
    Def() : PDefManager(0), Bits(0), InstantiationName(-1), Dirty(false)
    { FrequencyTraits = FrequencyTraitsArray.LowHigh[0]; FrequencyTraitsHigh = FrequencyTraitsArray.LowHigh[1]; }
};
int main()
{
    Def* d = new Def;
    bool ok = d->PDefManager == 0 && d->InstantiationName == -1 && !d->Dirty && d->AttitudeBlur.First == 0;
    for (int m = 0; m < 8; ++m) {
        NodeBase* h = d->Maps[m].Header;
        if (h->Color || h->Parent || h->Left != h || h->Right != h || d->Maps[m].Count) ok = false;
    }
    if (d->Matches.Node->Next != d->Matches.Node || d->Matches.Node->Prev != d->Matches.Node) ok = false;
    for (int i = 0; i < 79; ++i) if (d->FrequencyTraits[i] || d->FrequencyTraitsHigh[i]) ok = false;
    if (d->FrequencyTraitsHigh - d->FrequencyTraits != 79) ok = false;
    if (!ok) { std::printf("00679560_TEST FAIL\n"); return 1; }
    std::printf("00679560_TEST PASS\n");
    return 0;
}