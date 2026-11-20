# 2026-11-20 landing nf117: heal the two CMenuEntry vtable splits nf111 created (2 rows)

Records: `naming/2026-11-20_agentNF117/` (selfcheck 0 problems). The nf111 landing respelled CMenuEntry's vtables in its destructor row 0x0053E4E0
but the older all-local ctor row 0x0053DFE0 still bound the CClickableBase0/1/2 stand-ins, so 0x0124662C and 0x012465FC carried two names (the
gate's section-2 parser missed them; FINDINGS_LOG / GOTCHAS 2026-11-20).

## Rows (2): 2 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 0x0053DFE0 `??0CMenuEntry@NUISystem@@QAE@J@Z`: ported onto engine/NUISystem_CMenuEntry.h (+ NUISystem_CClickable.h); the local stand-in bases,
  local CClickable and the base-list CMenuEntry are gone; parameter `definition` (FableWin.pdb). First form matched.
- 0x0053DA30: `?GetDataFromDef@CMenuEntry@NUISystem@@QAEXXZ`, not the catalog's `InitializeRumbleActions` (in neither PDB). Two sources: Ego_r +
  FableWin list GetDataFromDef as CMenuEntry's only non-inline non-virtual void() member, and the shared header already declares it here; the body
  copies a def field into CClickable::InputDelay (+0x18C) and the ctor calls it last, as every sibling component ctor ends with its own
  GetDataFromDef (CClickable 0x0055B460 -> 0x0055B040). Respelled in place inside its existing local model; body unchanged.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0; no header changed. Referencing row 0x0053E4E0 good. No-new-split gate 0; table
spelling sets: 0x0124662C, 0x01246604, 0x012465FC 2 -> 1 each; 0x0053DA30 1 -> 1. Comments naming `InitializeRumbleActions` remain in 0x0053D720,
0x0055C810, 0x0055C860 (bind nothing; NAME_FIXES). The unlanded gap 0x0053E050 (a CMenuEntry-shaped ctor) also calls 0x0053DA30: told to B.
