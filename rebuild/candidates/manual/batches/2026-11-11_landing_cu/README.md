# 2026-11-11 landing cu -- chunk Q12 (parked near misses, Opus 5.5): 1 row

vland --land: 1 win (RELOCATION_MATCH), test PASS. bindings_multi_check is at its NullCoord baseline.
- **0x00D91960 NScript::CQ_HobbeCaveScript::CHobbeRoaming::Main.**
  - The idle-loop ending from Q10 and Q11 shares its test only after a plain `while (sacrifice.IsAlive()) {...}` loop.
  - New opt-in FABLE_RETAIL_CQ_HOBBECAVESCRIPT_HOBBEROAMING.
  - vland's RELOC_ALIAS warning is the MSVCR71 `rand` import thunk.

**Headers**

Q12's overlay was 3-way merged with base d66afb340: all clean, default=same.
- New: CCreatureAction_MidAttackSide.h and NThingShot_CTCShotDefinitionNymphFun.h, used by differ rows only.
- Differ-row opt-ins: C3DVector DOT_ORDERED_YXZ, CRGBColour GETD3DCOLOR and CRenderManager2D DRAW2DLINE.

**Names**
- Queued as land1011cu: 0x00F35B10 is `?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z`. Landed bindings spell it
  RegisterFilter@CBanditCronies and two per-entity forms.
- Labels: ghidra_out/labels_landing_2026-11-11cu.tsv.

The chunk record is in pending/2026-11-11_linux_handQ12 (39 near misses). The closest are 0x008B6AD0 (one fxch), 0x00DD9520 (one
scheduling slot), 0x009DD960 and 0x007BC004. Those, the closest of Q8-Q11, and six fast-Normalise drafts make up the rework chunk Q13.
