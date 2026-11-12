## 0x007E2A60

# DOSSIER 0x007E2A60 CTCFineDialogue::GlobalUpdate (707 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 007e2a60 0x2d3)
- __thiscall (ecx=this -> edi), `ret 4` (one bool arg, unused). NO frame pointer (/Oy on), NO SEH prologue even though a
  CWideString local with a dtor lives in the answer branch (no /EHsc unwind state -> TU compiled without EH, or callees nothrow;
  sibling Show 0x007E27F0 likewise has no SEH).
- Prologue: `sub esp,18h ; push ebx ; push ebp ; push esi ; push edi` -- ALL FOUR callee-saved regs pushed at entry; single shared
  epilogue at 0x007E2D19 (`pop edi/esi/ebp/ebx ; add esp,18h ; ret 4`). Every early-out (`!WaitingForConfirmation`, `!message`)
  jumps to that one epilogue. This is the old draft's only residue (697 v 707): VC shrink-wrapped the pushes.
- Locals (esp after pushes): [esp+13h] materialised HasTC bool (reused 4x: `shr ; and cl,1 ; mov [esp+13h],cl ; je`),
  [esp+14h] CWideString text, [esp+18h]/[esp+1Ch] key copies for LowerBound + CCharString temp, [esp+20h] message,
  [esp+24h] messages (CMessageEventManager*). When HasTC is false, `stats` (esi) and the scoreboard (ecx) are loaded from
  [esp+24h] -- i.e. the uninitialised pointer shares the `messages` slot (no fallback, same as Show's "dead slot" idiom).
- No size-opt idioms visible (no `optimize("s")` signs); plain /O2-style.

## 2. Identity (two sources)
- vtable: rebuild/manifest/rtti-vtables.tsv `CTCFineDialogue 01270274 slot 26 -> 007e2a60` (xref.py: only ref is .rdata 0x012702DC = slot 26).
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x007DE690) `?GlobalUpdate@CTCFineDialogue@@UAEX_N@Z`; FableWin (_fablewin_api.tsv 0x017950E5) same.
  Ego_r order ctor 7de320 / IsAllowed 7de380 / Show 7de3d0 / GlobalUpdate 7de690 / GetInterfaceType 7de980 == retail order
  ctor 7e2730,7e2760 / IsAllowed 7e2790 / Show 7e27f0 / **7e2a60** / GetInterfaceType 7e2d30. Ego_r size 0x2F0 vs retail 0x2D3.
- Manifest label is CORRECT. Signature: `virtual void CTCFineDialogue::GlobalUpdate(bool)` (public virtual override of
  CTCBase::GlobalUpdate, Ego_r ?GlobalUpdate@CTCBase@@UAEX_N@Z). Mangled leaf `?GlobalUpdate@CTCFineDialogue@@UAEX_N@Z`.

## 3. Callees (retail target | landed file | landed name | PDB name | draft disagreement)
- 0x00686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | CTCBase::GetWorld | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (const, returns CWorld&) | draft: returns CWorld* -> FIX (CTCBase.h line ~160 `CWorld& GetWorld() const;`)
- [world+0x60] = CWorld::GetMessageEventManager() inline (CWorld.h line 254, opt-in FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER; member PMessageEventManager_Data +0x60) | draft: `->PMessages` -> use accessor
- 0x004AB8B0 | 00/4a/CMessageEventManager_FindMostRecentMessage_004ab8b0.cpp | FindMostRecentMessage<CMessageEventFilter_IsOfType> | Ego_r ??$FindMostRecentMessage@VCMessageEventFilter_IsOfType@@@CMessageEventManager@@QBEPBVCMessageEvent@@ABVCMessageEventFilter_IsOfType@@@Z (const member template) | draft: non-template, local filter struct
- 0x0099AED0 CWideString::CWideString() (00/99/CWideString_CWideString_0099aed0.cpp) OK
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (00/40/CVectorMap_LowerBound_0040f020.cpp) -- via inline CThing::GetTC; draft local CInterfaceMap -> use shared CVectorMap/CThing
- 0x007BAE70 | CTCTrespasser_IsCurrentlyTrespassing_007bae70.cpp | ?IsCurrentlyTrespassing@CTCTrespasser@@QAE_NXZ OK
- 0x0099EBF0 CCharString(const char*, long=-1) (CCharString_Constructor_0099ebf0.cpp); 0x0099EAE0 ~CCharString OK
- 0x009C95E0 | NGameTextCDataBank_GetTextBySymbol_009c95e0.cpp | `const CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString&) const` (ret 8, hidden result) | draft: non-const, global CDataBank -> FIX (namespace NGameText, const)
- 0x0099B7D0 `const CWideString& CWideString::operator=(const CWideString&)`; 0x0099B510 ~CWideString OK
- 0x0065A1FD | CTCVillage_IsHeroMurderer_0065a1fd.cpp | ?IsHeroMurderer@CTCVillage@@QAE_NXZ OK
- 0x0057B327 | CTCHeroStats_AddMoney_0057b327.cpp | ?AddMoney@CTCHeroStats@@QAEXJ_N@Z OK (in CTCHeroStats.h under _BOASTS / _ADDMONEY / _PROPERTYOWNED branches)
- 0x0065C2D0 | CTCVillage_ClearCrimes_0065c2d0.cpp | ?ClearCrimes@CTCVillage@@QAEXXZ OK
- 0x00449960 | CPlayerManager_GetMainPlayer_00449960.cpp | ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ (CONST, returns const CPlayer&; non-const is 0x00449970) | draft: `CMainPlayer* GetMainPlayer()` -> WRONG
- 0x00487DC0 | CPlayer_GetPControlledCreature_00487dc0.cpp | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ | draft: `CMainPlayer::GetCharacterThing()` -> WRONG name+class
- 0x0055F3B0 | CTCHeroOnlineScoreboard_UpdateScoreForActivity_0055f3b0.cpp | ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z (first arg enum CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, value 0x24) | draft: `long activity` -> FIX type
- 0x006590BF | CTCVillage_InvalidateAndClearAllSecuritySweepTasks_006590bf.cpp (leaf `_Forward`, a bogus forwarder) | retail bytes `jmp 0x006590A0` (a jump thunk to the body 0x006590A0, itself landed as free __fastcall `CTCVillage_InvalidateAndClearAllSecuritySweepTasks`) | PDB ?InvalidateAndClearAllSecuritySweepTasks@CTCVillage@@AAEXXZ -- PRIVATE member; caller needs friend/access. NAME_FIXES candidates: both landed spellings.
- 0x0057DF85 | NOT LANDED (manifest functions.tsv: KickHeroToMostRecentSafeRegion CTCHeroStats __thiscall void) | Ego_r 0x005711D1 ?KickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ OK
- 0x00434C00 | CDisplayEngine_StartScreenFadeOut_00434c00.cpp | ?StartScreenFadeOut@CDisplayEngine@@QAEXMMVCRGBColour@@M@Z | draft: class `CDisplayManager` -> WRONG class (CDisplayEngine.h line 40 declares it)
- 0x00492E4B | CSubtitleRenderer_SetText_00492e4b.cpp | ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@KM_N2@Z: (font, text, unsigned long flags, float time, bool click_past, bool pause_game) | draft: last param `unsigned long mode` -> WRONG (bool; pushes 0,0 then 1 = time 0.0f, click_past false, pause_game true). Opt-in FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT.
- 0x00493B5B | CMessageEventManager_RemoveMessage_00493b5b.cpp | ?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z OK (opt-in FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE)
- 0x00686DF0 | CTCBase_RemoveFromGlobalTCs_00686df0.cpp (landed as free __fastcall `CTCBase_RemoveFromGlobalTCs(CTCBase*)`) | PDB ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ; CTCBase.h line 107 declares the member. Use the member.
- Globals: 0x013B86A0 = CMainGameComponent::PMainGameComponent (protected static; `CMainGameComponent::Get()`); 0x013B8790 =
  CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass (`CPlayerGui::Get()`, as IsAllowed does) | draft: g_MainGameComponent/g_PlayerGui stand-ins.
- Strings 0x01270360 / 0x0127033C: the two TEXT_GUI_CRIME_KICKED_OUT_* symbols (draft order: TRESPASS first, MURDER second).

## 4. Members touched
- this (CTCFineDialogue, Ego_r = retail, 28 bytes): Thing +4 (CTCBase), Fine +0x0C, GivePlayerChanceToPayFine +0x14 (bool),
  WaitingForConfirmation +0x15 (bool), Village +0x18 (CTCVillage*). No shared CTCFineDialogue.h -- landed siblings declare the class
  locally over the shared engine/CTCBase.h (IsAllowed 0x007E2790 does exactly this, members private).
- CThing (engine/CThing.h): UsingComponents std::bitset<274> +0x20 (TCI 190 -> word +0x34 bit30; 4 -> +0x20 bit4; 224 -> +0x3C bit0;
  269 -> +0x40 bit13); Components CVectorMap +0x44 (retail; Ego_r says +0x48). Opt-ins: FABLE_RETAIL_CTHING (GetTC(id) forceinline),
  FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC<T>(id,&p) written as key-copy/map-pointer/LowerBound/end-or-greater -- matches the
  `lea esi,[x+44h] ... mov esi,[esi+4]` shape here), FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE (HasTC with `const bool& found` -- the
  materialised [esp+13h] byte). DO NOT use FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (that tests in place; retail here materialises).
- TCI names (engine/ETCInterfaceType.h): 0xBE TCI_TRESPASSER, 4 TCI_HERO_STATS, 0xE0 TCI_HERO_REMEMBER_CRIMES (draft's "isHero" is
  wrong: it is a presence test of TCI_HERO_REMEMBER_CRIMES on the same thing, component not fetched), 0x10D TCI_HERO_ONLINE_SCOREBOARD.
- CTCHeroStats (engine/CTCHeroStats.h, retail layout lines 241-243): FinePaid +0x100 (draft "FinesPaid"), TimesThrownOut +0x104
  (draft "TimesKickedOut"). Ego_r tsv shows these 8 bytes later (+0x108/+0x10C) -- the header's retail layout is authoritative.
  Check which CTCHeroStats.h branch exposes FinePaid/TimesThrownOut + AddMoney together (default generated layout vs _BOASTS/_PROPERTYOWNED).
- CMessageEvent (engine/CMessageEvent.h, FABLE_RETAIL_CMESSAGEEVENT): ExtraData +0x3C (draft "Answer"). Message type 0x10 =
  MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO (EMessageEventType.h:21). Filter: engine/CMessageEventFilters.h CMessageEventFilter_IsOfType(EMessageEventType).
