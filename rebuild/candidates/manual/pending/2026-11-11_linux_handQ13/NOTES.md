# Chunk Q13: notes

Every grade ran with `FABLE_HDR_OVERLAY=$S/w22/agentQ13/hdr`. The overlay ended up empty (see hdr/README.md), so every
source compiles against the repo headers.

Result: **3 RELOCATION_MATCH** (src/ + test/) and **23 DIFFER** (differ/).

## Matches

| Row | Identity | Lever |
|---|---|---|
| 0x00DD9520 | NScript::CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene (?SplittingUpCutscene@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ, Ego_r + FableWin) | The row's inline CScriptThing::GetAngleXY slot wrapper returns its call parenthesised, `return (PImp.Data->GetAngleXY());`. The PAREN tuple moves the fstp of the teleport angle temporary one slot later, after the GetPos null re-test (0x00DD9631..0x00DD9637). |
| 0x0092ACE0 | CAIStateGroup_ApprenticeHero::GoToLibrary (?GoToLibrary@CAIStateGroup_ApprenticeHero@@IAE_NXZ, FableWin.pdb) | (1) The look target is a named by-value `C3DVector target = direction + position;`. (2) The turn speed 0x0138CBA8 is a file-static `static float g_LookAtTurnSpeed = 0.01f;`. An extern schedules `mov edx,[0138CBA8]` before the first target fld; the static schedules it after (0x0092ADD1..0x0092ADDE). |
| 0x009DD960 | CRenderManager2D::Draw2DLine (Ego_r + FableWin) | Two plain reads, `v0.RHW = g_ScreenRHW; v1.RHW = g_ScreenRHW;`, placed after the two Z zeroes. c2 then merges them into one load plus `mov eax,edx` (0x009DD9C8..0x009DD9D2). Placed before the Z zeroes, they compile to two loads. |

Tests: all three pass under g++ -std=c++03 and under VC7.1 /Od (`tools/vctest.py`).

Landing note for 0x0092ACE0: the row defines its own `static float g_LookAtTurnSpeed = 0.01f;`. That is internal linkage,
.data, value 0.01f per the landed 0x0092AFE0 notes. reloc_bindings has `?g_LookAtTurnSpeed@@3MA` -> 0x0138CBA8 from the
landed 0x0092AFE0 (extern). The PDBs have no public symbol at 0x0138CBA8, which fits a TU-static. The landed siblings
0x0092AFE0 / 0x0092AE70 / 0x0092A8A0 declare it `extern float`. If those rows share one TU with this one at landing, they
should also see the static. The spelling g_LookAtTurnSpeed is the landed descriptive name: the prior draft's
g_LibraryLookTurnSpeed was respelled to it (one address, one name).

## Unmatched rows (what each needs next)

- **0x00989830** CShaderRenderManager::InitialiseLight, 98v99, 9 grades.
  - Residue: c2 CSEs index-1 (`mov ecx,eax`) where retail rematerialises `lea ecx,[ebx-1]` (0x0098986B).
  - Needs a spelling that computes the component before the base add without CSE. Every named-local order CSEs; the operator[]-argument-first form avoids the CSE but changes the base register.
  - Header debt: CShaderLight.
- **0x00918A40** CAIStateGroup_GazeAtWork::IsRunnable, 114v116, 7 grades.
  - Residue: retail keeps the shop-keeper TC in eax across an IL block end (`mov eax,[eax+4] ; pop esi ; mov ecx,eax`).
  - The source construct that leaves a block boundary after the expanded GetTC load is still open.
  - Ten spellings are inert, including GFDebugPDynamicCast in the expanded body. Loads in both arms give the eax copy but two loads.
  - Header debt: local GazeAtWork models.
- **0x0047042A** CSoundMap::TransferBinaryIn, 716v716, 3 grades.
  - Residue: one SIB operand order (`lea ecx,[eax+ecx+4]`, 0x004706C9).
- **0x005A6769** CTCInventoryTrade::ConstructNormalShopListPC, 1336v1336, 1 grade.
  - Residue: the wanted-sum operand order.
  - c2prio shows the reassociation is decided before regalloc, so a different expression tree is needed, not a statement order.
  - Header debt: the stand-in CThingPlayerCreature.
- **0x00ADD690** C3DBoundingSphere::FastBuildFromPoints, 646v648, 4 grades.
  - Residue: the 4x unroller's remainder counter.
- **0x006D8980** CTCHeroExperience::UpgradeTrainableStat, 180v183, 2 grades.
  - Residue: register candidates (Abilities start in ebx, level+1 copy).
- **0x00E4AA00** CV_BanditTollScript::CBanditLeader::Main, 1356v1356.
  - Residue: one register in the scope-end handle dtor (rotation pointer).
  - Needs the dtor block generated before the angered block, or one more expression temporary in the angered block.
- **0x00ED9BB0** CV_TalentlessBardScript::ProcessSongsClothes, 298v306.
  - Residue: the entry zero store is a conditional-temporary flag home of a construct that otherwise folds away.
  - Header debt: CV_TalentlessBardScript has no header.
- **0x00666310** CThingCreatureBase::OnCreate, 915v908.
  - Residue: the RMW `and byte ptr [esi+15Ch],7Dh`. Even a raw `byte &= 0x7d` goes through al at this site, so this is not a member-access spelling.
  - Header debt: local model.
- **0x00852E10** CScriptModuleFOVZoomStayRelative ctor, 447v445.
  - Residue: the fast-Normalise Newton r is kept on the x87 stack where retail spills it.
  - Needs three new camera-module headers (layouts in the differ file).
- Regraded as they sit, not reworked (local models; migration first):
  - 0x004FEB70, 0x00435190, 0x00843310, 0x009392A0, 0x00F00C90, 0x008B6AD0, 0x007BC004, 0x0095BCF0, 0x00935580, 0x00A177A0, 0x00A18FC0, 0x00A364E0, 0x00B45A40.
  - 0x009392A0 had no Leaf line; one was added.
  - 0x0095BCF0 and 0x00A364E0 local models mangle as struct/public. The retail leaf is kept and the grade used the model's leaf.

## Callee and global names used

- 0x0138CBA8 g_LookAtTurnSpeed (descriptive, landed, reloc_bindings).
- 0x0139652C g_ScreenRHW (descriptive, landed 0x009DC870, reloc_bindings).
  - FableWin.pdb has a per-TU `dynamic initializer for BIGGEST_RHW_VERTEX_VALUE`, so the original is probably that TU-local const. It is unproven for this address, so the spelling is kept.
- No proven corrections to landed code: NAME_FIXES.tsv is empty.
