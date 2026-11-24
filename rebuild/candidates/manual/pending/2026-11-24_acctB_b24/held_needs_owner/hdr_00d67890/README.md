# hdr/ overlay -- 0x00D67890 CQ_GuildTrainingWoodsWillScript::Main (agent B24A11, 2026-11-24)

| header | opt-in | what it adds | evidence |
|---|---|---|---|
| engine/CQ_GuildTrainingWoodsWillScript.h (NEW header: no repo header exists, any prefix) | none needed (new file; only NScript includers) | the level-script class (CLevelScriptBase<CQ_SunnyvaleMasterData>, flags +0x48..+0x4c, sizeof 0x50), its methods' declarations, the inline InitialiseVariables body, nested CWillWhisper (Alloc, ParentClass +0x14, PMasterData +0x18, sizeof 0x1c) | Ego_r.pdb struct_layouts_egor (CQ_GuildTrainingWoodsWillScript 100 bytes, CWillWhisper 28 bytes; retail = PDB - 0x14); FableWin.pdb names 0x03772C90..0x03775CB0; FableWin InitialiseVariables 0x03774DC0 (five stores, member order); reloc_bindings / functions.tsv spellings of 0x00D68720 / 0x00D68600 / 0x00D677D0 / 0x00D68C50 / 0x00D68CD0. Based on the chunk-845 draft header (pending/2026-11-07_linux_hand845/hdr), re-written, not copied. |