- CWorld +0x60 PMessageEventManager_Data (CWorld.h, FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER -> GetMessageEventManager()).
- CMainGameComponent (engine/CMainGameComponent.h; FABLE_RETAIL_CMAINGAMECOMPONENT +): PTextBank +0x14 (_TEXTBANK -> GetTextBank() const),
  PPlayerManager +0x1C (_PLAYERMANAGER -> PeekPlayerManager() const; retail calls the CONST GetMainPlayer 0x449960 so Peek, not
  _GETPLAYERMANAGER's GetPlayerManager which binds 0x449970), PDisplayEngine +0x28 (_DISPLAYENGINE -> GetDisplayEngine()),
  PCutSceneMainFont +0x1614C (_TEXTBANK -> PeekCutSceneMainFont()). Retail reads the global directly each time (`mov eax,[13B86A0h]`),
  not through out-of-line CTCBase::PeekComponent 0x686D60 / PeekPlayerManager 0x686D50.
- CPlayerGui +0x18 subtitle renderer (CPlayerGui.h: GetSubtitleRenderer() inline; FABLE_RETAIL_CPLAYERGUI, IsAllowed's spelling `CPlayerGui::Get().GetSubtitleRenderer()`).
- CRGBColour (engine/CRGBColour.h) built in the arg slot: B,G,R = 0, A = 0xFF (byte stores +2,+1,+0 then +3).
- Locally modelled in the old draft though a shared header exists: CCharString, CWideString, CRGBColour, CDataBank(NGameText_CDataBank.h),
  CPlayerManager, CDisplayManager(->CDisplayEngine.h), CMainGameComponent, CSubtitleRenderer, CPlayerGui, CMessageEvent,
  CMessageEventFilter_IsOfType(CMessageEventFilters.h), CMessageEventManager, CWorld, CThing, CTCBase, CTCHeroStats, CTCVillage, interface map (CVectorMap.h).

## 5. Callee visibility
- FindMostRecentMessage<IsOfType> is a COMDAT out of line at 0x004AB8B0: the template body (CMessageEventManager.h,
  FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT) may get inlined by VC; if so declare it only / keep it out of line (TavernKeeper rows
  0x00968460 / 0x00968510 use the opt-in with IsOfType -- copy them).
- CVectorMap::LowerBound 0x0040F020 out of line (as in every GetTC row). CThing::HasTC / GetTC, CWorld::GetMessageEventManager,
  CMainGameComponent accessors, CPlayerGui::Get / GetSubtitleRenderer, CRGBColour ctor: inline (no calls in retail).
- Same-TU neighbours defined earlier: ctor 0x007E2730/0x007E2760, IsAllowed 0x007E2790, Show 0x007E27F0 (none called here).
- Unaligned call targets (0x0065A1FD, 0x0057B327, 0x0057DF85, 0x00492E4B, 0x00493B5B, 0x006590BF thunk) are other TUs' bodies -- just declarations.

## 6. Sibling landed rows
- rebuild/src/compiled/00/7e/CTCFineDialogue_Show_007e27f0.cpp: FABLE_RETAIL_CTHING/_CCHARSTRING/_CWIDESTRING, CThing.h, CVectorMap.h,
  CCountedPointer.h; `if (Thing->HasTC(TCI_HERO_STATS)) stats = ...&Thing->GetTC(...)` with no else (dead-slot idiom);
  CMainGameComponent::Get()->PDataBank->GetTextBySymbol("SYM") with implicit const char* -> CCharString temp. No pragmas/flags.
- rebuild/src/compiled/00/7e/CTCFineDialogue_IsAllowed_007e2790.cpp: FABLE_RETAIL_CTCBASE, _CTCBASE_COMPONENT, _CGAMECOMPONENT,
  _CMAINGAMECOMPONENT, _CMAINGAMECOMPONENT_PEEKDISPLAYENGINE, _CDISPLAYENGINE_SCREENFADEOUT, _CPLAYERGUI, _CSUBTITLERENDERER(_INCUTSCENE),
  _CWORLD, _CWORLD_QUESTCOMPLETIONUI; local class CTCFineDialogue : public CTCBase with private members. No pragmas.
- rebuild/src/compiled/00/96/CAIStateGroupTavernKeeper_GetPaymentFinishedMessage_00968460.cpp (`// Flags: none`): FindMostRecentMessage + IsOfType idiom.
- rebuild/src/compiled/00/7b/CTCTavernBar_OnInitialActivate_007ba550.cpp: `PeekPlayerManager().GetMainPlayer()` const chain (0x449960).

## 7. Old draft model errors
- Control flow: nested ifs with shared tail are right in substance; residue is ONLY the shrink-wrapped pushes. Pending NOTES tried
  early returns/nesting/hoisting. Idea: retail needs ebx/ebp live across the whole function -- the `message`/`messages` values reloaded
  from stack before RemoveMessage (eax=[esp+20h], esi=[esp+24h]) suggest both are named function-scope locals; correct types
  (GetWorld() returning a reference, const GetMainPlayer chain, enum args, bool pause_game) change register pressure and may fix it.
- Wrong types/names: GetMainPlayer (const CPlayer&), GetPControlledCreature, CDisplayEngine, SetText last arg bool, ExtraData,
  FinePaid/TimesThrownOut, TCI_HERO_REMEMBER_CRIMES not "isHero", GetWorld returns CWorld&, GetTextBySymbol const in NGameText.
- Draft GetInterface() helper returns uninitialised `result` -- matches retail's dead-slot read, keep that semantics but via shared header.
- Draft's HasTC as __forceinline is OK only if the bool still materialises; retail stores it to [esp+13h] each time (`const bool&` form).

## 0x0075B000

# DOSSIER 0x0075B000 CTCPhysicsRigidBody::InitialiseBody (709 bytes)

## 1. Retail shape (xdis 0075b000 0x2d5)
- thiscall, `this` -> ebx; plain `ret` (0x0075B2C4); NO frame pointer (`sub esp,9Ch; push ebx,ebp,esi,edi`) -> default /Oy.
- NO SEH prologue even though a CPhysicsMeshObject local (virtual dtor, destroyed at 0x0075B2B5 `call 0xA127D0`) lives
  across calls. Sibling SetPhysicsMeshBankIndex 0x0075A530 landed with `// Flags: /EHsc` and also has no EH frame; try plain
  /O2 first, /EHsc second.
- Frame slots (esp0 = after the 4 pushes): mesh object esp0+0x3C (0x40 bytes); edge-result temp esp0+0x18 (reused by all
  three Get*Edge calls, then by the operator+ temp); |X| at esp0+0x14, |Y| at esp0+0x10; CoG temp esp0+0x24..0x2C;
  resize value esp0+0x30 (never written: default-constructed, uninitialised C3DVector); matrix temp esp0+0x7C (0x30).
- Constants: 3.2f @0x01269AF8 (TU-local, not the shared pool), 1.0f @0x0122DED8, 3.0f @0x0122DED4 (read from Fable.exe).
- Length idiom x3: `fld Z; fld Y; fld X; fld st0; fmul st1; fld st2; fmul st3; faddp; fld st3; fmul st4; faddp; fsqrt;
  fstp st3; fstp st0; fstp st0` = inline C3DVector::GetMagnitude() on the by-value edge temporary (engine/C3DVector.h:275/277,
  FABLE_VECTOR_INLINE; `sqrt(GetSquaredMagnitude())` default, `GFRoot` under FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF).

### Exact retail sequence (x=|XEdge|, y=|YEdge|, z=|ZEdge|)
1. mesh = GetPhysicsMeshObject(); x = XEdge.mag; y = YEdge.mag; z = ZEdge.mag (z stays on x87).
2. mass = y*z*x*3.2f  (`fld y; fmul st1(z); fmul x; fmul 3.2`)  -> source `b*c*a*3.2f` with a=x,b=y,c=z (draft OK).
3. OneOverMass(+0xA0) = 1.0f/mass (`fld 1.0; fdiv st1`).
4. Stores interleaved: zeros d4,d0,cc,c4,c0,bc,b4,b0,ac then 1.0f to c8,b8,a8,a4 -> EXACTLY
   CMatrix3x4::SetIdentity under FABLE_RETAIL_CMATRIX3X4_SETIDENTITY (`E12=E13=E21=E23=E31=E32=E41=E42=E43=0.0f; E11=E22=E33=1.0f;`
   header says retail stores E43..E12 then E33,E22,E11) on InverseBodyInertiaTensor(+0xA8), then CoefficientOfRestitution(+0xA4)=1.0f
   (stored last, same eax=1.0f register).
5. zz = z*z first (kept on x87); yy = y*y written back into y's slot (`fst [y]`);
   E11(+0xA8) = 3.0f / ((yy + zz) * mass)   [fdivr 3.0]
   xx = x*x written back into x's slot; E22(+0xB8) = 3.0f / ((xx + zz) * mass)
   E33(+0xC8) = 3.0f / ((xx + yy) * mass)    (`fld xx; fadd yy; fmul mass`)
6. mesh.GetRigidBodyBoundingPoints(Body.BodyBoundingVertices @ this+0xE4)  (edi = this+0xE4, pushed early at 0x0075B0BF).
7. CoG temp = mesh.GetCentreOfGravity() (hidden ptr esp0+0x24); Body.CMPosition(+0xD8) = temp (3 dword copies via edx=this+0xD8).
8. Body.CurrentConfig.BoundingVertices(+0x198).resize(Body.BodyBoundingVertices.size(), C3DVector());
   Body.LastConfig.BoundingVertices(+0x24C).resize(same size re-read, same value slot)  -- size() = signed /12 (0x2AAAAAAB).
9. Zeroing (z,y,x order each = C3DVector::Clear, FABLE_RETAIL_C3DVECTOR_CLEAR `X=Y=Z=0.0f`): CMVelocity (+0x3C, emitted via ebx
   before `lea ebp,[ebx+0xF0]`), AngularMomentum (+0x48), AngularVelocity (+0x9C), CMForce (+0x54), Torque (+0x60), config=ebp.
10. config.CMPosition = CoGtemp + Position (Position = CTCPhysicsBase +0x0C). Retail reads the LOCAL CoG temp (esp0+0x24..2C),
    NOT Body.CMPosition -> the CoG result must be a named local. operator+ temp goes through esp0+0x18..0x20 then dword copy.
11. call [vtbl+0x120] (slot 72, `const CRightHandedSet& GetRHSet() const`), result -> edx, ecx=&tmp(esp0+0x7C):
    C3DMeshObject::GetPositionMatrixFromRHSet (static __fastcall, CMatrix3x4 by value); `rep movsd` 12 dwords -> config.Orientation (+0xFC).
12. CalculateVertices(config) (push ebp; ecx=this); Body.LastConfig(+0x1A4).operator=(config); ~CPhysicsMeshObject(mesh).

## 2. Name / signature
- FableWin.pdb: `?InitialiseBody@CTCPhysicsRigidBody@@AAEXXZ` = private: void __thiscall CTCPhysicsRigidBody::InitialiseBody(void)
  (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv + ghidra_out/_fablewin_api.tsv, debug addr 0x0076A150).
- Second source: sole caller CTCPhysicsRigidBody::OnCreate 0x0075B2D0 tail-jumps here (`jmp 0x75b000` @0x0075B34A; xref.py);
  landed row rebuild/src/compiled/00/75/CTCPhysicsRigidBody_OnCreate_0075b2d0.cpp declares `void InitialiseBody(); // 0x0075B000`.
- Ego_r egor_pdb_names has no entry at retail 0x0075B000 (that range is CTCStealth STL in the Ego build: different address map).
- Manifest label CORRECT. Not virtual (no vtable slot). Leaf mangled: `?InitialiseBody@CTCPhysicsRigidBody@@AAEXXZ` (private).

## 3. Callees (landed file / landed name / PDB name / draft spelling)
| addr | landed row | PDB (FableWin) | draft | flag |
|---|---|---|---|---|
| 0x006B0850 | 00/6b/CTCPhysicsBase_GetPhysicsMeshObject_006b0850.cpp | ?GetPhysicsMeshObject@CTCPhysicsBase@@QBE?AVCPhysicsMeshObject@@XZ (const, by value) | non-const member of CTCPhysicsRigidBody | WRONG class/const |
| 0x00A11C20 | 00/a1/CPhysicsMeshObject_GetBoundingBoxXEdge_00a11c20.cpp | ?GetBoundingBoxXEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ | GetXAxis(C3DVector& out) | WRONG name+sig |
| 0x00A11C90 | .../CPhysicsMeshObject_GetBoundingBoxYEdge_00a11c90.cpp | GetBoundingBoxYEdge() const, C3DVector by value | GetYAxis | WRONG |
| 0x00A11BA0 | .../CPhysicsMeshObject_GetBoundingBoxZEdge_00a11ba0.cpp | GetBoundingBoxZEdge() const (landed spelling) | GetZAxis | WRONG |
| 0x00A12840 | .../CPhysicsMeshObject_GetRigidBodyBoundingPoints_00a12840.cpp | ?GetRigidBodyBoundingPoints@CPhysicsMeshObject@@QBEXAAV?$vector@VC3DVector@@...@std@@@Z | arg type CC3DVectorVector (made up) | type should be std::vector<C3DVector>& |
| 0x00A11EC0 | .../CPhysicsMeshObject_GetCentreOfGravity_00a11ec0.cpp | ?GetCentreOfGravity@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ | same | OK |
| 0x0075C040 | 00/75/stdvector_resize_0075c040.cpp | std::vector<C3DVector>::resize(unsigned int, const C3DVector&) (STLport, out of line) | CC3DVectorVector::resize(unsigned long,..) | WRONG: use std::vector<C3DVector> |
| vslot 72 (+0x120) | 00/75/CTCPhysicsRigidBody_GetRHSet_0075bf70.cpp | ?GetRHSet@CTCPhysicsRigidBody@@UBEABVCRightHandedSet@@XZ (const) | non-const | add const |
| 0x00A358F0 | 00/a3/C3DMeshObject_GetPositionMatrixFromRHSet_00a358f0.cpp | ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z | free fn MakeMatrix returning C3DMatrix& | WRONG name, class, return type |
| 0x00759EC0 | 00/75/CTCPhysicsRigidBody_CalculateVertices_00759ec0.cpp | ?CalculateVertices@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z | takes top-level CRigidBodyConfig | WRONG: config is NESTED CRigidBody::CRigidBodyConfig |
| 0x0075BA30 | 00/75/CRigidBodyConfig_operator_0075ba30.cpp | CRigidBody::CRigidBodyConfig::operator= (implicit) | top-level | nest it |
| 0x00A127D0 | 00/a1/CPhysicsMeshObject_CPhysicsMeshObject_00a127d0.cpp | ~CPhysicsMeshObject (virtual in header) | non-virtual | header has virtual dtor |

## 4. Members
- CTCPhysicsBase::Position C3DVector +0x0C (engine/CTCPhysicsBase.h:200, protected) under FABLE_RETAIL_CTCPHYSICSBASE (+FABLE_RETAIL_CTCBASE).
- CTCPhysicsRigidBody::Body (CRigidBody) +0xA0: engine/CTCPhysicsRigidBody.h FABLE_RETAIL_CTCPHYSICSRIGIDBODY keeps it OPAQUE
  (`unsigned char Body[0x1b8]`, protected). No engine/CRigidBody.h exists -> CRigidBody / nested CRigidBodyConfig must be modelled
  locally (as CalculateVertices 0x00759EC0 does) or a header added by the coordinator.
- CRigidBody (struct_layouts_egor.tsv; retail offsets = PDB minus vector shrink): OneOverMass float +0x00, CoefficientOfRestitution float +0x04,
  InverseBodyInertiaTensor CMatrix3x4 +0x08, CMPosition C3DVector +0x38, BodyBoundingVertices vector<C3DVector> +0x44 (12 bytes retail),
  CurrentConfig +0x50, LastConfig +0x104 (config 0xB4 retail, PDB 0xB8).
- CRigidBodyConfig: CMPosition +0x00, Orientation CMatrix3x4 +0x0C, CMVelocity +0x3C, AngularMomentum +0x48, CMForce +0x54, Torque +0x60,
  InverseWorldInertiaTensor CMatrix3x4 +0x6C, AngularVelocity +0x9C, BoundingVertices +0xA8.
- CMatrix3x4 (engine/CMatrix3x4.h): E11..E43, ROWS OF THREE (4x3, size 0x30). Opt-ins: FABLE_RETAIL_CMATRIX3X4_SETIDENTITY (exact
  store order match), FABLE_RETAIL_CMATRIX3X4_DATA_UNION.
- CPhysicsMeshObject (engine/CPhysicsMeshObject.h, size 0x40, virtual dtor): FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES declares only
  GetBoundingBoxXEdge / YEdge. GetBoundingBoxZEdge, GetCentreOfGravity, GetRigidBodyBoundingPoints are NOT declared in the header
  (grep) -> needs a nested opt-in (e.g. extend _EDGES) or coordinator header edit.
- C3DVector (engine/C3DVector.h): GetMagnitude (inline), operator+ (FABLE_VECTOR_INLINE, value ctor), Clear() under FABLE_RETAIL_C3DVECTOR_CLEAR.
- CRightHandedSet: engine/CRightHandedSet.h. C3DMeshObject: engine/C3DMeshObject.h (GetPositionMatrixFromRHSet declared only in the row; grep of rebuild/include found none).
- Draft LOCAL models that have shared headers: C3DVector, CMatrix3x4, CRightHandedSet, CPhysicsMeshObject, CTCPhysicsRigidBody, std::vector (stlport/fable_containers.h);
  C3DMatrix and CC3DVectorVector are invented (replace by CMatrix3x4 / std::vector<C3DVector>).

## 5. Callee visibility
- resize 0x0075C040: STLport resize called OUT OF LINE -> FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (stlport/fable_containers.h:553,
  `__declspec(noinline)` visible body). size() inline (fable_containers.h:460).
- Inline (no call in retail): C3DVector::GetMagnitude, operator+, Clear, CMatrix3x4::SetIdentity, vector::size.
- Out of line, other TUs: GetPhysicsMeshObject (CTCPhysicsBase TU), all CPhysicsMeshObject methods (0xA11..), GetPositionMatrixFromRHSet.
- Same TU, earlier addresses: CalculateVertices 0x00759EC0 (called, not inlined), GetRHSet 0x0075BF70 (virtual call), CRigidBodyConfig::operator=
  0x0075BA30 (implicit, emitted later as COMDAT; called).

## 6. Siblings
- 00/75/CTCPhysicsRigidBody_OnCreate_0075b2d0.cpp: local class model + FABLE_RETAIL_CTHING; no flags.
- 00/75/CTCPhysicsRigidBody_CalculateVertices_00759ec0.cpp: local CRigidBody with nested CRigidBodyConfig, CMatrix3x4 as 4 C3DVector rows,
  STLport vector stand-in; no flags. Copy its CRigidBody model.
- 00/75/CTCPhysicsRigidBody_SetPhysicsMeshBankIndex_0075a530.cpp: `// Flags: /EHsc` (RELOCATION_MATCH), calls 0x00A358F0 too (fastcall).
- 00/75/CTCPhysicsRigidBody_GetRHSet_0075bf70.cpp etc.: FABLE_RETAIL_CTCBASE + FABLE_RETAIL_CTCPHYSICSBASE + FABLE_RETAIL_CTCPHYSICSRIGIDBODY.
- 00/75/CRigidBodyConfig_operator_0075ba30.cpp: implicit memberwise operator= model.

## 7. Draft model errors
1. CMatrix3x4 modelled as float M[3][4]: diagonal M[1][1]/M[2][2] land at +0x14/+0x28 -> wrong (+0xBC/+0xD0). Retail diagonal is E11/E22/E33 at +0x00/+0x10/+0x20 (+0xA8/+0xB8/+0xC8).
2. Inertia diagonal formulas wrong: draft E11 = 3/((xx+zz)m), E22 = 3/((yy+zz)m), E33 = 3/((yy+xx)m). Retail: E11 = 3/((yy+zz)m),
   E22 = 3/((xx+zz)m), E33 = 3/((xx+yy)m) (zz computed first, yy then xx stored back over y/x slots).
3. config.CMPosition uses Body.CMPosition; retail uses the local CoG temporary -> `C3DVector centre = mesh.GetCentreOfGravity();`.
4. Zeroing order/form: retail CMVelocity, AngularMomentum, AngularVelocity, CMForce, Torque, each z,y,x (Clear()); draft assigns
   C3DVector(0,0,0) temporaries starting with AngularVelocity.
5. Identity: hand-written SetIdentity in wrong layout; use header SetIdentity (opt-in) then CoefficientOfRestitution = 1.0f.
6. Callee spellings/types (section 3): GetXAxis->GetBoundingBoxXEdge() by value, MakeMatrix->C3DMeshObject::GetPositionMatrixFromRHSet,
   CRigidBodyConfig must be nested in CRigidBody, CC3DVectorVector -> std::vector<C3DVector>, GetRHSet/GetPhysicsMeshObject const.
7. Orientation assignment: `config.Orientation = C3DMeshObject::GetPositionMatrixFromRHSet(GetRHSet());` (rep movsd of the 0x30 temp).

## 0x008548B0

# DOSSIER 0x008548B0 CCreatureAction_SpiritAttack::CreateAttackEffect (723 bytes)

## 1. Retail shape (xdis 008548b0 0x2e3)
- __thiscall, no params, plain `ret` (0x00854B82), int3 pad to 0x00854B90 (= landed FrameUpdate).
- `sub esp,70h; push ebx/ebp/esi/edi` -- NO frame pointer (no optimize("y",off)), NO SEH prologue (no /EHsc despite
  CCharString / CDefPointer / CIntelligentPointer / CParticleUpdateParameters locals). Speed-optimised (no `s` idioms).
- Registers: esi=this (later esi=this+0xB0 = &PParticleEmitter, then esi=emitter TC), ebx=target thing, edi=index then
  ability def, ebp=0 (zero for both CDefPointer inits AND the spirit pointer default), [esp+2Ch]=&PCreature cached.
- Flow: target=PTarget.GetPItem(); {CCharString("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF" 0x01235B14) ;
  CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name,1); ~CCharString}; CDefPointer def (esp+14) zeroed
  AFTER the GetDef arg pushes; Get().GetDef(index,&def); CDefPointer<const CSpecialEffectsDef> effects (esp+18) zeroed;
  if (def->GetSubDef(&effects)) { spirit GetTC 0xC7 (byte test [+38h] bit7, jns; if absent spirit stays ebp=0);
  effects->GetEffectFromString(spirit->GetAttackEffectName() /*temp*/, effectIndex /*esp+28*/); ~temp;
  CThing* e = CTCDParticleEmitter::Create(effectIndex, target->GetPos(), false) [fastcall ecx,edx,push 0];
  PParticleEmitter = CIntelligentPointer<CThing>(e) [temp: base ctor 0xA01B10, vptr 0x01238C6C, SetPItem; then
  GetPItem(temp) -> SetPItem(this+0xB0); ~ 0xA01C10];
  if (PParticleEmitter && !PParticleEmitter->Dead [+0x91 bit0]) { GetTC 0x4B ([+28h] test ch,8); if (emitter) {
   emitter->AttachToThing(*PCreature, 0xC, MAIN_FOCAL_POSITION_NAME, 0, 0.0f);
   start=spirit->GetFocalPos() (NO +0.8 Z here, unlike FrameUpdate); end=target->PTCGraphicAppearance->GetFocalPos(1.0f);
   pair start(CCRC::Calc(0,"LIGHTNING_START",15),start); pair end(CCRC::Calc(0,"LIGHTNING_END",13),end);
   CParticleUpdateParameters<ulong,ulong> p  -> ctor CALLED out of line 0x006E24D0 (FrameUpdate inlines it!);
   push_back(start) INLINED (finish!=eos ? copy 16 bytes ; else _M_insert_overflow 0x0076ADA0);
   Dirty=1 (one store, [p+0x11]); push_back(end) CALLED 0x0076B120 (no Dirty store after it);
   p.Update() CALLED 0x0076C750; emitter->SetEmitterUpdateParameters(p) 0x006E11A0; ~p CALLED 0x006E2500 }}}
  epilogue: release effects (esp+18) first, then def (edi; reloaded from [esp+14] only on the if-path join 0x854B55,
  kept in edi across GetSubDef on the false path) -- `dec [x+4]; jne; call [vtbl+4]`.
- Only one Dirty store before the out-of-line push_back => source is two Inserts (push_back;Dirty=true) where VC7.1
  merged the stores, or one Insert-equivalent; the old draft's "inline push_back, Dirty, call push_back" reproduces it.

## 2. Name (proven, 2 sources) -- manifest label is RIGHT
- Ego_r.pdb (egor_pdb_names.tsv 0x00843C50) and FableWin.pdb (_fablewin_api.tsv 0x0180993B):
  `?CreateAttackEffect@CCreatureAction_SpiritAttack@@AAEXXZ` -> PRIVATE, non-virtual, non-const `void CreateAttackEffect()`.
- Ego_r order Initialise/ctor/FinishAttackEffect/FinishAction/CreateAttackEffect/FrameUpdate/PerformAction matches retail
  neighbourhood 0x8545F0 .. 0x854DD0. Only caller: 0x00854EF8 inside PerformAction 0x00854DD0 (vtable 0x01275D74 slot 3, not landed).
- Not in vtable (non-virtual).

## 3. Callees (landed file under rebuild/src/compiled/; PDB name; DRAFT spelling to fix)
| addr | PDB / landed name | draft spelling |
|---|---|---|
| 00A01B50 | CBaseIntelligentPointer::GetPItem() const (00/a0/..._00a01b50) | ok |
| 0099EBF0 / 0099EAE0 | CCharString(const char*,long=-1) / ~CCharString (00/99) | ok |
| 0044C6B0 | `static CGameDefinitionManager& __fastcall CGameDefinitionManager::Get()` ?Get@CGameDefinitionManager@@SIAAV1@XZ (00/44) | WRONG: CDefinitionManager::Get() returning pointer |
| 009AD390 | `long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const` (both PDBs; landed file mislabelled COptimisedPrimitive_ActualToUniqueIndex_009ad390.cpp) | WRONG: GetDefIndexFromName(name,bool) |
| 007A35D0 | CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) const (00/7a/CDefinitionManager_GetDef007a35d0) -- T = CSpecialAbilitiesLightningSpellDef (landed 0x008BC790 uses same addr+same string) | WRONG type CSpecialAbilityDef |
| 00669580 | `CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>*) const` (both PDBs; 00/66) | WRONG: CSpecialAbilityDef::GetSubDef, non-const T |
| 0040F020 | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (00/40) | ok (via GetTC) |
| 00670010 | `CCharString CTCCoopSpirit::GetAttackEffectName() const` (both PDBs; 00/67) | WRONG: GetActionUseAnimationGroupName, non-const |
| 00669D30 | `bool CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) const` (both PDBs; 00/66) | WRONG: LookupEffect(name,long*) void |
| 004C73D0 | `const C3DVector& CThing::GetPos() const` (00/4c/CThing_GetPosition_004c73d0) | WRONG: GetPosition returning ptr |
| 006E0880 | `static CThing* __fastcall CTCDParticleEmitter::Create(long, const C3DVector&, bool)` (Ego_r SIPAVCThing@@JABVC3DVector@@_N; engine/CTCDParticleEmitter.h FULL) NOT landed | WRONG: free CreateEffectThing(long,const C3DVector*,bool) |
| 00A01B10 / 00A01B90 / 00A01C10 | CBaseIntelligentPointer ctor / SetPItem / ~ (vptr 0x01238C6C = CIntelligentPointer<CThing>) | ok |
| 006E0BE0 | `void CTCDParticleEmitter::AttachToThing(CThing&, long, const CCharString&, long, float)` (00/6e) | ok (draft passes CThing* -> must be reference) |
| 0066FFD0 | `C3DVector CTCCoopSpirit::GetFocalPos() const` (00/66) | draft non-const |
| 004C1060 | `C3DVector NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) const` (00/4c) | WRONG: GetTargetPoint |
| 004014A0 | `static unsigned long __fastcall CCRC::Calc(unsigned long,const void*,unsigned long)` (engine/CCRC.h) | WRONG: Calc_004014a0 |
| 006E24D0 / 006E2500 | NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> ctor / virtual dtor | ok (namespace missing) |
| 0076ADA0 / 0076B120 | std::vector<std::pair<unsigned long,C3DVector> >::_M_insert_overflow / push_back | draft uses SVectorParam/CVectorParamVector stand-ins |
| 0076C750 | ?Update@?$CParticleUpdateParameters@KK@NParticleEngine@@QAEXXZ (called, not inlined) | ok |
| 006E11A0 | CTCDParticleEmitter::SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<ulong,ulong>&) | ok |
Global 0x013BAC48 = `extern const CCharString MAIN_FOCAL_POSITION_NAME;` ("focus_eye_01"; Ego_r+FableWin dynamic-initializer names,
ghidra_out/labels_landing_2026-11-12dj.tsv; landed spelling in CThingCreatureDef_Validate_00670d50.cpp). Draft `g_SpiritEmitterHelper` WRONG.
Strings: 0x01235B14 "SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF", 0x0126C810 "LIGHTNING_START"(15), 0x0126C800 "LIGHTNING_END"(13).

