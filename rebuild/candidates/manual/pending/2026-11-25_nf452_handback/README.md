# NF452 hand-back (2026-11-25): GameCODA CSalRenderDisable and CSalRenderPan families

`d1/` and `p1/` hold each batch's README (every gate number), NOTES, TABLE_CHANGES, NAME_FIXES and vftable check. `STATUS.tsv` lists all 43 ACCEPT lines over 31 addresses.

## Landed: 43 rows in unit 0x00C55270 (`batches/2026-11-24_landing_nf452_d1`, `_p1`)
p1 is stacked on d1, so the two batches landed in that order.

**d1 (21 rows): CSalRenderDisable, plus its SSE2 and MMX subclasses.**
- 0x00C55270, the unit owner. It is re-ported and now news the real classes. Its 8 funclets come along.
- Thin rows: 0x00C55070 and 0x00C550D0, plus the new 0x00C550B0 and 0x00C55110.
- ctor 0x00C6E760, dtor 0x00C6E780.
- Params3DInit 0x00C6EBA0, with UpdateFilters 0x00C6EBE0 as its thin row.
- SetGain 0x00C6EC50, SetPosition 0x00C6EC80.
- The two SIMD Process rows 0x00C6EEF0 and 0x00C6FE10.

**p1 (22 rows): CSalRenderPan, plus its SSE2 and MMX subclasses (6 channels in retail, 4 in the SDK).**
- The d1 unit rows.
- New thin rows 0x00C55090 and 0x00C550F0.
- ctor 0x00C6D9D0, dtor 0x00C6DA30.
- Params3DInit 0x00C6DE60, with UpdateFilters 0x00C6DEA0 as its thin row.
- SetGain 0x00C6E020.
- Process rows 0x00C6E410 and 0x00C6F990.

**Six new headers:**
- `CSalRenderDisable.h` (0x38) and `CSalRenderPan.h` (0x78), both with their size asserted. They share FilterPan through `FABLE_GAMECODA_FILTERPAN_DEFINED`.
- Their SSE2 and MMX subclasses.

**Catalog changes:**
- Four C3DAnimationUtil mislabels become the SDK SIMD classes. These close the NF441 NAME_FIXES and the T3 nf372 identity lines for 0x00C55090, 0x00C550B0, 0x00C550F0 and 0x00C55110. Each address now has one spelling.
- The four new thin rows get `/O2 /Oy /W3 /EHsc`.
- The source file names keep their stand-in spelling, as in NF445.

## Rulings (advisor decision, owner delegated)
- **r452a, `namespace Row00c6f990` for the MMX tap mixers: granted as existing debt, not a precedent.**
  - The batch binds no `Row00c6` spelling, and the table has no `MixGain*MMX` lines.
  - HEAD already binds four Row-scoped helper names at retail addresses; the stand-in regex does not catch them. They are queued as the T3 nf452 family line.
- **r452b, the CAudioMaster LOCAL_CLASS line moving from 0x00C6DEA0 to its owner 0x00C6DE60: accepted.** It is the same option S slice and the same HEAD debt.
- **r452c, the names whose bodies are not exact (both Process rows, the Pan ctor): proven, on the NF445 basis.**
  - Source 1 is the SDK CodeView record.
  - Source 2 is the retail tie:
    - The Process rows sit at the SDK slot of their vftables with `ret 8`. The vt432 check reads `a` at every slot except slot 0, and the relocation sequence matches.
    - The Pan ctor stores `??_7CSalRenderPan` and is the base ctor of 0x00C55090 and 0x00C550F0. Its 36 v 30 instructions come from the 6-channel bounds.
- **Lander:** d1 and p1 were held at first on T3 nf372 lines keyed on 0x00C550B0 / 0x00C55110 (and 0x00C55090 / 0x00C550F0). These are the rows' own addresses, and the batch fixes exactly those identities, so they were landed with `REL=nf372`.

## Table-incident check
NF451's repo table write overlapped NF452's run. NF452's simulated d1 table equals HEAD dfaf7c8908's table plus exactly d1's nine TABLE_CHANGES lines, so its gates ran on a clean table.

## Coordinator checks at landing
- **Lander:** d1 21 / 21 and p1 22 / 22, 0 held, 0 recheck failed. The option S members are 5 + 7.
- **Baselines:**
  - bindings_multi: 1 (NullCoord);
  - funclets: 1,704 checked, 0 broken;
  - s units: 159 checked, 0 failed;
  - descriptive_check: 190 lines, 0 FAIL.
- **equiv_check:** all six headers are new; 57,160 rows checked, 0 differing (`EQUIV_CHECK.txt`).

## Queued
- **NAME_FIXES_QUEUE (nf452):** MixFadeIn / MixFadeOut for Disable (0x00C6E790, 0x00C6E820) and for Pan (0x00C6DA40, 0x00C6DAD0). Each ties exactly (53 / 54 instructions) and matches the SDK object order.
- **T3 nf452 family:** the Row-scoped SIMD helpers.
- **T3 nf452 slots:** the targets left unresolved by the /FORCE link: base Process 0x00C6E8F0 / 0x00C6DBA0 and Pan SetPosition 0x00C6E050, which have no rows. Booked as slot debt alongside R-NF445-1.
- **Held under `COORD_HOLD_NF452` locks:**
  - the slot-0 stand-ins (`??_G` 0x00C6D350 / 0x00C554C0 / 0x00C55550, dtors 0x00C554E0 / 0x00C55570);
  - the Hrtf family's pre-locked rows.
  - pk_all already shows the new headers emitting the `??_G` / `??1` copies EQ at their retail homes.
- **Still open:** the CAudioMaster local-view port in the SIMD Process rows (HEAD debt).
