// CAIStateGroup_BalverineCornered::~CAIStateGroup_BalverineCornered() -- the implicit destructor: PLastJumpTarget (CIntelligentPointer<CThingCreatureBase>
//   +0x1C), PTarget (CIntelligentPointer<CThingCreatureBase> +0x14), then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_BalverineCornered@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900533, 27 bytes. Identity (NF472, 0x008FD288 stand-in binder): its only caller is the deleting destructor of
//   vftable 0x0128C0BC (RTTI .?AVCAIStateGroup_BalverineCornered@@ : CAIStateGroupBase, ??_G 0x00900517, call at +3); the destroyed offsets are
//   Ego_r.pdb's PLastJumpTarget +0x1C / PTarget +0x14 (engine/CAIStateGroup_BalverineCornered.h retail view, 0x34-byte class; TimeOfLastJump +0x24 and
//   LandPos +0x28 have no destructor); both PDBs flag the destructor compiler-generated. The catalog name already said so. Was
//   ?_Dest_val_CSymbolInfo___@@YIXPAUSymPair@@PAX@Z on a stand-in pair (two local ~CCharString members bound to 0x00A01C10, the base as
//   ??1CSymbolInfoTail@@QAE@XZ) labelled _Dest_val<pair<CCharString,CSymbolInfo>>.
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_BalverineCornered out of line. 0x00900517, the
//   class's deleting destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; r470a).
#include "../src/compiled/00/90/CAIStateGroupBalverineCornered_scalardeletingdestructor_00900517.cpp"
