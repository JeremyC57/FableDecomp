## 0x008B0B60


## 0x008B0B60 CCombatSequenceGeneric::OnCreateSpecial -- RELOCATION_MATCH (strict headers), source final.cpp, test test.cpp (vt.py PASS)
- What made it match: the port onto the shared headers (engine/CCountedPointer.h, CDefString.h, CDefClassBase.h, CDefPointer.h,
  stlport map/vector, engine/CCombatSequenceGeneric.h) removed the old "this in edi / zero in ebx" residue by itself (479v432), then:
  (1) FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR -- the CCountedPointer<CCombatSequenceBase>(T*) ctor 0x006EF6F0 is called but its
  body is visible, so Data/Info stay in edi and the release uses the dec flags; (2) FABLE_RETAIL_STLP_TREE_CONSTFIND (inline const
  find -> _M_find 0x006F1360, with FABLE_RETAIL_STLP_TREE_MFIND_DECL); (3) `CCharString name = Def->GetInstantiationName().GetString();`
  (the conversion operator costs a copy ctor + dtor); (4) the creator read into a local before the call; (5) `EActionRegisteredType
  type = *it;` before `CCombatAction action;`-use (enum read before creature/target). Loop tail = PushAction(action) inlined (real
  landed body of 0x008B0B10 in the row file; the two lines written out grade the same up to that point).
- Headers / flags: no compiler flags. hdr overlay: engine/CCombatSequenceGeneric.h (FABLE_RETAIL_CCOMBATSEQUENCEGENERIC_ONCREATESPECIAL),
  engine/CCombatActionBase.h (FABLE_RETAIL_CCOMBATACTIONBASE_GETPCOMBATACTIONFROMENUM, default class branch; needs
  FABLE_RETAIL_BASE_OBJECT + FABLE_RETAIL_CCREATUREACTIONBASE for the 0x114 size assert). See hdr/README.md.
- Local models (no shared header exists): CCombatSequenceBase, CCombatSequenceDef (CDefClassBase + ActionList +0x54), CCombatAction.
- Descriptive names (not in any PDB): typedef TSequenceCreator, parameter `specials`, locals.
- Ideas graded (11): PushAction call vs two inline lines (same); conversion-operator / direct-init / cast spellings of the name (459v432);
  local CCombatActionBase vs shared header (both match; shared header kept); FABLE_RETAIL_LEADER_INITIALISE branch of
  CCombatActionBase.h does not compile in this TU (SRC_FAIL) -- use the default branch opt-in.
- Landed names shown wrong by this row (see NAME_FIXES.tsv): 0x006EF6F0 landed as `CCountedPointer_006ee3d0::CCountedPointer_006ee3d0(void*)`
  is CCountedPointer<CCombatSequenceBase>::CCountedPointer(CCombatSequenceBase*) (its only argument here is the PDB-typed creator's
  CCombatSequenceBase* result; FableWin.pdb has the CCountedPointer<CCombatSequenceBase> instantiation); landed 0x008B0700 GetName calls
  def vslot 10 `GetNameStringId` -- it is CDefClassBase::GetInstantiationName (Ego_r ?GetInstantiationName@CDefClassBase@@UBE?AVCDefString@@XZ,
  engine/CDefClassBase.h slot 10 0x0042DA10).
- Repair (verifier findings): the special-path copy is now `CloneActions(*sequence)` with the real body of the PDB-named protected helper
  CCombatSequenceBase::CloneActions (Ego_r ?CloneActions@CCombatSequenceBase@@IAEXABV1@@Z 0x008EC0D0, FableWin 0x02A37EB0; no out-of-line
  body in Fable.exe, so inline) -- still RELOCATION_MATCH, also strict headers; test mirrors it (vt.py PASS). NAME_FIXES.tsv now hands back
  0x008B0B10 = CCombatSequenceBase::PushAction(CCombatAction&) (landed spelling AddAction(const CCountedPointer&) is wrong).

## 0x009486D0


## 0x009486D0 CAIStateGroup_GoToPub::FreeSeat -- RELOCATION_MATCH (4 grades: 1 SRC_FAIL pair, a1/a2 432v432 3 hunks, b2 match, final + strict)
- What made it match: the port onto the shared headers (engine/CThing.h GETTC_EXPANDED, CIntelligentPointer.h retail + _ASSIGN,
  CAIStateGroupBase/CAIBrain GetCreature(), CTCCarrying.h, CTCTavernTable.h, the retail CThing chain defines so CThingObject /
  CThingAICreature derive CThing) with the Ego_r member names fixed the old "-1 hoisted into ebp" residue by itself (436 -> 432,
  3 hunks left). The last 3 hunks were evaluation order: the carried thing goes into a local first
  (`CThing* held = carrying->GetThingInPrimarySlot(); if (held == PTankard.Get())`). `PTankard = 0;` and the explicit
  `CIntelligentPointer<CThing>(0)` temporary grade the same.
