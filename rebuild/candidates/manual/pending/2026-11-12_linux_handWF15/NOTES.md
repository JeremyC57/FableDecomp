## 0x00BDF100

## 0x00BDF100 CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren -- RELOCATION_MATCH (grade 1)
- Matched on the first grade by porting onto shared headers: nested class (correct mangled name), std::vector<C3DBoundingSphere>
  from fable_containers.h with FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL, plain push_back at both sites (VC7.1 itself inlines the
  overflow body only at the collection site -- no hand-written inline copy needed), BuildFromSubSpheres(const C3DBoundingSphere*, long).
- Defines: FABLE_RETAIL_CLOCALDETAILCACHEMAP_QUADTREE, _CALCBOUNDINGSPHERE (new), FABLE_RETAIL_CMOVABLERESOURCE, FABLE_RETAIL_C3DVECTOR,
  FABLE_RETAIL_C3DBOUNDINGSPHERE_BUILDFROMSUBSPHERES, FABLE_RETAIL_C3DBOUNDINGSPHERE_COPY (new), FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL. No flags.
- Name fix: old draft's C3DBoundingSphere::SetFromSpheres(..., unsigned) at 0x00ADDBE0 is BuildFromSubSpheres(const C3DBoundingSphere*, long) (Ego_r + FableWin).

## 0x005F6BD6

## 0x005F6BD6 CTCInventoryAbilities::SetAbilityAsAvailable -- RELOCATION_MATCH (wf15, 1st author, ~30 grades)
- What made it match: (1) port onto shared headers (CTCInventoryAbilities.h + player chain opt-ins); (2) real same-TU bodies of
  IsAbilityAvailable 0x005EDF08 (inline) and GetNumAssignedReceivedAbilities 0x005EE146 (FABLE_FORCEINLINE; plain inline stays out of line);
  (3) KEY: std::find / std::__find REAL STLport bodies visible as noinline fastcall COMDATs (new FABLE_RETAIL_STLP_FIND_NOINLINE_BODY):
  with only a declaration &ability escapes so ability is never enregistered; with the bodies visible ability sits in ebx and `last` is
  reused after the call; (4) received-order insert as `added` flag + break, outer dup loop `if (added) break;` (goto forms = 8 hunks).
- Headers ($W/hdr, see README): CVectorMap.h FABLE_RETAIL_CVECTORMAP_VALFIND (FableWin ValFind member template),
  CTCInventoryAbilities.h FABLE_RETAIL_CTCINVENTORYABILITIES_AVAILABILITY, stlport/fable_algorithm.h FABLE_RETAIL_STLP_FIND_NOINLINE_BODY.
  No extra flags (pragmas y-off / s-on in source).
- Landed name proven wrong: 0x005F8150 is landed as vectorEWeaponClass_UmoveEWeaponClass (`std::vector<EWeaponClass>::_Umove`); it is
  STLport std::find<EHeroAbility*,EHeroAbility> (fastcall forwarder to __find 0x005F7ACC; this row calls it with RAO begin/end/&ability).

## 0x00A60D40

## 0x00A60D40 CChecksumCacheFile::Open -- RELOCATION_MATCH (grade 4 of the session, 7 variants)
- What made it match: port onto shared headers (fable_string.h, engine/CAFile/CDiskFileWin32(+DEFAULTCTOR)/CFileDataInputStream,
  stlport/fable_map.h default branch: inline clear, out-of-line operator[]) and, decisively, `if (stream.ReadSLONG() != 4) return;`
  as an EARLY RETURN (second exit through the inline ~CDiskFileWin32). With the nested `if (== 4) {...}` the compiler stores the
  vptr as an immediate, keeps 4 in esi and puts the CCharString temp below the file; the early return gives retail's vptr-in-esi
  (re-loaded after the loop) and frame layout. count<=0 needs no explicit return (plain for loop).
