## 0x007E2A60


## 0x007E2A60 CTCFineDialogue::GlobalUpdate -- RELOCATION_MATCH (wf17 author)
- What made it match: the old "shrink-wrapped pushes" residue was a model error, not a compiler tie. (1) CRGBColour from the shared
  header with FABLE_RETAIL_CRGBCOLOUR (Initialise copy ctor builds the colour in its arg slot) removed the push/frame residue;
  (2) FinePaid / TimesThrownOut through the inline Ego_r accessors Get/SetFinePaid, Get/SetTimesThrownOut (reg reload + spill for
  fild); (3) the scoreboard fetched into a local before the call; (4) full port onto shared headers (GETTC_EXPANDED +
  own-name FABLE_RETAIL_CTHING_HASTC_MATERIALISED materialised HasTC); (5) FindMostRecentMessage<IsOfType> declaration only (body visible = inlined loop).
- Headers (overlay $W/hdr): CTCHeroStats.h FABLE_RETAIL_CTCHEROSTATS_FINEPAID, CTCVillage.h FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
  (public EndSecuritySweep 0x006590BF; no friend), CThing.h FABLE_RETAIL_CTHING_HASTC_MATERIALISED, CDisplayEngine.h
  FABLE_RETAIL_CDISPLAYENGINE_STARTSCREENFADEOUT, CMessageEventManager.h FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL. No flags.
- Local (no shared header exists): CTCTrespasser, CTCHeroOnlineScoreboard (EOnlineScoreboardActivity enumerator 0x24 name unknown, cast).
- Name fixes (repair pass, proved, in $W/NAME_FIXES.tsv): 0x006590BF is ?EndSecuritySweep@CTCVillage@@QAEXXZ (Ego_r.pdb public
  at the same 5-byte jmp thunk 0x0069D8D1, signature f448c9f6d17c6e3f in both exes; FableWin.pdb 0x02161BAA) -- the row now calls
  Village->EndSecuritySweep() (public, so the friend is gone); 0x006590A0 owns ?InvalidateAndClearAllSecuritySweepTasks@CTCVillage@@AAEXXZ
  (Ego_r 0x0069D86E, FableWin 0x021619D9; reloc_bindings has QAE and the landed leaf is a free function); 0x00686DF0 name is already
  CTCBase::RemoveFromGlobalTCs in the manifest -- only the landed leaf ?CTCBase_RemoveFromGlobalTCs@@YIXPAVCTCBase@@@Z is wrong,
  PDB ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ (Ego_r 0x006B21B0, FableWin 0x021919D0). One symbol, one address each: no conflict.
