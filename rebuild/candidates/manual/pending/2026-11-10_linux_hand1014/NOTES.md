# Chunk 1014 notes

**Header overlay:** every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (p.sh / pl.sh / gall.sh runs below). See `hdr/README.md`
for every opt-in; the default path of each changed header is byte-identical to `rebuild/include`.

## Matched (src/ + test/)
| row | identity | lever |
|---|---|---|
| 0x00703210 | `?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z` (Ego_r + FableWin) | GetPhysicsTC() per branch + visible noinline CTCParamsPhysicsBase ctor (new header engine/CTCParamsPhysicsBase.h) + inline default CRightHandedSet |
| 0x00B84720 | typed `AddSubPrimitive(ESubPrimitiveType, ...)` (FableWin.pdb) | named float local for the scaled distance; `GFFloatToLongNear` (own `// Retail-asm:` line, fistp 0x00B84735) |
| 0x009FA620 | `?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z` | new engine/GFGetNextPowerOfTwo.h (FableWin names, FABLE_FORCEINLINE; line 2 `// Inline-budget:` note) |
| 0x007EA2F0 | `?GetTargetingPositionOnThing@NTargeting@@YI?AVC3DVector@@ABVCWorld@@ABVCThing@@M@Z` | shared-header bodies (bullet time, GetPosition(float)) + `GetPosition(t) + C3DVector(0,0,0.25f)` fallback; helper names via landed static-init globals `g_CCharString_013bac50/4c/dc/48` |

Callee names in the matched objects were checked against their relocations (relocs.py): Alloc, ConstructFromParams (protected, IAE),
??0CTCParamsPhysicsBase, ??1CBaseClassNonCopyable@@MAE@XZ, DoActivationSequence; vector<CSortElement>::_M_insert_overflow 0x00B85EC0;
IsAffectedByBulletTime, ConvertInterpolateToBulletTimeInterpolate (QBEMM), NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint (QBE, C3DVector&).

## Unmatched (differ/), with next steps
- **0x006D6FA0** GetTrainableStatPossibleLevelUpAmount, DIFFER(228v220). c2 hoists `TrainableStatLevels[stat]` out of the loop and
  strength-reduces `(level+n)*72`; retail re-reads the level per pass and keeps `stat` in ebp, and loads ExperiencePointsAvailableToSpend
  before the fraction store. Tried: GFLimitLower clamp in the max level (no change, opt-in removed), available read first, subtraction
  through a by-reference inline helper (225v220, diag only), goto loop (225). Next: model the loop as retail's debug shape in
  FableWin.pdb (look for an inline accessor that takes the level by reference, e.g. a GetTrainableStatLevelUpCost(stat, level) helper).
- **0x009E3F60** CPixelFormat::ConvertRGBToPFDData, DIFFER(179v189) against the corrected 189-byte extent (BOUNDS_FIX.tsv, SPLITS.tsv).
  Ours CSEs the GD3DFormats row address; retail re-reads each bit count with absolute `[reg+0x129BA4C]`-style addressing. Next: give each
  channel its own inline accessor indexing `GD3DFormats[Format].XBits` (no shared row pointer), or a static-member array view.
- **0x00929FC0** CAIStateGroup_ApprenticeHero::GetObjectWithScriptName, DIFFER(265v242). Our found-path list destructor is inlined;
  retail calls the list dtor COMDAT 0x006FE660, and the GFRandom result lands in edx not ecx. Tried unsigned index/--index, while walk
  (208, calls both), early return (280). Next: a list type whose ~list is out of line (the shared STLport list's destructor as a
  noinline COMDAT under an opt-in) with the for walk.
- **0x005DCC00** CAnimationSet::GetRandomAnim, DIFFER(278v264). Register allocation: retail this=ebp, iterator=edi homed [esp+28h],
  &anim=ebx. Best: the while-loop form (`it` from FindFirstAnimWithName, `first = it`). Next: candidate collection through a same-TU
  helper or a different vector<const pair*> reserve size expression.
- **0x00725F10** GetAllThingsInRoughArea<CThingCreatureBase, vector<CThingCreatureBase*>, CThingFilter_IsNot> (instance settled from
  relocations: overflow 0x006F2680, CThingFilter_IsNot::operator() 0x006A76A0, mask 0xF). The shared header body instantiated
  explicitly gives 733v590: retail iterates the mapwho layer table by pointer (0x0137DADC = &LayerInfo[0].ScaleShift) against
  g_NoMapwhoLevels and has frame 0x50 (ours 0x58). Next: a second rough-area body under its own opt-in with the pointer walk, or
  compare against the landed instances 0x0075BC40 / 0x00969FB0 (both local models) to find which spelling those match.
- **0x006E94D0** IsCreatureInAttackRingSector (catalog name Combat_CheckRingRangeToTarget is descriptive), prior draft regraded:
  DIFFER(426v426), instruction-identical, frame 0x24 v retail 0x1c: retail packs the second map-find key onto the first key's slot.
  A named uid local inside the inline GetCombatantInfo changed nothing. The prior is a local padded model (rule debt): needs shared
  CCombatManager / CDefendingCombatantInfo headers before it can land anyway. Not copied to differ/ (prior lives in hand644).
- **0x00681E60** SetPersonalityOverride, prior regraded DIFFER(626v626) (one SIB byte); local model, needs COpinionPersonalityDef /
  COpinionOfHeroDef headers. Not reworked; prior in hand475.
- **0x00A229E0** CRightHandedSet(const C3DAngles&), prior regraded DIFFER(691v689) (frame 0x50 v 0x4c, x87 spill order). Not reworked.
- **0x006A16A0** CTCCarrying::RemoveThingInCarrySlot (784 B) and **0x006CBAC0** CTCCreatureNavigation::GetNextNavPosOnRoute (880 B):
  no prior draft, not reached in this chunk (identities are the catalog names, unverified).

## Header debt
- engine/GFRandom.h (new): landed rows (0x00969E70, 0x005472A0, 0x007FEDE0, ...) keep local GFRandom copies.
- engine/CBulletTimeManager.h: the map member exists only under the opt-in; default layout is still opaque.
- engine/CRightHandedSet.h: several mutually exclusive default-ctor opt-ins now exist; worth unifying.
- engine/CTCParamsPhysicsBase.h: new; other CTCParams* classes still undeclared.
- Landed 0x0075BC40 / 0x00969FB0 / 0x00725F10-family rough-area instances use local models; one shared body per template is the goal.

## Bounds
0x009E3F60 is 189 bytes, not 576 (BOUNDS_FIX.tsv; SPLITS.tsv carries the same so finish.py cuts the right oracle if it lands later).
