# agentC24 notes

## 0x0058F9BC CTCInventory::OpenPCPhotojournal -- RELOCATION_MATCH (2012 bytes, /GS), test PASS, vland PASS
Name: Ego_r.pdb ?OpenPCPhotojournal@CTCInventory@@QAEXXZ (donor 0x005FE741) + FableWin.pdb; CTCInventory.h OPENPC opt-in already declares it.
Extent confirmed: ends `leave ; ret` at 0x00590197 (2012 bytes).
Flags /GS + `#pragma optimize("s", on)` (biased lea-ebp frame + cookie), as the landed HasScreenshots 0x00581B33.
What closed it (9 grades): first draft from the disassembly with the OpenPCMapMenu / OptionsMenu idioms graded 2013v2012;
- implicit const char* -> const CCharString& for CreateMenuEntryComponent's 4th argument (temporary built before the argument pushes);
- implicit unsigned short* -> const CWideString& for GetFileModifiedTime (address re-taken with lea instead of the ctor's eax);
- one cast pointer local each for the thumbnail (CSprite*), description (CText*) and photo sub-list (CList*): retail keeps each in
  edi; with repeated static_casts c2 gave the CCharString -1 length constant ebx for the whole loop and spilled the scroll pointers;
- 510.0f (0x43FF0000), not 511.
Retail quirk kept: the loop re-reads GetFileModifiedTime(fileName) of the scan buffer's LAST name into an unused CDateAndTime
(0x0058FB8F..0x0058FBA9), commented in the source.
Header: FABLE_RETAIL_CDATEANDTIME_LESS (hdr/README.md). Descriptive local names only (no new function names).
Name fix: HasScreenshots 0x00581B33 calls 0x0041A600 "GetSaveDir"; it is GetPhotojournalDir (NAME_FIXES.tsv).

## 0x007BFE30 NThingShot::CTCShotDefinitionBase::UpdateCollision(bool&) -- DIFFER(1628v1648), parked (~25 grades), best in differ/
Name: Ego_r.pdb ?UpdateCollision@CTCShotDefinitionBase@NThingShot@@MAEXAA_N@Z (donor 0x007C0E70) + header COLLISION opt-in. Extent ok (ret 4 @ 0x007C049D).
Decoded fully (source in differ/): weapon-def UseExtendedProjectileTrails flag; CTCWeaponTrail last two trail points; centre/line of
the last segment; C2DBoxI(centre, (long)((|line|+0.5)*1.5)); GetAllThingsInRoughArea<CThing>(GFFilter_Bind3(IsNot(shooter alive ?
shooter : ParentThing), anonymous CThingFilter_ShotsToCheckForCollisionWith, CThingFilter_SolidToThing(*ParentThing)));
per thing: pause-mode / rolling skips, GetTC(TCI_HIT_LOCATIONS) && not in HitThingsList (find == end), triangle test x2 (CArray<C3DVector>(3),
0.25f) or finite-line test (0.0f), nearest by squared distance from prev.first; then PBestCollisionHitLocation + ParentThing->OnCollision(1,..).
Callee names: 0x007C2D10 = CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CFilter_Bind3<IsNot, ShotsToCheck.., SolidToThing>>
(Ego_r 0x007C1C40); 0x00661620 = CThingFilter_SolidToThing ctor; 0x008169B0 = GetPCombatFirstHitLocationIntersectionWithTriangle;
0x007C2AB0 = CArray<C3DVector>(unsigned int) (FableWin @I@Z).
Closed so far: ignore as `CThing* ignore = ParentThing; if (IS_THING_ALIVE(shooter)) ignore = shooter;` (1601 -> 1628, un-merged the arms);
`if (GetTC && find == end) { body }` (the bool polarity).
Open residue (retail order): (1) prologue: retail this=ebx, weaponDef ptr in edi, bool spilled to [esp+12h]; ours this=edi, bool in bl;
(2) 0x007C00FE: retail stores Bind3 F1/F2 BEFORE the SolidToThing ctor call (tried named IsNot / shots / solid locals, direct CFilter_Bind3
construction, named filter: no change); (3) loop head: ours aligned with jmp+padding, retail falls in unaligned (nested-if instead of
continue: no change); (4) arm A x operand order (retail fld last.first.x first); (5) dist^2 x87: retail keeps the 3 components on the stack
(fld st(0); fmul st(1)...; fstp st(3)); named offset vector and SQUAREDMAGNITUDE_ZYX change nothing.
Header: hdr/engine/CTCHitLocations.h FABLE_RETAIL_CTCHITLOCATIONS_TRIANGLE (assert pinned), new hdr/engine/CThingFilter_SolidToThing.h.
Next ideas: callee-body visibility for the SolidToThing ctor (diagnostic only, it is another TU's), GetAllThingsInRoughArea visible
noinline body (FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY) for the things vector escape; c2prio on `this`.

## 0x006823D0 CTCCreatureOpinionOfHero::FrameUpdate -- DIFFER(960v962), ~15 grades, best in differ/ (close: 10 hunks)
Name: header vtable slot 9 (FrameUpdate) + Ego_r/FableWin ?FrameUpdate@CTCCreatureOpinionOfHero@@UAEXXZ. Extent ok (`ret` @ 0x00682791).
Shape (decoded, in differ/): unknown-threat source killed when (GetWorld().GetFrame() - FrameLastUpdatedUnknownThreat) > ConstantFPS*30 or the
hero is the perceived, aware, IgnoreFOV-line-of-sight thing (CTCPerceiveThing +0x2F); TransientOffsets walk with a `next` iterator (run in:
add, peak capping with GFFloatAbs, clamp +-2, --FramesToRunIn; hold: --FramesOfCappedPeak; run out: add, clamp, --FramesToRunOut; else erase);
every ConstantFPS*20 frames (unsigned 64-bit (frame + UniqueID) % n, __aullrem) the 92 KnownDeeds lists drop handles the hero's
CTCHeroOpinionDeedLog (GetTC 0x51, out pointer uninitialised) no longer holds; NumberOfTimesHit decays by frames/FPS/60 after 30 frames.
Closed: index the opinion array at every use (no `float&`), (unsigned long) frame + unsigned divisor, cached inner `deedsEnd`.
Open: (1) frame 0x18 v 0x14 -- retail reads the peak from the GFFloatAbs parameter slot [esp+18h]; ours gives the named `peak` its own slot
(tried const, reference, declared at loop / function scope, unnamed (moves the abs below the test), split assignment, a bits-temporary
GFFloatAbs text (worse); fwsize: GFFloatAbs / GetUniqueID / GetConstantFPS texts SAME as FableWin). A named `runOut` local fixes the RunOut
abs order (0x00682538) but adds two slots. (2) `mov ecx,ebx ; push 1` order at Kill(true). (3) UniqueID high word loaded after ConstantFPS.
Header: hdr/engine/CTCCreatureOpinionOfHero.h FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREATSOURCE (PUnknownThreatOpinionSource typed
CIntelligentPointer<CThing>, include added under the opt-in; 30 asserts pinned by pin_asserts.py).
Next ideas: frame-packer trace (pk tools) for the peak / f slot coalescing; try the run-in block written with a `float& value` reference
only for the clamp (retail re-indexes there too, so probably not).

## Untouched / outlined rows
- 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long, unsigned long) (Ego_r ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z):
  not authored (mip-level clamp against the graphic's mip count, the texture-resource reuse path, the CResource loading request). Landed
  siblings: StopLoading 0x009FDC20, Sizeof 0x009FDBE0, ResetTexture 0x009FE440.
- 0x007C0690 NThingShot::CTCShotDefinitionBase::Init (target-thing overload; header INIT opt-in declares it): decoded, not authored:
  PTargetThing = CIntelligentPointer<const CThing>(target); target position from the target's TC 8 (vfunc +0x94 with 1.0f) unless the
  target's player-controller (0x004C7A30 / 0x004874D0 / +0x20A) says otherwise; else the shooter's aim (0x007F59F0 / 0x007EA2F0) or a
  velocity lead (target type 2, +0x21A, TC 8 float 0x00802390, world+0x34 vfunc +0x24 velocity * time + GetPos); else from the physics
  positions with a normalised, angle-clamped direction (0x00A14440, 0x00BFEBA2, 0x00A13C60) scaled by 0x0126E17C; then forwards to the
  landed position overload 0x007BF790 (twin source: rebuild/src/compiled/00/7b/NThingShotCTCShotDefinitionBase_Init_007bf790.cpp).
- 0x007EF200 = CTCCarriedActionUseExpression::OnActionUse(CThing&) (rtti-vtables slot 28 = OnActionUse, as CTCCarriedActionUseLamp
  0x007ECEE0; Ego_r ?OnActionUse@CTCCarriedActionUseExpression@@UAEXAAVCThing@@@Z donor 0x007E8CE0): /GS speed TU; decoded, not authored
  (base OnActionUse 0x007EAB90; alive/flag checks; targeting TC 8 IsTargetedThingOfType 2 / 1; expression def +0xC; quest activation via
  CQuestManager::ActivateQuest 0x004B4A10 or a CGameEvent 0x0041649C; CCreatureAction_PerformExpressionExtended (ctor 0x008430B0, dtor
  0x00718D50) through CThingCreatureBase::SetCurrentAction 0x006644F0; CTCShowableItem::OnShowEnded 0x005D8E50). The catalog vfunc_28 is a placeholder.
- 0x0074E860, 0x0072CF40, 0x00735FD0, 0x007E15C0: not opened.
