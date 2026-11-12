## 0x00972CC0

# DOSSIER 0x00972CC0 -- CAIStateGroup_JackOfBladesStage1::MeleeCombat (612 bytes)

## 1. Retail shape (xdis 00972cc0 0x274)
- thiscall, plain `ret` (no args). this -> edi. Returns `mov eax,2` = AI_ACTION_FINISHED (engine/EAIAction.h: NULL0,TERMINATE_PROCESS1,FINISHED2,FAILED3,IN_PROGRESS4).
- `push ebp; mov ebp,esp; and esp,-8; sub esp,0x58; push ebx,ebp,esi,edi` -- the and-esp/-8 frame comes from an 8-aligned
  local (same as landed sibling 0x0093DDE0 AttackEnemy: "the 8-byte look-at temporary inside the loop gives the and esp,-8 frame").
  ebp is then reused as a general register. No SEH, no /GS.
- Locals (esp after pushes): +0x14 float distance, +0x18 temp CIntelligentPointer<const CThing> (8), +0x20 CBrainOutput (0x48:
  Types+0x20, LookFlags+0x24, Nav +0x28 (Proximity+0x34 imm 0, MovementType+0x38 esi, Speed+0x3c esi), PAction +0x40/+0x44, LookAtThing +0x60).
  Local+0x10..+0x14 is unused (frame 0x58 vs draft 0x54).
- Flow: prologue `if (creature->IsInMeleeAsAttacker()) GetWorld().PThingCombatManager->SetAttackerAsReadyToAttack(*creature,true)`
  (creature reloaded via [[edi+4]+0x18] twice). Loop top 0x972d00 (aligned by `lea esp,[esp]` filler):
  inline IsStillRunnable (PTargetThing alive (GetPItem!=0 && !(+0x91&1)), !IsStopping(), PArenaCentre alive) -- fail -> 0x972f10 Terminate(); return 2.
  Body: CBrainOutput ctor inline (LookAtThing ctor 0xA01B10 + vptr 0x01237F1C store, inline PAction release), Types|=0x20 (in memory `or`),
  SetAsLookAtThing inline (GetPItem(PTargetThing)->esi, Types|=4 via eax, temp ptr ctor+vptr(ebp)+SetPItem, GetPItem, LookAtThing.SetPItem, temp dtor),
  LookFlags=0x11, PBrain->SetOutput(output), PBrain->vslot2 (SuspendSuspendableProcess, result ignored);
  then distance: creature=PBrain->Creature, target=PTargetThing.GetPItem() (ebp), GetPos(creature)->esi, GetPos(target)->eax,
  dx=t.X-c.X, dy=t.Y-c.Y, fsqrt(dx*dx+dy*dy) (2D magnitude of target-creature), stored to [esp+0x14] BEFORE output dtor;
  output dtor inline: LookAtThing dtor call 0xA01C10, then PAction count release (dec/destroy/operator delete 0xBFE9BC; zero obj+count).
  Latch: inline IsStillRunnable again (fail -> return 2 WITHOUT Terminate, shared epilogue 0x972f17),
  `distance < GetCreature().GetCombatTypeDef().FrontMiddleLimit` (fcomp [eax+0x70]; test ah,5; jp exit) else return 2,
  HasHitsCausedRush() false -> jmp top; true -> return 2 via a SEPARATE epilogue 0x972f03 (pop order edi,esi,ebp,ebx vs 0x972f17 interleaves mov eax,2 after pop edi).
- The vptr constant 0x01237F1C is materialised in ebp once per iteration (`mov ebp,0x1237f1c` at 0x972d81) and stored twice (LookAtThing +0x60 and temp +0x18).

## 2. Identity (two sources) -- manifest label class is right, details wrong in draft
- Ego_r.pdb (egor_pdb_names.tsv 0x00997D30) and FableWin.pdb (_fablewin_api / naming_stragglers 0x02BEAC30, 0x017C8E95):
  `?MeleeCombat@CAIStateGroup_JackOfBladesStage1@@IAE?AW4EAIActionReturn@@XZ` = protected, non-const, non-virtual, returns EAIActionReturn, no params.
- Caller: only 0x00976769 inside CAIStateGroup_JackOfBladesStage1::Update 0x00975BB0 (vtable 0x0128D6B4 slot 12; parked draft
  rebuild/candidates/manual/pending/2026-11-11_codex_pass21/return/parked/00975bb0/00975bb0.cpp calls `MeleeCombat();`).
- Shared header rebuild/include/engine/CAIStateGroup_JackOfBladesStage1.h already declares `EAIActionReturn MeleeCombat();` (protected)
  under FABLE_RETAIL_JACK_STAGE1_UPDATE. Draft returns `long` -> WRONG.
- PDB order (Ego_r: HasHitsCausedRush 0x997290 < MeleeCombat 0x997d30) matches retail order (0x972340 < 0x972CC0): same TU.

