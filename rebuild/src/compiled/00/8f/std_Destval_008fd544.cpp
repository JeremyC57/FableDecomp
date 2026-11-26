// CAIStateGroup_UsesSeat::~CAIStateGroup_UsesSeat() -- the implicit destructor: PSeat (CIntelligentPointer<CThingObject> +0x14), then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_UsesSeat@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD544, 19 bytes. Identity (NF470 / NF471, T3 nf303 0x008FD288): its callers are the deleting destructors of vftables 0x0128B53C (RTTI .?AVCAIStateGroup_UsesSeat@@, ??_G 0x008FF22B), 0x0128BD34 (SitForMeal, ??_G 0x008FFF98) and 0x0128BF04 (SitInPub, ??_G 0x0090021B), whose own implicit destructors are inlined there, and ~CAIStateGroup_MinionSitOnSeatBase 0x008FD56A (tail jump); the destroyed offsets
//   are Ego_r's (engine/CAIStateGroup_UsesSeat.h); both PDBs flag ~CAIStateGroup_UsesSeat `public virtual compiler-generated` (NF470 evidence/pdb_binder_dtor_attrs.txt).
//   Was @FableDestVal_008fd544@4 on a stand-in FableDestVal_008fd544_t (inner / outer).
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_UsesSeat out of line. 0x008FF22B, the class's deleting
//   destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; ruling r470a).
#include "../src/compiled/00/8f/CAIStateGroupUsesSeat_scalardeletingdestructor_008ff22b.cpp"
