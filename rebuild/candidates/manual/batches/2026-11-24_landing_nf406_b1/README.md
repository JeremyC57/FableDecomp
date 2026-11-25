# Landing nf406 b1: four GetClassTypeName virtuals respelt from extern "C" stand-ins

The rows are 0x004DDA94 CTCTargetingAI, 0x004D8154 CTCDCameraPoint, 0x004E07C6 CTCDHeroSuit and 0x004E3148 CTCDExplosion. Each is its own unit.

Each row moves to `?GetClassTypeName@<Class>@@UBE?AVCCharString@@XZ`, and calls the respelt static `?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ` (nf402_b1) instead of a made-up `?Init_<addr>@T@@`.

Name sources:
- the Ego_r.pdb and FableWin.pdb name sets;
- retail vftable slot 22, CTCBase's GetClassTypeName, which points at each address (for example 0x01241254 -> 0x004DDA94).

The retail body is `mov ecx,[esp+4]; call static; mov eax,[esp+4]; ret 4`: a const virtual thiscall that forwards the hidden result slot.

Class texts:
- CTCTargetingAI and CTCDExplosion use the layout-rename form (CTCBerserk 0x004D4A86).
- CTCDCameraPoint and CTCDHeroSuit are local classes over engine/CTCDriverBase.h.
- All use the polymorphic FABLE_RETAIL_CTCBASE branch. A size-assert compile with a failing control shows the virtual overrides slot 22.

Table: adds `?StatGetClassTypeName@CTCTargetingAI@@SI?AVCCharString@@XZ -> 004d2a74` (witness 004dda94).

NF406 gates:
- vland HEAD/new: all RELOCATION_MATCH PASS.
- Tests rewritten as linked tests; the mutation controls fail.
- pk: 0 NEQ, 0 reach flips.
- /FORCE link: 4 -> 4. Those 4 duplicates come from nf402_b1's out-of-line statics against RegisterTCs, and are being fixed separately.

Coordinator: 4/4 landed, 0 LITERAL_DIFF, self_symbols respelt with column 4 aligned, T3 nf402 0x004DDA94 closed, baselines OK.
