# Chunk 828 hdr/ hand-back

Copied fresh from rebuild/include and re-diffed against it before hand-back (plain `diff`: additions only, no existing line touched
or reflowed). Opt-in only, so no landed includer can change bytes (no landed source defines the new macro; the one landed includer,
CTheRealGuildmaster::Main 0x00D58490, does not define it). Includers rechecked: none needed.

| header | change | evidence | path |
|---|---|---|---|
| engine/CQ_GuildTrainingMeleeScript.h | adds the nested `CMeleeOpponent : CEntityScriptBase` definition after the class (the repo already forward-declares it inside the class): ctor, Alloc 0x00D566A0, GetParentScript 0x00D56740, Init 0x00D56750, Main 0x00D56790; ParentClass* +0x14, PMasterData +0x18, BadHit +0x1c, RepeatMelee +0x20; sizeof 0x24 (static assert) | Ego_r.pdb layout (struct_layouts_egor.tsv, retail = PDB - 0x14); the landed Alloc 0x00D566A0 news 0x24 and stores vtable 0x012D0328 (slots: 0 ??_E 0x00D56770, 1 Main, 2 Init, 3 GetParentScript); both PDB name sets hold ?<Name>@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@; Main reads ParentClass at [esi+0x14]. Drafted by chunk 810 (pending/2026-11-06_linux_hand810/hdr), whose copy predates the repo's chunk 815 header | opt-in `FABLE_RETAIL_CQ_GUILDTRAININGMELEESCRIPT_MELEEOPPONENT`, one block before the final `#endif // FABLE_RETAIL_NSCRIPT` |

Only the parked row differ/00d56790.cpp uses it (it SRC_FAILs against the repo header, which has no CMeleeOpponent body).
