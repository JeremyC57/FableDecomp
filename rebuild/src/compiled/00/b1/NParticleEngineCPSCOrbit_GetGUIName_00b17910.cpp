// NParticleEngine::CPSCOrbit::GetGUIName() -- the component's editor name: "Orbit"
// Leaf: ?GetGUIName@CPSCOrbit@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/b1/CActionDoCreatureAction_GetActionName_00b17910.cpp: vtable slot 6 of NParticleEngine::CPSCOrbit; Ego_r.pdb lists GetGUIName / StaticGetGUIName on the particle component classes; literal "Orbit"
// (was: NParticleEngine::CPSCOrbit::GetGUIName)
// Retail 0x00b17910 - __fastcall, returns CCharString by value from a string literal.
// 'this' is unused; body constructs CCharString(literal, -1) into the hidden return buffer.

class CCharString {
public:
    // (const char*, int length = -1) constructor; out-of-line (matches call 0x99ebf0)
    CCharString(const char* s, int len);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_data;
};

namespace NParticleEngine {
class CPSCOrbit {
public:
    CCharString GetGUIName() const;
};
}

CCharString NParticleEngine::CPSCOrbit::GetGUIName() const {
    return CCharString("DoCreatureAction", -1);
}