- Headers / flags: no flags, no pragmas. Overlay opt-in `FABLE_RETAIL_CTCTAVERNTABLE_FREESEAT` in hdr/engine/CTCTavernTable.h
  (declares CTCTavernTable::FreeSeat(long,long) 0x007B8AA0). Strict-headers regrade passes.
- Local declarations (no shared header exists): CAIStateGroup_GoToPub (Ego_r names/offsets), CTCTavernTankard (ReplaceTankard
  0x00947C30, declaration only). IS_THING_ALIVE is the retail template from engine/ThingAlive.h.
- Tried and rejected: `PTankard.Get() == carrying->GetThingInPrimarySlot()` (436v432, 17 hunks).
- Names proved wrong in the old draft (not landed spellings, nothing for NAME_FIXES): CTCSeatingArea -> CTCTavernTable, PSeating -> PTable,
  SeatAreaIndex -> SeatIndex, SeatIndex(+0x64) -> ActionPointIndex. Leaf is protected (IAE), not QAE.
- Repair (verifier findings): the row-local non-template IS_THING_ALIVE was replaced by engine/ThingAlive.h
  (FABLE_RETAIL_IS_THING_ALIVE + FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT for the retail pointer != 0 test),
  and _pad40[0x24] by the Ego_r members C3DVector Pos (+0x40) / CRightHandedSet RHSet (+0x4C) from the shared headers.
  Still RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1; test updated to the template spelling, vt PASS.

## 0x0094F480