## 4. Members
- this: PCreature +0x08 (CCreatureActionBase, protected CIntelligentPointer<CThingCreatureBase>; engine/CCreatureActionBase.h
  FABLE_RETAIL_CCREATUREACTIONBASE), PTarget +0xA8, PParticleEmitter +0xB0 (Ego_r names, CIntelligentPointer<CThing>;
  engine/CCreatureAction_SpiritAttack.h is flattened/generated, sizeof retail 0xB8). Draft name `PEffect` WRONG -> PParticleEmitter.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING [+ _GETTC_EXPANDED]): UsingComponents bitset<274> +0x20 (bytes +0x28 / +0x38 tested),
  Components CVectorMap +0x44, PTCGraphicAppearance (NGraphicAppearance::CTCGraphicAppearance*) +0x64, Dead uchar:1 +0x91 bit0,
  IsDead() inline. GetTC<T>(T**) forceinline via T::StatGetInterfaceType (CTCCoopSpirit 0xC7, CTCDParticleEmitter 0x4B).
- CDefPointer RefCount +4, Destroy = vtbl slot 1 (engine/CDefPointer.h).
- CParticleUpdateParameters: vptr+0, VectorParams +0x04 (vector 12 bytes, Dirty +0x11), UpdateParams +0x14; 0x24 bytes
  (engine/NParticleEngine_CParticleUpdateParameters.h needs FABLE_RETAIL_CVECTORMAP_INSERT + FABLE_RETAIL_STLP_SORT_FASTCALL;
  its ctor/Update are inline there -- retail CALLS both here, so declare them out of line or rely on budget).
- Draft LOCAL models that have shared headers: C3DVector (engine/C3DVector.h), CCharString (fable_string.h FABLE_RETAIL_CCHARSTRING),
  CThing (engine/CThing.h), CBaseIntelligentPointer/CIntelligentPointer (engine/CIntelligentPointer.h FABLE_RETAIL_CINTELLIGENTPOINTER
  [+_ASSIGN, _FORCEINLINE_CTOR]), CDefPointer (engine/CDefPointer.h), CDefinitionManager (engine/CDefinitionManager.h),
  CSpecialEffectsDef (engine/CSpecialEffectsDef.h), CTCDParticleEmitter (engine/CTCDParticleEmitter.h FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL),
  CParticleUpdateParameters (engine/NParticleEngine_CParticleUpdateParameters.h), CVectorMap (engine/CVectorMap.h),
  CTCGraphicAppearance (engine/NGraphicAppearance_CTCGraphicAppearance.h, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL),
  CCRC (engine/CCRC.h), CDefClassBase::GetSubDef (engine/CDefClassBase_GetSubDef.h), CCreatureActionBase, CTCCoopSpirit (_quarantine only).

## 5. Callee-body visibility
- push_back 0x0076B120: inlined at site 1, called at site 2 => the STLport push_back body WAS visible (plain inline, budget
  split) -- as CVectorMap.h INSERT_KEYVALUE notes for 0x005DD670 (expanded at first 17 sites, called at rest). Landed shot rows
  (0x007C25A0, 0x007BE3E0) instead define push_back under `#pragma auto_inline(off)` in-TU to fix store order.
  Opt-ins: FABLE_RETAIL_CVECTORMAP_INSERT (+_INSERT_FORCEINLINE, _INSERT_KEYVALUE), FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE /
  _OUTOFLINE (stlport/fable_containers.h:479-497).
- GetDef 0x007A35D0: landed 0x008BC790 (same def, same string) needed a same-TU stand-in body under auto_inline(off) so the def
  stays in a register for the final release -- retail here likewise keeps def in edi across GetSubDef. Copy that idiom.
- GetSubDef<CSpecialEffectsDef> 0x00669580: FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (visible-but-noinline body => caller keeps
  the def in a register after the call).
- CParticleUpdateParameters ctor 0x006E24D0 / Update 0x0076C750 called out of line (in FrameUpdate the ctor is inline). IS_THING_ALIVE:
  engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (inline here: 3 GetPItem calls). CIntelligentPointer(T*) temp: expanded inline
  (base ctor + vptr + SetPItem) => FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR + _ASSIGN.

## 6. Siblings
- 00/85/CCreatureActionSpiritAttack_FrameUpdate_00854b90.cpp (same class, next address, RELOCATION_MATCH, no pragmas): same
  LIGHTNING_START/END body; idioms `const CTCCoopSpirit* tc_spirit=0; PCreature->GetTC(&tc_spirit);`, std::pair locals,
  update_params.VectorParams.Insert(...) x2, CThing::GetTC local model, PTarget/PParticleEmitter members.
- 00/85/CCreatureActionSpiritAttack_FinishAction_008547d0.cpp: IS_THING_ALIVE(PParticleEmitter); layout notes (+0xA8/+0xB0).
- 00/8b/CCreatureActionCastWillLightningSpell_..._008bc790.cpp: lightning def lookup in scoped block + same-TU GetDef stand-in.
- 00/7b/NThingShotCTCShotDefinitionForcePushBall_InitAttachedEffects_007be3e0.cpp: Create(…)+AttachToThing+push_back 0x0076B120
  (uses optimize y-off/s-on -- NOT applicable here); 00/8e/..GuildmasterPowerUpShield_FrameUpdate_008e20f0.cpp: full opt-in list on
  shared headers (FABLE_RETAIL_CTHING_GETTC_EXPANDED, _CTCDPARTICLEEMITTER_FULL, _CVECTORMAP_INSERT, _CVECTORMAP_CTOR, _STLP_SORT_FASTCALL ...).

## 7. Old draft model errors (DIFFER 748v723; hand283 STATUS: register roles this esi v edi, release/store scheduling)
- Nearly every callee spelled wrong (table above); members PEffect->PParticleEmitter; global name wrong; def type wrong.
- `spirit` uninitialised -- retail default 0 (ebp); write `const CTCCoopSpirit* tc_spirit = 0;` like FrameUpdate.
- GetPos returns const ref, AttachToThing takes CThing& (*PCreature), Create is static fastcall in CTCDParticleEmitter.
- GetDef/GetSubDef bodies invisible in draft -> def reloaded / registers shift (the reported this/def register-role diff).
- Local GetDefIndex helper + extra CDefinitionManager::Get pointer form; retail scope: string temp destroyed before GetDef call.
- Draft calls PCreature through CBaseIntelligentPointer w/o template; retail caches &PCreature in a stack slot ([esp+2Ch]).

## 0x00958840

# DOSSIER 0x00958840 CAIStateGroup_MinionReactToCrime::Update (724 bytes)

## 1. Retail shape (xdis 00958840 0x2e4)
- thiscall virtual, `ret` (no args), `sub esp,20h ; push ebx,ebp,esi,edi` -- NO frame pointer (no /Oy-), NO SEH prologue
  (no EH frame although CCharString temp + CDefPointer local have dtors => TU built without /EHsc, as the siblings).
- Locals: [esp+13h] bool presence byte (reused by each GetTC), [esp+14h] CDefPointer<const CMessageEventDef> (zeroed at 0x958935),
  [esp+18h] CCharString temp, [esp+1Ch] IP temp / LowerBound key / perceive ptr (ebp spilled there), [esp+24h..2Ch] C3DVector position.
- Single exit: every early-out jumps to 0x958B05 `Terminate()`; the def release (0x958AF3) is only on the inner path => def is scoped
  inside the `if (perceived)` block.
- Loop: 0x958A86 head `do { r = AIAction_FindAGuardAndYellForHelp(pos, OpinionDeedHandle, true); } while (r == 4);` (`cmp ebp,4 ; je head`).

## 2. Identity (proven, label CORRECT)
- Name: `?Update@CAIStateGroup_MinionReactToCrime@@UAEXXZ` = `void CAIStateGroup_MinionReactToCrime::Update()` (public virtual).
- Source 1: rebuild/manifest/rtti-vtables.tsv: vtable 0x0128C558 slot 12 = 0x00958840 (xref.py: .rdata 0x0128C588 = 0x0128C558+0x30,
  and 0x0128C5C4 = FromSleep vtable 0x0128C594+0x30); landed OnActivate's base model names slot 12 (+0x30) `Update`.
- Source 2: ghidra_out/egor_pdb_names.tsv Ego_r order InitStateGroup 0x0097D360, GetSomeoneElseGettingHelpMessage 0x0097D370,
  OnActivate 0x0097D3D0, Update 0x0097D450 == retail order 0x00958760 / 0x00958770 / 0x009587D0 / 0x00958840 (all neighbours landed).

## 3. Callees (landed file | landed name | PDB name | draft disagreement)
- 0x00A01B10 CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp ??0CBaseIntelligentPointer@@QAE@XZ (ctor of IP temp; vptr 0x01244344 = CIntelligentPointer vtable per rtti-vtables)
- 0x00A01B50 ..._GetPItem_00a01b50.cpp ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*, draft CThing*)
- 0x00A01B90 ..._SetPItem_00a01b90.cpp ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z
- 0x00A01C10 ..._CBaseIntelligentPointer_00a01c10.cpp ??1CBaseIntelligentPointer@@UAE@XZ
- 0x00958770 cleanup1_GetSomeoneElseGettingHelpMessage_00958770.cpp ?GetSomeoneElseGettingHelpMessage@CAIStateGroup_MinionReactToCrime@@AAEPBVCMessageEvent@@J@Z (private, returns const CMessageEvent*; draft non-const)
- 0x004365B0 CVectorMap_LowerBound_004365b0.cpp (const LowerBound, used by the 0x34 PerceiveThing lookup => PeekTC on const)
- 0x0040F020 CVectorMap_LowerBound_0040f020.cpp (non-const; 0x4F twice, 0xBC)
- 0x007446A0 CTCPerceiveThing_GetPerceivedThing_007446a0.cpp ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (const, returns const CThing*; DRAFT non-const)
- 0x0099EBF0 CCharString_Constructor_0099ebf0.cpp CCharString(const char*, long) (-1); 0x0099EAE0 ~CCharString
- 0x008FCFB0 CAIStateGroupBase_PeekDefinitionManager_008fcfb0.cpp ?PeekDefinitionManager@CAIStateGroupBase@@IBEABVCGameDefinitionManager@@XZ -- DRAFT "GetPDefinitionManager" WRONG
- 0x007923C0 CDefinitionManager_GetDef007923c0_007923c0.cpp GetDef<CMessageEventDef>(const CCharString&, CDefPointer&) (landed name has the "GetDef007923c0" artefact)
- 0x008FCFC0 CAIStateGroupBase_GetWorld_008fcfc0.cpp ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ -- DRAFT "GetPWorld" WRONG (egor_pdb_names 0x008fcfc0 ScorpionSummon hit is an Ego_r-address coincidence, ignore)
- 0x004C73D0 CThing_GetPosition_004c73d0.cpp ?GetPos@CThing@@QBEABVC3DVector@@XZ -- DRAFT "GetPosition" WRONG
- 0x00687730 CMessageEventManager_AddMessage_00687730.cpp ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z
  = AddMessage(const CMessageEventDef&, const C3DVector&, CThing*, CThing*, long) -- DRAFT used pointers for def/position
  (same bytes, wrong spelling); manager reached as world+0x60 (CWorld PMessageEventManager_Data, FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER -> GetMessageEventManager()).
- 0x008FCF90 CAIStateGroupBase_GetPlayerManager_008fcf90.cpp (anonymous stub body) PDB ?GetPlayerManager@CAIStateGroupBase@@IBEAAVCPlayerManager@@XZ
  (Ego_r 0x00847DE0, same order) -- DRAFT "GetPPlayerManager" WRONG
- 0x00449970 CPlayerManager_GetMainPlayer_00449970.cpp ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (returns CPlayer&; draft pointer)
- 0x00487DC0 CPlayer_GetPControlledCreature_00487dc0.cpp ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ -- DRAFT "GetPCharacter" WRONG
- 0x006D4DB0 CTCFollowed_IsAIFollower_006d4db0.cpp ?IsAIFollower@CTCFollowed@@QBE_NPBVCThingAICreature@@@Z (const CThingAICreature*)
- 0x006D56E0 CTCFollowed_QuitAIFollower_006d56e0.cpp ?QuitAIFollower@CTCFollowed@@QAEXAAVCThingAICreature@@@Z (CThingAICreature&; draft pointer)
- 0x0097FEA0 NOT landed. manifest functions.tsv: AIAction_FindAGuardAndYellForHelp CAIStateGroupBase thiscall EAIActionReturn.
  PDB: ?AIAction_FindAGuardAndYellForHelp@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@VC3DVector@@K_N@Z
  = protected `EAIActionReturn AIAction_FindAGuardAndYellForHelp(C3DVector, unsigned long, bool)` -- DRAFT: member of the derived
  stand-in, long return, `long` 2nd arg -- WRONG. No opt-in declares it in engine/CAIStateGroupBase.h yet (FINDAGUARD macro has only
  FleeToThingProximity / FindAGuardAndGiveMessage) -> a nested opt-in must be added.
- 0x008FCF40 CAIStateGroupBase_Terminate_008fcf40.cpp ?Terminate@CAIStateGroupBase@@QAEXXZ
- vcall [vtbl+4] at 0x958B02: the inlined CDefPointer<const CMessageEventDef> dtor (refcount at +4, slot 1 delete) -- engine/CDefPointer.h ~CDefPointer(){Release();}.
- Data: "STANDARD_MESSAGE_EVENT_GOING_FOR_HELP" @ 0x01294E74.

## 4. Members
- this (engine/CAIStateGroup_MinionReactToCrime.h, FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIME; pulls FABLE_RETAIL_CAISTATEGROUPBASE(_MEMBERS),
  needs FABLE_RETAIL_CINTELLIGENTPOINTER): PBrain +0x04 (CAIStateGroupBase), OpinionDeedHandle +0x14 long, GuardTold +0x18 bool,
  AllowDuplicateReactToCrime +0x19 bool, ForceOpenBuilding +0x1B CIntelligentPointer<CThingBuilding> (byte-packed). Update() already declared.
- PBrain->+0x18 Creature: engine/CAIBrain.h; GetCreature() via FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE
  (+ _CAIBRAIN_CREATURE_REFERENCE as OnReplace). Retail reloads [esi+4]->[+18h] each use => inline GetCreature() per use.
- creature +0x168: CThingAICreature::HomeBuilding (CIntelligentPointer<CThingBuilding>; struct_layouts Ego_r +400) and FableWin
  ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ inline: engine/CThingAICreature.h FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
  (body "in the includer once CThingBuilding is complete": `return (CThingBuilding*)HomeBuilding.GetPItem();`-style). Draft modelled it as CThingCreatureBase::PHome -- wrong class/name.
- CThing: UsingComponents +0x20 bitset, Components +0x44, Dead bit0 +0x91 -> engine/CThing.h FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED
  (GetTC(&p) non-const / PeekTC(&p) const). IS_THING_ALIVE via engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE.
- CThingBuilding ForceOpenCount retail +0xD0 (Ego_r +0xE8): engine/CThingBuilding.h FABLE_RETAIL_CTHINGBUILDING_FORCEOPENCOUNT offers only
  DecrementForceOpenCount. PDB (FableWin.exe.signatures) has ?IncrementForceOpenCount@CThingBuilding@@QAEXXZ (inline) -> add it beside Decrement.
- CTCCreatureOpinionOfHero InFindAGuardAndYellForHelp: Ego_r +0x64F (struct_layouts), retail +0x343. NOT in engine/CTCCreatureOpinionOfHero.h.
  FableWin has inline ?SetInFindAGuardAndYellForHelp@CTCCreatureOpinionOfHero@@QAEX_N@Z and ?IsInFindAGuardAndYellForHelp@..@@QBE_NXZ -> needs a new opt-in
  (member at retail +0x343 + Set accessor). Interface type 0x4F (presence bit 15 of dword +0x28).
