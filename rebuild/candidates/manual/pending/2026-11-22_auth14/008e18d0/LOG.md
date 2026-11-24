# 0x008E18D0 -- AUTH14 log (S/w22/auth14/BRIEF.md: two grades + decision table, no hunt)
Base: AUTH11 e5.cpp (== pending/2026-11-22_auth11/008e18d0/e5.cpp), e5n.cpp = e5 minus `#define FABLE_RETAIL_CTHING_GETTC_EGOD_WRAPPER`
(C5 off; with the define absent the overlay CThing.h is token-identical to HEAD). Overlay hdr/ = AUTH11's (AUTH10's IMPLICIT_DTOR
opt-in + AUTH11's add-only GETTC_EGOD_WRAPPER opt-in; orig == HEAD). Config: stock VC7.1, q.py --flags /Gr /d2inlT65, no hook.
## PRE-REGISTERED: grade e4 (baseline reproduction of 2044v2073), e5, e5n once each in that config. No re-grade, no variant.
## Results of the pre-registered grades (grades.txt)
- e4 DIFFER(2165v2073) -- does NOT reproduce AUTH10's 2044v2073 (equals AUTH10's e3 figure at /Gr T64..66).
- e5 DIFFER(1944v2073); e5n DIFFER(1944v2073); e5.q.out == e5n.q.out byte for byte (identical leaf disassembly).
- C5 live check (pp/pp.py, /E with the same env): e5.i has the wrapper body (`GFDebugPDynamicCast(&base` x1, overlay CThing.h resolved),
  e5n.i has none. Header drift since AUTH10 (01:45 base 4b50840a07 .. HEAD): CCountedPointer.h is in the TU but every change is under
  FONTBANK `#if defined(...)` opt-ins this row does not define; CGraphicDataBankInit.h / NUISystem_CManager.h not in the TU. Drift ruled out.
## PRE-REGISTERED baseline diagnostics (same e4 text, not a variant; run once each, then stop whatever the result)
- e4 with FABLE_HDR_OVERLAY = S/w22/auth10/008e18d0/hdr (AUTH10's overlay); e4 with FABLE_HDR_OVERLAY unset. Then readout of e5n
  (force_host.py, empty force list, /Gr /d2inlT65, overlay = hdr/), READOUT ONLY.
## Results of the baseline diagnostics
- e4 with AUTH10's overlay: DIFFER(2165v2073); e4 with no overlay: DIFFER(2044v2073). So AUTH10's recorded 2044 was taken WITHOUT
  the overlay (IMPLICIT_DTOR opt-in absent, the default declared dtor). The proof config (fv.sh, AUTH10 overlay) gives 2165 unforced.
- e5n readout (READOUT ONLY): inl/e5n.json, 261 row decisions (e5: 270). It lacks e5's Find and GFDebugPDynamicCast row decisions
  (C5 live in e5, absent in e5n); every call/expand result in the row equals e5's. Loop-1 LowerBound moves f 1.0/sc 88 -> 0.625/95.
- Threshold pinned from records (e4/e5/e5n/e4i jsons, T65 S9): score 65 -> call; max expanded score 63; 64 not observed. So c2
  expands iff score < T (AUTH11's "calls when score > T" is off by one at the boundary). Always-inline iff c1-4 <= S: 0 violations.
- Grades used 5 (3 pre-registered + 2 baseline diagnostics). Readouts used 1. STOP (no hunt).
## PRE-REGISTERED readout (advisor, after DECISIONS draft): are the sky remainder copies byte-neutral?
- fv.sh W/e4.cpp W/force2.json /Gr /d2inlT65, FABLE_HDR_OVERLAY=W/hdr; force2.json = AUTH10's force1.json minus the two
  __uninitialized_copy entries (k3 / k5, the sky remainders), i.e. only the loop-1 dtor (k1 -> call) and loop-2 LowerBound (k3 -> call).
  RELOCATION_MATCH (0 masked diff ins) = the remainders are byte-neutral (two forced sites, not four); DIFFER = AUTH11's contradiction
  stands. READOUT ONLY (a forced build is never a grade). Run once.
## Result of the forced readout (READOUT ONLY)
- fv2.out: forced 2; mdiff DIFFINS 14, len 2073 v 2073. The 14 are mask cells only: 1.0f pushes and stores, g_CRCTable and CRC-string
  displacements, and two vtable stores, each "<A>" against a literal. gobj.py (V.mask, the q.py mask) on W/e4.f.obj gives
  **RELOCATION_MATCH** (e4.f.gobj.txt).
- So e4 + {loop-1 ~CParticleUpdateParameters -> call, loop-2 LowerBound -> call} at /Gr /d2inlT65 with W/hdr matches. The two sky
  remainder __uninitialized_copy decisions (left "call", sc 68) are byte-neutral: the forced sites are two, not four.
- AUTH12's p6s9.json (stock flags, S9) records T 74 with 73 expanded and 74 called: the boundary is confirmed (expand iff score < T),
  and stock T is 74.

## Coordinator, pre-registered (2026-11-22): e5 and e5n graded once each at /Gr with the default inline threshold (no /d2inlT), overlay hdr/. Acceptance: MATCH / RELOCATION_MATCH.
- coordinator: e5n /Gr default-T with NO overlay, graded once (is the IMPLICIT_DTOR opt-in needed?).
