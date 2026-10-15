// CPerItemData::CPerItemData(const CPerItemData& other) -- copy constructor: the augmentation vector copied out of line, then the six scalar members copied in layout order
// Leaf: ??0CPerItemData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C14C4, length 57 bytes.
// Callees: the retail std::vector<long> copy constructor @ 0x00514160 (labelled "Std_Vector_Construct_Copy").
// Layout: Ego_r.pdb CPerItemData (0x28 bytes with a 16-byte vector); the shipped vector is {first, last, end}, so every
// member after Augmentations sits 4 bytes lower: NumberOfKills +0x0c, TimeWielded +0x10, BestTrophyWitnessesAheadToDate
// +0x14, QuestPersistent +0x18, QuestPersistentRefCount +0x1c, BestFishWeight +0x20 (retail size 0x24). No engine header.
// Shape: a size TU (`#pragma optimize("s", on)`: every member copied through eax). `other` loaded into edi and pushed before `this` is parked in esi, the vector copy constructor called first,
// then one `mov eax,[edi+N] ; mov [esi+N],eax` pair per member (the bool through al; the float copied as a dword: the
// memberwise copy constructor of a POD-tailed record copies its float through an integer register), `mov eax,esi`; `ret 4`.
#pragma optimize("s", on)

struct CLongVector {                               // retail std::vector<long>: first / last / end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CLongVector(const CLongVector& other);         // 0x00514160
};

struct CPerItemData {
    CLongVector Augmentations;                     // +0x00
    long        NumberOfKills;                     // +0x0c
    long        TimeWielded;                       // +0x10
    long        BestTrophyWitnessesAheadToDate;    // +0x14
    bool        QuestPersistent;                   // +0x18
    long        QuestPersistentRefCount;           // +0x1c
    float       BestFishWeight;                    // +0x20
    CPerItemData(const CPerItemData& other);
};

CPerItemData::CPerItemData(const CPerItemData& other)
    : Augmentations(other.Augmentations)
    , NumberOfKills(other.NumberOfKills)
    , TimeWielded(other.TimeWielded)
    , BestTrophyWitnessesAheadToDate(other.BestTrophyWitnessesAheadToDate)
    , QuestPersistent(other.QuestPersistent)
    , QuestPersistentRefCount(other.QuestPersistentRefCount)
    , BestFishWeight(other.BestFishWeight)
{
}