- CTCPerceiveThing (0x34, bit 20 of +0x24): engine/CTCPerceiveThing.h FABLE_RETAIL_CTCPERCEIVETHING (check it declares GetPerceivedThing() const).
- CTCFollowed (0xBC, bit 28 of +0x34): engine/CTCFollowed.h has NO FABLE_RETAIL_* opt-ins -> declare IsAIFollower/QuitAIFollower via a new opt-in.
- CWorld +0x60 message-event manager: engine/CWorld.h FABLE_RETAIL_CWORLD + FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER (OnReplace uses GetWorld().GetMessageEventManager()).
- CPlayerManager / CPlayer: engine/CPlayerManager.h (GetMainPlayer), engine/CPlayer.h (FABLE_RETAIL_PLAYER...) -- see 0x00931010 sibling.
- Locally modelled classes in the draft that HAVE shared headers: CThing, CThingBuilding, CThingCreatureBase(AICreature), CBaseIntelligentPointer /
  CIntelligentPointer, CInterfaceMap (=engine/CVectorMap.h), C3DVector, CCharString, CDefPointer, CMessageEventDef, CDefinitionManager,
  CMessageEventManager, CWorld, CPlayer, CPlayerManager, CAIBrain, CTCPerceiveThing, CTCFollowed, CTCCreatureOpinionOfHero; also the
  CAIStateGroupMinionReactToCrimeMethods subclass hack (use the real class via the header's retail branch).

## 5. Callee visibility
- Inline in retail (no out-of-line call): GetCreature, GetHomeBuilding, IS_THING_ALIVE, IncrementForceOpenCount (`inc [eax+0D0h]`),
  GetTC/PeekTC expansions, CWorld::GetMessageEventManager (+0x60 load), SetInFindAGuardAndYellForHelp, CDefPointer dtor, C3DVector copy ctor.
- Out of line: all IP methods, both LowerBounds, CCharString ctor/dtor, GetDef 0x007923C0, AddMessage, GetPos, the CAIStateGroupBase
  getters, GetMainPlayer, GetPControlledCreature, CTCFollowed members, AIAction_FindAGuardAndYellForHelp, Terminate.
- Same-TU neighbours (earlier addresses, landed): InitStateGroup 0x00958760, GetSomeoneElseGettingHelpMessage 0x00958770 (its TU
  inlines FindMostRecentMessage template), OnActivate 0x009587D0. GetDef<CMessageEventDef> 0x007923C0 is a template instance from another
  TU (not visible). The old draft's same-TU stand-in bodies for LowerBound and GetDef were LTCG-guesses; the landed OnReplace uses plain
  declarations + header macros and matched -- prefer that.

## 6. Siblings (copy idioms)
- rebuild/src/compiled/00/95/CAIStateGroupMinionReactToCrime_OnReplace_00958b20.cpp (BEST: same class, pure shared headers; macro set:
  BASE_OBJECT, CCLASS, DEF_CLASS_BASE, CTHING, CTHING_STATTYPEFLAGS, CTHING_GETTC_EXPANDED, CTHINGPHYSICAL, CTHINGGAMEOBJECT,
  CTHINGCREATUREBASE, CTHINGAICREATURE, CTHINGBUILDING_FORCEOPENCOUNT, CINTELLIGENTPOINTER, CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN,
  CINTELLIGENTPOINTER_COMPARE_NULL_INT, IS_THING_ALIVE, CCHARSTRING, CMESSAGEEVENT(_ORIGINATOR_ORIGINAL), CMESSAGEEVENTMANAGER_REMOVE,
  CAISTATEGROUPBASE(_MEMBERS,_GETCREATURE,_GOTOBED_WORLD), CAIBRAIN_GETCREATURE, CAIBRAIN_CREATURE_REFERENCE, CWORLD(_MESSAGEEVENTMANAGER),
  CTCBASE, CTCCREATUREOPINIONOFHERO(_GREET,_SETDEEDMASK), CAISTATEGROUP_MINIONREACTTOCRIME). No pragmas, no Flags line.
  Its idioms: `if (IS_THING_ALIVE(ForceOpenBuilding)) ForceOpenBuilding->DecrementForceOpenCount();` (three GetPItem calls = exactly retail
  0x958894..0x9588B6 here, with Increment), `CTCCreatureOpinionOfHero* p; if (GetCreature().GetTC(&p)) ...`.
- .../00/95/CAIStateGroupMinionReactToCrime_OnActivate_009587d0.cpp (GetTC 0x4F + CCharString named local scope).
- .../00/95/cleanup1_GetSomeoneElseGettingHelpMessage_00958770.cpp.
- .../00/93/CAIStateGroupMinionReactToCriminal_IsRunnable_00931010.cpp (GetPlayerManager().GetMainPlayer().GetPControlledCreature() via engine/CPlayerManager.h).
- .../00/93/CAIStateGroupPerformAction_Update_009352c0.cpp (GetPerceivedThing const, do-while AIAction loop `== 4`).
- .../00/84/CCreatureActionCallSpouseHome_PerformAction_00849140.cpp (GetDef<CMessageEventDef> + AddMessage pattern, local model).

## 7. Old draft model errors vs disassembly
- (Main residue) First 0x4F block: retail `je 0x9589FE` SKIPS the store on a miss (presence byte [esp+13h]); the draft's GetTC<T>(type)
  returns an uninitialised pointer and stores unconditionally. Write it like the second block:
  `CTCCreatureOpinionOfHero* p; if (GetCreature().GetTC(&p)) p->SetInFindAGuardAndYellForHelp(true);`
  NB the two 0x4F blocks test presence differently: first `shr ecx,0Fh ; and cl,1 ; mov [esp+13h],cl ; je` (bool homed, like 0x34/0xBC),
  second (after loop) `test ch,ch ; jns` -- the second is likely `GetTC` whose bool result is consumed directly while the first's
  presence byte is shared/homed; compare to the HasTC `const bool& found` spelling in CThing.h (FABLE_RETAIL_CTHING_HASTC_FORCEINLINE variant).
- ForceOpenBuilding assignment: retail builds a CIntelligentPointer<CThingBuilding> TEMP (ctor 0xA01B10, vptr store, SetPItem(item)), then
  GetPItem(temp)/SetPItem(dst), dtor => `ForceOpenBuilding = GetCreature().GetHomeBuilding();` (T* -> temp IP via converting ctor +
  copy operator=). Needs the CIntelligentPointer(T*) ctor inline (FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR?) and ASSIGN form
  `item = other.GetPItem(); SetPItem(item)` -- check which header macro yields `call GetPItem ; push eax ; call SetPItem`.
- Then IS_THING_ALIVE(ForceOpenBuilding) + ForceOpenBuilding->IncrementForceOpenCount() (three GetPItem calls).
- Perceive lookup is CONST (0x004365B0): `const CTCPerceiveThing* pperceive; if (GetCreature().PeekTC(&pperceive) && pperceive->GetPerceivedThing())`.
- Def lookup: `CDefPointer<const CMessageEventDef> def;` then `if (PeekDefinitionManager().GetDef(CCharString("STANDARD_MESSAGE_EVENT_GOING_FOR_HELP"), def))`
  -- temp name destroyed before the bl test (retail `mov bl,al ; call ~CCharString ; test bl,bl`): a temporary in the condition, not the draft's named scoped local + bool.
- AddMessage: `GetWorld().GetMessageEventManager().AddMessage(*def, GetCreature().GetPos(), &GetCreature(), 0, OpinionDeedHandle)`
  (retail: edi=creature loaded BEFORE GetWorld call; args pushed handle,0,creature,&pos,def).
- `C3DVector pos = pperceive->GetPerceivedThing()->GetPos();` copied to frame [esp+24h] BEFORE the first 0x4F block.
- hero: `CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature(); if (hero && !hero->IsDead() && pperceive->GetPerceivedThing() == hero)`
  then `CTCFollowed* f; if (hero->GetTC(&f) && f->IsAIFollower(&GetCreature())) f->QuitAIFollower(GetCreature());`
- loop: `EAIActionReturn r; do r = AIAction_FindAGuardAndYellForHelp(pos, OpinionDeedHandle, true); while (r == 4);` then
  second 0x4F `SetInFindAGuardAndYellForHelp(false)`, `if (r == 2) GuardTold = true;` -- EAIActionReturn enumerators 2/4 (define per Ego_r).
- Terminate() outside the ifs; the def's lifetime ends at the end of the perceive block (release before Terminate).
- engine/CTCPerceiveThing.h: the default (flat) branch defines GetPerceivedThing() INLINE (line 60) -- retail calls 0x007446A0 out of line, so use the FABLE_RETAIL_CTCPERCEIVETHING branch (line 125, chunk 765 row 0x007446A0).

## 0x00B08250

# DOSSIER 0x00B08250 NParticleEngine::CPSCSingleSprite::SetTrailPositionsFromSpline (724 B)

## 1. Retail shape (xdis 00b08250 0x2e4)
- __thiscall, `ret 10h` (4 dword args). NO frame pointer (`sub esp,50h; push ebx/ebp/esi/edi`) -> /Oy on, no optimize("y",off).
- No SEH prologue (no /EHsc state frame). Frame 0x50: CMatrix3x4 at esp+0x30 (48 B), GetPoint out at esp+0x24, copied point
  at esp+0x18 (transformed in place there, then copied into the node), else-branch pos at [esp+0x10], step at [esp+0x14].
- Args after prologue: system=[esp+0x64], params=[esp+0x68], width=[esp+0x6c], time=[esp+0x70].
- Body: (a) inline STLport list clear of Trail (+0xb8 head ptr reloaded every iteration; free 0xBFEA14; then head->next=head->prev=head).
  (b) `if (UseSplinePoints /*+0x96 byte*/)`: count = Max<size ? Max : size with size() = (end-begin)/12 SIGNED (`imul 2AAAAAABh; sar edx,1`),
  computed TWICE (once for the compare, again for the result) -> literal `a < b() ? a : b()` with a signed long accessor, not std::min.
  count kept in ebx, index in ebp; `if (count<=0) skip` then bottom-tested loop. Per iteration: system/params/time reloaded from arg slots.
  (c) else: step = 1.0f/(float)MaxTrailLength (`fild [esi+88h]; fdivr [0122DED8]`=1.0f), pos=0; do-while (no entry test)
  `pos += step; while (pos <= 1.0f)` (`fst [esp+10h]; fcomp 1.0; fnstsw; test ah,41h; jnp`). Here ebx=system, ebp=time hoisted
  (the old NOTES wording "system in ebp" is backwards: retail 0xB083ED `mov ebx,[esp+64h]` system, 0xB083F1 `mov ebp,[esp+70h]` time).
- Each iteration: GetPointAtXxx(params, out@0x24, i/pos, time); copy out->p@0x18; `test byte [system+50h],40h; jne skip` ->
  GetSystemTransform(m) + inline TransformPoint(p); then list push_back: edi=[esi+0xb8] (end) loaded BEFORE malloc(0x18),
  placement-new null test, node data = p.X,p.Y,p.Z, width read straight from arg slot [esp+6ch]; link node before end.
- Transform row order (all three rows): `fld E3k; fmul z(mem); fld E2k; fmul y; faddp; fld E1k; fmul x; faddp; fadd E4k` -- i.e.
  `E3k*zz + E2k*yy + E1k*xx + E4k`, matrix element loaded first. xx,yy were pushed on x87 before row X (fld x; fld y) and reused
  as st(3)/st(2) in rows Y/Z; z read from memory. EXACT match for CMatrix3x4.h opt-in FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
  (rebuild/include/engine/CMatrix3x4.h:169-183, proven byte-exact in landed 00a89740 C3DMesh2::GetHelperPointPositionAnimated).

## 2. Name / signature (two sources)
- Ego_r PDB names: ghidra_out/egor_pdb_names.tsv 00ac98f0 and FableWin signatures (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv 02f382f0 / 01835c16):
  `?SetTrailPositionsFromSpline@CPSCSingleSprite@NParticleEngine@@IAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@MM@Z`
  -> PROTECTED (I), non-virtual __thiscall, void(CParticleSystem& system, const CParticleUpdateParameters<unsigned long,unsigned long>& params, float, float).
  Manifest label name is RIGHT; rebuild/manifest/functions.tsv:40572 types params as POINTERS (wrong: they are & and const&).
- Param names (width, time) are descriptive; arg 3 goes into CSpriteTrailEntry::Width, arg 4 forwarded as GetPointAt* `time`.
- Callers (xref.py): 0x00B087A2 (inside CPSCSingleSprite::Update 0x00B08530, parked DIFFER) and 0x00B08800 (inside
  PreparePrimitives 0x00B087C0, parked DIFFER(2951v2967)). Neither landed.

## 3. Callees
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00BFEA14 | rebuild/src/compiled/00/bf/global_free_00bfea14.cpp | free | CRT free | ok |
| 0x00BFEA0E | rebuild/src/compiled/00/bf/global_malloc_00bfea0e.cpp | malloc | CRT malloc (STLport node alloc) | ok |
| 0x00B219A0 | rebuild/src/compiled/00/b2/NParticleEngineCPSCSplineBase_GetPointAtIndex_00b219a0.cpp | CPSCSplineBase::GetPointAtIndex | ?GetPointAtIndex@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@JM@Z (const params&, C3DVector&, long, float) | WRONG: draft uses pointers |
| 0x00B21940 | rebuild/src/compiled/00/b2/NParticleEngineCPSCSplineBase_GetPointAtPos_00b21940.cpp | CPSCSplineBase::GetPointAtPos | ?GetPointAtPos@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@MM@Z | WRONG: draft uses pointers |
| 0x00AF2250 | rebuild/src/compiled/00/af/NParticleEngineCParticleSystem_GetSystemTransform_00af2250.cpp | CParticleSystem::GetSystemTransform(CMatrix3x4&) const | ?GetSystemTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z | WRONG: draft calls it GetInverseSystemTransform (different PDB function, Ego_r 00ab5880) and non-const |
- 0x0122DED8 = float 1.0f constant.
- No vtable calls.

## 4. Members
- this (CPSCSingleSprite; NO shared header engine/CPSCSingleSprite.h, nor CPSCSplineBase.h, nor CEnginePrimitiveSpriteTrail.h).
  Retail offsets are Ego_r minus 4 (12-byte STLport CArray in CCardinalSpline vs PDB 16):
  - +0x10 Spline (CCardinalSpline; PDB +0x10) -> ControlPoints begin +0x10 / end +0x14. Shared header engine/CCardinalSpline.h
    (ControlPoints private; inline `long GetControlPointCount() const { return ControlPoints.size(); }` -- signed long, matches `jl`).
  - +0x88 MaxTrailLength (long; PDB +0x8C).
  - +0x96 UseSplinePoints (bool; PDB +0x9A).
  - +0xAC TrailUpdatePrimitive (CEnginePrimitiveSpriteTrail; PDB +0xB0), its `list<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry> Trail` at +0x0C -> abs +0xB8.
  - CSpriteTrailEntry {C3DVector Position +0; float Width +0xC} (16 B; nested in CEnginePrimitiveSpriteTrail per PDB list type).
- system (CParticleSystem, engine/CParticleSystem.h): +0x50 bit 6 = StayWithEmitter bitfield; header offers
  `FABLE_FORCEINLINE void TransformPosIfWorld(C3DVector&) const` (line 48) + GetSystemTransform decl under FABLE_RETAIL_CPARTICLESYSTEM.
  Header comment says TransformPosIfWorld has a debug copy 0x02F384F0 IN THE CPSCSingleSprite TU -> it is the original inline used here.
- CMatrix3x4 (engine/CMatrix3x4.h): FABLE_RETAIL_CMATRIX3X4 + FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS. Do NOT also define
  FABLE_RETAIL_CPARTICLE_SPRITE / _CMATRIX3X4_PARTICLE_POINT / _TRANSFORMPOINT_SUMS (earlier #elif wins).
- C3DVector: engine/C3DVector.h (has the memberwise user copy ctor AddTrailPosition needed).
- Locally modelled by the old draft but a shared header exists: C3DVector, CMatrix3x4, CArray, CCardinalSpline, CParticleSystem, CPSCBase
  (engine/CPSCBase.h), CParticleUpdateParameters (engine/NParticleEngine_CParticleUpdateParameters.h, used by EnterTimelineEnd 00b0a130),
  STLport list (rebuild/include/stlport/fable_containers.h list: inline clear() lines 852-861 and push_back -> insert -> _M_create_node).

## 5. Visibility
- list clear / push_back / insert / _M_create_node: inlined in retail (default fable_containers.h; do NOT set
  FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE / _PUSHBACK_NOINLINE / _INSERT_OUTOFLINE).
- TransformPosIfWorld, TransformPoint, GetControlPointCount: inline (forceinline as in headers).
- Same-TU neighbours defined earlier: AddTrailPosition 0x00B081A0 (landed, immediately before; same push_back idiom, NOT called here),
  ClearAllPrimitives 0x00B07570, ReadBinary 0x00B068F0, InterpolateTo 0x00B062A0, SmoothFadeValue 0x00B05E10. GetPointAt*/GetSystemTransform are other TUs -> declarations only.

## 6. Sibling landed rows
- rebuild/src/compiled/00/b0/NParticleEngineCPSCSingleSprite_AddTrailPosition_00b081a0.cpp: no Flags, no pragmas; local std::list model
  whose push_back(CSpriteTrailEntry(pos,width)) gives exactly retail's node code (end loaded before malloc, null test, width from arg).
- rebuild/src/compiled/00/a8/C3DMesh2_GetHelperPointPositionAnimated_00a89740.cpp: `// Flags: none`, defines FABLE_RETAIL_CMATRIX3X4,
  FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS, FABLE_RETAIL_C3DVECTOR; point copied to a local and transformed in place.
- rebuild/src/compiled/00/af/NParticleEngineCPSCRenderSprite_PreparePrimitives_00afcc30.cpp: `// Flags: /d2inlT1537 /d2inlS15`,
  FABLE_RETAIL_CMATRIX3X4, FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_CARRAY ... (particle-engine idiom set).
- CPSCSingleSprite InterpolateTo 00b062a0 / ReadBinary 00b068f0 / SmoothFadeValue 00b05e10: `// Flags: none`, no pragmas -> TU default /O2 /Oy.

## 7. Old draft model errors
1. Callee GetSystemTransform misnamed GetInverseSystemTransform (and non-const); GetPointAt* and own params use pointers, retail PDB uses const&/&.
2. Function is protected in PDB (draft public) -- name mangling IAE.
3. Local ToSystemSpace with `m.E31*z + m.E21*y + m.E11*x + m.E41` on a local struct: use system.TransformPosIfWorld(p) with
   ZYX_SUMS TransformPoint (xx/yy/zz const copies inside the member) -- the parked "x87 operand canonicalisation lane gap" is the
   exact residue that ZYX_SUMS fixed in 00a89740 (its default parenthesised form emitted same instrs but different schedule).
4. count: draft uses local CArray::Size() returning long; use Spline.GetControlPointCount() (shared header, signed long), keep
   `MaxTrailLength < n() ? MaxTrailLength : n()` (two evaluations are visible in retail). Draft's count spill to [esp+10h] vs retail ebx
   may follow from the transform's register pressure.
5. Node push: draft's emplace_back(p,width) custom; use Trail.push_back(CEnginePrimitiveSpriteTrail::CSpriteTrailEntry(p, width)) with
   the AddTrailPosition idiom. Retail order: point -> copy p -> TransformPosIfWorld(p) -> push_back; the copy (0x24 -> 0x18) is real.
6. Else loop form ok (for(pos=0; pos<=1.0f; pos+=step) gives no entry test since 0<=1 is constant).

## 0x006CF530

# DOSSIER 0x006CF530 CTCWife::FindAppropriateGift (728 B)

## 1. Retail shape (xdis 006cf530 0x2e8)
- thiscall, plain `ret` (0x6CF807), void, no params. NO frame pointer: `sub esp,38h ; push ebp ; mov ebp,ecx` (ebp = this, /Oy on -> do NOT add optimize("y",off)). No SEH frame, no /GS cookie. /O2-style (no size idioms in this body).
- Saves ebp, then esi (after the 0x23 bit test), then ebx/edi (after the first GetTC) -- early-out `je 0x6CF803` restores only ebp.
- Frame slots (frame-relative = after 3 pushes): +0x10 budget (also the fild temp of price), +0x14 gift def, +0x18 item def AND later the value def (same slot), +0x1C building def (PeekPSimDefinition hidden result), +0x20 cached list node (ebp spill), +0x24 best value (init 0x7FFFFFFF), +0x28 map iterator, +0x2C best gift key, +0x30 this, +0x34 map header, +0x38 list head, +0x3C the by-value stock map (3 dwords).

## 2. Identity
- Name PROVEN (2 sources): Ego_r.pdb `?FindAppropriateGift@CTCWife@@AAEXXZ` (egor_pdb_names/egor_signatures @Ego_r 0x007060B0) and FableWin.pdb same spelling (_fablewin_api.tsv 0x017AFCB0 / 0x02488520; FableWin.exe.signatures.tsv). Already declared in rebuild/include/engine/CTCWife.h:153 (private, `void FindAppropriateGift();`, comment 0x006CF530). Manifest label is CORRECT.
- Mangled leaf: `?FindAppropriateGift@CTCWife@@AAEXXZ` -> private, non-const, __thiscall, void(void).
- Caller (xref.py): one call at 0x006D25D4, inside CTCWife::FrameUpdate 0x006D1E30 (vtable slot 9, CTCWife.h header comment) -- not landed.
- Same retail TU neighbourhood: CTCWife.cpp rows 0x006CEC70..0x006D4730.

## 3. Callees (landed file / landed name / PDB name) -- draft spelling vs PDB
| addr | landed row | real name | draft spelling (WRONG unless noted) |
|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (inside inline CThing::GetTC) | CInterfaceMap::LowerBound (local model) |
| 0x007EE540 | 00/7e/CTCVillageMember_GetPVillage_007ee540.cpp | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | CTCHome::GetContents -- WRONG (iface 0x23 = CTCVillageMember, returns CTCVillage*) |
| 0x00686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` (returns CWorld&) | CTCWife::GetWorld returning CWorld* |
| 0x0049D810 | 00/49/CWorld_GetWorldSeed_0049d810.cpp | `?GetWorldSeed@CWorld@@QBEPAKXZ` (CWorld.h:143/555) | GetPRandomSeed -- WRONG |
| 0x004978A0 | 00/49/global_GFFloatRandom_004978a0.cpp | `?GFFloatRandom@@YIMMPAK@Z` (engine/GFFloatRandom.h) | ok |
| 0x00497890 | 00/49/global_GFROR13_00497890.cpp | `?GFROR13@@YIJK@Z` returns LONG (engine/GFRandom.h / GFROR13.h) | draft returns unsigned long |
| (inline) | engine/GFRandom.h | `?GFRandom@@YIKKPAK@Z` inline: seed*0x24A1+0x24DF, GFROR13, % range | draft RandomBool -- spell as `GFRandom(0xFFFFFFFF, seed) > 0x7FFFFFFF` (retail `or ecx,-1 ; div ecx ; cmp edx,7FFFFFFFh ; jbe`) |
| 0x00BFEA70 | global_ftol2 | __ftol2 (cast to long) | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (via CIntelligentPointer<CThingBuilding>) | ok-ish (local model) |
| 0x0082D2D0 | 00/82/CThingBuilding_PeekPSimDefinition_0082d2d0.cpp | `?PeekPSimDefinition@CThingBuilding@@QBE?AV?$CDefPointer@$$CBVCSimBuildingDef@@@@XZ` (BY VALUE, hidden result, ret 4) | CThing::GetDef(CThingDefPointer*) -- WRONG |
| 0x00774610 | 00/77/CTCShop_GetStockList_00774610.cpp | `?GetStockList@CTCShop@@QBE?BV?$map@JJ...@std@@XZ` (const map<long,long> by value) | CTCStock::GetStock -- WRONG (iface 0x53 = CTCShop, TCI_SHOP) |
| 0x006869D0 | 00/68/CTCBase_GetDefinitionManager_006869d0.cpp | `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ` (const ref) | returns pointer |
| 0x0042AF3C | 00/42/CDefinitionManager_GetDefT_0042af3c.cpp | CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) const; sibling 0x006D0630 uses T = CThingBaseDef | CItemDef local |
| 0x006D3EF0 | 00/6d/CDefClassBase_GetSubDefCGiftDef_006d3ef0.cpp | `??$GetSubDef@VCGiftDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCGiftDef@@@@@Z` | GetGiftDef -- WRONG |
| 0x0059350C | 00/59/CDefClassBase_GetSubDefCStockItemDef_0059350c.cpp | `??$GetSubDef@VCStockItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCStockItemDef@@@@@Z` | GetValueDef -- WRONG |
| 0x005B16FF | 00/5b/CRegion_SetMiniMapRegionExitTextOffsetX_005b16ff.cpp (garbage label/leaf `?M@S@@`) | STLport _Rb_tree_iterator::operator++() (push esi; [esi]=_M_increment 0x004292C0([esi]); return this) -- unaligned ICF'd COMDAT | CStockMap::iterator::Increment |
| 0x004DF885 | 00/4d/CActiveFile_OnReadFinished_004df885.cpp (garbage label, leaf `_Forward`) | std::map<long,long>::~map (jmp to tree dtor 0x004DD2B1; identity per 00/6e/CGameScriptInterface_scalardeletingdestructor_006e7d50.cpp) | CStockMap::~CStockMap |
| vslot 1 | -- | CDefPointee Delete (`call [eax+4]` when count hits 0) | Destroy |

## 4. Members
- CTCWife (engine/CTCWife.h, FABLE_RETAIL_CTCWIFE; pulls CTCBASE + DEF_CLASS_BASE): ParentThing +0x04 (CTCBase), GiftGivingPriceValue +0x70 long, GiftToGiveDef +0x74 long (static-asserted in header).
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED, as siblings OnGreet/Died/OnHeroViolence use): present-bit set +0x20 (0x23 -> byte +0x24 bit 8; 0x53 -> dword +0x28 bit 0x80000), interface CVectorMap +0x44 (end at +0x48), dead flag +0x91 bit0. GetTC<T> expanded inline (LowerBound call + `it==end || key<it->first ? end : it` -> ->second). Retail tests HasTC bit before each GetTC.
- CTCVillage (engine/CTCVillage.h, FABLE_RETAIL_CTCVILLAGE): VillageBuildings +0x28 `list<CIntelligentPointer<CThingBuilding>>` (struct_layouts_egor CTCVillage+40; STLport list = one node ptr). Node: next +0, prev +4, value +8 (`lea ecx,[ebp+8]`). Loop: head=[v+0x28], n=head->next.
- CSimBuildingDef (engine/CSimBuildingDef.h, FABLE_RETAIL_CSIMBUILDINGDEF): Property +0x3C long (retail = Ego_r 0x40-4); retail `test byte [def+3Ch],10h` = Property & 0x10. Draft calls it CThingDef::Flags -- WRONG.
- CThingBuilding.h: FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF declares `CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;`.
- CTCShop.h: TCI_SHOP (0x53) StatGetInterfaceType exists, but GetStockList is NOT declared in CTCShop.h (grep: no hit) -- a header declaration `const std::map<long,long> GetStockList() const; // 0x00774610` is needed (leaf in 00/77/CTCShop_GetStockList_00774610.cpp: QBE?BV?$map@JJ...: const-qualified return).
- CGiftDef (engine/CGiftDef.h): Type +0x28 EGiftType (enumerators unproven; retail compares ==1), IsWeddingRing +0x2C bool. Draft Kind/Special WRONG.
- CStockItemDef (engine/CStockItemDef.h, FABLE_RETAIL_CSTOCKITEMDEF): DefaultPrice +0x28 long. Draft CValueDef::Value WRONG.
- CDefPointer (engine/CDefPointer.h): count IVRefCount +4, Delete = vslot 1. Release variants: default in-place, FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF (`mov ebx,[eax+4]; mov ecx,eax; add eax,4; dec ebx; ...; mov [eax],ebx` + zeroing), _RELEASE_POINTEE, _RELEASE_SELFDELETING_POINTEE.
- Locally modelled in the draft though shared headers exist: CTCWife, CThing, CInterfaceMap(CVectorMap.h), CDefPointer, CBaseIntelligentPointer/CIntelligentPointer, CDefinitionManager, CWorld, CTCVillage(as CHomeContents), CTCVillageMember(as CTCHome), CTCShop(as CTCStock), CThingBuilding/CSimBuildingDef(as CThingDef), CGiftDef, CStockItemDef, std::map (fable_map.h), GFRandom.

## 5. Callee-body visibility
- GetTC/HasTC: inline expanded (FABLE_RETAIL_CTHING_GETTC_EXPANDED).
- GFRandom inline (GFRandom.h); GFROR13 out of line (retail_asm row).
- GetDef<T> / GetSubDef<T>: called out of line; sibling 0x006D0630 (same TU, same defs) defines FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY ("visible but called, which keeps the handles' releases in the `dec dword ptr [ecx+4]` form") -- exactly the inner releases here (item via ebp, gift, value).
- map<long,long>::~map 0x004DF885 and iterator::operator++ 0x005B16FF are called OUT OF LINE (both unaligned ICF'd COMDATs). fable_map.h has FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE (noinline tree ctor/dtor -- but retail calls ~map, not the tree dtor; a map-level dtor opt-in does not exist) and NO opt-in for out-of-line iterator operator++ (operator++ is inline at fable_map.h:73). The author likely needs a new per-TU opt-in (noinline map dtor + noinline operator++), or match how 0x005A30A1 / 0x005907AB modelled it (they declare map dtor 0x004DD2B1 locally -- check whether that passes the shared-header rule).
- CTCVillageMember::GetPVillage, PeekPSimDefinition, GetStockList, GetPItem: out of line (other TUs).
- IsInLoveWithHero 0x006CF900 is defined earlier in the same TU (as 0x006D0630 shows) -- not called here.

## 6. Sibling landed rows (idioms)
- 00/6d/CTCWife_ReceivedNiceGiftIsOpinionReactionHappening_006d0630.cpp -- `// Flags: none`, no pragmas; macros: BASE_OBJECT, CCHARSTRING, CCLASS, DEF_CLASS_BASE, CDEFSTRINGTABLE, STLP_LIST, CDEFINITIONMANAGER, CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, CGAMEDEFINITIONMANAGER, CRESOURCELIST, CRESOURCE, CDEFCLASSBASE_GETSUBDEF_BODY, CTCBASE, CTHING, CINTELLIGENTPOINTER, ..., CWORLD, CWORLD_PEEKDEFINITIONMANAGER, CTCWIFE, CTCWIFE_MARRY, CTCWIFE_NICEGIFT. Same GetDef<CThingBaseDef>/GetSubDef<CGiftDef>/IsWeddingRing pattern: `CDefPointer<const CThingBaseDef> object_def; CDefPointer<const CGiftDef> gift_def; GetWorld().PeekDefinitionManager().GetDef(idx,&object_def) && object_def->GetSubDef(&gift_def) && gift_def->IsWeddingRing`. BEST TEMPLATE.
- 00/6c/CTCWife_FindMaritalHome_006cfaa0.cpp -- neighbour, same GetPVillage + VillageBuildings walk + GetPItem; no pragmas; shared CVectorMap.h, still local CThing (FABLE_RETAIL_CTHING_GETTC_EXPANDED variant used by OnGreet/Died/OnHeroViolence).
- 00/6c/CTCWife_Unmarry_006cff10.cpp, 00/6c/CTCWife_IsAttractedToHero_006cfe80.cpp -- no pragmas/macros (TU default /O2, no frame pointer).
- 00/6d/CTCWife_Murdered_006d00a0.cpp -- `// Flags: none`, macro set CTHING..CTCWIFE_MARRIAGEEND.
- Rows with `// Flags: /GS` (HaveSex, InitiateDivorce, Marry) have buffers; this row has no cookie -> no /GS needed.

## 7. Old draft model errors (vs disassembly)
- All names in sec.3/4 (village/building/shop/stock-item/gift; GetWorldSeed; GFRandom).
- PeekPSimDefinition returns by value: write `CDefPointer<const CSimBuildingDef> def = building->PeekPSimDefinition();`. Its release at 0x6CF7B8 is the count-ref form (`mov esi,[eax+4]; mov ecx,eax; add eax,4; dec esi; mov edx,esi; cmp edx,edi; mov [eax],esi; jne; call [edx+4]; mov [esp+1C],0`) -- same as the GetSubDef temp release / RELEASE_COUNTREF; inner releases are in-place `dec [ecx+4]`. The draft's hand-made CThingDefPointer dtor was a workaround.
- Item def: retail keeps the item pointer in ebp after GetDef and REUSES the item slot (+0x18) for the value def; item released last (0x6CF77B via ebp), gift (0x6CF76D), value first (0x6CF759, only on the value path). Order of declaration: item, gift, then value in an inner block. Slot reuse suggests the item handle is not live in its slot -> try e.g. GetDef into a temp scoped narrower than the value def, or a different holder; unresolved (NOTES.md "retail keeps the item def in ebp and reuses its frame slot for the value def").
- Gift test: `if (!item->GetSubDef(&gift) || (gift->Type != 1 && !gift->IsWeddingRing))` -- draft matches retail flow.
- Price: retail loads +0x70 into a local between the GFFloatRandom and the second GetWorld (draft ok); `spread = r*r + 1.0f` (fld st0; fmul st1; fadd 1.0).
- Budget compare: `abs(cost-budget) < abs(budget-best) && cost-budget < budget/4` -> best=cost, key=it->first (draft ok). End: `if (bestGift > 0) { GiftToGiveDef = bestGift; GiftGivingPriceValue -= bestValue; }` (retail reads this from slot +0x30).
- Outer loop: retail rotated loop with pre-test (je to end), cached node in +0x20, `continue` on null/dead/no def; draft's for-loop is plausible.
- Building def gate: retail tests `def->Property & 0x10` THEN HasTC(0x53) (bit 0x80000 at +0x28) then GetTC<CTCShop>(0x53)->GetStockList(); stock map is a by-value local released by ~map 0x004DF885 call.

## 0x0079D7E0

# DOSSIER 0x0079D7E0 CTCBulletTime::UnPause (732 bytes)

## 1. Retail shape (xdis 0079d7e0 0x2ec)
- thiscall, `this` in esi; plain `ret` (0x0079DABB) -> no args. No frame pointer (`sub esp,48h ; push ebx ; push esi`, ebp/edi pushed
  only after the Pausable/Paused test = shrink-wrapped). No SEH prologue (no /EHsc frame), no size-opt idioms -> plain /O2 TU, no
  pragmas. Same as landed siblings Pause 0x0079CF30 / PlayEffect 0x0079CC80 (PlayEffect says "plain /O2 (no EH frame)"; no `// Flags`/`#pragma` in any CTCBulletTime row).
- ebx = the zero register until 0x0079DA36, where it is reused as the ScreenFilterSaturation vector base.
- At entry `mov [esp+14h],ebx` BEFORE the Pausable test = the conditional-temporary flag of the `"SLOWTIME"` CCharString built
  inside an `&&` (`test byte [esp+1Ch],1` at 0x0079D8AE then dtor). So the CCharString("SLOWTIME") is a temporary in the condition.
- Control flow: `if (Pausable && Paused) { Paused=false; ... }` (two `je 0x79dab6` to the common epilogue).
  1. sound: `GDefStringTable.GetString(PDef->LoopSound)` EXPANDED (push [def+0xCC], hidden ret ptr, ecx=0x013CA828), PlaySoundW(&str, 1),
     result stored to LoopSound (+0x38) BEFORE ~CCharString -> full-expression temp or named local both fit.
  2. `if (!m_tpLoopingEffect.Get() || m_tpLoopingEffect.Get()->IsDead())` (GetPItem twice, `shr cl,5 ; test cl,1`) -> else skip to 0x79D99C.
  3. effects CDefPointer at frame slot, Object=0; GetSubDef(&effects) 0x669580 (out of line, ecx = PDef.Object [esi+20h]);
     `mov ebp,[esp+14h]` = effects.Object cached in ebp after GetSubDef; `&& effects->GetEffectFromString(CCharString("SLOWTIME"), effect)`
     (ecx=ebp, effect index slot initialised 0). Result stored as a bool in frame byte [esp+13h] (1/ bl). Then CCharString temp dtor if flag.
  4. if true: CRightHandedSet built IN PLACE (6 stores: 0,0,1.0 / 0,1.0,0 = Up(0,0,1), Forward(0,1,0) -> inline ctor);
     y = GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT)*0.5f FIRST, then x = GFVXToAX((float)VIRTUAL_SCREEN_WIDTH)*0.5f (right-to-left ctor args
     of C2DVector(x,y)); 0.5f const 0x0122F59C; Create2DEmitter(effect, pos, 0x52, true, set) on GetParticleEmitterManager();
     CIntelligentPointer<CParticleEmitter>(emitter) ctor OUT OF LINE 0x00655318 into the dead effects slot; inline operator=
     (GetPItem on temp, SetPItem on edi=&m_tpLoopingEffect); temp dtor 0xA01C10; `if (IS_THING_ALIVE(m_tpLoopingEffect))` call 0x0079DC20
     (fastcall, ecx=edi); then `GetPItem()->... |= 0x10` at +0x44.
  5. CDefPointer dtor inline via ebp: `dec [ebp+4]; jne; call [vtbl+4]` (no null-out store).
  6. bypass vector at [esp+34h] (3 dwords zeroed), element temp {CIntelligentPointer<CThing> (vtable 0x01238C6C) = ParentThing, bool 1}
     built inline (base ctor 0xA01B10, vptr, SetPItem), push_back 0x6A3C70, element dtor 0xA01C10.
  7. GetWorld() 0x686A70; env chain read immediately: [eax+18h] -> [+1964h] into ecx (interpolator) -- matches Pause's
     `GetWorld().GetEnvironment().GetSpellEffectInterpolator()` one-expression shape.
     Then PDef reloaded from [esi+20h], index = m_nLevel<<2; TintB(0x178), TintG(0x16C), TintR(0x160) loaded on x87, A=1.0 stored
     first, then R,G,B into a CRGBFloatColour temp at [esp+1Ch] (reuses the element slot); vector bases: Brightness 0x154 in edi,
     Contrast 0x148 in ebp, Saturation 0x13C in ebx, Alpha 0x130 SPILLED to [esp+30h]. Pushes: &vector, &tint, bri, con, sat, alpha,
     `push 2` (priority), `push 0` (time = 0.0f). call 0x6B85D0. Result -> edi -> ScreenFilterKey (+0x50).
  8. GetTC(TCI_CARRYING=0x46) expanded: `test byte [esi+28h],40h`, LowerBound 0x40F020 on map +0x44 with key slot = 0x46,
     `cmp [eax],46h ; jle` -> carrying->SetAllCarriedThingsAsBypassFilter(key) 0x6A2520 (key = edi, the cached return).
  9. vector dtor 0x6A3770 (out of line). Epilogue.

## 2. Identity
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv / egor_signatures.tsv): `?UnPause@CTCBulletTime@@QAEXXZ` at Ego_r 0x007A3400 (817 bytes), next to
  Pause 0x007A29E0 / ImmediateCancel 0x007A2AE0 etc. -> public `void CTCBulletTime::UnPause()`, non-const, no args. Matches retail `ret`,
  thiscall, and the retail neighbourhood (Pause 0x79CF30, ImmediateCancel 0x79D020, FinalCleanup 0x79D2D0, OnKill 0x79D7C0, FinalCleanup tailjump 0x79D7D0,
  IS_THING_ALIVE 0x79DC20, GetInterfaceType 0x79DC50). manifest functions.tsv also `0079d7e0 UnPause CTCBulletTime __thiscall void`.
- Second source: the landed rows stdvector_vector_006a3770.cpp and STLvectorCScreenFilterSThingByPass_pushback_006a3c70.cpp cite
  0x0079D7E0 as CTCBulletTime's bypass-vector user; xref: callers 0x0072C00A and 0x0072C1AC are in two unlanded CTCSpecialAbilities
  functions between ResumeAbilitiesForCutscene 0x72BEC0 and FrameUpdate 0x72C370 that look up interface 0x92 (TCI_BULLET_TIME), test
  `[ecx+55h]` (Paused) and call Pause 0x79CF30 / this. Manifest label is CORRECT.
- Leaf: `?UnPause@CTCBulletTime@@QAEXXZ`. Not in a vtable (rtti-vtables.tsv has only slots 2/14 for 0x79D7D0/0x79D7C0).
- Header CTCBulletTime.h does NOT declare UnPause yet -> author adds a declaration beside Pause under FABLE_RETAIL_CTCBULLETTIME_MEMBERS
  (or a new opt-in), plus a PlaySoundW declaration (see below).

## 3. Callees (landed file / landed name / PDB name; draft spelling flagged)
| addr | landed row | PDB / landed name | draft spelling |
|---|---|---|---|
| 0x009D49B0 | 00/9d/CDefStringTable_GetString_009d49b0.cpp | ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z (const) | OK but global spelled `g_DefStringTable` -- WRONG, landed/PDB `GDefStringTable` (0x013CA828, engine/CDefString.h uses it). Better: no explicit call -- CDefString LoopSound converted (see 7) |
| 0x0079CE80 | 00/79/CTCBulletTime_PlaySoundW_0079ce80.cpp | Ego_r `?PlaySoundW@CTCBulletTime@@AAEKABVCCharString@@_N@Z` = private `unsigned long PlaySoundW(const CCharString&, bool)`; LANDED leaf says `QAEKKK` (unsigned long, unsigned long) -- landed spelling disagrees with PDB (NAME_FIXES candidate; the ~CTCBulletTime row 0x79D110 already declares `PlaySoundW(const CCharString&, unsigned long)`) | draft `PlaySound` -- WRONG (windows.h renames to PlaySoundW; PDB spells PlaySoundW) |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | CCharString(const char*, long=-1) / ~CCharString ("SLOWTIME" @0x0126C48C) | use fable_string.h (FABLE_RETAIL_CCHARSTRING), not local struct |
| 0x00A01B10/B50/B90/C10 | 00/a0/CBaseIntelligentPointer_* | ??0 / GetPItem (QBE PAVCBaseObject) / SetPItem / ??1 (UAE, virtual) | draft local struct; use engine/CIntelligentPointer.h |
| 0x00669580 | 00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp | ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z (const, CDefPointer<const T>*) | draft on the def struct, non-const -- wrong signature |
| 0x00669D30 | 00/66/CSpecialEffectsDef_GetEffectFromString_00669d30.cpp | ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z (const, long&) | draft `CalcAndLookup(..., long*)` -- WRONG name and wrong param (long& not long*) |
| 0x009E1E40 | 00/9e/global_GFVYToAY_009e1e40.cpp | ?GFVYToAY@@YIMM@Z `float __fastcall GFVYToAY(float)` | draft `GFAYToVY`, __stdcall -- WRONG |
| 0x009E1E10 | 00/9e/global_GFVXToAX_009e1e10.cpp | ?GFVXToAX@@YIMM@Z | draft `GFAXToVX`, __stdcall -- WRONG |
| globals 0x013961E8 / 0x013961F0 | (many rows) | `extern double VIRTUAL_SCREEN_WIDTH` / `VIRTUAL_SCREEN_HEIGHT` | draft `g_ScreenCentreX/Y` -- WRONG |
| 0x00AEAAF0 | 00/ae/NParticleEngine_GetParticleEmitterManager_00aeaaf0.cpp | ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ (free __fastcall, returns reference) | draft `CParticleEmitterManager::Get()` static -- WRONG |
| 0x00AEB790 | 00/ae/NParticleEngineCParticleEmitterManager_Create2DEmitter_00aeb790.cpp | Create2DEmitter(long, const C2DVector&, EEngine2DLayer, bool, const CRightHandedSet&) | draft passes `const C2DVector*` -- wrong type |
| 0x00655318 | 00/65/CIntelligentPointerNParticleEngineCParticleEmitter_CIntelligentPointer_00655318.cpp | ??0?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@QAE@PAV12@@Z | OK (out-of-line) -> FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE |
| 0x0079DC20 | 00/79/global_ISTHINGALIVE_0079dc20.cpp | IS_THING_ALIVE<CIntelligentPointer<NParticleEngine::CParticleEmitter>> __fastcall (template; only caller is UnPause 0x79D977) | draft member `IsThingAlive()` -- WRONG; free template in engine/ThingAlive.h |
| 0x006A3C70 | 00/6a/STLvectorCScreenFilterSThingByPass_pushback_006a3c70.cpp | std::vector<NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser>::push_back (STLport, out of line) | draft element `CScreenFilterBypassThing` -- WRONG; PDB element = CThingGraphicArgPasser (9 bytes) per ScreenFilterFadeTo mangling |
| 0x006A3770 | 00/6a/stdvector_vector_006a3770.cpp | ??1?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@...@std@@QAE@XZ | draft local struct |
| 0x00686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (protected const, returns CWorld&) | draft returns CWorld* -- wrong |
| 0x006B85D0 | 00/6b/CEnvironmentSpellEffectInterpolator_ScreenFilterFadeTo_006b85d0.cpp | ?ScreenFilterFadeTo@CEnvironmentSpellEffectInterpolator@@QAEPAXMW4ScreenEffectPriority@NScreenEffect@@MMMMABVCRGBFloatColour@@ABV?$vector@UCThingGraphicArgPasser...@@Z = void*(float time, ScreenEffectPriority, float alpha, sat, con, bri, const CRGBFloatColour&, const vector&) | draft `CEnvironmentSpellEffects::ScreenFilterFadeTo(long mode, long priority, ..., const CRGBAFloat&, const X*)` -- WRONG class, first param float (0.0f), CRGBFloatColour, vector by const REFERENCE |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound | via CThing::GetTC expanded |
| 0x006A2520 | 00/6a/CTCCarrying_SetAllCarriedThingsAsBypassFilter_006a2520.cpp | ?SetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXPAX@Z | OK name; NOT declared in engine/CTCCarrying.h (grep finds no declaration) -> needs a declaration/opt-in |
| vslot 1 of effects def | inline CDefPointer dtor (engine/CDefPointer.h) | | draft local CDefPointer |

## 4. Members
- this (engine/CTCBulletTime.h, FABLE_RETAIL_CTCBULLETTIME + FABLE_RETAIL_CTCBULLETTIME_MEMBERS [+ FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
  to type PDef as CDefPointer<const CSpecialAbilitiesBulletTimeDef> with operator->]): ParentThing +0x04 (CTCBase.h, GetParentThing()),
  PDef +0x20, m_nLevel +0x24, m_tpLoopingEffect +0x30 (CIntelligentPointer<NParticleEngine::CParticleEmitter>), LoopSound +0x38
  (unsigned long), ScreenFilterKey +0x50 (void*), Pausable +0x54, Paused +0x55. Draft's CTCBulletTimeImpl / f4 / PDef_Object /
  reinterpret LoopingEffect() casts are obsolete -- use the real members directly (as Pause does).
- CSpecialAbilitiesBulletTimeDef (engine/CSpecialAbilitiesBulletTimeDef.h, guard FABLE_RETAIL_DEF_CLASS_BASE; included by PlayEffect):
  CDefString LoopSound +0xCC; std::vector<float> ScreenFilterAlpha +0x130, Saturation +0x13C, Contrast +0x148, Brightness +0x154,
  TintR +0x160, TintG +0x16C, TintB +0x178 (struct_layouts_egor / _quarantine header asserts 0xCC, 0x130). Read with vector operator[].
- CParticleEmitter (engine/CParticleEmitter.h): Dead bit (retail +0x40 bit 5) via IsDead() under FABLE_RETAIL_CPARTICLEEMITTER_KILL;
  +0x44 `or 10h` = AlwaysUpdate (Ego_r bitfield uchar group +80: PreWaterEffect,WaterEffect,ZBufferWriteable,Visible,AlwaysUpdate=bit4)
  via `SetAlwaysUpdate(true)` under FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE (header cites `or al,10h` on +0x44 at 0x007CD2C6).
  Draft's `SetPersistent` / `IsFinished` are made-up names.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED): bitset +0x20, map +0x44; GetTC(TCI_CARRYING,&c).
- CWorld/CEnvironment: FABLE_RETAIL_CWORLD + FABLE_RETAIL_CWORLD_ENVIRONMENT (+0x18), FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
  (+0x1964, GetSpellEffectInterpolator()), CEnvironmentSpellEffectInterpolator.h FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
  (needs FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER for the enum + CThingGraphicArgPasser/ThingGraphicArgPassers typedef in
  engine/NScreenEffect_CScreenEffectPriorityStack.h). FADETO opt-in has no landed user yet (only declared for chunk 1010).
- CTCCarrying (engine/CTCCarrying.h): no SetAllCarriedThingsAsBypassFilter declaration in any opt-in (FABLE_RETAIL_CTCCARRYING_BYPASSFILTERS
  only adds BypassFilters +0x1c).
- Classes the draft models LOCALLY that have shared headers: C2DVector, C3DVector, CRightHandedSet, CCharString (fable_string.h),
  CDefStringTable, CParticleEmitter, CParticleEmitterManager, CSpecialEffectsDef, CDefPointer, CSpecialAbilitiesBulletTimeDef,
  CBaseIntelligentPointer/CIntelligentPointer, CTCCarrying, CThing (+CVectorMap), CEnvironment, CWorld, CEnvironmentSpellEffectInterpolator
  (draft calls it CEnvironmentSpellEffects), CRGBFloatColour (draft CRGBAFloat), CThingGraphicArgPasser (draft CScreenFilterBypassThing).

## 5. Callee-body visibility
- GetSubDef<CSpecialEffectsDef> 0x669580: template body visible in TU but emitted out of line and called -- PlayEffect 0x79CC80 copies the
  body verbatim (template definition in the row) -> copy it.
- GetEffectFromString 0x669D30: "same-TU noinline body", opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY (CSpecialEffectsDef.h) --
  PlayEffect uses it; its frame-slot sharing depends on the visible body (lesson Q32).
- IS_THING_ALIVE 0x79DC20: COMDAT right after this function, only caller UnPause -> template called out of line;
  FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (ThingAlive.h; "visible body tells VC7.1 the callee keeps no reference -> frame word shared") or
  plain FABLE_RETAIL_IS_THING_ALIVE (PlayEffect uses the plain one with CThing*; check which one emits the call).
- CIntelligentPointer<CParticleEmitter>(T*) 0x655318: out of line -> FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE; operator= is INLINE
  here (GetPItem+SetPItem expanded) -> FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (NOT ..._ASSIGN_OUTOFLINE).
- CRightHandedSet ctor inline (6 in-place stores) -> FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE (CRightHandedSet.h line 51) with
  CRightHandedSet(C3DVector(0,0,1), C3DVector(0,1,0)) (FABLE_RETAIL_C3DVECTOR inline ctor).
- CThingGraphicArgPasser(CThing*, bool) ctor inline (header line ~97); vector push_back / ~vector out of line (STLport; stlport/fable_containers.h).
- CDefString -> CCharString: retail expands GDefStringTable.GetString(TablePos) in place -> either explicit `GDefStringTable.GetString(...)`
  or implicit conversion `PlaySoundW(PDef->LoopSound, true)` with FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING (CDefString.h l.62).
- GetWorld 0x686A70 out of line (CTCBase.h); GetEnvironment/GetSpellEffectInterpolator inline accessors (Pause's opt-ins).

## 6. Siblings (copy idioms)
- 00/79/CTCBulletTime_Pause_0079cf30.cpp (mirror function!): macros FABLE_RETAIL_BASE_OBJECT, CTCBASE, CINTELLIGENTPOINTER,
  CINTELLIGENTPOINTER_ASSIGN, CPARTICLEEMITTER, CPARTICLEEMITTER_KILL, CTCBULLETTIME, CTCBULLETTIME_MEMBERS, CTCCARRYING_BYPASSFILTERS,
  CTHING, CTHING_GETTC_EXPANDED, CWORLD, CWORLD_ENVIRONMENT, CENVIRONMENT_SPELLEFFECTINTERPOLATOR, CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADEOUT;
  no pragmas. Idiom: `if (m_tpLoopingEffect.Get() && !m_tpLoopingEffect.Get()->IsDead())`, `GetWorld().GetEnvironment().GetSpellEffectInterpolator().X(...)`,
  `CTCCarrying* carrying; if (GetParentThing().GetTC(TCI_CARRYING, &carrying)) ...`.
- 00/79/CTCBulletTime_PlayEffect_0079cc80.cpp: GetSubDef template body + GETEFFECT_BODY + CDefPointer<const CSpecialEffectsDef> effects;
  long effect_index = 0; `if (PDef->GetSubDef(&effects) && effects->GetEffectFromString(name, effect_index))`; CIntelligentPointer temp
  takes the dead effects slot (same as here). Full macro list in that file (CCHARSTRING, CCLASS, DEF_CLASS_BASE, CDEFINITIONMANAGER,
  CDEFSTRINGTABLE, CRESOURCELIST, STLP_MAP, CSPECIALEFFECTSDEF_TYPENAME, CSPECIALEFFECTSDEF_GETEFFECT_BODY, IS_THING_ALIVE, CTCBULLETTIME_PLAYEFFECT...).
- 00/79/CTCBulletTime_CTCBulletTime_0079d110.cpp (actually ~CTCBulletTime): `CCharString sound = GDefStringTable.GetString(PDef->EndSoundStringID); PlaySoundW(sound, 1);` (local model; old style).
- 00/64/NPlayerGuiCDrawQuickAccessIcons_DrawEffectAtPos_0064546f.cpp: Create2DEmitter / GetParticleEmitterManager / CRightHandedSet /
  CIntelligentPointer<CParticleEmitter> CTOR_OUTOFLINE idioms (but that TU is /Oy- + optimize("s") -- do NOT copy its pragmas).
- 00/79/global_ISTHINGALIVE_0079dc20.cpp (the out-of-line IS_THING_ALIVE instance used here).
- Unlanded twin: the CTCBulletTime ctor 0x0079D360 (1141 bytes, manifest-gaps) does the same SLOWTIME + bypass push_back (0x0079D6B0).

## 7. Old draft model errors (vs disassembly)
- Names: g_DefStringTable->GDefStringTable, PlaySound->PlaySoundW (const CCharString&, bool), CalcAndLookup->GetEffectFromString(const CCharString&, long&) const,
  GFAYToVY/GFAXToVX->GFVYToAY/GFVXToAX (__fastcall), g_ScreenCentreX/Y->VIRTUAL_SCREEN_WIDTH/HEIGHT, CParticleEmitterManager::Get->
  NParticleEngine::GetParticleEmitterManager() (reference), CEnvironmentSpellEffects->CEnvironmentSpellEffectInterpolator,
  SetPersistent->SetAlwaysUpdate(true), IsFinished->IsDead, member IsThingAlive->free IS_THING_ALIVE, CScreenFilterBypassThing->
  CThingGraphicArgPasser, CRGBAFloat->CRGBFloatColour.
- Types: GetSubDef is CDefClassBase const template taking CDefPointer<const T>*; the draft's local CDefPointer/CSpecialEffectsDef lacks the
  visible bodies -> the residue "effects def pointer not kept in ebp" (NOTES.md l.51) is exactly the Q32 visibility lesson: with the
  GetSubDef template body + GETEFFECT_BODY visible (PlayEffect's recipe), VC7.1 knows the callees keep no reference and enregisters effects.Object in ebp.
- ScreenFilterFadeTo: vector passed by const reference (draft: pointer, same bytes but wrong signature); first arg float 0.0f; priority 2 enum.
- Create2DEmitter: position by const reference; draft takes `&C2DVector(...)` of a temporary (non-standard).
- CTCBulletTimeImpl subclass + casts -> use CTCBulletTime members directly; draft lacks UnPause/PlaySoundW declarations in the shared header.
- Control-flow form matches retail (&& short-circuit, nested if for IS_THING_ALIVE, vector local after the effect block, GetTC at end).

## 0x00977E00

# DOSSIER 0x00977E00 -- CAIStateGroup_Guildmaster::Update (732 bytes)

## 1. Retail shape (xdis 00977e00 0x2ec)
- thiscall, plain `ret` (no args), 4 exits (two early-terminate `ret`s at 0x977ede / 0x9780bf, common tail 0x9780d1/0x9780d2).
- NO frame pointer (`sub esp,0x364 ; push ebx ; push esi ; push edi`; `push ebp` only at 0x977edf on the long path) -> /Oy (default), no optimize("y",off).
- NO SEH prologue despite CDefPointer + action temporaries with dtors -> TU built WITHOUT /EHsc (lane default `// Flags: none`).
- 0x364 frame: action temporaries NOT overlapped across the switch cases (case0 temp @ pre-ebp esp+0x28, case1 @ +0x14c, case2 @ +0x260; 0x124/0x114 apart). Shared tail `lea ecx,temp ; jmp 0x978077 ; call ~CCombatActionBase` (tail-merged dtor call for the 3 cases).
- Frame words (pre-`push ebp` offsets): boss ptr local @ esp+0xC (uninitialised when !HasTC(TCI_BOSS): `mov edi,[esp+0xc]`), GetTC key local AND later the CDefPointer<const CGuildMasterDef> @ esp+0x10 (same slot: key is block-scoped inside GetTC), C3DVector pos @ esp+0x18.
- The def pointer: `lea edx,[def] ; push edx ; mov [def],0 ; call GetSubDef` (zero AFTER the push). Early-exit path releases def from memory (`mov ecx,[esp+0x10] ; dec [ecx+4] ; jne ; call [vt+4]`); long path caches def.Object in ebp at 0x977f0f (AFTER the 2nd CountLivingHelpers call), uses `[ebp+0x2c]` / `[ebp+0x28]` and releases through ebp at both tail exits.
- ebx = creature (first GetCreature) kept until GetSubDef: `mov ecx,[ebx+0x70]` = CThing::PBaseDef raw (no addref) -> `GetCreature().PeekBaseDef().GetSubDef(&def)` (CThing.h:260 PeekBaseDef inline), NOT GetPBaseDef() (by value would addref).
- ebx then reloaded with constant 0x80000 (= bit 19 of UsingComponents word +0x30 = TCI_PHYSICAL_SHIELD_SPELL 147) and reused at 0x977f1b `test [eax+0x30],ebx` -- the second HasTC(147) test then GetTC lookup WITHOUT a re-test (CSE of GetTC's inner HasTC).
- Seconds idiom (x2): `call GetWorld (result unused) ; mov eax,[0x1375550] ; mov [tmp],eax ; call CWorld::GetFrame ; sub eax,[esi+0x34|0x38] ; fild ; fidiv [tmp]`. Order GetWorld -> fps -> GetFrame = the BriarRoseWillUse 0x00956FE0 idiom `CWorld* world=&GetWorld(); long fps=CMainGameComponent::GetConstantFPS(); ... world->GetFrame()` (static called through the object). First value spilled to float (fstp, a call intervenes: CountLivingHelpers), second compared straight from st (no named float needed / or optimised away).
- Comparisons `fcomp [def+X] ; fnstsw ; test ah,0x41 ; jne skip` = `if (secs > def->X)`.
- switch: `AttackNum % 3` signed (cdq/idiv), `sub edx,0 ; je case0 ; dec ; je case1 ; dec ; jne end`.

## 2. Identity
- Name: CAIStateGroup_Guildmaster::Update, mangled leaf `?Update@CAIStateGroup_Guildmaster@@UAEXXZ` (public virtual void, no params, non-const).
- Evidence: (a) ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv 017a8410 `?Update@CAIStateGroup_Guildmaster@@UAEXXZ`; (b) ghidra_out/egor_pdb_names.tsv has the same leaf (count 1); (c) rebuild/manifest/rtti-vtables.tsv: CAIStateGroup_Guildmaster vtable 0x0128D444 slot 12 = 0x00977E00 (xref.py: only ref is .rdata 0x0128D474 = slot 12); slot 12 is Update for other state groups (SeekScriptTarget 0x0093C030 "vtable slot 12"); (d) landed IsStillRunnable 0x009779A0 notes "callers CAIStateGroup_Guildmaster::Update 0x00977E00 (x2)". Manifest label right modulo the doubled class prefix.
- NOTE egor_pdb_names.tsv addresses are Ego_r addresses, not retail (e.g. 0x00978580 there is a Dragon Clone) -- don't map by address.

## 3. Callees (landed file / landed name / PDB) -- draft spelling problems flagged with !!
| retail | landed row | landed / PDB name |
|---|---|---|
| 0x0040F020 | rebuild/src/compiled/00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (non-const), via CThing::GetTC expanded (FABLE_RETAIL_CTHING_GETTC_EXPANDED) |
| 0x00978190 | 00/97/CDefClassBase_GetSubDefCGuildMasterDef_00978190.cpp | `??$GetSubDef@VCGuildMasterDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCGuildMasterDef@@@@@Z`. !! draft `CGuildmasterDef` (lower m) and non-const GetSubDef(CDefPointer<CGuildmasterDef>*) -> must be CGuildMasterDef, const method, CDefPointer<const CGuildMasterDef>* |
| 0x007AEEC0 | 00/7a/CTCBoss_CountLivingHelpers_007aeec0.cpp | `?CountLivingHelpers@CTCBoss@@QAEJXZ` (non-const long). !! draft `CountLivingHelperCreatures() const` |
| 0x00956760 | 00/95/CCombatActionWillShieldInto_CCombatActionWillShieldInto_00956760.cpp | `??0CCombatAction_WillShieldInto@@QAE@AAVCThingCreatureBase@@MM@Z` (owner&, shieldUpTime 1800.0f, shieldHealth 5.0f). !! draft `CCombatAction_WillEnflameInto` with pointer arg -- WRONG CLASS |
| 0x0062E900 | 00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp | `CCombatActionBase::~CCombatActionBase()` (temp dtor for every action) |
| 0x0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | `?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z` (const CCreatureActionBase&, bool) protected. !! draft takes pointer, returns long; pass the temporary by const ref, `false` |
| 0x009779A0 | 00/97/CAIStateGroupGuildmaster_IsStillRunnable_009779a0.cpp | `?IsStillRunnable@CAIStateGroup_Guildmaster@@IAE_NXZ` (protected) |
| 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | `?Terminate@CAIStateGroupBase@@QAEXXZ` |
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | `?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ` -> CWorld& const. !! draft `GetPWorld()` returning CWorld* |
| 0x0049D870 | 00/49/CWorld_GetFrame_0049d870.cpp | `?GetFrame@CWorld@@SIJXZ` static __fastcall long. !! draft `GetWorldFrame` |
| 0x0079E300 | 00/79/CTCPhysicalShieldSpell_CancelMove_0079e300.cpp | `?CancelMove@CTCPhysicalShieldSpell@@QAEXXZ` |
| 0x00977820 | 00/97/CAIStateGroupGuildmaster_GetBetterTeleportPos_00977820.cpp | `?GetBetterTeleportPos@CAIStateGroup_Guildmaster@@IAE_NAAVC3DVector@@@Z` (C3DVector&, protected). !! draft takes C3DVector* (same bytes, wrong mangling). (landed row models it on a `CAIStateGroupGuildmasterMethods` helper class) |
| 0x008BCFB0 | 00/8b/CCreatureActionTeleportOut_CCreatureActionTeleportOut_008bcfb0.cpp | PDB `??0CCreatureAction_TeleportOut@@QAE@AAVCThingCreatureBase@@AAVCThing@@AAVC3DVector@@_N@Z` (refs, non-const C3DVector&); landed row spells pointers + const C3DVector& (landed spelling disagrees with PDB -> NAME_FIXES candidate); engine/CCreatureAction_TeleportOut.h under FABLE_RETAIL_CCREATUREACTION_TELEPORT has the PDB-ref ctor. Args: (creature, *PEnemy.GetPItem(), pos, true) |
| 0x00977B10 | 00/97/CAIStateGroupGuildmaster_FindAndStoreBodyguards_00977b10.cpp | `?FindAndStoreBodyguards@CAIStateGroup_Guildmaster@@IAEXXZ` |
| 0x00978510 | 00/97/CCombatActionGuildmasterShieldFollowersInto_..._00978510.cpp | CCombatAction_Guildmaster_ShieldFollowersInto(CThingCreatureBase&) -- case 0. !! draft "GuildmasterAttack0" |
| 0x009784A0 | 00/97/CCombatActionGuildmasterHealFollowersInto_..._009784a0.cpp | `??0CCombatAction_Guildmaster_HealFollowersInto@@QAE@AAVCThingCreatureBase@@@Z` -- case 1. !! draft "Attack1" |
| 0x00978580 | 00/97/CCombatActionGuildmasterSpeedUpFollowersInto_..._00978580.cpp | CCombatAction_Guildmaster_SpeedUpFollowersInto(CThingCreatureBase&) -- case 2. !! draft "Attack2" |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (returns CBaseObject*) |
| vslot +4 | -- | def pointee Delete (CDefPointer release), inline via CDefPointer.h |
| global 0x01375550 | -- | `?ConstantFPS@CMainGameComponent@@1JA` via inline `CMainGameComponent::GetConstantFPS()` (engine/CMainGameComponent.h:189; fable_game.h:73). !! draft `g_framesPerSecond` |
| GetCreature | inline | `?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ` -> CThingAICreature& (FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE) |

## 4. Members
- this (CAIStateGroup_Guildmaster, retail 0x3c; struct_layouts_egor.tsv): AttackNum long +0x14 (rw), PEnemy CIntelligentPointer<CThingCreatureBase> +0x18 (GetPItem), LastWillFrame long +0x34 (r, w=GetFrame()), LastTeleportFrame long +0x38 (r, w). PLastTeleportMarker +0x20 / TeleportMarkers +0x28 unused here. Shared header rebuild/include/engine/CAIStateGroup_Guildmaster.h is GENERATED and padded (base flattened, PEnemy opaque bytes, no opt-ins, no methods) -- every landed sibling models the class locally (IsStillRunnable, OnActivate). Base: engine/CAIStateGroupBase.h (PBrain +0x04, Terminated +0x10; opt-ins FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE, _AIACTIONS (GetWorld decl), _PERFORMACTION (AIAction_PerformActionAndWait decl)).
- PBrain->PCreature +0x18 (engine/CAIBrain.h, FABLE_RETAIL_CAIBRAIN_GETCREATURE).
- CThing (engine/CThing.h): UsingComponents bitset +0x20 (words +0x30 bit19=TCI_PHYSICAL_SHIELD_SPELL 147, +0x34 bit13 `test ah,0x20`=TCI_BOSS 173), Components CVectorMap +0x44, PBaseDef CDefPointer<const CThingBaseDef> +0x70. Opt-ins FABLE_RETAIL_CTHING_GETTC_EXPANDED (+ FABLE_RETAIL_STLP_BITSET_INDEX as IsStillRunnable).
- CGuildMasterDef (struct_layouts_egor.tsv, size 48): MinSecondsBetweenTeleport float +0x28, MinSecondsBetweenWillUse float +0x2c. !! draft names TeleportInterval / WillInterval. No engine/CGuildMasterDef.h; the landed GetSubDef row 0x00978190 defines `class CGuildMasterDef : public CSubDefClassBase` locally (engine/CSubDefClassBase.h, FABLE_RETAIL_DEF_CLASS_BASE).
- CDefPointer: engine/CDefPointer.h (Release opt-ins: _RELEASE_COUNTREF, _RELEASE_POINTEE, _RELEASE_SELFDELETING_POINTEE, _RESET, _POINTER_CTOR_ADOPT).
- Locally modelled by the draft although a shared header exists: C3DVector (engine/C3DVector.h), CInterfaceMap (engine/CVectorMap.h), CThing (engine/CThing.h), CThingCreatureBase (engine/CThingCreatureBase.h), CTCBoss (engine/CTCBoss.h, FABLE_RETAIL_CTCBOSS_LIVINGHELPERS declares CountLivingHelpers), CTCPhysicalShieldSpell (engine/CTCPhysicalShieldSpell.h), CBaseIntelligentPointer (engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER), CWorld (engine/CWorld.h:142 GetFrame), CDefPointer (engine/CDefPointer.h), CDefClassBase (engine/CDefClassBase.h), CCombatActionBase (engine/CCombatActionBase.h), CCombatAction_WillShieldInto (engine/CCombatAction_WillShieldInto.h, no opt-ins), CCreatureAction_TeleportOut (engine/CCreatureAction_TeleportOut.h, FABLE_RETAIL_CCREATUREACTION_TELEPORT), CAIBrain, CAIStateGroupBase, CAIStateGroup_Guildmaster, CMainGameComponent.

## 5. Callee visibility
- GetSubDef<CGuildMasterDef> 0x00978190 is a template COMDAT emitted right after this function's neighbourhood (same TU instantiates it). engine/CDefClassBase_GetSubDef.h offers FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE: visible body, kept out of line -- VC then knows only *out is written (row 0x0066BC10 used it to keep the def in a register). Candidate fix for the "def zero after push / ebp cache" residue. Briar Rose rows 0x00956FE0 used a same-TU noinline stand-in instead.
- Same-TU (defined earlier, adjacent retail addresses): Initialise 0x977730, OnReplace 0x977760, Cleanup 0x977770, IsRunnable 0x9777A0, GetBetterTeleportPos 0x977820, IsStillRunnable 0x9779A0, OnDeactivate 0x977A30, FindAndStoreBodyguards 0x977B10, OnActivate 0x977C40 -- all called out of line, so either declare-only or define with __declspec(noinline)/auto_inline(off) if a visible body changes scheduling (IsStillRunnable/GetBetterTeleportPos/FindAndStoreBodyguards are callees here).
- Action ctors 0x978510/0x9784A0/0x978580 (and their Clone/dtors 0x9785F0..0x9786B0) sit right after this function: inline ctors emitted out of line in this TU. 0x00956760 WillShieldInto ctor and 0x008BCFB0 TeleportOut ctor live in other TUs.
- LowerBound 0x0040F020 always out of line (GETTC_EXPANDED).
- CountLivingHelpers, GetWorld, Terminate, GetFrame, AIAction_PerformActionAndWait, CancelMove, GetPItem, ~CCombatActionBase: out of line, other TUs.

## 6. Sibling landed rows (idioms)
- 00/97/CAIStateGroupGuildmaster_IsStillRunnable_009779a0.cpp: `#define FABLE_RETAIL_STLP_BITSET_INDEX` + engine/CVectorMap.h, `FABLE_RETAIL_CTHING_GETTC_EXPANDED` + engine/CThing.h, `FABLE_RETAIL_CINTELLIGENTPOINTER` + engine/CIntelligentPointer.h; local CAIBrain/CAIStateGroupBase/CAIStateGroup_Guildmaster with `CIntelligentPointer<CThingCreatureBase> PEnemy` at +0x18; PeekTC(TCI_TARGETING,&p).
- 00/97/CAIStateGroupGuildmaster_OnActivate_00977c40.cpp: FABLE_RETAIL_CCHARSTRING, GETTC_EXPANDED, `#pragma auto_inline(off)` around a same-TU noinline stand-in; `GetCreature()->GetTC(TCI_ENTITY_MODE_MANAGER,&modes)`.
- 00/97/CAIStateGroupGuildmaster_FindAndStoreBodyguards_00977b10.cpp: FABLE_RETAIL_CCHARSTRING, GETTC_EXPANDED, auto_inline(off/on).
- 00/97/CDefClassBase_GetSubDefCGuildMasterDef_00978190.cpp: FABLE_RETAIL_DEF_CLASS_BASE, engine/CSubDefClassBase.h + engine/CDefPointer.h, CGuildMasterDef model.
- 00/95/CAIStateGroupBriarRoseWillUse_Update_00956fe0.cpp (same pattern state-group Update): GetWorld/GetConstantFPS/GetFrame ordering idiom, `AIAction_PerformActionAndWait(&Temp(...), false)`.
- 00/93/CAIStateGroupSeekScriptTarget_Update_0093c030.cpp: full shared-header Update (slot 12) opt-in set: CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CTHING_GETTC_EXPANDED, CVECTORMAP_INLINE_LOWERBOUND, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE(+_FINISHACTION/_ACTIONQUERY/_FREETOFINISH), CTHINGAICREATURE, CINTELLIGENTPOINTER(+_COMPARE_NULL_INT/_CONVERT_MUTABLE), IS_THING_ALIVE, C3DVECTOR.
- None of the Guildmaster siblings carry a `// Flags:` line (default flags, no /EHsc).

## 7. Old draft model errors vs disassembly
1. Wrong callee names/classes: WillEnflameInto -> WillShieldInto; Attack0/1/2 -> Guildmaster_ShieldFollowersInto / HealFollowersInto / SpeedUpFollowersInto; CountLivingHelperCreatures -> CountLivingHelpers; GetPWorld -> GetWorld (CWorld&); GetWorldFrame -> GetFrame; g_framesPerSecond -> CMainGameComponent::GetConstantFPS(); CGuildmasterDef -> CGuildMasterDef (members MinSecondsBetweenWillUse +0x2c / MinSecondsBetweenTeleport +0x28).
2. Def access: draft `creature->GetDef()` on a local m_pDef; real is PBaseDef (CDefPointer<const CThingBaseDef>) via PeekBaseDef(); GetSubDef is const and fills CDefPointer<const CGuildMasterDef>.
3. Def pointer zero-after-push / ebp late cache (the recorded 729v732 residue): try the shared CDefPointer (default ctor `Object(0)`) + the visible GETSUBDEF_NOINLINE body, rather than the hand CDefPointer struct.
4. fps/GetWorld order: draft reads fps BEFORE GetPWorld(); retail calls GetWorld first, then reads fps, then GetFrame -> `CWorld& w = GetWorld(); long fps = GetConstantFPS(); (float)(w.GetFrame()-LastWillFrame)/fps`.
5. key/boss frame words swapped (boss must be the lower slot esp+0xC, key/def share +0x10): the header GetTC(id,&p) (key local declared inside the test block) instead of the draft's local GetInterface; declare `CTCBoss* boss;` before the def.
6. AIAction_PerformActionAndWait takes const CCreatureActionBase& -- pass temporaries directly (no `&Temp(...)`), return EAIActionReturn ignored.
7. Shield cancel: `if (GetCreature().HasTC(TCI_PHYSICAL_SHIELD_SPELL)) { CTCPhysicalShieldSpell* s; GetCreature().GetTC(TCI_PHYSICAL_SHIELD_SPELL,&s); s->CancelMove(); }` -- retail reloads the creature (`mov ecx,[esi+4] ; mov eax,[ecx+0x18]`) once and drops the inner re-test.
8. Control flow otherwise matches: early `Terminate(); return;` after first IsStillRunnable false; `if (boss->CountLivingHelpers()==0) {...} else if (sinceWill > def->MinSecondsBetweenWillUse) { if (secsSinceTeleport > def->MinSecondsBetweenTeleport && GetBetterTeleportPos(pos)) {teleport; LastTeleportFrame=GetWorld().GetFrame();} else { FindAndStoreBodyguards(); switch(AttackNum%3){0,1,2}; ++AttackNum; LastWillFrame=GetWorld().GetFrame(); } }` then `if (!IsStillRunnable()) Terminate();`. Teleport: `++AttackNum` is `mov eax,[esi+0x14]; inc; mov [esi+0x14],eax` (scheduled around `mov ecx,esi`). Teleport ctor args: creature (loaded into edi BEFORE the pushes), *PEnemy.GetPItem(), pos, true.

## 0x00854DD0 (not landed)

# DOSSIER 0x00854DD0 -- CCreatureAction_SpiritAttack::PerformAction (720 bytes)

## 1. Retail shape (xdis 00854dd0 0x2e0)
- __thiscall, plain `ret` (two duplicated epilogues 0x855085 / 0x855098, tail-duplicated OnStrike call), `ret` ends at 0x0085509F; 0x008550A0 is the next row.
- NO frame pointer (`sub esp,7Ch; push ebx/ebp/esi/edi`) -> default /Oy; no SEH prologue even though temporaries with dtors exist
  (CIntelligentPointer temp, CHitParameters) -> no EH state => build without /EHsc unwinding (like siblings, `// Flags: none`).
- Registers: edi = this (spilled to [esp+10h] for the CreateAttackEffect call), ebp = &PCreature (this+8, every GetPItem re-called,
  never cached), esi = target, ebx = 8 constant (GetTC(8) bit, then type-flag test `test [esi+6Ch],bl`), later ebx = CTCCoopSpirit*.
- Frame: [esp+14h] TC out-pointer / CIntelligentPointer temp (vptr 0x01238C6C) / float temps; CHitParameters at [esp+2Ch] (0x60 bytes).
- Normalise: fld X-diff, Y-diff, Z-diff; `fst [esp+28h]; fmul [esp+28h]` (Z spilled) then Y*Y, X*X => sum Z*Z+Y*Y+X*X;
  GFFastOneOverSqrt inline (0x17C, table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C), factor = ((3-r*r*sq)*r)*0.5.

## 2. Identity (manifest label CORRECT)
- vtable CCreatureAction_SpiritAttack 0x01275D74 slot 3 = 0x00854DD0 (rebuild/manifest/rtti-vtables.tsv); slot 3 = PerformAction in
  every CCreatureAction (SpiritReturnToHero 0x00854720 slot 3, SpiritHitResponse 0x008550B0 slot 3, both landed as PerformAction).
- Ego_r.pdb (egor_pdb_names.tsv 0x00844350) and FableWin.pdb (_fablewin_api.tsv 0x01826DEC):
  `?PerformAction@CCreatureAction_SpiritAttack@@UAEXXZ` -> `public: virtual void __thiscall PerformAction(void)`.
- Only xref: the vtable (.rdata 0x01275D80). No direct callers.

## 3. Callees (landed file / landed name / PDB name; DRAFT MISMATCHES flagged)
| addr | landed row | name to use | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (via CThing::GetTC<T> forceinline) | draft CInterfaceMap::LowerBound -- WRONG, local |
| vslot 33 (+0x84) on TCI_TARGETING(8) TC | -- | CTCTargetingBase::GetPTargetedThing() const (CTCTargetingBase.h l.34/83; landed 0x00802610 CTCTargetingPlayer::GetPTargetedThing; 007f9b90 / 0064c869 cite slot 33) | draft CTCTargeting::GetTarget -- WRONG |
| 0x00A01B10 / 0x00A01B90 / 0x00A01C10 | CBaseIntelligentPointer ctor / SetPItem / dtor | temp `CIntelligentPointer<CThing>(target)` then `PTarget = temp` | ok in behaviour |
| 0x00670000 | 00/67/CTCPhysicsBase_GetAirResistance_00670000.cpp (MISLABELLED) | CTCCoopSpirit::GetMeleeTargetRange() const (`?GetMeleeTargetRange@CTCCoopSpirit@@QBEMXZ`, Ego_r+FableWin); body `mov eax,[ecx+28h]; fld [eax+38h]` = PCoopSpiritDef(+0x28)->MeleeTargetRange(+0x38, struct_layouts_egor CCoopSpiritDef) | draft GetRange -- WRONG; NAME_FIXES candidate for the landed row |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp, Leaf ?GetPos@CThing@@QBEABVC3DVector@@XZ | CThing::GetPos() const, returns const C3DVector& | draft GetPosition returning pointer -- WRONG |
| 0x006A4DF0 | 00/6a/CThingPhysical_IsInRange_006a4df0.cpp | CThingPhysical::IsInRange(const C3DVector&, float) const (bool) | draft non-const pointer arg |
| 0x008548B0 | NOT landed (DIFFER in hand283) | CCreatureAction_SpiritAttack::CreateAttackEffect() (private, PDB AAEXXZ) | ok |
| 0x00789238 | 00/78/CHitParameters_CHitParameters_00789238.cpp | CHitParameters(NHitLocations::CHitLocation*, CThing*, CThing*, const C3DVector&, const C3DVector&, bool knockDown, bool blockable, bool flourish, bool blockCounter), ret 24h | draft `void* location`; arg values (0,creature,0,pos,dir,false,true,false,false) correct |
| 0x00868CA0 | 00/86/CHitParameters_AddDamageWithAttribute_00868ca0.cpp | CHitParameters::AddDamageWithAttribute(long/EDamageAttribute, float) | ok (attr 0, 1.0f) |
| 0x00789358 | 00/78/CHitParameters00789358_..._00789358.cpp | CHitParameters::~CHitParameters() (CHitParameters.h FABLE_RETAIL_CHITPARAMETERS_CTOR declares it) | ok |
| vslot 65 (+0x104) on target | -- | CThingGameObject::OnHit(CHitParameters&) (see 00798770 FinishBeingForcePushed `owner->OnHit(hit)` slot 65) | draft CThing::Hit -- WRONG name |
| 0x0066FB10 | 00/66/CTCCoopSpirit_AddExperience_0066fb10.cpp | CTCCoopSpirit::AddExperience(long) (landed; PDB also has AddScore(long); body adds to +0x10 which Ego_r names `Score` -- ambiguous, keep landed) | ok |
| 0x006707E0 | 00/67/CTCCoopSpirit_OnStrike_006707e0.cpp | CTCCoopSpirit::OnStrike() (reads OnStrikeParticle +0x2c; PDB has both OnHit and OnStrike) | draft OnHit -- WRONG |

## 4. Members
- this+0x08 PCreature: CCreatureActionBase, CIntelligentPointer<CThingCreatureBase> -- engine/CCreatureActionBase.h opt-in
  FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE (+ FABLE_RETAIL_CINTELLIGENTPOINTER first).
- this+0xA8 PTarget (CIntelligentPointer<CThing>), +0xB0 PParticleEmitter: engine/CCreatureAction_SpiritAttack.h (opaque bytes,
  offsets asserted); FrameUpdate 00854b90 redeclares `class CCreatureAction_SpiritAttack : public CCreatureActionBase { CIntelligentPointer<CThing> PTarget, PParticleEmitter; }`.
- thing+0x20 UsingComponents, +0x44 Components (CThing.h, GetTC<T> FABLE_FORCEINLINE; FABLE_RETAIL_CTHING), bit 8 -> TCI_TARGETING, bit 0xC7 -> CTCCoopSpirit.
- thing+0x60 PhysicsTC (CThing.h, GetPhysicsTC() const) -> CTCPhysicsBase::Radius +0x34 (GetRadius(), CTCPhysicsBase.h; FABLE_RETAIL_CTCPHYSICSBASE).
- thing+0x6C ThingTypeFlags, bit 8 = creature bit (CThingCreatureBase mask 15 vs CThingGameObject 7): `GetTypeFlags() & 8` (CThing.h l.423),
  same idiom as landed 008c0c00 / 008c48a0 / 008c92c0.
- thing+0x91 bit0 Dead: `IsDead()` (CThing.h l.280).
- CHitParameters (CHitParameters.h, 0x60): Blocked +0x26 / HitNegated +0x27 -> IsBlocked() / IsHitNegated() (draft "Deflected" wrong).
  The 9-arg ctor and AddDamageWithAttribute are NOT declared in the shared header (no opt-in); landed 00798770 declares them locally.
- CTCCoopSpirit: engine/_quarantine/CTCCoopSpirit.h (fields only: PCoopSpiritDef +0x28, Score +0x10); no method opt-ins -> declare GetMeleeTargetRange/AddExperience/OnStrike + StatGetInterfaceType 0xC7 (FrameUpdate 00854b90 idiom).
- Draft models LOCALLY although shared headers exist: C3DVector, CThing, CTCPhysics(Base), CHitParameters, CBaseIntelligentPointer/CIntelligentPointer,
  CCreatureAction_SpiritAttack, CInterfaceMap (=CVectorMap), CTCTargeting (=CTCTargetingBase).

## 5. Visibility
- GetTC<T>, IsDead, GetTypeFlags, GetPhysicsTC, GetRadius, C3DVector ops, Normalise, GFFastOneOverSqrt: inline (headers).
- Out of line (not visible): GetPItem/SetPItem/ctor/dtor (0xA01Bxx), LowerBound 0x0040F020, GetPos, IsInRange, GetMeleeTargetRange (own TU 0x0067xxxx),
  CHitParameters ctor/dtor/AddDamage, AddExperience, OnStrike.
- Same TU, earlier: CreateAttackEffect 0x008548B0 (address < row; called, not inlined -- fine as an extern member decl).
- C3DVector opt-ins: FABLE_RETAIL_C3DVECTOR + FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX gives `(Z*Z+Y*Y+X*X)` with the default Normalise
  (= retail's `fst; fmul` Z spill pattern, cited at 0x00A587B2 in the header). Newton variants: GFFASTONEOVERSQRT_NEWTON_{GROUPED,SQUARE_PAREN,STEP_PAREN,PAREN}
  (no-code paren tuples that move the scheduling cut -- the residue here IS an x87 schedule/register-role issue, try these).
  operator-: default or FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP.

## 6. Siblings
- 00/85/CCreatureActionSpiritReturnToHero_PerformAction_00854720.cpp (`// Flags: none`, FABLE_RETAIL_BASE_OBJECT, _CCREATUREACTIONBASE,
  _CCREATUREACTIONBASE_PCREATURE, _CTHING, _CINTELLIGENTPOINTER, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTCPHYSICSBASE;
  `PCreature->GetPos()`, `PCreature->GetPhysicsTC()`).
- 00/85/CCreatureActionSpiritAttack_FrameUpdate_00854b90.cpp (same class; GetTC with `if(i==map.end()||type<i->first) i=map.end()`,
  CTCCoopSpirit wrapper over _quarantine header with StatGetInterfaceType 0xC7).
- 00/79/CTCForcePushed_FinishBeingForcePushed_00798770.cpp (same CHitParameters 9-arg ctor + AddDamageWithAttribute + OnHit slot 65).
- 00/85/CCreatureActionSpiritAttack_FinishAction_008547d0.cpp, _Initialise_008545f0.cpp, _Clone_00855230.cpp.

## 7. Old draft model errors
- Names: GetPosition->GetPos (reference), GetRange->GetMeleeTargetRange, OnHit->OnStrike, Hit->OnHit, GetTarget->GetPTargetedThing,
  Deflected->HitNegated, m_flags6c->ThingTypeFlags/GetTypeFlags()&8, m_flags91->Dead/IsDead().
- Local stand-ins everywhere (verifier warns / strict rejects); FILL10 vtable padding.
- Normalise sum order (Z,Y,X) written by hand; use header ZYX opt-in. The recorded residue (702v720) is the x87 schedule (retail keeps
  X/Y on the stack, only Z spilled) and edi(this)/ebp(&PCreature) swap -- try PCreature->GetPos() through the header's CIntelligentPointer
  `operator->` (Get() -> GetPItem) rather than a GetCreature() helper, and the Newton paren variants.
- Control flow is right: GetTC(8) unconditionally used (uninitialised out-ptr if absent), `PTarget = CIntelligentPointer<CThing>(target)`,
  `if(target && !target->IsDead() && (target->GetTypeFlags()&8))`, IsInRange(PCreature->GetPos(), range + target->GetPhysicsTC()->GetRadius()),
  `if(!hit.IsBlocked() && !hit.IsHitNegated()) spirit->AddExperience(1)`, final fresh GetTC(0xC7)->OnStrike().

## 0x0075A7A0 (not landed)

# DOSSIER 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions (726 bytes)

## 1. Retail shape (xdis 0075a7a0 0x2e6)
- __thiscall, `ret 4` (one ref param). NO frame pointer (`sub esp,0x7c` first; no push ebp/mov ebp,esp) -> default /Oy.
- No SEH prologue, no EH state. Frame 0x7c (draft got 0x74 -> retail has ~2 more 12-byte-ish locals / slots).
- Register homes: edi=this whole function; esi=&config; ebx=&config.AngularMomentum (+0x48); ebp=&config.AngularVelocity (+0x9c),
  all hoisted before loop. Loop counter i at [esp+14], count (size() of CollidingVertexIndex, computed ONCE, signed `sar 2`, `jle`/`jl`)
  at [esp+1c], strength-reduced byte offset of CollisionNormal[i] at [esp+10] (+=0xc each pass; begin [edi+40] re-read every use).
- Loop top aligned with `lea ebx,[ebx]` filler after `jmp` (normal VC7.1 loop alignment, /O2 not /Os).
- Body order (per iteration):
  a) vertex = config.BoundingVertices[CollidingVertexIndex[i]]: X,Y fld'd directly, Z moved as INT via ecx to [esp+58] (a named/copied
     C3DVector local; copy ctor -> integer copy of one component), then r = vertex - config.CMPosition stored [20/24/28].
  b) config.AngularVelocity ^ r (cross, components computed X,Y,Z: w.Y*r.Z-w.Z*r.Y ...), + config.CMVelocity -> velocity (X kept in [68], Y on st, Z [64]).
  c) dot(velocity, normal) summed Z then Y then X: (v.Z*n.Z + v.Y*n.Y) + v.X*n.X  -> times (CoefficientOfRestitution + 1.0f [0x0122DED8 = 1.0f literal]) then fchs -> numerator [esp+18].
     i.e. `-( dot * (CoR + 1.0f) )` with CoR+1 evaluated after dot (fld [edi+a4]; fadd 1.0; fmulp; fchs).
  d) r ^ normal computed Z-comp first, then Y, then X (right-to-left ctor args) stored [2c/30/34]; call 0xA56530 (InverseWorldInertiaTensor * that, ecx=esi+0x6c, hidden result [esp+74]).
  e) (result ^ r) dotted with normal (Z,Y,X order again) + Body.OneOverMass [edi+a0]; `fdivr [esp+18]` -> j = numerator/denominator.
  f) impulse = normal * j (X,Y,Z: fld st0; fmul n.X ...), then impulse*OneOverMass added to CMVelocity (+0x3c/40/44) via two stored temps [3c][40].
  g) AngularMomentum += r ^ impulse (via ebx; temps [44][48][4c?]); call 0xA56530 with &AngularMomentum (ebx) -> result [esp+84] copied
     dword-by-dword (mov edx) into AngularVelocity (ebp) => plain struct assignment of the returned C3DVector.
