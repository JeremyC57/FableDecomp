// CAIStateGroup_RangedCombatUsePreferredFirePoint::~CAIStateGroup_RangedCombatUsePreferredFirePoint() -- the implicit destructor: PFirePoint (CIntelligentPointer<CThing> +0x1C), PEnemy (CIntelligentPointer<CThingCreatureBase> +0x14), then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_RangedCombatUsePreferredFirePoint@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900F43, 27 bytes. Identity (NF470 / NF471, T3 nf303 0x008FD288): its only caller is the deleting destructor 0x00900F27, slot 0 of vftable 0x0128C854 (RTTI .?AVCAIStateGroup_RangedCombatUsePreferredFirePoint@@); the destroyed offsets
//   are Ego_r's (engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h); both PDBs flag ~CAIStateGroup_RangedCombatUsePreferredFirePoint `public virtual compiler-generated` (NF470 evidence/pdb_binder_dtor_attrs.txt).
//   Was ?Dest_val@@YIXPAUPair@@@Z on a stand-in pair labelled _Dest_val<pair<CCharString,CSymbolMap::CSymbolInfo>>.
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_RangedCombatUsePreferredFirePoint out of line. 0x00900F27, the class's deleting
//   destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; ruling r470a).
#include "../src/compiled/00/90/CAIStateGroupRangedCombatUsePreferredFirePoint_scalardeletingdestructor_00900f27.cpp"
