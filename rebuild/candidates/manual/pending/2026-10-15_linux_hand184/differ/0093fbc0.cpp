// Grade: DIFFER(175v173) -- one register: retail pushes the creature straight from ecx before mov ecx,eax; ours copies it to edx first (mov edx,ecx ; push edx), as in the Horn twin 0x00940820 | tried: plain, const parameter
// CAIStateGroup_VillageTaskSecuritySweep::IsRunnable() -- clear the task thing, then (when the villager has a
// village and the creature has interface 0x34) take the village's security-sweep point for this creature;
// runnable when one was found.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093FBC0, length 173 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor
// @ 0x00A01C10 (two converting temporaries, CIntelligentPointer<CThing> vtable 0x0128B098), GFGetPVillageInterface
// @ 0x007EE540 (__fastcall on the villager pointer +0x14; the manifest's IS_THING_ALIVE label is propagated),
// CTCVillage::GetSecuritySweepThing(CThing*) @ 0x006590F4; the creature's interface-0x34 bit (bit 20 of +0x24)
// tested inline.
// Shape: `PTaskThing = 0;` and `PTaskThing = horn;` are the converting temporary (inline ctor + vtable store +
// SetPItem) and the inline operator= (`SetPItem(other.GetPItem())`); the pointer's address is formed once (edi);
// the pointer-to-bool return is `neg ; sbb ; neg`; PBrain is re-read into esi for the HasTC test and the argument.
// Same model as the landed siblings 0x0093EDF0 / 0x0093FAD0.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0128B098
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CTCVillage {
    CThing* GetSecuritySweepThing(CThing* creature);   // 0x006590F4
};

struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_VillageTaskSecuritySweep : CAIStateGroupBase {
    CTCVillager* PVillager;                     // +0x14 the creature's villager component
    CIntelligentPointerThing PTaskThing;        // +0x18 CIntelligentPointer<CThing>
    virtual bool IsRunnable();
};

bool CAIStateGroup_VillageTaskSecuritySweep::IsRunnable()
{
    PTaskThing = 0;
    CTCVillage* village = GFGetPVillageInterface(PVillager);
    if (village != 0 && PBrain->PCreature->HasTC(0x34))
        PTaskThing = village->GetSecuritySweepThing(PBrain->PCreature);
    return PTaskThing.GetPItem() != 0;
}
