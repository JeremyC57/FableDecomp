// NParticleEngine::CPSCSpline::GetGUIName() -- the component's editor name: "Spline"
// Leaf: ?GetGUIName@CPSCSpline@NParticleEngine@@
// Literal (NF418, 2026-11-25): was extern data `g_actionName_str` (table ?g_actionName_str@@3PADA -> 0x0129F608); now spelled at the use site:
//   "Spline", retail's pooled literal at 0x0129F608 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/b1/CActionDoCreatureAction_GetActionName_00b1b470.cpp: vtable slot 6 of NParticleEngine::CPSCSpline; Ego_r.pdb lists GetGUIName / StaticGetGUIName on the particle component classes; literal "Spline"
// (was: NParticleEngine::CPSCSpline::GetGUIName -> CCharString by value)
// retail 0x00b1b470

class CCharString { public:
    char* m_data;
    CCharString(const char* s, long n);
};


namespace NParticleEngine {
class CPSCSpline { public:
    CCharString GetGUIName();
};
}

CCharString NParticleEngine::CPSCSpline::GetGUIName()
{
    return CCharString("Spline", -1);
}
