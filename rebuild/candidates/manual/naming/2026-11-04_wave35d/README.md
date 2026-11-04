# Naming pass wave 35d (2026-11-04): the 30 wave-35a rows deferred on misspelled source rows, plus 35b/35c follow-ups

One agent built an overlay in scratch (`scripts/`, the wave 35a generators). The coordinator applied it with
`../2026-11-04_wave35c/scripts/apply_w35c.py` and re-graded the result from the working tree.

## Result
- **35 rows respelled.** Re-grade from the working tree: **35 / 35 MATCH or RELOCATION_MATCH + PASS**
  (`logs/log0.txt`, `logs/log_00cdd2f0.txt`).
  - 24 of the 30 wave-35a D5 rows: 22 `??_G` scalar deleting destructors (`gen_sdd.py`), plus 00cdec30 OnPersist and 00dcdec0
    GetParentScript (`gen_ent.py`).
  - 2 constructors that the source rows needed: 0x00E2AB60 `NScript::CV_AmbushScamScript` (shared header) and 0x00ED0E60
    `NScript::CV_SingingStonesScript` (shares the TU of the landed destructor row 0x00ED39F0).
  - Follow-ups:
    - 00dcded0 CGuardTeamMember -> CCrateTeamMember.
    - Global 0x01375748 spelled `NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations` in 00e02950 00cfee30 00e569d0
      00ea0b10.
    - 00e9c300 and 00cdd2f0 now use `engine/CScriptBase.h` instead of the `CBase_00cb8110` stand-in. In 00cef9a0 and 00e0e7b0
      only the comments changed.
- **Coordinator fix:** 00cdd2f0 now includes the header, which brings inline functions into the object. Its source had no
  `// Leaf:` line, so the default grade compared a different function (DIFFER). The agent's harness named the leaf itself.
  The source now carries `// Leaf: ??0CVtable_012c2f50@@QAE@PAX@Z` and grades RELOCATION_MATCH + PASS.
- **Inherited warnings (pre-existing):** 00e0e7b0 `??1CVector_008ac970`, 00ea0b10 `g_pGameScriptInterface`.
- **D1 accepted:** 20 of the `??_G` rows drop the constructor TU's `virtual ~X();` declaration. Retail's `??_G` calls the base
  destructor directly, so the class's destructor is compiler-generated. About 20 wave-35b constructor/Alloc rows still declare a
  destructor retail lacks. That doesn't change their bytes; a later tidy can remove it.
- **Bindings:** +4 ADD (`??_7` and `??1` for CV_AmbushScamScript and CV_SingingStonesScript). `bindings_multi_check.py` prints 0.
  The RETIREs are not applied.

## Deferred (`DEFERRED.tsv`)
- 6 `??_G` rows (00cef9a0 00d1cbd0 00dc2540 00dfa080 00e0e7b0 00e2ab30): their constructors' class models lack the members whose
  destructors retail runs inline.
- 00cbd510 ~CScriptBase (D3): the shared header models the containers as opaque bytes, and retail's ~CScriptBase stores no vtable.
- `NAME_FIXES.tsv` (D4) lists:
  - 00e02950, 00ea0b10 and 00cfee30 are not NScript-qualified yet.
  - 00ed76e0 still binds `??0CScriptBaseNative`.
  - 12 `??_G` stand-ins still spell 0x00CBD510 `AutoTinyCompositeDestructor_00cbd510`.