## 3. Callees (landed file / landed name / PDB name / draft spelling)
| addr | landed row | PDB / landed name | draft | flag |
|---|---|---|---|---|
| 0x00662410 | 00/66/CThingCreatureBase_IsInMeleeAsAttacker_00662410.cpp | ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ (const) | CanBeReadyToAttack | WRONG |
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (returns CWorld&, const) | GetPWorld returning CWorld* | WRONG |
| 0x006EA450 | 00/6e/CCombatManager_SetAttackerAsReadyToAttack_006ea450.cpp | ?SetAttackerAsReadyToAttack@CCombatManager@@QAEXABVCThingCreatureBase@@_N@Z (const CThingCreatureBase&, bool), ret 8 | takes CThingCreatureBase* | WRONG type |
| 0x00A01B10/1B50/1B90/1C10 | 00/a0/CBaseIntelligentPointer_* | ctor / GetPItem (QBE..PAVCBaseObject) / SetPItem(CBaseObject*) / virtual dtor | ok names | via engine/CIntelligentPointer.h |
| 0x008FD010 | 00/8f/CAIStateGroupBase_IsStopping_008fd010.cpp | ?IsStopping@CAIStateGroupBase@@IBE_NXZ (protected const) | non-const | minor |
| 0x0088B8F0 | 00/88/CAIBrain_SetOutput_0088b8f0.cpp | ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z | SetBrainOutput | WRONG |
| vslot 2 (+8) of PBrain | 0x00A44840 | ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ (bool) | Yield (void) | WRONG |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | GetPosition | WRONG |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | ok | |
| 0x00666D10 | 00/66/CThingCreatureBase_GetCombatTypeDef_00666d10.cpp | GetCombatTypeDef() const -> const CCombatTypeDef& (CThingCreatureBase.h:98 under FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE) | returns pointer, non-const | WRONG |
| 0x00972340 | 00/97/CAIStateGroupJackOfBladesStage1_HasHitsCausedRush_00972340.cpp | ?HasHitsCausedRush@CAIStateGroup_JackOfBladesStage1@@IAE_NXZ (protected) | ok | not declared in the Stage1 shared header |
| 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ | ok | |
| (inlined) 0x00971B00 | 00/97/CAIStateGroupJackOfBladesBase_IsStillRunnable_00971b00.cpp | ?IsStillRunnable@CAIStateGroup_JackOfBladesBase@@IAE_NXZ (protected; landed row spells it QAE, wrong -- NAME_FIXES.tsv) | open-coded in draft | retail INLINES it twice |
| (inlined) 0x0093E760 | 00/93/CBrainOutput_SetAsLookAtThing_0093e760.cpp | CBrainOutput::SetAsLookAtThing(const CThing&,long) | (CThing*,long) | param type |

## 4. Members
- this +0x04 PBrain (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS); +0x10 Terminated (via Terminate).
- +0x1c PArenaCentre CIntelligentPointer<CThing>, +0x24 PTargetThing CIntelligentPointer<CThingCreatureBase>
  (CAIStateGroup_JackOfBladesBase.h, FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE). Draft models PTargetThing as <CThing> -- wrong template arg.
- CAIBrain +0x18 `Creature` (CThingAICreature*) -- engine/CAIBrain.h, GetCreature via FABLE_RETAIL_CAIBRAIN_GETCREATURE; SetOutput via
  FABLE_RETAIL_CAIBRAIN_SETOUTPUT; vslot 2 via FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE (base CSuspendableProcessMicrothread). Draft calls it PCreature.
  CAIStateGroupBase::GetCreature() inline (CAIBrain.h:121, FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE).
- CWorld +0x4c `PThingCombatManager` (scoped_ptr<CCombatManager>, struct_layouts_egor) -- draft "CombatManager". engine/CWorld.h + FABLE_RETAIL_CWORLD.
- CThing +0x91 bit0 = Dead (CThing.h:280 IsDead()); use IS_THING_ALIVE from engine/ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE).
- CCombatTypeDef +0x70 = `FrontMiddleLimit` (engine/CCombatTypeDef.h:45, FABLE_RETAIL_CCOMBATTYPEDEF) -- draft "Range" WRONG.
- CBrainOutput (Ego_r 0x48): Types, LookFlags, NavigationInput, PAction CCountedPointer<CCreatureActionBase>, MoveToPos, LookAtPos,
  LookAtThing CIntelligentPointer<const CThing>. Only declaration-level shared spelling is engine/campfire_types.h (private members,
  SetAsPumpCombatActions = Types|=0x20 per PDB method list); every landed user (0x0093DDE0, 0x0097D6F0) models CBrainOutput/CNavigationInput LOCALLY.
- Draft-local classes that HAVE shared headers: C3DVector (engine/C3DVector.h), CThing, CThingCreatureBase, CCombatTypeDef, CBaseIntelligentPointer /
  CIntelligentPointer (engine/CIntelligentPointer.h), CCountedPointer (engine/CCountedPointer.h, opt-ins FABLE_RETAIL_CCOUNTEDPOINTER,
  _RELEASEDATA, _VISIBLE_CALLED_DTOR ...), CCombatManager, CWorld, CAIBrain, CAIStateGroupBase, CAIStateGroup_JackOfBladesBase/Stage1.

