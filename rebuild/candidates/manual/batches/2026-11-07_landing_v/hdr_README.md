# Chunk 845 header overlay

No default path changed: every edit of an existing repo header is an opt-in branch; the other five files are new headers.

- engine/CQ_BanditCampBossBattleScript.h (repo copy, fresh at hand-back): opt-in FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSIN1
  adds the nested CAssassin1 (Ego_r.pdb 28 bytes: ParentClass +0x14, PMasterData +0x18; vtable 0x012C8B9C slots: dtor 0x00D04A40,
  Main 0x00D04AE0, Init 0x00D04A60, GetParentScript 0x00D04A30; Alloc 0x00D04990) and its size assert. Includers rechecked with the
  overlay: 0x00D038E0, 0x00D2AA40 (through CQ_EndGameFocalSitesScript.h) RELOCATION_MATCH.
- engine/CScriptBase.h (repo copy, fresh at hand-back): opt-in FABLE_RETAIL_CSCRIPTBASE_ADDSPAWNEDFUNCTION_DEFAULT_REGION wraps the
  existing AddSpawnedFunction line in #ifdef/#else/#endif and declares it with the default region argument `CCharString("")`
  (evidence: retail 0x00ED8680 builds the "" temporary after the new-expression and destroys it before the name temporaries;
  Ego_d.pdb has no SpawnFunction-style wrapper). Rows must include fable_string.h first. 1085 includers keep the textually
  identical default path; sample rechecked: 0x00ED9790, 0x00ED7790 RELOCATION_MATCH. Used by differ/00ed8680.cpp only.
- New: engine/CQ_GuildTrainingWoodsWillScript.h, engine/CV_BeggarAndChildScript.h (used by the matched src/00e57e60.cpp),
  engine/CV_BookCollectingScript.h, engine/CV_HeroDollsScript.h, engine/CV_TalentlessBardScript.h -- Ego_r.pdb layouts at retail
  offsets (PDB - 0x14, minus 4 after each STLport vector), method addresses from the calls, evidence in each file's comment.