- Headers: overlay hdr/engine/CDataInputStream.h FABLE_RETAIL_CDATAINPUTSTREAM_READDATEANDTIME (ReadDateAndTime 0x00995770),
  hdr/engine/CDateAndTime.h FABLE_RETAIL_CDATEANDTIME_CTOR (public out-of-line ctor 0x009E9AB0). Flags: none. CChecksumCacheFile
  modelled locally (no shared engine header; same as landed siblings). CEntry members TimeStamp / unsigned long Checksum (PDB).
- Tried before match: nested if/PathExists scope variants, count-down / while loops, named version local (all 19-hunk DIFFER).
- Old draft name errors (not landed): fake CFileDataInputStreamX, CEntry::Time, long Checksum -- no NAME_FIXES needed.
- Repair (verifier finding 1): hdr/engine/CDateAndTime.h rewritten as added-lines-only -- original line 4 kept verbatim under
  `#ifndef FABLE_RETAIL_CDATEANDTIME_CTOR`, ctor-bearing class in `#else`. `diff ... | grep -c '^<'` = 0. Regraded RELOCATION_MATCH
  (default and LANDVERIFY_STRICT_HEADERS=1); test PASS.

## 0x0091DDD0


## 0x0091DDD0 CAIStateGroup_HoverAllAroundTest::Update -- DIFFER(626v626), 11 hunks, frame-slot order only (author wf15, 2026-11-12)
Best source: differ/0091ddd0.cpp (variants in v/; s1.cpp = best). Same length, every instruction identical except the stack
slots of two locals: retail player CIntelligentPointer<CThing> at esp+0x18 and the spilled random fraction at esp+0x20 (shared
later with the cos-Z*dist temp); ours frac at 0x18, player at 0x1c.
What got it from 509v626 to 626v626:
- ported onto shared headers (CAIStateGroupBase _MEMBERS/_GETCREATURE/_AIACTIONS/_SPEECH/_MEALACTIONS, CAIBrain _GETCREATURE,
  CThingAICreature chain, CIntelligentPointer, CWorld, CPlayer (FABLE_RETAIL_PLAYER), CWorldMap, C3DVector AccurateNormalise);
- target = start + C3DVector(0,0,1) * (float)GFSin(frac) * dist + perp * (float)GFCos(frac) * dist, perp = (dir.Y, -dir.X, 0)
  (vertical circle; engine/GFSin.h + GFCos.h take TURNS and multiply by (double)6.2831855f -- that is the TWO_PI fmul);
- loop: while (alive) { ...; if (AIAction_MoveToPos(1,target,moving,3,true,true,0,true,0.0f) == 2) continue; Terminate(); break; }
- the first random MUST be an inline function (a hand-written block gives a different x87 schedule: cos kept live, 628v626);
  the second is the out-of-line GFFloatRandom(10.0f, GetPWorldSeed()) 0x004978A0. The inline helper name RandomFractionInline is
  made up (descriptive; no PDB/FSE name found). Unsolved: why retail inlined one GFFloatRandom and called the other.
Ideas tried for the slot residue (all unchanged 11 hunks unless noted): frac/dist/moving/target declared at loop top / fn top /
in a block; const frac/dist; frac as double / inline returning double / local in inline; angle float / double local / no
angle; IsAlive inline fn, IS_THING_DEAD template (out-of-line copy is landed 0x0091DDA0, placeholder name), operator->,
GET_ORIGINAL_CHAIN, CTCTARGETINGAI_ATTACK / UPDATE_LIST SetPointer chains, CTOR_OUTOFLINE (worse), copy-init / const player,
named player creature, me by reference, up as local, copy-init vectors, const start, unpacked CIntelligentPointer probe (no
change; overlay removed), flags /Ob2, optimize("t"); same-TU GFFloatRandom real body (both inlined -> worse); frac/dist as
args of an inline helper (worse).
Best next idea: find the real spelling of the first random (an inline in a header Lionhead had: search FSE / EgoCore / Fable2
sources for a float-random-in-[0,1) inline, e.g. GFFloatRandom overload or a CAIStateGroupBase helper); its inlinee symbols
are what decide the slot order. Also try the row inside a TU that contains the neighbouring rows (0x0091DDA0 IS_THING_DEAD
out of line before it, 0x0091E050 after) -- symbol numbering in the TU may drive the allocator.
No headers changed (hdr/ empty). No name fixes proven.

