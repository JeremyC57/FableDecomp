# NF330 lessons (2026-11-24)
- After a container restart the first Wine cl.exe boots the prefix services (services.exe, winedevice, plugplay, svchost, rpcss); they
  inherit the caller's stdout / stderr. A grader that captures output (subprocess.run(capture_output=True), vprobe2.py) then waits for ever
  on a zombie cl.exe. Warm Wine first with a detached run whose output goes to /dev/null (`nohup wine cmd /c exit >/dev/null 2>&1 </dev/null &`);
  if it already happened, kill only the services your own cl spawned (same start time) and warm up.
- Base sizes for genmodel must come from the base model's own sizeof assert, not from the family default: CCreatureAction_BlockBase is 0x124,
  not CCombatActionBase's 0x114 (a wrong --basesize keeps base-tail members and the offset asserts fail in the precheck).
- Re-triage the base models before generating: several "thin" rows sit on landed bases whose model is incomplete (Gaze lacked slots 3 / 10 / 13 / 15,
  the Strafe PROTOTYPE models declared no overrides). The strict probe then fails at the base's slots; the fix is an add-only _RTTI_DECLS block in the
  base model, proved through the derived probe and a negative control without it.
- flatten_pilot v5 treats only `_VIRTUAL_MODEL` macros of the class's own header as this run's own; a base opt-in reached through a `// Needs` line
  needs an explicit exemption (`--own-optins`), or those rows silently drop out as "no landed switch".
- genmodel copies opt-in lines inside the flat class verbatim and lifts every `+0x` member out of them: a flat model with a member opt-in
  (`#ifdef _TYPED` members `#else` flat members) comes out with both member sets unguarded. Check each VM block for carried `#if` lines that guard members.
- vprobe's `new X` form emits no vftable for a class whose ctor is declared out of line (Strafe PROTOTYPE): prove such a base through its derived models.
