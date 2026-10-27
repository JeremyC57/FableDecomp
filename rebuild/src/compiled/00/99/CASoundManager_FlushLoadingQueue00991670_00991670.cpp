// CASoundManager::FlushLoadingQueue_00991670() -- run the sound bank's loading queue to completion (no time limit) (descriptive name)
// Leaf: ?FlushLoadingQueue_00991670@CASoundManager@@UAEXXZ
// Retail: Fable.exe @ 0x00991670, 23 bytes, thiscall plain `ret`.
// Identity: vtable slot 3 (+0x0C) of CASoundManager 0x0129A574 (read from Fable.exe); CSoundManagerLH's slot 3 (landed
// 0x00A3EB20) calls this body first. Neither PDB name set has any CASoundManager member, so the name is descriptive.
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

class CASoundManager {
public:
    virtual ~CASoundManager();                              // slot 0
    virtual void _slot1();
    virtual void _slot2();
    virtual void FlushLoadingQueue_00991670();              // slot 3 (+0x0C)

    unsigned char _pad04[0x24];
    CResourceBank* PSoundBank;                              // +0x28 (descriptive)
};

void CASoundManager::FlushLoadingQueue_00991670()
{
    PSoundBank->ProcessLoadingQueue(false, 0.0);
}