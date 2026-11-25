# NF429 hand-back (2026-11-25): the AI state-group family

The family is the `CAIStateGroup_*` classes and `CAIStateGroupBase`, 565 tracker rows in 231 classes. NF429 wrote one generator per shape. `NOTES.md` has the method, the shape histogram and every gate.

**Landed: 47 rows** in `batches/2026-11-24_landing_nf429_{c1,c1p,c2,f1,j1}`.

| Batch | Rows | Shape |
|---|---:|---|
| c1 | 29 | constant returns and empty bodies |
| c1p | 3 | the same, on the UsesSeat chain (ruling q2) |
| c2 | 7 | `ret 8` empty bodies; size-mode `return (EAIStateGroupType)1;` |
| f1 | 2 | spelled float literals (0.0f, 1.5f) replacing made-up data symbols (NF427 q3) |
| j1 | 6 | Initialise forwarding to `CAIStateGroupBase::Initialise`; tail jumps to an own member |

**Headers.** There are 32 add-only `FABLE_RETAIL_<C>_VIRTUAL_MODEL` opt-ins. Each model was probed against retail's vftable, and every slot was SAME. Dropping an opt-in gives the repo header back.

**Equivalence.** The coordinator reran equiv_check copy against copy on the tree at landing time.

**Held: 11 generated rows** (`hold_g12/RESULTS.tsv`). The reasons are in `T3.txt`: a base with no header, a template base, competing models, a stand-in jump target, and a shared `_Methods` stand-in class.

**Residue: 553 lines in `T3.txt`.** They are on the T3 queue as nf429 family lines.

## Rulings
- **q2, granted (coordinator, 2026-11-25).** A row may use the landed switch `FABLE_RETAIL_CAISTATEGROUP_USESSEAT_VIRTUALS_PASS27` as a model prerequisite.
  - Row 0x00938380 already defines it, so this is not a first-definer case.
  - The switch completes the hand-written UsesSeat override set, and the probe gives 19/19 SAME.
  - NF324's brief avoided `_PASS` prerequisites only so that it would not create them.
- **q1, open.** Which route for the 218 state-group deleting destructors?
  - (a) accept the base special-member COMDATs when they are byte- and reloc-identical to a landed copy, or
  - (b) ctor-owned Option S units only.

  No pilot has been run, so this waits for one before a ruling.

## Released T3 lines
These 40 lines (`evidence/t3_resolved.tsv`) are released by the landing (`REL=nf324,nf319`):
- nf324: 30 port, 5 held and 1 rejected;
- nf319: 4 held.

## Name fixes
`NAME_FIXES.tsv` lists two made-up float data symbols that should be pooled literals:
- `?g_CombatMinDistance@@3MA` should be `__real@00000000`. Five other rows and one test still name it.
- `?g_value_0125aadc@@3MA` should be `__real@3fc00000`.

They are queued with NF427's float NAME_FIXES.
