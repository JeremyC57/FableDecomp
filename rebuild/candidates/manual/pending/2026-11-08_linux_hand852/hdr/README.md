# chunk 852 header overlay

Both files are fresh copies of `rebuild/include/engine/` (diffed again at hand-back: the repo copies are unchanged, the overlay only
adds lines). Every edit is an opt-in `#ifdef` branch; the default path is textually unchanged.

- `engine/CQ_BanditCampBossBattleScript.h`
  - `FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_HOSTAGE`: declares the entity script `CCampHostage : CEntityScriptBase`
    (Ego_r 28 bytes): Alloc 0x00D07D20, GetParentScript slot 3 0x00D07DC0, private Main slot 1 0x00D07E50 / Init slot 2 0x00D07DD0,
    `ParentClass` +0x14, `PMasterData` +0x18 (vtable 0x012C8F8C), plus a sizeof assert (0x1c).
  - `FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GAMEMASTER`: declares `CBCGameMaster : CEntityScriptBase` (Ego_r 36 bytes, the
    retail new size in Alloc 0x00D066B0): GetParentScript 0x00D06750, private Main 0x00D067A0 / Init 0x00D06760 / OnPersist slot 4
    0x00D0EE10 / OnPredicateFail slot 5 0x00D06770, `AIState` +0x1c, `GivenPass` +0x20 (vtable 0x012C8DE4), plus a sizeof assert (0x24).
  - Used by the parked rows 0x00D07E50 and 0x00D067A0.
- `engine/C3DVector.h`
  - `FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE`: declares `float GetMagnitudeAndNormalise();` (0x00A14480,
    ?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ, already bound at that address in reloc_bindings.tsv). Used by the parked row
    0x00981410. Landed callers (e.g. 0x008444A0) still model it locally; nothing else changes.

Siblings graded with this overlay: 0x00D038E0 (CQ_BanditCampBossBattleScript::Main, the only landed includer of the bandit-camp
header) and 0x00980770 (a C3DVector.h includer in the AIAction family) both stay RELOCATION_MATCH.
