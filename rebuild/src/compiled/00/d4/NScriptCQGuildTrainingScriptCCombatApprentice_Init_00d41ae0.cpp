// NScript::CQ_GuildTrainingScript::CCombatApprentice::Init() -- sets the byte flag at +0x1c
// Leaf: ?Init@CCombatApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D41AE0, oracle length 5 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x1c],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x1c and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 8 bodies.
// Naming wave 35: identity reg 00d3c1b7 "CombatApprentice" in 00d3bc60; Alloc 00d41a30; vtable 012ce094 slot 2; PDB ?Init@CCombatApprentice@CQ_GuildTrainingScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::sub_00d41ae0 (wave3/labels_alloc.tsv); PDB ?Init@CCombatApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ in FableWin.pdb.
//   Was `?sub_00d41ae0@CClass_00d41ae0@@UAEXXZ`; this source now defines the PDB member (class CClass_00d41ae0 renamed CCombatApprentice); same bytes.
namespace NScript {
class CQ_GuildTrainingScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CCombatApprentice;
};
}
using NScript::CQ_GuildTrainingScript;
namespace NScript {
class CQ_GuildTrainingScript::CCombatApprentice {
public:
    virtual void v00();
    unsigned char _pad[0x18];
    bool m_flag;                           // +0x1c
private:
    virtual void Init();                          // PDB ?Init@CCombatApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ
};
}

void NScript::CQ_GuildTrainingScript::CCombatApprentice::Init()
{
    m_flag = true;
}