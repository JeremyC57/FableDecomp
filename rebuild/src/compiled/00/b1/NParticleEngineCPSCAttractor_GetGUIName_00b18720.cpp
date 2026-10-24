// NParticleEngine::CPSCAttractor::GetGUIName() -- the component's editor name: "Attractor"
// Leaf: ?GetGUIName@CPSCAttractor@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/b1/CActionDoCreatureAction_GetActionName_00b18720.cpp: vtable slot 6 of NParticleEngine::CPSCAttractor; Ego_r.pdb lists GetGUIName / StaticGetGUIName on the particle component classes; literal "Attractor"
// (was: Faithful VC7.1 reconstruction of)
//   NParticleEngine::CPSCAttractor::GetGUIName  (0x00b18720)
//
// Retail disasm:
//   push esi
//   mov  esi, [esp+8]        ; esi = hidden return buffer (CCharString*)
//   push -1
//   push 0x129f5dc           ; const char* literal
//   mov  ecx, esi            ; this = return buffer
//   call CCharString::CCharString(const char*, int)  ; 0x99ebf0
//   mov  eax, esi
//   pop  esi
//   ret  4
//
// A GetGUIName() const returning a CCharString by value, NRVO'd into the
// caller-supplied return buffer, initialised from a string literal with an
// explicit length arg of -1.

class CCharString
{
public:
    // Declaration only: defined in another TU (retail 0x99ebf0), so the
    // compiler MUST emit a real call, matching retail's push/push/call.
    CCharString(const char* s, int len);
private:
    char* m_data;
};

namespace NParticleEngine {
class CPSCAttractor
{
public:
    CCharString GetGUIName() const;
};
}

CCharString NParticleEngine::CPSCAttractor::GetGUIName() const
{
    return CCharString("ACTION_DO_CREATURE_ACTION", -1);
}