## 5. Visibility
- IsStillRunnable (out-of-line copy 0x971B00) is expanded inline here twice -> same TU, visible. The parked Update draft already
  defines `inline bool CAIStateGroup_JackOfBladesBase::IsStillRunnable(){return IS_THING_ALIVE(PTargetThing) && !IsStopping() && IS_THING_ALIVE(PArenaCentre);}`
  ("native971b00, also expanded at the combat-loop latch"). Header declares it under FABLE_RETAIL_JACK_STAGE1_UPDATE (protected; both PDBs say protected IAE: Ego_r egor_pdb_names.tsv 0x00996980, FableWin _fablewin_api.tsv 0x0179970D / 0x02BE5100 `?IsStillRunnable@CAIStateGroup_JackOfBladesBase@@IAE_NXZ`; the landed row's `QAE` Leaf is wrong -- corrected in repair 3).
- HasHitsCausedRush 0x972340 is same TU, defined earlier, but called OUT of line (142 bytes; not inlined).
- CBrainOutput ctor / SetAsLookAtThing / dtor all inline here (unlike 0x0097D6F0 which calls 0x835620/0x93E760 out of line).
- CIntelligentPointer ctor/dtor/GetPItem/SetPItem out of line (0xA01Bxx) -- default FABLE_RETAIL_CINTELLIGENTPOINTER behaviour.
- 2D distance: C3DVector.h offers GetMagnitude2D under FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D (`GFFastSqrt(X*X+Y*Y)`, needs `#pragma intrinsic(sqrt)`)
  or FABLE_RETAIL_UPDATE_NEAREST_INTERESTING (running-statement form); operator- has FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP. Likely source:
  `(PTargetThing->GetPos() - GetCreature().GetPos()).GetMagnitude2D()`; the unused 8-byte-aligned frame slot (local+0x10, and esp,-8) may be the dead C3DVector/temp.

## 6. Siblings
- 0x0093DDE0 CAIStateGroup_MeleeCombatNearby2::AttackEnemy (00/93/...AttackEnemy_0093dde0.cpp) -- landed, near-identical shape (same and esp,-8 frame,
  IsInMeleeAsAttacker + SetAttackerAsReadyToAttack prologue, output Types|=0x20, SetAsLookAtThing(...,0x11), SetOutput, SuspendSuspendableProcess);
  local models of CBaseIntelligentPointer/CCountedPointer (FABLE_FORCEINLINE dtor + Reset)/CNavigationInput/CBrainOutput; no pragmas. COPY ITS CBrainOutput MODEL.
- 0x00971ED0 Stage1::GetRushToPosForLunge (Flags: none): shared-header idiom -- FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _GOTOBED_WORLD,
  _CAISTATEGROUP_JACKOFBLADESBASE, CINTELLIGENTPOINTER, _GET_ORIGINAL_CHAIN, _COMPARE_NULL_INT, CTHING, CTHINGPHYSICAL, CTHINGGAMEOBJECT,
  CTHINGCREATUREBASE, CWORLD, IS_THING_ALIVE; uses GetWorld(), IS_THING_ALIVE(PTargetThing), PTargetThing->GetPos().
- 0x00972340 Stage1::HasHitsCausedRush (CVectorMap.h, `#pragma auto_inline(off/on)`), 0x00972280 Stage1::IsRunnable, 0x00971B00 Base::IsStillRunnable (local models).
- Parked 0x00975BB0 Update: full FABLE_RETAIL_JACK_STAGE1_UPDATE define list incl. CAIBRAIN_GETCREATURE, CTHINGCREATUREBASE_COMBATTYPE, CCOMBATTYPEDEF,
  C3DVECTOR_MAGNITUDE_SQRTF, C3DVECTOR_NORMALISE_SQUARE_HELPER, `#pragma intrinsic(sqrt)`.

## 7. Draft model errors
- Names: CanBeReadyToAttack->IsInMeleeAsAttacker, GetPWorld->GetWorld (CWorld&), SetBrainOutput->SetOutput, Yield->SuspendSuspendableProcess (bool),
  GetPosition->GetPos, Range->FrontMiddleLimit, CombatManager->PThingCombatManager, PCreature->Creature; return type long -> EAIActionReturn (AI_ACTION_FINISHED).
- SetAttackerAsReadyToAttack takes const CThingCreatureBase& (draft pointer: same bytes but wrong spelling); GetCombatTypeDef returns const&.
- IsStillRunnable should be called (inline def), not open-coded IS_THING_ALIVE chains; PTargetThing is CIntelligentPointer<CThingCreatureBase>.
- Retail residue (notes, 3 tries): vptr 0x01237F1C held in ebp, frame 0x58 (+4 bytes: an extra dead temp, likely a C3DVector difference / 8-aligned temp),
  Types |= 0x20 as memory `or`, this/zero register swap. Draft's `__declspec(align(8))` on CBrainOutput is a hack: Ego_r has no such alignment;
  sibling 0x93DDE0 gets and esp,-8 without it (from its 8-byte look-at temporary).
- The separate return-2 epilogue for the HasHitsCausedRush-true exit vs the shared one for the other exits suggests the rush exit is a distinct `return` statement.

