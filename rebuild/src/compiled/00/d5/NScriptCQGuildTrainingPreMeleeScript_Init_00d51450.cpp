// NScript::CQ_GuildTrainingPreMeleeScript::Init() -- clears the 4 flag bytes at +0x49, +0x4a, +0x54, +0x55
// Leaf: ?Init@CQ_GuildTrainingPreMeleeScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D51450, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor al, al ; mov byte ptr [ecx + 0x49], al ; mov byte ptr [ecx + 0x4a], al ; mov byte ptr [ecx + 0x54], al ; mov byte ptr [ecx + 0x55], al ; ret 
// Tiny-body shape `ClearFlags` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
// Naming wave 35: identity outer script vtable 012cf788 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d51520 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CQ_GuildTrainingPreMeleeScript; PDB ?Init@CQ_GuildTrainingPreMeleeScript@NScript@@; refs=1; was _unattributed::ClearFlags_00d51450 (wave3/labels_alloc.tsv); PDB ?Init@CQ_GuildTrainingPreMeleeScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?ClearFlags_00d51450@CClass_00d51450@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CClass_00d51450 renamed CQ_GuildTrainingPreMeleeScript); same bytes.
namespace NScript {
class CQ_GuildTrainingPreMeleeScript {
public:
    unsigned char _pad0[0x45];   // (-4: now the vptr of the virtual member)
    unsigned char m_flag0;                 // +0x49
    unsigned char m_flag1;                 // +0x4a
    unsigned char _pad2[0x9];
    unsigned char m_flag2;                 // +0x54
    unsigned char m_flag3;                 // +0x55
public:
    virtual void Init();                          // PDB ?Init@CQ_GuildTrainingPreMeleeScript@NScript@@UAEXXZ
};
}
using NScript::CQ_GuildTrainingPreMeleeScript;

void NScript::CQ_GuildTrainingPreMeleeScript::Init()
{
    m_flag0 = 0;
    m_flag1 = 0;
    m_flag2 = 0;
    m_flag3 = 0;
}