- Epilogue: CollisionNormal.clear() inlined (STLport erase(begin,end) -> __copy random-access loop: n=(finish-finish)/12 via imul 0x2AAAAAAB, element copy, dec/jne; _M_finish=result)
  then CollidingVertexIndex.clear() inlined (__copy_trivial: `cmp eax,eax` last==first -> _M_finish=first; else memmove 0x00BFEAE6). Two separate ret paths.

## 2. Name / signature (two sources)
- Ego_r.pdb: ghidra_out/egor_pdb_names.tsv 00769a70 `?ResolveCollisions@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z`
- FableWin.pdb: ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv (017c00b5 / 025c1cb0) same mangled name.
- Caller: Simulate 0x0075AD50 (call site 0x0075AE68, landed rebuild/src/compiled/00/75/CTCPhysicsRigidBody_Simulate_0075ad50.cpp, declares ResolveCollisions 0x0075A7A0).
- => `private: void __thiscall CTCPhysicsRigidBody::ResolveCollisions(class CRigidBody::CRigidBodyConfig &)`. Manifest label CORRECT.
  NOTE scoping: CRigidBodyConfig is NESTED in CRigidBody (`V...@CRigidBody@@`), a `class` (V), param non-const ref, PRIVATE (A in AAE).
  Draft declares `struct CRigidBodyConfig` at global scope and a public member -> wrong mangled leaf. Copy CalculateVertices' model
  (`struct CRigidBody { class CRigidBodyConfig { public: ... }; ... }`).
