# 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering: parked (p25H, 2026-11-20)

**State:** DIFFER(2296v2300). The best source is `best_v2_state_text.cpp`, with 670 diff lines (`g.py --full`). That is the draft
(pending/2026-11-07_linux_hand827) plus Ego_d's single `state_text` local, reused for both the debug text and the state-group name
(Ego_d lines 941 / 949, 27 B each); the draft regraded unchanged on today's headers gives 716. No header change, so there is no overlay
diff.

## Residue (unchanged in kind from the draft)
- **Register allocation:** retail keeps `info` in ebp for the whole function and re-reads `info.Engine` ([ebp+34h]) as the `this` of every
  SetPersistentPrimitive / AddPrimitive call, after the primitive temporary is built. It loads the vtable into ebx before the temporary.
- **Ours:** we load Engine once, spill it ([esp+28h]) and reuse ebp for the vtable.
- **Frame slots:** they are shifted by one dword from the first store on (retail `mov [esp+48h],IMM` before the list clear(), ours
  [esp+44h]).

## Measured (each against the draft)
| change | grade | diff lines |
|---|---|---|
| draft unchanged | DIFFER(2296v2300) | 716 |
| PDB reference member `CIEngine& Engine` (FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO, `info.Engine.`; `v1_reference_engine.cpp`) | same bytes | 716 |
| `float height` declared before the def name | DIFFER | 858 |
| unused `C3DVector up` in the building block | dead, no change | 716 |
| Ego_d single `state_text` local | DIFFER(2296v2300) | 670 (best) |
| Ego_d `vect` offset (`C3DVector vect(0,0,2.0f)`, `PeekPos() + vect`, `vect.Z += 0.3f`) on top of state_text (`v3_egod_vect.cpp`) | DIFFER(2299v2300) | 718 |

`vect + PeekPos()` gives the same result as `PeekPos() + vect`.

## Ego_d skeleton (`egod_lines.tsv` / `egod_locals.txt`, 50 lines)
Block locals:
- {vect, def_name {state_text}}
- {perception}
- {up}
- {tracker, draw_pos, level {name}}

Line map (my reading):

| Ego_d line | size | reading |
|---|---|---|
| 917 | | clear |
| 918 | | base call |
| 926 | | names flag |
| 928 | 58 | def_name |
| 930 | 30 | `C3DVector vect(0,0,2)` |
| 934 / 935 | 66 / 150 | handle push_back / line |
| 936 / 937 | 72 / 253 | handle push_back / text |
| 939 | | brain test |
| 941 / 949 | | state_text |
| 944 / 952 | 12 each | `vect.Z += 0.3f` |
| 958 / 960 / 962 / 963 | | stealth flag / HasTC / perception / its Draw |
| 965 | | buildings flag |
| 967 | 30 | `C3DVector up(0,0,1.5)` |
| 968 | 70 | home == work test |
| 971 / 972 | 78 / 166 | push_back / line |
| 974 | | else |
| 976 / 982 | | if home / if work |
| 990 | | nav flag |
| 992 | | draw_pos = focal |
| 993 | | `draw_pos.Z += 0.7f` |
| 994 | | level |
| 995 | | AddPrimitive |
| 998 | 190 | tracker test |
| 1000 | | `draw_pos.Z += 0.2f` |
| 1001 | | name |
| 1002 | | AddPrimitive |
| 1006 | | return false |

## Next leads
- **Colour-slot discriminator (advisor lead, first to try on resume):** retail writes both name-block colour temporaries to the same slot,
  [esp+10h] (at 0x00833C91.. and 0x00833D3A..). Ours puts the first in [esp+10h] and the second in [esp+14h]. That extra live dword is a
  candidate cause of the one-dword frame shift (first store [esp+48h] in retail, [esp+44h] in ours).
  - **Resume point:** v3 (`v3_egod_vect.cpp`) made three changes at once: colour temporaries, the inline font, and the `vect` offset form.
    First check v3's first store ([esp+44h] or [esp+48h]). Then split the three changes into single-change variants on v2 and judge each
    by slot assignment (do both colours share [esp+10h]? does the first store move to [esp+48h]?), not by diff-line count.
- **Full Ego_d rewrite:** `up` + `PeekPos() + up` in the building block, `draw_pos` and `level` / `name` scoped as Ego_d has them, with
  the state_text form kept. Then compare frame-slot assignment first: the one-dword shift should move when the local set matches.
- **c2 symbol tooling:** the retail ebp=info choice is a c2 priority question, so check it with the symlog / c2prio tools
  (w26/p25A/tools/c2) on the best source.
- **Not yet done:** a sweep with dummies that reach c2's symbol pool (the literal-initialised kind does not), per today's p25F
  correction.

FINDINGS-ready: 2026-11-20 (Account B p25H, 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering, parked) DIFFER(2296v2300),
670 diff lines with Ego_d's single `state_text` local (from 716). Retail keeps `info` in ebp and re-reads info.Engine [ebp+34h] after
each primitive temporary is built, with the vtable loaded into ebx before it; ours spills Engine once. The frame is one dword off from
the first store. The PDB reference member `CIEngine& Engine` gives byte-identical output to the pointer member. Ego_d's `vect` offset
form shrinks the size gap (2299v2300) but adds diff lines. Next: the colour-slot lead in Next leads. Files: $W/parked/00833c00/.
