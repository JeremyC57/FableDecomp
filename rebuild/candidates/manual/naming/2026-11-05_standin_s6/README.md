# Stand-in naming, step 6 first pass (2026-11-05): 1,796 rows

## Why
Owner rule (2026-11-05): stand-ins are fine when they are consistent and as accurately named as the code allows, in Lionhead style.
The work list was `../2026-11-04_standin_leaf/STANDIN_INACCURATE.tsv`, 9,739 rows.

## Result
- **No bytes changed.** Every row passes gate.py and vland `--recheck-landed` (`<wave>/logs/`), with the same parity as before.
- **Waves:**

  | Wave | What changed | Rows |
  |---|---|---|
  | w1 | `?DestVal@Slot@@` → `CCountedPointer<T>::~CCountedPointer`, explicitly instantiated from `engine/CCountedPointer.h` (the header's Data-first destructor) | 767 |
  | w2 | a free function that lacked its class → `Class_Method` | 332 |
  | w2b | 26 w2 rows whose test takes `&Name` | 26 |
  | w3 | free stand-ins proven by an RTTI vtable slot plus a PDB name → `RealClass_RealMethod` | 230 |
  | w4 | RTTI-proven member stand-ins on classes with no header: real class and method | 80 |
  | w5 | the same on header classes: method renamed only | 193 |
  | w6 | `?ShareData@CCPtr@@` → `CCountedPointer<T>::ShareData` | 150 |
  | w7 | `?Copy@T@@` → the `CCountedPointer<T>` copy ctor | 18 |

- **Pointee T:** the real T only when a typed caller binds the header spelling AND FableWin or Ego_r has that symbol. Otherwise it
  is `CUnprovenPointee_<addr>`, the EXTRA_RULES 2026-10-28 form.

## Labels and queue
- **Labels:** 30 two-source labels in `ghidra_out/labels_standin_s6_2026-11-05.tsv`, applied (30 catalog + 30 manifest renames).
  - Mostly `CCountedPointer<T>::~CCountedPointer` replacing `::Release`. The PDB has no `Release`, only `ReleaseData`, which
    zeroes Info first; these bodies zero Data first, which is the destructor.
  - 0x00CE1040 is now `CCountedPointer<CEntityScriptBase>::ShareData`: every landed caller binds that T. The header comment that
    said CScriptThing is corrected.
- **Queue:** 933 lines added to `NAME_FIXES_QUEUE.tsv` with tags `s6w*`.
  - PRIORITY lines list the other landed spellings to unify.
  - One-source names are queued as PROPOSAL and are not applied.

## Also
- `rebuild/tests/00/56/CTCBoastUI_RequestActivate_0056a860_test.cpp` called its function through `__asm`. It now calls the
  `__fastcall` function directly. 160 other tests still contain asm; that is a separate task.
- **Left:** 7,946 rows, by family, in `NEXT.md`.
