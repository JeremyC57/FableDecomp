// NParticleEngine::CPSCRenderMesh::GetGUIName() -- the component's editor name: "Mesh Renderer"
// Leaf: ?GetGUIName@CPSCRenderMesh@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/b0/CActionDoCreatureAction_GetActionName_00b05a90.cpp: vtable slot 6 of NParticleEngine::CPSCRenderMesh; Ego_r.pdb lists GetGUIName / StaticGetGUIName on the particle component classes; literal "Mesh Renderer"
// (was: NParticleEngine::CPSCRenderMesh::GetGUIName (returns CCharString by value))
// retail 0x00b05a90: build CCharString from literal @0x129f534 with len -1.

struct CCharString {
    char* m_data;
    CCharString(const char* s, int len);
};

// ctor @0x99ebf0
__declspec(noinline) CCharString::CCharString(const char* s, int len) {
    m_data = (char*)s;
    if (len) m_data = (char*)s + 1;
}

namespace NParticleEngine {
struct CPSCRenderMesh {
    CCharString GetGUIName();
};
}

CCharString NParticleEngine::CPSCRenderMesh::GetGUIName() {
    return CCharString((const char*)0x129f534, -1);
}