- Not virtual (no vtable slot).

## 3. Callees
| target | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00A56530 (x2) | rebuild/src/compiled/00/a5/CMatrix3x4_operator_00a56530.cpp | CMatrix3x4::operator*(const C3DVector&) const, leaf ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z | same (row Leaf) | `C3DVector operator*(const C3DVector&) const` OK; members m_row0.. local (CMatrix3x4.h uses E11..E43 and declares NO operator*; Simulate/CalculateVertices model CMatrix3x4 locally) |
| 0x00BFEAE6 | rebuild/src/compiled/00/bf/global_memmove_00bfeae6.cpp | memmove (CRT) | CRT | draft `#include <string.h>` memmove OK; comes from STLport __copy_trivial |
No other calls / no virtual calls. C3DVector ops (^, -, +, +=, *float, dot) are all expanded inline.

## 4. Members
this (CTCPhysicsRigidBody, header rebuild/include/engine/CTCPhysicsRigidBody.h, opt-in FABLE_RETAIL_CTCPHYSICSRIGIDBODY (+FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCPHYSICSBASE)):
- +0x40 CollisionNormal  std::vector<C3DVector> (PDB name; retail branch real std::vector from stlport/fable_containers.h, protected)
- +0x4c CollidingVertexIndex std::vector<long>
- +0xa0 Body (CRigidBody) -- header keeps it OPAQUE `unsigned char Body[0x1b8]`; no CRigidBody header exists anywhere.
  +0xa0 = Body.OneOverMass (float), +0xa4 = Body.CoefficientOfRestitution (float) (struct_layouts_egor.tsv CRigidBody +0/+4).
  Draft models these as direct members OneOverMass/CoefficientOfRestitution of CTCPhysicsRigidBody -> wrong names (should be Body.OneOverMass ...).
