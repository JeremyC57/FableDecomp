// CTCCreatureGenerator::HasHitCreationLimit() const -- true when the generator's live creatures reach its active-creature limit, when the world-wide generated-creature count reaches the global limit, or when it has created its total generation limit (each limit -1 = none)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074A190, length 75 bytes (+ int3 padding).
// Callees: none. Globals: the global generated-creature limit long @ 0x013824A8 (-1 = none) and the
// current count long @ 0x013BAD54.
// Layout: engine/CTCCreatureGenerator.h (Ego_r.pdb) is 0x10/0x1c bytes high here (retail CArray 0xc,
// STLport list 4, vectors 12), so the model is local: CreatedCreatures +0x18 (STLport list: the sentinel
// node pointer), GenParams.ActiveCreatureLimit +0x3c, GenParams.TotalGenerationLimit +0x40, CreationCount +0x84.
// Shape: three sequential `if (limit != -1 && count >= limit) return true;` tests sharing the `mov al,1`
// tail; the list size is the STLport distance loop (`++first` before `++n`) compared unsigned (`jae`)
// against the long limit; the global count is compared from memory against the limit in eax (`jge`).
struct SCreatureNode {                                      // STLport _List_node<CIntelligentPointer<CThing> >
    SCreatureNode* Next;
    SCreatureNode* Prev;
};

struct CRetailList_Creature {                               // STLport list: the sentinel pointer only
    SCreatureNode* Node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SCreatureNode* first = Node->Next; first != Node; ) {
            first = first->Next;
            ++n;
        }
        return n;
    }
};

extern long g_GeneratedCreatureLimit;                       // 0x013824A8
extern long g_GeneratedCreatureCount;                       // 0x013BAD54

struct CTCCreatureGenerator {
    void*                __vftable;                         // +0x00
    unsigned char        _pad04[0x14];                      // +0x04 CTCBase, CreatureFamilies
    CRetailList_Creature CreatedCreatures;                  // +0x18
    unsigned char        _pad1c[0x20];                      // +0x1c CreatureTriggerers, GenParams head
    long                 ActiveCreatureLimit;               // +0x3c GenParams.ActiveCreatureLimit
    long                 TotalGenerationLimit;              // +0x40 GenParams.TotalGenerationLimit
    unsigned char        _pad44[0x40];                      // +0x44
    long                 CreationCount;                     // +0x84
    bool HasHitCreationLimit() const;
};

bool CTCCreatureGenerator::HasHitCreationLimit() const
{
    if (ActiveCreatureLimit != -1 && CreatedCreatures.size() >= (unsigned long)ActiveCreatureLimit)
        return true;
    if (g_GeneratedCreatureLimit != -1 && g_GeneratedCreatureCount >= g_GeneratedCreatureLimit)
        return true;
    if (TotalGenerationLimit != -1 && CreationCount >= TotalGenerationLimit)
        return true;
    return false;
}