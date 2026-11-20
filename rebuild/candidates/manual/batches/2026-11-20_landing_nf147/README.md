# 2026-11-20 landing nf147: q6408 mode-pointer remainder and q4802 vector-assign (10 rows)

Records: `naming/2026-11-20_agentNF147/` (selfcheck 0 problems). 2 entries, both still blocked: q6408 has only its self row 0x0073ACC0
left (all 34 binders now bind the claim); q4802 has 6 rows left (0x00762E10, 0x00764E90, 0x00B21DD0, 0x00BCD850, 0x00BCD8F0, 0x00BCD950).

## Rows (10): all RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q6408, the 8 binders NF144 left: 0x008C48A0 / 0x008C0C00 (the landed 0x008C4990 shape), 0x008C0D10, 0x00848DB0, 0x008C1C30 (onto
  NCreatureMode_CModeBriarRoseConcentrate.h), 0x008676A0 (onto NCreatureMode_CModeDrained.h, with its callee's self row 0x00868220
  respelled to the PDB (CThing&, CThing&)), 0x008A08A0 (onto NCreatureMode_CModeScreamerSuperAttack.h), 0x008A1A10 (header-less CModeScripted
  on the shared CModeStandBase with a CAnimFilterOfGroup member).
- q4802: 0x00B21E90 on engine/CCardinalSpline.h, binding the claim; catalog CompilerFlags set to its `// Flags: /d2inlS84`.

## Headers (add-only opt-ins; token-identical without them)
NCreatureMode_CModeBriarRoseConcentrate.h `_CTOR`, NCreatureMode_CModeDrained.h `_CTOR` (each pins its assert line), CAnimFilterOfGroup.h
`_CTOR`. No landed includer defines them. The prep's `(cpp-err)` on the BriarRoseConcentrate macro is the header's existing
`#error` (CDefPointer.h wants FABLE_RETAIL_DEF_CLASS_BASE first): the unmodified header gives the same error with the macro defined alone.

## Decisions (advisor, owner delegated)
1. 0x008A1A10's `*(CThing*)thing.GetThing()` cast between the row's stand-in and the real class: accepted (no new local model, no table
   growth on the mode-ctor or CModeStandBase addresses); header claim queued for the row's CThing_Retail / CScriptThing / CPlayer.
2. 0x008C0D10's in-place respell into NCreatureMode: NOT landed. The gate found a created split (0x00740450's ctor bound under the
   NCreatureMode spelling while 0x00742880 binds the global one; `logs/split_gate_first.txt`), so NF147's stated fallback applies: the
   mode class keeps HEAD's global `struct` token on the shared CModeBase; 0x008C2D60 binds DeleteData of the global-token instance at the
   same count (was the stand-in Destroy_008c2d60). Regraded RELOCATION_MATCH PASS (`logs/fallback_008c0d10.log`); gate 0 after it.
   Queued: move 0x00742880 and 0x008C0D10 together onto the NCreatureMode spelling.
3. 0x00B21E90's pick-any CArray<C3DVector>::operator=: accepted (no landed object defines it strongly; no row binds the name). Queued
   as PRIORITY on 0x00B21DD0, whose stand-in call uses that exact name.

## Checks
- 314 rows rechecked (gate referencing rows + renamed rows + includers of the three headers), 314 good. Gate 0 after the fallback;
  bindings_multi_check 1.
- Table: one set grew, 0x00666820 1 -> 2, a SAME witness. Shrank: 0x00718A00 3 -> 1, 0x0073ACC0 4 -> 1, 0x00861E00 5 -> 4, 0x0073E8F0 2 -> 1.
- One definition: owners of every TABLE-bound COMDAT the new objects emit, plus 0x00740450, 0x008D7370, 0x008C2240, 0x00B21DD0,
  0x00742880 and the landed 0x008C1F70, were compiled into the context link (335 objects). Landing link (10 objects): none. Context link:
  the only clashes involving a landing object are GetEffectFromString in 0x008C1C30 against 0x00667450; HEAD's 0x008C1C30 clashes the
  same way (`logs/link_control_head.log`). The nf144 debt line is superseded by a nf147 line naming 0x00667450. Other clashes involve
  only untouched rows or owners (GetSubDef<CSpecialEffectsDef> among 0x00669580 / 0x006637D0 / 0x00667450 / 0x00845350).
