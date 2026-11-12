## 0x00867B40

# DOSSIER 0x00867B40 (465 bytes) -- CCombatAction_GenericStrikeResponseKnockdown::FinishAction

## 1. Retail shape (xdis 00867b40 0x1e1)
- thiscall, plain `ret` (no args), NO frame pointer (`sub esp,0x128; push ebp; push esi; push edi`), no SEH prologue (temporaries' dtors are not EH-protected -> no /EHsc unwinding, or none needed). Frame 0x128.
- esi=this, ebp=&PCreature (this+8, kept for every GetPItem), edi=map ptr then animation index, bl=flip (pushed AFTER the TC lookup, popped before fnstsw).
- Frame slots (pre-`push ebx` offsets): [esp+0xC]=flip copy (byte), [esp+0x10]=key 0x31 AND later modes ptr (shared slot: `*ptc` written into the key's slot), [esp+0x14]=animation index (spilled right after the virtual call), [esp+0x14..0x1B] reused for the CCountedPointer<const C3DAnimationStats> result (Data,Info) after the index is loaded into edi; [esp+0x1C..] = one 0x118-byte temporary action slot shared by the three tails.
- Flow: base FinishAction; idx = this->GetAnimationIndex() (virtual slot 7, `call [eax+0x1c]`); creature=GetPItem once; TC 0x31 lookup expanded inline (bit test `[eax+0x24] & 0x20000`, LowerBound 0x40F020 out of line, `mov edi,[edi+4]` end via the map-pointer register, `cmp [eax],0x31; jle` = conditional-expression form); flip=FlipFacing(+0x13a); if(flip){ if(idx){ stats=PeekWorld().PMainMeshBank->PeekAnimation(idx)->GetAnimationStats(); if(stats->HasRotation()) flip=false; /*~CCountedPointer inline*/ } if(flip && modes->IsModeActive(0x1D)) flip=false; } modes->RemoveMode(0x1D);
  if(creature->Health(+0xB4) < DELTA_NOTIONAL_ZERO){ if(creature->PBaseDef(+0x70)->LeaveDeadCreature(+0x148 bit0)) creature->SetCurrentAction(CCreatureAction_Die(*creature, flip)); else creature->Kill(true); }
  else if(IsKnockdownToStun(*creature)) creature->SetCurrentAction(CCreatureAction_BeStunned(*creature, flip)); else creature->SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdownRecover(*creature, flip));
- Each SetCurrentAction: target GetPItem first (into esi), then args right-to-left (flip pushed, GetPItem, push). BeStunned tail `jmp 0x867c78` merges into the Die tail (SetCurrentAction + ~CCreatureActionBase 0x693EF0); Recover tail calls ~CCombatActionBase 0x62E900 directly (Recover/RecoverBase dtors implicit). 3 separate epilogues.
- FP: `fld [eax+0xb4]; fcomp [0x129BA3C]; fnstsw; test ah,5; jp else` = `Health < DELTA_NOTIONAL_ZERO`.
- Kill path: `push 1` BEFORE the GetPItem call.

## 2. Identity
- Name: CCombatAction_GenericStrikeResponseKnockdown::FinishAction(), Leaf `?FinishAction@CCombatAction_GenericStrikeResponseKnockdown@@UAEXXZ` (public virtual, void, non-const).
  Source 1: Ego_r.pdb (egor_pdb_names.tsv 0x0083DA70). Source 2: rtti-vtables.tsv: vtable 0x01277344 (RTTI CCombatAction_GenericStrikeResponseKnockdown) slot 4 = 0x00867B40, and slot 4 of CCreatureActionBase's vtable 0x01259D24 is 0x00694430 = CCreatureActionBase::FinishAction (landed 0x859A20 comment). Only xref: that vtable entry (.rdata 0x01277354).
- Manifest label (doubled class) is right in substance; spell it singly.
- Twin: 0x00867D20 CCombatAction_GenericResponseToExplosion::FinishAction (vtable 0x0125AEDC slot 4) has the same residue (hand261 NOTES).

## 3. Callees (landed file / landed name / PDB name) -- draft disagreements flagged (!!)
| addr | landed row | PDB / correct name | draft |
|---|---|---|---|
| 0x00859A20 | 00/85/CCombatActionBase_FinishAction_00859a20.cpp | ?FinishAction@CCombatActionBase@@MAEXXZ (protected virtual) | ok |
| vslot 7 [+0x1C] | (0x008565B0 in this vtable) | ?GetAnimationIndex@CCombatActionBase@@UBEJXZ / CCreatureActionBase UBEJXZ, `long ... const` | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | ok (via CIntelligentPointer.h FABLE_RETAIL_CINTELLIGENTPOINTER) |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound(const ETCInterfaceType&) non-const | !! draft local CInterfaceMap::LowerBound(const long*) |
| 0x00692950 | 00/69/global_Destval_00692950.cpp (MISLANDED name "Dest_val") | ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ (Ego_r 0x005192B0, right after GetWorld 0x005192A0; retail GetWorld 0x00692940 landed as CCreatureActionBase_GetWorld; bodies identical except jmp 0x4C7AA0 vs 0x4C7AB0). Other rows call it GetWorld (0x8CB940) / GetPCreatureAnimationOwner (0x8597A0, 0x856DD0) -> NAME_FIXES candidate | !! draft "GetPEngine" returning CEngineView* |
| (jmp target) 0x004C7AB0 | 00/4c/CViewBase_GetDisplayEngine_004c7ab0.cpp (wrong) | ?PeekWorld@CThing@@IBEABVCWorld@@XZ (Ego_r 0x004C9650 after GetWorld 0x004C9640; retail 0x4C7AA0/0x4C7AB0 pair) -> NAME_FIXES candidate | n/a |
| [world+0x3C] | -- | CWorld::PMainMeshBank (CCountedPointer<CMeshDataBank>, Ego_r CWorld +60; Data word read) | !! draft CEngineView::AnimationBank |
| 0x00A242C0 | 00/a2/CMeshDataBank_PeekAnimation_00a242c0.cpp | ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z (const, unsigned long, returns const C3DAnimationInfo*) | !! draft CAnimationBank::GetAnimation(long) |
| 0x00A269C0 | 00/a2/C3DAnimationInfo_GetAnimationStats_00a269c0.cpp | ?GetAnimationStats@C3DAnimationInfo@@QBE?AV?$CCountedPointer@$$CBVC3DAnimationStats@@@@XZ (const) | ok name, !! draft non-const + local CCountedPointer |
| vslot 4 [+0x10] on stats | C3DAnimationStats.h | C3DAnimationStats::HasRotation() const (slots: 0 dtor, 1 Validate, 2 GetMovementOffsetAtTime, 3 GetMovementAtTime, 4 HasRotation; vtable 0x0129D514 slots 2-6 = _purecall) | !! draft invented "IsVetoingFlip" |
| 0x00BFE9BC | operator delete | ok (from inline ~CCountedPointer) |
| 0x0073A8A0 | 00/73/CTCCreatureModeManager_IsModeActive_0073a8a0.cpp | ?IsModeActive@CTCCreatureModeManager@@QBE_NK@Z (const) | !! draft HasMode(non-const) |
| 0x0073ABA0 | 00/73/CTCCreatureModeManager_RemoveMode_0073aba0.cpp | ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z | ok |
| 0x00A01B50 x7 | see above | | |
| 0x0129BA3C data | -- | ?DELTA_NOTIONAL_ZERO@@3MB `extern const float DELTA_NOTIONAL_ZERO;` (154 landed rows) | !! draft g_fEpsilon |
| 0x008ED650 | 00/8e/CCreatureActionDie_CCreatureActionDie_008ed650.cpp | ??0CCreatureAction_Die@@QAE@AAVCThingCreatureBase@@_N@Z (`ret 8`; vtable 0x01287FC4) | !! draft CCreatureAction_StabbedToDeath(ptr,bool) -- StabbedToDeath's ctor takes only the creature |
| 0x00865940 | 00/86/CCreatureActionBeStunned_CCreatureActionBeStunned_00865940.cpp | ??0CCreatureAction_BeStunned@@QAE@AAVCThingCreatureBase@@_N@Z | ok name, takes reference |
| 0x007F05C0 | 00/7f/CCombatActionGenericStrikeResponseKnockdownRecover_..._007f05c0.cpp | ??0CCombatAction_GenericStrikeResponseKnockdownRecover@@QAE@AAVCThingCreatureBase@@_N@Z (FableWin.pdb) | !! draft "CCombatAction_KnockdownGetUp" |
| 0x006644F0 | 00/66/CThingCreatureBase_SetCurrentAction_006644f0.cpp | ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z | ok |
| 0x00693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ??1CCreatureActionBase (virtual dtor called directly for the Die/BeStunned temporaries) | ok |
| 0x0062E900 | 00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp | ??1CCombatActionBase | ok |
| 0x004C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | ?Kill@CThing@@QAEX_N@Z (Ego_r 0x004CB630) | ok (member of CThing, not CThingCreatureBase) |
| 0x00865AC0 | 00/86/global_IsKnockdownToStun_00865ac0.cpp | ?IsKnockdownToStun@?A0x2d832f72@@YI_NABVCThingCreatureBase@@@Z -- __fastcall FILE-STATIC (anonymous namespace) free function of THIS TU | !! draft models it as member CThingCreatureBase::IsKnockdownToStun() |

## 4. Members read
- this+0x08 PCreature: CIntelligentPointer<CThingCreatureBase> (CCreatureActionBase.h; FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE / FABLE_RETAIL_CINTELLIGENTPOINTER).
- this+0x13A FlipFacing (bool): engine/CCombatAction_GenericStrikeResponseKnockdown.h under FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN (retail model: CCombatActionBase 0x114 + AnimName[4] +0x114, PThingThatHit[8], PointOfHit[12], GotPointOfHit, DirectionOfHit[12], GotDirectionOfHit +0x139, FlipFacing +0x13A; sizeof 0x13C; Ego_r +0x152). It derives the shared CCombatActionBase.h.
- creature+0x20 CThing::UsingComponents (bitset<274>), +0x44 CThing::Components (CVectorMap) -- engine/CThing.h, FABLE_RETAIL_CTHING_GETTC_EXPANDED gives exactly the retail inline GetTC body (key copy inside the test, map pointer local, conditional-expression end-or-greater). Interface 0x31 = TCI_ENTITY_MODE_MANAGER (ETCInterfaceType.h:54) = CTCCreatureModeManager::StatGetInterfaceType().
- creature+0x70 CThing::PBaseDef (CDefPointer<const CThingBaseDef>; Ego_r +0x80, retail -0x10) -> CThingCreatureDef::LeaveDeadCreature bit 0 of +0x148 (Ego_r +0x150 bool:1, first bit). Header engine/CThingCreatureDef.h (FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS); landed reader of the same bit: 00/66/CThingCreatureBase_OnDie_00663f50.cpp (local model `bool LeaveDeadCreature : 1; // +0x148 bit 0`).
- creature+0xB4 CThingPhysical::Health (float; Ego_r +0xCC; inline GetHealth() exists in FableWin, CThingPhysical.h has `float GetHealth() const { return Health; }` but WARNING: per 0x865AC0 notes CThingPhysical.h places Scale at +0xB4 in the default layout -- check before using it).
- world+0x3C CWorld::PMainMeshBank (CCountedPointer<CMeshDataBank>, Ego_r +60; CWorld.h has FABLE_RETAIL_CWORLD).
- Draft-local classes that have shared headers: CCombatAction_GenericStrikeResponseKnockdown, CCombatActionBase, CCreatureActionBase, CThingCreatureBase (CThing.h), CBaseIntelligentPointer (CIntelligentPointer.h), CCountedPointer (CCountedPointer.h), CVectorMap/CInterfaceMap (CVectorMap.h), CTCCreatureModeManager, CMeshDataBank (draft "CAnimationBank"), C3DAnimationInfo, C3DAnimationStats, CThingCreatureDef (draft "CCreatureDefView"), CCreatureAction_Die (CCreatureAction_Die.h), CWorld (draft "CEngineView"). No shared header for CCreatureAction_BeStunned or ...KnockdownRecover (only quarantine/RecoverBase.h).

## 5. Callee visibility
- IsKnockdownToStun 0x865AC0: same TU, file-static, defined EARLIER (lower address) and NOT inlined (called out of line, __fastcall). Model as `namespace { bool __fastcall IsKnockdownToStun(const CThingCreatureBase&); }` declared/defined before; a visible body may get inlined -- keep it declared only, or define with noinline.
- Same-TU neighbours (combat-response TU, addresses 0x865940..0x868430): BeStunned ctor 0x865940, RecoverBase ctor 0x867AF0, Knockdown ctor 0x867A70, FinishAction of StrikeResponseBase 0x8663D0, RecoverFromDeath FinishAction 0x866FB0. Ctors of Die (0x8ED650) and Recover (0x7F05C0) are elsewhere (out of line).
- GetTC: __forceinline expanded (CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED); CVectorMap::LowerBound out of line (0x40F020).
- ~CCountedPointer<const C3DAnimationStats>: expanded inline (dec/reload/cmp 0/DeleteFunc(Data)/delete Info), default branch of CCountedPointer.h (do NOT define _CALLED_DTOR / _VISIBLE_CALLED_DTOR). Retail reloads Info from the slot after `dec` (header comment line 25 matches).
- PeekWorld 0x692950 called out of line (it is an Ego_r out-of-line member) -- declare only.
- No STLport helpers.

## 6. Siblings (idioms)
- 00/86/CCombatActionGenericBreakOffDrain_Initialise_00867820.cpp: `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED`, CThing.h + CVectorMap.h + CIntelligentPointer.h; `CTCCreatureModeManager* pModes; PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes); pModes->RemoveMode(69);` -- the exact mode-manager idiom.
- 00/86/CCombatActionGenericResponseToDrain_Initialise_008676a0.cpp: FABLE_RETAIL_CCHARSTRING + FABLE_RETAIL_CTHING_GETTC_EXPANDED + CCountedPointer.h.
- 00/86/CCombatActionGenericStrikeResponseKnockdownRecoverFromDeath_FinishAction_00866fb0.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED (FinishAction sibling).
- 00/86/CCombatActionGenericStrikeResponseBase_FinishAction_008663d0.cpp: SetCurrentAction(CCreatureAction_BeStunned(*creature, flip)) temporary idiom, ctor takes reference.
- 00/66/CThingCreatureBase_OnDie_00663f50.cpp: LeaveDeadCreature bit model.
- None of them use #pragma optimize or // Flags: lines (default /O2 /Oy, no frame pointer, matches retail).

## 7. Draft model errors
- Wrong callee names: StabbedToDeath->CCreatureAction_Die, KnockdownGetUp->CCombatAction_GenericStrikeResponseKnockdownRecover, GetPEngine->PeekWorld (const, returns const CWorld&), AnimationBank->PMainMeshBank (counted pointer), GetAnimation->PeekAnimation (const), IsVetoingFlip->HasRotation, HasMode->IsModeActive (const), g_fEpsilon->DELTA_NOTIONAL_ZERO, CanBeStabbedToDeath->LeaveDeadCreature, IsKnockdownToStun is a file-static __fastcall free function (const CThingCreatureBase&).
- Constness: PeekWorld/PeekAnimation/GetAnimationStats are const -> the chain is on const objects; stats is CCountedPointer<const C3DAnimationStats>.
- GetInterface: draft uses `if (...) it = last;` re-assignment and a `long` key; retail is the header's conditional-expression form with an ETCInterfaceType key copy inside the test (header notes: the if form goes through eax). Use CThing::GetTC(TCI_ENTITY_MODE_MANAGER, &modes) under FABLE_RETAIL_CTHING_GETTC_EXPANDED -- likely the fix for the residue (modes must stay in the key-shared frame slot; index goes to edi).
- Ctors take CThingCreatureBase& (pass *PCreature / *GetPItem()), not pointers (bytes same, names/mangling differ).
- Kill(bool) is CThing's member.
- Draft is otherwise right on flow, tails and temporaries.

## 0x00762BA0

# DOSSIER 0x00762BA0 CTCShapeManager::OnSerialise (468 bytes)

## 1. Retail shape (xdis 00762ba0 0x1e4)
- thiscall, `ret 8` (2 args: context [ebp+8], bool [ebp+0xc] unused). Frame pointer + `and esp,-8 ; sub esp,2Ch` (aligned frame; the
  old draft already reproduced this frame per NOTES), push ebx/esi/edi; esi=this, ebx=&context. No SEH prologue.
- Stack: [esp+12] bool isRelative (=1), [esp+13] bool default temp (=0), [esp+14] long numShapes, [esp+18] long default temp 0 (later
  reused as the save-loop index), [esp+1c..20] C2DCoordI offset (zeroed with integer edi: `xor edi,edi; mov [1c],edi; mov [20],edi`),
  [esp+2c..34] C3DVector pos copy (3 dwords), [esp+10] scratch vtable of second map.
- IsSaving (Mode +0x18 ==1||==3) is re-read 3x inline. First IsSaving branch has two arms with IDENTICAL pushes (&default,&value) then
  common `push "IsCoordsRelativeToMap"(0x126A340) ; call Transfer<bool> 0x4045C0`; default byte stored after the pushes (= GetDefaultVal<bool>()).
- if(isRelative): pos = ParentThing->GetPos() copied (call 0x4C73D0); ebx=PeekWorldMap() (0x686D20), edi=PeekWorldMap() (2nd call);
  ebx->vslot16(+0x40)(&pos) -> eax pushed; edi->vslot23(+0x5c)(hidden &tmp, region); 8-byte result copied to offset.
- if saving: numShapes = (last-first)>>3. Transfer<long>("NumShapes" 0x126A334, numShapes, default 0) 0x410BE0.
- Third IsSaving: SAVE path at 0x762CEE is placed AFTER the load path (load path falls through).
  LOAD: bounds stores in order +0x28,+0x2c = 0xD01502F9 (-1e10f, eax shared), then `lea ebx,[esi+20h]` +0x20,+0x24 = 0x501502F9 (1e10f);
  `if (numShapes==0)` -> exit D; loop i=0..num (`do{..}while(++i!=num)` shape, num reloaded from stack each iter): edx=context, push &bounds(ebx),
  &offset, i; ecx=this; call 0x762620 (static __fastcall). Exit A: SelectedShape(+0x18)=last(+0x10); ret.
  SAVE: ebx=it=first; index([esp+18])=0; if(first==last) exit C; loop: copy CCountedPointer {Data=[it],Info=[it+4]} (inc [Info] if Info),
  Data->vslot2(+8)(context, index, &offset) pushed &offset,index,context; `++index` AFTER the call; inline dtor (dec [Info]; if 0: call [Info+4]
  with ecx=[Info+8]; operator delete(Info) 0xBFE9BC); it+=8; cmp it, last(reloaded). Exit B: `mov ecx,eax` SelectedShape=last.
- FOUR separate exits (A load-loop end, B save-loop end, C save-empty, D load-zero), each `SelectedShape = PersistantShapes.end(); return` + full epilogue.

## 2. Identity (manifest label CORRECT in name/class; the old draft's signature is WRONG)
- Ego_r.pdb (egor_pdb_names.tsv 0x00773C30) `?OnSerialise@CTCShapeManager@@UAEXAAVCPersistContext@@_N@Z` = `void OnSerialise(CPersistContext&, bool)`, public virtual.
- Source 2: rtti-vtables.tsv CTCShapeManager vtable 0x0126A2A4 slot 1 = 0x00762BA0; CTCBase.h slot 1 = protected `virtual void OnSerialise(CPersistContext&, bool)`
  (ret 8). Ego_r order InitialiseFrom 0x773B30 / OnSerialise 0x773C30 / EditUpdateDelete 0x773E00 = retail 0x762AB0 / 0x762BA0 / 0x762D80.
- Draft used `(CPersistContext* context, long version)` -> must be `(CPersistContext& context, bool)` (reference + bool).

## 3. Callees
| retail | landed file | landed name | PDB | draft spelling |
|---|---|---|---|---|
| 0x004045C0 | 00/40/CPersistContext_Transferbool_004045c0.cpp | CPersistContext::Transfer<bool>(const char*,bool&,const bool&) | ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z | ok (but call via 2-arg Transfer(name,value) = GetDefaultVal) |
| 0x00410BE0 | 00/41/CPersistContext_Transferlong_00410be0.cpp | Transfer<long> | ??$Transfer@J@... | ok |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const | ?GetPos@CThing@@QBEABVC3DVector@@XZ (CThing.h:411) | DRAFT WRONG: "GetPosition" |
| 0x00686D20 | 00/68/CTCBase_PeekWorldMap_00686d20.cpp | CTCBase::PeekWorldMap() const -> const CWorldMap& | ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ | DRAFT WRONG: "GetWorldMap" returning CWorldMap* (GetWorldMap is 0x00686D30) |
| CWorldMap slot 16 +0x40 | CWorldMap.h:54 | `long GetMapNumberAt(const C3DVector&) const` | header comment: vtable 0x01244AEC slot16 0x004FC0C0 | DRAFT WRONG: "GetRegionAtPos" |
| CWorldMap slot 23 +0x5c | CWorldMap.h:60 | `C2DCoordI GetMapPos(long) const` (by value, hidden ptr) | (count of CWorldMap.h virtuals: dtor0..PeekMapAt15, GetMapNumberAt 16/17, GetMap18, PeekMap19, GetAll..20/21, PostRegionLoad22, GetMapPos23) | DRAFT WRONG: "GetRegionOffset" returning C2DVector |
| NShape::CShape slot 2 +0x08 | NShapeCShape_OnSerialise_00761c00.cpp | `virtual void OnSerialise(CPersistContext&, long, const C2DCoordI&)` | ?OnSerialise@CShape@NShape@@UAEXAAVCPersistContext@@JABVC2DCoordI@@@Z | DRAFT WRONG: "Serialise(CPersistContext*,long,const C2DVector&)" |
| 0x00762620 | NOT landed | NShape::CShape::StatOnSerialise | Ego_r 0x007736A0 + FableWin `?StatOnSerialise@CShape@NShape@@SIXAAVCTCShapeManager@@AAVCPersistContext@@JABVC2DCoordI@@AAVC2DBoxF@@@Z` (public static __fastcall) | DRAFT WRONG: static of CTCShapeManager, pointer params, C2DVector offset |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | - | ok (via CCountedPointer dtor) |
- 0x762620 position: retail AddNewShape 0x762580 < 0x762620 < InitialiseFrom 0x762AB0 = Ego_r AddNewShape 0x773620 < StatOnSerialise 0x7736A0 < InitialiseFrom 0x773B30 (same TU, defined earlier).
- Strings 0x126A340 "IsCoordsRelativeToMap" and 0x126A334 "NumShapes" are the draft's reading (not re-verified here).

## 4. Members
- CTCShapeManager (engine/CTCShapeManager.h, sizeof 0x30): PersistantShapes vector<CCountedPointer<NShape::CShape>> +0x0c (_M_finish +0x10),
  SelectedShape iterator +0x18, ClosedShapeBounds C2DBoxF +0x20 (struct_layouts_egor: PersistantShapes, SelectedShape, Activated, ..., ClosedShapeBounds).
  Typed vector/iterator only under FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES (it also declares private DrawShapes; default is opaque bytes) - members are PRIVATE:
  OnSerialise is a member so fine. Also FABLE_RETAIL_CTCSHAPEMANAGER (+ _VSLOTS, _TRACK, _GETINSTANTIATIONFROMENUM, _GETPRINCIPALVECTOR).
  Header does NOT declare OnSerialise override -> needs a declaration (add opt-in, e.g. FABLE_RETAIL_CTCSHAPEMANAGER_ONSERIALISE; none exists).
- CTCBase (engine/CTCBase.h, define FABLE_RETAIL_CTCBASE): ParentThing +0x04 (public), protected PeekWorldMap() const declared (line 151), GetWorldMap 0x686D30 also.
- CPersistContext (engine/CPersistContext.h, FABLE_RETAIL_CPERSISTCONTEXT): Mode +0x18, inline IsSaving() {Mode==MODE_SAVE_TEXT||Mode==MODE_SAVE_BINARY};
  FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS gives `Transfer(name,value)` -> `Transfer(name,value,GetDefaultVal<T>())` (protected static fastcall GetDefaultVal; specialisations
  `template<> inline bool/long __fastcall CPersistContext::GetDefaultVal<..>()` are written in the row as in NShapeCShape_OnSerialise_00761c00.cpp lines ~98-100).
- C2DBoxF (engine/C2DBoxF.h): members TLX,TLY,BRX,BRY (NOT TopLeft/BottomRight); FABLE_RETAIL_C2DBOXF gives 4-float ctor; retail stores BRX,BRY first then TLX,TLY.
- C2DCoordI (engine/C2DCoordI.h): long X,Y; ctor (long,long) -> `C2DCoordI offset(0,0)` gives the integer zero stores.
- CCountedPointer (engine/CCountedPointer.h): copy ctor inline (++Info->RefCount), default dtor is inline (`if(Info){--RefCount; if 0 {DeleteFunc(Info->Data); delete Info;}} Data=0;Info=0;`).
  Retail dtor has no visible Data/Info zero stores (dead locals, fine). Opt-ins: FABLE_RETAIL_CCOUNTEDPOINTER, _RELEASEDATA, _DTOR_DECL_ONLY, _VISIBLE_CALLED_DTOR (do NOT want decl-only: retail inlines it).
- NShape::CShape (engine/NShape_CClosedShapeBase.h): slot 0 dtor, 1 GetType, 2 OnSerialise(CPersistContext&,long,const C2DCoordI&). No StatOnSerialise declaration in any header (grep) -> add one.
- CWorldMap (engine/CWorldMap.h, FABLE_RETAIL_CWORLDMAP): GetMapNumberAt / GetMapPos as above.
- Draft LOCAL models with shared headers: C2DVector, C3DVector, C2DBoxF, CPersistContext, CThing, CWorldMap, NShape::CShape, CCPPointerInfo/CCountedPointer, CTCShapeManager.

## 5. Callee visibility
- Transfer<bool>/<long>: template COMDATs, 16-aligned, called out of line (declaration only is enough, as the landed OnSerialise rows do).
- GetPos 0x4C73D0: inline accessor in Ego_r but called out of line in retail: keep it declaration-only (CThing.h:411 already).
- PeekWorldMap: CTCBase TU, called twice out of line -> declaration only.
- StatOnSerialise 0x762620: same TU defined earlier (visible to compiler) but called out of line; a declaration is fine unless register allocation demands visibility.
- CCountedPointer copy ctor / dtor: expanded inline (must be visible inline bodies).

## 6. Siblings
- 00/76/CTCShapeManager_DrawShapes_00761980.cpp: Flags none; defines FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CWORLD, _CTHING, _CTCSHAPEMANAGER,
  _CTCSHAPEMANAGER_DRAWSHAPES; includes fable_string.h, C3DVector.h, CThing.h, CTCBase.h, CTCShapeManager.h, NShape_CClosedShapeBase.h (the best idiom template).
- 00/76/CTCShapeManager_GetInstantiationFromEnum_007624e0.cpp: same base macros + stlport/fable_containers.h, CCountedPointer.h.
- 00/76/NShapeCShape_OnSerialise_00761c00.cpp: the callee; GetDefaultVal specialisations, auto_inline(off) noinline Transfer specialisation trick.
- 00/68/CTCCreatureOpinionOfHero_OnSerialise_00684610.cpp and 00/7a/CTCAIScratchpad_OnSerialise_007abbd0.cpp: CTC OnSerialise rows using
  FABLE_RETAIL_CPERSISTCONTEXT + FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS.
- Older siblings InitialiseFrom 0x762AB0 / EditUpdateDelete 0x762D80 / AddNewShape 0x762580 model everything locally (do not copy their style).

## 7. Old draft model errors
- Signature (CPersistContext*, long) -> (CPersistContext&, bool).
- Offset type C2DVector(float) -> C2DCoordI(long): explains "zero stores via integer edi" residue directly.
- Wrong callee names/types: GetPosition->GetPos, GetWorldMap->PeekWorldMap (const ref), GetRegionAtPos->GetMapNumberAt, GetRegionOffset->GetMapPos (C2DCoordI),
  CShape::Serialise->OnSerialise, StatOnSerialise belongs to NShape::CShape with references.
- Retail evaluates PeekWorldMap() twice into ebx (used for GetMapNumberAt) then edi (used for GetMapPos); the draft's named local for the
  first + second call inline gave the ebx/edi swap; try `PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos))` without the local and with const refs.
- Bounds stores: draft wrote TopLeft first; retail BRX/BRY (-1e10) first then TLX/TLY via ebx=&ClosedShapeBounds (which is also the StatOnSerialise arg).
- Exits: retail has 4 separate `SelectedShape = end(); return` epilogues; draft merged. Save index `++index` is a statement after the call.
- Use IsSaving() (inline) three times, Transfer(name,value) 2-arg named-defaults for both (default temps written after the pushes).

## 0x00A12AB0

# DOSSIER 0x00A12AB0 CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume (468 bytes)

## 1. Retail shape (xdis 00a12ab0 0x1e4)
- __thiscall, `ret 0Ch` (3 dword args: name ref, pos ref, float radius). No frame pointer (no push ebp), no SEH. Flags: none (as siblings).
- Prologue `sub esp,40h ; push esi` (push esi after first fld). Frame 0x40 = 1/det (4) + local point (12) + inverse CMatrix3x4 (48) -- EXACTLY
  the frame of landed sibling 0x00A11F20 IsSpherePartContainedInBoundingBox (frame 0x40, named local inverse, not a returned temporary).
- Post-push frame map: [esp+4]=1/det, [esp+8/0xC/0x10]=local.X/Y/Z, inverse at esp+0x14: E11 0x14, E12 0x18, E21 0x20, E22 0x24,
  E31 0x2C, E32 0x30, E43 0x40 stored; E13/E23/E33/E41/E42 stay on the x87 stack (same split as 0x00A11F20). Constant 0x0122DED8 = 1.0f (fdivr).
- Order: inverse (0xA12AB3..0xA12C39 incl. transform), `mov eax,[esp+4Ch]` (pos) and `mov esi,[ecx+4]` (Mesh.Object) interleaved at 0xA12B98/9F,
  THEN radius*OO_Scale (`fld [esp+50h] ; fmul [ecx+3Ch] ; fstp [esp+4Ch]` 0xA12C3D -- scaled radius homed in the DEAD pos argument slot, computed
  AFTER the transform, unlike 0x00A11F20 which scales first), then `je ; inc [esi+4]` (AddRef), push scaled radius, push &local (lea esp+0Ch->+8),
  push name, `mov ecx,esi ; call 0x00A6CA80`. Release: `test esi,esi ; fst [esp+4Ch] ; je ; dec [esi+4] ; jne ; mov eax,[esi] ; fstp st0 ;
  mov ecx,esi ; call [eax+4]` (slot 1 = CIVCountedPointeeBase::Delete), reload float; NO null store (dying local/temporary).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00b807f0) and FableWin.pdb (_fablewin_api.tsv 01845c29 / 031d2c70; FableWin.exe.signatures.tsv):
  `?PeekRoughProportionOfSphereContainedInVolume@CPhysicsMeshObject@@QBEMABVCCharString@@ABVC3DVector@@M@Z`
  = `float CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume(const CCharString&, const C3DVector&, float) const`.
