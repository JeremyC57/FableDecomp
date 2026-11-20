# NF117 notes (2026-11-20, round 10): heal the CMenuEntry vtable splits left by nf111

## Result
- 0x0053DFE0 `??0CMenuEntry@NUISystem@@QAE@J@Z`: **ported** onto engine/NUISystem_CMenuEntry.h (which pulls NUISystem_CClickable.h and
  its chain). The local stand-in bases CClickableBase0/1/2, the local CClickable and the local CMenuEntry are gone. RELOCATION_MATCH +
  PASS on form 1. The compiler now emits the vtable stores from the shared hierarchy:
  - 0x0124662C -> `??_7CMenuEntry@NUISystem@@6BCUIInterface@1@@` (was `...6BCClickableBase0@1@@`)
  - 0x01246604 -> `??_7CMenuEntry@NUISystem@@6BCObserver@1@@` (was `...6BCClickableBase1@1@@`)
  - 0x012465FC -> `??_7CMenuEntry@NUISystem@@6BCActionParam@1@@` (was `...6BCClickableBase2@1@@`)
  These are the spellings the destructor row 0x0053E4E0 binds. Fable.exe scan: only 0x0053DFE0 and 0x0053E4E0 reference the three
  vtables, so after this hand-back each vtable has one spelling.
- 0x0053DA30: **self rename** `InitializeRumbleActions` -> `GetDataFromDef` (`?GetDataFromDef@CMenuEntry@NUISystem@@QAEXXZ`).
  The row is a claimed extra row (lock `nflocks/0053da30`, NF117). It is respelled in place inside its existing local model, with no
  port and no change to its body. RELOCATION_MATCH + PASS. Its only landed binder is 0x0053DFE0 (this hand-back), so no split is
  created. The exe scan also finds an unlanded caller at 0x0053E0AC.

## Callee name 0x0053DA30 (two sources)
1. Name sets: the Ego_r.pdb and FableWin.pdb publics list `?GetDataFromDef@CMenuEntry@NUISystem@@QAEXXZ`.
   - Among CMenuEntry's members it is the only non-virtual `void ()` member. The others are virtual, plus the inline
     IsMenuEntryAvailable and the static GSetForceProcessing.
   - `InitializeRumbleActions` (also "InitialiseRumble") appears in none of Ego_r / FableWin publics, symbols or types (0 hits each).
   - The shared header already declares `void GetDataFromDef(); // 0x0053DA30`.
2. The body and its callers:
   - 0x0053DA30 fetches the entry's def (own slot 108) and copies def +0x184 into this +0x18C. +0x18C is `CClickable::InputDelay` in
     engine/NUISystem_CClickable.h (Ego_r CClickable InputDelay +444, retail 0x30 lower; Ego_r CUIDef InputDelay +416). It then turns the
     def's action ids into CActions on the entry's action lists.
   - Its caller is the constructor, which calls it last. Every sibling component constructor ends the same way, with its own
     GetDataFromDef: CClickable(long) 0x0055B460 -> 0x0055B040, plus the ~20 `?GetDataFromDef@C*@NUISystem@@QAEXXZ` lines in
     reloc_bindings.tsv.

## Other copies of the ctor body
- The landed thunk / ??_G rows 0x0053E8C0, 0x0053E8D0 and 0x0053E8E0 carry the same CMenuEntry(long) body on the same header as
  their "constructor's TU". It calls GetDataFromDef(); the code tokens match this row, and only the parameter name differs (`def` there).
  This row uses `definition`, the FableWin.pdb S_BPREL32 name of ??0CMenuEntry@NUISystem@@QAE@J@Z. Those rows already bind
  GetDataFromDef and the shared-hierarchy vtables, so no regrade is needed.
- 0x0053D720 (GoForward) mentions 0x0053DFE0 only in a layout comment.
- The second caller of 0x0053DA30, at 0x0053E0AC, lies in an unlanded manifest gap: 0x0053E050, 103 bytes, under host 0x0053DFE0
  in manifest-gaps.tsv. It has the same ctor shape and calls 0x0055B4C0 and 0x0053DA30, which looks like a second CMenuEntry
  constructor; it is not proven here. It has no catalog row and no COORDINATION_CLAIMS / UNMATCHED_ROWS line. Whoever authors it must
  bind `?GetDataFromDef@CMenuEntry@NUISystem@@QAEXXZ`; tell Account B in the COORDINATION_LOG.
- Catalog uniqueness: no other address carries CMenuEntry::GetDataFromDef. The checks were build_candidates.ps1,
  auto-re-candidates.tsv, rebuild/manifest and reloc_bindings.tsv.

## Open / doubt (not changed here)
- The 0x0053DA30 local model's member names `RumbleOnSelected` / `RumbleOnBack` / `RumblePre` / `RumbleOnVisible` / `RumbleOnUnselected`
  / `RumbleRepeat` are probably wrong. With a uniform 0x1C shift from PDB CUIDef, def +0xC4..+0xD8 would be Action / ActionOnBack /
  ActionOnSelected / ActionOnUnselected / ActionOnDestruction / PreAction, and +0x184 InputDelay. Retail's def layout is not proven
  (the PDBs come from a later revision), and the local slot names (AddActionOnSelected for the +0xC4 field, ...) do not line up with
  that mapping. So they are left as the old row's descriptive local names. A real port of 0x0053DA30 onto the shared headers
  (CDefPointer.h, NUISystem_CAction.h, a CUIDef header) would settle them. vland warns SHARED_HEADER_AVAILABLE CDefPointer on it; the
  model is pre-existing, not new.
- Comment-only mentions of `InitializeRumbleActions (0x0053DA30)` remain in 0x0053D720, 0x0055C810 and 0x0055C860 (NAME_FIXES.tsv).
  None of them binds the symbol.

## Landing notes
- The self rename moves the 0x0053DA30 source and test file names
  (`00/53/NUISystemCMenuEntry_InitializeRumbleActions_0053da30.cpp` / `_test.cpp`). No EH funclet row includes either file:
  grep of rebuild/src/compiled/01 for 0053da30 / InitializeRumble finds nothing.
- vland warnings in recheck_final.log are expected until the catalog label moves:
  - RELOC_NAME on 0x0053DFE0 (label still InitializeRumbleActions);
  - STANDIN_LEAF on 0x0053DA30 (catalog name).
- Tests: 0x0053DFE0's test respells its stand-in member `InitializeRumbleActions` to `GetDataFromDef`. The test keeps its
  Entry/Clickable stand-ins, which are not real class names. 0x0053DA30's test changes one comment line. PassPatterns are unchanged.
- Flags: neither row has a Flags line or CompilerFlags, and neither row changes them.

## Descriptive names
None added. GetDataFromDef is the PDB name. The Rumble* local member names in 0x0053DA30 are pre-existing and descriptive (see above).
