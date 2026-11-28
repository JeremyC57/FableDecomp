# 0x005458A0 NUISystem::CSpellContainer::AssignSpell -- ESCALATE (AUTH30, 2026-11-27)

## Where it stands (our gates)
- Bytes: Codex's draft (c0.cpp) grades RELOCATION_MATCH 621/621.
- Pick-any: one NEQ copy, `??1?$_List_base@PAVCComponent@NUISystem@@...@std@@QAE@XZ`, 62 bytes against retail 0x0042ABCA's
  23-byte size-mode body (clear called, `pop ecx`). It is leaf-referenced (source a).
- HEAD witness: landed same-TU sibling 0x00545B10 (UnAssignSpell, compiled at HEAD into head/00545b10.obj) emits the IDENTICAL copy:
  bytes equal, relocations equal, COMDAT selection ANY in both (cmpbody + a selection read). So the copy was already NEQ at HEAD.
- The 2026-11-24 advisor rule ("a new row whose leaf calls a NEQ copy identical to one already landed"):
  1. same copy already landed: yes (00545B10, bytes + relocs + selection).
  2. no fix on record: measured here (LOG.md):
     - g1: shared-header size-body opt-in (per-type specialization between `optimize("s")` / `("", on)`, noinline, clear
       declaration-only) makes the copy EQ (23), but the leaf becomes DIFFER(618v621) (frame 0x14 -> 0x1c, edi/ebp swap).
     - g2: the same without noinline expands it: DIFFER(652v621).
     - g3: noinline body with clear visible-but-noinline: copy and clear EQ, leaf DIFFER(597v621).
     - g4: no row-local specialization (shared default template body, compiled at the end of the TU in the row-end size mode): copy
       EQ (23), but the leaf then calls `??1CDeletion` (66 B) where retail expands it: DIFFER(621v621).
     The leaf needs this TU's own /O2 body of ~_List_base visible in place; the 23-byte body exists only in the size TUs at
     0x0042xxxx. This is the link-order case Codex reported.
  3. every other leaf-called copy EQ under both checkers: yes (c0).
  4. booked: the row would carry a `// Pick-any:` note naming 00545B10 (not yet written; waiting for the ruling below).

## What still blocks
c0 shapes its copies with three row devices:
- (a) a row-local `template<> inline std::_List_base<NUISystem::CComponent*, ...>::~_List_base()` specialization (STLport's own
  body text), so the body is compiled in place at /O2 instead of as an end-of-TU instance (g4 shows why it is needed);
- (b) a row-end `#pragma optimize("s", on)` placed before the row's inline CDeletion constructors. g5 (the pragma removed): the leaf
  still matches, but 11 end-of-TU copies turn /O2 and NEQ, 3 of them leaf-referenced (CCountedPointer<CComponent> ctor 0x00429C15,
  _M_insert_dispatch 0x0042B4C3, ~_List_base). Their owners are size TUs (0x0042ABCA's row sets `optimize("s", on)`).
- (c) `#pragma inline_depth(0)` round the first SetDeletion (Inline-budget: the CDeletion copy ctor is called at 0x00545A0B).

## Questions for the ruling
1. May (a) stay as a row-local explicit specialization of the STLport member, or must it move into a shared-header opt-in (an
   `_O2_BODY` specialization in stlport/fable_containers.h, same text)?
2. May (b) land as end-of-TU scaffolding? It is the size-mode counterpart of R-NF409-2 (row-end `optimize("", on)`), but it also
   changes the row-text CDeletion inline constructors that follow it, which R-NF409-2 excludes.
3. If both are yes, the row lands as 2026-11-24 debt on the ~_List_base copy (witness 00545B10). Remaining gates then: my own test,
   name_gate --test, purity, the Ego_d review.

Grades used: c0 + g1..g5 (6).