- Manifest label name/class is RIGHT; the old draft's parameter type `long volume` is WRONG (it is `const CCharString&`).
- Caller evidence: 0x00785363 (only xref) pushes `0x013BAC98` = static CCharString BUILDING_BOUNDRY (labels: ??__EBUILDING_BOUNDRY@@YAXXZ
  0x0121E940; atexit dtor 0x012297F0) as arg 1, `lea ecx,[ebp-28h]` = a CPhysicsMeshObject returned by 0x006B0850 GetPhysicsMeshObject; result fcomp.
- Parameter names: suggest name / pos / radius (header style: pos, radius as 0x00A11F20). Not a virtual (no vtable slot).

## 3. Callees
- 0x00A6CA80: landed rebuild/src/compiled/00/a6/CPhysicsMesh_PeekRoughProportionOfSphereContained_00a6ca80.cpp; landed + PDB name
  `?PeekRoughProportionOfSphereContained@CPhysicsMesh@@QBEMABVCCharString@@ABVC3DVector@@M@Z` (thiscall ret 0Ch).
  DRAFT DISAGREES: it calls `CPhysicsMesh::PeekRoughProportionOfSphereContainedInVolume(long,...)` -- wrong name AND wrong param type.
- `call [eax+4]` on CPhysicsMesh: CIVCountedPointeeBase slot 1 `Delete() const` (engine/CIVCountedPointeeBase.h, 0x0042D930). Draft calls it `Release()` (wrong).
- Inline (no call): CMatrix3x4::SetInverse(const CMatrix3x4&) and CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) (FableWin.pdb names; no
  retail out-of-line copy), CIVCountedPointer copy ctor + dtor/Release, CIVCountedPointeeBase::AddRef/Release.
