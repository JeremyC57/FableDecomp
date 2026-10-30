// NScript::CQ_GuildTrainingWoodsDepartureScript::WatchForTermination() -- wait until the woods departure mission has failed or succeeded; re-enable experience spending; then fail or complete the active quest, and deactivate Q_GuildTrainingWoodsDeparture
// Leaf: ?WatchForTermination@CQ_GuildTrainingWoodsDepartureScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D62140, length 280 bytes.
// Name: the row is referenced once, at 0x00D61F5A inside the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20,
//   which spawns it as the "WatchForTermination" thread; Ego_r.pdb has
//   ?WatchForTermination@CQ_GuildTrainingWoodsDepartureScript@NScript@@QAEXXZ. (Catalog label
//   Quest_GuildTrainingWoods_Departure_Init is invented.)
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CWideString(const unsigned short*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, and through the script
//   interface (Game, +0x40): NewScriptFrame (slot 7), DeactivateQuestLater (slot 281), SetQuestAsCompleted (slot 288),
//   SetQuestAsFailed (slot 289), SetExperienceSpendingAsEnabled (slot 572), GetActiveQuestName (slot 655, by value).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a,
//   DepartureMissionPoint +0x4c.
// Shape: the CQ_GuildTrainingWoodsWillScript::WatchForTermination 0x00D68600 form (same chunk) with
//   SetExperienceSpendingAsEnabled(true) between the post-wait thread test and the outcome test; the deactivation names
//   the quest ("Q_GuildTrainingWoodsDeparture", 0x012CDFD8) through an implicit CCharString temporary.
// Model: local CScriptBase / CLevelScriptBase / CGameScriptInterfaceBase models (the engine headers are flattened, no
//   vtables); quest members are the PDB names at the retail offsets.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);            // 0x0099EBF0
    ~CCharString();                                             // 0x0099EAE0
};
class CWideString { public:                                     // 4 bytes
    void* PStringData;
    CWideString(const unsigned short* text);                    // 0x0099B6B0
    ~CWideString();                                             // 0x0099B510
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptDataBase;
class CPersistContext;
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual ~CScriptBase();                                     // slot 0
protected:
    virtual void RegisterMain();                                // slot 1
    virtual void Main();                                        // slot 2
    virtual void Init();                                        // slot 3
public:
    virtual void OnPersist(CPersistContext& context);           // slot 4
    unsigned char _body[0x3c];                                  // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                           // 0x00CB7940
};
class CLevelScriptBase : public CScriptBase { public:           // PDB CLevelScriptBase<TData> (TData enters no symbol here)
    CGameScriptInterfaceBase* Game;                             // +0x40
    CScriptDataBase*          MasterScriptData;                 // +0x44
};

namespace NScript {
class CQ_GuildTrainingWoodsDepartureScript : public CLevelScriptBase {
public:
    bool MissionSucceeded;                                      // +0x48
    bool MissionFailed;                                         // +0x49
    bool MissionOver;                                           // +0x4a
    long DepartureMissionPoint;                                 // +0x4c
    void WatchForTermination();
};
} // namespace NScript

void NScript::CQ_GuildTrainingWoodsDepartureScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetExperienceSpendingAsEnabled(true);
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, true, false);
    }
    Game->DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0);
}