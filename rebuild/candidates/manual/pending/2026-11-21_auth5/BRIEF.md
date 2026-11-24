# AUTH5: Account A authoring agent (2026-11-21): the NShape pair 0x00763060 + 0x007602F0 (one TU)

Owner orders, unchanged: do not park; keep working until the rows match. Stalls go to the advisor through the coordinator.
Rows: 0x00763060 NShape::FractureShape (best DIFFER(1132v1146)) and 0x007602F0 NShape::IsShapeConvex (best DIFFER(1126v1126), bracket class).
They share a TU and the same-TU helpers 0x00760020 / 0x00760190 (landed). Locks read AUTH5 (`$S/w22/nflocks/<addr>/owner`).
Be frugal with context (earlier agents ran out): residue counts and differing hunks only, never whole listings.

## Read first
1. `$S/w22/auth1/BRIEF.md` (Paths, Tools, Hard rules, Hand-back: all apply; your outputs go to `$S/w22/auth5/`),
   `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
2. History: `$S/w22/auth1/{007602f0,00763060}/{ESCALATE,LOG}.md`; `rebuild/candidates/manual/pending/2026-11-21_auth1/README.md` and
   `_auth2/README.md`; FINDINGS_LOG entries for p25J / p25L / p25O / p25P / p25Q (grep 007602F0, 00763060, "Ego_d calibration") and the
   2026-11-21 VC10 entries at the end of FINDINGS_LOG; `pending/2026-11-20_p25_open/007602f0/`, `.../00763060/` if present.
3. docs/BUILDING.md section "VC10 SP1 on the Linux lane" and `tools/decomp_pipeline/linux_lane/lane_tools/odall10.py` (its docstring).
4. `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` binds (Inferred locals, fitted arms).

## NEW: VC10 SP1 is on the lane (cl 16.00.40219.01, the compiler Ego_d / FableWin were built with)
Ego_d line sizes are now EXACT evidence, but only when measured right. Step 0 before citing any size:
a. **Library + flags.** Find the NShape compiland in Ego_d (egl.py / the PDB's compiland and S_ENVBLOCK records) and pick its
   `odall10.py --lib` preset. The library command lines we have are cut at `-Fp`; confirm `-Gr` (fastcall default) by reading the
   Ego_d mangled name of a free NShape function (`YI` = fastcall, `YA` = cdecl). If the full line shows other switches, pass them after `--`.
b. **STL mode.** The contested lines are iterator statements (0x00763060 lines 197-204, 0x007602F0 lines 309-338). Default odall10 mode
   compiles them against the VC7.1 STL model and prints confident wrong numbers. Measure them only in a STANDALONE `--stl` text (VC10
   <vector>, C3DVector declared as Ego_d has it: user copy ctor, no operator=, Normalise returns C3DVector&; FINDINGS p25P).
c. **Validate the STL mode on this compiland.** `--stl` is validated only on BBBLibrary (13 vector<unsigned long> members of
   lib_bank_file.obj). This compiland may have a different PCH (standard_headers.hpp) that sets iterator-debug / SCL macros. Reproduce
   the compiland's own `std::vector<C3DVector>` (and its iterator) member sizes from Ego_d with `--stl --lib <its library>` first; if they
   differ, find the macro (e.g. _ITERATOR_DEBUG_LEVEL, _SECURE_SCL, _HAS_ITERATOR_DEBUGGING) that reproduces them, and record it.
d. **Re-measure AUTH1's 199/204 rejection.** AUTH1's `odl.py` dropped the wrapped bytes of 6+ byte instructions (fixed 2026-11-21). Re-measure
   with odall10 and record, before anything else, whether the rejection of lines 199/204 rests on line attribution or on size, and whether
   it survives. Likewise the 0x007602F0 24/44 v 41/58 decode.
Exact Ego_d line agreement settles a disputed text without an owner ruling; then hunt the TLC (VC7.1 3077 /O2) text that matches retail.
Grading and landing stay VC7.1 3077 (q.py / rg1.sh); VC10 is evidence only.

## Report-only (no lock, do not edit): 0x0093D270 line 55
Measure Ego_d's line 55 (76 B) of CAIStateGroup_MeleeCombatNearbyBase::IsRunnable with odall10 (right preset) against the candidate texts in
`$S/w22/auth1/0093d270/` and report the sizes in your hand-back.

## Stall rule
25 graded variants with no residue reduction on a row, OR every idea left breaks a rule: write `$S/w22/auth5/<addr>/ESCALATE.md` and return.
On a match: source + `_test.cpp` + NOTES.md + MATCHES.tsv line (both rows hand back together if the TU text is shared), then return.
Keep STATUS.tsv / LOG.md current. Never write into /home/user/FableDecomp.
