# agent1022 (chunk 1022) notes

**Every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported** (gall.sh does not export it; export it before calling gall.sh / g.sh,
or cl under Wine hangs on the missing opt-in headers). Overlay contents: hdr/README.md.

## Results (4 matched, 1 parked after 7 grades, 1 phantom, 2 parked unworked)
| Row | Identity | Grade | Lever |
|---|---|---|---|
| 0x00A02DF0 | ?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z | RELOCATION_MATCH (10) | same-TU callee bodies before the row; GetListIndex ternary; `area == v[i]` order |
| 0x00418289 | ?Update@CMainGameComponent@@MAEXXZ (catalog label was generated) | RELOCATION_MATCH (3) | `/O1 /Oy- /GS` size TU + pragmas; implicit double->float GFGetTime stores; new shared retail-asm GFFPUSetDoublePrecision |
| 0x00417747 | ?UpdateForcedTickSpeed@CMainGameComponent@@QAEXXZ | DIFFER(600v596), 0.941 (7) | parked: the `fld [m] ; fld1 ; fdivrp st(1)` reciprocal pair (0x00417750..0x00417761) |
| 0x0078C210 | ??0CTCBerserk@@QAE@AAVCThing@@@Z | RELOCATION_MATCH (2) | SPLITS 602; block-scoped named def-name CCharString locals; max level in a local; shared GFFloatToLongCeil |
| 0x004CA019 | (phantom) | -- (0) | 9 bytes into the landed CThing::ConstructFromParams 0x004CA010; drop the row |
| 0x0061A6A0 | ?OnCreate@CTCInventoryQuests@@UAEXXZ | not graded (0) | parked for budget; skeleton in differ/0061a6a0.cpp |
| 0x00B67270 | CEngineLandscapeRenderer::BuildBlendingTables | not graded (0) | parked for budget; skeleton in differ/00b67270.cpp |
| 0x00691D70 | ?ProcessInput@CInputProcessCreatureMovement@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z (catalog label was generated) | RELOCATION_MATCH (1) | SPLITS 528 (code + switch tables); `/GS` |

## Splits (SPLITS.tsv)
- 0x0078C210: the ctor is 602 bytes (`ret 4` at 0x0078C467); the catalog's 688 swallows 0x0078C470 (60 bytes, a TC 0x31 PeekTC getter, not named here).
- 0x00691D70: 528 bytes = code to `ret 8` at 0x00691F59 + jump table 0x00691F5C (2 dwords) + byte table 0x00691F64 (28 entries); next function 0x00691F80 (68 bytes). The catalog's 624 runs into it.
gall.sh was taught to grade SPLITS rows with tools/q2.py (q.py with the oracle cut to `QLEN`).

## Identities / callees proven this chunk
See LABELS.tsv (new labels) and NAME_FIXES.tsv (landed spellings proven wrong: 0x0049D9E0, 0x0041E61B, 0x00416047, 0x00A038C0,
0x00A03780, 0x00687D00, 0x00445BA0, 0x00A0D390, 0x00687CF0). ProcessInput's callees: GetControlMovementFromGameActionEvent
0x00687D00 (Ego_r 0x006B2E10), IsEventButtonReleased 0x00687B30, IsEventButtonPressed 0x00687A90, IsEventGameAction 0x00687CF0,
CInputEvent GetDeviceType 0x00A03B50 / GetType 0x00A03B40 / GetKey 0x00A03B70, CGameEvent::AddData 0x009F1760 (AddUHUGE / AddFloat /
AddSLONG inline), CProcessedInput AddGameEvent 0x00A0D340 / SetAsPerformedEvent 0x00A0D390, CGamePlayerInterface::IsToggleSneak
0x00445BA0, CPlayer::GetPControlledCreature 0x00487DC0, CTCStealth (TCI_STEALTH 54) Sneaking +0x0F / SneakingToggleSet +0x10.
Berserk ctor: CTCBase(CThing&) 0x00686800, CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390,
GetDef<CSpecialAbilitiesBerserkDef> 0x0078DA50, def members (retail offsets) FramesToFadeOut +0x40, BerserkDuration +0x7C,
BerserkDamageToEnemyMultiplier +0x88, BerserkDamageToMeMultiplier +0x98, MaxIncreaseInStrength +0xA4, BerserkScale +0xB0,
RumbleLevel/Smoothness/Attack/Decay +0xF0/+0xFC/+0x108/+0x114; TCI_HERO_EXPERIENCE 104, TCI_COMBAT 48, TCI_HERO_ABILITIES 111;
PeekDefinitionManager 0x00415FBC, GetDefGlobalIndexFromName 0x009AD410, GetAbilityLevel 0x005EDE93 (HERO_ABILITY_BERSERK 8), SetLevel 0x0078BF00.

## Made-up names
None introduced. (GFFPUSetDoublePrecision is the helper's descriptive name -- no PDB spelling found for the inline-asm block; it
follows GFFPUSetRoundingNearest's house spelling. Mark descriptive at landing.)

## Next steps for the unmatched rows
- 0x00417747: the residue is the two reciprocals; untried levers: a helper inline function returning `1.0 / x` from a register
  argument, `/Op`, or an x87 compile-time pragma; a c2prio probe on the reciprocal alone first.
- 0x0061A6A0: needs a real engine/CTCInventoryQuests.h member list (+0x130 CBase subobject, +0x134 CHandleVector, +0x140
  vector<CQuestCardItem>, +0x150) and names for the two statics 0x013B8C64 / 0x013B8C68; the `and esp,-8` frame says an 8-aligned local.
- 0x00B67270: needs the CEngineLandscapeRenderer texture array (+0x5BC, 5 x 8) and the texture create / lock / unlock spellings
  (0x009FA280 / 0x009FA450 / 0x009F9DE0) plus the format helper 0x009E3790 / 0x009E3800; the round block is a shared retail-asm helper.