config (CRigidBody::CRigidBodyConfig, struct_layouts_egor.tsv, retail offsets = PDB offsets up to +0xa8): CMPosition +0 C3DVector, Orientation +0xc CMatrix3x4,
  CMVelocity +0x3c, AngularMomentum +0x48, CMForce +0x54, Torque +0x60, InverseWorldInertiaTensor +0x6c CMatrix3x4, AngularVelocity +0x9c,
  BoundingVertices +0xa8 vector<C3DVector> (retail size 0xb4).
- Shared header usable: engine/C3DVector.h (X,Y,Z). Ops available: operator- / + (const), operator*(float) (const), operator^ (cross, under FABLE_RETAIL_C3DVECTOR,
  variants FABLE_RETAIL_C3DVECTOR_CROSS_{XYZ,XY,X}_GROUP), Dot() with variants FABLE_RETAIL_C3DVECTOR_DOT_GROUPED / _DOT_ORDERED_YXZ / _ZYX / _XYZ.
  Retail dot order here is Z,Y,X both times -> try FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX first (or default one-line X+Y+Z which VC often evaluates in reverse).
  FableWin confirms C3DVector has ??T (operator^ cross), ??D(float), ??H, ??G, ??Y(+=), ?Dot@C3DVector@@QBE?BMABV1@@Z, and global ?GFDotProduct@@YIMABVC3DVector@@0@Z.
  No float*C3DVector global in FableWin -> draft's `operator*(float, const C3DVector&)` and CrossProduct/DotProduct free helpers are invented; use `normal * j`, `a ^ b`, `.Dot()`.
