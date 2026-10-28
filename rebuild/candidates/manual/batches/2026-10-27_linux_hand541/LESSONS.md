# Lessons, chunk 541 retry

## GOTCHAS one-liners
- CCountedPointer ctor split (1 inline + N out-of-line calls to 0x429C15): give CCPPointerInfo an inline ctor `(Data, DeleteFunc)` with RefCount(1), and write `if (data) Info = new CCPPointerInfo(Data, DeleteData); else Info = 0;`. VC then inlines only the first construction, as retail does (ndiff 120 -> 11).
- PDB manglings like `...MV5@MV5@@Z` (a repeated by-value class arg without a back-reference) come from a `const` by-value parameter. Declare one of the pair `const T` to reproduce the spelling.
- An STLport vector's `push_back` must be `__forceinline` to inline in VC7.1 RTM. The `__true_type` tag needs a user ctor, `__true_type() { }`, or VC emits a zero byte store for the value-init.
- `!(*it)->IsDead()` inside `&&` emits `not al`; retail's nested `if` form does not.
- Lane gap (3 rows): retail drops the dead `_STLP_alloc_proxy(…, (_Node*)0)` null store of a local list/vector and keeps its fields in registers across calls. RTM, QFE-4035, `/Oa` and `<stdlib.h>` malloc all keep the store. Removing the init by hand closes part of it (e6f0 ndiff 21 -> 17), which proves the cause, but that is not a real STLport source.
- Same-length x87 scheduling residue (CWorldMap 0x63D0A0 / 0x63D4A0): `fld` issued before `push esi`, and pointer loads after the first struct copy. Neither the Normalise inline form nor class-key fixes moved it.

## Naming
- Manifest labels propagated across byte-identical bodies swap twin functions: 0x63D0A0 and 0x63D4A0 differ only in the sampling callee (ground vs theme).
- The Ego_r declaration order of PointIsWithinCellTR/BL/TL/BR follows address order 0x63B890 / 0x63B920 / 0x63B9B0 / 0x63BA30.
- `OnOpenMenus` is a static fastcall member, `NUISystem::CManager` (SIXXZ), not a free function.

## Review at landing
See NOTES.md, section "Review at landing". In short:
- 0x63A7E0 needs a name decision.
- 0x428494 needs a decision on which parameter is const.
- The q.py label for 0063d4a0 is stale.
- Three rows share the STL lane gap.