- 0x00A12AB0 calls nothing else.

## 4. Members
- this+0x04 Mesh `CIVCountedPointer<CPhysicsMesh const>`, +0x08 PositionMatrix `CMatrix3x4`, +0x3C OO_Scale float (struct_layouts_egor.tsv;
  +0x38 Scale unused). Shared header rebuild/include/engine/CPhysicsMeshObject.h: Mesh typed under FABLE_RETAIL_CPHYSICSMESHOBJECT (+ GetMesh()
  inline returning a counted copy), PositionMatrix under FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX. Both private: the function must be a member.
- Header does NOT declare PeekRoughProportionOfSphereContainedInVolume (nor IsInContainmentVolume 0x00A128E0): a new declaration-only opt-in is needed
  (e.g. FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENT). CCharString is already forward-declared there.
- rebuild/include/engine/CPhysicsMesh.h does NOT declare PeekRoughProportionOfSphereContained(const CCharString&,...) either: needs a new declaration-only
  opt-in (e.g. FABLE_RETAIL_CPHYSICSMESH_CONTAINMENT), or the row forward-declares CCharString and gets it via that opt-in.
- CPhysicsMesh refcount: CIVCountedPointeeBase::IVRefCount +0x04 (mutable), needs FABLE_RETAIL_DEF_CLASS_BASE for the real class (vtable, AddRef/Release/Delete).
- CMatrix3x4 E11..E43 in engine/CMatrix3x4.h; C3DVector in engine/C3DVector.h (siblings define FABLE_RETAIL_C3DVECTOR).
- Classes the draft models LOCALLY that have shared headers: C3DVector, CMatrix3x4, CPhysicsMesh, CIVCountedPointer, CPhysicsMeshObject (all must go).
  No shared CCharString header exists; a forward decl (header already has `class CCharString;`) suffices for a reference param.

## 5. Visibility / opt-ins
- FABLE_RETAIL_CMATRIX3X4 -> FABLE_FORCEINLINE SetInverse; FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS -> forceinline two-arg TransformPoint
  (comment says both 0x00A11D10 and 0x00A11F20 match with it; gives E3k,E2k,E1k / 1,3,2 term order -- the "per-product operand order" the old note
  could not reproduce).
- FABLE_RETAIL_CIVCOUNTEDPOINTER -> counted copy ctor, default ~CIVCountedPointer(){Release();} with Release = `object=Object; if(object){ if(object->Release()==0)
  object->Delete(); Object=0; }` (dead store drops for a local). Alternatives if scheduling differs: _DTOR_DIRECT, _RELEASE_INNER (CIVCountedPointer.h).
- GetMesh() inline accessor (FableWin.pdb ?GetMesh@CPhysicsMeshObject@@QBE?AV?$CIVCountedPointer@$$CBVCPhysicsMesh@@@@XZ) under FABLE_RETAIL_CPHYSICSMESHOBJECT
  -- plausible original spelling `GetMesh()->PeekRough...(...)` (temporary), versus a named local copy `CIVCountedPointer<const CPhysicsMesh> mesh(Mesh)`
  (landed 0x00A128A0 HasContainmentVolume uses the named-copy form, though with local stand-in classes).
- Callee 0x00A6CA80 is in another TU (0x00A6xxxx, CPhysicsMesh source) -> out-of-line call, correct as declared. No STL.

## 6. Siblings (same class, same file region 0x00A114B0..0x00A12CC0)
- 00/a1/CPhysicsMeshObject_IsSpherePartContainedInBoundingBox_00a11f20.cpp (closest twin, 428 bytes, frame 0x40): defines FABLE_RETAIL_DEF_CLASS_BASE,
  _CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS, _CPHYSICSMESHOBJECT, _CPHYSICSMESHOBJECT_MATRIX, _CIVCOUNTEDPOINTER, _CMATRIX3X4,
  _CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS, _CPHYSICSMESH_BOUNDINGBOXINTERSECTED; body:
  `float scaled_radius = radius * OO_Scale; CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix); C3DVector local; inverse.TransformPoint(pos, local); return Mesh->Has...(scaled_radius, local, local);`
- 00/a1/CPhysicsMeshObject_GetNavigationLevelAt_00a11d10.cpp (Flags: none; + FABLE_RETAIL_C3DVECTOR, _CPHYSICSMESHOBJECT_NAVIGATION, _CPHYSICSMESH_NAVIGATION).
- 00/a1/CPhysicsMeshObject_HasContainmentVolume_00a128a0.cpp (CCharString arg + counted local copy `CIVCountedPointer<CPhysicsMesh> mesh(Mesh); return mesh->HasContainmentVolume(name);`, local stand-ins).
- 00/a1/CPhysicsMeshObject_GetBoundingBoxXEdge_00a11c20.cpp, _IsPointContainedInBoundingBox_00a12890.cpp (Flags: none). No #pragma optimize in any sibling.
- Parked twin 0x00A128E0 IsInContainmentVolume (same pending dir, DIFFER 538v464) -- same fix applies.

## 7. Old draft model errors
- Returned-temporary `centre * PositionMatrix.GetInverse()` (frame 0x70 vs 0x40) -> use named local inverse + SetInverse + TransformPoint(pos, local).
- Hand-written operator* sum order -> use the header's SOURCE_DEST_SUMS TransformPoint.
- `long volume` -> `const CCharString& name`; callee name wrong (InVolume) -> CPhysicsMesh::PeekRoughProportionOfSphereContained.
- `radius *= OO_Scale` placed BEFORE the inverse; retail computes it after the transform (homed in pos's dead slot). Try: scale after TransformPoint
  (`radius *= OO_Scale;` or pass `radius * OO_Scale` directly), then counted copy, then call.
- Counted copy must come after the transform; Mesh pointer load is hoisted by the scheduler anyway.
- All local stand-in classes -> shared headers.

## 0x00726160

# DOSSIER 0x00726160 (469 bytes)

## 1. Retail shape (xdis 00726160 0x1e5)
- `__thiscall`, `ret 4`, NO frame pointer (sub esp,18h; push ebx/esi/edi), NO SEH, no /O1 idioms. Plain /O2 like siblings (no pragma).
- ebx = this; `[ebx]` = the single 4-byte functor member (CTCPhysicsBase*); edi = arg CThing*.
- Arg slot [esp+28h] is reused three times: key temp 0x4d for LowerBound, then `radii`, then `sign`.
- Flow: (a) owner=Player->ParentThing; if owner->HasTC(77) {tc=Find(77)->second; if(!tc->IsSolidToThing(other)) return;}
  (b) if other->HasTC(77) {tc=other Find(77); if(!tc->IsSolidToThing(Player->ParentThing)) return;}  (ecx=tc loaded BEFORE [ebx]->+4 is reloaded+pushed)
  (c) if other->PhysicsTC->IsPushableByHero() return;  (d) if !other->HasTC(173) return;  ([edi+34h]&0x2000)
  (e) pos=other->GetPos() (call 0x4C73D0); dX=pos.X-P->Position.X -> [esp+0c], dY -> [esp+10] (only 2 floats stored: C2DVector/2 floats, not C3DVector);
      radii = Player->Radius + other->PhysicsTC->Radius (Player first); if !(fabs(pos.Z-P.Z) < radii) return;
  (f) if !Player->CanCollideWithThings() return; if !other->PhysicsTC->CanCollideWithThings() return;
  (g) x87: fld dY; fmul dY; fld dX; fmul dX; faddp; fcomp radii  -> if !(len2 < radii) return  (compared to radii, NOT radii^2)
  (h) v = Player->GetFacingVector() (slot 70, hidden-return temp at [esp+18h]); push = -v (fld Z,fchs,fld Y,fchs,fld X,fchs; stored X,Y,Z to [esp+0c..14]);
      world (esi) = CMainGameComponent::Get()->World (+0x24) loaded BEFORE CWorld::GetFrame();
      sign: `and 80000001h/jns/dec/or/inc` (signed %2), then `mov [slot],-1.0; je; mov [slot],1.0` ;
      GFFloatRandom(30.0f, world->GetWorldSeed()) * sign * (1/360 @0x01238E00) * sign -> push.RotateXY(angle) ; Player->ApplyControlledImpulse(push) (slot 81).

## 2. NAME (manifest label is WRONG)
- Real: `CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature::operator()(CThing*) const`
  mangled `??RCApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature@@QBEXPAVCThing@@@Z`.
- Evidence 1: sole caller 0x00726677 is in 0x00726660 = second _For_each of CTCPhysicsControlled::UpdateAdditionalImpulse (0x00725C20 call),
  whose functor is 4 bytes (`push esi` = this). PDB layout (struct_layouts_egor.tsv): this class is 4 bytes `CTCPhysicsBase* Player` @0.
  The other candidates are 20/24 bytes (CApplyAdditionalPlayerImpulseIfTooClose 20B = 0x726610/0x726340; CApplyAdditionalImpulseIfInCombatAndTooClose 24B = 0x727500/0x727220).
- Evidence 2: FableWin.pdb ?? (fablewin_pdb_names.tsv / _fablewin_api.tsv): `??R...ToPlayerIfInsideUnpushableCreature@@QBEXPAVCThing@@@Z` (0x0252AC40/0x017DEB64),
  ctor `??0...@@QAE@AAVCTCPhysicsBase@@@Z`, `?GetPushRotationExtentDegrees@...@@ABEMXZ` (private const float -> inline returning 30.0f, the RandFloat range),
  Ego_r `??$_For_each@PAPAVCThingCreatureBase@@VCApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature@@...` 0x0073D380 (Ego_r has no out-of-line operator()).
  Semantics fit: player pushed back by its own facing when inside an unpushable (IsPushableByHero false) boss (TCI_BOSS).
- Signature: void, const member, param `CThing*` (non-const pointer).

## 3. Callees
| retail | landed row | landed/PDB name | draft spelling |
|---|---|---|---|
| 0x0040F020 | rebuild/src/compiled/00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (out of line) | CInterfaceMap::LowerBound -- WRONG (use engine/CVectorMap.h) |
| 0x00770DE0 | 00/77/CTCSolidToThing_IsSolidToThing_00770de0.cpp | ?IsSolidToThing@CTCSolidToThing@@QBE_NPBVCThing@@@Z | CTCCombatTargets::Contains -- WRONG |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ (out of line) | CThing::GetPosition -- WRONG |
| 0x0049D870 | 00/49/CWorld_GetFrame_0049d870.cpp | static __fastcall ?GetFrame@CWorld@@SIJXZ | CWorld::GetWorldFrame (member) -- WRONG |
| 0x0049D810 | 00/49/CWorld_GetWorldSeed_0049d810.cpp | ?GetWorldSeed@CWorld@@QBEPAKXZ (unsigned long*) | GetPRandomSeed -> CRandomSeed* -- WRONG |
| 0x004978A0 | 00/49/global_GFFloatRandom_004978a0.cpp | float __fastcall GFFloatRandom(float range, unsigned long* seed) | CRandomSeed::RandFloat -- WRONG |
| 0x00A14040 | 00/a1/C3DVector_RotateXY_00a14040.cpp | ?RotateXY@C3DVector@@QAEXM@Z (engine/C3DVector.h:336) | RotateInPlace -- WRONG |
| vslot 57 +0xE4 | 0x006B11D0 | CTCPhysicsBase::CanCollideWithThings() const | IsCollidable -- WRONG |
| vslot 70 +0x118 | 0x006B1410 | CTCPhysicsBase::GetFacingVector() const (no-arg overload, slot 70 per CTCPhysicsBase.h:120) | GetVelocity -- WRONG |
| vslot 81 +0x144 | 0x006B1300 | CTCPhysicsBase::ApplyControlledImpulse(const C3DVector&) | ApplyImpulse -- WRONG |
| vslot 83 +0x14C | 0x006AFE50 (landed 00/6a/CTCPhysicsBase_IsPushableByHero_006afe50.cpp) | IsPushableByHero() const | IsStatic -- WRONG |
(vslots read from vtable 0x0125D2C4 in Fable.exe + manifest functions.tsv.)
Globals: 0x013B86A0 = CMainGameComponent::PMainGameComponent via inline `CMainGameComponent::Get()`, +0x24 World. 0x01238E00 = float 1/360 (0.0027777778).
Interface ids: 0x4D = TCI_SOLID_TO_THING (77), 0xAD = TCI_BOSS (173) (engine/ETCInterfaceType.h:82,178).

## 4. Members
- functor: `CTCPhysicsBase* Player` @0 (PDB).
- CTCPhysicsBase (engine/CTCPhysicsBase.h, opt-in FABLE_RETAIL_CTCPHYSICSBASE, needs FABLE_RETAIL_CTCBASE): ParentThing +4 (CTCBase, PDB), Position C3DVector +0x0C, Radius float +0x34.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): UsingComponents bitset<274> +0x20 (asserted), Components CVectorMap +0x44, PhysicsTC +0x60. HasTC(type) / GetTC(type) (FABLE_FORCEINLINE, GetTC = `*Components.Find(type)->second`); also GetTC<T>/PeekTC<T>, FABLE_RETAIL_CTHING_HASTC_FORCEINLINE, FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- CVectorMap (engine/CVectorMap.h): LowerBound out of line; Find inline; FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND / _ORIGINAL_FIND (retail here expands Find: call LowerBound; cmp end; cmp [eax],4Dh; jle).
- CTCSolidToThing (engine/CTCSolidToThing.h, FABLE_RETAIL_CTCSOLIDTOTHING). CWorld.h: GetFrame/GetWorldSeed. CMainGameComponent.h (FABLE_RETAIL_CMAINGAMECOMPONENT): Get(), World.
- C2DVector.h (ctor (x,y), operator-), C3DVector.h (FABLE_RETAIL_C3DVECTOR; RotateXY).
- Draft LOCALLY models (all have shared headers): CThing, CTCBase, C3DVector, C2DVector, CWorld, CMainGameComponent, CVectorMap(CInterfaceMap), CPhysicsObject(=CTCPhysicsBase), CTCSolidToThing(as CTCCombatTargets).

## 5. Callee visibility
- Out of line in retail: LowerBound 0x40F020, GetPos 0x4C73D0 (7-byte inline-looking body, emitted out of line -> declare only), GetFrame, GetWorldSeed, GFFloatRandom, RotateXY, IsSolidToThing.
- Inlined: CThing::HasTC (bit test in place), CVectorMap::Find (via GetTC), CMainGameComponent::Get, GetPushRotationExtentDegrees (30.0f), functor members.
- Same TU (CTCPhysicsControlled.cpp): 0x00725B70 UpdateAdditionalImpulse, 0x00726340 IfTooClose::operator(), _For_each 0x726610/0x726660. No STLport map helpers involved.

## 6. Siblings (same TU, same idioms)
- rebuild/src/compiled/00/72/CApplyAdditionalPlayerImpulseIfTooClose_operator_00726340.cpp (RELOCATION_MATCH, no pragmas/flags, near twin):
  sign idiom `float sign = (CWorld::GetFrame() % 2 == 0) ? -1.0f : 1.0f; float angle = GFFloatRandom(45.0f, world->GetWorldSeed()) * sign; float turns = angle * (1.0f / 360.0f); offset.RotateXY(turns * sign);`
  with `CWorld* world = CMainGameComponent::Get()->World;` read BEFORE GetFrame; owner HasTC(0x4d) -> `((CTCSolidToThing*)owner->GetTC(0x4d))->IsSolidToThing(other)`;
  GetTC modelled as forceinline LowerBound + `(it == end || type < it->first) ? end : it)->second`; `#pragma intrinsic(fabs, sqrt)`. Its local vslot names are wrong too (GetCollidesWithThings/GetVelocity/AddAdditionalImpulse) -> NAME_FIXES.
- 00/72/CTCPhysicsControlled_UpdateAdditionalImpulse_00725b70.cpp: `// Flags: none`, macros FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CTHING, _CTCPHYSICSBASE, _CTCPHYSICSBASE_SEARCHTOOLS, _CCLASSFACTORY, _CTHINGMANAGER(_ALLPAUSE), _CINTELLIGENTPOINTER, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTHINGSEARCHTOOLS; declares the 4B functor as `CTCPhysicsControlled* Physics` (PDB: CTCPhysicsBase* Player).
- 00/72/std_foreach_00726660.cpp (caller), 00/72/global_foreach_00726610.cpp, 00/72/CTCPhysicsNavigator_UpdateAdditionalImpulse_00726be0.cpp.

