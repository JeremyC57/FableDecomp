# Chunk 805 header overlay

Four repo headers, each copied fresh from `rebuild/include/engine/` and given one opt-in branch. The default path of every
header is textually unchanged (only added `#ifdef` blocks; the one replaced line in CQ_HangingTreeGoodScript.h is kept
verbatim in the `#else` arm). Diffed against the repo copies before hand-back: no repo change since the copy.

| Header | Opt-in macro | What it adds | Evidence |
|---|---|---|---|
| engine/CQ_GuildTrainingScript.h | FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_ENTITIES (with ..._MAIN) | the nested entities CAppleGirl and CSkillTarget (0x28 each: ParentClass +0x14, PMasterData +0x18; AppleMode / CurrentApples / ChildAppleMode / HaveChatted and Speed / DummyNumber / DummyWorth) | Ego_r.pdb layouts (struct_layouts_egor.tsv); landed Allocs 0x00D3D060 / 0x00D41BF0 allocate 0x28; vtables 0x012CD7E8 / 0x012CE0CC slot order; PDB access (Main/Init private) |
| engine/CV_LostTraderScript.h | FABLE_RETAIL_CV_LOSTTRADERSCRIPT_TRADER | CLostTraderTrader's private helpers TraderYellForHelp 0x00EA4980, TraderYellAtHero 0x00EA4BB0, MakeRandomComment 0x00EA4DE0 | Ego_r.pdb ?...@CLostTraderTrader@CV_LostTraderScript@NScript@@AAEXXZ; labels_chunk692 (string and donor-order evidence); call sites in 0x00EA36B0 |
| engine/CQ_EndGameFocalSitesScript.h | FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_JACKOFBLADES | the nested entity CEndGameJackOfBlades (0x1C) | Ego_r.pdb layout; Alloc 0x00D2C260 allocates 0x1C; vtable 0x012CC2E8 (0 0x00D2C320, 1 Main 0x00D2D930, 2 Init 0x00D2C310, 3 GetParentScript 0x00D2C300) |
| engine/CQ_HangingTreeGoodScript.h | FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT_GUARD (with ..._HANGINGTREEGOODSCRIPT) | the full CHTG_Guard entity (0x1C) in place of the stub nested declaration | Ego_r.pdb layout; Alloc 0x00D70520 allocates 0x1C; vtable 0x012D27A8 (0 0x00D706C0, 1 Main 0x00D706E0, 2 Init 0x00D705D0, 3 GetParentScript 0x00D705C0, 5 OnPredicateFail 0x00D70650, 6 OnInterrupted 0x00D706B0) |

Users: src/00d41d00.cpp (GuildTraining ENTITIES), src/00ea36b0.cpp (LOSTTRADER_TRADER), differ/00d3d150.cpp (GuildTraining
ENTITIES), differ/00d706e0.cpp (HANGINGTREEGOOD_GUARD), differ/00d2d930.cpp (ENDGAMEFOCALSITES_JACKOFBLADES).
differ/00ec9d30.cpp uses no overlay header.

Includers rechecked read-only with this overlay (`recheck.py` + `vland.py --recheck-landed`, LANDVERIFY_WORK under agent805,
deleted after): all 13 landed includers of the four headers MATCH / RELOCATION_MATCH + PASS (SIBLINGS.tsv).
CQ_EndGameFocalSitesScript.h has no landed includer. Not required for opt-in edits; done as a check.
