// CFinalTabletSarcophagus::Main() -- the Necropolis sarcophagus holding the final Oracle tablet: unless tablet A is already taken, put up the magic-barrier forcefield, wait for three tablets, mark the sarcophagus, wait for the hero within 6 units of the barrier point, play the summoners cutscene once (re-raising the forcefield), wait for the final summoners to die, open the sarcophagus, give the hero expression A, take the tablet, have the hero comment twice and re-enable the Northern Wastes 2 creature generators
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFFB60, 1252 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing dtor 0x004AA840 (inline copies at the first forcefield), CBaseClass dtor 0x0099A2E0, operator delete 0x00BFE9BC,
//   IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CQ_AwakeningTheOracleScript::
//   PlayCutscene(CCharString, bool) 0x00CFF9A0 (label "Main_InitializeHeroMorphComponents" is wrong), CCharString 0x0099EBF0 /
//   0x0099EAE0; GameCalls slots 7, 70, 72, 99 CreateObjectOnEntity, 125, 225, 350, 352, 364, 366, 376, 447, 558, 565.
// Layout: Ego_r.pdb CFinalTabletSarcophagus (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_AwakeningTheOracleScript, retail = PDB - 0x14: TabletsCollected +0x4c, SpawnedFinalSpotsStuff +0x50,
//   FinalSummonersAlive +0x60, GotA +0x70), PMasterData +0x18.
// Shape: ~CScriptThing is a plain inline body (it expands for the first forcefield's discarded temporaries);
//   inline_depth(0) on the second forcefield statement leaves those temporaries and every later exit out of line.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing() { }                                 // out-of-line copy 0x004AA840
};
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
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
};
}
using NScript::CQ_AwakeningTheOracleScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CFinalTabletSarcophagus : CEntityScriptBase {
    CQ_AwakeningTheOracleScript* ParentClass;           // +0x14
    void* PMasterData;                                  // +0x18
    void Main();
};

void CFinalTabletSarcophagus::Main()
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
#pragma inline_depth(0)
        Game->CreateObjectOnEntity("OBJECT_SCRIPT_PALISADE_FORCEFIELD", Game->GetThingWithScriptName("ATO_MagicBarrierHere"), "ATO_MagicBarrier");
#pragma inline_depth()
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