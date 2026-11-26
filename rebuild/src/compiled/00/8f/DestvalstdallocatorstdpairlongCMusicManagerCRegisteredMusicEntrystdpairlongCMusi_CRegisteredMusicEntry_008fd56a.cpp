// CAIStateGroup_MinionSitOnSeatBase::~CAIStateGroup_MinionSitOnSeatBase() -- the implicit destructor: POwner (CIntelligentPointer<CThing> +0x34), then
//   the base ~CAIStateGroup_UsesSeat (tail jump 0x008FD544); no vtable store.
// Leaf: ??1CAIStateGroup_MinionSitOnSeatBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD56A, 19 bytes. Identity (NF471 NAME_FIXES, NF472): FableWin.pdb ??1CAIStateGroup_MinionSitOnSeatBase@@UAE@XZ,
//   both PDBs flag it compiler-generated; catalog CAIStateGroup_MinionSitOnSeatBase::~CAIStateGroup_MinionSitOnSeatBase. Its callers are the deleting
//   destructors (call at +3) of vftables 0x0128B584 (RTTI .?AVCAIStateGroup_MinionSitOnSeatBase@@ : CAIStateGroup_UsesSeat, ??_G 0x008FF271) and of its
//   five leaves 0x0128B5D4 / 0x0128B624 / 0x0128B674 / 0x0128B6C4 / 0x0128CFBC / 0x0128D00C (MinionSitAtWork / AtInn / AtSchool / AtHome / SitOnSeat /
//   SitOutside, ??_G 0x008FF29F / 0x008FF2D0 / 0x008FF2FE / 0x008FF36F / 0x00902140 / 0x009021DD: their own implicit destructors inlined), plus a row-less
//   5-byte jmp at 0x008FD57D. The destroyed offset is Ego_r.pdb's POwner +0x34 (engine/CAIStateGroup_MinionSitOnSeatBase.h retail view, 0x3C-byte
//   class). Was the extern "C" free stand-in @_Dest_val_pair_long_CRegisteredMusicEntry@4.
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_MinionSitOnSeatBase out of line. 0x008FF271, the
//   class's deleting destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424; r470a).
#include "../src/compiled/00/8f/CAIStateGroupMinionSitOnSeatBase_scalardeletingdestructor_008ff271.cpp"