## 0x0094F480 CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable -- RELOCATION_MATCH (author pass, 15 grades)
- What made it match: (1) the port onto the shared headers; (2) slot 29 (+0x74) of the targeting component is
  `CTCTargetingBase::GetPBestEnemyCreatureForAttack() const` (returns CThingCreatureBase*), NOT GetPTargetedThing (that is slot 33,
  +0x84, by the header's PDB order and the CTCTargetingAI / CTCTargetingPlayer vtables); (3) one nested condition with a single
  `return false` (separate early returns gave the first test its own epilogue and a late `push edi`); (4) the default C3DVector
  branch (no FABLE_RETAIL_C3DVECTOR: one-line GetSquaredMagnitude2D loads Y first); (5) the creature read through the inline
  `CAIStateGroupBase::GetCreature()` at EVERY use and no creature local -- a `CThingAICreature*` local (any placement) or
  `PBrain->Creature` spelled directly made VC7.1 share one Creature load between the toEnemy statement and the ahead statement
  (retail loads it twice from one PBrain register and keeps the second in ebx across the GetFacingVector call).
- Needs: overlay header hdr/engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h (opt-in FABLE_RETAIL_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY);
  repo opt-ins FABLE_RETAIL_CTHING_ORIGINAL_VTABLE (GetMaxRunningSpeed slot 81), FABLE_RETAIL_CTHING_GETTC_EXPANDED,
  FABLE_RETAIL_STLP_BITSET_INDEX, FABLE_RETAIL_CTCPHYSICSBASE(+_LASTPOSITION), FABLE_RETAIL_CAISTATEGROUPBASE(+_MEMBERS,_GETCREATURE),
  FABLE_RETAIL_CAIBRAIN_GETCREATURE, FABLE_RETAIL_CINTELLIGENTPOINTER(+_CTOR_OUTOFLINE,_ASSIGN_OUTOFLINE). No flags, no pragma.
  Strict-header regrade (LANDVERIFY_STRICT_HEADERS=1): RELOCATION_MATCH. Test: vt.py PASS.
- Ideas that did NOT match (do not repeat): separate early `return false`s (441v437); creature local after / before toEnemy,
  as pointer or as reference (434v437, Creature load shared in ebx); no local with PBrain->Creature spelled directly (439v437,
  this in ebx, GetPos's this reloaded after the call); local used only for GetPos (436..442); GetCreature() in the ahead
  statement only (437v437, X add commuted); FABLE_RETAIL_C3DVECTOR (2D magnitude loads X first).
- No made-up names. Name fixes: see NAME_FIXES.tsv (0x004DDAE5 vfunc29; the DOSSIER's / TrollBase row's "slot 29 = GetPTargetedThing").

## 0x005F6793


## 0x005F6793 CTCInventoryAbilities::CycleAssignedExpressions -- RELOCATION_MATCH (9 grade calls, 20 variants)
- What made it match: the REAL landed body of GetPrioritisedExpressionDefs (0x005EED8C, same class TU) in the row file plus
  `FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE` (push_back body visible, still called). VC7.1 then sees that nothing captures the
  local vector and hoists `expressions.size()` out of the do-loop by itself (slot [ebp-10h]), which also gives retail's register
  pick (zero = esi, button = ebx) and `mov ecx,ebx ; lea ebx,[ecx+eax*4]`.
- Source shape: twin of CycleAssignedAbilities 0x005F64DD with the empty test BEFORE the reset loop, no clamp, no modulo line.
- Flags: `/O1 /Oy-`. Headers: repo headers only, no overlay opt-in added (hdr/ is empty).
- Tried and rejected: declaration-only callee (444v442, size recomputed in the loop); hand-hoisted `long n = expressions.size()`
  (440v442: zero/button registers swapped, `lea ebx,[ebx+eax*4]`); `it=it+n` / `begin()+n` / `&expressions[n]` (no change);
  if(!empty){} wrap (436); twin statement order (444); GetPrioritisedExpressionDefs body with declaration-only push_back (444);
  GetNumAssignableSpecialMoveButtons body visible (444).
- Name fix candidate: landed OnInitialActivate 0x005F6B47 declares `CycleAssignedExpressions(long direction)` and
  `CycleAssignedAbilities(long)` locally; both PDBs say `(bool)` (`QAEX_N@Z`). Written to NAME_FIXES.tsv.
- Descriptive name used: CASoundManager::Get2DEmitter() (vtable +0x24) is a made-up descriptive name (no original found in any
  PDB/FSE/EgoCore; marked so in rebuild/include/engine/CASoundManager.h). Every other name is the exact PDB spelling.
- Repair (verifier findings, both applied): (1) the descriptive Get2DEmitter is now flagged in the source Callees comment and
  here; (2) the guarded inner do-while is justified in the source: retail 0x005F6862 `and [ebp-4],0 ; call 0x5EDCF0 ; test eax,eax ;
  jle 0x5F6897 ; mov eax,ebx ; jmp 0x5F6878` (one-time guard then a jump into the body, same as the landed twin 0x005F64DD); a plain
  for loop grades 441v442 (rep/forloop.cpp). Source bytes unchanged, still RELOCATION_MATCH (also with LANDVERIFY_STRICT_HEADERS=1).

## 0x00847DF0


## 0x00847DF0 CCreatureAction_UseLamp::Initialise -- RELOCATION_MATCH (wf7 author, 4 grades)
- What made it match: the port onto the shared headers plus three source facts: (1) the animation entry is bound to a reference local
  (`const CAnimationEntry& anim = PCreature.Get()->GetRandomAnimFromNameWithFilter("LIGHT_LAMP", filter); SetAnimation(anim, -1, 1);`),
  so the "LIGHT_LAMP" temporary dies before the pushes; (2) the module's reference arguments are `*PCreature, *PObjectToUse`
  (operator*, FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE) -- VC7.1 then evaluates lamp, creature, duration and pushes afterwards
  (the old draft's raw GetPItem() calls gave right-to-left); (3) CAnimFilterOfGroup has NO user-declared destructor (an inline
  `virtual ~CAnimFilterOfGroup() {}` adds a derived-vptr store before ~Group that retail does not have) and its inline ctor is
  `Group = group; GroupCRC = group.GetCRC();` with CCharString::GetCRC inline (FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE).
  The guard is IS_THING_ALIVE written out (Get() != 0 && !Get()->IsDead()) then the const PeekTC(TCI_ON_ACTION_USE, &action_use).
- Headers / flags: no flags, no pragmas. Overlay $W/hdr (see hdr/README.md): CCreatureActionBase.h (_SETANIMATION,
  _GETTOTALTIMEWORLDFRAMES), CAnimFilterOfGroup.h (_INLINE_CTOR), CTCActionUseBase.h (_ANIMGROUPNAME), CCreatureAction_UseLamp.h
  (FABLE_RETAIL_CCREATUREACTION_USELAMP). IsPlayerCharacter comes from FABLE_RETAIL_CTHING_ORIGINAL_VTABLE. Strict-header regrade passes.
- Local declarations (no shared header exists): NCameraModes::CScriptModuleBase / CScriptModuleUseLamp (ctor 0x00853120 only).
- Names: no made-up names. Landed spellings proved wrong (see NAME_FIXES.tsv): the landed 0x00853120 row declares the ctor with
  `const CThing*` parameters; the PDB (??0CScriptModuleUseLamp@NCameraModes@@QAE@ABVCThing@@0J@Z) has `const CThing&` (same ABI). The old
  draft's IsPlayerControlled / GetGameContext / GetCameraEnvironment / CScriptModuleOpenChest / CAnimFilterGroupName were wrong (dossier).

## 0x0097D6F0


## 0x0097D6F0 CAIStateGroupBase::AIAction_LookAtThing -- RELOCATION_MATCH (wf7 author, 7 graded variants in 3 grade calls + final + strict)
- What made it match: the port onto the shared headers by itself fixed the old 422v444 residue (GetPos returns `const C3DVector&`,
  GetFocalPos returns a C3DVector through a hidden temp, so the if/else `pos =` assignments keep one 12-byte copy per arm; a ternary
  initialiser tail-merges them, 424v444). The last hunk (`cmp esi,eax` v `cmp eax,esi`) was the operand order of the self test:
  `thing.Get() == &GetCreature() || GetCreature().IsPosInViewAngle(...)` (thing on the LEFT; the creature is still read first into esi
  and re-read for the call because GetCreature() is spelled twice, not cached in a local).
- Headers / flags: default lane flags, no pragma. Overlay hdr/engine/CThingCreatureBase.h adds
  FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE (declaration only). Uses engine/EAIAction.h (PDB enumerators), CThing.h (FABLE_RETAIL_CTHING
  chain), CThingAICreature.h, CAIBrain.h (_SUSPENDABLE_BASE, _SETOUTPUT, _GETCREATURE), CAIStateGroupBase.h (_MEMBERS, _AIACTIONS,
  _GETCREATURE), ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE + CINTELLIGENTPOINTER_COMPARE_NULL_INT), CCountedPointer.h (default dtor).
  Strict-headers regrade: RELOCATION_MATCH. Test: vt.py PASS.
- Local classes (no shared header exists): CBrainOutput, CNavigationInput (Ego_r.pdb layout, same as landed 0x0097D580; ctor 0x00835620 and
  SetAsLookAtThing 0x0093E760 declared only, implicit destructor inlined). A shared engine/CBrainOutput.h would be worth making.
- Names proved wrong in landed code (see NAME_FIXES.tsv): 0x006650E0 is `CThingCreatureBase::IsPosInViewAngle` const (Ego_r.pdb 0x0053F1B0 +
  FableWin.pdb `?IsPosInViewAngle@CThingCreatureBase@@QBE_NABVC3DVector@@M_N1@Z`; the dossier matched the body), not "CanSeeTarget" /
  "IsLookingAtPos"; brain vtable slot 2 is `SuspendSuspendableProcess` (CSuspendableProcessMicrothread.h), not "Yield";
  0x0088B8F0 is `CAIBrain::SetOutput`, not "SetBrainOutput"; 0x0093E760 takes `const CThing&`; 0x004C73D0 is `CThing::GetPos`.
  The header's parameter names for this function ("frames", "speed") are really the look flags and the view angle (names only).
- No made-up names in this row.
- Repair (verifier round): NAME_FIXES.tsv now carries all three proved names -- 0x006650E0 IsPosInViewAngle, 0x0088B8F0
  `CAIBrain::SetOutput` (?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z, Ego_r.pdb 0x0082F690 + FableWin signatures; landed
  spelling `SetBrainOutput`) and 0x00A44840 `CSuspendableProcessMicrothread::SuspendSuspendableProcess` (brain vtable slot 2,
  ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ, Ego_r.pdb 0x00B9E450 + FableWin signatures; landed
  spelling `Yield`), each listing the landed files to respell. CNavigationInput::MovementType is now `EMovementType`
  (engine/EMovementType.h, PDB type of +0x10); still RELOCATION_MATCH (also strict headers), vt.py PASS. The landed 0x0097D580
  has the same `long` simplification (for the coordinator).

## 0x006AD3C0


## 0x006AD3C0 CThingPlayerCreature::ApplyRelativeMovementAcceleration -- RELOCATION_MATCH (wf7 author, 5 grades + strict regrade)
- What made it match: the port onto the shared headers (every callee renamed per the dossier; GetCurrentRenderCamera returns the
  0x38-byte CCamera, which gives the 0x68 frame) got to 438/441 v 447; then two source facts: (1) `const CTCPhysicsBase* physics =
  PhysicsTC;` read BEFORE `mode_manager->GetCurrentMode()` (retail `mov ebp,[edi+60h]` ahead of the call; the mode vtable is then
  spilled to the dead argument slot); (2) `acceleration * scale` written in BOTH arms of the find test (`it->second.first +=
  acceleration * scale` / `insert(value_type(action, pair<C3DVector,long>(acceleration * scale, 1)))`) instead of one named
  `scaled` vector -- VC7.1 hoists the products, keeps X/Y on the x87 stack and stores only Z. Plain `direction.RotateXY(angle)`
  (no `float&` lever needed here, unlike sibling 0x006A9690).
- Grades: v1/v2 named scaled, RotateXY direct / float& (438 v 447, wrong slot 0x48 for GetMaxAcceleration); v1b +
  FABLE_RETAIL_CTHING_ORIGINAL_VTABLE (441 v 447, slot 87 right); v3 physics local + `const C3DVector scaled` (471 v 447);
  v4 physics local + inline products = RELOCATION_MATCH.
- Headers / flags: no compiler flags. Overlay $W/hdr/engine/CThingPlayerCreature.h adds opt-in
  FABLE_RETAIL_CTHINGPLAYERCREATURE_ACCELERATIONMAP (RelativeMovementAccelerationComponents +0x1a8 typed as
  std::map<EGameAction, std::pair<C3DVector,long> >, includes stlport/fable_map.h; default path unchanged). The row also needs
  FABLE_RETAIL_CTHING_ORIGINAL_VTABLE (base declares GetMaxAcceleration at retail slot 87; VSLOTS2 alone puts it at +0x48),
  FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT / _VSLOTS2, FABLE_RETAIL_CCAMERA_ANGLES, FABLE_RETAIL_CMODEBASE_VIRTUALS,
  FABLE_RETAIL_CTCCREATUREMODEMANAGER. map::find / map::insert stay out of line with the default fable_map.h (no STLP opt-in).
- Names: no made-up names. Landed spellings proved wrong: none new (the dossier's callee table stands; sibling 0x006A9690's local
  "GetMaxSpeed" for slot 87 is GetMaxAcceleration and its "SpellMode" +0x220 is CPlayer::UsingFreeCamera in the shared header --
  noted by the scout, not re-proved here).
- Test: $W/006ad3c0_test.cpp (vt.py PASS): new entry scaled by speed/max, accumulation + count, negligible speed keeps scale 1 and
  skips GetMaxAcceleration, camera-angle choice, idle counter cleared.

## 0x007735D0

# 0x007735D0 CTCShop::GetAttitudeBuyPriceMultiplier -- MATCH (RELOCATION_MATCH, strict headers too)

Source: $W/007735d0.cpp, test: $W/007735d0_test.cpp (vt.py PASS), payload.json / oracle.tsv written by addmatch.py. Flags: none.

What made it match (two steps):
1. Port onto the shared headers as one body (no invented __forceinline helper): PeekTC(TCI_BUYABLE_HOUSE,&house) + GetOwnedByPlayer(),
   the GetCurrentBuyPrice def prefix verbatim, GetShopKeeper() three times, PeekWorld().PeekOpinionReactionManager(), the header
   std::map const find (declaration-only), PeekPlayerManager().GetMainPlayer().GetPControlledCreature(), GetTC(TCI_HERO_STATS,&stats).
   That alone still gave the parked draft's register picture (430v448: this=edi, ParentThing/thingDef=ebp, zero=esi).
2. The lever: in the keeper block the opinion manager and the keeper are NAMED REFERENCES declared in that order before the call:
       const COpinionReactionManager& manager = PeekWorld().PeekOpinionReactionManager();
       const CThing& keeper = *GetShopKeeper();
       EOpinionAttitudeType attitude = manager.GetThingAttitudeToPlayer(keeper);
   The manager reference alone (v20/v21/t9) or a keeper-pointer local changes nothing; the `const CThing& keeper` local is what
   moves this -> ebp, ParentThing -> edi, zero -> ebx, thingDef -> esi and swaps the two def slots (thingDef frame+0, shopDef +4).
   A `const CWorld& world` local in front (t7) matches as well; t8 (without it) is the landed spelling.

Headers / macros it needs: the GetCurrentBuyPrice set plus FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER, FABLE_RETAIL_CTHING_GETTC_EXPANDED,
FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CTHINGPHYSICAL / _CTHINGGAMEOBJECT / _CTHINGCREATUREBASE / _CTHINGPLAYERCREATURE,
FABLE_RETAIL_CTCSHOP_SHOPKEEPER, FABLE_RETAIL_CTCBUYABLEHOUSE, FABLE_RETAIL_CTCHEROSTATS, FABLE_RETAIL_CWORLD,
FABLE_RETAIL_CPLAYERMANAGER, FABLE_RETAIL_PLAYER, and two NEW opt-ins in the overlay ($W/hdr, see hdr/README.md):
FABLE_RETAIL_CTCHEROSTATS_TRADESKILL (engine/CTCHeroStats.h: declares GetTradeSkill 0x0057C239) and
FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYER (engine/COpinionReactionManager.h: the PDB const `GetThingAttitudeToPlayer(const
CThing&) const` without the rest of FABLE_RETAIL_CTCWIFE_MARRY).

Names: g_TradeSkillAttitudeScale (float at 0x01382998, .data, 0.84f; read only by this row at 0x00773748 and by its Sell twin 0x00773790 at 0x00773908) is a
DESCRIPTIVE made-up name (found in no PDB / label file). Landed name noted, not changed: COpinionReactionManager::GetThingAttitudeToPlayer
0x00701840 is landed as non-const `long (const CThing*)`; the PDB form (const, `const CThing&`, returns EOpinionAttitudeType) is what this
row binds (same as the header's FABLE_RETAIL_CTCWIFE_MARRY declaration) -- see NAME_FIXES.tsv.

Probes that did NOT move the registers (do not repeat): multiplier declared at top / `= 1.0f` single return; nested vs early-return
forms of the house test and of the GetSubDef test; `thing` pointer local; shopDef / thingDef raw-pointer locals; map reference local;
keeper CIntelligentPointer reference / pointer local; inline helper for the keeper test; manager reference / pointer alone; world
reference alone; CDefPointer release spellings (default / COUNTREF / POINTEE); map const find body visibility (CONSTFIND*,
FIND_TEMPLATE); default (non-expanded) GetTC forms; nested keeper / hero conditions. GetDef / GetSubDef bodies must stay visible
(decl-only GetDef: 450 bytes, thing def re-read through ecx). Diagnostic that pointed the way: an artificial late use of ParentThing
on the found path (probe/pl_b.cpp) gave retail's register roles, i.e. retail has one more callee-saved value live there.

Twin: 0x00773790 CTCShop::GetAttitudeSellPriceMultiplier (parked with the same residue) is byte-identical apart from +0x70 / +0x74
(AttitudeSellPriceMultiplierDefault / AttitudeSellPriceMultiplier): the same source with those two members should match.

## 0x00773790


## 0x00773790 CTCShop::GetAttitudeSellPriceMultiplier -- author pass 1 (wf7): DIFFER(430v448), 127 grades (about 25 of them probes)

Best source: differ/00773790.cpp (ported onto the shared headers, flat body, PDB callee spellings). Headers: overlay
hdr/engine/CTCHeroStats.h (FABLE_RETAIL_CTCHEROSTATS_TRADESKILL, declaration of GetTradeSkill). No flags.

Exact residue: every instruction shape is retail's (house test, GetDef / GetSubDef, both releases incl. the found path's missing
null test, 3 x GetPItem, find with hidden result, FPU order). Only the callee-saved register web differs:
  retail  this=ebp  ParentThing=edi (edi reused: &PShopKeeper)  thingDef=esi (with the map / reaction temps; reloaded from
          [esp+10h] after find and after GetTradeSkill)  zero=ebx, pushed lazily at 0x007737EA (ebx reused: shopDef)
          slots: thingDef [esp+10h] below shopDef/multiplier [esp+14h]; house key shares the shopDef slot
  ours    this=edi  ParentThing=ebp (ebp reused: thingDef)  zero=esi (esi reused: &PShopKeeper)  shopDef=ebx pushed after the
          GetSubDef test; slots swapped (house key shares the thingDef slot); so the two `return 1.0f` tails merge (-18 bytes).

What the grades proved (do not repeat):
- The 430-byte result is an attractor: 60+ source spellings give the identical bytes. No effect: house test as PeekTC(id,&p) /
  HasTC + PeekTC / `&&` / inline bool helper / non-expanded PeekTC<T> + GetTC<T> (Components.Find, ORIGINAL_CHAIN, FORCEINLINE_FIND);
  thing local or reference; GetParentThing().GetDefGlobalIndex(); thingDef-> / .Get()-> / a def local / comma expression with
  GetDef; declaration order and scopes of thingDef / shopDef / attitude / it / multiplier (nested blocks, else block, function
  scope); end() != it, end local, assigned iterator; reactions local; GetShopKeeper() (FABLE_RETAIL_CTCSHOP_SHOPKEEPER); hero as
  const CThing + PeekTC; hero `&&`; keeper nested ifs; `bool found`; `== false` + else; whole body in a forced-inline helper (plain,
  + bool sell, + member pointers); the Buy twin before / after in the TU; explicit template instantiation; tree find body visible
  (FABLE_RETAIL_STLP_TREE_CONSTFIND[_TEMPLATE_BODY], FIND_TEMPLATE, + noinline); CDefPointer release default form.
- Worse: `float multiplier = 1.0f; if (GetSubDef) {...} return multiplier` (411); if (found) {...return m;} return 1.0f (438);
  hero early returns (491 / 489, dtors duplicated); thingDef declared at function top (432: store and push ebx at entry);
  CDefPointer release COUNTREF (462) / POINTEE (476); without the GetDef body (450: thing def no longer enregistered, wrong release
  shape) / without the GetSubDef body (487); LowerBound const body hidden (445: key reloaded, retail folds `cmp [eax],0D5h`) or both
  hidden (448 bytes but 32 hunks); flags /Oa /Ow /G5 /G6 /G7 /GB /Ox /Op /Ob1 /GS /EHsc /Oy- /Os and optimize("s"): none.
- Probes (not legitimate sources) that located the mechanism:
  * `this` falls to ebp (retail) as soon as ONE of its five uses goes (PeekWorld or GetDefinitionManager off `this`): ParentThing
    then takes edi as in retail -- but thingDef follows into edi and zero stays esi. `this` and the ParentThing temp are a near tie.
  * Keeping ParentThing live across the GetSubDef call (a store of it after the call) gives retail's GROUPING exactly
    ({ParentThing, &PShopKeeper}, {house map, thingDef}, {zero, shopDef}, this alone; lazy `push ebx` at the defs block; separate
    1.0f tails), with the registers rotated (esi/edi/ebp) because the extra use raises ParentThing's priority.
  * Without the keeper and hero blocks thingDef sits in esi and no zero register exists; the hero map temp (esi) is what pushes
    thingDef off esi in our build, and the zero register then appears in the freed esi. In retail thingDef STAYS in esi and is
    evicted / reloaded around the temps, which our compiler only does when no other register is free for it.
  Reading: in retail's IL the ParentThing temp (edi) is still live when thingDef.Object is first loaded (so the thing def cannot
  take edi), although no retail instruction uses edi between 0x007737E3 and 0x00773858.

