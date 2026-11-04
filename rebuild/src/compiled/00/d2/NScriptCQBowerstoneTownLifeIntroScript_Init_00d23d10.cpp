// NScript::CQ_BowerstoneTownLifeIntroScript::Init() -- clears the 3 flag bytes at +0x48, +0x49, +0x4a
// Leaf: ?Init@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D23D10, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x48], al ; mov byte ptr [ecx + 0x49], al ; mov byte ptr [ecx + 0x4a], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
// Naming wave 35: identity outer script vtable 012cb52c slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d23dc0 (slot 2) registers 2 entity scripts whose PDB Alloc outer is CQ_BowerstoneTownLifeIntroScript; PDB ?Init@CQ_BowerstoneTownLifeIntroScript@NScript@@; refs=1; was _unattributed::ClearFlags_00d23d10 (wave3/labels_alloc.tsv); PDB ?Init@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?ClearFlags_00d23d10@CClass_00d23d10@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CClass_00d23d10 renamed CQ_BowerstoneTownLifeIntroScript); same bytes.
namespace NScript {
class CQ_BowerstoneTownLifeIntroScript {
public:
    unsigned char _pad0[0x44];   // (-4: now the vptr of the virtual member)
    unsigned char m_flag0;                 // +0x48
    unsigned char m_flag1;                 // +0x49
    unsigned char m_flag2;                 // +0x4a
public:
    virtual void Init();                          // PDB ?Init@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXXZ
};
}
using NScript::CQ_BowerstoneTownLifeIntroScript;

void NScript::CQ_BowerstoneTownLifeIntroScript::Init()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
}