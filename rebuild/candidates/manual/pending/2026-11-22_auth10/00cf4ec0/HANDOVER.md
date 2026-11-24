# HANDOVER (early return for context, not an escalation) 0x00CF4EC0 CQ_ArenaHoldingScriptScript::CFanMarker::Main (6164 B), AUTH10 2026-11-22
Start: `c0.cpp` = AUTH1's c0 (pending/2026-11-21_auth1/00cf4ec0, = Codex 2026-11-13 differ draft), no flags, no overlay: DIFFER(6244v6164),
142 masked diff ins (`$S/w22/auth10/tools/mdiff.py`). The draft still carries row-local inline models (must be ported onto shared headers
before landing: EXTRA_RULES) -- not done yet.

## New tool for this row (from 0x008E18D0, see ../008e18d0/ESCALATE.md)
`$S/w22/auth10/tools/fv.sh <src> <force.json> [flags]` (env ROW=00cf4ec0 LEAF='?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ'
FABLE_HDR_OVERLAY=/nonexistent): compiles under c2 with the inline hook, forces chosen (callee, occurrence) results, and prints the masked
diff. Decision table of c0: `inl/c0.json` (241 decisions; dump with the snippet in LOG.md).

## Measured
- Forcing the implicit ~CScriptGameResourceObjectMovieBase to EXPAND at occurrences 8..14 (`inl/fm.json`; ours calls the COMDAT
  0x006E7B80 at the 4 live sites, retail expands all 4) -> DIFFER(6220v6164), 131 masked diff ins, 18 hunks; the movie-dtor calls are gone.
- What is left at the top (hunk r36/b36, +49 B): ours expands the CCountedPointer<CScriptThing> release of the `SetPredicate(CIsThingAlive(Me))`
  temporary inline (`dec [eax]; ... call; push; call; add esp,4`); retail has none there (the AUTH1 note: retail expands ~CIsThingAlive
  around a CALLED ~CScriptThing 0x004AA840). Forcing ~CScriptThing occurrence 75 (1.0/63) to call changed nothing: find the occurrence
  that belongs to the top temporary (candidates: the early group #8/#18/#23, k0..k2, f 1.0, score 63, all expanded) and force it to 0.
- Remaining calls diff after fm.json: loop-1 movie exit pads (retail calls ~CMovieImpPointer / ~CBaseObject at 0x16FD..0x1757, ours calls
  ~CTimer + CCountedPointer<Movie> dtors at 0x173B..0x17ED): the exit-pad layout the draft header names.
## Next
1. Pin the remaining forced set (top ~CScriptThing, the exit pads) until the forced build is RELOCATION_MATCH: that proves the text.
2. Port onto the shared headers (re-check the forced match after the port), then hunt the plain-C++ forms for each forced decision.
  Occurrence numbers must be re-read from each forced run's json (forcing a site shifts later indices).
