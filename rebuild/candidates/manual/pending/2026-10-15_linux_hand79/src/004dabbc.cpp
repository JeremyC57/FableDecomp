// CTCActionUseBed::CTCActionUseBed(CThing*) -- CTCActionUseBase(owner), own vtable, idle bed state, usable by the hero, two empty thing pointers
// Leaf: ??0CTCActionUseBed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DABBC, length 75 bytes (a /Os module: unaligned entry,
// `push dword ptr [esp+0Ch]` pass-through, `or dword ptr [..],-1`).
// Callees: CTCActionUseBase::CTCActionUseBase(CThing*) @ 0x004DA939, CIntelligentPointer<CThing>::
// CIntelligentPointer(CThing*) @ 0x004A96F0 (x2, with a null thing). Own vtable 0x0123F784 =
// CTCActionUseBed (rtti-vtables.tsv); the destructor is 0x007EBDD0.
//
// Layout = Ego_r.pdb CTCActionUseBed (0x34 bytes, byte-packed): the 0x14-byte base, WaitingForFadeOut
// +0x14, WaitingForFadeIn +0x15, InAtomicAction +0x16, WaitingForConfirmation +0x17,
// PlayerNumberOfThingUsingBed +0x18, Cost +0x1c, UseableByHero +0x20, OwnedByHero +0x21,
// ForceTrespassOnceFadedOut +0x22, and the two CIntelligentPointer<CThing> members PBuilding +0x23 and
// PThingUser +0x2b at UNALIGNED offsets (pack(1)).
// Shape: base constructor; the -1 player number is scheduled before the vtable store, the zero bytes
// and words share ebx (also the null argument pushed for both pointer constructors).
#pragma optimize("s", on)

struct CThing;

struct CTCActionUseBase {                                   // 0x14 bytes, vtable 0x0123F09C
    virtual void v00();
    unsigned char _body[0x10];                              // CTCBase + PActionUseDef + Usable
    CTCActionUseBase(CThing* owner);                        // 0x004DA939
};

#pragma pack(push, 1)
struct CIntelligentPointer {                                // CIntelligentPointer<CThing>: vtable + item
    virtual void v00();
    void* m_pItem;
    CIntelligentPointer(CThing* thing);                     // 0x004A96F0
};

struct CTCActionUseBed : CTCActionUseBase {                 // vtable 0x0123F784
    bool                WaitingForFadeOut;                  // +0x14
    bool                WaitingForFadeIn;                   // +0x15
    bool                InAtomicAction;                     // +0x16
    bool                WaitingForConfirmation;             // +0x17
    long                PlayerNumberOfThingUsingBed;        // +0x18
    long                Cost;                               // +0x1c
    bool                UseableByHero;                      // +0x20
    bool                OwnedByHero;                        // +0x21
    bool                ForceTrespassOnceFadedOut;          // +0x22
    CIntelligentPointer PBuilding;                          // +0x23
    CIntelligentPointer PThingUser;                         // +0x2b
    virtual void v00();
    CTCActionUseBed(CThing* owner);
};
#pragma pack(pop)

CTCActionUseBed::CTCActionUseBed(CThing* owner)
    : CTCActionUseBase(owner)
    , WaitingForFadeOut(false)
    , WaitingForFadeIn(false)
    , InAtomicAction(false)
    , WaitingForConfirmation(false)
    , PlayerNumberOfThingUsingBed(-1)
    , Cost(0)
    , UseableByHero(true)
    , OwnedByHero(false)
    , ForceTrespassOnceFadedOut(false)
    , PBuilding(0)
    , PThingUser(0)
{
}
