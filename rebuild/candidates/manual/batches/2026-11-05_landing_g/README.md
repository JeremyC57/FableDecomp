# Landing 2026-11-05g: chunk 780 (1 of 5 NScript ledger rows)

Authored by one expert agent. The row is plain C++ (`/Gr`) and grades `genuine`.

| Address | Name | Grade | Key spelling |
|---|---|---|---|
| 0x00DECAD0 | NScript::CQ_SecretPassageScript::Main | RELOCATION_MATCH PASS | the ledger called it CV_TravellingHeroesScript::Main; the PDB name sets, the ten bindings' literals and the spawns prove CQ_SecretPassageScript |

- **Headers** (`hdr_README.md`): two opt-ins, both `default=same` after merging onto chunk 779's fable_string.h:
  FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE (in-class `operator==(const T*)` = `Compare(...) == 0`, retail's
  `call Compare; neg; sbb; inc`) and FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS (RelationCounters 0x0143E928,
  RelationSuffixes 0x0143E934). Neither macro has a landed user, so no includer recheck was needed.
- **Landing:** `vland --land` (`logs/land.log`). Labels `ghidra_out/labels_landing_2026-11-05g.tsv` (7; landed rows whose names
  the agent corrected are queued instead, per GOTCHAS); manifest rows added for CDF_SummonsCreatures::Alloc 0x00E702C0 and
  GetParentScript 0x00E70360; `bindings_multi_check.py` 0; `sync_manifest_from_catalog.py --dry-run` 0.

## Parked (`pending/2026-11-05_linux_hand780/`, each with its residue and next idea)
- **0x00E7C010 CV_FishermanScript::NPCAttitudeProcess:** DIFFER(1624v1623), 12 spellings. Retail's two prelude terminate checks and
  the hit-loop bottom check share one inline ~CScriptThing exit block; VC7.1 never reproduced the sharing.
- **0x00EA2080 CV_KnotholeGladeGatesScript::ControlMainBalverines:** DIFFER(2279v1682), 14 spellings. The two arm-first terminate
  checks are inline dtor copies in ours and cold-tail jumps in retail.
- **0x00E4FA20 CV_BeardyBaldyScript::Main:** DIFFER(1763v1756), 27 spellings. The best variant differs only by a lazy `push edi`
  after the first terminate check; every form that restores the entry push cools the barber IsAlive site to a call.
- **0x00E6FC40 CDF_Dragon::Main: not attempted.** The 1828-byte row is Main, its 8-entry jump table 0x00E702A0,
  CDF_SummonsCreatures::Alloc 0x00E702C0 and GetParentScript 0x00E70360. It needs a split before it can be graded. The agent's
  full decode of Main is in `agent_NOTES.md`.

## Names
5 queue lines (tag `land0511g`): 0x00DECAD0 Main, 0x00DED0A0 = CQ_SecretPassageScript::InitialiseVariables (landed as
CQ_GuildTrainingScript::CMeleeApprentice::Init), 0x00E50100 = CV_BeardyBaldyScript::WatchForQuestFinished (landed under
CV_AmbushScamScript), 0x00E6FC40's scope, and a one-source PROPOSAL that 0x00EA3490 belongs to CV_LostTraderScript.
The Lua ports this brief called missing are in the repo after all: `refs/script_recovery/seed_corpus/sources/<Quest>/FSE/`.
