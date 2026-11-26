// CAIStateGroup_WanderAroundShops::~CAIStateGroup_WanderAroundShops() -- the implicit destructor: PrettyThing (CIntelligentPointer<CThing> +0x1C), Shop (+0x14), then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_WanderAroundShops@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090010E, 27 bytes. Identity (NF470 / NF471, T3 nf303 0x008FD288): its only caller is the deleting destructor 0x009000F2, slot 0 of vftable 0x0128BE4C (RTTI .?AVCAIStateGroup_WanderAroundShops@@ : CAIStateGroup_WorkBase<CAIStateGroupBase>); the destroyed offsets
//   are Ego_r's (engine/CAIStateGroup_WanderAroundShops.h); both PDBs flag ~CAIStateGroup_WanderAroundShops `public virtual compiler-generated` (NF470 evidence/pdb_binder_dtor_attrs.txt).
//   Was ?Dest_val_SymPair@@YIXPAUSymPair@@@Z on a stand-in pair labelled _Dest_val<pair<CCharString,CSymbolMap::CSymbolInfo>>.
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_WanderAroundShops out of line. 0x009000F2, the class's deleting
//   destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; ruling r470a).
#include "../src/compiled/00/90/CAIStateGroupWanderAroundShops_scalardeletingdestructor_009000f2.cpp"