## NAME_FIXES candidates
- std_foreach_00726660.cpp: functor class spelled CApplyAdditionalImpulseIfInCombatAndTooClose{CTCPhysicsControlled*} -> CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature{CTCPhysicsBase* Player}; leaf std::_For_each.
- CTCPhysicsControlled_UpdateAdditionalImpulse_00725b70.cpp: 20-byte functor spelled CApplyAdditionalImpulseIfInCombatAndTooClose -> CApplyAdditionalPlayerImpulseIfTooClose (PDB 20B; 0x726610 row agrees).
- 0x00726340 row vslot names (see above).

## 7. Old draft model errors
- Wrong class/member name, operator() not const, member type CPhysicsObject (should be CTCPhysicsBase* Player).
- All callee spellings wrong (table above); GetFrame is a static fastcall, RandFloat is the free fastcall GFFloatRandom(range, seed).
- Sign: draft `sign=-1; if(%2) sign=1;` -- sibling's ternary `(GetFrame()%2==0) ? -1.0f : 1.0f` matched in 0x726340; use named `angle`/`turns` locals like the sibling.
- Length^2: retail computes dY*dY first then dX*dX then faddp; try sibling's GetXYLengthSquared form or `dX*dX + dY*dY` (VC7.1 may evaluate right operand first) -- draft's `Y*Y+X*X` left slot residue.
- Delta: only X,Y stored -> C2DVector(pos.X-P.X, pos.Y-P.Y) (engine/C2DVector.h ctor) or two floats; z diff computed in place for fabs.
- Push vector: `C3DVector push = -Player->GetFacingVector();` (temp + negation) then RotateXY(...), ApplyControlledImpulse(push).
- Second SolidToThing check: `other->GetTC(TCI_SOLID_TO_THING)` result in ecx before `Player->ParentThing` is reloaded -- write as one expression `((CTCSolidToThing&)other->GetTC(..)).IsSolidToThing(Player->ParentThing)` (owner thing NOT cached in a local).

## 0x00A90900

# DOSSIER 0x00A90900 (471 bytes) -- N3DPrimitive::CDeviceIndependent::AddTriangleList(const C3DMeshFileTrianglesChunk&)

## 1. Retail shape (python3 $S/w17/xdis.py 00a90900 0x1e7)
- thiscall, `ret 4` (one stack arg: the chunk reference). `sub esp,20h; push ebx,ebp,esi,edi` -- NO frame pointer (/Oy on),
  NO SEH prologue even though two destructible locals live across calls => TU built WITHOUT /EHsc (neighbours 0x00A90700,
  0x00A90AE0, 0x00A91640 also have no SEH). Not size-optimised (speed idioms: counted-down copy loop, inline resize/push_back).
