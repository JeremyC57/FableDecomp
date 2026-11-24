# NF336 lessons

- A per-TU `#define` must come before the family preamble. Opt-ins defined after the preamble's includes do nothing. mkrow336.py
  writes `work/bodies/<a>.pre` above the preamble for this reason. In a composed TU the same effect makes every later part's per-TU
  opt-in silent (COMPOSE.md).
- The implicit destructor of a class whose vtable the TU emits is expanded into the emitted `??_G` copy. When retail's `??_G` calls
  the dtor out of line, declare the dtor (decl-only opt-in). Making it "visible but called" still expands it. (0x00E95F10:
  `??_GCSpawnedFuncBase` 97 / 56 bytes -> 30 bytes EQ 0x00CDD4A0.)
- An implicit virtual dtor that the `??_G` copy inlines is still emitted as its own COMDAT, referenced only from `.debug$F`
  (`??1?$CLevelScriptBase<T>`, 5-byte `jmp ~CScriptBase`). That is an added unreferenced pick-any COMDAT (NF320 Q2). Check every
  ONLY_N COMDAT for a code referrer, not only for retail equality. (A referrer scan of the relocations does this in a few lines.)
- Do not redirect a script's stdout into a file the script also writes itself (`includers.py > includers.txt`): the summary line
  overwrote the first rows, and equiv_check silently skipped six rows. Diff the row list against the checked set.
- Untruncated censuses only: a `| head` on a grep that decides a table delete hides callers.
- cl error lines from headers carry the header's name. Attribute errors by file, not by line number of the composed TU.
- `vt336.py` reads as many slots as asked. Pass retail's real slot count (CV_HauntedHouseScript: 5), or it reads the next vtable.
