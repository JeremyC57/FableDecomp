// CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase() -- the implicit destructor: PTargetThing (CIntelligentPointer<CThingCreatureBase> +0x14), then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_EnemyCreatureBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD769, 19 bytes. Identity (NF470 / NF471, T3 nf303 0x008FD288): its callers are the deleting destructors of vftable 0x0128B8BC (RTTI .?AVCAIStateGroup_EnemyCreatureBase@@, ??_G 0x008FF744) and of its Hover* / Whisper* / CreatureUseCoverBase subclasses, and the derived destructors 0x00902F71 / 0x00903383; the destroyed offsets
//   are Ego_r's (engine/CAIStateGroup_EnemyCreatureBase.h); both PDBs flag ~CAIStateGroup_EnemyCreatureBase `public virtual compiler-generated` (NF470 evidence/pdb_binder_dtor_attrs.txt).
//   Was @FableDestVal_008fd769@4 on a stand-in FableDestVal_008fd769_t (inner / outer).
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_EnemyCreatureBase out of line. 0x008FF744, the class's deleting
//   destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; ruling r470a).
#include "../src/compiled/00/8f/CAIStateGroupEnemyCreatureBase_scalardeletingdestructor_008ff744.cpp"
