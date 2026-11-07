# Landing 2026-11-06w: chunk 822 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows, sharing the lane with the chunk 823 agent. Five rows were retries of earlier parks; one was new.
All names are proven. Both landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00DB86B0 | NScript::CQ_NewOakValeIntroScript::CNOVI_LiveFather::Main | RELOCATION_MATCH PASS (3 attempts; 91 lines before) |
| 0x00EBCB60 | NScript::CV_MayorsInvitationScript::MonitorParanoidQuest | RELOCATION_MATCH PASS (new, 176 bytes, first grade) |

- **CNOVI_LiveFather.** The final thread check is written at the end of both `IsXbox()` arms instead of once after the if / else.
  VC7.1 then cross-jumps the two into the single call retail makes, and the hint-loop returns share retail's pad.
- **MonitorParanoidQuest.** This is the thread function that CLadyGrey::Main (0x00EB9980) stores at 0x00EBAFEE. Its name is
  `?MonitorParanoidQuest@CV_MayorsInvitationScript@NScript@@QAEXXZ` in Ego_r.pdb and FableWin.pdb, and it is the emitted symbol.
  It was added to the manifest at landing `_s`.
- **Headers:** none changed; the agent's hdr/ holds only its README (`pending/.../hdr_README.md`).
- **vland:**
  - No new reloc binding.
  - One warning: RELOC_NAME on the std::map dtor placeholder 0x00CDBFB0, already queued.
- **Names:**
  - No labels file.
  - The agent's two NAME_FIXES are already queued: 0x00DAEBF0 GetVillagerSpeechIndex (`land0511w`) and 0x00EF7AB0
    `??1CIsThingConscious@NScript@@UAE@XZ` (`cx13`). Its label line asks to retire the leaf placeholder `CScriptThingFunctor_00ef7ab0`
    at 0x00EF7AB0, which `cx13` covers.
  - One more spelling bound there, `??1CIsThingAlive@N_00ef7ab0@@QAE@XZ` (caller 0x00EFB360), is queued as `land0611w`.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`).
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand822/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00D18DF0 NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main | DIFFER(3337v3321), 2 lines, 16 attempts | the hand-written `__forceinline` movie dtor adds a vptr store at both sites; left implicit, both sites call 0x006E7B80 (6 lines). `/d2inlS15` (diagnostic) expands both: a caller-size limit |
| 0x00DAF080 NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl::Main | DIFFER(5571v5504), 30 lines, 10 attempts | inside the expanded ~CIsThingConscious ours also expands the member ~CScriptThing where retail calls 0x004AA840. Matches under the diagnostic `/d2inlT60`, but not T72, T66 or T40 |
| 0x00DAC760 NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Main | DIFFER(5906v5867), 47 lines, 9 attempts | one extra expanded seh_me pad at the hit block's entry thread check (0x00DADB4D); proven to be the single site by a semantics-changing probe |
| 0x00DADF80 NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main | DIFFER(2790v2787), 50 lines, 9 attempts | the eight speech-list picks; `.size()` (retail's spelling) now grades 2846v2787, registers swapped, though chunk 797 measured it size-exact: a header moved since |
