// Behaviour test for 00940820: src/00940820.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAIStateGroup_VillageTaskHorn::IsRunnable()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_VillageTaskHorn::IsRunnable() -- clear the task thing, then take the village's horn for this
// creature (when the villager has a village); runnable when a horn was found.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00940820, length 164 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor
// @ 0x00A01C10 (two converting temporaries, CIntelligentPointer<CThing> vtable 0x0128B0A8), GFGetPVillageInterface
// @ 0x007EE540 (__fastcall on the villager pointer +0x14; the manifest's IS_THING_ALIVE label is propagated),
// CTCVillage::GetHornForCreature(CThing*) @ 0x006590D4 (manifest "_Umove<EWeaponClass*>" is propagated).
// Shape: `PTaskThing = 0;` and `PTaskThing = horn;` are the converting temporary (inline ctor + vtable store +
// SetPItem) and the inline operator= (`SetPItem(other.GetPItem())`); the pointer's address is formed once (edi);
// the pointer-to-bool return is `neg ; sbb ; neg`. Same model as the landed sibling 0x0093EDF0.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0128B0A8
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CTCVillage {
    CThing* GetHornForCreature(CThing* creature);   // 0x006590D4
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

struct CAIStateGroup_VillageTaskHorn : CAIStateGroupBase {
    CTCVillager* PVillager;                     // +0x14 the creature's villager component
    CIntelligentPointerThing PTaskThing;        // +0x18 CIntelligentPointer<CThing>
    virtual bool IsRunnable();
};

bool CAIStateGroup_VillageTaskHorn::IsRunnable()
{
    PTaskThing = 0;
    CTCVillage* village = GFGetPVillageInterface(PVillager);
    if (village != 0) {
        PTaskThing = village->GetHornForCreature(GetCreature());
    }
    return PTaskThing.GetPItem() != 0;
}

// ---- stubs and driver ----
static int g_ctors = 0, g_dtors = 0, g_sets = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; ++g_sets; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
static CTCVillage g_village; static bool g_haveVillage = true; static CTCVillager* g_villagerSeen;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager) { g_villagerSeen = villager; return g_haveVillage ? &g_village : 0; }
static CThing* g_horn; static CThing* g_askedFor;
CThing* CTCVillage::GetHornForCreature(CThing* creature) { g_askedFor = creature; return g_horn; }
CAIStateGroupBase::~CAIStateGroupBase() {}
int main()
{
    static unsigned char gs[sizeof(CAIStateGroup_VillageTaskHorn)]; memset(gs, 0, sizeof gs);
    CAIStateGroup_VillageTaskHorn* g = (CAIStateGroup_VillageTaskHorn*)gs;
    static CAIBrain brain; CThing* creature = (CThing*)0x4444; brain.PCreature = creature; g->PBrain = &brain;
    g->PVillager = (CTCVillager*)0x5555; g->PTaskThing.m_pItem = (CThing*)0x9999;
    g_horn = (CThing*)0x7777;
    CHECK(g->CAIStateGroup_VillageTaskHorn::IsRunnable() == true); CHECK(g->PTaskThing.m_pItem == g_horn); CHECK(g_askedFor == creature); CHECK(g_villagerSeen == (CTCVillager*)0x5555);
    CHECK(g_ctors == g_dtors);
    g_horn = 0; CHECK(g->CAIStateGroup_VillageTaskHorn::IsRunnable() == false); CHECK(g->PTaskThing.m_pItem == 0);
    g_horn = (CThing*)0x7777; g_haveVillage = false; CHECK(g->CAIStateGroup_VillageTaskHorn::IsRunnable() == false); CHECK(g->PTaskThing.m_pItem == 0);
    if (fails == 0) printf("00940820_TEST PASS\n"); else printf("00940820_TEST FAIL\n");
    return fails;
}