// CAIStateGroup_JackDragon::~CAIStateGroup_JackDragon() -- the implicit destructor: the seven CIntelligentPointer members PInvisibleBodgeCreature +0x44 down to PEnemy +0x14, then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_JackDragon@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009006DD, 67 bytes. Identity (NF470 / NF471, T3 nf303 0x008FD288): its only caller is the deleting destructor 0x009006C1, slot 0 of vftable 0x0128C170 (RTTI .?AVCAIStateGroup_JackDragon@@); the destroyed offsets
//   are Ego_r's (engine/CAIStateGroup_JackDragon.h); both PDBs flag ~CAIStateGroup_JackDragon `public virtual compiler-generated` (NF470 evidence/pdb_binder_dtor_attrs.txt).
//   Was ??1CGlobalDef_009006dd@@QAE@XZ on a stand-in CGlobalDef_009006dd : CDefBase_008fd288.
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_JackDragon out of line. 0x009006C1, the class's deleting
//   destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; ruling r470a).
#include "../src/compiled/00/90/CAIStateGroupJackDragon_scalardeletingdestructor_009006c1.cpp"
