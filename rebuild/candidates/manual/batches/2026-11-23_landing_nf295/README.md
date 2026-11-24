# Landing nf295 (Account A, 2026-11-23): T1 "other" class / namespace, NF294's continuation (9 rows, 1 header opt-in, 1 self port)

Hand-back: `pending/2026-11-23_nf295_handback/`; helpers (pickany.py, cmpcomdat.py; cls.py / tri4.py now block-bounded) under
`naming_batch/agent_tools/nf295/`. NF294's 4 follow-ups and its entries 61-120 (64 addresses): 9 rows changed; 33 skipped-port, 20
skipped-held; no rejects, no vtable change.

- **0x0097EC00 AIAction_MoveToPos (partial):** 0x0094A6D0 through a new add-only opt-in
  `FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_MOVETOPOS` in `engine/CAIStateGroupBase.h` (a copy of NF287's LOOKATPOS block; equiv_check
  1,970 includers, 0 differ; the planted control flags 82); 0x0093CB20 / 0x0091E4B0 declare the member on their own real-name local
  `CAIStateGroupBase` (0x0093CB20 includes the header under a rename macro). Types widened byte-neutrally: `long` -> the PDB enums
  (engine/fable_ai_action_enums.h), `bool*` -> `bool&`, `CThing*` -> `const CThing*`. 0x00968230 stays a port.
- **0x0073A9B0 CAnimEntryLite ctor (closes 0x0073D180 / 0x00754F10 / 0x00863090 too):** 0x008E9190 and 0x00752640 onto
  `engine/NAnimationEntryBodge_CAnimEntryLite.h`; the self row 0x0073A9B0 ported onto the same header in the same unit (Q3: without it
  0x008E9190 would lose a ctor reference HEAD resolved), catalog renamed `NAnimationEntryBodge::CAnimEntryLite` (files renamed; rechecked
  again after the rename). 0x00752640's counted-pointer copies under the real names (`??4`, the `(T*)` ctor, DeleteData) each equal retail at
  the owner's address (pickany.py); its `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` define makes its unused ReleaseData copy equal to owner
  0x00754F10's, the only order retail has (Q5, accepted). 0x008E9190's unused inline made-up `Clear` now sits on the real class name (Q4:
  debt, NAME_FIXES).
- **0x00A0D2D0 RemoveFromInterface:** 0x00634470 re-ported onto its landed siblings' shape (0x00633A60 / 0x00633880); function, relocations
  and `.drectve` equal; the object also emits 5 unreferenced 3-byte `boost::scoped_ptr<T>::operator->` pick-any copies, the kind sibling
  0x00633A60 already emits 15 of (Q2, accepted).
- **0x00993C10** (binder 0x00B6D940) protected member on its real-name local CDataInputStream; **0x0084AF90** (binder 0x00846660) local
  `CEventTalkedTo` nested in `NEntityEvents`, class-keyed, taking `CThing&`.
- Table: 3 stale witnesses (0x00E8BE10 / 0x00E8BD30 / 0x00E8CA60: 0x00E8B9C0's HEAD object binds only the NScript spellings) and the moved
  binders' lines: 13 deleted, 1 added. Q1: 0x0093C4E0 (the flat CAIStateGroup_StealthSeekEnemy.h has no CAIStateGroupBase base) is a T3 port.

## Recheck
`logs/recheck.log` (landed tree): 9 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT; `logs/recheck_0073a9b0_after_rename.log` PASS.
