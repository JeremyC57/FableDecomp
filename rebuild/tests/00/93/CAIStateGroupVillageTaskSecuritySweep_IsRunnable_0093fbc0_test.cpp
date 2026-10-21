// Behaviour test for 0093fbc0: src/0093fbc0.cpp copied verbatim, callees stubbed so calls and arguments are observable.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
// Same model as the landed siblings 0x0093EDF0 / 0x0093FAD0 and the Horn twin 0x00940820: the inline operator= names
// the item (`CThing* item = other.GetPItem(); SetPItem(item);`) and the creature is read through the inline
// GetCreature() (PBrain->PCreature) accessor; with both, the pointer's lea follows the GetPItem call and the creature
// argument is pushed straight from ecx as retail.
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
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
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
    CThing* GetCreature() const { return PBrain->PCreature; }
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
    if (village != 0 && GetCreature()->HasTC(0x34))
        PTaskThing = village->GetSecuritySweepThing(GetCreature());
    return PTaskThing.GetPItem() != 0;
}

// ---- stubs and driver ----
static int g_sets = 0;
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
CAIStateGroupBase::~CAIStateGroupBase() { }
static CTCVillage g_village; static CTCVillage* g_villageRet = 0; static CTCVillager* g_villagerArg = 0;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager) { g_villagerArg = villager; return g_villageRet; }
static CThing* g_sweepRet = 0; static CThing* g_sweepArg = 0; static int g_sweepCalls = 0;
CThing* CTCVillage::GetSecuritySweepThing(CThing* creature) { ++g_sweepCalls; g_sweepArg = creature; return g_sweepRet; }
int main()
{
    CThing creature; for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    CThing sweep;
    CAIBrain brain; brain.PCreature = &creature;
    static double buf[16]; CAIStateGroup_VillageTaskSecuritySweep& g = *(CAIStateGroup_VillageTaskSecuritySweep*)buf; g.PBrain = &brain; g.PVillager = (CTCVillager*)0x1234;
    g.PTaskThing.m_pItem = &sweep;
    // no village: cleared, not runnable
    g_villageRet = 0; g_sweepRet = &sweep;
    CHECK(!g.CAIStateGroup_VillageTaskSecuritySweep::IsRunnable()); CHECK(g.PTaskThing.m_pItem == 0); CHECK(g_villagerArg == (CTCVillager*)0x1234); CHECK(g_sweepCalls == 0);
    // village but creature lacks interface 0x34
    g_villageRet = &g_village;
    CHECK(!g.CAIStateGroup_VillageTaskSecuritySweep::IsRunnable()); CHECK(g_sweepCalls == 0);
    // interface 0x34 present (word 1, bit 20)
    creature.m_presentBits[1] = 1UL << 20;
    CHECK(g.CAIStateGroup_VillageTaskSecuritySweep::IsRunnable()); CHECK(g_sweepCalls == 1); CHECK(g_sweepArg == &creature); CHECK(g.PTaskThing.m_pItem == &sweep);
    // the village has no sweep point
    g_sweepRet = 0;
    CHECK(!g.CAIStateGroup_VillageTaskSecuritySweep::IsRunnable()); CHECK(g_sweepCalls == 2); CHECK(g.PTaskThing.m_pItem == 0);
    CHECK(g_sets > 0);
    if (fails == 0) printf("0093fbc0_TEST PASS\n"); else printf("0093fbc0_TEST FAIL\n");
    return fails;
}