- Classes the draft models LOCALLY that have shared headers: C3DVector (engine/C3DVector.h), CMatrix3x4 (engine/CMatrix3x4.h, but it lacks operator* -> landed siblings keep a local CMatrix3x4), vector (stlport/fable_containers.h std::vector), CTCPhysicsRigidBody (engine/CTCPhysicsRigidBody.h).
  CRigidBody / CRigidBodyConfig: no shared header (local model is required; copy CalculateVertices').

## 5. Visibility
- CMatrix3x4::operator* 0xA56530 is far away (other TU, CMatrix3x4 maths); called out of line -> declaration only.
- STLport vector clear()/erase/__copy/__copy_trivial: inline expanded here (fable_containers.h default; do NOT define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE / ERASE_RANGE_OUTOFLINE). size()/operator[] inline.
- Same TU neighbours (CTCPhysicsRigidBody.cpp): SetPhysicsMeshBankIndex 0x0075A530 before, ctor 0x0075AA80, CheckForCollisions 0x0075AAF0, Simulate 0x0075AD50 after;
  out-of-line STLport COMDATs for vector<C3DVector> (fill 0x0075B430, copy 0x0075B560, copy_backward 0x0075B5B0, _M_fill_insert 0x0075BB00, resize 0x0075C040)
  and CMatrix3x4 ops 0x0075B600/0x0075B6C0 exist in this TU but are NOT called here.

## 6. Sibling landed rows (rebuild/src/compiled/00/75/)
- CTCPhysicsRigidBody_CalculateVertices_00759ec0.cpp: best template -- local `struct CRigidBody { class CRigidBodyConfig {...}; ... }`, local CMatrix3x4 with operator* decl, local `template vector`, local CTCPhysicsRigidBody {_pad00[0xa0]; CRigidBody Body;}. No pragmas/flags. Says engine header Body opaque so local model used.
- CTCPhysicsRigidBody_Simulate_0075ad50.cpp: caller; local clear() helpers reproduce the same two inline clears (CopyVectors loop + CopyTrivial memmove) -- note it was byte-matched with these local forms.
- CTCPhysicsRigidBody_ComputeForces_00759d30.cpp: local CRigidBodyConfig, Body.OneOverMass as `float OneOverMass // +0xa0 (Body)`, inline C3DVector ops locally.
- CTCPhysicsRigidBody_GetFacingVector_00759b20.cpp: uses `#define FABLE_RETAIL_CTCBASE / FABLE_RETAIL_CTCPHYSICSBASE / FABLE_RETAIL_CTCPHYSICSRIGIDBODY` + engine/CTCPhysicsRigidBody.h.
- Only one CTCPhysicsRigidBody-family row carries `// Flags: /EHsc`; none use #pragma optimize. Default flags.

## 7. Draft model errors vs retail
- Wrong signature mangling: global struct CRigidBodyConfig and public method (should be CRigidBody::CRigidBodyConfig class, private).
- Body members flattened into this; fine for offsets but names wrong.
- Draft binds `const C3DVector& normal = CollisionNormal[i]` once; retail re-reads CollisionNormal._M_start ([edi+40]) at each use with a separate byte offset -> retail likely indexes CollisionNormal[i] at each use (not a cached reference), so the compiler strength-reduces i*12.
- Retail copies the vertex into a C3DVector (int-move of Z) before subtracting -> `C3DVector vertex = config.BoundingVertices[...]` or r computed from a by-value temporary; draft subtracts straight from the reference.
- Frame 0x7c vs draft 0x74: retail keeps more named locals (vertex copy [50..58], velocity, impulse, the two matrix-result temps [74],[84]).
- Dot order: retail Z,Y,X; draft DotProduct(a,b) a.Z*b.Z + a.Y*b.Y + a.X*b.X ok-ish but use header Dot variant.
- Numerator: retail `-(dot * (CoR + 1.0f))` (fld CoR; fadd 1.0; fmulp; fchs) -> draft matches.
- impulse*OneOverMass: retail computes impulse = normal*j kept on x87 then each component * OneOverMass (`fld OneOverMass; fld st2; fmul st1`), i.e. `impulse * Body.OneOverMass` (member operator*(float)), not `OneOverMass * impulse`.
- AngularVelocity = InverseWorldInertiaTensor * AngularMomentum: result copied via integer moves (struct copy of returned temp) -- matches assignment.
- Use std::vector clear() for both lists (header) instead of hand loops; equivalent bytes expected (Simulate proved the forms).

