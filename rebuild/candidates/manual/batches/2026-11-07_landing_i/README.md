# Landing 2026-11-07i: chunk 831 (2 of 6 rows landed, 4 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00689DF0 CInputProcessInventory::ProcessInput | **RELOCATION_MATCH + PASS, landed** | `// Flags: /GS`; same-TU `__declspec(noinline)` IS_THING_ALIVE template; keeps the landed IsEventButton spellings; 9 SHARED_HEADER_AVAILABLE warnings (local class models; header debt in agent_NOTES.md) |
| 0x00E0F8A0 NScript::CQ_WaspBossScript::CWaspHelper::Main | **RELOCATION_MATCH + PASS, landed** | was the placeholder `_global::CScriptGameResourceObjectScriptedThingBase_HandleWaspQueenHelper`; established CTimer forceinline / noinline members; header opt-in `FABLE_RETAIL_CQ_WASPBOSSSCRIPT_WASPHELPER` copied into rebuild/include (additive; its 3 landed includers recheck RELOCATION_MATCH) |
| 0x00D6D080 NScript::CQ_HangingTreeEvilScript::Introduction | DIFFER(1500v1516), 33 | resize(n) inline split (temporary's ctor/dtor called where the hero handle's expand) |
| 0x008DEC80 NCreatureMode::CModeJackOfBladesFlash::FrameUpdate | DIFFER(1647v1665), 108 | CIntelligentPointer::operator= called where retail expands (inline budget) |
| 0x008DBC80 NCreatureMode::CModeDragonFlying::FrameUpdate | DIFFER(2323v2319) | frame-slot order; x87 normalise order |
| 0x00873E10 NCameraModes::CPCMouseLook::Update | DIFFER(2384v2386) | x87 product order in the first dot product |

- **Labels:** the five placeholder rows got their proven names (`ghidra_out/labels_landing_2026-11-07i.tsv`, plus 0x008DCC80 Flash). The coordinator
  had been running apply_labels.py without file arguments today; the 2026-11-07e / g / i files were applied at this landing (7 manifest renames).
- **Names:** 12 queue lines (`land0711i`), incl. GFGetSignedAngleDifference 0x00A441E0, GetObservedThing 0x0084F1B0 (five spellings), 0x00633BE0
  (six spellings), the unresolved 0x004C10E0 (bound GetHeadPosition, wrong), the descriptive Jack-flash globals, and a decision line for the
  anonymous-namespace global 0x013BAFDC JACK_GLOW_START_FRAME (a per-file hash means a second TU cannot emit that symbol).
- **Headers:** the HangingTreeEvil `_INTRODUCTION` opt-in serves only the parked row; it stays in `pending/2026-11-07_linux_hand831/hdr/`.
