# NF425 hand-back (2026-11-25): the Clone / Copy virtual family

The family is the 518 `rtti_identified` addresses whose retail RTTI slot names `?Clone@<C>@@...` (350) or `?Copy@<C>@@...` (168).
Each was landed under a stand-in self, usually a free `?<C>_Clone@@YI...` function.

**Landed: 285 rows** in `batches/2026-11-24_landing_nf425_{j1,j2,j3,s1,o1,o2,o3,h1,h2,h3,i1..i4,j1h,s1h}`. Every row matches and its test passes.

| Form | Rows landed | What the row now is |
|---|---:|---|
| COPY_JMP (j) | 89 | the real `C::Copy`, `*this = *(const C*)other;`, tail-calling the out-of-line `operator=` |
| Option S thin rows (s1) | 26 | `C::Copy` defined by the `operator=` owner's file (UNITS kind S) |
| CLONE_OOL (o) | 95 | `C::Clone` on a local class (o1, o2) or on the class's `engine/<C>.h` (o3) |
| CLONE_INLINE on the header (h) | 64 | `C::Clone` on `engine/<C>.h`, through the NF329 VIRTUAL_MODEL opt-in or another polymorphic branch |
| CLONE_INLINE on a local class (i) | 11 | as h, on a local class |

**Held: 117 accepted rows.**
- **111 on a wrong vftable slot** (`HELD_VTSLOTS.txt`: 109 i rows and 2 h3 rows).
  - The class model lacks overrides retail has, so the `??_7` the row emits binds a slot to the base function. The commonest case is slot 12, `CCombatActionBase::Initialise`, where retail has the class's own Initialise; others are FinishAction, IsToFaceTarget and IsToEvadeAllHits.
  - Per-row slots are in `evidence/vtslots/vtw_<b>.tsv`, written by `tools/vtw425.py` from the retail table and the bound spellings.
  - Agent NF427 has them: it gives the creature-action classes headers that declare their overrides, then retries these rows.
- **6 under the nf386 queue line** (`HELD_NF386.txt`). That line moves these Copy rows behind a header class switch; they stay with it.

Residue that was not generated (105 rows), and the debt the landed rows book, are in `T3.txt`. The T3 queue carries them as nf425 family lines.

## Ruling r1 (advisor decision, owner delegated, 2026-11-25): CLONE_INLINE rows may change `??_7` and add a pick-any `??_G`, if no slot is wrong
The real member makes VC7.1 emit two things HEAD did not:
- the class's `??_7` vftable COMDAT, carrying the real class's slots instead of HEAD's one made-up slot;
- the pick-any `??_G` it references.

A real class whose Clone builds a copy has to emit both. Every added `??_G` is byte- and relocation-equal to retail's slot 0 (186/186).

NF425's own check counted only agreeing slots. The coordinator counted wrong ones too: a slot bound to a retail address other than retail's dword at that index. Retail's table length was read from the exe; it equals ours (e.g. 81).

The grant covers only rows with **0 wrong slots**:

| Batch | Clean | Held |
|---|---:|---:|
| h1 | 50 | 0 |
| h2 | 10 | 0 |
| h3 | 4 | 2 |
| i1..i4 | 11 | 109 |

## The 74 j rows on a local class while `engine/<C>.h` exists (advisor decision, owner delegated)
They land: each HEAD source already modelled the class locally and none included `engine/<C>.h`, so landing moves no row off a shared header. The class header lacks Copy and/or `operator=`.

The header port is queued: the nf425 header family line, with the per-class list in `T3.txt` (`header <C>`). These rows carry the SHARED_HEADER_AVAILABLE warning until it is done.

## Released T3 lines
18 nf324 `skipped-port` / `skipped-held` lines (j1h, s1h), on the rows this hand-back ports:
- the stack-count and `edx` parameter mismatches came from the free stand-in;
- the duplicate strong `Copy` definitions are now the s1 thin-row units.