- Frame 0x20: [esp+10h] raw list ptr copy (reloaded into ebp), [esp+14h] `this` (stored at entry; also the ADDRESS passed as
  the C3DTriangle2() default arg of resize -- never-written POD temp packed onto it), [esp+1Ch] the CIVCountedPointer<CTriangleList>
  local (its address is push_back's x), [esp+24h] strip Info spill, [esp+28h/2Ch] FindFirstSubChunkOfType by-value temporary
  {Data,Info}, [esp+34h] the dead `chunk` param slot reused for the unsigned short() resize temp (`mov dword [esp+34h],0`) and
  then as the __false_type tag address of _M_insert_overflow.
- Flow: new(0x20) -> inline ctor (IVRefCount=1 at +4 BEFORE vtable 0x0129DF7C, then six zero words +8..+1C) / null branch;
  GetList (0xABC440) then GetCount (0xABA370) on the chunk; Triangles.resize(count, C3DTriangle2()) OUT OF LINE (0xA97A50, args n,&x);
  copy loop (6-byte records, [edi] reloaded each iteration, count-down `dec esi`); FindFirstSubChunkOfType("XTST" @0x0129DF60)
  const 0xAA7F40 into temp; ShareData-shaped copy into a NULL local (ebx=esi=0 first; only if temp.Info!=0: ebx=Data, ++RefCount,
  esi=Info) -- this is CCountedPointer::SetStaticCast/ShareData on an empty local, NOT the converting copy ctor; temp dtor
  (dec/ DeleteFunc/ delete) ; if (strip.Data) { n=(finish-start)>>1 (`sar`, signed size); StripIndices.resize(n) INLINE (erase path
  with unfolded `cmp ecx,ecx` __copy_trivial+memmove, else _M_fill_insert 0x5149F0 with &[esp+34h]); up-counting copy loop j<n
  re-reading strip->StripVertices start [ebx+10h] and list->StripIndices start [edi] each iteration (word moves) };
  TriangleLists.push_back(pointer) INLINE (finish +14h vs end +18h; placement-new null test; `inc [ebp+4]`; else
  _M_insert_overflow 0xA97BE0(pos,&x,&tag,1,true)); strip local release (dec [esi]; DeleteFunc(Info->Data); delete Info) ;
  list local release (`test ebp; je; dec [ebp+4]; jne; mov eax,[ebp]; mov ecx,ebp; call [eax+4]` = Release()==0 -> Delete() slot 1,
  no null store: dying local). Destruction order: strip first, then list => strip declared after list, both at function scope.

## 2. Name / signature (two sources)
- ghidra_out/egor_pdb_names.tsv + egor_signatures.tsv + _fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv:
  `?AddTriangleList@CDeviceIndependent@N3DPrimitive@@AAEXABVC3DMeshFileTrianglesChunk@@@Z`
  => PRIVATE, thiscall, `void AddTriangleList(const C3DMeshFileTrianglesChunk& chunk)`, class in namespace N3DPrimitive.
  Manifest label "CDeviceIndependent::AddTriangleList" is right except missing namespace. Leaf for header: `AddTriangleList`
  (full mangled above). The draft's `C3DMeshFileTrianglesChunk*` non-const pointer is WRONG (const reference).
- There is a PUBLIC overload `?AddTriangleList@CDeviceIndependent@N3DPrimitive@@QAEXABV?$CIVCountedPointer@VCTriangleList@N3DPrimitive@@@@@Z`
  (FableWin) -- almost certainly `{ TriangleLists.push_back(list); }`, which retail expanded here (no call). Spell the end as
  `AddTriangleList(triangle_list);` with that overload defined inline (in class / earlier in TU) -- or push_back directly.
- Callers: 0x00A919D6 and 0x00A9202A, both inside unlanded 0x00A91640 (gapscan; C3DPrimitive2 builder; `mov ecx,[ebx+2Ch]` =
  C3DPrimitive2's scoped_ptr<CDeviceIndependent>). Vtable: not virtual (rtti-vtables: CDeviceIndependent 0x0129DFA8 slot0 only).

## 3. Callees
| addr | landed file | landed name | PDB name / note |
|---|---|---|---|
| 0xBFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new | ok (new CTriangleList, 0x20) |
| 0xABC440 | 00/ab/C3DMeshFileTrianglesChunk_GetList_00abc440.cpp | C3DMeshFileTrianglesChunk::GetList() const | Ego_r ?GetList@C3DMeshFileTrianglesChunk@@QBEPBVC3DTriangle2@@XZ. Draft "GetTriangles" WRONG |
| 0xABA370 | 00/ab/CWorldMap_DrawGetNoMaps_00aba370.cpp | CWorldMap_DrawGetNoMaps (__fastcall free fn, LANDED NAME WRONG) | Ego_r ?GetCount@C3DMeshFileTrianglesChunk@@QBEKXZ (unsigned long GetCount() const; (end-begin)/6 at +2C/+30). Draft "GetNoTriangles" WRONG. -> NAME_FIXES.tsv candidate |
| 0xA97A50 | 00/a9/STLvectorC3DTriangle2_resize_00a97a50.cpp | std::vector<C3DTriangle2>::resize(n,const C3DTriangle2&) | ok; draft "CTriangleVector/CTriangle" WRONG types |
| 0xAA7F40 | 00/aa/CChunkedFileSuperChunk_FindFirstSubChunkOfType_00aa7f40.cpp | CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) const | returns `const CCountedPointer<CChunkedFileChunk>` (const QBE overload; 0xAA80E0 is the non-const twin) |
| 0xBFE9BC | operator delete | | count block delete |
| 0xBFEAE6 | memmove | | via STLport __copy_trivial in inline erase |
| 0x5149F0 | 00/51/stdvectorunsignedshort_Mfillinsert_005149f0.cpp | vector<unsigned short>::_M_fill_insert | ok |
| 0xA97BE0 | 00/a9/stdvector_Minsertoverflow_00a97be0.cpp | vector<CIVCountedPointer<N3DPrimitive::CTriangleList>>::_M_insert_overflow(pos,const T&,const __false_type&,unsigned,bool) | Draft used __TRUE_type: WRONG (leaf has __false_type) |
| [Info+4] | -- | CCPPointerInfo::DeleteFunc | CCountedPointer release |
| vtbl slot 1 | -- | CIVCountedPointeeBase::Delete() const | list release |

## 4. Members
- CDeviceIndependent (NO shared header; retail 0x50 bytes per landed ctor 0x00A98100): vptr(CBaseClassNonCopyable) +0,
  Vertices +4, **TriangleLists +0x10** (CArray<CIVCountedPointer<N3DPrimitive::CTriangleList>>; finish +14h, end +18h),
  Groups +1C, BoundingSphere +28, UTangents +38, VTangents +44 (struct_layouts_egor.tsv names; Ego_r offsets are 16-byte-CArray).
- N3DPrimitive::CTriangleList (NO shared header; retail 0x20, vtable 0x0129DF7C, scalar dtor 0xA97830): : CIVCountedPointeeBase
  (vptr, IVRefCount +4), **Triangles +8** CArray<C3DTriangle2>, **StripIndices +0x14** CArray<unsigned short> (Ego_r names).
  FableWin inline accessors: ResizeTriangleList(unsigned long), SetTriangle(unsigned long,const C3DTriangle2&),
  ResizeStrip(unsigned long), SetStripIndex(unsigned long,unsigned short), GetTriangleCount, GetStripSize, PeekStrip, PeekTriangleList.
  Ctor ??0CTriangleList@N3DPrimitive@@QAE@XZ inline here.
- C3DMeshFileTrianglesChunk: engine/C3DMeshFileTrianglesChunk.h, opt-in FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK (needs
  FABLE_RETAIL_CCHUNKEDFILECHUNK + FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK); TriangleList +0x2C. Header LACKS GetList/GetCount
  declarations -- add them as declaration-only in the row (or header opt-in) since retail CALLS them.
- C3DMeshFileXTriangleStripChunk: engine/C3DMeshFileXTriangleStripChunk.h, opt-in FABLE_RETAIL_C3DMESHFILEXTRIANGLESTRIPCHUNK;
  StripVertices +0x10 (CArray<unsigned short>). PDB inline accessors GetCount() const (QBEKXZ), GetStripVertex(unsigned long) const
  -> unsigned long, static __fastcall GetStaticChunkType() (SIPBDXZ) returning "XTST" (literal 0x0129DF60, same address the
  XLoader ctor 0xAA6178 pushes) -- header has none of these yet.
- CCountedPointer<T>: engine/CCountedPointer.h {Data +0, Info +4}, CCPPointerInfo {RefCount, DeleteFunc, Data}. Use
  SetStaticCast (expands ShareData) as landed C3DAnimFileMovementVectorChunk::GetMovementTrack 0x00AAE970.
- CIVCountedPointer<T> / CIVCountedPointeeBase: engine/CIVCountedPointer.h, engine/CIVCountedPointeeBase.h. Opt-ins:
  FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR (IVRefCount(1) before derived vtable -- matches 0xA9091B),
  FABLE_RETAIL_CIVCOUNTEDPOINTER (copy ctor AddRef), FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW (adopting T* ctor, no AddRef --
  retail stores the new list into [esp+1Ch] with no inc), _DTOR_DIRECT / _RELEASE_INNER (dying-local release shapes).
- C3DTriangle2: engine/C3DTriangle2.h (POD, no ctor -> C3DTriangle2() temp never written, as retail).
- CArray: engine/CArray.h (public STLport vector; FABLE_RETAIL_CARRAY for begin()-based operator[]).
- Draft models LOCALLY (all have shared headers): CCountedPointer/SCountBlock (-> CCountedPointer.h), CRefPtr/CRefCounted
  (-> CIVCountedPointer.h/CIVCountedPointeeBase.h), CRetailVector/CTriangleVector/CTriangleListArray (-> CArray.h/stlport),
  CTriangle (-> C3DTriangle2.h), C3DMeshFileStripChunk (-> C3DMeshFileXTriangleStripChunk.h), C3DMeshFileTrianglesChunk,
  CChunkedFileChunk, __copy_trivial (in stlport/fable_containers.h). Only CDeviceIndependent and CTriangleList lack headers.

## 5. Callee visibility
- Out of line in retail: GetList/GetCount (other TU, C3DMeshFileTrianglesChunk.cpp: declare only), vector<C3DTriangle2>::resize(n,x)
  (0xA97A50 COMDAT; resize(n) one-arg wrapper itself expanded), FindFirstSubChunkOfType, _M_fill_insert, _M_insert_overflow.
- Inline in retail: CTriangleList ctor, CIVCountedPointer ctor/dtor, CCountedPointer SetStaticCast/ShareData/dtor,
  vector<unsigned short>::resize + erase (__copy_trivial memmove), CArray<CIVCountedPointer>::push_back, strip accessors.
- CONFLICT to solve: stlport resize is one template; ushort resize inline but C3DTriangle2 resize(n,x) called. Per-TU macro
  FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (noinline for ALL T) would break the ushort expansion; likely VC7.1's own heuristic
  (C3DTriangle2 erase goes through non-trivial __copy 0xA744B0 since not POD-tagged) or CTriangleList::ResizeTriangleList being
  the inline wrapper. Try default inline first; if C3DTriangle2 resize expands, a C3DTriangle2-only explicit declaration
  (`template<> void std::vector<C3DTriangle2>::resize(size_type,const C3DTriangle2&);`) is the narrow fix.
- Other stlport opt-ins seen: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE/_OUTOFLINE (NOT wanted: push_back is inline here),
  FABLE_RETAIL_STLP_FALSE_TAG_POD (not wanted: tag never written), FABLE_RETAIL_CARRAY_PUSHBACK (Lionhead CArray::push_back wrapper).

## 6. Siblings (same TU neighbourhood / same classes)
- 00/a9/N3DPrimitiveCDeviceIndependent_CDeviceIndependent_00a98100.cpp (local CArray/CDeviceIndependent model, offsets above).
- 00/a9/N3DPrimitiveCDeviceIndependent_Sizeof_00a90450.cpp, 00/a9/C3DPrimitive2_Sizeof_00a90860.cpp (includes CCountedPointer.h),
  00/a9/C3DPrimitive2_C3DPrimitive2_00a90700.cpp -- all no /EHsc, no pragma.
- 00/aa/C3DAnimFileMovementVectorChunk_GetMovementTrack_00aae970.cpp: exact SetStaticCast(FindFirstSubChunkOfType(X::GetStaticChunkType())) idiom.
- 00/a3/C3DAnimationBlendState_CreateInterpolate_00a3bf10.cpp and 00/5d/CAnimationEntry_StatRegisterClass_005dd670.cpp: CIVCountedPointer
  opt-in stacks (DEF_CLASS_BASE, POINTEEBASE_CTOR, CIVCOUNTEDPOINTER, FROM_RAW, RELEASE_INNER, CONVERT/ASSIGN).
- 00/a9/stdvector_Minsertoverflow_00a97be0.cpp (the push_back overflow callee; __false_type tag).

## 7. Old draft model errors
- Signature: pointer, non-const, public, no namespace -> must be private `const C3DMeshFileTrianglesChunk&` in N3DPrimitive.
- Strip pointer built with a converting copy ctor -> retail is an empty local + SetStaticCast/ShareData (ebx/esi zeroed first);
  this is likely the ebx/ebp swap the history reports.
- List smart pointer: draft CRefPtr(raw) + raw pointer; retail = CIVCountedPointer adopting ctor (FROM_RAW) stored at [esp+1Ch]
  with compiler-cached raw copy at [esp+10h]; push_back argument is that local (frame 0x20 vs draft 0x18).
- Tag type __true_type for _M_insert_overflow -> __false_type (CIVCountedPointer non-POD).
- Accessor spellings: GetTriangles/GetNoTriangles -> GetList/GetCount; strip via GetCount()/GetStripVertex(i); list via
  ResizeTriangleList/SetTriangle/ResizeStrip/SetStripIndex (FableWin inline names).
- Loops: triangle loop `for(i=0;i<count;++i) list->SetTriangle(i, triangles[i])` (compiler counts down); strip loop
  `for(j=0;j<n;++j) list->SetStripIndex(j, (unsigned short)strip->GetStripVertex(j))`, n = strip->GetCount() once.
- Compile WITHOUT /EHsc.

## 0x008DC720

# DOSSIER 0x008DC720 -- NCreatureMode::CModeDragonStrafeBurninate::StartBeamEffect (478 bytes)

## 0. THE KEY FACT: a byte-twin is already landed
`rebuild/src/compiled/00/8d/CModeDragonFireBeam_StartBeamEffect_008db620.cpp` (retail 0x008DB620, 481 B) is the same
function for the sibling class: identical prologue zero-stores (incl. the duplicated `[orig+18h]=0` + `[orig+10h]=0`),
identical GetDummyObject call, counted-def temporary with ebx flag bits 1/2, `[esp+0xf]` found-bool, same Create / SetPItem /
GetPItem / LowerBound(0x4B) / AttachToThing sequence (compared with `xdis.py 008db620 0x1e1`). START FROM THAT SOURCE, not the
old draft. Its idioms: `CDefPointer<CSpecialEffectsDef> effects; long effectType = 0; C3DVector position; CRightHandedSet axes;`
-- name CCharString in a scope block -- `if (CDefClassReference(Owner->m_pDef)->GetSubDef(effects) && effects.m_p->GetEffectFromString("BEAM_ATTACK", effectType))`
-- `PBeamWeaponEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effectType, *(&position), false));`
-- GetInterface(&emitter,0x4b) inside the && chain -- operator= reads GetPItem into a named local before SetPItem --
CDefPointer dtor `if (m_p && --m_p->m_refCount == 0) m_p->Destroy();` -- same-TU noinline out-only stand-ins for GetSubDef and
GetEffectFromString under `#pragma auto_inline(off/on)`. No optimize pragma, no Flags line.

Differences of THIS row vs FireBeam (from the two disassemblies):
- No early `if (BeamActive) return;` -> retail pushes ebx/esi/edi at entry, this in ESI, def pointer cached in EDI (FireBeam: edi=this, esi=def).
- String "BURNINATE_ATTACK" 0x01285BA4 (FireBeam "BEAM_ATTACK").
- After the found test: `call 0x0049D870` (CWorld::GetFrame) FIRST, its result stored to [esi+0xA0] (LastExplodeFrame) interleaved with the Create argument set-up, then Create.
- After the PBeamWeaponEffect assignment + temp dtor: `mov byte [esi+0xBC],1` (FlameActive = true) BEFORE the GetPItem tests (FireBeam sets BeamActive after the attach block).
- Final def release at the common exit (`mov ecx,[esp+10h]` ... after `pop edi/esi/ebx`).

## 1. Retail shape
`sub esp,38h; push ebx; push esi; push edi` -- no frame pointer (no /Oy-), no SEH (no EH state although CCharString/CIntelligentPointer
temporaries have dtors -> TU built without /EHsc, like the sibling), plain `ret` (thiscall, no args), /O2-style (no size idioms).
Stack (offsets relative to esp after the 3 pushes): +0x0F found bool, +0x10 CDefPointer<CSpecialEffectsDef> effects (=0),
+0x14 CCharString temp (both names), +0x18 effectType (=0, stored TWICE: [esp+1c] after 1 push and [esp+20] after 2 pushes; same in
FireBeam -- reproduced by the FireBeam source), later reused for the CIntelligentPointer temp and the 0x4B key,
+0x20 C3DVector position, +0x2C CRightHandedSet (24 B). ebx = conditional-temporary flags (1 = counted def copy, 2 = name temp).

## 2. Name / signature (two sources)
Ego_r.pdb `ghidra_out/egor_pdb_names.tsv` + FableWin.pdb `ghidra_out/_fablewin_api.tsv` / `naming_stragglers/FableWin.exe.signatures.tsv`:
`?StartBeamEffect@CModeDragonStrafeBurninate@NCreatureMode@@AAEXXZ` -> private, `void __thiscall StartBeamEffect()`, non-const.
Manifest label correct (class needs namespace NCreatureMode). Not virtual (vtable 0x01268718 slots 0,1,3,4,5,6 are other fns).
Caller: 0x008DE10E inside 0x008DE080 CModeDragonStrafeBurninate::FrameUpdate (unlanded, manifest-gaps).

## 3. Callees (landed file / PDB spelling)
- 0x0099EBF0 CCharString(const char*, long=-1): compiled/00/99/CCharString_Constructor_0099ebf0.cpp
- 0x0099EAE0 ~CCharString: compiled/00/99/CCharString_CCharString_0099eae0.cpp
- 0x004C08C0 NGraphicAppearance::CTCGraphicAppearance::GetDummyObject -- NOT landed. PDB (FableWin 0x004c1b40 sym):
  `?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z`
  = `bool GetDummyObject(const CCharString&, long, unsigned long, C3DVector&, CRightHandedSet&, float) const`.
  DRAFT DISAGREES (pointer params, non-const, `const CCharString*`). FireBeam used `const CCharString&` + pointers (same bytes).
- 0x00669580 `??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z`
  compiled/00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp -- real sig is const member taking `CDefPointer<const CSpecialEffectsDef>*`
  (draft and FireBeam use a non-const reference stand-in; bytes identical). Shared header engine/CDefClassBase_GetSubDef.h, opt-in FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE.
- 0x00669D30 `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z` compiled/00/66/CSpecialEffectsDef_GetEffectFromString_00669d30.cpp (draft OK). engine/CSpecialEffectsDef.h opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY.
- 0x0049D870 `?GetFrame@CWorld@@SIJXZ` (static __fastcall long) compiled/00/49/CWorld_GetFrame_0049d870.cpp. DRAFT WRONG: `GetWorldFrame()` free fn -> use `CWorld::GetFrame()` (UpdateBeam sibling does).
- 0x006E0880 `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z` (static __fastcall, `CThing* Create(long, const C3DVector&, bool)`), not landed;
  manifest functions.tsv: Create / CTCDParticleEmitter. DRAFT WRONG: `CreateEffectThing(long, const C3DVector*, bool)`; FireBeam spells it right.
  (Other landed rows e.g. CCreatureActionJackFloatUp_PerformAction_008c0fb0.cpp also spell CreateEffectThing -> NAME_FIXES candidate.)
- 0x00A01B10 ??0CBaseIntelligentPointer@@QAE@XZ, 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ (virtual dtor),
  0x00A01B90 ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z, 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
  (compiled/00/a0/CBaseIntelligentPointer_*.cpp). Draft's SetPItem(CThing*) / non-virtual dtor are wrong spellings. CIntelligentPointer<CThing> vtable 0x01238C6C.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) compiled/00/40/CVectorMap_LowerBound_0040f020.cpp; draft's CInterfaceMap::LowerBound(const long*) is the retired spelling -> use engine/CVectorMap.h.
- 0x006E0BE0 `?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z` compiled/00/6e/CTCDParticleEmitter_AttachToThing_006e0be0.cpp
  = (CThing&, long, const CCharString&, long, float). DRAFT WRONG (pointers).
- vtable slot 1 (+4) on the def = Destroy (CDefBase), called on refcount 0.

## 4. Members
Mode (retail = Ego_r layout minus 4; no shared header for CModeDragonStrafeBurninate; landed OnDeactivate 0x008DCB00 models it as
CModeBase{vptr, CThing* ParentThing +4} -> CModeDragonFlyingBase -> CModeDragonFlying (0x94) -> Timer +0x94):
- +0x04 ParentThing (CModeBase) -- draft calls it Owner (FireBeam also "Owner").
- +0x98 `CIntelligentPointer<CThing> PBeamWeaponEffect` -- draft "Beam" WRONG.
- +0xA0 `long LastExplodeFrame` (written with CWorld::GetFrame) -- draft "BeamStartTime" WRONG.
- +0xBC `bool FlameActive` -- draft "BeamActive" WRONG.
(struct_layouts_egor.tsv CModeDragonStrafeBurninate: Timer 152, PBeamWeaponEffect 156, LastExplodeFrame 164, FlameStartPos 168, FlameEndPos 180, FlameActive 192, FlameStartPercentage 196, FlameStopPercentage 200; Ego_r +4.)
CThing (engine/CThing.h, shared; opt-in FABLE_RETAIL_CTHING_GETTC_EXPANDED used by OnActivate/OnDeactivate of this class, gives GetTC(id,&p) with the expanded LowerBound ternary; FABLE_RETAIL_CTHING_HASTC_FORCEINLINE exists):
+0x20 UsingComponents (bitset<274>; 0x4B tested as `[+28h] & 0x800` -> `test ch,8`), +0x44 Components (CVectorMap), +0x64 PTCGraphicAppearance,
+0x70 PBaseDef (CDefPointer<const CThingBaseDef>, the counted copy), +0x91 bit0 Dead (IsDead()).
Draft models locally: CThing, CCharString, CRightHandedSet (engine/CRightHandedSet.h), C3DVector (engine/C3DVector.h), CDefPointer (engine/CDefPointer.h),
CIntelligentPointer (engine/CIntelligentPointer.h, opt-ins FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN*/COPY*/CTOR_OUTOFLINE), CSpecialEffectsDef, CDefClassBase,
CTCGraphicAppearance (NGraphicAppearance_CTCGraphicAppearance.h, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT), CTCDParticleEmitter (engine/CTCDParticleEmitter.h), CInterfaceMap.
Note FireBeam (the matched twin) itself still uses local models for most of these + engine/C3DVector.h + engine/CVectorMap.h; copying it is the safe path.

## 5. Visibility
GetSubDef<CSpecialEffectsDef> and GetEffectFromString are out of line in retail but their bodies must be visible as out-only
stand-ins (same-TU noinline) so effects/effectType addresses don't escape -- that's what FireBeam does and what places the zero
stores. LowerBound 0x0040F020 out of line (CVectorMap.h same-TU noinline body). Intelligent-pointer ctor/dtor/Get/Set out of line.

## 6. Siblings
- 0x008DB620 CModeDragonFireBeam::StartBeamEffect (twin, see 0) -- auto_inline pragmas only.
- 0x008DC900 CModeDragonStrafeBurninate::UpdateBeam (compiled/00/8d/NCreatureModeCModeDragonStrafeBurninate_UpdateBeam_008dc900.cpp) -- local mode model with LastExplodeFrame +0xA0 / FlameActive +0xBC, CWorld::GetFrame.
- 0x008DCB00 OnDeactivate / 0x008DC6C0 OnActivate -- `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` + engine/CThing.h, mode class chain.
- 0x008D7BB0 EndBeamEffect (compiled/00/8d/NCreatureModeCModeDragonStrafeBurninate_EndBeamEffect_008d7bb0.cpp).

## 7. Old-draft model errors
- Control flow: `if (!found) return;` -- retail has no early return; body is `if (found) {...}` with the def release at one exit (FireBeam form).
- Names: Beam/BeamStartTime/BeamActive/Owner/GetWorldFrame/CreateEffectThing -> PBeamWeaponEffect/LastExplodeFrame/FlameActive/ParentThing/CWorld::GetFrame/CTCDParticleEmitter::Create.
- GetDummyObject / AttachToThing pointer args where PDB has references; Create takes `const C3DVector&` (FireBeam passes `*(&position)`).
- `Owner->GetDef()` returning CCountedDefPointer by value vs FireBeam's `CDefClassReference(Owner->m_pDef)->` temporary: use FireBeam's.
- CDefPointer dtor written via Release() helper vs FireBeam's `if (m_p && --m_p->m_refCount == 0) m_p->Destroy();`.
- HasTC + separate GetTC (two forceinlines) vs FireBeam's single GetInterface(&emitter, 0x4b) in the && chain.
- operator= not reading GetPItem into a named local.
- FlameActive=true placement: after the assignment, before the GetPItem tests (draft has this right).

## 0x006F3A30

# DOSSIER 0x006F3A30 (481 B) -- STLport _S_sort<const CThingCreatureBase*, allocator, CThingCompare_Nearest>

## 1. Retail shape (python3 $S/w17/xdis.py 006f3a30 0x1f1)
- No frame pointer (`sub esp,10Ch ; push ebx/ebp/esi ; mov esi,ecx ; push edi`), no SEH prologue, `ret 0Ch`.
  -> /Oy (default /O2), NO /EHsc frame; counters built with plain `vector constructor iterator` 0x004038C0 (not the eh one).
- ecx = the list (`__that`), comparator 12 bytes by value on the stack (ret 0Ch). /Gr TU: free STL templates are __fastcall
  (sibling _S_merge 0x006F04C0 takes the two lists in ecx/edx, comp on stack, ret 0Ch).
- Frame 0x10C: [esp+10h] = &that spill (later reused as the final-loop down counter fill-1), [esp+14h] = carry._M_node
  (carry also cached in ebx), [esp+18h] = _M_transfer tmp spill, [esp+1Ch..11Bh] = counter[64].
- Alignment fillers `lea ebx,[ebx]` (6 B) at 0x6F3A8A and `lea esp,[esp]` (7 B) at 0x6F3B49 = loop-top alignment (normal /O2).
- Flow: size<2 early-out (`next==node || next->next==node`, jumps to epilogue, no temporaries built) ->
  carry ctor inline (`push 0Ch ; call malloc`; sentinel self-linked; NO `Node=0` store survives) ->
  `vector constructor iterator(&counter, 4, 0x40, 0x006F2BB0)` -> fill=0 (ebp) -> rotated `while(!that.empty())`:
  splice(carry.begin(), that, that.begin()) inline (guard `position==i || position==j`, then the six-store _M_transfer
  with tmp spilled to [esp+18h]) -> `i=0 (edi); while(i<fill && !counter[i].empty()) { _S_merge(counter[i] /*ecx*/, carry /*edx=&[esp+14h]*/, comp); carry.swap(counter[i++]); }`
  (swap = `mov eax,ebx ; mov ebx,[esi] ; mov [esi],eax ; mov [esp+14h],ebx`) -> `carry.swap(counter[i]); if(i==fill) ++fill;`
  -> `for(i=1;i<fill;++i) _S_merge(counter[i], counter[i-1], comp)` (count-down in [esp+10h], edi walks &counter[i])
  -> `that.swap(counter[fill-1])` -> counters destroyed inline in REVERSE (ebp=40h down-count, edi from &counter[64]):
  clear() loop of free, sentinel relinked, `if (node) free(node)` -> carry destroyed: clear loop, relink, `free(ebx)`
  with NO null test (only the counter path tests null; carry's node lives in ebx).
- Comparator copy for each merge: `sub esp,0Ch ; mov eax,esp ; mov [eax],ecx ; [eax+4] ; [eax+8]` (dword copy of 3 floats).

## 2. Identity
- Manifest label `std::list::sort` (functions.tsv: `006f3a30 sort std::list`) is CLOSE BUT WRONG FUNCTION: the real
  list::sort member is the 0x31-byte forwarder at 0x006F4040 (`mov edx,[esp+4] ; sub esp,0Ch ; copy comp ; call 0x006F3A30 ;
  ret 0Ch`, ecx passed through) = STLport 4.6 `template<class _StrictWeakOrdering> void sort(_StrictWeakOrdering __comp)
  { _S_sort(*this, __comp); }`. 0x006F4040 is not in functions.tsv. So 0x006F3A30 = STLport _list.c
  `template <class _Tp, class _Alloc, class _StrictWeakOrdering> void _S_sort(list<_Tp,_Alloc>& __that, _StrictWeakOrdering __comp)`.
  Older label "CBinaryTree_Alt::Rebuild" (hand260 rows.tsv) is a placeholder.
- Element type / comparator (two sources): Ego_r.pdb egor_pdb_names.tsv 0x0071a120
  `??$sort@VCThingCompare_Nearest@@@?$list@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@QAEXVCThingCompare_Nearest@@@Z`
  and FableWin.pdb fablewin_pdb_names.tsv 0x02433550 (same symbol; + merge 0x0243b380). Third: callers -- landed
  CCombatManager list rebuild 0x006EBA60 (calls 0x6F3A30 at 0x6EBB8C/0x6EBBB0 on its ranged/melee creature lists) and
  landed CThingCompare_Nearest::operator()<const CThingCreatureBase> 0x006EF290 (its only caller is _S_merge 0x6F04C0).
  -> list<const CThingCreatureBase*>, comparator CThingCompare_Nearest (12 B, member `Pos` C3DVector; struct_layouts_egor.tsv).
- Mangled leaf (CONSTRUCTED from STLport signature + /Gr; no PDB holds STLport's _S_sort):
  `??$_S_sort@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@VCThingCompare_Nearest@@@std@@YIXAAV?$list@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@0@VCThingCompare_Nearest@@@Z`
  (YI = __fastcall, X = void; `@0@` std backref as in FableWin `??$_Debug_order@...YIXV?$_List_iterator@...@0@`).
- Signature: `void __fastcall std::_S_sort(list<const CThingCreatureBase*>& __that, CThingCompare_Nearest __comp)`.

## 3. Callees
| target | landed row | landed name | real name / spelling issue |
|---|---|---|---|
| 0x00BFEA0E | rebuild/src/compiled/00/bf/global_malloc_00bfea0e.cpp | malloc | CRT malloc (allocator::allocate) |
| 0x00BFEA14 | rebuild/src/compiled/00/bf/global_free_00bfea14.cpp | free | CRT free (allocator::deallocate) |
| 0x004038C0 | rebuild/src/compiled/00/40/global_vectorconstructoriterator_004038c0.cpp | `vector constructor iterator` | compiler helper -- emitted for `list<T> __counter[64]` |
| 0x006F2BB0 (ptr arg) | rebuild/src/compiled/00/6f/CRetailListnode0xc_Initialise_006f2bb0.cpp | CRetailList_node0xc::Initialise | really the ??_F default-ctor closure of list<const CThingCreatureBase*> (ctor has default allocator arg; `head=0; malloc(0xC); self-link; ret`). Landed name is a placeholder -> NAME_FIXES candidate. |
| 0x006F04C0 | rebuild/src/compiled/00/6f/global_Smerge_006f04c0.cpp | _S_merge (__fastcall, SMarkerList&) | STLport _S_merge<const CThingCreatureBase*,allocator,CThingCompare_Nearest>; its local model wrongly uses CThingMarker / SMarkerList and a local CThingCompare_Nearest {X,Y,Z; long operator()} |
- Old draft spelling vs PDB: draft calls the list `SMarkerList`, element `CThingMarker*` (WRONG: const CThingCreatureBase*),
  function `SMarkerList::sort` member (WRONG: free `_S_sort`, __fastcall). Landed caller 0x006EBA60 calls the comparator
  `CCompareByDistance` (WRONG: CThingCompare_Nearest, two PDBs) and the target `list::sort` -- NAME_FIXES for that caller.

## 4. Members touched
- list: +0 `_M_node._M_data` (STLport _List_base, shared stlport/fable_containers.h `_STLP_alloc_proxy _M_node`).
- node: +0 `_M_next`, +4 `_M_prev` (_List_node_base), +8 `_M_data` (not read here). No engine class member is touched.
- CThingCompare_Nearest: only copied as 12 opaque bytes. Shared header rebuild/include/engine/CThingCompare_Nearest.h
  (private `C3DVector Pos`, ctor from const C3DVector&, template operator() returning T*); no FABLE_RETAIL_* macros.
- Shared stlport list (rebuild/include/stlport/fable_containers.h ~l.703-1000) has NO splice / swap / sort / _S_merge /
  _M_transfer -> an opt-in (e.g. new FABLE_RETAIL_STLP_LIST_SORT) would have to add STLport 4.6 _list.h splice(pos,list&,i),
  swap, _List_global/_M_transfer, `sort(comp){_S_sort(*this,comp);}`, plus _list.c _S_merge/_S_sort, with
  FABLE_STLP_FASTCALL (defined to __fastcall under _MSC_VER only when one of the opt-ins in l.17 is defined -- extend that list).
  Existing list macros: FABLE_RETAIL_STLP_LIST, _LIST_CLEAR_NOINLINE, _LIST_CLEAR_OUTOFLINE, _LIST_BASE_OUTOFLINE,
  _LIST_BASE_DTOR_VISIBLE_NOINLINE, _LIST_DTOR_OUTOFLINE, ... (none needed: everything here is inline in retail).
- allocator::deallocate in the shared header is `if(p) free(p);` (l.329) -- matches the counters' null test.
- Locally modelled in the old draft though a shared header exists: CThingCompare_Nearest (engine/CThingCompare_Nearest.h),
  CThingMarker (engine/CThingMarker.h -- and the wrong type anyway), the STLport list/node (stlport/fable_containers.h);
  CThingCreatureBase has engine/CThingCreatureBase.h.

## 5. Callee-body visibility
- _S_merge 0x006F04C0 is out of line (called twice) and at a LOWER address in the same /Gr STL TU -> body visible, defined
  earlier, simply not inlined (template too big). Declaration-only is acceptable but the landed one is a plain definition.
- list ctor: 64 counters via ??_F closure 0x006F2BB0 (out of line, lower address, same TU) but carry's ctor is INLINE.
- list dtor / clear(): fully inline (both the counter loop and carry). splice/_M_transfer/swap/empty: inline.
- comparator operator() 0x006EF290 is only reached through _S_merge -- not relevant here.
- 0x006F4040 (list::sort forwarder) is at a HIGHER address -> defined after _S_sort in the TU (same as STLport order:
  _S_sort instantiated by the member's call).

## 6. Siblings (same TU neighbourhood, 0x006F0000-0x006F5000)
- global_Smerge_006f04c0.cpp: no Flags/pragmas; __fastcall spelled explicitly; local models (do NOT copy the types).
- std_partition_006f2b30.cpp: `ret 8`, /Gr `__fastcall` free STL template, no pragmas.
- STL_swap_006f15e0.cpp: /Gr __fastcall, `#pragma pack(push,1)` around engine/C3DVector.h.
- stdRbtree_clear_006f0bc0.cpp: `#define FABLE_RETAIL_STLP_MAP` + stlport/fable_map.h (shared-header pattern for STLport).
- stdvectorCDefendingCombatantInfoCProng_operator_006f3c20.cpp (next row): `#define FABLE_RETAIL_CINTELLIGENTPOINTER`,
  `#pragma auto_inline(off/on)` brackets.
- Caller CCombatManager_CCombatManagerRebuildCombatLists_006eba60.cpp: `// Flags: /O2 /Oy /W3`, __fastcall, FABLE_FORCEINLINE helpers.

## 7. Old draft model errors (DIFFER 496 vs 481, +15 B)
- Carry destruction: draft ~SMarkerList does `if (Node) free(Node)` for carry too; retail frees carry's node with no null
  test (counters keep it). ~9-15 B of the excess likely here; also check that carry's `Node = 0` dead store disappears.
- Wrong function kind/name/types (member SMarkerList::sort on CThingMarker*): should be __fastcall free
  `_S_sort(list<const CThingCreatureBase*>& __that, CThingCompare_Nearest __comp)`; register allocation (this in esi,
  carry in ebx, fill in ebp, i in edi) follows from a reference parameter + the STLport body.
- Draft's counter dtor order / form: retail destroys counters 63..0 inline (compiler array-dtor unrolled to a loop) then carry
  -- consistent with declaration order carry, counter[64] (reverse destruction). Keep that order.
- Draft's splice calls _M_transfer which has its own `position != last` guard; retail shows only the splice guard
  (`position==i || position==j`) -- fine if the redundant test folds, but verify.
- Draft is otherwise the verbatim STLport 4.6 _S_sort; the final swap and merge ladder match retail.

## 0x0085D180

# DOSSIER 0x0085D180 NThingMap::CThingMap::InitMapwhoLevels (481 bytes)

## 1. Retail shape (xdis 0085d180 0x1f1)
- __thiscall, `ret` (no args), `sub esp,24h; push ebx/esi/edi` (ebp pushed later, inside the guarded loop at 0x85D1CF). No frame pointer, no SEH -> default flags (siblings use `// Flags: none`).
- this->ebx; edi = &MapwhoLevelInfo (this+4) kept across the whole body; MapwhoLevelInfo._M_start reloaded (`mov eax,[edi]`) before every store.
- Prologue: `MapwhoLevelInfo.resize(MaxLayers, tmp)` call 0x85E870 with tmp at [esp+1Ch] UNINITIALISED (=> CMapwhoLevelInfo() is an inline EMPTY ctor);
  then `Cells.resize(MaxLayers, tmp)` call 0x85ED20 with a vector temp at the SAME slot zeroed (3 dwords) and **no destructor of the temp afterwards** (no free, no EH state).
  The out-of-line one-arg resizes 0x0085ED90 (vector<vector<Cell>>) and 0x0085E970 (vector<Cell>) show the same shape (zeroed temp, call, no dtor).
- Loop: `for (long i = 0; i < MaxLayers; ++i)` signed (jle guard, jl back-edge), guarded-do form; i lives in [esp+1Ch] (memory),
  three strength-reduced cursors: esi = i*20 (info offset), [esp+10h] = i*12 (Cells offset), ebp = &LayerInfo[i] (0x0137DAD8 + 16*i, in a register).
- Per level: ecx = LayerInfo[i].Size loaded once; `MapwhoLevelInfo[i].NoThings = 0` is stored BEFORE the `size == -1` test.
- Size == -1 arm: Size=-1, Extents.X=1, Extents.Y=1, ScaleShift=-1 (in that order), then Cells[i].resize(1) **fully inlined**:
  CThingMapCell temp = 0 at [esp+18h] (built before the size compare), `if (1 < size()) erase(begin()+1,end())` inline (copy loop of
  `_M_finish - last` elements with its count spilled to [esp+18h] = temp slot, `_M_finish = result`) else `_M_fill_insert(end(), 1-size(), temp)` call 0x0085E6C0.
- Else arm: X=(BRX-TLX)/size; Y=(BRY-TLY)/size (cdq/idiv, width recomputed from memory each time: edx=TLX first, eax=BRX, sub);
  `if (X*size < width) ++X` (load/inc/store), `if (Y*size < height) ++Y` (`inc dword [eax]`), then Cells[i].resize(Y*X, CThingMapCell())
  **out of line** call 0x0085E8D0 (n = `mov edx,Y; imul edx,X`), temp zero stored after the pushes; then ScaleShift = LayerInfo[i].ScaleShift ([ebp+4]) and
  Size = LayerInfo[i].Size ([ebp], RELOADED after the call) -- in that order.

## 2. Identity
- Name proven (two sources): Ego_r.pdb `?InitMapwhoLevels@CThingMap@NThingMap@@IAEXXZ` (egor_pdb_names.tsv 00853a50), FableWin.pdb same mangling (_fablewin_api.tsv 017b6736/029d96c0,
  FableWin.exe.signatures.tsv). Protected (`IAE`), `void ()`, non-const, non-virtual. Manifest label "CThingMap::InitMapwhoLevels" is right but lacks namespace NThingMap.
- Caller: only 0x0085D3B9 in landed ctor `NThingMap::CThingMap::CThingMap` rebuild/src/compiled/00/85/NThingMapCThingMap_CThingMap_0085d370.cpp.
- Globals: 0x013BA848 = `NThingMap::MaxLayers` (`?MaxLayers@NThingMap@@3JA`, landed spelling in Clear/GetAllThings/GetMemoryFootprint/dtor rows) -- draft's `g_NoMapwhoLevels` is WRONG.
  0x0137DAD8 = `NThingMap::LayerInfo[]`, element `NThingMap::CLayerInfo {long Size, ScaleShift; CRGBColour UsedColour, UnusedColour;}` (16 bytes; ctor `??0CLayerInfo@NThingMap@@QAE@JJABVCRGBColour@@0@Z`,
  `?InitLayerInfo@NThingMap@@YIXXZ`), declared `extern const CLayerInfo LayerInfo[];` in engine/NThingMap_CThingMap.h (MAP_VALIDATE branch). Draft's `SMapwhoLevelDef g_MapwhoLevelDefs` is WRONG.

## 3. Callees
| addr | landed file | landed name | PDB / note |
|---|---|---|---|
| 0x0085E870 | 00/85/stdvector_resize_0085e870.cpp | vector<NThingMap::CMapwhoLevelInfo>::resize(n,const&) | proven; Ego_r only has 1-arg `?resize@?$vector@VCMapwhoLevelInfo...QAEXI@Z` (Dinkumware) |
| 0x0085ED20 | 00/85/stdvector_resize_0085ed20.cpp | vector<vector<CThingMapCell>>::resize(n,const&) | proven; leaf in file |
| 0x0085E8D0 | 00/85/STLvector_resize_0085e8d0.cpp | vector<NThingMap::CThingMapCell>::resize(n,const&) ret 8 | callers 0x85D317 (here) and 0x85E982 (1-arg wrapper) |
| 0x0085E6C0 | 00/85/stdvectorCUnprovenEnum40085dc30_Mfillinsert_0085e6c0.cpp | vector<CUnprovenEnum4_0085dc30>::_M_fill_insert | **NAME_FIX**: called here on Cells[i] => element is NThingMap::CThingMapCell (`vector<NThingMap::CThingMapCell>::_M_fill_insert`); landed descriptive enum name is wrong |
Draft spellings: draft's callee descriptions are right except globals (see 2); draft's CThingMapCell member `PFirst` is wrong -> PDB `PFirstNode` (CMapwhoNode*).

## 4. Members (Ego_r struct_layouts_egor.tsv; retail STLport 12-byte vectors)
- CThingMap (header rebuild/include/engine/NThingMap_CThingMap.h): +0x04 MapwhoLevelInfo (vector<CMapwhoLevelInfo>), +0x10 Manager (PDB: `CThingMapManager&` in MAP_VALIDATE branch; pointer in default branch), +0x14 Cells (vector<vector<CThingMapCell>>), +0x20 Area (C2DBoxI), +0x30 NoThings. Base CBaseClassNonCopyable (vptr +0).
- C2DBoxI (engine/C2DBoxI.h): TLX +0, TLY +4, BRX +8, BRY +0xC (PDB names; draft's TopLeft/BottomRight sub-structs are wrong). FABLE_RETAIL_C2DBOXI gives `C2DExtentsI GetExtents() const {return C2DExtentsI(BRX-TLX,BRY-TLY);}` -- used by sibling Validate for exactly this math.
- C2DExtentsI (engine/C2DExtentsI.h): X, Y; opt-ins FABLE_RETAIL_C2DEXTENTSI_INITIALISE / _FORCEINLINE_CTOR.
- CMapwhoLevelInfo (20): Size +0, Extents +4, ScaleShift +0xC, NoThings +0x10. CThingMapCell (4): PFirstNode. PDB has user ctors `??0CMapwhoLevelInfo@NThingMap@@QAE@XZ` and `??0CThingMapCell@NThingMap@@QAE@XZ` -- retail behaviour: CMapwhoLevelInfo() empty, CThingMapCell() : PFirstNode(0).
  In the header's FABLE_RETAIL_MAP_VALIDATE branch both ctors are DECLARED ONLY (would emit calls) -> author needs inline bodies (new opt-in or row-local note); default branch has no ctors at all.
- Header opt-ins present: FABLE_RETAIL_MAP_VALIDATE (full PDB class: InitMapwhoLevels protected decl, MaxLayers, CLayerInfo/LayerInfo with CRGBColour, GetCell inline, protected members), FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE, FABLE_RETAIL_CTHINGMAP_GETAREA, FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST.
- Draft models LOCALLY (all have shared headers): vector (-> stlport/fable_containers.h std::vector), C2DCoordI, C2DBoxI, C2DExtentsI, CMapwhoLevelInfo, CThingMapCell, CThingMap (-> engine/NThingMap_CThingMap.h), CThingMapManager.

## 5. Callee visibility
- All three resize bodies are STLport inline members emitted as COMDATs in this TU's neighbourhood (0x85E870/0x85E8D0/0x85ED20, plus uncalled 1-arg wrappers 0x85E970/0x85ED90 whose emission suggests the source called the ONE-ARG `resize(n)` -> `resize(n, _Tp())`; Ego_r names only the 1-arg resizes too).
- The missing temp-vector destructor after 0x85ED20 means c2 saw resize's body and proved the temp untouched: the pattern of `FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE` in stlport/fable_containers.h:540 (`__declspec(noinline)` with visible body; chunk 777 row 0x0055F710). But that macro is TU-wide and would also stop the inline Cells[i].resize(1) expansion -- the per-site split (first site inlined, the Y*X site out of line) is the open problem (draft tried in-class, auto_inline(off), __forceinline). Other macros: FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL (1-arg decl only), FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE (keep OFF: erase is inline here).
- _M_fill_insert out of line (declared only in fable_containers.h, fine).
- Same-TU landed neighbours defined earlier: Validate 0x0085CDE0, Clear 0x0085CEC0, GetMemoryFootprint 0x0085CFE0, GetAllThings 0x0085D010, ~CThingMap 0x0085D0A0; ctor 0x0085D370 follows.

## 6. Sibling rows (rebuild/src/compiled/00/85/)
- NThingMapCThingMap_Validate_0085cde0.cpp: `// Flags: none`, `#define FABLE_RETAIL_MAP_VALIDATE`, `#define FABLE_RETAIL_C2DBOXI`, `#include "engine/NThingMap_CThingMap.h"`, uses `MaxLayers`, `Area.GetExtents().X/size`, `GetCell((unsigned char)c1,..)` -- the idiom to copy.
- NThingMapCThingMap_Clear_0085cec0.cpp, _GetMemoryFootprint_0085cfe0.cpp, _GetAllThings_0085d010.cpp, _CThingMap_0085d0a0.cpp (dtor), _CThingMap_0085d370.cpp (ctor): no pragmas, default flags, local models (older style).
- STLvector_resize_0085e8d0.cpp / stdvector_resize_0085ed20.cpp show the exact STLport resize/erase/__copy_ptrs bodies the inline arm must expand to.

## 7. Old draft model errors
- Wrong global names/types (g_NoMapwhoLevels, g_MapwhoLevelDefs/SMapwhoLevelDef) -> NThingMap::MaxLayers, NThingMap::LayerInfo (const CLayerInfo[]).
- Local vector lacks STLport layout/erase (__copy_ptrs/_Destroy) and has no dtor at all -- that is why the temp dtor was absent "for free"; with the real header vector the dtor must be folded via visible-body resize.
- Local C2DBoxI with TopLeft/BottomRight; real is TLX/TLY/BRX/BRY (+GetExtents()).
- Size==-1 arm: draft `Cells[i].resize(1)` (1-arg) OK in spirit; else arm draft passes explicit `CThingMapCell()` 2-arg -- likely source was `Cells[i].resize(MapwhoLevelInfo[i].Extents.Y * MapwhoLevelInfo[i].Extents.X)` (1-arg) per Ego_r 1-arg-only names.
- Draft writes `width > X*size`; retail cmp is `X*size` vs width (`jge` skip) -> `if (X*size < width)` likely; consider `Area.GetExtents().X` spelling (Validate sibling) for the repeated reloads.
- Draft spills `this` and gets a larger frame; retail keeps this in ebx, &MapwhoLevelInfo in edi, LayerInfo row pointer in ebp, i in memory.

## 0x005BE8F3

# DOSSIER 0x005BE8F3 -- NInventory::CTCInventoryBase::CreateInventoryObject (482 bytes)

## 1. Retail shape (xdis 005be8f3 0x1f2)
- thiscall, `ret 4`, one arg (long itemDefIndex). Frame pointer: `push ebp; mov ebp,esp; sub esp,0Ch` ... `leave` -> `#pragma optimize("y",off)`.
- Size idioms `push 48h; pop edi`, `push 39h; pop esi`, `push 1Ah; pop edi` -> `#pragma optimize("s",on)`. No SEH frame although a CCharString temp is destroyed (same as every sibling: the TU has no EH unwinding for it). Same pragmas as the neighbours 0x5BDF08 / 0x5BEDCB / 0x5BEAF5.
- Frame: [ebp-0Ch] = this (spilled once, only reread for the final `push [ebp-0Ch]`), [ebp-8] = CThing* copy for IS_THING_ALIVE, later reused as interface key / augmentations ptr, [ebp-4] = CCharString temp, later GetMaximumNumberOfSlots result. The dead argument slot [ebp+8] holds the CPerItemData* then the min count then keys.
- Registers: ebx = thing (xor ebx,ebx at entry; its 0 is ALSO what is pushed for the two zero args of Create -> `CThing* thing = 0;` at the top, returned at the end in every path), esi = this, edi = per-item data.
- Flow: `if (GetNumberOfItemsOfTypeInInventory(id) > 0) { thing = Create(...); if (IS_THING_ALIVE(thing)) { data = GetPerItemDataForItem(id); if (data) {weapon; augmentations; trophy} item block (runs even when data == 0) } } return thing;`
- Create call arg order (right to left): CCharString("" @0x0122D70E, -1) temp built FIRST, then `mov ecx,[esi+4]` (ParentThing) after it, push &name, 0, 0, movsx PlayerNumber(+0x90), then GetPos() with ecx=ParentThing; fastcall ecx=id, edx=&pos. The temp's dtor runs right after Create (before IS_THING_ALIVE).

## 2. Identity (two sources -- name proven)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, donor 0x0062118F) and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x02367469):
  `?CreateInventoryObject@CTCInventoryBase@NInventory@@IAEPAVCThing@@J@Z` = protected, non-virtual, non-const, `CThing* CreateInventoryObject(long)`.
- Manifest label CTCInventoryBase::CreateInventoryObject is RIGHT (namespace NInventory). Shared header already declares it:
  rebuild/include/engine/NInventory_CTCInventoryBase.h under `FABLE_RETAIL_CTCINVENTORYBASE_CREATEOBJECT` (`protected: CThing* CreateInventoryObject(long itemDefIndex);`).
- Callers (xref): call sites in CTCInventory 0x583E26 (OnItemSelected), CTCInventoryWeapons 0x5C44FA AugmentWeapon, 0x5C4671 GetFreeAugmentationSlotForWeapon, 0x5CB2B0 SetThingAsActiveWeapon, 0x5CB974 OnInitialActivate -- all landed using the CREATEOBJECT opt-in.

## 3. Callees (landed file | landed name | PDB name)
| addr | landed row | PDB spelling | draft spelling |
|---|---|---|---|
| 0x5BDF08 | 00/5b/CTCInventoryBase_GetNumberOfItemsOfTypeInInventory_005bdf08.cpp | ?GetNumberOfItemsOfTypeInInventory@CTCInventoryBase@NInventory@@QBEJJ@Z (CONST) | non-const -- fix; header has it under FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED |
| 0x99EBF0 / 0x99EAE0 | CCharString ctor (const char*, long) / dtor (fable_string.h, FABLE_RETAIL_CCHARSTRING) | ok | ok; literal "" 0x0122D70E |
| 0x4C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ (const C3DVector&) | draft "GetPosition" returning pointer -- WRONG; use GetPos() (CThing.h:411) |
| 0x703210 | 00/70/CThingObject_Create_00703210.cpp | ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (static fastcall, returns CThingObject*) | draft free "CreateThing" with C3DVector* -- WRONG; `CThingObject::Create(id, ParentThing->GetPos(), ParentThing->PlayerNumber, 0, 0, CCharString(""...))`; sibling rows pass `""` / name |
| 0x40DCE0 | 00/40/global_ISTHINGALIVECThingBuilding_0040dce0.cpp | ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z | ok |
| 0x5BE8D3 | 00/5b/CTCInventoryBase_GetBoughtPrice_005be8d3.cpp (landed name GetBoughtPrice, returns long -- WRONG) | ?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z (const, returns const CPerItemData*) -- labels_naming_wave1_2026-10-28 + labels_landing_2026-11-09c; its callee 0x5BE8B1 is CCategory::GetPerItemDataForItem, not GetBoughtPrice | draft GetBoughtRecord/SBoughtRecord -- WRONG; header: FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA. NAME_FIX for the landed 0x5BE8D3 row |
| 0x410DE0 | 00/41/STLbitset_test_00410de0.cpp | ?test@?$bitset@$0BBC@std@@QBE_NI@Z | draft CPresenceBits -- use std::bitset<274> via FABLE_RETAIL_STLP_BITSET_TEST_DECL |
| 0x40F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap::LowerBound (non-const) | draft local CInterfaceMap -- use engine/CVectorMap.h via CThing::GetTC |
| 0x765D90 | 00/76/CTCObjectAugmentations_GetMaximumNumberOfSlots_00765d90.cpp | ?GetMaximumNumberOfSlots@CTCObjectAugmentations@@QBEJXZ | draft "GetNumberOfSlots" -- WRONG |
| 0x765D80 | ..._GetNumberOfSlots_00765d80.cpp | ?GetNumberOfSlots@CTCObjectAugmentations@@QBEJXZ | draft "GetNumberOfAugmentations" -- WRONG |
| 0x766D70 | ..._AddNewSlot_00766d70.cpp | ?AddNewSlot@CTCObjectAugmentations@@QAEXXZ | draft "AddSlot" -- WRONG |
| 0x768120 | ..._AttachAugmentationToSlot_00768120.cpp | ?AttachAugmentationToSlot@CTCObjectAugmentations@@QAEXJJ@Z | ok |
| 0x7681D0 | ..._RemoveAugmentationFromSlot_007681d0.cpp | RemoveAugmentationFromSlot(long) | ok |
| 0x5D8E40 | 00/5d/CObservable_AddExclusiveObserver_005d8e40.cpp (landed name SetMember_dword0x10_005d8e40 / file CObservable::AddExclusiveObserver -- WRONG) | ?ConnectToInventory@CTCInventoryItem@@QAEXAAVCTCInventoryBase@NInventory@@@Z: Ego_r 0x00559DA0, 0x10 bytes, immediately followed by ReturnToInventory 0x00559DB0 exactly as retail 0x5D8E40 -> ReturnToInventory 0x5D8E50 (CTCInventoryItem.h:18); body stores into PInventory +0x10; caller pushes `this` = reference arg | draft "SetOwnerInventory(CTCInventoryBase*)" -- WRONG; write `item->ConnectToInventory(*this)`. NAME_FIX for 0x5D8E40. Not declared in CTCInventoryItem.h yet (needs a declaration/opt-in) |

## 4. Members
- this: CTCInventoryBase (engine/NInventory_CTCInventoryBase.h, : CTCBase) -- only ParentThing +0x04 (CTCBase, FABLE_RETAIL_CTCBASE). Draft models CTCInventoryBase LOCALLY (with __vftable) -- use header.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): UsingComponents bitset +0x20, Components CVectorMap +0x44, PlayerNumber (signed char) +0x90 (CThing.h:85), GetPos(). Draft models CThing / CThingObject / C3DVector / CCharString LOCALLY -- all have shared headers.
- CPerItemData (NInventory_CTCInventoryBase.h under FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA; retail 0x24 bytes, Ego_r 0x28): Augmentations std::vector<long> +0x00 (first +0, last +4), NumberOfKills +0x0C, TimeWielded +0x10, BestTrophyWitnessesAheadToDate +0x14.
- Interfaces (engine/ETCInterfaceType.h): 0x48 = TCI_WEAPON (72), 0xBF = TCI_OBJECT_AUGMENTATIONS (191), 0x39 = TCI_TROPHY (57), 0x1A = TCI_INVENTORY_ITEM (26). Draft's "CTCType48/CTCType39" are invented.
- CTCWeapon (engine/CTCWeapon.h, FABLE_RETAIL_CTCWEAPON + FABLE_RETAIL_CTCWEAPON_MEMBERS): NumThingsKilled +0x1C (= data->NumberOfKills), TimeWielded +0x20.
- CTCTrophy: NO shared header. Ego_r struct_layouts: BestWitnessesAheadToDate long +0x18 (= data->BestTrophyWitnessesAheadToDate). Local minimal model is the only option (pad 0x18 + long, or : CTCBase with pad to 0x18).
- CTCInventoryItem (engine/CTCInventoryItem.h, FABLE_RETAIL_CTCINVENTORYITEM): PInventory +0x10; ConnectToInventory not declared.
- CTCObjectAugmentations (engine/CTCObjectAugmentations.h): the retail (FABLE_RETAIL_CTCBASE) branch only declares GetFreeSlot / AttachAugmentationToSlot (+ GETAUGMENTATIONS / DAMAGEMULTIPLIER / MEMBERS opt-ins); GetNumberOfSlots / GetMaximumNumberOfSlots / AddNewSlot / RemoveAugmentationFromSlot exist only in the generated non-retail branch (lines 28-36) -- a new opt-in or a local declaration is needed.

## 5. Callee visibility
- IS_THING_ALIVE<CThing*>: retail call shape `mov [ebp-8],ebx ; lea ecx,[ebp-8]` (thing kept in ebx, copy in a frame slot) = the sibling idiom of 00/5c/CTCInventoryWeapons_SetThingAsActiveWeapon_005cb2b0.cpp: a SAME-TU body under `#pragma auto_inline(off)` `template <> inline bool FABLE_FASTCALL IS_THING_ALIVE<CThing*>(CThing* const& thing){ if (thing != 0 && thing->IsDead() == 0) return true; return false; }` -- VC7.1 sees the ref is not kept, so the thing stays in a register. Draft's "CThingObject->CThing temp" is the crude version of this.
- GetTC: use `FABLE_RETAIL_CTHING_GETTC_EXPANDED` (CThing.h:324-360) `thing->GetTC(TCI_x, &ptr)` -- out-pointer form; expands HasTC (bitset::test out of line 0x410DE0, FABLE_RETAIL_STLP_BITSET_TEST_DECL) + CVectorMap::LowerBound out of line 0x40F020 + the conditional `(it==end()||type<it->first)?end():it` and keys copied to a local inside the test. The mirror function NInventory::CItem::UpdateItemStats 0x5BEDCB (same TU, 0x4D8 bytes later) notes: "every interface fetch goes through the inline out-parameter GetInterface (the key words then share one frame slot, the last one the dead argument slot); writing the lookups out longhand gives a third frame word" -- exactly the [ebp-8]/[ebp+8] key slots here.
- Last block `xor esi,esi` before HasTC(0x1A) then unconditional `esi->ConnectToInventory(*this)` = `CTCInventoryItem* item = 0; thing->GetTC(TCI_INVENTORY_ITEM, &item); item->ConnectToInventory(*this);`.
- GetPerItemDataForItem 0x5BE8D3, GetNumberOfItemsOfTypeInInventory 0x5BDF08 are same-TU earlier functions (called, not inlined -- fine). std::min: inline STLport `min` (address-select `lea eax,[ebp+8] ; jl ; lea eax,[ebp-4] ; mov eax,[eax]`).

## 6. Siblings (copy idioms)
- 00/5b/NInventoryCItem_UpdateItemStats_005bedcb.cpp (mirror: thing -> per-item data; y off + s on; local GetInterface out-param model).
- 00/5b/CTCInventoryBase_AddItemToInventory_005bf32d.cpp: y off, s on, FABLE_RETAIL_CTHING_GETTC_EXPANDED, FABLE_RETAIL_STLP_BITSET_TEST_DECL.
- 00/5b/NInventoryCTCInventoryBase_SetNumberOfSelectedItemOwned_005be284.cpp: y off, s on, FABLE_RETAIL_CTCBASE/CTHING/CCHARSTRING/DEF_CLASS_BASE/CTCINVENTORYBASE_SELECTEDOWNED/CVECTORMAP_FORCEINLINE_FIND, `// Flags: none`.
- 00/5c/CTCInventoryWeapons_SetThingAsActiveWeapon_005cb2b0.cpp: y off, s on, FABLE_RETAIL_CTCINVENTORYBASE(+_CREATEOBJECT), CTHING_GETTC_EXPANDED, CTCWEAPON(+_MEMBERS), CTCINVENTORYITEM, same-TU IS_THING_ALIVE<CThing*>.
- 00/5b/CTCInventoryBase_GetNumberOfItemsOfTypeInInventory_005bdf08.cpp: y off, s on, auto_inline(off/on) brackets.

## 7. Draft model errors (vs disassembly)
- Wrong/invented names: GetPosition->GetPos (reference), CreateThing->CThingObject::Create (const C3DVector&, const CRightHandedSet*, unsigned long), GetBoughtRecord/SBoughtRecord->GetPerItemDataForItem/CPerItemData, GetNumberOfSlots/GetNumberOfAugmentations/AddSlot swapped->GetMaximumNumberOfSlots/GetNumberOfSlots/AddNewSlot, SetOwnerInventory(ptr)->ConnectToInventory(ref), CTCType48/39->CTCWeapon/CTCTrophy, constness of GetNumberOfItemsOfTypeInInventory / GetPerItemDataForItem.
- Everything modelled locally (CThing, CCharString, C3DVector, CTCInventoryBase, CVectorMap, bitset) -- use shared headers.
- FindTC returning pointer instead of GetTC(id,&ptr) out-param form -> key/frame slot mismatch (draft had one extra frame slot, 483 v 482).
- IS_THING_ALIVE declared only (no same-TU body) -> forces the address-of temp; the documented residue (ParentThing hoisted, this in edi instead of esi, literal 0 pushes instead of ebx) is the register-allocation consequence; the same-TU auto_inline(off) body + `CThing* thing = 0` at top are the known fix pattern.
- Loop forms are right: `while (aug->GetNumberOfSlots() < count) aug->AddNewSlot();` (retail jumps to the test first), then `for (i=0;i<count;++i) data->Augmentations[i] > 0 ? Attach(aug,i) : Remove(i)` (push i hoisted before the branch). count = std::min(GetMaximumNumberOfSlots, (long)Augmentations.size()) with max computed FIRST (stored [ebp-4]) then size; retail picks size when size < max (STLport min(a,b) = b<a?b:a -> min(max, size)).
- Weapon block: store order NumThingsKilled then TimeWielded. Trophy block: unconditional copy (no >0 test here).

## 0x008C6860

# DOSSIER 0x008C6860 -- CCreatureAction_TrollThrowProjectile::PerformAction (482 bytes)

## 1. Retail shape (xdis 008c6860 0x1f2)
- __thiscall, virtual, plain `ret` (0x8C6A41). NO frame pointer: `sub esp,10h; push ebx/ebp/esi/edi`, ebp is a general register
  (FPO, default /Oy). NO SEH prologue despite three CDefPointer dtors + a CCharString temp (no EH state; siblings same).
- Frame 0x10. Slot map (esp offsets after the 4 pushes):
  - [esp+13h] byte: HasTC(0x46) result `shr ecx,6; and cl,1; mov [esp+13h],cl; je exit` -- a STORED bool (dead store), i.e. the
    default CThing::HasTC body `const bool& found=UsingComponents.test(type); return found;` (CThing.h:298), NOT the
    FinishAction 0x8C6A50 form `test byte [eax+28h],40h` (FABLE_RETAIL_CTHING_HASTC_FORCEINLINE / GETTC_EXPANDED form).
  - [esp+1Ch]: key 0x46 for LowerBound, later reused for the accuracy long (fild).
  - [esp+14h]: CDefPointer<const CWeaponDef> GetSubDef out slot (`xor ebp,ebp; mov [esp+14h],ebp` BEFORE the PCreature GetPItem).
  - [esp+18h]: CDefPointer<const CThingShotDef> GetDef out slot (zeroed AFTER the GetSubDef call, before GetWorld); after the call
    its value is MOVED into ebp (no addref, slot not released) -> slot reused for the GetString CCharString temp, then zeroed again
    as the CDefPointer<const CMeleeCombatAbilityDef> passed by reference to Init. => by-value return of the shot def
    (e.g. a __forceinline wrapper returning CDefPointer, or GetDef into a temp then moved), shotDef lives in ebp.
- Carrying TC kept in ebp from the lookup (ternary end-or-greater pick, `mov ebp,[eax+4]`), then ebp reused for shotDef.
- Order of ops: PCreature.GetPItem -> HasTC -> LowerBound(0x46) -> PShot.GetPItem (null test) -> PShot.GetPItem (+0x91 bit0 dead
  test) -> PShot.GetPItem -> carrying->RemoveThing -> weaponDef=0 -> PCreature.GetPItem->PBaseDef(+0x70)->GetSubDef(&weaponDef)
  -> shotDef=0 -> GetWorld()->DefinitionManager(+0x10) -> GetDef(weaponDef->Shot(+0xA0), &shotDef) -> PCreature.GetPItem ->
  GetShotAccuracyPercentage -> GetString(shotDef->ShotDefinitionClassName +0x60) into temp -> AddTC(name, true, 0) on
  PShot.GetPItem (shot TC in ebx) -> ~CCharString -> melee def = 0 -> if (UseTargetPos +0x128).
- Branch A (UseTargetPos): fild accuracy; push &melee, 0 (0.0f), 1.0f, accuracy, &TargetPos(esi+11Ch), 0 (weapon),
  PCreature.GetPItem() (evaluated AFTER the TargetPos push), then by-value shotDef copy (`push ecx; mov [esp],ebp; if(ebp) inc [ebp+4]`)
  -> call 0x007BF790.
- Branch B: PCreature.GetPItem() evaluated FIRST into edi (before the fild), then fild, push &melee,0,1.0f,accuracy,
  PTarget(+0xA8).GetPItem(), 0, edi, copy -> call 0x007C0690. (Draft's named `shooter` local reproduces this; keep it.)
- Epilogue, three DIFFERENT release shapes, none stores Object=0:
  1. melee def [esp+18h]: null test, then `mov esi,[eax+4]; mov ecx,eax; add eax,4; dec esi; mov [eax],esi; mov edx,esi; test edx,edx`
     = the count-through-reference form (CDefPointer.h FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF shape; DragonRearUpFull 0x8F3630
     gets it with a local CMeleeCombatAbilityDef : CDefVirtual, CRefCounted{DecRef(); GetRefCount()} ReleaseTemporary model).
  2. shotDef (ebp): `test ebp,ebp; je; dec [ebp+4]; jne; call [eax+4]` (in-place default form).
  3. weaponDef [esp+14h]: NO null test: `mov ecx,[esp+14h]; dec [ecx+4]; jne; call [edx+4]`. IceTroll 0x8C6DB0 notes (g6): with the
     same-TU CDefClassBase::GetSubDef<T> template body visible, "its release loses the null test". Or written as an
     explicit unconditional release (DragonRearUpFull writes `weaponDef->Release()` by hand).

## 2. Name (proven, 3 sources)
- `?PerformAction@CCreatureAction_TrollThrowProjectile@@UAEXXZ` -- Ego_r.pdb (egor_pdb_names.tsv 008fddb0), FableWin
  (_fablewin_api.tsv 017dc53f), vtable 0x01281694 slot 3 (rtti-vtables.tsv; xref only caller = .rdata 0x012816A0). Ego_r order
  Initialise/FrameUpdate/PerformAction/FinishAction (8fd9f0/8fdcd0/8fddb0/8fdfd0) = retail 8c64f0/8c67a0/8c6860/8c6a50 (all
  but this one landed). Manifest label is RIGHT. `void PerformAction()` public virtual, non-const, no params.
- Class derives CCombatActionBase (retail size 0x114); this class sizeof 0x130.

## 3. Callees (landed row / PDB spelling) -- draft disagreements flagged [!]
- 0x00A01B50 CBaseIntelligentPointer::GetPItem `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (const, returns CBaseObject*)
  rebuild/src/compiled/00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp. Use engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER), `.Get()`.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (non-const QAE) 00/40/CVectorMap_LowerBound_0040f020.cpp.
  [!] draft's `CInterfaceMap::LowerBound(const long*)` local model -> use engine/CVectorMap.h typedef as FrameUpdate/FinishAction do.
- 0x006A19B0 `?RemoveThing@CTCCarrying@@QAEXAAVCThing@@@Z` 00/6a/CTCCarrying_RemoveThing_006a19b0.cpp. [!] takes CThing& (draft CThing*).
  engine/CTCCarrying.h opt-in FABLE_RETAIL_CTCCARRYING_REMOVETHING.
- 0x0042B1A6 CDefClassBase::GetSubDef<CWeaponDef> `??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z`
  00/42/CDefClassBase_GetSubDefCWeaponDef_0042b1a6.cpp (`ret 4`, frame-pointer TU). [!] draft calls it GetSubDef<CAbilityDef> with a
  `CAbilityDef::ShotDefIndex` -- the def is CWeaponDef and +0xA0 is CWeaponDef::Shot (engine/CWeaponDef.h:52, static-asserted).
- 0x00692940 `?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ` (protected, returns CWorld&) 00/69/CCreatureActionBase_GetWorld_00692940.cpp;
  engine/CCreatureActionBase.h opt-in FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS. [!] draft `GetGameContext()`/`CActionGameContext`.
  Some siblings spell it GetPWorld (DragonRearUpFull, Initialise) -- PDB is GetWorld. CWorld +0x10 = `CGameDefinitionManager* DefinitionManager` (Ego_r).
- 0x0078B230 CDefinitionManager::GetDef<CThingShotDef>(long, CDefPointer<const CThingShotDef>*) (landed file
  00/78/CDefinitionManager_GetDef0078b230_0078b230.cpp, byte-exact; returns bool). [!] draft `CDefinitionManager::GetDef(long, CDefPointer<CThingShotDef>*)` OK-ish; type should be const CThingShotDef.
- 0x00663B10 `?GetShotAccuracyPercentage@CThingCreatureBase@@QAEJXZ` (long, non-const) 00/66/... -- called on PCreature item (CThingCreatureBase). Draft put it on CThing [!].
- 0x009D49B0 CDefStringTable::GetString(long) -> CCharString hidden result (`ret 8`) 00/9d/...; global `GDefStringTable` 0x013CA828
  (?GDefStringTable@@3VCDefStringTable@@A). [!] draft `g_DefStringTable`. Header engine/CDefStringTable.h.
- 0x004C9D60 `?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z` (3rd param const CTCParamsBase*) 00/4c/CThing_AddTC_004c9d60.cpp; declared in CThing.h.
- 0x0099EAE0 CCharString::~CCharString 00/99/CCharString_CCharString_0099eae0.cpp (fable_string.h).
- 0x007BF790 NThingShot::CTCShotDefinitionBase::Init (C3DVector overload) PDB
  `?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1ABVC3DVector@@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z`
  (Ego_r 007be730, FableWin 017d75c1). Not landed. [!] target is `const C3DVector&` (draft: pointer -- same bytes), ability `const CDefPointer<const CMeleeCombatAbilityDef>&`,
  class is in namespace NThingShot.
- 0x007C0690 Init (thing overload) `...PAVCThing@@1PBV4@MMM...` (const CThing* target), declared in engine/NThingShot_CTCShotDefinitionBase.h
  under FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT (only this overload; the C3DVector one must be added locally or the header extended).
  Args: (shotDef by value, shooter=creature, weapon=0, target, (float)accuracy, 1.0f, 0.0f, melee).
- Vtable calls: slot 1 (`call [reg+4]`) = def pointee Delete/Release on all three def releases.

## 4. Members
- this+0x08 PCreature CIntelligentPointer<CThingCreatureBase> (CCreatureActionBase, Ego_r) -- engine/CCreatureActionBase.h.
- this+0xA8 PTarget CIntelligentPointer<CThing> (CCombatActionBase.h, opt-in FABLE_RETAIL_CCOMBATACTIONBASE_MELEE, needs
  CIntelligentPointer/CDefPointer/CCountedPointer/fable_containers/C3DVector first). [!] draft calls it PObjectToUse.
- this+0x114 PShot CIntelligentPointer<CThing>, +0x11C TargetPos C3DVector, +0x128 UseTargetPos bool, +0x12C ShotAlpha long
  (engine/CCreatureAction_TrollThrowProjectile.h, generated, sizeof 0x130 asserted; PShot is opaque bytes there, TargetPos split
  X/Y/Z). [!] draft `PProjectile` -> PDB `PShot`. Landed FrameUpdate/FinishAction model the class locally over a local
  CCombatActionBase {vptr; pad; PCreature +8; pad to 0x114}.
- CThing (engine/CThing.h): +0x20 UsingComponents bitset (bit 0x46 -> byte 0x28 bit 6), +0x44 Components CVectorMap, +0x70
  PBaseDef CDefPointer<const CThingBaseDef> (GetPBaseDef()), +0x91 bit0 = dead (IsDead()). Draft's `PDef`/`m_flags91` [!].
- CWeaponDef +0xA0 Shot (engine/CWeaponDef.h). CThingShotDef +0x60 ShotDefinitionClassName (CDefString; header field
  ShotDefinitionClassName_TablePos, engine/CThingShotDef.h; engine/CDefString.h exists). CWorld +0x10 DefinitionManager.
- Locally modelled by the draft although a shared header exists: CCharString, CDefPointer, CDefStringTable, CThing, CTCCarrying,
  NThingShot::CTCShotDefinitionBase, CBaseIntelligentPointer, CCreatureActionBase, CCreatureAction_TrollThrowProjectile, C3DVector, CVectorMap.

## 5. Callee visibility
- GetSubDef<T> template body: IceTroll 0x8C6DB0 (RELOCATION_MATCH) defines the CDefClassBase::GetSubDef<T> template body in the same TU and
  credits it with "def stays in a callee-saved reg / its release loses the null test" -- exactly our weaponDef release. Siblings
  Initialise 0x8C64F0 and DragonRearUpFull 0x8F3630 use `#pragma auto_inline(off)` + `__declspec(noinline)` same-TU stand-in bodies for
  GetSubDef<CWeaponDef> and GetDef.
- GetPItem, LowerBound, GetWorld, RemoveThing are out of line in retail (called), CDefPointer ctor/dtor/copy are inline
  (__forceinline in CDefPointer.h), HasTC/GetTC inline.
- Opt-ins available: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_GETTC_EXPANDED, FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (do NOT use: wrong
  HasTC shape here), FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CTCCARRYING_REMOVETHING, FABLE_RETAIL_CCOMBATACTIONBASE_MELEE,
  FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS, FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT, FABLE_RETAIL_DEF_CLASS_BASE,
  FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF (melee-def release shape) / _RELEASE_POINTEE.

## 6. Siblings (rebuild/src/compiled/00/8c/ unless noted)
- CCreatureActionTrollThrowProjectile_FinishAction_008c6a50.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED + engine/CThing.h + CVectorMap.h +
  FABLE_RETAIL_CINTELLIGENTPOINTER; local CCombatActionBase (0x114) and local class (PShot/TargetPos/UseTargetPos/ShotAlpha).
- CCreatureActionTrollThrowProjectile_FrameUpdate_008c67a0.cpp: same includes + FABLE_RETAIL_CTHING; `PShot.Get()`, `IsDead()`.
- CCreatureActionTrollThrowProjectile_Initialise_008c64f0.cpp: GetSubDef<CWeaponDef> + GetDef(weapon->Shot) pattern with same-TU
  auto_inline(off) noinline bodies; local CDefPointer with `if (Object && --RefCount==0) Destroy()` dtor.
- CCreatureActionIceTrollIcicleAttack_PerformAction_008c6db0.cpp: shared engine/CDefPointer.h + engine/CSubDefClassBase.h, same-TU
  GetSubDef<T> template body; `CDefPointer<const T> x(NULL_DEF); PCreature->GetPBaseDef()->GetSubDef(&x)`.
- 00/8f/CCreatureActionDragonRearUpFull_PerformAction_008f3630.cpp: SAME shot idiom (GetDef shot def -> raw pointer local, GetString temp
  in a block `{ CCharString name = GDefStringTable.GetString(shotDef->TCName); shot = AddTC(name,true,0); }`, Init 0x7BF790 with
  CDefPointerByValue<CThingShotDef> by-value param whose copy ctor addrefs, melee CDefPointer in its own block, hand release of the
  weapon def). Also 00/8c/CCreatureActionBriarRoseShotFarOutof_PerformAction_008c0900.cpp and 00/8f/..DragonLandFireball_PerformAction_008f3100.cpp (Init 0x7C0690).
  No `// Flags:` or optimize pragmas in these siblings (default flags, FPO).

## 7. Draft model errors vs disassembly
- Uses GetTC<CTCCarrying>(0x46) after a separate stored bit: OK in spirit, but the bool must be the HasTC stored byte and the
  carrying pointer selected by the end-or-greater ternary. Draft computes `hasCarrying` from m_presentBits[2] (+0x28) by hand -- OK bits, wrong spelling.
- shotDef held as a CDefPointer local in memory (frame 0x18) -- retail keeps it in ebp via a by-value/moved temp; melee def and string
  temp share its slot (block-scope them: string temp in `{}` before the melee def is declared, as DragonRearUpFull does).
- weaponDef release has a null test in the draft (CDefPointer dtor) -- retail has none.
- Init first arg by-value CDefPointer copy whose copy ctor null-tests (`test ebp,ebp; je`) -- the draft's CDefPointer copy ctor matches;
  DragonRearUpFull's CDefPointerByValue does NOT null-test (different), so use a null-testing copy here.
- Wrong names: PProjectile->PShot, PObjectToUse->PTarget, GetGameContext->GetWorld, CAbilityDef/ShotDefIndex->CWeaponDef/Shot,
  TCName->ShotDefinitionClassName, g_DefStringTable->GDefStringTable, RemoveThing(CThing*)->(CThing&), PDef->PBaseDef,
  GetShotAccuracyPercentage belongs to CThingCreatureBase, Init lives in NThingShot.

