# Landing 2026-11-06c: chunk 802 (2 of 6 rows landed, 3 parked, 1 unattempted)

One expert agent authored the rows (sharing the lane with the chunk 801, then 803, agents). Both landed rows are plain C++ and grade
`genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x007ABBD0 | CTCAIScratchpad::OnSerialise | RELOCATION_MATCH PASS (the ledger's `vfunc_1`) |
| 0x008EACB0 | NCreatureMode::CModeHoverBase::OnActivate | RELOCATION_MATCH PASS (the anim set-up is the PDB's `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z`) |

- **Headers** (`hdr_README.md`): twelve additive opt-in branches:
  - CPersistContext, CTCCreatureOpinionOfHero, CThingCreatureBase, CThingPlayerCreature, CTCCreatureModeManager, C2DBoxF;
  - CRegionMinimap, CWorldMap, CTCTalk, NGraphicAppearance_CTCGraphicAppearance, CTCCombat, CThingPhysical.
  - hdrcheck2 (`logs/hdrcheck2.log`) reads `default=same` for all twelve.
  - DIFF macros:
    - three are new: CMODEBASE_VIRTUALS, CTCGRAPHICAPPEARANCE_SCALE, CPERSISTCONTEXT_NAMED_DEFAULTS;
    - one is existing: FABLE_RETAIL_CTCCOMBAT. CTCCombat.h split one member line to gate a `public:` behind the new
      CTCCOMBAT_SHOTOVERRIDE, and hdrcheck2 could not preprocess the header (`cpp-err`).
  - The coordinator rechecked the existing macro's 2 landed users against the new tree. 0x00667D30 and 0x007355F0 both still
    grade RELOCATION_MATCH (`logs/recheck_macro_users.log`).
- **vland warnings:** 2 RELOC_NAME:
  - 0x0099E4C0 `CCharString::operator_char_const*`, a label formatting difference only;
  - 0x0073A680, a placeholder label (queued).
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-06c.tsv`. It has the six rows, with 0x00684610 proven to be
    `CTCCreatureOpinionOfHero::OnSerialise` (not the ledger's COpinionReactionManager::TransferOpinionData), plus unlanded callees.
  - 12 queue lines (tag `land0611c`). Three targets now carry several bound spellings. The new row uses the one spelling both PDBs
    list:
    - CModeBase::OnActivate 0x0073AAC0 (4 spellings);
    - CModeBase::GetWorld 0x0073A680 (9 spellings);
    - CThingCreatureBase::GetRandomAnimFromName 0x00662D80 (3 spellings).
  - The queue lines ask the older rows to adopt that spelling.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand802/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00684610 CTCCreatureOpinionOfHero::OnSerialise | DIFFER(2826v2823), 8 attempts | 122 lines on the same instruction stream: string frame slots, two default temporaries retail builds once outside the loops, two helpers inline at some sites only |
| 0x0066CF00 CRegionMinimap::PopulateRegionMinimap | DIFFER(3252v3248), 6 attempts | ebx/ebp swap between x and the theme-def pointer; the def assignment keeps the object in a register where retail rereads it; 8 extra frame bytes |
| 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update | DIFFER(2680v3080), 5 attempts (first draft) | VC7.1 merges the two facing branches retail keeps apart; several unbound callees; four CTCPhysicsBase slot names known only by vtable offset |
| 0x00563920 CTCHeroOnlineScoreboard::DisplayScoreboardGUI | unattempted (budget) | its shape from the listing is in NOTES.md |
