# hdr/ overlay -- 0x00D04AE0 CAssassin1::Main (agent B24A11, 2026-11-24)

| header | opt-in macro | what it adds | evidence |
|---|---|---|---|
| engine/CQ_BanditCampBossBattleScript.h (today's repo header, full copy) | FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSIN1 | nested `class CAssassin1 : public CEntityScriptBase` (Alloc 0x00D04990, GetParentScript slot 3 0x00D04A30, Main slot 1 0x00D04AE0, Init slot 2 0x00D04A60, ParentClass +0x14, PMasterData +0x18) + `sizeof == 0x1c` assert. Additive: default branch unchanged. | Ego_r.pdb struct_layouts_egor CAssassin1 28 bytes (ParentClass +20, PMasterData +24); FableWin.pdb names (0x03691A10..0x03691C40); reloc_bindings / functions.tsv spellings of 0x00D04990 / 0x00D04A30 / 0x00D04A60; vtable 0x012C8B9C. Re-applied from the chunk-845 overlay (pending/2026-11-07_linux_hand845/hdr), not reused as a file. Independent of the staged ASSASSINMARKER opt-in (pending/2026-11-24_acctB_b24/hdr/00d11930): the two sections merge without overlap. |

Merge note: this ASSASSIN1 section and the staged ASSASSINMARKER section (pending/2026-11-24_acctB_b24/hdr/00d11930) are both inserted
at the same anchor (just before the BANDITKING section) and each adds a static assert at the end (ASSERT_AT 237 here, 224 there):
textually adjacent, so a merge must keep both blocks; they do not conflict in content.
