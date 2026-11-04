// NScript::CV_HiddenBootyScript::Init() -- clears the 4 flag bytes at +0x48, +0x49, +0x4a, +0x4b
// Leaf: ?Init@CV_HiddenBootyScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E929A0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x48], al ; mov byte ptr [ecx + 0x49], al ; mov byte ptr [ecx + 0x4a], al ; mov byte ptr [ecx + 0x4b], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
// Naming wave 35: identity outer script vtable 012ec9a4 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e92a50 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CV_HiddenBootyScript; PDB ?Init@CV_HiddenBootyScript@NScript@@; refs=1; was _unattributed::ClearFlags_00e929a0 (wave3/labels_alloc.tsv); PDB ?Init@CV_HiddenBootyScript@NScript@@UAEXXZ in FableWin.pdb.
//   Was `?ClearFlags_00e929a0@CClass_00e929a0@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CClass_00e929a0 renamed CV_HiddenBootyScript); same bytes.
namespace NScript {
class CV_HiddenBootyScript {
public:
    unsigned char _pad0[0x44];   // (-4: now the vptr of the virtual member)
    unsigned char m_flag0;                 // +0x48
    unsigned char m_flag1;                 // +0x49
    unsigned char m_flag2;                 // +0x4a
    unsigned char m_flag3;                 // +0x4b
public:
    virtual void Init();                          // PDB ?Init@CV_HiddenBootyScript@NScript@@UAEXXZ
};
}
using NScript::CV_HiddenBootyScript;

void NScript::CV_HiddenBootyScript::Init()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
    m_flag3 = 0;
}