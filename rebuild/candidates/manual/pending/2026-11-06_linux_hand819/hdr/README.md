# Chunk 819 header overlay

There are two headers. Both are opt-in or new, so the default preprocessed text of every existing includer is unchanged.

## engine/CQ_GuildTrainingWillScript.h (NEW)
- **Path:** the whole body sits behind `#ifdef FABLE_RETAIL_NSCRIPT`. There is no `#else` branch and no `#error`.
- **Contents:** `NScript::CQ_GuildTrainingWillScript : CLevelScriptBase<CQ_SunnyvaleMasterData>` and its nested `CTheRealGuildmaster : CEntityScriptBase`.
- **Script members:** TargetsHit +0x48, TutorialState +0x4c, TotalTrainingDummiesCounter +0x50, GenericTutorialCounter +0x54, HeroHit +0x58, TestFinished +0x5c, BanditsDefeated +0x5d, DummyWizardsVector +0x60 (an STLport vector of CScriptThing). sizeof is 0x6c.
- **Nested-class members:** ParentClass& +0x14, PMasterData +0x18. sizeof is 0x1c.
- **Layout evidence (two sources):**
  - Ego_r.pdb `struct_layouts_egor.tsv` gives CQ_GuildTrainingWillScript as 132 bytes. Retail offsets are the PDB offset minus 0x14 (retail CLevelScriptBase is 0x48), then minus 4 for the STLport vector.
  - The retail Alloc 0x00D61BB0 allocates 0x6C bytes, and the landed ctor 0x00D5DC50 builds that object.
  - CTheRealGuildmaster::Main 0x00D5E0C0 reads TutorialState +0x4c, TestFinished +0x5c and BanditsDefeated +0x5d.
  - The landed CTheRealGuildmaster::Alloc 0x00D5DDE0 allocates 0x1C bytes and stores entity vtable 0x012D1070. Its slots are 0 scalar deleting dtor 0x00D5DEA0, 1 Main 0x00D5E0C0, 2 Init 0x00D5DE90, 3 GetParentScript 0x00D5DE80.
- **Spellings:** Ego_r.pdb and FableWin.pdb, `?Main@CTheRealGuildmaster@CQ_GuildTrainingWillScript@NScript@@EAEXXZ` (private virtual). GetParentScript is UBE and Alloc is SI.
- **Includers:** none landed. The 16 landed CQ_GuildTrainingWillScript rows keep their local models (SIBLINGS.tsv). The only user is this chunk's parked row 0x00D5E0C0 (differ/00d5e0c0.cpp).

## engine/CQ_BanditCampBossBattleScript.h (chunk 812 opt-ins, carried unchanged)
- **Source:** a byte-identical copy of `$S/w22/agent812/hdr/engine/CQ_BanditCampBossBattleScript.h`, which chunk 812 parked as a pending header. It is the repo copy plus two additive blocks, `#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GATE2GUARD1` and `#ifdef ..._CAMPHOSTAGEGUARD`:
  - the nested classes CGate2Guard1 / CCampHostageGuard;
  - OpenGate(float, CCharString) 0x00D0EBC0.
- **Diff checked:** `diff rebuild/include/engine/CQ_BanditCampBossBattleScript.h hdr/...` shows only those two added blocks. The repo copy has not changed since chunk 812.
- **Users:** only the two parked retries 0x00D0D910 / 0x00D09010 define the macros.
- **Landed includers, regraded read-only with this overlay:**
  - 0x00D038E0 (direct): RELOCATION_MATCH.
  - 0x00D2AA40 (through engine/CQ_EndGameFocalSitesScript.h): RELOCATION_MATCH.
- **Not touched:** chunk 818's headers (CV_AmbushScamScript, CQ_GuildTrainingSkillScript, CV_BanditTollScript, CV_MayorsInvitationScript, CQR_EscortTrader, CV_ChickenKicking, CScriptGameResourceObjectScriptedThingBase).
