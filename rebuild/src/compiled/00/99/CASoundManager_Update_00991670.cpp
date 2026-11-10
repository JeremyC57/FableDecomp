// CASoundManager::Update() -- per-frame sound update of the base: run the sound bank's loading queue to completion (no time limit)
// Leaf: ?Update@CASoundManager@@UAEXXZ
// Retail: Fable.exe @ 0x00991670, 23 bytes, thiscall plain `ret`.
// Identity: vtable slot 3 (+0x0C) of CASoundManager 0x0129A574 (read from Fable.exe); CSoundManagerLH's slot 3 (landed
// 0x00A3EB20) calls this body first. Name (agentSND 2026-11-10): Fable Anniversary 360 PDB ?Update@CASoundManager@@UAAXXZ
// (lib_sound_manager_base.cpp, 20 bytes) and ?Update@CSoundManagerUN@@; the Anniversary PC vtables keep Update at the matching
// slot (CASoundManager 0x02E5895C slot 4 = the TLC slot 3 after one inserted slot; its UN override calls the base first, as
// CSoundManagerLH 0x00A3EB20 does); CWorld::RenderUpdate 0x0049E080 calls it once a frame. Was FlushLoadingQueue_00991670.
// The row is not a catch block (gap-scan label "Catch@0040dc8c"): it uses no frame and returns with `ret`.
// Label correction: catalog "_global::Catch@0040dc8c"; reloc_bindings has the stand-in ?Prepare@CListOwner_00a3eb20@@QAEXXZ
// (landed 0x00A3EB20's model).
// Callees: 0x009FC410 = CResourceBank::ProcessLoadingQueue(bool, double) (Ego_r.pdb ?ProcessLoadingQueue@CResourceBank@@QAEX_NN@Z:
// 0x009FC410 forwards both arguments to CResourceList::ProcessLoadingQueue 0x009FC250 on this+0x40, and Ego_r.pdb
// CResourceBank holds ResourceList +4 and LoadingQueue +0x40; the catalog/manifest label "_Cons_val<...>" and the
// bindings ?ProcessLoading@CResourceList_009f8230 / CGraphicDataBank_Calls / ?Process@CLoadingQueue_00a23b00 are
// stand-ins).
// Layout: the bank pointer at +0x28 (descriptive member name); no PDB layout for CASoundManager.
// Shape: the double 0.0 comes from the shared 8-byte pool constant (fld qword / fstp into the argument slot), `false`
// pushed after it.
class CResourceBank {
public:
    void ProcessLoadingQueue(bool limitTime, double endTime);   // 0x009FC410
};

class CASoundBank;
class CWideString;
class CASoundManager {
public:
    virtual ~CASoundManager();                              // slot 0
protected:
    virtual CASoundBank* CreateSoundBank(const CWideString& fileName);   // slot 1 (engine/CASoundManager.h)
    virtual void SetEnableAtmos(bool enable);                            // slot 2
public:
    virtual void Update();                                  // slot 3 (+0x0C)

    unsigned char _pad04[0x24];
    CResourceBank* PSoundBank;                              // +0x28 (descriptive)
};

void CASoundManager::Update()
{
    PSoundBank->ProcessLoadingQueue(false, 0.0);
}