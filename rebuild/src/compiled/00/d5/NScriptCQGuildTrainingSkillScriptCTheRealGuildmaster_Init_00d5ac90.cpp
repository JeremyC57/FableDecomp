// NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Init() -- sets the byte flag at +0x1c
// Leaf: ?Init@CTheRealGuildmaster@CQ_GuildTrainingSkillScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D5AC90, oracle length 5 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x1c],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x1c and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 8 bodies.
// Naming wave 35: identity reg 00d5ab87 "TheRealGuildmaster" in 00d5ab40; Alloc 00d5abe0; vtable 012d09d8 slot 2; PDB ?Init@CTheRealGuildmaster@CQ_GuildTrainingSkillScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::sub_00d5ac90 (wave3/labels_alloc.tsv); PDB ?Init@CTheRealGuildmaster@CQ_GuildTrainingSkillScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?sub_00d5ac90@CClass_00d5ac90@@UAEXXZ`; this source now defines the PDB member (class CClass_00d5ac90 renamed CTheRealGuildmaster); same bytes.
namespace NScript {
class CQ_GuildTrainingSkillScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CTheRealGuildmaster;
};
}
using NScript::CQ_GuildTrainingSkillScript;
namespace NScript {
class CQ_GuildTrainingSkillScript::CTheRealGuildmaster {
public:
    virtual void v00();
    unsigned char _pad[0x18];
    bool m_flag;                           // +0x1c
private:
    virtual void Init();                          // PDB ?Init@CTheRealGuildmaster@CQ_GuildTrainingSkillScript@NScript@@EAEXXZ
};
}

void NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Init()
{
    m_flag = true;
}