## 0x0091DDD0 attempt 2 (wf15, 2026-11-12) -- still DIFFER(626v626), 11 hunks, same slot residue; best source unchanged (differ/0091ddd0.cpp; variants in v2/)
Frame map (pre-push esp offsets, both builds): 0x10 S0 (perp.X spill / sin-term temps), 0x17 `moving` (bool, top byte of the
0x14 dword), 0x24 unsigned->double fild temp, 0x28 target, 0x34 dir, 0x44 start. Retail: player 0x18 (8-aligned), S1 {frac,
cos-Z temp} 0x20. Ours: S1 0x18, player 0x1c. Probe with frac = constant (S1 gone): player goes to 0x18 -> S1's slot is
being ranked before the player by the allocator.
Tried this attempt, all unchanged 11 hunks unless noted: real GFFloatRandom body as inline helper with range param (582v626,
the >0.0001 test does not fold; __fastcall version not inlined, 580); frac declared at fn top before player / after start;
frac address-taken (out-param, pointer, const ref); player as lifetime-extended temp bound to const ref (both spellings);
IS_THING_DEAD real body non-inline in TU (+/-/Ob2; /O2 already auto-inlines it); IS_THING_ALIVE spelling `player != 0` with
FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT (+operator->); random helper defined AFTER Update (still inlined); GFSin/GFCos
bodies included after Update (forward-declared); hand-written random as block / comma expr / scoped block (628v626, as
attempt 1); named float/double/ulong intermediates inside the helper; for(;;)+break loop; up local; offset local (620);
default ctor + Set() (z1 12 hunks, z2 11); unpacked CIntelligentPointer overlay probe (overlay removed again); flags /Ob1 /G6
/G7 /GR /Zi /Gr /GS (no change), /GX /Op /Oy- (worse); QFE-4035 compiler (no change); target via an inline helper taking
(dist, frac) evaluated right-to-left (469v626, helper not inlined).
Best next idea: the slot ranking is not declaration order, not address-taken-ness, not inline-symbol origin, not compiler
build. Next levels to try: (1) look for a landed VC7.1 row where an IP local sits BELOW a spilled float temp and copy its
spelling; (2) build a 3-line toy (IP local + float spilled across a call) to learn which property moves the IP below the
spill (e.g. whether the spill comes from a `float` vs a value returned by an out-of-line call, or the IP's dtor being
virtual/non-virtual in the TU's view of CBaseIntelligentPointer); (3) check whether CBaseIntelligentPointer's dtor
visibility (virtual ??1 0x00A01C10 declared vs inline) changes the IP's ranking.
Also tried (attempt 2): raw sin/cos with `static const float TWO_PI = 6.28318548f` (CTools spelling), with and without a double angle local before/after dist (unchanged / 634v626).

## 0x0091DDD0 attempt 3 (wf15, 2026-11-12) -- RELOCATION_MATCH (626 bytes)
What made it match: the loop exit. `if (AIAction_MoveToPos(...) == 2) continue; Terminate(); return;` (return, not break).
The instructions are the same either way. What changes is the block-frequency weights the frame allocator ranks slots by: with
`return` the player CIntelligentPointer outranks the {frac, cos-Z temp} spill group, so it gets esp+0x18 and the group gets 0x20.
`if (... != 2) { Terminate(); break; }` gives the right layout but 655 bytes.
Method: compile with /FAs ($W/toy/probe.py + lay.sh) and read the `_sym$ = -N` frame map, about 2 s per variant. Adding real refs to
`player` reorders the slots, so the ranking is by weight. Inline-layer spellings (GET_ORIGINAL_CHAIN, operator->, IsThingDead) do not.
Headers/flags: shared headers only, no overlay, base flags. RandomFractionInline is a made-up descriptive name (inline GFFloatRandom(1.0f) twin without the 0.0001 test).
No landed names proven wrong.

## 0x008DB810

## 0x008DB810 NCreatureMode::CModeDragonFireBeam::UpdateBeam -- RELOCATION_MATCH (13 grades)
- Made it match: port onto shared headers (CDefPointer<const CJackDragonDef> + noinline GetSubDef, PeekGameCamera()->GetCameraManager(const mgr**),
  GetCameraTools().GetLandscapePointIntersectionFromTo const, CTCDExplosion::Create/SetThingAsImmune, GetTC expanded); each def index its own
  statement and `middle` a named local before the second name (fixed the extra frame slot / esi); the last residue (end/hitPos slots swapped,
  x87 scratch at +44 instead of +20) was the beam end built in place: `C3DVector end = orientation.GetUp() * -1.0f * 1000.0f; end += mouth;`.
- Headers: overlay hdr/engine/CJackDragonDef.h (FABLE_RETAIL_CJACKDRAGONDEF, polymorphic CSubDefClassBase-derived def + inline StatGetTypeName)
  and hdr/engine/NCameraModes_CTools.h (FABLE_RETAIL_NCAMERAMODES_CTOOLS_LANDSCAPEFROMTO const decl). No flags.
- Layout (repair): real hierarchy CModeDragonFireBeam : CModeDragonHover : CModeStandBase : CModeBase (RTTI rtti_bases.tsv:2203, caller 0x008E0780);
  CModeStandBase from shared engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE, 0x68), CModeDragonHover declared in the row (no shared
  header) with its 0x68..0x80 members opaque. Still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1); test PASS.
