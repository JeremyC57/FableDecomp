# NF399 b2 Q1 / Q2: advisor decision (owner delegated), 2026-11-25. Conditional.

**Q1. Land b2 as one unit** (004df54f + 004df507 + 004df365 + opt-in FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_DECL_ONLY + TABLE_CHANGES), with 0x006CC1E0's ~vector copy booked as debt.
- Why it is debt rather than a hold: NF399's probes show 0x006CC1E0's leaf matches only while ~_Vector_base is inline-visible.
  - p5a: the decl-only model moves the leaf.
  - p5b: a forceinline local still emits the 14-byte copy.
  - So retail's own TU could not have carried the jmp form, and a hold would wait for a fix that cannot exist.
- R5 does not apply. Its scope is a CArray-at-vector line witnessed by an unported row, and here the witness 004df507 lands in the unit.
- This applies the NF328 D1 precedent: a differing pick-any copy that no leaf references is debt, while a row that references it through its leaf holds.
- Scope (R7, no blanket exception): the copy is referenced only by .debug$F, the leaf is proven to need the inline definition, and both in-row fixes were measured to fail.
- Consequence for an object-mode build: a jmp definer must precede 0x006CC1E0 in link order.

Conditions to check before landing (agent NF400):
- C1: every landed row that references the name rechecks under the new table with no grade loss, and each undefined reference resolves to 0x004DF54F in retail.
- C2: no `CMember_004df54f` user remains.
- Baselines after landing.

**Q2. Accept** the two scaffolding sections in 004df507 (implicit ctor 9 -> 20 B, MakeOne 22 -> 33 B), on two conditions:
- C3: a reloc-masked retail search for the 20-byte ctor body. A unique hit counts as tier 2; if retail builds the object differently, book it as debt now.
- C4: the layout is confirmed against Ego_r and retail, i.e. sizeof 0x14 and the vector's offset. The class grew 8 bytes but a vector is 12, so it must be explained what the vector replaced.
