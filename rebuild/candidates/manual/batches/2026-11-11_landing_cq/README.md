# 2026-11-11 landing cq -- chunk Q10 (parked near misses, Opus 5.5): 4 rows

vland --land: 4 wins (RELOCATION_MATCH), every test PASS. bindings_multi_check is at its NullCoord baseline.
- **0x0078F8F0 CTCDivineWrath::CancelForCutscene.** Written like its landed sibling CTCUnholyPower::CancelForCutscene 0x00794E60.
  New opt-in FABLE_RETAIL_CTCDIVINEWRATH_STOPSOUND_INLINE.
- **0x00D43190 NScript::CQ_GuildTrainingScript::CKillBird::Main.**
  - The old draft had the wrong identity (CPretendGuard). Proven by the registration "KillBird" 0x00D3C493, Alloc 0x00D43040, vtable
    0x012CE32C slot 1, and both PDBs.
  - Spelled like 0x00CFCCA0. New opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_KILLBIRD.
- **0x00E6AD20 CV_ChapelOfEvilScript::WatchForAcolyteDeath and 0x00EDC980 CV_TempleOfLightScript::WatchForAcolyteDeath.**
  - Byte-twins of each other. Both use the device of the landed 0x00D16570: a row-local `FABLE_FORCEINLINE ~CScriptThing` and scoped
    `#pragma inline_depth(0)`.
  - Both carry 0x00D16570's `// Revisit: inline-depth` marker, which the coordinator added (comment only).

**Headers**

hdrcheck2 reports default=same, and both headers are line-supersets of the repo copy: CTCDivineWrath.h and CQ_GuildTrainingScript.h.

**Names**
- Labels: ghidra_out/labels_landing_2026-11-11cq.tsv.
- No name fixes.

The chunk record is in pending/2026-11-11_linux_handQ10 (36 near misses). The closest:
- 0x00E4AA00 CBanditLeader::Main, 1356v1356: one register (eax v ecx at 0x00E4AE44).
- 0x00ED9BB0: an entry flag store.
- 0x00666310: a memory-operand `and`.
- 0x00435190: needs the GetDef<CEngineDef> body in the TU.

Q10 notes that 35 of its 40 drafts were local models. Moving rows onto the shared headers is now the main work in this pool.
