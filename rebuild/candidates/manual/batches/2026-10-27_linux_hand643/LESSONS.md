# Chunk 643 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **An early-out that copies a shared zero local can be the frame-order lever.** `C2DVector force(0); if (off) { Output =
  CKinematic(force); return; }` framed `away` above `force`; building the early-out's own `CKinematic(C2DVector(0, 0))` and
  declaring `force` after the `if` gives retail's frame AND one shared pair of zero stores ahead of the `je` (VC7.1 hoists the
  identical stores of both paths). A user copy ctor on C2DVector also fixed the frame but re-stored the zeros in the
  early-out (0x00A873E0).
- **Respell the parameter kind first:** `Process(const CSteeringAgent&)` -> PDB `const CSteeringAgent*` plus the PDB inline
  accessors on the pointee (`IsDead() { return NavigatorInterface->IsDead(); }` etc.) fixed every eax/edx vcall-temp swap in
  one grade (0x00A873E0; same lever as agent637's GetRadius(bool)).
- **3D subtraction in a 2D predicate re-keys x87 operand order.** `C2DVector delta(pos - CentrePos)` (C3DVector operator-,
  dead Z lane, C2DVector(const C3DVector&)) made the first inlined copy of a 4-way unrolled find_if square dx before dy as
  retail; four open-coded 2D forms (dx/dy locals, radius locals) could not. Before parking "x87 canonical order", rebuild the
  expression from the PDB's vector operators (0x008AAC50).
- **STLport vector indexing through `*(begin() + n)` and `size() = end() - begin()`** gives retail's per-access
  `mov eax,[base] ; lea ecx,[eax+esi*8]`; `_M_start[i]` CSEs `i*8` into a register and swaps esi/edi (0x008BF500 loop).
- `strcmp(text, "") == 0` returned directly gives `sete`; the retail CCharString::operator==(const char*) null arm is
  `if (strcmp(text, "") == 0) return true; return false;` (0x00871290 probes).

Review at landing:
- No row needed more than 6 grades. Grades per row: 00a873e0 11 probes (parallel one-lever variants), 008aac50 3, 00b50370 5,
  008bf500 4, 009faf70 2, 00871290 5, 00774730 2, 00bbcc70 2, 0056f30b 1, 00c3d470 1.
- 0x00A873E0 uses local CSB/CSBAgents models next to engine/CSB.h / engine/CSBAgents.h (flattened vptr; header debt).
- The landed 0x00870D70 (NCreatureActionCActionEventAddObject_SetDataFromString) declares 0x00870C90 as a global with a pointer
  out-param and its class outside NCreatureAction; the PDB has an anonymous-namespace fastcall with `CCharString&`, and the
  class's SetDataFromString is `NCreatureAction::...::SetDataFromString` protected virtual (MAE). See NAME_FIXES.tsv.
- The landed out-of-line 0x008AABC0 grades MATCH with the PDB-vocabulary predicate used here; the landing may unify the two
  TUs' CVectorFilter_FurtherFromPointThan::operator() bodies.
- The inherited 0x00871290 best attempt carried `#pragma pack(push, 1)`; the parked file has it removed.
- The 00bbcc70 best attempt grades 411v415 on disk (the brief recorded 415v415 for a variant that was not saved).
- reloc_bindings.tsv: 0x00A01B50, 0x009A4EC0, 0x00A05840, 0x009F2E20, 0x009BEF80 carry many non-PDB spellings (NAME_FIXES.tsv).
