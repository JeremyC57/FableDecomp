# 2026-11-13 landing fw -- 0x00D17A80 NScript::CQ_BountyHuntScript::WatchForBoasts (one Fable author pass)

Source: `pending/2026-11-12_linux_handWF19F/`. Three Opus author passes in workflow wf19 left this row at DIFFER(705v785): the body
matched, but the destruction of the boast vector at five early returns was inlined to the wrong depths. Fourth row of the
Opus-only workflows that Opus could not match. The owner asked for one Fable author pass (2026-11-13, "be fast, but accurate";
scout, verifiers and repair on Opus). It matched on the Fable author's first attempt (about 55 minutes for the whole one-row
workflow); both Opus verifiers passed it after a comment / name repair.

**What made it match -- and a NEW kind of per-row flag (owner: please note).** The author traced c2's inliner under a debugger.
VC7.1 has two inliners: the heuristic one (thresholds /d2inlT, /d2inlS) is used only for functions c1 flags when /Og is on the
cl COMMAND LINE; otherwise a budget inliner runs (1000 units per function, shared out site by site, so the earliest sites get the
least). Retail's per-site split of this function is the budget inliner's. The row therefore compiles with
`// Flags: /Og- /d2vol10` and `#pragma optimize("g", on)` in the source: /Og off the command line but global optimisation on, and
the c2-internal "volume" raised so the old body check lets ~vector through (threshold 8; 7 gives 675 bytes; 8, 9, 10, 14, 17, 20
all give retail's 785).
- The source header says plainly that cl.exe has no documented way to pass a volume, that HOW retail's build set it is unknown,
  and that the flags emulate an unidentified build setting of the original TU; they are not a claim about the original command line.
- Precedent: about 45 landed rows already carry undocumented c2 inline switches (/d2inlT.., /d2inlS..) on the same reasoning. No
  landed row used /Og- or /d2vol before this one.
- The source is plain C++; no asm, no forced inlining. If the owner does not accept this flag pair, the row is one revert.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1 (catalog CompilerFlags `/O2 /Oy /W3 /Og- /d2vol10`).
Also fixed in the workflow's repair: the global 0x0143E8F8 is `pDSTGame` (the established landed spelling), not a made-up name.
Header: CQ_BountyHuntScript.h (BOASTS opt-in, declarations). Checks: bindings_multi_check at baseline; funclet_includes_check
1317 checked, 0 broken. The parked differ source in `pending/2026-11-12_linux_handWF19/differ/` is removed (superseded).
Queued (land1011fw): 0x004D9C5D, 0x0074F680 (STLport _Vector_base dtor / ctor, folded COMDATs landed under stand-in names).
