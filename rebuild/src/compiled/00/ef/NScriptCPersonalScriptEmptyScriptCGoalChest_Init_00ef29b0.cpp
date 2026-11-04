// NScript::CPersonalScript_EmptyScript::CGoalChest::Init() -- clears the byte flag at +0x48 of the object at +0x14
// Leaf: ?Init@CGoalChest@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF29B0, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+0x14] ; mov byte ptr [eax+0x48],0 ; ret
// Setter through one pointer member: loads the object at +0x14 and writes 0 into its byte at
// +0x48. Class and member names are descriptive (vtable-slot label for the class when there is
// one). One of 8 bodies of this shape.
// Naming wave 35: identity reg 00ef07c2 "GoalChest" in 00ef0090; Alloc 00ef2900; vtable 012f6254 slot 2; PDB ?Init@CGoalChest@CPersonalScript_EmptyScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::ClearFlag_00ef29b0 (wave3/labels_alloc.tsv); PDB ?Init@CGoalChest@CPersonalScript_EmptyScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?ClearFlag_00ef29b0@CClass_00ef29b0@@UAEXXZ`; this source now defines the PDB member (class CClass_00ef29b0 renamed CGoalChest); same bytes.
struct CTarget {
    unsigned char _pad[0x48];
    bool m_flag;                           // +0x48
};

namespace NScript {
class CPersonalScript_EmptyScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CGoalChest;
};
}
using NScript::CPersonalScript_EmptyScript;
namespace NScript {
class CPersonalScript_EmptyScript::CGoalChest {
public:
    virtual void v00();
    unsigned char _pad[0x10];
    CTarget* m_target;                     // +0x14
private:
    virtual void Init();                          // PDB ?Init@CGoalChest@CPersonalScript_EmptyScript@NScript@@EAEXXZ
};
}

void NScript::CPersonalScript_EmptyScript::CGoalChest::Init()
{
    m_target->m_flag = false;
}