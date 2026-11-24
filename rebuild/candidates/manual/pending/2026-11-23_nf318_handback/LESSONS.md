# NF318 lessons (2026-11-23, header-route rejects)
- The scripted switch route failed mostly because it DROPPED the row's other lines: keep every local declaration of the HEAD row (made-up
  externs, local models) and change only the definition line; 25 of 28 flatten-switch1 SRC_FAILs compiled that way at once.
- A row that includes engine/X.h before `#define FABLE_RETAIL_X` compiles the flat branch: hoist the define above every include.
- Take the definition's return type from the header declaration; an `int` / `unsigned char` stand-in return becomes `bool` / the enum with
  equal bytes in 25 of 27 rows. Where it does not (0x008913C0 returns via `mov eax,1`), stop: it is a body question.
- A precedent's preamble can drag `/alternatename` lines into `.drectve` (C3DPrimitive2's STLport set): use the switches the class header
  itself names when the precedent block is large.
- Before landing a strong self for a member that NScript rows define as an in-TU `inline` copy, compile those rows: their objects emit
  the PDB symbol as a COMDAT and the strong self is LNK2005 (5 CScriptThing members). A grep for `X::M(` definitions finds them.
- vland --recheck-landed on a HEAD payload and a new payload gives both objects for the objsec proof from the exact final texts; comment
  edits after a scratch grade then need no second objsec pass.
- `recheck.sh <tag>` writes work/recheck_<tag>.log: a tag with a `/` silently does nothing (the redirect fails). Use flat tags.
- grade_lane.sh cds into work/: pass absolute list paths.
