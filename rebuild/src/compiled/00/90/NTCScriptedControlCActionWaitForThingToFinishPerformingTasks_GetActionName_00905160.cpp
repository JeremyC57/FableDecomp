// NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::GetActionName() -- the scripted action's name for logging: "CActionWaitForThingToFinishPerformingTasks"
// Leaf: ?GetActionName@CActionWaitForThingToFinishPerformingTasks@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_00905160.cpp: vtable slot 4 of NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionWaitForThingToFinishPerformingTasks"
// (was: NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::GetActionName  @ 0x00905160)
// Struct-return __fastcall member: returns CCharString("DoCreatureAction", -1) by value.
// The 'this' pointer is unused by the body.

class CCharString { public:
    char* m_pData;
    CCharString(const char* s, long n);
};

namespace NTCScriptedControl {
struct CActionWaitForThingToFinishPerformingTasks {
    int dummy;
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}
