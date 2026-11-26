// CAIStateGroup_GoToBed::~CAIStateGroup_GoToBed() -- the implicit destructor: BedSide (CCharString +0x30), PBed (CIntelligentPointer<CThing> +0x14),
//   then the base ~CAIStateGroupBase (tail jump 0x008FD288); no vtable store.
// Leaf: ??1CAIStateGroup_GoToBed@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD2CA, 27 bytes. Identity (NF470, T3 nf303 0x008FD288): its callers are the deleting destructors of
//   vftables 0x0128B104 (RTTI .?AVCAIStateGroup_GoToBed@@, ??_G 0x008FEB97) and 0x0128B140 (.?AVCAIStateGroup_MinionSleep@@ : GoToBed, ??_G 0x008FEBC5,
//   ~MinionSleep inlined); the destroyed offsets are Ego_r's PBed +0x14 / BedSide +0x30 (engine/CAIStateGroup_GoToBed.h, 96-byte class).
//   Was ??1Pair@@QAE@XZ on a stand-in pair (MemberA / MemberB / MemberC) labelled _Dest_val<pair<CCharString,CSymbolInfo>>.
// TU: VC7.1 emits the implicit destructor as a pick-any COMDAT in a TU that destroys a CAIStateGroup_GoToBed out of line. 0x008FEB97, the
//   class's deleting destructor, does so on the shared header, byte- and relocation-equal to retail, so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424).
#include "../src/compiled/00/8f/CAIStateGroupGoToBed_scalardeletingdestructor_008feb97.cpp"
