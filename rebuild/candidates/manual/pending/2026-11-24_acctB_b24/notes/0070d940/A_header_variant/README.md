# 0x0070D940 with GetMeshAnim from a shared header (Account A, 2026-11-24): LANDED as acctB_b26 (owner yes)

The question: can the row get CAnimSet::GetMeshAnim (0x0070C900) from a header instead of its verbatim strong same-TU copy?

Yes. `row_header_variant.cpp` is B's staged row with the local copy removed and `#define FABLE_RETAIL_CANIMSET_GETMESHANIM_INLINE_BODY`
added. `CAnimSet.h.patch` adds that opt-in to B's CAnimSet.h overlay: the same body, spelled `inline`. Graded by `$S/gma/vtest.sh`
(B's whole overlay plus this header):
- `grade.txt`: RELOCATION_MATCH PASS, as B's row.
- `pkall.log` / `pickany.log`: the GetMeshAnim copy is now `pickany` (a COMDAT), not `strong`, and is EQ to retail 0x0070C900. Every
  other copy is as in B's row. Only the listing order of the unreferenced 14-byte `list<CAnimSet>::empty` copy moves.

Retail fact for the opt-in comment: Ego_r.pdb and FableWin.pdb both put GetMeshAnim, every other CAnimSet method and
CalcInterpolatedMeshAnim in animate_thing.obj (ghidra_out/egor_obj_functions.tsv, fablewin_obj_functions.tsv). Retail compiled the
body in the caller's TU as an ordinary out-of-line member. Neither spelling reproduces that TU in a one-row-per-TU tree; the header
form avoids a second strong definition of a landed symbol.

Still open: the landed 0x0070C900 row models its classes locally (no shared headers), so the body exists twice (that row and the
header). Porting 0x0070C900 onto the shared headers with this body would make the header the one source; not attempted.
Owner approved 2026-11-24; landed in batches/2026-11-24_landing_acctB_b26.