Best next idea: find the source fact that keeps the ParentThing value live past the GetDef call without emitting a use -- e.g.
the def lookup or the house test written through an inline accessor whose reference / `this` argument is bound once for a longer
scope (an inline CTCBase / CThing helper taking the thing, in the FableWin.pdb inline list), or a real retail spelling of
CDefPointer::operator-> / GetDef that reads thingDef.Object inside the GetDef statement. The Buy twin 0x007735D0 has the same
residue and will match with the same fix (swap Sell -> Buy members). Second idea: the same test on IsOpeningHours 0x00773950
(next function, same lookups, still local-model) to see which spelling there keeps ParentThing in edi beside `this` in ebp.

Names: 0x01382998 is spelled g_TradeSkillAttitudeScale (descriptive, made up: no PDB / string name found; 0.84f per the old draft).
Name fix for the coordinator: landed caller rebuild/src/compiled/00/79/CTCShopKeeper_GetFriendlynessIndicator_00799350.cpp declares
0x00773790 as `CTCShop::GetBasePriceMultiplier()` (line 18); Ego_r.pdb 0x00780980 + FableWin.pdb say
?GetAttitudeSellPriceMultiplier@CTCShop@@QBEMXZ (and engine/CTCShop.h already declares it so).

## 0x00773790 CTCShop::GetAttitudeSellPriceMultiplier -- author pass 2 (wf7): RELOCATION_MATCH (30 grades + 11 probes; 157 grades in all)

