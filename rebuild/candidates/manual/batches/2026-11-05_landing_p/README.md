# Landing 2026-11-05p: chunk 789 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows. Both landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00E776F0 | NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager::Main | RELOCATION_MATCH PASS (6 grades) |
| 0x00EB8900 | NScript::CV_MayorsInvitationScript::CMI_InfoKeeper::Main | RELOCATION_MATCH PASS (30 grades; line-2 `// Inline-split:` note) |

- **Key spellings:**
  - 0x00E776F0: the seh-handle IsValid / Release go through a local `CV_ExposeMayorScript& parent = ParentClass;`.
  - 0x00EB8900: an early `if (HeroLearnedSecret) return;`, plus a `do {} while (0)` around one terminate check. That is a
    single-site inline lever, recorded with the `// Inline-split:` note as INLINE_DEPTH_REVISIT.md requires.
- **Headers** (`hdr_README.md`). Default paths are unchanged.
  - New: CV_ExposeMayorScript.h, CV_MayorsInvitationScript.h, CQ_HeroSoulsBriarScript.h, CHeroSoulsCreatureDef.h (0x34),
    CHeroSoulsWaveDef.h (0x38).
  - CQ_AwakeningTheOracleScript.h gains the opt-in FABLE_RETAIL_CQ_AWAKENINGTHEORACLESCRIPT_CORACLETABLET for the nested
    COracleTablet. Its one includer, 0x00CFB080, was rechecked.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05p.tsv`.
  - 2 queue lines (tag `land0511p`): 0x00CFEE30 is COracleTablet::RemoveCursedTablets in its full scope, and 0x00704580 is
    CCountedPointer<CScriptThing>::operator= (landed as a placeholder).
- **Landing:** `vland --land` (`logs/land.log`).

## Parked (`pending/2026-11-05_linux_hand789/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00CFE030 CQ_AwakeningTheOracleScript::COracleTablet::Main | DIFFER(3511v3575), 23 lines | Retail expands ~CScriptThing at exactly one shared exit |
| 0x00D82640 CQ_HeroSoulsBriarScript::CBriarRose::Main | DIFFER(3286v3260), 38 lines | The CIsThingAlive copy-inline case (chunk 786 family) and a duplicated idle-loop test |
| 0x00DEA3F0 CQ_RansomVictimScript::CVictimsBrother::Main | DIFFER(3056v3076), 161 lines | Frame slot order and first-loop exit stub placement |
| 0x00D7FE90 CQ_HeroSoulsArenaScript::WatchForRoundsInArena | DIFFER(2105v3148), 398 lines | VC7.1's per-site inline split of ~vector<CScriptThing> 0x008AC970 |
