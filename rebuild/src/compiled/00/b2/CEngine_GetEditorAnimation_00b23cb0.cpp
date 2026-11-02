// CEngine::GetEditorAnimation(const CWideString& name) const -- set the named animation up in the resource manager's editor mesh bank and return its animation info
// Leaf: ?GetEditorAnimation@CEngine@@UBEPBVC3DAnimationInfo@@ABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23CB0, length 34 bytes, thiscall, tail jump (`ret 4` in the callee).
// Identity: the only reference is slot 35 of vtable 0x012A0F3C (RTTI CEngine : CIEngine, CDeviceResetCallback@12). Ego_r
//   ?GetEditorAnimation@CEngine@@UBEPBVC3DAnimationInfo@@ABVCWideString@@@Z (public const virtual, one CWideString reference).
// Callees: CEngineResourceManager::SetupEditorAnimation(const CWideString&) @ 0x00B3BC90 (Ego_r + FableWin ?SetupEditorAnimation@
//   CEngineResourceManager@@QAEKABVCWideString@@@Z; the body works the wide name against the mesh bank at +0x14 and follows
//   ~CEngineResourceManager 0x00B3B8E0; the catalog label "CEngine::GetEditorAnimation" on it is this row's name, propagated),
//   CMeshDataBank::PeekAnimation(unsigned long) const @ 0x00A242C0 (Ego_r + FableWin ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z;
//   tail jump with the index stored over the argument slot).
// Global: the engine's resource manager pointer 0x01436E98 under its landed spelling g_pResourceManager (no PDB data symbol;
//   NAME_FIXES_QUEUE c600 / c672 keep it open for the coordinator).
// Layout: Ego_r CEngineResourceManager MeshBanks[3] (CMeshDataBank*) at +0x10, so +0x14 is MeshBanks[1] (no containers before it).
// Shape: the global is re-read after the call (the callee may change it); the mesh bank is a named const local: with the bank
//   expression inline as the object of the call, VC7.1 stores the index over the argument slot BEFORE the two ecx loads.
class CWideString;
class C3DAnimationInfo;

class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};

class CEngineResourceManager { public:
    void*          __vftable;                                // +0x00 (CEngineComponent base)
    unsigned char  _pad04[0x10 - 0x04];
    CMeshDataBank* MeshBanks[3];                             // +0x10
    CMeshDataBank* MainMeshBank;                             // +0x1c
    unsigned long SetupEditorAnimation(const CWideString& name);   // 0x00B3BC90
};
extern CEngineResourceManager* g_pResourceManager;         // 0x01436E98

class CEngine { public:                                      // vtable 0x012A0F3C
    virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString& name) const;
};

const C3DAnimationInfo* CEngine::GetEditorAnimation(const CWideString& name) const
{
    unsigned long index = g_pResourceManager->SetupEditorAnimation(name);
    const CMeshDataBank* bank = g_pResourceManager->MeshBanks[1];
    return bank->PeekAnimation(index);
}