Matched source: 00773790.cpp, test 00773790_test.cpp (vt.py PASS), strict-headers regrade RELOCATION_MATCH, payload.json / oracle.tsv
written by addmatch.py (module CTCShop, name GetAttitudeSellPriceMultiplier). No flags. Headers: the row's overlay
hdr/engine/CTCHeroStats.h (FABLE_RETAIL_CTCHEROSTATS_TRADESKILL: declaration of ?GetTradeSkill@CTCHeroStats@@QBEMXZ 0x0057C239) must land
with it. The differ/ copy was removed (the row is no longer DIFFER).

What made it match: in the keeper block the reaction manager and the keeper are two named locals, in this order, before the call:
    const COpinionReactionManager& reactions = PeekWorld().PeekOpinionReactionManager();
    const CThingCreatureBase* keeper = PShopKeeper.Get();
    EOpinionAttitudeType attitude = reactions.GetThingAttitudeToPlayer(*keeper);
Pass 1 had tried the reactions local alone (same 430 bytes); the one-expression form and every other spelling stay on the 430-byte
attractor. With both locals VC7.1 places the reaction manager pointer in esi ahead of &PShopKeeper, and the whole retail web follows:
&PShopKeeper edi (after ParentThing), this ebp, thing def esi (reloaded from its slot after find and after GetTradeSkill), zero ebx
with the lazy `push ebx` at 0x007737EA, the two slots in retail's order, the two `return 1.0f` tails separate (448 bytes).

