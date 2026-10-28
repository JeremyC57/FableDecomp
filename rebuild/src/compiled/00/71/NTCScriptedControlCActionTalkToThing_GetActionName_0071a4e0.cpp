// NTCScriptedControl::CActionTalkToThing::GetActionName() -- the scripted action's name for logging: "CActionTalkToThing"
// Leaf: ?GetActionName@CActionTalkToThing@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/71/CActionDoCreatureAction_GetActionName_0071a4e0.cpp: vtable slot 4 of NTCScriptedControl::CActionTalkToThing (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionTalkToThing"
// (was: NTCScriptedControl::CActionTalkToThing::GetActionName @ 0071a4e0)
// __fastcall, returns CCharString BY VALUE (hidden return pointer).
// Retail: push esi; esi=[esp+8]=hidden ret buf; push -1; push 0x12659e4;
//         mov ecx,esi; call CCharString(const char*,long); mov eax,esi; pop esi; ret 4.

class CCharString { public:
    char* p;
    CCharString(const char* s, long len);
};

namespace NTCScriptedControl {
class CActionTalkToThing {
    int dummy;
public:
    CCharString GetActionName();
};
}

CCharString NTCScriptedControl::CActionTalkToThing::GetActionName()
{
    return CCharString((const char*)0x12659e4, -1);
}
