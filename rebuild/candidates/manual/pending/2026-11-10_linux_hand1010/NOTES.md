# Chunk 1010 notes (CGameScriptInterface, 16 rows)

**All grades and probes ran with `FABLE_HDR_OVERLAY=$A/hdr` exported** (see hdr/README.md for every opt-in). gall.sh defaults
the overlay to this directory as well.

## Results
| Row | Identity (Ego_r/FableWin) | Grade | Grades | Lever / residue |
|---|---|---|---|---|
| 0x0089DA70 | ?EntitySetAlpha@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z | RELOCATION_MATCH | 7 | IS_THING_ALIVE(carried[i]) template + re-read element; byte alpha local |
| 0x008930F0 | ?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@0@Z | RELOCATION_MATCH | 10 | shared headers + same-TU noinline GetPFirstThing body + two separate null-return ifs |
| 0x00897FA0 | ?MakeHeroCarryItemInHand@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z | RELOCATION_MATCH | 1 | shared-header rebase (GETTC_EXPANDED, inventory/carrying headers) |
| 0x00895190 | ?GiveThingItemInSlot@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@1@Z | RELOCATION_MATCH | 1 | shared-header rebase (CThingObject::Create, CTCCarrying slot opt-ins) |
| 0x0089FEB0 | ?AddCrimeCommitted@CGameScriptInterface@@UBEXABVCScriptThing@@W4ECrime@@_NPBV2@3W4EOpinionPostDeedType@@@Z | RELOCATION_MATCH | 1 | shared-header rebase + STLP_LIST_SIZE_DISTANCE + cached end iterator + `if (criminal)` |
| 0x00896390 | ?OpenChest@CGameScriptInterface@@UBE_NABVCScriptThing@@_N@Z | DIFFER(711v711) | 1 | block order: direct Open() arm v shared false tail |
| 0x008A9880 | ?ScreenFilterFadeTo@CGameScriptInterface@@UBEPAXMMMMMABVCRGBFloatColour@@ABV?$vector@UCScreenFilterSThingByPass@@...@Z | DIFFER(347v363) | 1 | const-0 piece edi v ebx, destroy-loop end register, missing loop-head align block |
| 0x00896B60 | ?KickOffQuestStartScreen@CGameScriptInterface@@UBEXABVCCharString@@_N1@Z | DIFFER(437v445) | 0 (15 probes) | epilogue cross-jump |
| 0x008A8C60 | EntitySetAsSittingOnFloor | DIFFER(472v472) | 0 (5 probes) | name / filter temporary frame slots |
| 0x008A93A0 | ?EntityWillTeleportToArea@CGameScriptInterface@@UBE_NABVCScriptThing@@VC3DVector@@MM@Z | DIFFER(618v620) | 0 (5 probes) | this/thing ebx swap |
| 0x0089B5B0 | ?StartScriptingEntity@CGameScriptInterface@@UAE_NABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@W4EScriptAIPriority@@@Z | DIFFER (2 lines) | 0 | inline ~CCountedPointer Info reload register |
| 0x0089C910, 0x0089CE70, 0x008A7A70, 0x008A8260, 0x0088EDC0 | (catalog names) | not worked | 0 | ran out of session; priors unchanged |

The grade budget was NOT fully spent on the parked rows (the owner asked for it); the session ran out before the
remaining rows could be rebased. Those rows are the next pass's work, in this order (likeliest first): 0x008A8C60,
0x00896B60, 0x0089B5B0 (both need new headers), 0x008A7A70, 0x008A8260, 0x0089CE70, 0x0089C910, 0x0088EDC0.

## Next per unmatched row
- **0x00896390 OpenChest** (differ/): 711v711, only the order of the last two blocks (retail: theft-deed true tail, `Open()` arm, false tail; ours: true tail, false tail, Open arm). `if (!started) goto fail;` (work/00896390_d.cpp) places Open right but lays the false tail at the SetCurrentAction test (703v711). Try more arm structures (a nested helper for the theft-deed chain, `if (useHero)` with a `do{}while(0)`), then c2prio-free block probes.
- **0x008A9880**: on shared headers; register pair + `lea esp,[esp]` alignment block of the bypass loop head. Next: a vector copy / destroy shape from the STLport header bodies (UNINIT_COPY/FILL_N opt-ins tried), or a `for` over iterators.
- **0x00896B60**: local models; needs CTCQuestCompletionUI (Activate / SaveInitialValues / Active, Closing, Skipped), CScriptRunner (`g_pScriptRunner` 0x013D2838, m_bStopRequested +5), CQuestManager::AddActivatedQuestName, CWorld::AutoSaveQuestStart headers; then try the PDB GiveHeroTutorial inline body.
- **0x008A8C60**: local models; rebase on CCreatureAction_SitDown.h / CThingCreatureBase (GetRandomAnimFromNameWithFilter, SetCurrentAction) / CAnimFilterBase.h, then probe the temporaries' frame slots.
- **0x008A93A0**: shared headers already; c2prio says this -20 v thing -11 (retail needs this above thing). Next: reduce the thing's block weight (blocks 18 / 23) with a reference to the creature taken once.
- **0x0089B5B0**: needs a retail CScriptGameResourceObjectScriptedThing class (base CScriptGameResourceObjectScriptedThingBase, ctor 0x009039D0 `??0...@@QAE@AAVCGameScriptInterface@@AAVCThing@@JW4EScriptAIPriority@@@Z`, static `CreateNullHandle` 0x00903810 fastcall), CTCScriptedControl::IsLocked() inline (Locked +0x18), CThingAICreature::GetCurrentStateGroupScriptPriority 0x006D5AD0, the type byte +0x10, g_pScriptRunner->m_bStopRequested.

## Callee names (two sources each; LABELS.tsv / NAME_FIXES.tsv)
All callee spellings in the matched sources are the PDB spellings; NAME_FIXES.tsv lists landed spellings that are wrong
(GetAllCarriedThings, CThingObject::Create, IsCarrySlotFree, GetInventoryType/Owner, Kill, GetDefGlobalIndexFromName,
GetPControlledCreature, GetMapNumberFromScriptName, list::size 0x0065F17B, CTCChest::GetOpenAnimNameForCreature / Open,
CTCVillageMember::GetPVillage, CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo).

## Header debt
- New headers: CThingFilter_HasScriptName.h, CScreenFilterSThingByPass.h, CTCChest.h (hdr/engine).
- Missing headers met: CTCQuestCompletionUI, CScriptRunner (0x00896B60, 0x0089B5B0); flat generated CScriptGameResourceObjectScriptedThing.h.
- `IS_THING_ALIVE<T>` has no shared header: the matched rows define the PDB template locally (inline body or the
  out-of-line declaration bound to 0x0040DCE0). A shared `engine/IS_THING_ALIVE.h` would remove the duplicates.
