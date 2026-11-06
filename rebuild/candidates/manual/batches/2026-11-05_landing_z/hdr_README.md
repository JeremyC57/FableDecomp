# chunk 799 header hand-back

All three headers live in hdr/engine/. Overlay: FABLE_HDR_OVERLAY=$A/hdr. The default path of every existing header is unchanged.

## engine/CPersonalScript_EmptyScript.h (existing header, opt-in added)
- Base: a fresh copy from rebuild/include/engine at edit time. The only change is a new block guarded by
  `#ifdef FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES` (about 411 lines). It completes the 27 nested entity classes that
  Main 0x00EF0090 binds: each gets its retail-offset members and a static Alloc, so `&CPersonalScript_EmptyScript::C<X>::Alloc`
  and the CEntityScriptBinding<T> instantiations compile.
- Evidence: Ego_r.pdb nested-class names and the Alloc mangled names (egor_pdb_names.tsv); the binding literals in Main's .rdata; each
  Alloc address is a landed row (the 30 CPersonalScript_EmptyScript::C*::Alloc rows).
- Default path (macro not defined): byte-for-byte the repo header.
- Includers rechecked with the overlay and the macro NOT defined (hdr/INCLUDERS_CPersonalScript_EmptyScript.txt, SIBLINGS.tsv): 7 landed
  rows, all still graded as landed. 00ef1690, 00ef71e0, 00ef7be0 and 00ef80a0 are RELOCATION_MATCH; 00ef3d00, 00ef24c0 and 00ef3de0 are MATCH.

## engine/CQ_HobbeToothContestScript.h (new)
- Level script, CLevelScriptBase-derived, size per Ego_r.pdb minus 0x14. Quest flags and members are at the retail offsets that Main
  0x00D9A190 reads (layout from struct_layouts_egor.tsv minus 0x14, checked against each [esi+off] access in the retail body).
  Methods: RegisterMain, Main and the spawned threads at their retail addresses.
- No prior includers (new file). Guarded with #error when FABLE_RETAIL_NSCRIPT is not defined, the same as sibling quest headers.

## engine/CQ_WhiteBalverineKnotholeGladeScript.h (new)
- Members: MissionFailed +0x48, MissionSucceeded +0x49, WifeCutsceneStart +0x4a, WifeCutsceneStop +0x4b, GreenWife +0x4c,
  BalverineState (long) +0x50, CScriptThing KG_Chief +0x54, QuestStartScreenShown +0x60; size 0x64. All are Ego_r.pdb offsets minus 0x14,
  and each is cross-checked against Main 0x00E13F10 and OnPersist 0x00E13EE0.
- Methods: RegisterMain 0x00E13E20, Main 0x00E13F10, Init, OnPersist 0x00E13EE0, WatchForTermination 0x00E15500.
- The six nested entity classes (CWB_Villager, CWB_WhiteBalverine, CWB_ScaredVillager, CWB_Guard1, CWB_Guard2, CKG_Chief) are
  forward-declared only. The row TU completes them locally with a static Alloc (FABLE_WBK_ENTITY macro in src/00e13f10.cpp) until their
  bodies are reconstructed.
- No prior includers (new file). Guarded with #error when FABLE_RETAIL_NSCRIPT is not defined.
