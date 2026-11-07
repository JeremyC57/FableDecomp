# Landing 2026-11-06p: chunk 815 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows. It shared the lane with the chunk 814 and 816 agents. The six rows were unnamed gap-scan entity Mains
of 9.2-9.9 KB, the largest so far. The agent proved every name from vtable slot 1 (stored by the row's Alloc or ctor) and from both
PDB name sets. The landed row is plain C++ and grades `genuine`. The batch keeps the `2026-11-06` series name, although it landed on
2026-11-07.

| Address | Name | Grade |
|---|---|---|
| 0x00D58490 | NScript::CQ_GuildTrainingMeleeScript::CTheRealGuildmaster::Main | RELOCATION_MATCH PASS. It ends in a 7-entry jump table. A row-local `inline CScriptThing::~CScriptThing() { }` reproduced retail's split of first temporary expanded, later ones called (534 lines down to 64). The end healths are named locals before the x87 score expression, and the hit test is written `(A && B) \|\| MsgIsHitBy(...)` |

- **Headers:** only the new engine/CQ_GuildTrainingMeleeScript.h is copied in. It sits behind `FABLE_RETAIL_NSCRIPT` with no
  `#error` branch, and the landed row needs it. It holds the owner class (sizeof 0x58) and the nested CTheRealGuildmaster (sizeof 0x20).
  No landed row included a header of that name before.
- **Overlay kept in `pending/2026-11-06_linux_hand815/hdr/`:**
  - the new engine/CV_BeardyBaldyScript.h;
  - additive opt-ins in CQ_TraderConflictGoodScript, CV_ArcheryCompetitionScript, CV_BodyGuardScript and CQ_HobbeCaveScript. All 12
    landed includers of those four headers were rechecked with the overlay and all are MATCH / RELOCATION_MATCH + PASS.
- **vland:**
  - Three RELOC_NAME warnings on STLport map ctor/dtor placeholder labels (0x00CDBFB0, 0x009AC2D0, 0x009AC310).
  - One new binding, AddLogbookTutorialEntryPC 0x00CBEA81. It matches the PDB spelling and its landed catalog row, and is bound at no
    other address.
- **Names:**
  - Labels: `ghidra_out/labels_landing_2026-11-06p.tsv` holds the six Mains plus two unlanded callees:
    - 0x00E621F0 `CV_BodyGuardScript::CBodyGuard::SetHenchmanAsHired`;
    - 0x00E36750 `CV_ArcheryCompetitionScript::DoCountDown`.
  - Four renames are queued under tag `land0611p`:
    - 0x00E53CB0 is `CBB_BeardyBaldyMan::GetRandomSpeech` (catalog `CAC_Owner::GetRandomSpeech`).
    - 0x00E362B0 `CAC_Owner::GetRandomSpeech` lacks its `NScript::CV_ArcheryCompetitionScript` scope.
    - 0x00E53670 is `IsHeroWearingAnyBeard` (catalog `IsHeroWearingBeard`).
    - Row 00e33720 binds 0x00E337C0 as `CQ_AmbushTradersScript::ResetRandomSpeechTime`; it is `CV_ArcheryCompetitionScript::ResetRandomSpeechTime`.
  - FadeOutAndKillEntity `_NM1` and the six pGlobalScriptDef spellings were already queued, so they were not queued again.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`) and the labels are applied.
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand815/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DFE0F0 NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Main | DIFFER(9501v9501), 10 attempts, 59 lines | size exact. Retail cross-jumps the whole TRADERB body into the TRADERC body, while ours keeps B's own thread check plus 3 alignment pads. Retried in chunk 817 |
| 0x00E50FD0 NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main | DIFFER(10006v9885), 7 attempts, 521 lines | blocker 1: the outer by-value copy is expanded, but the Par1 copy inside the CSpawnedFunc1 ctor is called (0x004ABE90) |
| 0x00E33EA0 NScript::CV_ArcheryCompetitionScript::CAC_Owner::Main | DIFFER(9303v9228), 5 attempts, 648 lines | blocker 1 twice (CIsThingAlive and CSpawnedFunc1), and a frame 4 bytes larger |
| 0x00E5FD40 NScript::CV_BodyGuardScript::CBodyGuard::Main | DIFFER(9535v9381), 4 attempts, 887 lines | retail expands ~CScriptThing for 2 of 8 temporaries; ours expands 4 with the inline dtor and 0 without. The return tails are merged differently |
| 0x00D973D0 NScript::CQ_HobbeCaveScript::CBanditSacrifice::Main | DIFFER(9883v9821), 3 attempts, 2115 lines | the same 0xC8 frame, but locals sit in different slots, which shifts most lines; blocker 1 as well |

**Blocker 1 (the CScriptThing copy called inside an expanded predicate or CSpawnedFunc1 ctor)** is still open. These spellings were
tried and none moved it:
- an explicit row-local inline copy ctor;
- a plain-inline CSpawnedFunc1 ctor;
- the copy defined after Main, or after the CIsThingAlive ctor.
