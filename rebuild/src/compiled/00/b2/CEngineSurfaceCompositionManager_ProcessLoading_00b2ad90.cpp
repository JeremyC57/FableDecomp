// CEngineSurfaceCompositionManager::ProcessLoading(bool, double) -- forwards the loading tick to its two graphic data banks (+0x4fc, +0x808)
// Leaf: ProcessLoading
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AD90, length 57 bytes. Manifest label "CTCHero::GiveTutorialForCategory" is
// propagated; the name and signature are the Ego_r.pdb symbol ?ProcessLoading@CEngineSurfaceCompositionManager@@QAEX_NN@Z.
// One caller, CEngineResourceManager::ProcessLoading @ 0x00B3B7F0 (call site 0x00B3B863), on the surface-composition manager
// singleton @ 0x01436EAC (as in the landed CEngine::UpdateTextureMorphCache 0x00B239C0).
// Callees: the bank ProcessLoading(bool, double) @ 0x009FC410 twice (manifest "_Cons_val<..CAnimationEntry..>" is propagated).
// Shape: the double is re-copied for each call (`fld [esp+..] ; sub esp,8 ; fstp [esp]`, the first fld hoisted above the
// pushes), the bool argument is kept in edi as a whole dword across both calls, `ret 0Ch`.
// Local model: engine/CEngineSurfaceCompositionManager.h has the PDB (debug STL) layout, where the three CGraphicDataBank members
// sit at +0x94/+0x41c/+0x7a4 (0x388 each); the retail release layout puts the two banks this body touches at +0x4fc and +0x808
// (0x30c apart; which two of the three PDB banks they are is not established), so the header cannot be used and the banks
// are modelled at their retail offsets.
struct CGraphicDataBank_Calls {
    unsigned char _body[0x30c];
    void ProcessLoading(bool force, double time);           // 0x009FC410
};

struct CEngineSurfaceCompositionManager_Retail {
    unsigned char _pad000[0x4fc];
    CGraphicDataBank_Calls m_bankAt4FC;               // +0x4fc
    CGraphicDataBank_Calls m_bankAt808;           // +0x808
    void ProcessLoading(bool force, double time);
};

void CEngineSurfaceCompositionManager_Retail::ProcessLoading(bool force, double time)
{
    m_bankAt4FC.ProcessLoading(force, time);
    m_bankAt808.ProcessLoading(force, time);
}