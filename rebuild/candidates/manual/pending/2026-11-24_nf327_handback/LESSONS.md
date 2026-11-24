# NF327 lessons (2026-11-24)
- A NEW header in the opt-in form is mechanical: generator output, the model under `_VIRTUAL_MODEL`, then pin_asserts.py with the
  GENERATOR tree as "old". Stripping the opt-in then gives the generator text back byte for byte (hdrcheck against work/gen0).
- A row can MATCH and still fail objsec on a helper it does not call: VC7.1 emits unreferenced pick-any COMDATs for in-class / `static
  inline` helpers. Moving such a helper off an alias class renames it, changes its linkage and even its body (`lea eax,[ecx+4]` became an
  `add eax,4` register-convention static). Keep the helper where HEAD had it, or ask.
- A Clone over a base with a member that has a destructor (CIntelligentPointer) is not the 0xA8 shape: VC7.1 emitted the implicit
  `??1` chain as COMDATs and called it from `??_G`, where retail inlined it. Read retail's slot-0 body (tools/xdis.py) before building.
- A binder's vftable is part of its objsec: an access / override opt-in that a Clone row needs changes every emitter's table, and the
  emitter may sit on a different base branch (KRAKEN_ROAR) than the Clone, so its table is not retail's either way.
- A pure slot (`= 0`) is provable: the base's own ??_7 is emitted in a derived probe TU (the implicit ctor chain), with `__purecall` at
  the pure slots; a control without `= 0` puts made-up names there.
- An add-only opt-in inside a large `#ifdef` region (CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT) leaves the planted control
  unflagged for includers that never enter the region; classify the misses (no define / define after the include) instead of reading
  the count as a failure.
- ref_scan.py and recheck.py need the repo as cwd; a symlink root in the work folder (tools, rebuild) keeps whatever vland creates
  relative to cwd out of the repo. recheck.py needs its output directory to exist.
- At load ~45 the per-group runs were still fast (3 Clone rows in about a minute); equiv_check over a 1,709-row closure took ~10 minutes.
