// CTCWallMount::CTCWallMount(CThing* owner) -- wall-mount component constructor: CTCBase(owner), bought for 0, no trophy pointer, trophy id 0
// Leaf: ??0CTCWallMount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DE4A0, length 64 bytes (ret 4). Vtable 0x0126FB7C = CTCWallMount
// (rtti-vtables.tsv); created by the factory @ 0x004D5BE3 (operator new(0x20)). The catalog label
// "CTCTavernBar::CTCTavernBar" is propagated over this constructor and rejected.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Vtable 0x01238C6C = CIntelligentPointer (the trophy pointer's own vtable).
// Ego_r.pdb CTCWallMount (0x20): BoughtForAmount +0x0c, Trophy (CIntelligentPointer<CThing>) +0x10, TrophyID (ulong64) +0x18.
// Shape: an initialiser list in declaration order: the immediate zero for BoughtForAmount after the vtable store, the
// trophy pointer's base constructor and vtable, then the two halves of the 64-bit id as immediates.
struct CThing;
struct CBaseObject;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    CTCBase(CThing* owner);                  // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +0x04
    CBaseIntelligentPointer();               // 0x00A01B10
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
    virtual void v00();
    CIntelligentPointer() { }
};

struct CTCWallMount : CTCBase {              // vtable 0x0126FB7C
    long                        BoughtForAmount;   // +0x0c
    CIntelligentPointer<CThing> Trophy;            // +0x10
    unsigned __int64            TrophyID;          // +0x18
    virtual void v00();
    CTCWallMount(CThing* owner);
};

CTCWallMount::CTCWallMount(CThing* owner)
    : CTCBase(owner)
    , BoughtForAmount(0)
    , Trophy()
    , TrophyID(0)
{
}