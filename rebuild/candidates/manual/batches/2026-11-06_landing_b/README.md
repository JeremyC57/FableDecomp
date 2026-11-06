# Landing 2026-11-06b: chunk 801 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows (sharing the lane with the chunk 800, then 802, agents). The landed row is plain C++ and grades
`genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x007B0CD0 | CTCFishing::FrameUpdate | RELOCATION_MATCH PASS (the frame rate read is the header's `CMainGameComponent::GetConstantFPS()`, `?ConstantFPS@CMainGameComponent@@1JA`) |

- **Headers** (`hdr_README.md`), all opt-in and used only by parked rows; landed now so a retry starts from the repo:
  - `engine/CThingAICreature.h` FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER (slot 36 DrawPreparePrimitivesForRendering, DebugRenderHandles
    +0x194, PBrain +0x1A8);
  - `engine/CAIBrain.h` FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT (the two state-group text accessors);
  - `engine/CIntelligentPointer.h` FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE;
  - `stlport/fable_containers.h` FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE and FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT.
  - hdrcheck2 read `default=DIFF` for CAIBrain.h: the overlay added an unguarded `class CCharString;` forward declaration. The
    coordinator moved it inside the opt-in `#ifdef`; hdrcheck2 then reads `default=same` for every header, and only the new macros
    read DIFF. No landed source defines any of the new macros.
- **vland warnings:** 9 SHARED_HEADER_AVAILABLE. The row models CMainGameComponent, CJoystick, CInputManager, CWorld, CPlayer,
  CPlayerManager, CTCBase, CSubtitleRenderer and CTBaseSingletonNoDefaultAlloc locally (CTCFishing has no header). That is retype debt
  for `retype_landed.py`, as in 23 earlier landings that carried the same warning.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-06b.tsv` (the six rows, including slot 36 of the CThingAICreature vtable 0x0127293C =
    `DrawPreparePrimitivesForRendering`, proven from the vtable, the CThingCreatureBase slot-36 base call and both PDBs).
  - 16 queue lines (tag `land0611b`):
    - CAIBrain::PeekCurrentStateGroupName 0x0088B7D0 / GetCurrentStateGroupDebugText 0x0088B7E0;
    - CThingCreatureBase::GetCreatureType 0x00662000;
    - CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut 0x006B82E0;
    - the GetAllThingsInRoughArea filters 0x007A9300 / 0x007A9580;
    - Blackjack Split 0x007DB5C0;
    - the IS_THING_DEAD / IS_THING_ALIVE templates;
    - list insert 0x007464A0;
    - ConstantFPS 0x01375550 (16 landed spellings);
    - FinishFishing 0x007B0950, which the landed 0x007B0CC0 binds under a placeholder (coordinator line).
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand801/`)
| Row | Grade | Residue |
|---|---|---|
| 0x007A7D20 CTCBattleCharge::CreateHitEffect | DIFFER(2504v2520), 10 attempts | 64 lines: frame 0x140 v 0x148 (one-byte temporaries / box slot packing), the box values stored as computed, two registers in the inline sound lookup |
| 0x007DB6A0 CTCTavernGameCardBlackjack::CardsMainGame | DIFFER(2544v2556), 10 attempts | 63 lines: case 0x1C's SetState tail cross-jumps into the first epilogue copy where retail uses the later one (0x007DC03E); two register picks. Uses GFFloatToLongFloor/Ceil, so a match grades `retail_asm` |
| 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering | DIFFER(2296v2300), 11 attempts | 648 lines: Y copied through the FPU where retail moves it as an integer; info.Engine cached in ebp. The frame now equals retail's 0x88 |
| 0x00850A80 CContextSensitiveItemDef::IsValidOverride | DIFFER(2684v2576), 11 attempts | 378 lines: frame 0x3C v 0x30 (retail keeps out pointers in dead parameter slots, 0 in edi); case 5's epilogue merge; case 8 converts Y first. Uses GFFloatToLong helpers |
| 0x00960120 CAIStateGroup_DeedReactionRoot::Update | DIFFER(1576v1600), 10 attempts | 618 lines: VC7.1 narrows the bit-16 HasTC test to a byte load where retail keeps the dword `shr ecx,10h` (five spellings narrow alike) |
