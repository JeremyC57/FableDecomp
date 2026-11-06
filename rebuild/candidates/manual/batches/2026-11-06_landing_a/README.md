# Landing 2026-11-06a: chunk 800 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows (sharing the lane with the chunk 799, then 801, agents). Both landed rows are plain C++ and
grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D3BC60 | NScript::CQ_GuildTrainingScript::Main | RELOCATION_MATCH PASS (3 grades) |
| 0x00544D20 | NUISystem::CSpellContainerList::ProcessEvent | RELOCATION_MATCH PASS (4 grades; the shared NUISystem headers took it from 297 diff lines to 38, and the switch case order fixed the block order) |

- **Headers** (`hdr_README.md`):
  - new engine/CQ_EndGameFocalSitesScript.h (0x94, nested CEndGameScythe 0x1C);
  - opt-in branches: CQ_BanditCampBossBattleScript.h `_BANDITKING`, CQ_GuildTrainingScript.h `_MAIN`, CSubtitleRenderer.h `_CLEAR`,
    NUISystem_CSpellContainer.h `CSPELLCONTAINER_VIRTUALS` (slots 142-148), CScriptGameResourceObjectMovieBase.h `_FORCEINLINE_CTOR`.
  - The overlay was additive against the current tree. hdrcheck2 reads `default=same` where it can preprocess; for
    CQ_BanditCampBossBattleScript.h and CScriptGameResourceObjectMovieBase.h it could not (`same[0](cpp-err)`), so the coordinator
    read those two diffs: every added line is inside a new macro's `#if`. The agent regraded all 104 includers with the final overlay:
    every one MATCH / RELOCATION_MATCH + PASS (`pending/2026-11-06_linux_hand800/SIBLINGS.tsv`).
- **vland warnings:** 2 RELOC_CONFLICT + 2 RELOC_NAME, one finding. 0x00544D20 binds `??1?$CCountedPointer@VCAction@NUISystem@@@@QAE@XZ`
  and the CComponent dtor to the 5-byte stubs 0x00429B72 / 0x00429C53 (`jmp 0x00429160` / `jmp 0x004291DE`). The landed 0x0056F615 /
  0x00596A68 bind the same symbols to the 44-byte bodies.
  - engine/CCountedPointer.h's 955-row model makes the body the dtor. Only NUISystem UI code calls the stubs (12 and 26 call sites).
  - So the stub is most likely another function that tail-calls the dtor; the new row's spelling is the suspect one.
  - The row stays landed (its bytes are genuine). Two CHECK lines are queued (`land0611a`) to find the stub's PDB identity by body
    match, then respell the row's two declared-only specializations.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-06a.tsv` (the six rows, the CSpellContainerList slot functions 0x005453D0 /
    0x005455A0 / 0x005455B0, the GuildTraining entity Allocs).
  - 12 queue lines (tag `land0611a`): SetAsPerformAction 0x00860130, CSubtitleRenderer::Clear 0x00492BAB (two rows spell it ClearText),
    SetSpellToAssign 0x00544100, SetEmptySlotDef 0x00546060, GetBaseSpells 0x00546070, and the two stub CHECKs.
  - 0x0042B646 now carries three bound spellings; the new row uses the PDB one
    (`list<CActionParam*>::push_back`), which queue line `land0311f` already asks the others to adopt.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0.

## Parked (`pending/2026-11-06_linux_hand800/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00D2ED20 NScript::CQ_EndGameFocalSitesScript::CEndGameScythe::Main | DIFFER(2255v2248), 12 attempts | 14 lines: retail expands the implicit CScriptGameResourceObjectMovieBase dtor per site (`call 0x006E7AB0`, a GROBase vtable store, `call 0x0099A430`); ours calls the COMDAT. The same gap as chunk 797's 0x00D18DF0 |
| 0x00D0A830 NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main | DIFFER(2700v2720), 8 attempts | 22 lines, the same MovieBase dtor gap (stopped under 10 attempts on purpose: the identical gap took 14 grades in chunk 797) |
| 0x007D8E20 CTCTavernGameSpotTheAddition::FrameUpdate | DIFFER(1900v1900), 18 attempts | 22 lines: one stack-frame slot assignment |
| 0x0094AB00 CAIStateGroup_GoToPub::Update | DIFFER(1968v2067), 7 attempts | inline budget: retail expands the list push_back and one LowerBound that ours keeps out of line |
