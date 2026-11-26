// CAIStateGroup_MinionReactToCriminal::~CAIStateGroup_MinionReactToCriminal() -- the implicit destructor: PHome (CIntelligentPointer<CThingBuilding> +0x1C),
//   PHero (CIntelligentPointer<CThingPlayerCreature> +0x14), then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_MinionReactToCriminal@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FECEC, 27 bytes. Identity (NF472, 0x008FD288 stand-in binder): its only caller is the deleting destructor of
//   vftable 0x0128B1F4 (RTTI .?AVCAIStateGroup_MinionReactToCriminal@@ : CAIStateGroupBase, ??_G 0x008FECD0, call at +3); the destroyed offsets are
//   Ego_r.pdb's PHome +0x1C / PHero +0x14 (engine/CAIStateGroup_MinionReactToCriminal.h retail view, 0x30-byte class); both PDBs flag the destructor
//   compiler-generated. The catalog name already said so. Was ?Dest_val@CSymbolInfo___@@QAEXXZ on a stand-in pair (two local ~CCharString members
//   bound to 0x00A01C10, the base as ?PairFirstDtor@CSymbolInfo___@@QAEXXZ) labelled _Dest_val<pair<CCharString,CSymbolInfo>>.
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_MinionReactToCriminal out of line. 0x008FECD0, the
//   class's deleting destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; r470a).
#include "../src/compiled/00/8f/CAIStateGroupMinionReactToCriminal_scalardeletingdestructor_008fecd0.cpp"
