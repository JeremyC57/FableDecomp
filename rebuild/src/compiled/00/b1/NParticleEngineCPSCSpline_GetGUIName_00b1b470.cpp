// NParticleEngine::CPSCSpline::GetGUIName() -- the component's editor name: "Spline"
// Leaf: ?GetGUIName@CPSCSpline@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/b1/CActionDoCreatureAction_GetActionName_00b1b470.cpp: vtable slot 6 of NParticleEngine::CPSCSpline; Ego_r.pdb lists GetGUIName / StaticGetGUIName on the particle component classes; literal "Spline"
// (was: NParticleEngine::CPSCSpline::GetGUIName -> CCharString by value)
// retail 0x00b1b470

class CCharString { public:
    char* m_data;
    CCharString(const char* s, long n);
};

// address 0x129f608 = a global string literal's address (push imm)
extern char g_actionName_str[];

namespace NParticleEngine {
class CPSCSpline { public:
    CCharString GetGUIName();
};
}

CCharString NParticleEngine::CPSCSpline::GetGUIName()
{
    return CCharString(g_actionName_str, -1);
}
