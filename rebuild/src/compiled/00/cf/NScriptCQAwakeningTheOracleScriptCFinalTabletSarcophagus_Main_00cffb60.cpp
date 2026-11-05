// NScript::CQ_AwakeningTheOracleScript::CFinalTabletSarcophagus::Main() -- the Necropolis sarcophagus holding the final Oracle tablet: unless tablet A is already taken, put up the magic-barrier forcefield, wait for three tablets, mark the sarcophagus, wait for the hero within 6 units of the barrier point, play the summoners cutscene once (re-raising the forcefield), wait for the final summoners to die, open the sarcophagus, give the hero expression A, take the tablet, have the hero comment twice and re-enable the Northern Wastes 2 creature generators
// Leaf: ?Main@CFinalTabletSarcophagus@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFFB60, 1252 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing dtor 0x004AA840 (inline copies at the first forcefield), CBaseClass dtor 0x0099A2E0, operator delete 0x00BFE9BC,
//   IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CQ_AwakeningTheOracleScript::
//   PlayCutscene(CCharString, bool) 0x00CFF9A0 (label "Main_InitializeHeroMorphComponents" is wrong), CCharString 0x0099EBF0 /
//   0x0099EAE0; GameCalls slots 7, 70, 72, 99 CreateObjectOnEntity, 125, 225, 350, 352, 364, 366, 376, 447, 558, 565.
// Layout: Ego_r.pdb CFinalTabletSarcophagus (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_AwakeningTheOracleScript, retail = PDB - 0x14: TabletsCollected +0x4c, SpawnedFinalSpotsStuff +0x50,
//   FinalSummonersAlive +0x60, GotA +0x70), PMasterData +0x18.
// Shape: ~CScriptThing is a plain inline body (it expands for the first forcefield's discarded temporaries);
//   by VC7.1's per-site frequency rule (docs/pipeline/INLINE_DEPTH_REVISIT.md) the second forcefield statement's temporaries and every later exit stay out of line (plain inline).
// Naming wave 35: identity reg 00cfb12f "FinalTabletSarcophagus" in 00cfb080; Alloc 00cfbc00; vtable 012c7b88 slot 1; PDB ?Main@CFinalTabletSarcophagus@CQ_AwakeningTheOracleScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CFinalTabletSarcophagus::Main (wave3/labels_alloc.tsv); PDB ?Main@CFinalTabletSarcophagus@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CFinalTabletSarcophagus@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/C3DVector.h"
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing() { }                                 // out-of-line copy 0x004AA840
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CQ_AwakeningTheOracleScript { public:
    char pad00[0x4c];
    long TabletsCollected;                              // +0x4c (PDB +0x60)
    bool SpawnedFinalSpotsStuff;                        // +0x50 (PDB +0x64)
    char pad51[0x60 - 0x51];
    long FinalSummonersAlive;                           // +0x60 (PDB +0x74)
    char pad64[0x70 - 0x64];
    bool GotA;                                          // +0x70 (PDB +0x84)
    void PlayCutscene(CCharString name, bool wait);     // 0x00CFF9A0
public:
    class CFinalTabletSarcophagus;   // the entity class below (naming wave 35)
};
}
using NScript::CQ_AwakeningTheOracleScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
namespace NScript {
class CQ_AwakeningTheOracleScript::CFinalTabletSarcophagus : public CEntityScriptBase {
public:
    CQ_AwakeningTheOracleScript* ParentClass;           // +0x14
    void* PMasterData;                                  // +0x18
private:
    virtual void Main();                          // PDB ?Main@CFinalTabletSarcophagus@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
};
}

void NScript::CQ_AwakeningTheOracleScript::CFinalTabletSarcophagus::Main()
{
    Game->SetThingAsUsable(Me, false);
    if (ParentClass->GotA) {
        if (IsActiveThreadTerminating())
            return;
        return;
    }
    Game->CreateObjectOnEntity("OBJECT_SCRIPT_PALISADE_FORCEFIELD", Game->GetThingWithScriptName("ATO_MagicBarrierHere"), "ATO_MagicBarrier");
    while (ParentClass->TabletsCollected <= 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    CScriptThing barrierPoint = Game->GetThingWithScriptName("ATO_MagicBarrierHere");
    while (!IsDistanceBetweenThingsUnder(barrierPoint, Game->GetHero(), 6.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->SpawnedFinalSpotsStuff) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->SpawnedFinalSpotsStuff = true;
        ParentClass->PlayCutscene("CS_ORACLE_NECROPOLIS_SUMMONERS_APPEAR", true);
        Game->CreateObjectOnEntity("OBJECT_SCRIPT_PALISADE_FORCEFIELD", Game->GetThingWithScriptName("ATO_MagicBarrierHere"), "ATO_MagicBarrier");
    }
    Game->SetTeleportingAsActive(false);
    while (ParentClass->FinalSummonersAlive != 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    ParentClass->PlayCutscene("CS_ORACLE_NECROPOLIS_OPEN_SARC", true);
    Game->SetTeleportingAsActive(true);
    ++ParentClass->TabletsCollected;
    Game->GiveHeroExpression("EXPRESSION_A", -1, false);
    ParentClass->GotA = true;
    Game->TakeObjectFromHero("OBJECT_SS_EXPRESSION_TABLET_A");
    Game->MiniMapRemoveMarker(Me);
    long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_B03_SCYTHE_RETURN_TO_SNOWSPIRE_10", false, Game->GetHero(), Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_B03_SCYTHE_RETURN_TO_SNOWSPIRE_20", false, Game->GetHero(), Game->GetHero());
    Game->SetCreatureGeneratorsEnabled("NorthernWastes2", true);
    do
        Game->NewScriptFrame();
    while (!IsActiveThreadTerminating());
}