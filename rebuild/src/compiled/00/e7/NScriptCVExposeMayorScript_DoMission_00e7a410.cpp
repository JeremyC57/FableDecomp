// NScript::CV_ExposeMayorScript::DoMission() -- thread: run the Expose Mayor quest stage by stage: from stage 0 wait for BowerstonePosh and run PhaseOne, then PhaseTwo .. PhaseFive (a frame and a termination test after each), wait for stage 5 and finish with EndMission
// Leaf: ?DoMission@CV_ExposeMayorScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7A410, 355 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, at 0x00E75E28 inside the quest's Main: the CSpawnedFunc member-pointer
//   store (`mov [edi+34h], 0E7A410h`) right after the spawned thread's name "DoMission" 0x012CB73C is built for the
//   CSpawnedFuncBase ctor 0x00CDD450; Ego_r.pdb ?DoMission@CV_ExposeMayorScript@NScript@@QAEXXZ (0x00ED9760, right before
//   ?Main@CV_ExposeMayorScript@NScript@@UAEXXZ). Every callee is a landed CV_ExposeMayorScript member. Catalog label sub_00e7a410.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CV_ExposeMayorScript::PhaseOne 0x00E7A580, PhaseTwo 0x00E7A5C0,
//   PhaseThree 0x00E7A9F0, PhaseFour 0x00E7AB50, PhaseFive 0x00E7A740, EndMission 0x00E7B050 (Ego_r ?<Phase>@CV_ExposeMayorScript@NScript@@QAEXXZ);
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; Game (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded.
// Literal: "BowerstonePosh" 0x012C6C6C.
// Layout (retail = Ego_r.pdb - 0x14, the landed ctor 0x00E755F0 / Init 0x00E75680): Path +0x48, StageNumber +0x4c (STAGE_IN_PATH).
// Shape: `if (StageNumber == 0) { if (terminating) return; WAIT(IsLevelLoaded); PhaseOne(); ...frame... }`, then each later
//   phase followed by a frame and a termination test, a WAIT for stage 5 and its trailing test before EndMission.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ExposeMayorScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum PATH_ID { };                                   // Ego_r.pdb nested enums (enumerator names not in the PDB)
    enum STAGE_IN_PATH { };
    PATH_ID       Path;                                 // +0x48
    STAGE_IN_PATH StageNumber;                          // +0x4c
    void PhaseOne();                                    // 0x00E7A580
    void PhaseTwo();                                    // 0x00E7A5C0
    void PhaseFive();                                   // 0x00E7A740
    void PhaseThree();                                  // 0x00E7A9F0
    void PhaseFour();                                   // 0x00E7AB50
    void EndMission();                                  // 0x00E7B050
    void DoMission();
};

void CV_ExposeMayorScript::DoMission()
{
    if (StageNumber == 0) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("BowerstonePosh")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PhaseOne();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    PhaseTwo();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    PhaseThree();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    PhaseFour();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    PhaseFive();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (StageNumber != 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    EndMission();
}
}   // namespace NScript