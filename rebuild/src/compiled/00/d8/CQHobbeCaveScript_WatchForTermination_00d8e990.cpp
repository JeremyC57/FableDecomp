// CQ_HobbeCaveScript::WatchForTermination() -- switch the cave creature generators off until the mission ends, switch them back on, fail the quest with the granny/grandson reason or complete it, take the hexagon door key from the hero, wait for Granny's House to unload and deactivate the quest
// Leaf: WatchForTermination
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8E990, 1200 bytes (int3 padded).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0 / ~ 0x0099B510,
//   IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7, 11 IsLevelLoaded, 70 GetHero, 125 TakeObjectFromHero,
//   184 IsObjectInThingsPossession, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 289 SetQuestAsFailed,
//   558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName.
// Layout: Game +0x40, MissionSucceeded +0x4B, MissionFailed +0x4C, FailReason +0x70 (as CQ_HobbeCaveScript::Init 0x00D8D640).
// Shape: each failure reason is a whole SetQuestAsFailed call in its own arm (VC cross-jumps them into one tail).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CScriptThing { public:                                   // 12 bytes {vptr, PImp.Data, PImp.Info}
    void* VTable;
    void* PImpData;
    void* PImpInfo;
    ~CScriptThing();                                    // 0x004AA840
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"

struct __declspec(novtable) CEntityScriptBindingBase {
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQ_HobbeCaveScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[3];
    bool                  MissionSucceeded;                // +0x4B
    bool                  MissionFailed;             // +0x4C
    unsigned char         _pad4d[0x70 - 0x4d];
    long                  FailReason;                   // +0x70
    void WatchForTermination();
};

void CQ_HobbeCaveScript::WatchForTermination()
{
    Game->SetCreatureGeneratorsEnabled("HobbeCaveSideRoom", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveNymphChamber", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveMainChamber", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveEntranceTunnel", false);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveLarder", false);
    Game->SetCreatureGeneratorsEnabled("Greatwood", false);
    Game->SetCreatureGeneratorsEnabled("GreatwoodCaves", false);
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCreatureGeneratorsEnabled("HobbeCaveSideRoom", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveNymphChamber", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveMainChamber", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveEntranceTunnel", true);
    Game->SetCreatureGeneratorsEnabled("HobbeCaveLarder", true);
    Game->SetCreatureGeneratorsEnabled("Greatwood", true);
    Game->SetCreatureGeneratorsEnabled("GreatwoodCaves", true);
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        if (FailReason == 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, (const unsigned short*)L"TEXT_QST_036_QUEST_FAILED_GRANNY_DIED", true);
        } else if (FailReason == 1) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, (const unsigned short*)L"TEXT_QST_036_QUEST_FAILED_GRANDSON_DIED", true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, (const unsigned short*)L"", true);
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    }
    bool hasKey = Game->IsObjectInThingsPossession("OBJECT_HEXAGON_DOOR_KEY", Game->GetHero());
    if (hasKey) {
        if (IsActiveThreadTerminating())
            return;
        Game->TakeObjectFromHero("OBJECT_HEXAGON_DOOR_KEY");
    }
    while (Game->IsLevelLoaded("GrannysHouse") != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}