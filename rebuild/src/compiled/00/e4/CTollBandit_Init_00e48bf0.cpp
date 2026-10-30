// CTollBandit::Init() -- reset the toll state, take the pay-me distance from the script def, find the bandit escape marker, then put the bandit in the friendly bandit faction and stop it wanting money
// Leaf: ?Init@CTollBandit@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E48BF0, length 271 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, ~CBaseClass @ 0x0099A2E0, operator
// delete @ 0x00BFE9BC, CGameScriptInterface slots 72 GetThingWithScriptName (+0x120), 601 EntitySetInFaction (+0x964) and
// 478 EntitySetThingAsWantingMoney (+0x778) (ghidra_out/cgsi_slots_aligned.tsv).
// Globals: pGlobalScriptDef @ 0x0143E90C (TollBanditPayMeDistance at +0x414); strings 0x012E4DA4 "M_BanditEscapePoint",
// 0x012D218C "FACTION_BANDITS_FRIENDLY".
// Layout: Ego_r.pdb CTollBandit (= retail): Game +0x04, Me +0x08 (the entity-script base, as the landed CM_TollBandit::Main
// 0x00E48A30), ParentClass +0x14, PMasterData +0x18, TollState +0x1c, TollBanditOnlyCollectsToll +0x20,
// KilledReluctantTrader +0x21, TollBanditPayMeDistance +0x24, BanditEscapeMarker +0x28 (CScriptThing).
// Shape: the marker is assigned from the GetThingWithScriptName temporary through the inline CScriptThing / CCountedPointer
// operator= (the source's fields read first, the old block released, the new one add-ref'd), then the temporary's
// inline destructor (vtable store, release, zeroing, ~CBaseClass) and the name's; both names are implicit conversions.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x414];
    long          TollBanditPayMeDistance;               // +0x414
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
extern const char g_strBanditEscapePoint[];              // 0x012E4DA4 "M_BanditEscapePoint"
extern const char g_strFactionBanditsFriendly[];         // 0x012D218C "FACTION_BANDITS_FRIENDLY"

class CV_BanditTollScript;
class CQ_SunnyvaleMasterData;
class CTollBandit {
public:
    void*                   __vftable;                   // +0x00
    CGameScriptInterface*   Game;                        // +0x04
    CScriptThing            Me;                          // +0x08
    CV_BanditTollScript*    ParentClass;                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
    long                    TollState;                   // +0x1c
    bool                    TollBanditOnlyCollectsToll;  // +0x20
    bool                    KilledReluctantTrader;       // +0x21
    long                    TollBanditPayMeDistance;     // +0x24
    CScriptThing            BanditEscapeMarker;          // +0x28
    void Init();
};

void CTollBandit::Init()
{
    TollState = 1;
    TollBanditOnlyCollectsToll = false;
    KilledReluctantTrader = false;
    TollBanditPayMeDistance = pGlobalScriptDef->TollBanditPayMeDistance;
    BanditEscapeMarker = Game->GetThingWithScriptName(g_strBanditEscapePoint);
    Game->EntitySetInFaction(Me, g_strFactionBanditsFriendly);
    Game->EntitySetThingAsWantingMoney(Me, 0);
}