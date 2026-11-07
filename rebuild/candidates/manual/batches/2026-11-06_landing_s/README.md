# Landing 2026-11-06s: chunk 818 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows, sharing the lane with the chunk 817 and 819 agents. The chunk had two parts:
- retries of three chunk 816 near misses;
- three new entity Mains of 12.8-15.2 KB.

The agent proved every new name from vtable slot 1 (stored by the Alloc) and both PDB name sets. The landed row is plain C++ and
grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00EB9980 | NScript::CV_MayorsInvitationScript::CLadyGrey::Main | RELOCATION_MATCH PASS (13 attempts) |

- **Extent fixed at landing.** The chunk cut CLadyGrey at 12944 bytes. Its true length is 12753, ending at its `ret` at 0x00EBCB50.
  The coordinator had trusted jtext's `maxbranch`, which was a branch inside the next function (GOTCHAS coordinator line). The landing
  used the agent's trimmed oracle, and `rows.tsv` / chunk_history.tsv are corrected.
- **Manifest.** The bytes after it are a separate function nobody had catalogued: 0x00EBCB60
  `CV_MayorsInvitationScript::MonitorParanoidQuest`, 176 bytes, ending at the `ret` at 0x00EBCC0F before the Alloc 0x00EBCC10. The
  landed Main binds it in its PDB spelling. It was added to `rebuild/manifest/boundary-starts.tsv` and the manifest, so it is now an
  open row (known functions 57,614).
- **Headers:**
  - Only engine/CV_MayorsInvitationScript.h is copied in, with the additive opt-in `_LADYGREY` the landed row needs.
  - The rest of the overlay stays in `pending/2026-11-06_linux_hand818/hdr/`: the EscortTrader, ChickenKicking and ScriptedThingBase
    `FORCEINLINE_ASSIGN` opt-ins, plus chunk 816's BanditToll and GuildTrainingSkill headers.
  - The agent checked 360 landed includers with g++ -E (identical output).
- **vland:** RELOC_NAME on the three STLport map ctor/dtor placeholders (already queued many times; not re-queued).
- **Names:**
  - Labels: `ghidra_out/labels_landing_2026-11-06s.tsv` holds the three new Mains, MonitorParanoidQuest and 0x00E2E400
    `CV_AmbushScamScript::ResetRandomSpeechTime`. That one was in the manifest only; the Trickster Main binds it.
  - Two catalog renames are queued under tag `land0611s`:
    - 0x00E1FEB0 is `CQR_EscortTraderScript::ResetTraderRandomSpeechTime`. The catalog has `CV_AmbushScamScript::ResetRandomSpeechTime`, and its callers bind `RestartRandomSpeechTimer`.
    - 0x00E2E440 is `CAS_Trickster::GetRandomSpeech`. The catalog has `CAC_Owner::GetRandomSpeech`, a label propagated from a byte-identical body.
  - The chunk 816 TollBandit fixes were already queued.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`) and the labels are applied.
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.
  - The one new binding, MonitorParanoidQuest, has no second address and no second symbol.

## Parked (`pending/2026-11-06_linux_hand818/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00E2C100 NScript::CV_AmbushScamScript::CAS_Trickster::Main | DIFFER(8945v8945), 2 lines, 22 attempts over chunks 816 and 818 | one push / mov order swap in the last movie exit pad, 0x00E2E38E |
| 0x00E64FB0 NScript::CV_ChickenKickingScript::CChickenMaster::Main | DIFFER(15210v15210), 16 lines, 10 attempts | register choice only: eax and edx swapped at a vptr load (0x00E6716A, 0x00E671E0). Retried in chunk 820 |
| 0x00D5AE70 NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main | DIFFER(11636v11636), 235 lines on this grader, 24 attempts | stack slots of the scalars and temporaries at esp+0x44..0x4C |
| 0x00E4B5B0 NScript::CV_BanditTollScript::CTollBandit::Main | DIFFER(10631v10594), 923 lines | the frame now matches (0x118), using a sibling block for the earlier CTimer. Left: the dtor-flag and temporary slot order, one extra ~CTimer call in retail, one exit-pad order |
| 0x00E20600 NScript::CQR_EscortTraderScript::CTraderToEscort::Main | DIFFER(14494v14510), 681 lines, 11 attempts | blocker 1 (CIsThingAlive copy); first exit-pad placement; per-site inline splits for GetPos, IsOpenDoor and one movie / CTimer pad |
