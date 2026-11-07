# Landing 2026-11-07c: chunk 828 (1 of 6 rows landed, 5 parked)

One expert agent authored the six retries (names already proven), sharing the lane with the chunk 826 and 829 agents.

| Row | Grade | Note |
|---|---|---|
| 0x00E2C100 NScript::CV_AmbushScamScript::CAS_Trickster::Main | **RELOCATION_MATCH, landed** | the final wait loop written as `for (;; NSF) { if (T) return; ... }` gives retail's this-first order at the shared return pad 0x00E2E38E; first new attempt |
| 0x00D56790 NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main | DIFFER(6150v6151) | stack slots from stage 3 on (CScriptThing locals at esp+0x40 v 0xb0); stage-6 colouring; 12 attempts |
| 0x00E9E420 CBriarRose::Main | DIFFER(6035v5915) | ~CTimer / ~CScriptThing / movie destructor expanded per exit (blocker family); 12 attempts |
| 0x00D0C410 CBanditForger::Main | DIFFER(5255v5373) | frame 0xa0 v 0xa4; loop 1 rotated; 12 attempts |
| 0x006C6040 InformRelevantThingsOfDeed | DIFFER(2809v2377) | md 1026 -> 716 with a `switch`; the two copies do not merge (GetTC key slot, bit-test register); 10 attempts |
| 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update | DIFFER(2928v3080) | md 903 -> 823; frame 0x800 v 0x80c, HasTC bool slots; 10 attempts |

- **Bindings:** 6 new; bindings_multi_check shows only the known NullCoord.
- **Headers:** new opt-in `FABLE_RETAIL_CQ_GUILDTRAININGMELEESCRIPT_MELEEOPPONENT` (nested CMeleeOpponent, chunk 810 layout) is used only by
  the parked 0x00D56790; it stays in `pending/2026-11-07_linux_hand828/hdr/`.
- **Names:** LABELS.tsv empty. NAME_FIXES.tsv: 0x007E4040 (static CTCWillResponse member, bound as a free function) and 0x008B7F80
  (reference ctor, bound with pointer/long parameters) queued as `land0711c`; 0x008C33E0 was already queued (sec4b).
- Parked rows, tests and the agent's notes: `pending/2026-11-07_linux_hand828/`.
