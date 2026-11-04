// NScript::CQ_WaspBossScript::CWaspHelper::Init() -- clears the 3 flag bytes at +0x1c, +0x1d, +0x1e
// Leaf: ?Init@CWaspHelper@CQ_WaspBossScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0F870, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x1c], al ; mov byte ptr [ecx + 0x1d], al ; mov byte ptr [ecx + 0x1e], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
// Naming wave 35: identity reg 00e0edb7 "WaspHelper" in 00e0ea40; Alloc 00e0f7c0; vtable 012e0284 slot 2; PDB ?Init@CWaspHelper@CQ_WaspBossScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::ClearFlags_00e0f870 (wave3/labels_alloc.tsv); PDB ?Init@CWaspHelper@CQ_WaspBossScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?ClearFlags_00e0f870@CClass_00e0f870@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CClass_00e0f870 renamed CWaspHelper); same bytes.
namespace NScript {
class CQ_WaspBossScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CWaspHelper;
};
}
using NScript::CQ_WaspBossScript;
namespace NScript {
class CQ_WaspBossScript::CWaspHelper {
public:
    unsigned char _pad0[0x18];   // (-4: now the vptr of the virtual member)
    unsigned char m_flag0;                 // +0x1c
    unsigned char m_flag1;                 // +0x1d
    unsigned char m_flag2;                 // +0x1e
private:
    virtual void Init();                          // PDB ?Init@CWaspHelper@CQ_WaspBossScript@NScript@@EAEXXZ
};
}

void NScript::CQ_WaspBossScript::CWaspHelper::Init()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
}