How it was found (probes, none of them a source): the residue was never ParentThing's lifetime. The thing def load is placed last and
takes whatever callee-saved register is left; extra uses of it change nothing (q1-q3). What decides the web is the order of the
reaction manager temporary against &PShopKeeper: probe s5 (the reaction manager given a second use) produced retail's exact
allocation plus the probe's 6-byte store, and the two-local spelling gives the same order with no extra use.
Other things settled on the way: the tree find body visible (FABLE_RETAIL_STLP_TREE_CONSTFIND reaches map::find: the object then holds
_M_find / _Rb_tree::find / map::find COMDATs) leaves the bytes unchanged; real noinline bodies of GetPControlledCreature / GetMainPlayer
in the TU, the QFE-4035 compiler, and flags /Z7 /Zi /GT /GA /Oi /Gr /GF /vmg /J change nothing; ParentThing and &PShopKeeper held in
one variable change nothing (the compiler allocates per live range, not per variable); 0x0057C239 GetTradeSkill is unaligned because
CTCHeroStats.cpp is a size-optimised TU, not because it is an inline (the dossier's guess), so its body is not visible here.

The Buy twin 0x007735D0 GetAttitudeBuyPriceMultiplier should match with the same source and the Buy members
(AttitudeBuyPriceMultiplierDefault +0x68, AttitudeBuyPriceMultiplier +0x6C) -- not graded here (one row per agent).
Names: g_TradeSkillAttitudeScale (0x01382998) is descriptive, made up (no PDB / string name found). Name fix stands (NAME_FIXES.tsv):
the landed caller CTCShopKeeper_GetFriendlynessIndicator_00799350.cpp spells 0x00773790 as CTCShop::GetBasePriceMultiplier.

## 0x00773790 repair pass (wf7): RELOCATION_MATCH kept
Fixed: hdr/engine/CTCHeroStats.h rebuilt from the current repo header + only the FABLE_RETAIL_CTCHEROSTATS_TRADESKILL block (the
PROPERTYOWNED branch, repo lines 288-296, is back; `diff` vs repo = 5 added lines). Regrade RELOCATION_MATCH (also strict headers),
vt.py PASS, payload rewritten by addmatch.py. Source and test unchanged.

## 0x007FD650


## 0x007FD650 CTCDPhysicalObstruction::FrameUpdate -- RELOCATION_MATCH (wf7 author, 5 grade calls / 10 variants)
- What made it match: (1) the port onto the shared headers with the scout's identity fixes -- 3-argument
  CIEngine::SetPersistentPrimitive (third arg 0), GetThingManager() returning a reference + CThingManager::IsDrawCombatCollisionDebug(),
  CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), the CEnginePrimitiveLine ctor body visible but out of line
  (FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE): that alone fixed the old "frame 0x60 v 0xD8" residue (449v448, frame
  right; with the ctor only declared it is 437v448). (2) The last byte + the this-in-esi/ebp register residue was the lift spelling:
  retail is `StartPos + C3DVector(0.0f, 0.0f, 2.0f)` through the inline C3DVector::operator+ (VC7.1 folds the `+ 0.0f` of X and Y),
  not `C3DVector(StartPos.X, StartPos.Y, StartPos.Z + 2.0f)`. operator+'s `this` = &StartPos / &EndPos is what puts the two addresses in esi / edi.
- Headers / flags: Flags none. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_FRAMEUPDATE
  (engine/CTCDPhysicalObstruction.h), FABLE_RETAIL_CTHINGMANAGER_ISDRAWCOMBATCOLLISIONDEBUG (engine/CThingManager.h).
- Tried and rejected: declared-only ctor (437v448); member-wise lift (449v448, this in esi); local `const C3DVector&` refs to
  StartPos/EndPos (448v448 but 28 hunks); a made-up inline Lift(const C3DVector&) helper for line 3 only (449) / for all three
  (matches, but made-up -- replaced by the real operator+ which gives the same bytes).
- No made-up names in the row. No landed name proved wrong. UpdateAdditionalImpulse 0x007FD580 is only declared (no body in the TU).
- Test: 007fd650_test.cpp (vt.py PASS); strict-header regrade RELOCATION_MATCH.