- NAME FIX (NAME_FIXES.tsv): 0x0073A680 is ?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ, not CViewBase::GetDisplayEngine
  (landed rebuild/src/compiled/00/73/CViewBase_GetDisplayEngine_0073a680.cpp). Body `mov eax,[0x013B8A1C] ; mov eax,[eax+24h] ; ret` =
  CThing::PCurrentThingManager->World (CThingManager +0x24 is `CWorld& World`). Sources: Ego_r.pdb 0x00751030 (same 9 bytes / 3 insns,
  egor_signatures.tsv); FableWin.pdb 0x024C31E0 (fablewin_pdb_names.tsv:68898); all 40 retail callers of 0x0073A680 (0x008621CD..0x008EAD18)
  are CModeStandBase / NCreatureMode mode methods. Folded body: the single retail address keeps CTCCreatureModeManager::CModeBase::GetWorld
  (the only callers are modes; GetDisplayEngine is Ego_r 0x006793D0 and returns a CDisplayEngine&, not the World member).

## 0x005FBAA0


## 0x005FBAA0 CTCInventoryMap::GetRelativePosOnMiniMap -- DIFFER(634v634), 5 hunks (first author pass, ~60 grades + 17 probes)
Best source: differ/005fbaa0.cpp (= v/v24.cpp). Ported onto shared headers (no overlay needed): STLP_VECTOR_COPY_OUTOFLINE,
CTCBASE, DEF_CLASS_BASE (CDefPointer #error otherwise -> cl hangs), C2DCOORDI_COPY, CWORLD, CWORLDMAP, CMAP_WORLDACCESSORS, CREGION,
CCHARSTRING, SCOPED_PTR, CGAMECOMPONENT, CMAINGAMECOMPONENT(_ENGINEGRAPHICS), CGRAPHICDATABANK_GRAPHICSIZE/_FRAMESIZE,
CTCINVENTORYBASE(_MINIMAPGRAPHIC), CTCINVENTORYMAP_REGIONDESCRIPTION; includes engine/C3DVector.h explicitly.
What fixed the old 632v634 residue: shared headers + COPY_OUTOFLINE (vector start/size cached); C2DCOORDI_COPY (dead add eax,1D24h);
loop body without right/bottom locals (`if (br.X < pos.X + SizeX) br.X = pos.X + SizeX;`); post-loop order relX, relY, scaleY=1,
width, height.
Remaining residue: (1) scaleY packed into i's slot [esp+10h] (retail: [esp+24h], br.Y's never-used slot); (2) X-line
`(relative.X*scale-0.5f)*w` emits fld scale ; fmul relX (retail fld relX ; fmul scale).
Tried without effect (all keep the same 5 hunks): operand order in every FP line; X-line split into compound statements; scale as
a local (638, worse); C2DVector scale (651/634-30h); C2DVector ops for offset (643); direct member access (probe); double consts;
graphic sizes as ulong/top-declared/implicit conv; i/scaleX/scaleY declared at function top / before loop / types long,int,ulong;
statement orders of the post-loop block (v21/22/75/76 worse); inner blocks around bounds (v36/37/81-83); br as C2DBoxI, br ctor,
br(tl), br as longs (frame shrinks to 0x20, worse); front()/begin()/pos reference; flags /Op /Ob2 /Ow; no COPY_OUTOFLINE.
Probe finding (toy, $W/probe): VC7.1 orders `a.X * r.S` loads relX-first when nothing before the tail branches; ANY branch/loop
earlier in the function flips it to scale-first (probe 13). Retail has the loop yet keeps relX-first, so something in retail's tail
or in how relative/region enter the tail differs -- not found.
Best next idea: find the tail spelling that keeps relX-first in the branchy toy ($W/probe/t.cpp r*/s* functions compile in ~20s)
before touching the row; candidates not yet tried: relative written through a returned reference / inline helper with its own
params, the scale/size math done in a C2DVector temporary assigned member-wise, a different GetMiniMapGraphic call form
(result stored in a local), the bounds loop over a vector<long>::const_iterator.
Name fix: landed 0x005BEB57 spells ?GetMiniMapGraphic@CTCInventoryBase@NInventory@@QAEJPBVCRegion@@@Z; Ego_r.pdb 0x00620EC7 and
FableWin.pdb (0x01823160, 0x0236E25F) both say ?GetMiniMapGraphic@CTCInventoryBase@NInventory@@QBEKAAVCRegion@@@Z (const, CRegion&,
returns unsigned long) -- see NAME_FIXES.tsv.