- Repair: HasTC opt-in renamed to own FABLE_RETAIL_CTHING_HASTC_MATERIALISED (retail `and cl,1 ; mov [esp+13h],cl` at 0x007E2AC1 /
  0x007E2B9E / 0x007E2C27; the verifier's 0x007E2ACC / 0x007E2B9D / 0x007E2C2B are off by a few bytes). Gotcha: adding 3 comment
  LINES to the CThing.h overlay moved two frame slots ([esp+24h]/[esp+28h]); the same text on the #if line matches -- VC7.1 slot
  ties here depend on header line numbers. Regraded RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1; test PASS.

## 0x0075B000

## 0x0075B000 CTCPhysicsRigidBody::InitialiseBody -- RELOCATION_MATCH (wf17, ~75 grades)
- Made it match: port onto shared headers (CTCPhysicsRigidBody with Body as CRigidBody, CPhysicsMeshObject edges by value, CMatrix3x4::SetIdentity opt-in,
  C3DVector::Clear opt-in, STLport resize out of line); the inline CRigidBodyConfig::Reset() (FableWin.pdb name); the called implicit CRigidBodyConfig::operator=
  declared out of line; Orientation / CMPosition written via Body.CurrentConfig with the matrix result in a named local; the last 2 scheduling
  residues (int setup vs mass fmuls; second resize's lea ecx) fixed only by no-code parentheses moving VC7.1's 81-tuple cuts: 4 nested parens
  round the Z edge length + 1 round the E33 expression (1-3 parens elsewhere, operand orders, flags /G5-/G7 /GB /Ox /Oa /Ow, density as a global did not).
- Headers: overlay hdr/ (see hdr/README.md; new engine/CRigidBody.h). Flags: none (plain /O2).
- Landed spellings proven wrong (dossier sec. 3, Ego_r/FableWin): 0x00A358F0 is C3DMeshObject::GetPositionMatrixFromRHSet (static __fastcall, returns
  CMatrix3x4) -- SetPhysicsMeshBankIndex 0x0075A530 calls it GetMatrixFromRHSet. (The landed CalculateVertices 0x00759EC0 already takes
  CRigidBody::CRigidBodyConfig&; only the draft had it wrong, so it is not in NAME_FIXES.tsv.)
- Repair (verifier findings): Reset citation now FableWin.pdb (not in egor_pdb_names.tsv); comment added on the named resize value
  (same uninitialised slot [esp+0x30] for both resize calls); NOTES CalculateVertices line corrected.

## 0x008548B0


## 0x008548B0 CCreatureAction_SpiritAttack::CreateAttackEffect -- RELOCATION_MATCH (first authoring attempt, ~35 grades)
- What made it match: port onto shared headers (TrollThrowProjectile 0x008C6860 / CreateSkyBeam 0x008DAA80 / Guildmaster 0x008E20F0
  opt-in sets), GetDef body visible-noinline (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY) + GetSubDef noinline => def kept
  in edi; Insert forceinline; and the key fact: the source is `VectorParams.Insert(u_start_pos); VectorParams.push_back(u_end_pos);`
  -- one Dirty store only (both Insert-twice forms keep two stores, verified with noinline / inline push_back probes); the second is
  CArray<pair<ulong,C3DVector>>::push_back (Ego_r.pdb out-of-line instance ?push_back@?$CArray@U?$pair@KVC3DVector@@...@@QAEXABU...@Z)
  called out of line (FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE) while Insert's own push_back is the expanded STLport one
  (overlay opt-in FABLE_RETAIL_CVECTORMAP_INSERT_VECTOR_PUSHBACK). _M_insert_overflow body must stay visible (default; DECL_ONLY
  costs the edi key reuse). tc_spirit = 0 declared inside the GetSubDef if (declaring it at the top moves `xor ebp,ebp` early).
  The CParticleUpdateParameters ctor is called (0x006E24D0): FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE.
- Headers/flags: overlay hdr/engine/CVectorMap.h + NParticleEngine_CParticleUpdateParameters.h (hdr/README.md); no flags/pragmas.
  Local models: CTCCoopSpirit via the engine/_quarantine Fields wrapper (as landed FrameUpdate 0x00854B90); CSpecialAbilitiesLightningSpellDef
  as an empty CParentDefClassBase subclass (only header is the flattened _quarantine one).
- Names proven wrong in the old draft (DOSSIER table): GetDefIndexFromName->GetGlobalIndexFromClassIndex, CreateEffectThing->
  CTCDParticleEmitter::Create, LookupEffect->GetEffectFromString, GetActionUseAnimationGroupName->GetAttackEffectName, GetTargetPoint->
  GetFocalPos, g_SpiritEmitterHelper->MAIN_FOCAL_POSITION_NAME, PEffect->PParticleEmitter. Label 0x0076B120 "std::vector<...>::push_back"
  is likely CArray<std::pair<unsigned long,C3DVector> >::push_back (same bytes; not re-proven from a second source).
## Repair (verifier findings, 2026-11-12) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), test PASS
- Check 4: local `class CCreatureAction_SpiritAttack` removed; the row now includes engine/CCreatureAction_SpiritAttack.h with the new
  overlay opt-in FABLE_RETAIL_CCREATUREACTION_SPIRITATTACK_TYPED (hdr/engine/CCreatureAction_SpiritAttack.h, hdr/README.md). Bytes unchanged.
- 0x0076B120 name: proven CArray<std::pair<unsigned long,C3DVector> >::push_back from two sources (Ego_r.pdb 0x0077A190, FableWin.pdb
  0x017C1EFB/0x025F4840 list only the CArray instance, no out-of-line vector<pair<ulong,C3DVector>>::push_back; the landed body rewrapped
  as the CArray member grades RELOCATION_MATCH, carray_probe/). NAME_FIXES.tsv handed back to respell the landed row + labels + callers.
## Verifier findings not applied
- none

## 0x00958840

## 0x00958840 CAIStateGroup_MinionReactToCrime::Update -- DIFFER(737v724), wf17 author pass 1 (~45 grades + toy probes)
- Ported onto shared headers (OnReplace 0x00958B20 macro set + CINTELLIGENTPOINTER_ASSIGN, CTCPERCEIVETHING, CPLAYERMANAGER/PLAYER,
  CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER/AIACTIONS/SPEECH (GetPlayerManager), CRESOURCELIST+CDEFINITIONMANAGER+CGAMEDEFINITIONMANAGER,
  STLP_LIST(+SIZE_DISTANCE, for CTCFollowed.h), 4 overlay opt-ins in hdr/README.md). CMessageEventDef local model (no shared header): `: CDefClassBase {}`.
  PeekTC on the perceive component uses the explicit id (TCI_PERCEIVE_THING): engine/CTCPerceiveThing.h's retail branch has no StatGetInterfaceType here.
- Fixed vs old draft: GetDef's CCharString is a NAMED local in its own block with `bool found` (retail constructs the name before
  pushing &def; a temporary in the call pushes &def first); IP assignment needs FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (GetPItem/SetPItem);
  first 0x4F block written as `if (GetCreature().GetTC(&p)) p->Set...(true)`; correct PDB names (PeekDefinitionManager, GetWorld, GetPos,
  GetPlayerManager, GetPControlledCreature, AIAction_FindAGuardAndYellForHelp on CAIStateGroupBase).
- Bytes match up to the AddMessage call. RESIDUE (1): VC7.1 pre-computes an INLINE call in an argument list at the start of the statement
  (before the GetWorld call) -- `*def` (CDefPointer::operator*, any inline spelling: operator*, Get(), InternalGetObject, __forceinline,
  out-of-class template def, reference alias, pointer alias -- all hoist; toy probes in $W/probe/p2..p23). Retail loads it into ecx after
  GetPos, which is what a DIRECT field access gives (`*static_cast<const CMessageEventDef*>(def.Object)` -> only the release residue left,
  vZ.cpp; not a legit spelling). Everything after (pos copy, register names in the GetTC blocks, loop arg copy) is the cascade of that.
  RESIDUE (2): the closing ~CDefPointer is the register form (`mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi ; ... mov [eax],edi`)
  for the default Release, POINTEE, SELFDELETING(+RELEASE_DELETES), local-copy-no-zero; `if (Object && --Object->IVRefCount==0)` gives
  in-place dec but reloads Object; `object=Object; Object=0; if(--object->IVRefCount==0) object->Delete();` gives retail's exact shape
  PLUS a `mov [esp+14h],0` store (732v724). Retail = in place, single ecx load, no store.
- Tried without effect: CAIBRAIN_CREATURE_REFERENCE off, GETTC_ORIGINAL_CHAIN / default / HASTC_FORCEINLINE, C3DVector copy ctor with an
  assignment body, pos assigned vs constructed vs const, result/def/found declaration placement, manager or creature locals, separate
  popinion pointers, while-loop form, /Ow, /Oy-.
- Best next idea: find the Lionhead spelling of the def dereference that is not an inline call in the argument list (e.g. AddMessage reached
  through some inline wrapper whose body dereferences the def AFTER the outer arguments, or a CDefPointer layout where the pointee is a public
  member read directly), and a Release body that gives in-place `dec [ecx+4]` with the dead `Object = 0` eliminated; check other retail
  callers of 0x00687730 with a CDefPointer local (e.g. 0x00849140 CallSpouseHome, 0x00982750 SendMessage ctor) for the same arg order shape.

## 0x00958840 CAIStateGroup_MinionReactToCrime::Update -- RELOCATION_MATCH (wf17 author pass 2, 4 grades)
- What made it match: callee-body visibility. Defining FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY (existing shared opt-in in
  engine/CDefinitionManager.h: the same-TU noinline body of GetDef<T>(const CCharString&, CDefPointer<const T>*) const, landed instance
  0x007923C0) lets VC7.1 see the def handle is only written through `out`. Both residues went at once: `*def` is now read at its push
  (after GetPos) and the closing release is retail's in-place `dec dword ptr [ecx+4]` with no store. Pass 1's "inline calls in an
  argument list are hoisted" was the escape of &def, not an operator* fact. An in-row copy of the body (SetDynamicCast or `*out = def`) also matches.
- Headers/flags: pass-1 overlay (4 opt-ins in hdr/README.md) + the shared CDEFINITIONMANAGER_GETDEF_NAME_BODY; no flags. Strict-headers regrade RELOCATION_MATCH.
- Repair pass: added the GetHomeBuilding inline-body justification line (FableWin.pdb, retail lea ecx,[eax+168h]; call 0x00A01B50 at 0x0095884F; copies landed 0x00946400). Still RELOCATION_MATCH (also strict), test PASS.

## 0x00B08250


## 0x00B08250 CPSCSingleSprite::SetTrailPositionsFromSpline -- RELOCATION_MATCH (grade 2 of 3 total)
- What made it match: port off the old local model onto shared headers -- CParticleSystem::TransformPosIfWorld (inline, FABLE_RETAIL_CPARTICLESYSTEM)
  calling the real GetSystemTransform const (old draft named it GetInverseSystemTransform), CMatrix3x4 ZYX_SUMS TransformPoint, CCardinalSpline::GetControlPointCount
  (count = `Max < n() ? Max : n()`), STLport list from stlport/fable_containers.h (default, no STLP_LIST opt-in) clear()/push_back(CSpriteTrailEntry(p,width)),
  callee params as const&/& per PDB. Flags: none. Defines: CENGINEPERSISTENTPRIMITIVEBASE, CCHARSTRING, CPARTICLEEMITTER, DEF_CLASS_BASE, BASE_OBJECT, CARRAY,
  STLP_VECTOR, CPARTICLESYSTEM, CMATRIX3X4, CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS, C3DVECTOR. No header overlay needed. Strict-headers regrade: RELOCATION_MATCH.
- Local padded models (no shared header exists): CPSCSplineBase, CPSCSingleSprite, CEnginePrimitiveSpriteTrail (same as landed 0x00B081A0).
- Name fix: rebuild/manifest/functions.tsv types the params as pointers; PDB: (CParticleSystem&, const CParticleUpdateParameters<K,K>&, float, float), protected.

## 0x006CF530

## 0x006CF530 CTCWife::FindAppropriateGift -- RELOCATION_MATCH (8 grades)
- What made it match: port onto the shared headers (CThing GETTC_EXPANDED, CTCVillage list, CThingBuilding::PeekPSimDefinition by value,
  CTCShop::GetStockList by value, GetDef/GetSubDef bodies visible as in 0x006D0630) -- ~map / iterator++ go out of line by themselves;
  then ONE `budget` local (price loaded into it, then overwritten: slot sharing), cached list end, and the map iterator declared BEFORE
  its cached end (store order begin -> +0x28, end -> +0x34).
- Headers: overlay engine/CTCShop.h with new opt-in FABLE_RETAIL_CTCSHOP_GETSTOCKLIST (declaration of GetStockList 0x00774610). Flags: none.
- Draft names proven wrong (draft only, not landed): GetContents -> CTCVillageMember::GetPVillage, GetPRandomSeed -> CWorld::GetWorldSeed,
  GetStock -> CTCShop::GetStockList, GetGiftDef/GetValueDef -> GetSubDef<CGiftDef>/<CStockItemDef>, Kind/Special -> Type/IsWeddingRing.

## 0x0079D7E0

## 0x0079D7E0 CTCBulletTime::UnPause -- RELOCATION_MATCH (wf17, 8 grades)
- Matched by porting onto shared headers (PlayEffect's macro set + Pause's world chain) and: the GetSubDef template body in the TU;
  CIntelligentPointer<CParticleEmitter>(T*) declared as an explicit specialization (out of line 0x00655318) while the CThing ctor stays
  inline (CThingGraphicArgPasser); emitter held in a NAMED block-local CIntelligentPointer (`lea ecx` after the ctor, not `mov ecx,eax`);
  interpolator as a named reference before the ScreenFilterFadeTo call (GetWorld chain before the PDef loads); priority SCREEN_EFFECT_PRIORITY_SPELL (added to the ARGPASSER enum in the overlay), layer ENGINE_2D_LAYER_SPELLS (full enum via engine/NUISystem_CManager.h).
- Headers: overlay CTCBulletTime.h (_UNPAUSE) + CTCCarrying.h (_SETBYPASSFILTER) + NScreenEffect_CScreenEffectPriorityStack.h (SPELL enumerator); no flags/pragmas.
- Name fixes (NAME_FIXES.tsv, verified): 0x0079CE80 = Ego_r 0x007A2910 / FableWin 0x01822643 ?PlaySoundW@CTCBulletTime@@AAEKABVCCharString@@_N@Z (landed leaf QAEKKK and the 0x0079D110 decl (const CCharString&, unsigned long) are wrong);
  0x0079DE00 is NOT CTCBulletTime's: byte-identical propagated label, really ?PlaySoundW@CTCPhysicalShieldSpell@@IAEKABVCCharString@@_N@Z (Ego_r 0x007A37C0, FableWin 0x01811122; callers CancelMove 0x0079E300 / FrameUpdate 0x0079EB30).
- Repair (verifier round): ENGINE_2D_LAYER_SPELLS + SCREEN_EFFECT_PRIORITY_SPELL spelled; GetSubDef<T> body comment names landed 0x00669580 and the retail fact
  (probe: dropping the body -> DIFFER 743v732, 45 hunks: frame 0x4C, no ebp cache of `effects`). Still RELOCATION_MATCH, strict too.
## Verifier findings not applied
- #4 asked to record 0x0079DE00 as CTCBulletTime::PlaySoundW; it is CTCPhysicalShieldSpell::PlaySoundW (evidence above), recorded as such.

## 0x00977E00


## 0x00977E00 CAIStateGroup_Guildmaster::Update -- RELOCATION_MATCH (6 grades)
- What made it match: port onto shared headers (GETTC_EXPANDED, CDefPointer<const CGuildMasterDef> + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE visible-but-out-of-line GetSubDef, PeekBaseDef, GetWorld/GetConstantFPS/GetFrame order, by-ref action temporaries) fixed the old 729v732 def/frame residue at once; then shield cancel = single `if (GetCreature().GetTC(TCI_PHYSICAL_SHIELD_SPELL,&shield)) shield->CancelMove();` (no outer HasTC), and the final test is `if (!IsStillRunnable()) { Terminate(); return; }` (keeps retail's duplicated def-release epilogue).
- Headers/flags: overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_GUILDMASTER, FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO (hdr/README.md); default flags. Local classes only where no header exists: CGuildMasterDef (as landed 0x00978190), the three CCombatAction_Guildmaster_*FollowersInto (ctor declarations only).
- Draft names proven wrong (dossier §3/§7): WillEnflameInto->WillShieldInto, CountLivingHelperCreatures->CountLivingHelpers, GetPWorld->GetWorld, GetWorldFrame->GetFrame, CGuildmasterDef->CGuildMasterDef. Landed TeleportOut ctor row 0x008BCFB0 spells pointer args; PDB is refs (non-const C3DVector&) -- NAME_FIXES candidate (not changed here).

## 0x00854DD0 (not landed)

## 0x00854DD0 CCreatureAction_SpiritAttack::PerformAction -- DIFFER(697v720), 17 hunks (wf17 author, ~50 grades)
Best: differ/00854dd0.cpp (= v57). Ported onto shared headers: CThing (+_GETTC_EXPANDED, _ORIGINAL_VTABLE for OnHit slot 65),
CThingPhysical (+_ISINRANGE), CIntelligentPointer (+_ASSIGN: temp ctor + SetPItem/GetPItem assignment + dtor), CTCTargetingBase,
CHitParameters (+_CTOR, overlay _HIT_CTOR), C3DVector (+_SQUAREDMAGNITUDE_ZYX), CCreatureActionBase (+_PCREATURE);
CTCCoopSpirit = FrameUpdate-sibling wrapper over _quarantine fields (StatGetInterfaceType 0xC7, GetMeleeTargetRange/AddExperience/OnStrike).
What fixed the big register residue (57 -> 20 hunks): ONE `CTCCoopSpirit* tc_spirit` declared at function top and reused by both
GetTC(0xC7) calls (edi=this spilled, ebx=8 constant then spirit -- the whole prologue/first half now matches).
Range: `float range = GetMeleeTargetRange(); CTCPhysicsBase* physics = target->GetPhysicsTC(); CThingCreatureBase* creature = PCreature.Get();`
gives retail's call order (range call, PhysicsTC load, GetPItem, radius) -- only the fadd operand order stays swapped.
Remaining residue: (1) `fld radius; fadd range` vs retail `fld range; fadd radius` (0x00854ED2; operand swap / range+= / GetRadius
deferred all canonicalise to the same); (2) both C3DVector subtractions: ours Z,Y,X (ctor args right-to-left), retail X,Y,Z keeping X/Y
on the x87 stack and spilling only Z (`fst [esp+28h]; fmul [esp+28h]`); (3) final GetTC(0xC7) through ebx vs retail eax + tail dup.
Ideas tried (no match): /Op /EHsc /G5 /G7 /Ob1, optimize("s"); default GetTC (HasTC+Find); separate spirit vars (57 hunks);
range local variants (v12/26/27/28/54/56/58 break regalloc unless creature local also added); operator- memberwise overlay (exact
X-first + Z-spill subtraction, but the named result is COPIED into direction: 22-25 hunks); copy+`-=`, friend operator-, float-locals
operator-, explicit per-component blocks (X first but stores X/Y immediately or keeps all three in regs); ctor assign/forceinline;
Newton paren variants; NORMALISE_LOCALS(_XYZ); SQUAREDMAGNITUDE XYZ/default; (a-b).Normalise() (const return, does not compile).
Best next idea: find the operator- / direction spelling that evaluates X first AND constructs straight into `direction`
(memberwise overlay gets the subtraction bytes exact; the only gap there is the result->direction copy) -- e.g. a non-inline-at-c1
memberwise helper, or a direction built by a CThing helper (check FableWin for a GetDirectionTo-style inline on CThing/CThingPhysical);
the same X-first issue is open in wf13 0x00951D60 (operator+), solved there with a per-component block.
Names: GetPTargetedThing (not GetTarget), GetMeleeTargetRange (landed 0x00670000 is mislabelled CTCPhysicsBase_GetAirResistance), OnStrike
(not OnHit), OnHit vslot 65 (not Hit), IsHitNegated (not Deflected) -- per DOSSIER, confirmed in Ego_r/FableWin signatures.

## 0x00854DD0 attempt 2 -- DIFFER(710v720), 9 hunks (was 17); ~35 grades + 4 probes
Best: differ/00854dd0.cpp (= A2.cpp). Same headers/opt-ins as attempt 1 (no new opt-in used); no flags.
What improved it (two source facts):
1. Tail (eax + duplicated epilogue, 0x0085507B/0x0085508D) MATCHES: the first spirit TC `CTCCoopSpirit* tc_spirit` declared inside the
   target block, and the final strike through a separate `CTCCoopSpirit* tc_strike` at function end (no longer a shared variable).
2. Hit position MATCHES (X, Y, Z computed and stored one at a time, interleaved with the pushes, into [esp+14h]): the position is
   written member by member from `const C3DVector& creature_pos = PCreature->GetPos()` into a named `C3DVector hit_pos`, and an extra
   block `{ ... }` encloses hit_pos + CHitParameters + OnHit + AddExperience (without that block hit_pos gets its own slot, frame 0x84).
   The block encloses only the hit, NOT direction/CreateAttackEffect (those placements give 23 hunks).
Remaining (9 hunks, two places): (1) `fld [physics+34h] ; fadd range` vs retail `fld range ; fadd [physics+34h]` (0x00854ED2);
(2) direction subtraction Z,Y,X (all on the x87 stack) vs retail X,Y,Z with only Z homed (`fst/fmul [esp+28h]`) and the scaling's
`fxch st(1)` / `fld [esp+28h]`.
Probe facts: (a) with the hit removed (probe 2) or with direction copied before the hit (probe 4) the subtraction becomes X-first: the
Z-first order holds only because direction is address-taken by the CHitParameters ctor; (b) the Normalise sum spelling (ZYX/XYZ/default/
UNPAREN/SQUARE_HELPER/LOCALS) and Newton paren variants never change the order; (c) float locals x,y,z in any order + ctor/Initialise/
member stores: X always ends up computed last (on top) or X/Y get stored eagerly (member stores, 10 hunks); (d) 1-3 dead loops (empty or
unrolled 2-trip, wf15 lesson) change nothing; (e) the QFE-4035 compiler gives the same bytes.
Also tried (no gain): XY_GROUP, implicit copy ctor, operator- / ctor defined out of class, plain-inline arithmetic (Normalise then goes out
of line), const / const-ref ctor params, memberwise operator- (+COPY_ASSIGN_BODY), copy+`-=`, /G6 /G7 /Ox, range spellings on the new
base (`range +=`, distance local, const range, creature_pos ref local, fully inline expression -> regalloc collapses to 54-57 hunks).
Best next idea: the direction is the only open x87 question. Retail behaves as if the three differences were scalar temporaries computed
X first BEFORE direction's address escapes, with direction.Z stored once (its home) -- look for a spelling where direction is not yet
address-taken when built, e.g. the hit built from a by-value/copied direction that VC7.1 coalesces, or the CHitParameters ctor reached
through an inline helper of the TU taking `C3DVector` by value; then re-check whether the fadd order flips with it.

## 0x00854DD0 attempt 3 -- DIFFER(710v720), 9 hunks unchanged (best legit source still differ/00854dd0.cpp); ~25 grades + ~12 toy probes
Two level-changing facts found with toy compiles (cl /O2 /FAs on a minimal replica; see ATTEMPTS.log probes A3-1/A3-2):
1. fadd order (0x00854ED2): VC7.1 emits `fld range ; fadd [physics+34h]` ONLY when the radius is an lvalue (direct `physics->Radius`
   or a `const float&` getter). Any by-value inline getter (GetRadius in-class, deferred, forceinline, const return, local copy, cast,
   parens, flags /Op /O1 /G7 /Oa /Ow) gives `fld [physics+34h]` first. PROBE m1 (overlay opt-in FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_PUBLIC_PROBE
   making Radius public + `range + physics->Radius`) FIXES this hunk -> 8 hunks (differ/00854dd0_radius_lvalue_probe.cpp). Not adopted:
   Ego_r/FableWin say ?GetRadius@CTCPhysicsBase@@QBEMXZ (by value) and Radius is protected in the header -- needs a second source for
   the access (friend? public member in the retail header?) before it can land.
2. direction (0x00854F14..): X-first + only-Z-homed + the `fstp f; fxch st(1); fmul f; ... fld [z]` scaling appears ONLY for a vector
   whose address is never taken. `C3DVector offset = a - b; C3DVector direction = offset; direction.Normalise();` (x2,
   differ/00854dd0_offset_copy_probe.cpp) gives retail's exact instruction sequence but offset gets its own slot ([1c..24]) and direction
   moves to [28] (frame 0x84, 20 hunks). Retail homes Z at direction.Z ([28]) itself, so retail's vector is one object at [20..28].
Also tried (no gain): hit gets C3DVector(direction) / named hit_dir copies (21-24 hunks); temp-normalise `C3DVector(a-b).Normalise()`
(+IMPLICIT_COPY / COPY_ASSIGN_BODY); default ctor + assign; offset in a block; creature-pos ref then copy+`-=` (SUBTRACT_ASSIGN, call
order right but copies); parens/casts as no-code tuples; radius local/distance local/`+=` forms; fully inline range expressions
(54-57 hunks); range declaration placement.
Best next idea: find the source shape in which retail's single direction object at [20] is NOT address-taken while the subtraction is
built (e.g. a TU-local inline helper that returns the hit direction by value into the CHitParameters argument slot, or a hit built from
a by-value C3DVector that VC7.1 constructs in place at [20]); and find a second source for an lvalue radius access (header friend /
public Radius) -- with both, the row should be 0-1 hunks.

## 0x0075A7A0 (not landed)


## 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions -- RELOCATION_MATCH (~60 grades)
- Made it match: the body is Chris Hecker's rigid-body ResolveCollisions (named Position copy, numerator spelled `-(1.0f + e) * v.Dot(n)`,
  impulse `n * (num / den)` with NO named j) + header opt-ins FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX and NEW overlay opt-in
  FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP (operator^ with X and Z components parenthesised). The last residue (one `lea ecx,[esp+2c]`
  scheduled late) was the numerator spelling; `-(dot * (e + 1))` and named j both leave it.
- Model (repair 2026-11-12): CMatrix3x4 from the shared engine/CMatrix3x4.h + overlay opt-in FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR
  (declares the landed 0x00A56530). CTCPhysicsRigidBody / vectors still local (see below); CRigidBody / CRigidBodyConfig have no header. CopyVectors/CopyTrivial/CVectorVector/CLongVector are Simulate's
  descriptive (made-up) names for the inline STLport clear() forms. No flags. Overlay: hdr/engine/C3DVector.h.
- Names: manifest label correct (Ego_r + FableWin `?ResolveCollisions@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z`). No NAME_FIXES.

## Verifier findings not applied (repair 2026-11-12)
- Finding 1 (local `class CTCPhysicsRigidBody` while engine/CTCPhysicsRigidBody.h exists): the verifier is right that the header carries
  the retail offsets (+0x40/+0x4c/+0xa0, sizeof 0x258); the row comment saying otherwise is corrected. But the header cannot be used
  without losing the match: its flat branch has opaque byte members only, and its typed branch (FABLE_RETAIL_CTCPHYSICSRIGIDBODY) holds
  real std::vector<C3DVector>/<long> from stlport/fable_containers.h. With those (repair_diag/v_hdr1.cpp, with the trial overlay repair_diag/CTCPhysicsRigidBody.h opt-in
  FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY typing Body + declaring the member) the row grades DIFFER(729v726): VC7.1 drops retail's second
  induction slot ([esp+14h] i beside [esp+10h] byte offset) and no longer holds &config.AngularMomentum / &config.AngularVelocity in
  ebx/ebp (0x0075A7CB/0x0075A7CE). Isolation on the matching source: only CollisionNormal as std::vector breaks it (d3 index vector,
  d5 BoundingVertices: RELOCATION_MATCH; d4: 729v726); not the clear()/_Destroy path (d12 trivial-dtor traits: still 729) and not the index
  type; the single cause is STLport's operator[] going through begin(): local `C3DVector& operator[](long n) { return *(begin() + n); }`
  = 729v726 (d19), `{ return *(_M_start + n); }` = RELOCATION_MATCH (d20/d21). Tried with the std::vector form without success: begin()[i],
  a named normal pointer/reference/copy, const operator[], unsigned/int counts, every CROSS_* x DOT_* x SCALE_X_PAREN opt-in (50 grades),
  numerator/impulse/position spellings x cross/dot (40 grades), STLport fastcall/forceinline opt-ins, inline_depth 2..6, /Ob1 /Oa /Ow.
  Same model as the landed same-TU siblings CalculateVertices 0x00759EC0 and Simulate 0x0075AD50. Open lead: whatever makes retail's TU
  see CollisionNormal[i] without begin()'s extra inline level (a different STLport 4.6 configuration?) -- needs the real _vector.h.