## 0x005FBAA0 -- RELOCATION_MATCH (attempt 2, ~45 grades + 6 probes; strict-headers regrade RELOCATION_MATCH; vt PASS; addmatch done)
Source: CTCInventoryMap_GetRelativePosOnMiniMap_005fbaa0.cpp (= v2/final.cpp), test test_005fbaa0.cpp. No overlay, no flags.
Macros as in the first pass plus FABLE_RETAIL_C2DVECTOR (no effect on bytes, kept as graded).
What made it match (both residues came from ONE model fact, the aspect scale's shape):
1. Slot residue (scaleY in i's [esp+10h] v retail br+4 [esp+24h]): the two aspect scales are an 8-byte aggregate declared AFTER
   width/height and the divisions. br (C2DCoordI, 8 bytes) is then dead, the aggregate packs into br's slot, scale[0]/X lives on the
   x87 stack and scale[1]/Y at br+4 = [esp+24h]. A `C2DVector scale(1.0f,1.0f)` there already gives 634v634 with only the X-line left
   (v2/G2.cpp); declared before width/height it is 651.
2. X-line operand order (`fld [esi] ; fmul [ebp+2Ch]`): VC7.1's commutative x87 operand order here follows the number of loops the
   optimizer saw earlier in the function (probes: code-free empty loops before the tail flip it with period 4; ifs, unused locals,
   inline-level changes, include order and TU context do not). `float scale[2]; for (int k = 0; k < 2; ++k) scale[k] = 1.0f;` (a
   fully unrolled 2-trip loop) gives retail's order and the bytes. Made-up local names (no PDB locals): maps, tl, br, i, map, top,
   width, height, scale, k, graphicWidth, graphicHeight -- descriptive.
Lesson: an x87 commutative operand-order hunk that no expression spelling moves can be steered by the loop count before it; try an
aggregate/array local initialised by a loop before reaching for anything else.
Name fix from attempt 1 stands (GetMiniMapGraphic is QBEKAAVCRegion@@, NAME_FIXES.tsv).

## 0x007CD9A0

## 0x007CD9A0 CTCBurntEffect::FrameUpdate -- RELOCATION_MATCH (5 grades)
- Made it match: CRGBColour 4-arg ctor (shared CRGBColour.h body R,G,B,A) fixed the channel order (right-to-left arg eval: c3,c2,c1,c0);
  the dtor's kill-loop spelling `IS_THING_ALIVE(Effects[i])` + `CTCSpecialAbilities::FadeOrKillEffect` (element address computed once)
  fixed the ebp spill / frame 0x14 residue. Tint block written out twice in place (no SetTint helper; not a PDB name).
- Headers/flags: no overlay, no flags. Macros FABLE_RETAIL_CTCBASE, _CTHING, _CTHING_STATEACCESS, _CINTELLIGENTPOINTER (+_GET_ORIGINAL_CHAIN,
  _COMPARE_NULL_INT), _IS_THING_ALIVE, _DIVINE_FINALCLEANUP, _CTCSPECIALABILITIES_FULL, _CTCGRAPHICAPPEARANCE_TINT.
  CTCBurntEffect and CSpecialAbilitiesBurntEffectDef have no engine header: local models (def derives from the shared CParentDefClassBase, packed 0x39, per RTTI ghidra_out/rtti_bases.tsv; no hand pad, FadeInTimeSecs +0x3c, Color +0x48).
- Draft name errors fixed: free FadeOrKillEffect -> CTCSpecialAbilities::FadeOrKillEffect; RemoveTC(long) -> RemoveTC(ETCInterfaceType); GetPItem returns CBaseObject*.
- Repair (verifier findings): kill-at-end now `RemoveTC(TCI_BURNT_EFFECT)` (Ego_r enumerator 137, ETCInterfaceType.h) instead of `(ETCInterfaceType)0x89`; def base CDefPointeeBase+pad -> CParentDefClassBase (FABLE_RETAIL_DEF_CLASS_BASE). Still RELOCATION_MATCH, also strict headers.

## 0x00B211B0


## 0x00B211B0 CSplineRandomOffsetData::InterpolateTo -- RELOCATION_MATCH (grade 3 of the session, 8 variants)
- Matched by porting onto shared engine/CCardinalSpline.h (private members, original accessors Get/SetTension, Get/SetControlPoints,
  GetControlPointCount) + FABLE_RETAIL_STLP_VECTOR_C / _VECTOR_COPY_OUTOFLINE / _STLP_VECTOR / FABLE_RETAIL_CARRAY (visible STLport
  bodies fixed the old ebx/ebp swap), GFInterpolateArray in namespace NParticleEngine, and
  `long prevCount = PreviousSpline.GetControlPointCount(); if (ThisSpline.GetControlPointCount() != prevCount)` for the count compare.
- No header overlay changes, no flags. CPSCSplineBase/CSplineRandomOffsetData stay local (no shared header).
- Landed parent 0x00B22310 still uses local CArray/CCardinalSpline stand-ins (public members) -- could be ported the same way.

## 0x007BF510


## 0x007BF510 NThingShot::CTCShotDefinitionBase::OnCollision -- RELOCATION_MATCH (wf15, ~22 grades)
- Ported onto shared headers (sibling 0x007C1000's define set + C3DVECTOR_SQUAREDMAGNITUDE_ZYX + STLP_FIND_RANDOM_DISPATCH); real
  signature (ECollideType type, ...), members BestCollision*, IS_THING_ALIVE, GetTC expanded, (points.end()-2)->first.
- `PBestCollisionThing = thing;` (implicit (T*) conversion) leaves the ctor out of line and operator= inline like retail; the
  explicit `= CIntelligentPointer<CThing>(thing)` put operator= out of line too.
- Last residue (tag lea [esp+34h] v [esp+28h]) = callee body visibility: __find 0x007C2980 must be VISIBLE as __declspec(noinline)
  (FABLE_RETAIL_STLP_FIND_NOINLINE_BODY); then VC7.1 homes the empty tag on the live `type` slot. Same lesson as 0x0083FB20.
- Headers (overlay hdr/): CTCWeaponTrail.h FABLE_RETAIL_CTCWEAPONTRAIL_HASPOINTS, fable_algorithm.h FABLE_RETAIL_STLP_FIND_NOINLINE_BODY,
  CIntelligentPointer.h FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED (free mixed-type operator== -- its spelling is made up /
  descriptive: the original's form was not found; needed only to compile the visible __find body). No flags.
- Tried without effect on the tag slot: named bool, ternary, nested if, POD tags, traits/func tag, fastcall find wrapper,
  trail declared first, SetBestCollisionType setter, /Ob1; /GX breaks the frame.
- Name note: landed std_find_007c2980.cpp instantiates __find<CIntelligentPointer<const CThing>*, CIntelligentPointer<const CThing> >,
  but this caller passes a CIntelligentPointer<CThing> value (vtable 0x01238C6C, <const CThing> is 0x01237F1C), so the retail
  instantiation is __find<CIntelligentPointer<const CThing>*, CIntelligentPointer<CThing> > (see NAME_FIXES.tsv).
- Repair: GetTC((ETCInterfaceType)0x4c) respelled GetTC(TCI_WEAPON_TRAIL) (Ego_r enumerator); still RELOCATION_MATCH, also strict headers.

## 0x00972CC0 (not landed)


## 0x00972CC0 CAIStateGroup_JackOfBladesStage1::MeleeCombat -- RELOCATION_MATCH (grade ~16)
- What made it match: port onto shared headers (CAIStateGroup_JackOfBladesStage1/Base, CAIStateGroupBase, CAIBrain, CWorld GetCombatManager,
  CCombatManager, CIntelligentPointer, ThingAlive) put `this` in edi (local models left an esi/edi swap); landed sibling 0x0093DDE0's
  CBrainOutput model (init-list ctor, declared empty dtor, SetAsLookAtThing with an explicit temp CIntelligentPointer<const CThing>) gave
  the memory `or`s and ebp vptr; reading the target pointer (`PTargetThing.Get()`) BEFORE `me.GetPos()` fixed the last order/register residue.
  IsStillRunnable (0x00971B00) real body inline in the TU = landed row's nested-if spelling verbatim.
- Headers: overlay opt-ins FABLE_RETAIL_JACK_STAGE1_HASHITSCAUSEDRUSH (Stage1.h) and FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERREADY
  (CCombatManager.h); see hdr/README.md. No flags. CBrainOutput/CNavigationInput local (no shared header; same as sibling); PAction = shared CCountedPointer<CCreatureActionBase>.
- Draft name fixes confirmed (dossier): IsInMeleeAsAttacker, GetWorld, SetOutput, SuspendSuspendableProcess, GetPos, FrontMiddleLimit, PThingCombatManager.

### Repair (2026-11-12, verifier findings) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), test PASS
1. Local CCountedPointer_CCreatureActionBase / SCountBlock removed; PAction is the shared engine/CCountedPointer.h
   `CCountedPointer<CCreatureActionBase>` (Data/Info, CCPPointerInfo) + engine/CCreatureActionBase.h; no opt-in needed. The old
   "no shared header" note for the count block was wrong.
2. IsStillRunnable now spelled exactly as landed rebuild/src/compiled/00/97/CAIStateGroupJackOfBladesBase_IsStillRunnable_00971b00.cpp
   (nested ifs, PTargetThing.Get()->IsDead()); the earlier IS_THING_ALIVE one-expression spelling (from the parked 0x00975BB0 Update
   draft) gives the same bytes; the source comment says both.
3. The `creature` / `world` steering locals were not needed: `GetWorld().GetCombatManager().SetAttackerAsReadyToAttack(GetCreature(), true)`
   grades the same (right-to-left argument evaluation loads [[edi+4]+0x18] into esi at 0x00972CDE..0x00972CE1 before `call 0x8fcfc0`
   at 0x00972CE6). Removed; the source comment cites those bytes.

