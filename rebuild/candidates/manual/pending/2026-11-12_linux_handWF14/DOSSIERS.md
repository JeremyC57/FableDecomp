## 0x00959800

# DOSSIER 0x00959800 -- CAIStateGroup_HoverWanderAroundThing::IsRunnable (595 bytes)

## 1. Retail shape (xdis 00959800 0x263)
- __thiscall, plain `ret` (no args), `bool` in al. NO frame pointer (sub esp,0x30; push esi/edi; later push ebx/ebp).
  NO SEH prologue even though two vectors + a CIntelligentPointer temp with dtors are live -> no EH state (like the
  landed sibling 00969E70 `// Flags: none`, which also has no SEH with vector + intelligent-pointer temp).
- Frame (offsets from esp after all 4 pushes): [+0x14] &PThing (this+0x1c, saved before the 2nd push pair),
  [+0x13] empty CThingFilter_None byte, [+0x24..+0x2c] vector A (objects), [+0x18..+0x20] vector B (dead creatures),
  [+0x30..+0x3c] C2DBoxI {TLX=x-10, TLY=y-10, BRX=x+10, BRY=y+10}. The CIntelligentPointer temp is built at [+0x18]
  (over vector B's words) while vector B's start is cached in edi and freed later from edi.
- Flow: if IS_THING_ALIVE(PThing) return true (GetPItem called twice, test byte [item+0x91],1).
  ebp=0; zero-init A (+0x24) then B (+0x18).
  Block 1: GetCreature()->GetPos(); x=ftol(X) (ebx), y=ftol(Y); box(x-10,y-10,x+10,y+10); GetWorld(); [world+0x20]->
    0x00959380(&box,&filter,&A).   Block 2: identical, GetWorld after box; [world+0x20]->0x009595C0(&box,&filter,&B).
  If B non-empty: size=(finish-start)>>2 in ebx; GetWorld(); GetWorldSeed(); INLINE random: seed=seed*0x24A1+0x24DF;
    seed=GFROR13(seed) (fastcall); `xor edx,edx; cmp ebx,0; je; div ebx` == `range ? seed%range : 0`; pick=B[idx];
    temp ctor 0xA01B10, vptr 0x0125C29C (CIntelligentPointer<const CThingGameObject>), SetPItem(pick) 0xA01B90 on temp;
    PThing.SetPItem(temp.GetPItem()) (operator=); temp dtor 0xA01C10 called DIRECTLY (non-virtual);
    free(B.start) if nonnull (edi); free(A.start) if nonnull ([esp+0x24] reloaded); return true.
  Else if A non-empty: ebx=A.start (cached); GetWorld(); GetWorldSeed(); OUT-OF-LINE GFRandom 0x004CDAE0
    (fastcall ecx=size, edx=seed); pick=A[idx]; same temp/assign/dtor; free(B) via edi, free(A) via ebx; return true.
  Else: free(B) edi, free(A) ebx; return false.
- Three separate epilogues (true/true/false). Note random is INLINED in the first branch but CALLED in the second.

## 2. Identity (name proven by two sources)
- Ego_r.pdb (egor_pdb_names.tsv 0x0097DF80) and FableWin.pdb (_fablewin_api.tsv 0x017E945B):
  `?IsRunnable@CAIStateGroup_HoverWanderAroundThing@@UAE_NXZ` = `public: virtual bool __thiscall IsRunnable(void)` (non-const).
- Vtable: rtti-vtables.tsv CAIStateGroup_HoverWanderAroundThing 0x0128D1BC slot 11 = 0x00959800 (xref: only ref is
  .rdata 0x0128D1E8 = vtable+0x2C = slot 11). Base CAIStateGroupBase slot 11 = IsRunnable (`mov al,1`,
  CAIStateGroupBase.h comment). Slot 12 = Update 0x00959A60 (landed). Manifest label is CORRECT.
- Corroboration: Ego_r places the vector<const CThingDeadCreature*> GetAllThingsInRoughArea instance at 0x0097DCA0,
  just before IsRunnable 0x0097DF80 -- same TU ordering as retail (0x009595C0 just before 0x00959800).

## 3. Callees (landed file / landed name / PDB name; draft spelling flags)
| addr | landed row | name | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | draft returns CThing* -- WRONG type |
| 0x00A01B10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | ??0CBaseIntelligentPointer@@QAE@XZ | ok |
| 0x00A01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z | draft takes CThing* -- WRONG |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ??1CBaseIntelligentPointer@@UAE@XZ (VIRTUAL dtor) | draft non-virtual dtor -- WRONG |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft `GetPosition` -- WRONG |
| 0x00BFEA70 | 00/bf/global_ftol2_00bfea70.cpp | _ftol (compiler (long) cast) | ok |
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (protected const, returns CWorld&) | draft `CWorld* GetPWorld()` -- WRONG |
| 0x00959380 | NOT landed; parked rebuild/candidates/manual/pending/2026-10-27_linux_hand954/differ/00959380.cpp (K016 one-SIB lane gap) | CThingSearchTools::GetAllThingsInRoughArea<const CThingObject, std::vector<const CThingObject*>, CThingFilter_None> (probable; mask 0x807, overflow 0x0091FFE0; only caller is this row) | draft `GetTrackNodesInRoughArea` -- WRONG; manifest label GetPBestThingInRoughArea<HasTrackName..> is propagated junk |
| 0x009595C0 | 00/95/CThingSearchTools_GetAllThingsInRoughArea_009595c0.cpp | ??$GetAllThingsInRoughArea@$$CBVCThingDeadCreature@@V?$vector@PBVCThingDeadCreature@@...VCThingFilter_None@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_None@@AAV?$vector...@Z (const method, ret 0xC) | draft `GetCustomersInRoughArea` -- WRONG |
| 0x0049D810 | 00/49/CWorld_GetWorldSeed_0049d810.cpp | ?GetWorldSeed@CWorld@@QBEPAKXZ (const) | draft `GetPRandomSeed` -- WRONG |
| 0x00497890 | 00/49/global_GFROR13_00497890.cpp | ?GFROR13@@YIJK@Z (long __fastcall GFROR13(unsigned long)) | draft returns unsigned long -- minor |
| 0x004CDAE0 | 00/4c/global_GFRandom_004cdae0.cpp | ?GFRandom@@YIKKPAK@Z (unsigned long __fastcall(unsigned long range, unsigned long* seed)) | ok |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free (STLport vector dealloc) | ok |
| inline | -- | CWorld::GetThingSearchTools / PeekThingSearchTools ([world+0x20], engine/CWorld.h:190-191) | draft `world->SearchTools` field |
- Temp vtable 0x0125C29C = CIntelligentPointer<const CThingGameObject> (rtti-vtables; scalar dtor 0x0069BAF0 landed) --
  matches the PDB member type of PThing. Draft calls it CIntelligentPointer_CThing_vftable -- WRONG T.

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*) -> +0x18 CAIBrain::Creature (CThingAICreature*), Ego_r layout;
  accessor GetCreature(): engine/CAIStateGroupBase.h `FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE` (returns CThingAICreature&)
  + engine/CAIBrain.h `FABLE_RETAIL_CAIBRAIN_GETCREATURE`.
- this+0x1c PThing: CIntelligentPointer<CThingGameObject const> (struct_layouts_egor.tsv; class size 0x24;
  base CAIStateGroup_HoverMovementBase per Update 0x00959A60, own +0x14..+0x1b). Shared headers:
  engine/CAIStateGroup_HoverWanderAroundThing.h (generated, vptr-as-data, PThing as char[8] -- not usable for a vcall
  class; Update modelled locally for that reason), engine/CAIStateGroup_HoverMovementBase.h exists.
- item+0x91 bit0: CThing::Dead (bitfield, engine/CThing.h:88, IsDead() line 280) -- draft uses m_flags91.
- world+0x20: CWorld::PThingSearchTools_ptr (scoped_ptr<CThingSearchTools>, engine/CWorld.h:303/426, needs FABLE_RETAIL_CWORLD).
- C2DBoxI TLX/TLY/BRX/BRY (engine/C2DBoxI.h; ctors under FABLE_RETAIL_C2DBOXI_CTORS; C2DBoxI(const C2DCoordI&,long radius)
  default/`FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS`).
- CIntelligentPointer: engine/CIntelligentPointer.h `FABLE_RETAIL_CINTELLIGENTPOINTER` (packed, virtual dtor) +
  `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN` (operator= as GetPItem then SetPItem); ctor CIntelligentPointer(T*) inline SetPointer.
- IS_THING_ALIVE: engine/ThingAlive.h `FABLE_RETAIL_IS_THING_ALIVE` (template, `thing!=0` needs FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT).
- Draft models LOCALLY (shared header exists for each): C3DVector, CThing, CThingCreatureBase, CBaseIntelligentPointer,
  CIntelligentPointer, CWorld, CAIBrain, CAIStateGroupBase, CAIStateGroup_HoverWanderAroundThing, C2DBoxI (as CRoughArea),
  CThingSearchTools, std::vector (as CThingList; use stlport/fable_containers.h).

## 5. Callee-body visibility (KEY)
- 0x00959260 (vector overflow), 0x00959380 and 0x009595C0 (both GetAllThingsInRoughArea instances) sit IMMEDIATELY before
  this function in retail; Ego_r has the same ordering (0x0097DCA0 before 0x0097DF80). => the template bodies were visible
  in this TU. Sibling 00969E70 (SleepOnFloor::IsRunnable) solved exactly this register/frame symptom with
  `FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY` (engine/CThingSearchTools.h:211ff, __declspec(noinline) body: "the caller
  then knows the result vector is not captured and keeps its start in a register across the later calls"). That explains
  edi caching B.start across SetPItem and the temp reusing B's frame words -- the old draft's 6-try residue.
  Needs engine/NThingMap_CThingMap.h, FABLE_RETAIL_CTCMAPWHO, FABLE_RETAIL_CTCMAPWHO_SEARCHID, FABLE_RETAIL_CTHING_STATTYPEFLAGS.
- GFRandom: inline in branch 1, out-of-line call in branch 2 (0x004CDAE0). SleepOnFloor defines a local
  FABLE_FORCEINLINE GFRandom; here the author must reproduce one inline + one call (e.g. inline GFRandom body visible but
  second site not inlined -- or branch 1 is a different inline spelling, e.g. a CAIStateGroupBase::Random-style helper as in
  Update 0x00959A60's local model, and branch 2 calls GFRandom). Unresolved; try both.
- GetWorld/GetPItem/SetPItem/ctor/dtor/GetPos/GetWorldSeed are out of line (declarations only).

## 6. Siblings
- 00/96/CAIStateGroupSleepOnFloor_IsRunnable_00969e70.cpp -- CLOSEST idiom (vector + rough area + random + temp
  CIntelligentPointer assign). `// Flags: none`; defines FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CTHING,
  _CINTELLIGENTPOINTER, _CINTELLIGENTPOINTER_ASSIGN, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTHINGAICREATURE,
  _CAISTATEGROUPBASE, _CAISTATEGROUPBASE_MEMBERS, _CAISTATEGROUPBASE_PEEK, _CAISTATEGROUPBASE_GETCREATURE, _CAIBRAIN_GETCREATURE,
  _CTHINGSEARCHTOOLS, _CTHINGSEARCHTOOLS_ROUGHAREABODY, _CTHING_STATTYPEFLAGS, _CTCMAPWHO, _CTCMAPWHO_SEARCHID, _CWORLD, ...
  Body: `std::vector<CThing*> v; ...GetAllThingsInRoughArea<CThing>(box, filter, v); if (v.empty()) return false;
  PSleepPoint = CIntelligentPointer<CThing>(v[GFRandom(v.size(), GetPWorldSeed())]); return true;`
- 00/95/CAIStateGroupHoverWanderAroundThing_Update_00959a60.cpp -- same class/TU; local polymorphic model (PBrain +4,
  HoverMovementBase pad, PThing +0x1c), local IS_THING_ALIVE, GetWorld()/GetWorldSeed spelling, Random() inline helper.
  Only define: FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- 00/95/CAIStateGroupHoverHornetQueen_SetupArena_0095b3e0.cpp -- `PeekThingSearchTools().GetAllThingsInRoughArea(C2DBoxI(x-60,y-60,x+60,y+60), CThingFilter_All(), things)` with x/y longs; local models.
- 00/95/CAIStateGroupHoverKeepInArea_IsRunnable_00959f30.cpp, 00/95/CAIStateGroupHoverAttackArea_IsRunnable_0095a2c0.cpp
  (`#pragma intrinsic(sqrt)`) -- neighbouring hover IsRunnable rows, no special flags.

## 7. Old-draft model errors
- Search-call identity swapped: call 1 (0x959380 -> vector A at +0x24) collects const CThingObject*; call 2
  (0x9595C0 -> vector B at +0x18) collects const CThingDeadCreature*; branch preference is B (dead creatures, inline
  random) then A (objects, GFRandom call). Draft names them nodes/customers. Element types: vector<const CThingObject*>,
  vector<const CThingDeadCreature*>; filter CThingFilter_None (empty, one stack byte shared by both calls -> one local
  or two temporaries folded).
- Bodies of the two search instances not visible (the main frame/register residue; see section 5).
- Wrong names: GetPosition, GetPWorld (pointer), GetPRandomSeed, CIntelligentPointer<CThing>, m_flags91, CRoughArea.
- CIntelligentPointer temp model: retail calls the base ctor then stores the derived vptr then SetPItem -- matches the
  shared header with FABLE_RETAIL_CINTELLIGENTPOINTER (virtual ~CBaseIntelligentPointer); dtor called directly.
- GetWorld() is called AFTER the box is built (box stores precede the call) in both blocks; keep that order.
- Box: likely C2DBoxI(C2DCoordI((long)pos.X,(long)pos.Y), 10) or explicit (x-10,y-10,x+10,y+10); ftol of X first (ebx), Y second.

## 0x004C9920

# DOSSIER 0x004C9920 CThing::IsAffectedByBulletTime (598 bytes)

## 1. Retail shape (xdis 004c9920 0x266)
- __thiscall, no args, plain `ret` (several exits). Returns bool in al.
- NO frame pointer: `push ecx ; push ebp ; push esi ; push edi` -> /Oy (default, no optimize("y",off)). No SEH.
- The `push ecx` slot ([esp+0xc] after the pushes) is the ONLY local: the CVectorMap LowerBound key slot, reused by every lookup.
- ebp = constant 0x47 (TCI_CARRYABLE) hoisted for the whole function (used for the key store `mov [esp+10h],ebp` and `cmp [eax],ebp`).
- esi = this; the carrier/attached-thing deferrals are self tail calls turned into `mov esi,eax ; jmp 0x4c9930` (loop to top, which re-reads [0x013B8A1C]). Alignment pad `jmp 4c9930 ; lea ecx,[ecx]` before the loop head.
- Flow (exactly as retail; the old draft's control flow is RIGHT):
  top: mgr=[0x13B8A1C]; if mgr+0x54 -> PAUSE_A; if mgr+0x55 -> PAUSE_B (a30 retests al=0 -> a65); else NORMAL.
  NORMAL: HasTC(0x64 TCI_REGION_EXIT, +0x2c bit4) -> false; p = mgr->PlayerManager(+0x30) .GetMainPlayer() [CALL 0x449960, const overload]
    -> lea ecx,[eax+0x34] (CPlayer::PlayerCharacter) -> GetPItem 0xA01B50; if p && !(p+0x91 bit0 Dead) && !(this+0x93 bit5 IsAffectedByBulletTimeOverride) && p HasTC(0x92 TCI_BULLET_TIME, +0x30 bit18) -> false;
    Type(+0x10)==2 -> false; HasTC(0x47, +0x28 bit7): carrier = ((ternary)->second)->GetPCarriedByThing() [`mov ecx,[eax+4]` straight] ; if carrier -> loop;
    HasTC(0x4b TCI_PARTICLE_EMITTER, +0x28 bit11 `test ah,8`): e=lookup (`mov eax,[eax+4] ; lea edi,[eax+1ch]`), GetPItem(edi) x3: if !p -> true; if p==this -> true; this=GetPItem(); loop. else true.
  PAUSE_A (+0x54 set): if (this+0x93 bit0 NotPausedDuringCutscenes) && !IsUnderScriptControl() -> false; if (+0x92 bit7 PausedDuringCutscenes) && IsUnderScriptControl() -> true;
  a65: Type==2 -> false; HasTC(0x26 TCI_DOOR +0x24 bit6) false; HasTC(0xf2 TCI_BOB +0x3c bit18) false; HasTC(0x64) false;
    HasTC(0xf9 TCI_FADE_OUT_AND_IN +0x3c bit25): fade lookup `mov eax,[eax+4] ; mov ecx,eax` ; return !IsFading() (`neg al; sbb; inc`);
    HasTC(0x47): carrier loop (same as NORMAL);
    HasTC(0x4b): return !emitter->AlwaysUpdate (`movzx eax,byte[eax+45h]; shr eax,1; not; and 1`);
    b51: if mgr->NonScriptedEntitesPauseMode (re-read [0x13B8A1C]+0x54) return !IsUnderScriptControl(); return true.
- Note: +0x3c word is loaded once into eax and tested for 0x40000 then (after the +0x2c test) 0x2000000 -> HasTC(0xf2), HasTC(0x64), HasTC(0xf9) in that order.
- Return-false block 4c9b0a sits after the fade exit and before the 0x4b-in-pause block (retail block order: fade-return, false, 0x4b-return, script tail, true).

## 2. Name / signature
- Ego_r.pdb (egor_pdb_names.tsv 0x004CA7D0) and FableWin.pdb (_fablewin_api.tsv 0x017CEC64 / 0x01CF6F80, naming_stragglers): `?IsAffectedByBulletTime@CThing@@QBE_NXZ`
  => `bool CThing::IsAffectedByBulletTime() const` (public, non-virtual, CONST). Manifest label is RIGHT; the old draft LACKS `const`.
- All landed callers declare it `bool IsAffectedByBulletTime() const; // 0x004C9920` (e.g. rebuild/src/compiled/00/6e/CTCDParticleEmitter_AttachToThing_006e0be0.cpp). 21 call sites (xref.py).
- const consequence: lookups are the const PeekTC/LowerBound (0x004365B0 is the CONST LowerBound), carrier is `const CThing*`.

## 3. Callees
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00449960 | 00/44/CPlayerManager_GetMainPlayer_00449960.cpp | CPlayerManager::GetMainPlayer() const | ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ (returns const CPlayer&; non-const twin 0x00449970) | draft returns CPlayer* -> WRONG, must be `const CPlayer& GetMainPlayer() const` |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | draft returns CThing* (ok via CIntelligentPointer<T>::Get() static_cast, engine/CIntelligentPointer.h line 60) |
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (const) | const LowerBound | draft local CInterfaceMap -> use engine/CVectorMap.h |
| 0x00705170 | 00/70/CTCCarryable_GetCarrier_00705170.cpp (body is a fastcall stub named GetCurrentSelectedEntryData!) | file name GetCarrier | ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ (const, returns const CThing*); twin non-const at 0x00705180 (Ego_r order const 0x725060 then non-const 0x725070); engine/CTCCarryable.h line 18 declares it | draft "GetCarrier" WRONG -> GetPCarriedByThing. NAME_FIXES candidate: the landed row 0x00705170 spelling (GetCarrier / GetCurrentSelectedEntryData) |
| 0x004C8C90 | 00/4c/CThing_IsUnderScriptControl_004c8c90.cpp | CThing::IsUnderScriptControl | ?IsUnderScriptControl@CThing@@QBE_NXZ (CONST) | draft non-const -> add const |
| 0x0049D310 | 00/49/CTCFadeOutAndIn_IsFading_0049d310.cpp | CTCFadeOutAndIn::IsFading() const | ?IsFading@CTCFadeOutAndIn@@QBE_NXZ | ok |
- Global 0x013B8A1C = `CThing::PCurrentThingManager` (private static, engine/CThing.h line 569), accessors `GetPCurrentThingManager()` / `PeekPCurrentThingManager()` static __fastcall inline (CThing.h 558-559). Draft's `g_pThingManager` is the old stand-in spelling.
- No vtable calls.

## 4. Members
- this/p (CThing, engine/CThing.h; offsets = retail, header static-asserts UsingComponents 0x20, Components 0x44):
  - +0x10 CClass::Type (unsigned char; engine/CClass.h, `GetType() const`, needs FABLE_RETAIL_CCLASS which CThing.h defines). Draft "Category" -> real name Type.
  - +0x20 std::bitset<274> UsingComponents (HasTC(ETCInterfaceType) const, inline; FABLE_RETAIL_CTHING_HASTC_FORCEINLINE makes it __forceinline).
  - +0x44 Components CVectorMap; lookups via PeekTC(id,&p) const under FABLE_RETAIL_CTHING_GETTC_EXPANDED (+FABLE_RETAIL_CTHING), or GetTC(type) const reference form (Components.Find).
  - +0x91 bit0 Dead (IsDead()/IsAlive() inline), +0x92 bit7 PausedDuringCutscenes, +0x93 bit0 NotPausedDuringCutscenes, +0x93 bit5 IsAffectedByBulletTimeOverride (Ego_r +0xA1..0xA3; header bitfields in PDB order, all protected -> fine in a member).
- CThingManager (engine/CThingManager.h): +0x30 `CPlayerManager& PlayerManager` (private reference), +0x54 NonScriptedEntitesPauseMode, +0x55 AllEntitesPauseMode. Opt-ins: FABLE_RETAIL_CTHINGMANAGER -> IsNonScriptedEntitesPauseMode() const; FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE -> IsAllEntitesPauseMode() const. No PlayerManager accessor macro exists: FableWin has ?PeekPlayerManager@CThingManager@@QBEABVCPlayerManager@@XZ / GetPlayerManager (QAE) -- inline in retail here (`mov ecx,[ecx+30h]`). CThing::PeekPlayerManager() is OUT of line at 0x004C79F0 (CThing.h 498), so retail did NOT call it here (it would be a call) -> spell via the manager's own (inline) PeekPlayerManager/GetPlayerManager (add to header or friend access).
- CPlayer (engine/CPlayer.h): +0x34 `CIntelligentPointer<CThingPlayerCreature> PlayerCharacter` (asserted 0x34). CPlayer::GetPPlayerCharacterThing() const is out of line at 0x00487B70 (`add ecx,34h; jmp GetPItem`), so retail here inlined GetPItem directly -> spell as PlayerCharacter.Get()/GetPItem (header access needed), not GetPPlayerCharacterThing().
- CTCDParticleEmitter (engine/CTCDParticleEmitter.h; FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL): +0x1c `CIntelligentPointer<CThing> PAttachedToThing` (line 77), +0x45 bit1 `bool AlwaysUpdate:1` (line 95). Draft's "CTCFollower / Leader / IgnoresBulletTime" is WRONG: interface 0x4b is TCI_PARTICLE_EMITTER (ETCInterfaceType.h line 80).
- CTCFadeOutAndIn (no shared header; landed row models FadingOut +0xc, FadingIn +0xd over engine/CTCBase.h).
- CTCCarryable: engine/CTCCarryable.h (FABLE_RETAIL_CTCBASE).
- Classes the old draft models LOCALLY that have shared headers: CThing, CThingManager, CPlayerManager, CPlayer, CBaseIntelligentPointer (engine/CIntelligentPointer.h), CTCCarryable, CVectorMap (CInterfaceMap), and "CTCFollower" (really CTCDParticleEmitter).

## 5. Callee-body visibility
- Inlined in retail (must be visible inline): HasTC, the PeekTC/GetTC lookup (LowerBound called out of line), CThingManager pause getters and PlayerManager getter, CIntelligentPointer::Get, CClass::GetType, the CThing static manager accessor.
- Out of line (declare only): GetMainPlayer const 0x449960, GetPItem 0xA01B50, LowerBound const 0x4365B0, GetPCarriedByThing const 0x705170, IsUnderScriptControl 0x4C8C90 (same TU Thing.cpp, earlier address, but called), IsFading 0x49D310.
- Lookup idioms (landed 0x004BFB40 comment): the GETTC_EXPANDED ternary-inside-store lands the component straight in ecx (`mov ecx,[eax+4]`) -> retail's 0x47 lookups; an if-assign/out-param form gives `mov eax,[eax+4] ... mov ecx,eax` -> retail's 0xf9 lookup and the 0x4b lookups (`mov eax,[eax+4]; lea edi,[eax+1ch]` / `movzx eax,[eax+45h]`). So the 0xf9/0x4b lookups are likely a DIFFERENT spelling (e.g. reference GetTC(id) const / Components.Find / non-expanded PeekTC with GFDebugPDynamicCast) than 0x47.
- Key store happens after the bit test (`lea eax,[esp+0ch]; ... mov [esp+10h],ebp` after push) = key copied inside the test, matching GETTC_EXPANDED's documented shape.

## 6. Siblings
- rebuild/src/compiled/00/4c/CThing_IsUnderScriptControl_004c8c90.cpp (RELOCATION_MATCH: HasTC + ternary GetTC, local CThing model; same TU).
- rebuild/src/compiled/00/4c/CThing_InitialActivate_004c9690.cpp, CThing_Kill_004c9b80.cpp (next fn), CThing_WorldUpdateBulletTime_004c8ba0.cpp (#define FABLE_RETAIL_CTCBASE), CThing_RemoveTC_004c9840.cpp. None use special optimize pragmas; no `// Flags:`.
- Shared-header idiom rows: 00/75/CTCCameraPointBuilding_GetOwnerBuilding_0075d940.cpp (`#define FABLE_RETAIL_CTHING`, `FABLE_RETAIL_CTHING_GETTC_EXPANDED`, PeekTC + POwner.Get()); 00/4b/NGraphicAppearanceCTCGraphicAppearance_SetLightingChannel_004bfb40.cpp (+FABLE_RETAIL_CTHING_STATEACCESS); 00/4b/..._IsDrawingAfterWater_004bd3c0.cpp (0x47 carryable lookup + GetPCarriedByThing const, `// Flags:` empty).

## 7. Old draft model errors
- Missing `const` on IsAffectedByBulletTime and IsUnderScriptControl; GetMainPlayer must return const CPlayer& (draft pointer).
- 0x4b mis-identified as a "follower"; it is CTCDParticleEmitter PAttachedToThing / AlwaysUpdate.
- GetCarrier -> GetPCarriedByThing (returns const CThing*).
- All classes modelled locally; g_pThingManager stand-in -> CThing::PCurrentThingManager via the inline static accessor.
- Residue (580v598, 18 bytes SHORT): the 0xf9/0x4b lookups in retail go through eax (`mov eax,[eax+4]`), and the shared return-false block is after the fade exit; the draft used one lookup idiom (ternary) everywhere. Use the ternary GETTC_EXPANDED form only for 0x47 and a second (if-assign / reference GetTC) form for 0xf9 and 0x4b. Retail also calls GetPItem three times on PAttachedToThing (no named local): `if (!e->PAttachedToThing.Get() || e->PAttachedToThing.Get()==this) return true; return e->PAttachedToThing.Get()->IsAffectedByBulletTime();` (written as `if (Get() && Get()!=this) return Get()->IsAffected...`).
- The NORMAL-path player test: retail is p && !p->Dead && !IsAffectedByBulletTimeOverride && p->HasTC(0x92) (draft order correct).

## 0x00760E70

# DOSSIER 0x00760E70 (603 B) -- NShape::CTrack::GetMinDistanceToTrack

## 1. Retail shape (xdis 00760e70 0x26b)
- __thiscall, `ret 8`, returns bool in al (`mov al,1` / `xor al,al`). NO frame pointer (sub esp,44h; push edi/ebx/esi) -> no /Oy- pragma. No SEH. Not size-optimised (`lea esp,[esp]` loop-align pad at 0x760ED9, imul-by-0x2AAAAAAB /12).
- Registers: edi=this, ebx=pos (const C3DVector&, loaded from [esp+50h] AFTER the size test), esi=iterator. `best` (1e10f = 0x501502F9 immediate) lives in the dead `pos` ARG SLOT [esp+54h]; `distance` out ptr read from [esp+58h] only at the end.
- size test: (end-begin) signed /12 then `cmp eax,1 ; jbe` -> `Points.size() > 1` (size_type unsigned). Fail path `xor al,al` is placed LAST with its own epilogue (0x7610C2) -> `if (size>1) {...return true;} return false;` (or else-arm).
- Frame locals: [0c]=d2 temp; [10..18]=prev (3 int copies); [1c..24]=cur (3 int copies); [28],[2c]=pos.X,pos.Y only (flat pos, 8 bytes, no Z ever stored); [34..3c]=dir (Z stored as int 0) passed in ecx to 0xA14440; [44]=minY [48]=maxX [4c]=maxY -- contiguous like a C2DBoxF {TLX,TLY,BRX,BRY} at [40] whose TLX (minX) stays in st0 and is never stored.
- Loop: prev=*begin; it=begin+1; top test `it==end` (je to exit) then bottom-tested `jne` -> plain for(++it; it!=Points.end(); ++it). end re-read from [edi+0Ch] every pass (not cached; out-of-line call in body).
- Box: minX=prev.X (st0), minY=maxY=prev.Y, maxX=prev.X; `cur.X<prev.X`->minX (test ah,5 jp), `cur.Y<prev.Y`->minY, `cur.X>prev.X`->maxX (test ah,41h jne), `cur.Y>prev.Y`->maxY. Order minX,minY,maxX,maxY.
- Inside test (same pattern as landed C2DBoxF::ContainsPoint 0x00535CD0): flat.X>=minX (test ah,1 jne), flat.X<maxX (ah,5 jp), flat.Y>=minY, flat.Y<maxY.
- Inside arm: dir=C3DVector(cur.X-prev.X, cur.Y-prev.Y, 0.0f) [X computed first]; call 0xA14440 (AccurateNormalise, ecx=&dir, return ignored); perp=(dir.Y,-dir.X,0.0f): `fld dir.X; fchs` first; d = *pos - prev (uses ebx=pos, full X,Y,Z subtraction, loads dx,dy,dz); dot accumulates perp.Y*d.Y first, + perp.X*d.X, + d.Z * [0x0122DEDC] (0.0f literal, cited by landed 0x00B18AF0/0x00B1B0C0) -> matches header's FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ (`dot=Y*o.Y; dot+=X*o.X; dot+=Z*o.Z`), called as perp.Dot(*pos - prev) (or reverse); then fabs.
- Outside arm: d1 = |prev - flat| where Z term is prev.Z UNSUBTRACTED (flat has no Z: either C3DVector(x,y,0) with x-0 folded, or a C2DVector subtraction); sqrt intrinsic (fsqrt); d2 = |cur - flat| same; d2 spilled to [esp+0Ch]; `fcom d2 ; test ah,5 ; jnp keep d1` -> min = d1 < d2 ? d1 : d2 (compare is d1-on-stack vs d2).
- Best: `fcom best ; test ah,5 ; jp skip ; fstp best` -> `if (d < best) best = d;` (STRICT <).
- Tail: prev = cur via 3 int moves at loop bottom (before end reload); `*distance = best; return true;`.

## 2. Name / signature (two sources)
- FableWin.pdb: ?GetMinDistanceToTrack@CTrack@NShape@@ABE_NABVC3DVector@@AAM@Z (naming_stragglers/FableWin.exe.signatures.tsv @0x00771560 and 0x025DB980).
- Fable.exe binding: a matcher row in ghidra_out maps 760e70 -> same mangled name (score 99.8, true); egor_signatures: 00760e70 __thiscall bool GetMinDistanceToTrack.
- Landed caller NShape::CTrack::IsPosInShape 0x00761740 (rebuild/src/compiled/00/76/NShapeCTrack_IsPosInShape_00761740.cpp) declares `bool GetMinDistanceToTrack(const C3DVector& pos, float& distance) const;` and calls it at 0x0076174A (only xref).
- => PRIVATE, CONST, non-virtual: `bool NShape::CTrack::GetMinDistanceToTrack(const C3DVector& pos, float& distance) const`. Manifest label name is right; the draft's signature is WRONG (pointer params, non-const). Leaf: ?GetMinDistanceToTrack@CTrack@NShape@@ABE_NABVC3DVector@@AAM@Z.

## 3. Callees
- 0x00A14440 only call. Landed rebuild/src/compiled/00/a1/C3DVector_AccurateNormalise_00a14440.cpp as `C3DVector& C3DVector::AccurateNormalise()` (Ego_r/FableWin ?AccurateNormalise@C3DVector@@QAEAAV1@XZ, sqrt-based). DRAFT SPELLS IT `Normalise()` returning void -- WRONG (C3DVector::Normalise is the inline fast GFFastOneOverSqrt version). Declared in engine/C3DVector.h under `FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE` (line ~342). Different TU (C3DVector.cpp, 0xA14xxx) -> body not visible, out-of-line call is right.
- No vtable calls. Constants: 0x0122DEDC = 0.0f (compiler literal), 1e10f immediate.

## 4. Members
- this+0x08 / +0x0C = NShape::CShape::Points (std::vector<C3DVector>) begin/finish. struct_layouts_egor.tsv has no NShape::CShape row; layout cited by landed 0x00761F40 ctor / 0x00760C40 (Ego_r CShape {vptr, Owner&, Points, SelectedPoints}, retail STLport 12-byte vectors).
- SHARED HEADER EXISTS for the base: rebuild/include/engine/NShape_CClosedShapeBase.h defines `NShape::CShape` (Owner as CTCShapeManager&, std::vector<C3DVector> Points from stlport/fable_containers.h, SelectedPoints). Opt-ins there: FABLE_RETAIL_NSHAPE_CSHAPE_GETSTARTPOINT(_DECLARATION), _EDITORDRAWHIGHLIGHTPOINTS, _STATONSERIALISE (none needed). No NShape::CTrack header exists -> declare `class CTrack : public CShape` locally (TrackTraverser +0x20, TrackLength +0x24 if needed; not used here) with GetMinDistanceToTrack const.
- vector::size() in fable_containers.h:453 `size_type(_M_finish-_M_start)` -> gives the signed /12 + unsigned jbe.
- C3DVector: engine/C3DVector.h (X/Y/Z). Useful opt-ins: FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE, FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ, GetMagnitude() (default `(float)sqrt(GetSquaredMagnitude())`; GetSquaredMagnitude default is X*X+Z*Z+Y*Y; _XYZ / _ZYX variants exist), operator- (FABLE_VECTOR_INLINE = __forceinline), _SUBTRACT_XY_GROUP. C2DVector (engine/C2DVector.h) has ctor from C3DVector; C2DBoxF (engine/C2DBoxF.h, FABLE_RETAIL_C2DBOXF ctor(float x4), ContainsPoint out-of-line decl only).
- Draft models LOCALLY (all have shared headers): C3DVector (-> engine/C3DVector.h), CPointVector/std::vector (-> stlport/fable_containers.h), NShape::CShape base (-> engine/NShape_CClosedShapeBase.h). Free Dot/Min2/Max2 helpers are invented.

## 5. Callee visibility
- AccurateNormalise: out of line, other TU -> declaration only (header opt-in).
- Box min/max + containment are fully inlined (no calls) -- likely C2DBoxF-style inline or plain compares; retail ContainsPoint exists out of line at 0x00535CD0 but is NOT called here, so if a C2DBoxF is used its containment must be inline (header only declares it -> write it inline/plain).
- Same TU neighbours (retail order): 0x00760BC0 CShape::GetBounds, 0x00760C40 CTrack::GetTrackPosAtLengthFraction, 0x00760DA0 CTrack::GetTrackPosNearest, [0x00760E70 this], 0x007610D0 CSubdivisionTriangle::IsPosInShape, 0x00761740 CTrack::IsPosInShape. None called here. No STLport helpers called.

## 6. Siblings (copy idioms)
- 00/76/NShapeCTrack_IsPosInShape_00761740.cpp: includes engine/C3DVector.h, local CShape/CTrack, const-ref signature (the caller).
- 00/76/NShapeCShape_GetBounds_00760bc0.cpp: same min/max compare idioms (`<` -> test ah,5 jp; `>` -> test ah,41h jne), end re-read each pass.
- 00/76/NShapeCTrack_CTrack_00761f40.cpp: `#pragma intrinsic(sqrt)`, C3DVector.h, segment |p-prev| with sqrt.
- 00/76/CTCShapeManager_GetPrincipalVector_00760910.cpp / DrawShapes_00761980.cpp: `// Flags: none`, include engine/CTCShapeManager.h + engine/NShape_CClosedShapeBase.h after the FABLE_RETAIL_CCHARSTRING/CCLASS/DEF_CLASS_BASE/CTCBASE/CWORLD/CTHING/CTCSHAPEMANAGER defines.
- All NShape rows: `// Flags: none`, no optimize pragmas.

## 7. Old draft model errors
1. Signature: pointers + non-const -> must be `(const C3DVector& pos, float& distance) const` (PDB + landed caller).
2. Callee: `Normalise()` void -> `C3DVector& AccurateNormalise()` (0xA14440) via FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE.
3. Best update: draft `d <= best` -> retail `d < best` (test ah,5 jp, not ah,41h).
4. Min: draft Min2(d1,d2) = `d2<d1?d2:d1`; retail compares d1 (st0) against spilled d2 -> `d1 < d2 ? d1 : d2` (or if(d2<d1)/explicit).
5. Dot: draft free Dot X,Y,Z order; retail accumulates Y,X,Z -> member Dot with FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ.
6. Local struct models instead of shared headers (C3DVector, vector, CShape).
7. NOTES residue (frame 0x48 vs 0x44, cur through ebx/ebp): retail keeps pos in ebx the whole loop and reuses its arg slot for best; with a const& pos (not pointer copied) and correct helper forms the frame may shrink. Flat pos only X,Y stored -> consider `C2DVector flat(pos)` / C3DVector(pos.X,pos.Y,0) variants; the GetLength in the outside arm loads prev.Z un-subtracted.

## 0x006AC430

# DOSSIER 0x006AC430 CThingPlayerCreature::LearnExpression (606 bytes, 0x6ac430..0x6ac68d)

## 1. Retail shape (xdis 006ac430 0x26e)
- __thiscall, `ret 0xc` (3 dword args). Two epilogues: success `mov al,1` (0x6ac661), fail `xor al,al` (0x6ac682).
- NO frame pointer (`sub esp,0x10` then push ebx,ebp,esi,edi ALL FOUR at entry -- no shrink-wrap of ebp). No SEH prologue
  although the body has dtor-bearing locals (CCharString temp, CDefPointer) -> no /EHsc, same as sibling LearnAbility.
  Speed-optimised (not optimize("s")): no frame-pointer, `test ah,ah / jns` bitset tests inline.
- Frame (base = esp after the 4 pushes): [0x10] def out-slot (CDefPointer<CExpressionDef>, zeroed at 0x6ac498),
  [0x14] CCharString name(""), [0x18] `&this->Components` homed (written 0x6ac521, reloaded 0x6ac574, then reused as key 0x6f),
  [0x1c] expression global index (spilled 0x6ac45e, reloaded 0x6ac5bc), arg slots reused: [0x24] expressionName slot -> key 0x11 -> itemTC (0 then GetTC(0x1a));
  [0x28] quickAccessSlot (read 0x6ac5cb), [0x2c] silent (read 0x6ac57c / 0x6ac5fa) -> key 0x29.
- Registers retail: esi=this throughout until hero; ebx = def.Object (0x6ac4c3) then INVENTORY (0x6ac538); ebp = item
  (CThingObject::Create result, 0x6ac4e8); edi = DummyObject temp, then &this->Components (map), later itemTC in hero block.
  Old draft residue (NOTES): item edi / map ebx / inventory ebp, ebp pushed late -> 603 v 606.
- Control flow: IsExpressionAvailable -> jne fail; idx = PeekDefinitionManager().GetDefGlobalIndexFromName(name); `jle` fail
  (idx > 0); GetDefClassTypeNameFromGlobalIndex(idx) == "EXPRESSION" (0x01235D94) else fail (no def release on these paths);
  def.Object = 0; GetDef(idx, &def); { CCharString name("",-1) [str 0x0122D70E]; item = CThingObject::Create(def->DummyObject
  (+0x70, loaded BEFORE GetPos), GetPos(), PlayerNumber (movsx +0x90, loaded before GetPos), 0, 0, name); } ~CCharString;
  if (!item) -> release(ebx) fail; if (!item->HasTC(0x8f) [+0x30 bit15]) { item->Kill(false); release; fail }
  if (!HasTC(0x11) [+0x20 bit17]) release fail; inventory = GetTC(0x11); itemTC = 0; if item->HasTC(0x1a) [+0x20 bit26] itemTC = item->GetTC(0x1a);
  itemTC->AddToInventory(*inventory, false, true, 0, silent, false); if HasTC(0x6f) [+0x2c bit15] GetTC(0x6f)->AddLearnedExpression(idx);
  slot: if (quickAccessSlot == -1) slot = inventory->GetQuickAccessNextFreeSlot(item->DefGlobalIndex (movzx +0x8c)); if (slot != -1)
  SetThingAsQuickAccessItem(item->DefGlobalIndex, slot, false);  if (!silent && HasTC(0x29) [+0x24 bit9]) { hero = GetTC(0x29);
  if (hero && itemTC->GetTutorialCategory()) hero->GiveTutorialForCategory(itemTC->GetTutorialCategory(), false); }
  release def (reloaded from [esp+0x10]) -> return true.
- GetTC expansion each time: `call LowerBound(0x0040F020)(&key)` ; `cmp eax,[map+4]; je use_end; cmp [eax],K; jle keep; use_end: eax=end` ; `mov X,[eax+4]`.
- Def release: BOTH paths are `dec [reg+4] ; jne ; call [vtbl+4]` with NO null test (def was dereferenced at 0x6ac4c7, so VC drops a dtor's null test).
  Fail path releases via ebx (def kept in register); success path reloads [esp+0x10] (ebx was reused for inventory).
  Fail-path release target 0x6ac676 is shared by: item==null, Kill path, !HasTC(0x11).

## 2. Identity (proven, 3 sources)
- Ego_r.pdb egor_pdb_names.tsv: `?LearnExpression@CThingPlayerCreature@@QAE_NABVCCharString@@J_N@Z` (0x005499c0 Ego build).
- FableWin.pdb _fablewin_api.tsv / FableWin.exe.signatures.tsv: same mangled name (0x01802997, 0x01df89c0).
- Callers: landed 0x00847990 CCreatureAction_LearnExpression::PerformAction (RTTI vtable 0x0127461C slot 3) calls
  `PPlayerCreature->LearnExpression(name, -1, false)`; landed 0x0057BE71 CTCHeroStats::CheckForNewExpressions calls it (xref.py: 20 calls 0x57be..0x57c227).
- Signature: `bool CThingPlayerCreature::LearnExpression(const CCharString&, long, bool)` -- NON-const (QAE). Manifest label is CORRECT.
  Draft header "604 bytes" is wrong: 606.

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | PDB spelling | draft spelling (flag) |
|---|---|---|---|
| 0x006AC390 | 00/6a/CThingPlayerCreature_IsExpressionAvailable_006ac390.cpp | ?IsExpressionAvailable@CThingPlayerCreature@@QBE_NABVCCharString@@@Z (CONST) | non-const -- FLAG (landed row also non-const) |
| 0x004C7990 | 00/4c/CThing_PeekDefinitionManager_004c7990.cpp | ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ | GetDefinitionManager() returning ptr -- FLAG |
| 0x009AD410 | 00/9a/CDefinitionManager_GetDefGlobalIndexFromName_009ad410.cpp | ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z (const) | non-const -- FLAG |
| 0x009ACCE0 | 00/9a/..GetDefClassTypeNameFromGlobalIndex_009acce0.cpp | ?GetDefClassTypeNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z (const, by value) | non-const -- FLAG |
| 0x005B3440 | 00/5b/CDefString_operator_005b3440.cpp | ??8CDefString@@QBE_NPBD@Z | ok (const) |
| 0x005F81BE | 00/5f/CDefinitionManager_GetDef005f81be_005f81be.cpp (frame-pointer size TU, other TU) | FableWin ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@VCExpressionDef@@@@@Z -- out by POINTER | draft: by reference + a FAKE same-TU noinline body calling a made-up FindDef_* -- FLAG, delete |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / _0099eae0.cpp | CCharString(const char*, long) / ~CCharString | ok |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | GetPosition -- FLAG |
| 0x00703210 | 00/70/CThingObject_Create_00703210.cpp | ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (static __fastcall, position by REF) | free CreateThingObject with C3DVector* -- FLAG |
| 0x004C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | ?Kill@CThing@@QAEX_N@Z | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAEPAU?$pair@..@std@@ABW4ETCInterfaceType@@@Z (key by const ref) | local CInterfaceMap::LowerBound(const long*) -- FLAG |
| 0x005D8D50 | 00/5d/CTCInventoryItem_AddToInventory_005d8d50.cpp | ?AddToInventory@CTCInventoryItem@@QAE_NAAVCTCInventoryBase@NInventory@@_N1J11@Z (inventory BY REFERENCE) | CTCInventory* -- FLAG |
| 0x005D92B0 | 00/5d/CTCInventoryItem_GetTutorialCategory_005d92b0.cpp | ?GetTutorialCategory@CTCInventoryItem@@QBE?AW4ETutorialCategory@@XZ (const, enum) | GetCategoryIdentifier -- FLAG |
| 0x005EEDCF | 00/5e/CTCInventoryAbilities_AddLearnedExpression_005eedcf.cpp | ?AddLearnedExpression@CTCInventoryAbilities@@QAEXJ@Z | ok |
| 0x00582027 | 00/58/CTCInventory_GetQuickAccessNextFreeSlot_00582027.cpp | ?GetQuickAccessNextFreeSlot@CTCInventory@@QBEJJ@Z (CONST) | non-const -- FLAG |
| 0x00584157 | 00/58/CTCInventory_SetThingAsQuickAccessItem_00584157.cpp | ?SetThingAsQuickAccessItem@CTCInventory@@QAEXJJ_N@Z | ok |
| 0x006C9FA0 | 00/6c/CTCHero_GiveTutorialForCategory_006c9fa0.cpp | ?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z (enum arg) | long -- FLAG |
| vtbl slot 1 (+4) | def release | CDefPointeeBase / Destroy-at-zero | ok |

## 4. Members
- CThing (engine/CThing.h; PDB names; retail offsets -- Ego_r.pdb layout is +0x10/+4 off, use the retail header):
  UsingComponents std::bitset<274> +0x20 (HasTC 0x11/0x29/0x6f/0x8f/0x1a), Components CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>> +0x44 (start +0, finish +4),
  DefGlobalIndex unsigned short +0x8c (GetDefGlobalIndex()), PlayerNumber signed char +0x90 (GetPlayerNumber() returns unsigned long; movsx).
  Opt-ins: FABLE_RETAIL_CTHING (forceinline GetTC(ETCInterfaceType) -> *Components.Find(type)->second), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE,
  FABLE_RETAIL_CTHING_GETTC_EXPANDED / _GETTC_ORIGINAL_CHAIN (GetTC<T>(id,T**) / PeekTC forms), engine/CVectorMap.h FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
  (Find expanded in place: `call LowerBound; cmp eax,end; je; cmp [eax],K; jle` -- exactly this row's pattern) and _ORIGINAL_FIND / _INLINE_LOWERBOUND.
- CExpressionDef (engine/CExpressionDef.h, FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CDEFSTRINGTABLE + FABLE_RETAIL_CCHARSTRING): DummyObject +0x70 (CDefIndex = one long; PDB +0x74). RefCount +4, Destroy vslot 1 (CDefPointeeBase).
- CDefPointer (engine/CDefPointer.h): ~CDefPointer -> Release() null-tested; opt-ins FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF / _RELEASE_POINTEE / _RELEASE_SELFDELETING_POINTEE / _RESET / _POINTER_CTOR_ADOPT.
- CDefinitionManager.h: FABLE_RETAIL_CDEFINITIONMANAGER, _GETDEF_EXPRESSIONDEF (declares GetDef(long, CDefPointer<CExpressionDef>*) const @0x5F81BE),
  _GETDEF_EXPRESSION_NONCONST (FableWin-exact QAE spelling), _CLASSTYPENAME.
- CDefString.h: FABLE_RETAIL_CDEFSTRING, _EQUALS.  CTCInventoryItem.h: FABLE_RETAIL_CTCINVENTORYITEM, _ADDTOINVENTORY, _CATEGORY.
  CTCInventory.h: FABLE_RETAIL_CTCINVENTORY, _QUICKACCESS (line 301-303 SetThingAsQuickAccessItem). CTCInventoryAbilities.h: FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS.
  CTCHero.h: FABLE_RETAIL_CTCHERO_TUTORIAL (GiveTutorialForCategory). CThingObject.h: FABLE_RETAIL_CTHINGOBJECT_CREATE.
- Classes the old draft models LOCALLY that have shared headers: CCharString, CDefString, CDefPointeeBase, CExpressionDef, CDefPointer,
  CDefinitionManager, CVectorMap (as CInterfaceMap), CThing, CThingPlayerCreature, CTCInventory, CTCInventoryItem, CTCInventoryAbilities, CTCHero, CThingObject (as free fn), C3DVector.

## 5. Callee visibility
- 0x006AC390 IsExpressionAvailable: same class, immediately preceding address -> same TU, defined earlier (visible). Called out of line.
- 0x006AC690 LearnAbility follows -> same TU. Interface lookups (HasTC/GetTC/Find) are inline (forceinline in headers); LowerBound 0x0040F020 out of line (other TU, aligned).
- 0x005F81BE GetDef: unaligned, lives in a frame-pointer optimize("s") TU -> NOT visible here; retail treats &def as escaped
  (reloads [esp+0x10] after the CCharString ctor at 0x6ac4c3 and again at 0x6ac653). Delete the draft's fake same-TU body.
- 0x00582027 / 0x00584157 / 0x005EEDCF: unaligned addresses in size-optimised TUs elsewhere -> declarations only.
- No STLport helpers called.

## 6. Siblings
- 00/6a/CThingPlayerCreature_LearnAbility_006ac690.cpp (RELOCATION_MATCH, same TU, nearly identical recipe): no pragmas, no Flags;
  local CDefPointer with null-tested dtor, `{ CCharString name(""); item = CreateThingObject(def->InventoryObject, this, name); }`,
  forceinline CreateThingObject wrapper -> CThingObject::Create(defIndex, creature->GetPos(), creature->PlayerNumber, 0, 0, name),
  GetInterface(&p, id) template, shared engine/CVectorMap.h + C3DVector.h. Retail LearnAbility shrink-wraps edi; this row does NOT shrink-wrap.
- 00/6a/CThingPlayerCreature_IsExpressionAvailable_006ac390.cpp (RELOCATION_MATCH): CDefHolder<T> owning the GetDef out slot
  (`CDefPointer out; out.Object=0; bool found = GetDef(...,&out); CDefHolder def(out.Object);`) -> null test dropped after deref; forceinline Find as ONE ternary pick.
- 00/6a/CThingPlayerCreature_DrawIsDisplayingInformation_006a9560.cpp / FeelsGravity_006a9570.cpp: shared headers with
  FABLE_RETAIL_CTHINGPLAYERCREATURE(_VSLOTS), FABLE_RETAIL_CTHING(_GETTC_EXPANDED), CTCBASE, CCHARSTRING, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE.
- 00/6a/CThingPlayerCreature_ApplyRelativeMovementAcceleration_006ad3c0.cpp: macro set CCHARSTRING CWIDESTRING CCLASS DEF_CLASS_BASE CDEFSTRINGTABLE CTCBASE CTHING CTHINGPHYSICAL CTHINGGAMEOBJECT CTHINGCREATUREBASE CTHINGPLAYERCREATURE.
- Caller 00/84/CCreatureActionLearnExpression_PerformAction_00847990.cpp declares `bool LearnExpression(const CCharString& name, long index, bool silent)`.

## 7. Old draft model errors vs disassembly
- Register residue is the whole diff (603v606): retail item=ebp, map=edi (+ homed [esp+0x18]), inventory=ebx (ebx first holds def).
  Retail keeps def in ebx through Create/HasTC tests and releases the FAIL path from ebx -> the def pointer is a single value loaded
  once after the name block, then ebx reassigned to inventory; success path re-reads the out slot. Suggests: def held in a
  holder/local initialised from out.Object (IsExpressionAvailable's CDefHolder pattern) or the slot itself, with an explicit
  release on the success path re-reading the slot. Try the 006ac390 CDefHolder recipe.
- `&this->Components` is materialised once and homed in a stack slot ([esp+0x18]) -> a named `map` local CSE'd across 0x11/0x6f/0x29 lookups (edi);
  the item's own map (`lea edi,[ebp+0x44]`) is separate and transient.
- AddToInventory takes NInventory::CTCInventoryBase& (draft passes CTCInventory*); same bytes but wrong spelling.
- Fake GetDef body + FindDef_CDefinitionManager_GetDef_c430 stub is not reconstruction -- remove; use the header decl (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF).
- Const-correctness: IsExpressionAvailable, PeekDefinitionManager, GetDefGlobalIndexFromName, GetDefClassTypeNameFromGlobalIndex, GetQuickAccessNextFreeSlot, GetTutorialCategory are all const (QBE).
- Control flow of the draft otherwise matches (nested ifs, shared fail release, `quickAccessSlot != -1 || (slot = Next(...)) != -1`, double GetTutorialCategory call).

## 0x0076C9A0

# DOSSIER 0x0076C9A0 CTCEnemy::OnSerialise (607 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 0076c9a0 0x26f)
- thiscall, `ret 8` (two stack args), NO frame pointer (`sub esp,14h ; push ebx,ebp,esi,edi`) -> /Oy on (do NOT use optimize("y",off)), no SEH, /O2-style (two epilogues, one per branch).
- Registers: esi=this, edi=context (loaded `mov edi,[esp+2Ch]` = arg1), ebx=0 / temp-destruct flag word, ebp=0 constant then the enemy def pointer.
- Frame (offsets from esp after the 4 pushes): +13 byte `false` default; +14 save-branch factionName; +18 thingDef (save) / copied default (load);
  +1C zeroed AT ENTRY (`mov [esp+20h],ebx` after one push, before first Transfer) then reused as CCharString temps; +20 CDefString sret;
  +28 = the dead CONTEXT ARG slot: holds the `true` default, the enemy-def out handle, the `differs` byte, GetDefaultVal local (save) and factionName (load); +2C bool arg never read.
- Flow: Transfer("FriendsWithEverythingFlag",+0C,false) ; Transfer("EnableFollowersEnemyProxy",+0D,true) (both call 0x004045C0);
  inline IsSaving (`[edi+18h]==1 || ==3`, else jne to load branch 0x76CB79).
  SAVE: thingDef=0; GetDefinitionManager().GetDef(Thing->DefGlobalIndex(movzx word +8C), &thingDef); enemy out=0 (+28); factionName ctor (+14);
   if (PFaction) { found = thingDef->GetSubDef(&out); ebp = out.Object (loaded on BOTH found/not-found paths);
     if found: temp CCharString copy of *PFaction (+1C, flag bit1 via `mov ebx,1`), GetDefinitionManager().GetDefNameFromGlobalIndex(enemyDef->Faction(+28)) (sret +20), CDefString::operator!=(temp) }
   differs byte stored at +28 (`mov [esp+28h],bl` = 1 / 0) ; flag-guarded dtor of temp (test bl,1);
   if differs: esi=PFaction reload; ternary PFaction ? CCharString(*PFaction) [+28, flag 2] : CCharString("",-1) [+1C, flag 4]; factionName = (0x99EFB0); dtor flag4 then flag2.
   GetDefaultVal<CCharString> inline: local ctor +28, copy into +1C, dtor +28; Transfer("FactionName", factionName, default) 0x004109A0; dtor default, dtor factionName;
   release enemy def via ebp (test/dec [ebp+4]/jne/call [vtbl+4]); release thingDef reloaded from +18 into ecx (same compact form). epilogue.
  LOAD: factionName ctor at +28 (context slot); GetDefaultVal local +1C copied to +18, dtor +1C; Transfer("FactionName", factionName(+28), default(+18)); dtor default;
   if (factionName != "") (0x99E960, "" = 0x0122D70E) PFaction = PeekWorld().PFactionManager(+54)->GetPFaction(factionName); dtor factionName.
- The ebx flag pattern (bit1 for the comparison temp) = temporaries inside a short-circuit `&&` expression; `differs` stored from bl => likely `bool differs = PFaction && thingDef->GetSubDef(&e) && GetDefNameFromGlobalIndex(..) != <name>`.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x0077AB00) and FableWin.pdb (_fablewin_api.tsv + naming_stragglers 0x0182A550 / 0x02605760):
  `?OnSerialise@CTCEnemy@@MAEXAAVCPersistContext@@_N@Z` => protected virtual `void CTCEnemy::OnSerialise(CPersistContext&, bool)`.
- Vtable: rtti-vtables.tsv CTCEnemy vtable 0x0126A47C slot 1 = 0x0076C9A0 (xref.py: only reference is .rdata 0x0126A480). Manifest label correct; functions.tsv already says (CTCEnemy*, CPersistContext*, bool).
- Shared header rebuild/include/engine/CTCEnemy.h already declares `protected: virtual void OnSerialise(CPersistContext&,bool);`.
- OLD DRAFT ERROR: declares `(CPersistContext* context, long version)` -- must be `CPersistContext&` and `bool`.

## 3. Callees (landed file / landed name / PDB name)
- 0x004045C0 00/40/CPersistContext_Transferbool_004045c0.cpp  Transfer<bool>(const char*,T&,const T&) ; PDB ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z. Draft spelling OK (header template).
- 0x004109A0 00/41/CPersistContext_TransferCCharString_004109a0.cpp ; PDB ??$Transfer@VCCharString@@@CPersistContext@@QAEXPBDAAVCCharString@@ABV1@@Z.
  The 2-arg ??$Transfer@VCCharString@@@CPersistContext@@QAEXPBDAAVCCharString@@@Z (Ego_r 0x0040DAD0) is INLINE in retail = `Transfer(name,value,GetDefaultVal<T>())` -> use `context.Transfer("FactionName", factionName);` with FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (CPersistContext.h:92). Draft's local `EmptyString()` is this.
- 0x006869D0 00/68/CTCBase_GetDefinitionManager_006869d0.cpp: `const CGameDefinitionManager& CTCBase::GetDefinitionManager() const` (PDB ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ). DRAFT WRONG: `GetDefinitions()` returning pointer.
- 0x0042AF3C 00/42/CDefinitionManager_GetDefT_0042af3c.cpp: CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) const (header CDefinitionManager.h:51). Out of line, unaligned, frame-pointer+/Os COMDAT from another TU -> keep declaration-only (do NOT enable FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY). T = CThingBaseDef (draft) plausible.
- 0x006C7720 00/6c/CDefClassBase_GetSubDefCEnemyDef_006c7720.cpp ; PDB ??$GetSubDef@VCEnemyDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEnemyDef@@@@@Z (Ego_r 0x006FEC00). Header CDefClassBase.h:66 template. Draft OK (pointer arg).
- 0x009ACCC0 00/9a/CDefinitionManager_GetDefNameFromGlobalIndex_009accc0.cpp ; PDB ?GetDefNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z (const, returns CDefString by value, sret). Header CDefinitionManager.h:68. OK.
- 0x0076D640 00/76/CDefString_operator_0076d640.cpp ; PDB ??9CDefString@@QBE_NABVCCharString@@@Z (Ego_r 0x0077B970, inside the CTCEnemy run 0x77AA80..0x77BB30 => emitted in THIS TU). NOT declared in engine/CDefString.h (only `operator!=(const char*)` under FABLE_RETAIL_EXPRESSION_FRAME, line 20) -> needs a new opt-in or a declaration. Draft's local `PEntry` member: PDB name is `long TablePos`.
- 0x00686A80 00/68/CTCBase_PeekWorld_00686a80.cpp: `const CWorld& CTCBase::PeekWorld() const` (PDB ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ). DRAFT WRONG: `GetWorld()` returning pointer.
- 0x006B93D0 00/6b/CFactionManager_GetPFaction_006b93d0.cpp: `CFaction* CFactionManager::GetPFaction(const CCharString&) const` (PDB ?GetPFaction@CFactionManager@@QBEPAVCFaction@@ABVCCharString@@@Z). DRAFT WRONG: `FindFaction`. Shared engine/CFactionManager.h:9 declares it.
- CCharString: 0x0099E4B0 ctor(), 0x0099EBF0 ctor(const char*,long=-1), 0x0099EC30 copy ctor, 0x0099EAE0 dtor, 0x0099EFB0 operator= (returns const CCharString&), 0x0099E960 operator!=(const char*) const -- all in fable_string.h (FABLE_RETAIL_CCHARSTRING). Draft's operator= return type wrong (CCharString& vs const CCharString&).
- vtable calls: [vtbl+4] slot 1 of the def pointee (Destroy on zero count) for both def releases.

## 4. Members
- this+0x04 CTCBase::ParentThing (CThing*) (CTCBase.h); CThing+0x8C `unsigned short DefGlobalIndex` (CThing.h:83, accessor GetDefGlobalIndex() line 267 returns unsigned long; FABLE_RETAIL_CTHING).
- this+0x0C bool FriendsWithEverythingFlag, +0x0D bool EnableFollowersEnemyProxy, +0x10/+0x14 std::list EnemyThings/AllyThings (retail 4 bytes each), +0x18 `const CFaction* PFaction` (CTCEnemy.h; sizeof==28 asserted). PDB (struct_layouts_egor) puts PFaction at +0x28 only because Ego_r lists are 12 bytes.
  CTCEnemy.h opt-ins: FABLE_RETAIL_CTCENEMY_QUERIES (GetPFaction, IsFriendsWithEverything inlines), FABLE_RETAIL_CTCENEMY_FACTION. Header PFaction is `const CFaction*`; GetPFaction returns non-const CFaction* -> assignment fine.
- CEnemyDef+0x28 `long Faction` (PDB; engine/CEnemyDef.h, FABLE_RETAIL_CENEMYDEF). Draft name `FactionGlobalIndex` wrong.
- CFaction+0x00 `CCharString Name` (PDB CFaction 36 bytes: Name, AlliedFactions set, EnemyFactions set). No engine/CFaction.h.
- CWorld+0x54 `PFactionManager_ptr` (scoped_ptr<CFactionManager>), accessor `GetFactionManager() const` under FABLE_RETAIL_CWORLD_FACTIONMANAGER (CWorld.h:207) -- likely the source spelling `PeekWorld().GetFactionManager().GetPFaction(name)`.
- CPersistContext+0x18 `EMode Mode`; inline IsSaving() (default, not FABLE_RETAIL_CPERSISTCONTEXT_ISSAVING_OUTOFLINE).
- Draft models LOCALLY (shared headers exist): CCharString (fable_string.h), CDefClassBase, CDefPointer, CEnemyDef, CThingBaseDef, CDefString, CDefinitionManager, CThing, CFactionManager, CWorld, CPersistContext, CTCEnemy (+CTCBase). Only CFaction has no header.

## 5. Callee visibility
- CDefString::operator!=(const CCharString&) 0x0076D640: emitted in this TU (retail address inside the CTCEnemy block, after the std::find helpers; Ego_r 0x0077B970 inside the CTCEnemy run) -> it was an inline/header body visible to the compiler but emitted out of line. Consider a same-TU definition (auto_inline(off) or after OnSerialise); try both.
- GetSubDef<CEnemyDef> 0x006C7720, GetDef<T> 0x0042AF3C: other TUs (/O2 ret 4; /Os+frame). Chunks 28/32/34/35 found a same-TU `auto_inline(off)` GetSubDef stand-in fixed register caching; here retail RELOADS the out handle after the call (`mov ebp,[esp+28h]` on both paths) -- consistent with an opaque call, but test the stand-in if regalloc differs. Body opt-in: FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (CDefinitionManager.h:175).
- GetDefaultVal<CCharString> inlined (no NRVO: local, copy, dtor) -- via NAMED_DEFAULTS 2-arg Transfer.
- IsSaving, GetFactionManager, CDefPointer ctor/dtor inline.

## 6. Siblings
- 00/7a/CTCAIScratchpad_OnSerialise_007abbd0.cpp: FABLE_RETAIL_CTCBASE, CCHARSTRING, CPERSISTCONTEXT, CPERSISTCONTEXT_NAMED_DEFAULTS; shared CTCBase/CThing/CPersistContext headers (the named-default Transfer idiom).
- 00/77/CTCShop_OnSerialise_00775c60.cpp and 00/6d/CTCWife_OnSerialise_006d2bd0.cpp: FABLE_RETAIL_CCHARSTRING, DEF_CLASS_BASE, CDEFSTRINGTABLE, CTCBASE, CRESOURCELIST, CDEFINITIONMANAGER (+CPERSISTCONTEXT); include fable_string.h, engine/CGameDefinitionManager.h, engine/CDefString.h, engine/CPersistContext.h; GetDefinitionManager + CDefString usage.
- 00/76/CTCEnemy_AreThingsEnemies_0076d2f0.cpp: full CTCEnemy opt-in set (CTCENEMY_QUERIES, CTHING, CTCBASE, CCHARSTRING...).
- 00/76/CTCEnemy_SetFaction_0076c810.cpp (local models; `PeekWorld().PFactionManager->GetPFaction(faction)` with the manager in a local).
- None of the CTCEnemy rows use optimize pragmas; no `// Flags:` lines seen in them.

## 7. Old draft model errors
- Wrong signature (CPersistContext*, long) -> (CPersistContext&, bool); wrong callee names GetDefinitions/GetWorld/FindFaction; member names FactionGlobalIndex/PEntry.
- All types modeled locally; CDefPointer local has NO destructor and releases are hand-written -- with shared CDefPointer.h the dtor releases automatically; enemy-def lifetime must still end early (retail reuses its slot +28 for `differs` while keeping the pointer in ebp, and releases it AFTER factionName/default dtors, BEFORE thingDef).
- Destruction order at end (default, factionName, enemy def, thingDef) => declaration order thingDef, [enemy def], factionName. Construction order: thingDef=0, GetDef, enemy out=0, factionName ctor, then `if (PFaction)`.
- Unexplained: the entry store `[esp+1Ch]=0` before the first Transfer (some object zero-initialised at function scope, e.g. a CDefPointer declared before the Transfers). Draft has nothing there; frame 0x18 vs retail 0x14.
- Old residue (NOTES.md line 21): edi/ebp swap + param-slot reuse; tried IsSaving inline, default forms, member order.

## 0x00AF5430

# DOSSIER 0x00AF5430 -- NParticleEngine::CParticleSystem::ClearDecal

## 1. Retail shape (xdis 00af5430 0x26f)
- 0x25F bytes used = 607 (row length; ends `ret 4` at 0x00AF568C, int3 at 0x00AF568F). thiscall, `ret 4` = one arg (the iterator reference).
- Prologue: `push ebp; mov ebp,esp; and esp,-8; sub esp,1Ch` then push ebx/esi/edi. 8-byte-ALIGNED ebp frame. No SEH (no fs:0) even
  though there is a local with a dtor, so either no /EHsc in that TU or the dtor is nothrow-inlined (no unwind state stores seen).
- Frame slots (esp after the 3 pushes): [esp+1Ch]=this (spilled from ecx BEFORE the pushes: `mov [esp+14h],ecx` with 1 push),
  [esp+14h]=group index, [esp+18h]=group*12 (PrimIDList byte offset, `add eax,0Ch` per iteration), [esp+20h]/[esp+24h]= the local
  CEnginePrimitiveHandle (vptr 0x0122F598 / Data), 8-aligned. edi = decal (CDecal*) all function long; [esp+17h] (after 2 pushes) =
  the 1-byte random_access_iterator_tag temp for __copy.
- Lesson from other landed rows: the `and esp,-8` frame comes out of VC7.1 on its own when an 8-byte object local/temporary
  (vptr+pointer, e.g. CIntelligentPointer temps) lives in the frame -- 0x007AA6B0 CTCAssassinRush::OnInitialActivate notes
  "comes out of VC7.1 for this body as written (no double local needed)"; 0x007CA9E0, 0x007AF490, 0x006946C0 likewise. The old draft
  (572 bytes) did NOT get it, so its handle local probably was optimised into registers / not address-taken the same way.
- Control flow: outer `for(group=0;;++group)` is UNROTATED (top test at 0x00AF5470 `size()<=group -> exit`, `jmp` back at 0x00AF55EB),
  loop head 16-aligned by `lea esp,[esp]` padding. Inner primitive loop also unrotated (top test 0x00AF5500, `jmp` back 0x00AF5558).
- Body order per group:
  a) `handle = ParentHandleList[group]` (inline CEnginePrimitiveHandle::operator=): Clear() on the local (refcount==1 -> slot 3
     NotifyReleased(&handle); then dec count, ==0 -> slot 1 Delete), store Data=0, then copy other.Data and `inc [ecx+4]` (AddRef
     inline, non-virtual).
  b) `test ecx,ecx; setne al; test al,al; je exit` -- a MATERIALISED bool from an inline accessor (e.g. `if (handle.IsNull()) break;`
     / `!IsNull()` returning bool) -> breaks the whole loop.
  c) `if (PEngine && <handle non-null>)` -- plain tests (no setne).
  d) inner: `group < PrimIDList.size()` (signed /12 via imul 2AAAAAABh, compared unsigned jbe), `index < PrimIDList[group].size()`
     (sar 2), `id = PrimIDList[group][index]; test; setge dl; test dl,dl` -- materialised `id >= 0` bool from an inline function;
     then `PEngine->ClearChildPrimitive(handle, id)` (vtable +0x78, push id, push &handle; PEngine re-read from this each time).
  e) `if (group < ParentHandleList.size())` (re-test, sar 3): `ParentHandleList[group].Clear()` inline (slot3 if count==1; dec/
     slot1 Delete; Data=0 stored TWICE: 0x00AF5599 inside the null test and 0x00AF55A4 after -- the FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
     Clear + FABLE_RETAIL_CIVCOUNTEDPOINTER Reset(0) shape), then `PrimIDList[group].clear()` = STLport erase(begin(),end()) inline
     with the dead `cmp eax,eax` (last==finish) and memmove 0x00BFEAE6 arm.
- Tail: `ParentHandleList.erase(begin(), end())` INLINE: `__copy` 0x00440260 (__fastcall: ecx=last=finish, edx=finish... see call:
  ecx=[edi+6Ch] moved to edx, args push 0, &tag, first) then loop calling element vtable slot 0 (`push 0; call [eax]` = scalar
  deleting dtor with flag 0, i.e. `p->~CEnginePrimitiveHandle()` virtual), `_M_finish = i`. Then `PrimIDList.erase(begin(),end())`
  OUT OF LINE 0x00AF7910 (push finish, push start, ecx=&PrimIDList). Then the local handle's dtor inline (Clear + ~CBaseClass 0x0099A2E0).
- Prologue calls CBaseClass::CBaseClass 0x0099A2D0 on the local then stores vptr 0x0122F598 and Data=0 (inline handle ctor).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @00ab7f10) AND FableWin (ghidra_out/_fablewin_api.tsv + naming_stragglers
  FableWin.exe.signatures.tsv @017f2272/02f19500, 268 bytes, twin of ClearParticle 268 bytes):
  `?ClearDecal@CParticleSystem@NParticleEngine@@QAEXAAV?$_List_iterator@V?$_List_val@PAVCDecal@NParticleEngine@@V?$allocator@PAVCDecal@NParticleEngine@@@std@@@std@@@std@@@Z`
  => `public: void NParticleEngine::CParticleSystem::ClearDecal(std::list<CDecal*>::iterator &)` -- NON-const reference (AAV).
  Manifest label CParticleSystem::ClearDecal is CORRECT; namespace NParticleEngine. Draft's `const CDecalIterator&` is WRONG (const).
- Callers (xref.py): only 0x00AF569A = landed CParticleSystem::DeleteDecal 0x00AF5690
  (rebuild/src/compiled/00/af/CParticleSystem_DeleteDecal_00af5690.cpp) declaring `void ClearDecal(DecalIterator& it)` (non-const).
  Other xref hits (00af5797, 00af58e9, 00b1dfa8, 00b1fe56) have no recorded caller; probably DeleteAllDecals/ClearAllDecals inline paths.
- Retail containers are STLport: iterator = one node pointer (node: next, prev, data); `it._M_node->_M_data` = [eax] then [ecx+8].

## 3. Callees
| retail | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x0099A2D0 | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp | CBaseClass::CBaseClass | ??0CBaseClass | ok |
| 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | CBaseClass::~CBaseClass (??1CBaseClass@@UAE@XZ) | same | ok |
| vtbl+0x78 slot 30 on CIEngine | engine/CIEngine.h line 104 (FABLE_RETAIL_CIENGINE) | ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) | ?ClearChildPrimitive@CIEngine@@UAEXAAVCEnginePrimi... | WRONG: draft `RemovePrimitive(const handle&, long)` |
| primitive vtbl+0x0c slot 3 | engine/CEnginePersistentPrimitiveBase.h:33 | NotifyReleased(const CEnginePrimitiveHandle&) | CEnginePersistentPrimitiveBase | WRONG: draft `OnLastHandleReleased(CEnginePrimitiveHandle*)` |
| primitive vtbl+0x04 slot 1 | CIVCountedPointeeBase | Delete() const | CIVCountedPointeeBase::Delete | WRONG: draft `Destroy()` |
| handle vtbl slot 0 (`push 0; call [eax]`) | -- | CEnginePrimitiveHandle virtual dtor (scalar deleting, flag 0) | ??1CEnginePrimitiveHandle | ok-ish (draft erase_inline p->~T()) |
| 0x00440260 | 00/44/std_copyCEnginePrimitiveHandleCEnginePrimitiveHandle_00440260.cpp | std::__copy<CEnginePrimitiveHandle*,..,int> (__fastcall, ret 0Ch) | STLport __copy | draft declares free __fastcall `__copy` -- ok, but should be in namespace std as the landed row |
| 0x00BFEAE6 | 00/bf/global_memmove_00bfeae6.cpp | memmove | CRT | ok |
| 0x00AF7910 | 00/af/stdvector_erase_00af7910.cpp | std::vector<CArray<long> >::erase(iterator,iterator) | ?erase@?$vector@V?$CArray@J@@... | draft names it CVector::erase -- OK in substance; manifest label "Vector_CopyAndDeleteOld" is propagated |
Note: refcount field is CIVCountedPointeeBase::IVRefCount (+4, unsigned long); draft calls it m_handleCount (long).

## 4. Members
- this->PEngine +0x08 (CIEngine*): struct_layouts_egor CParticleSystem: _padding_ +0 (vptr, CBaseClass), PParentEmitter +4,
  PEngine +8. Shared header rebuild/include/engine/CParticleSystem.h (class NParticleEngine::CParticleSystem : CBaseClass; opt-ins
  FABLE_RETAIL_CPARTICLESYSTEM, _UPDATE, _PEEKEMITTERTRANSFORM). Draft models CParticleSystem LOCALLY with `void* __vftable` -- header exists.
- Decals list (Ego +44 = retail 0x20 per landed DeleteDecal) -- not touched here (iterator is the argument).
- CDecal: shared header rebuild/include/engine/CDecal.h with FABLE_RETAIL_CDECAL (polymorphic NParticleEngine::CDecal, sizeof 0x74):
  PrimIDList +0x5C `CArray<CArray<long> >`, ParentHandleList +0x68 `CArray<CEnginePrimitiveHandle>` (public members unless
  FABLE_RETAIL_CDECAL_UPDATE). Draft models CDecal LOCALLY with a pad -- header exists.
- CArray<T> (engine/CArray.h) = public std::vector<T> (STLport, 12 bytes); opt-ins: FABLE_RETAIL_CARRAY (operator[] via begin()+i),
  _AT, _GETASCARRAY_START, _SIZE_CTOR_*, _PUSHBACK*, _DEALLOCATEUNUSEDMEMORY. Use real vector size()/erase()/clear().
- CEnginePrimitiveHandle (engine/CEnginePrimitiveHandle.h; vtbl 0x0122F598, Data +4 private CIVCountedPointer<CEnginePersistentPrimitiveBase>):
  opt-ins FABLE_RETAIL_CENGINEPRIMITIVEHANDLE (inline Clear: refcount==1 -> NotifyReleased, Data.Reset(0)),
  FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR / _CLEAR_DTOR (dtor = Clear()), FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN (operator= with
  Clear() when also FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED), FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA / _ATTACHED (IsNull()).
- CIVCountedPointer (engine/CIVCountedPointer.h): FABLE_RETAIL_CIVCOUNTEDPOINTER, _RESET_FORCEINLINE, _RESET_ADOPT, _ASSIGN,
  _RELEASE_MEMBER, _RELEASE_INNER, _DTOR_DIRECT, _CONVERT, _FROM_RAW; CIVCountedPointeeBase _CTOR, _RELEASE_DELETES.
- CIEngine (engine/CIEngine.h, FABLE_RETAIL_CIENGINE): slot 30 ClearChildPrimitive.
- Locally modelled in the draft though a shared header exists: CBaseClass, CEnginePrimitiveHandle, CIVCountedPointer,
  CEngineInternalPrimitiveBase (really CEnginePersistentPrimitiveBase / CIVCountedPointeeBase), CIEngine, CDecal, CParticleSystem,
  CArray (as CVector).

## 5. Callee visibility
- CBaseClass ctor/dtor: out of line (0x0099A2D0/E0), declaration only.
- __copy<CEnginePrimitiveHandle*> 0x00440260: out-of-line STLport helper (COMDAT far away), called with the tag temp at [esp+17h];
  the 0x00AF7910 erase row notes VC7.1 homes the tag differently when the __copy BODY is visible -- check whether the
  CEnginePrimitiveHandle __copy body should be visible (landed row 0x00440260 has the body).
- vector<CArray<long> >::erase 0x00AF7910: out of line, address AFTER this function (0x00AF7910 > 0x00AF5430) -> same TU (particle
  system .cpp, template instantiation emitted at the end); body probably visible but not inlined (declare + maybe __declspec(noinline)
  / auto_inline(off) like DeleteAllDecals 0x00AF5770 uses `#pragma auto_inline(off)` lines 111-116).
- vector<CEnginePrimitiveHandle>::erase and vector<long>::erase(clear) are INLINED (no call).
- CParticleSystem::ClearParticle 0x00AF5090 (twin, parked) is defined earlier in the same TU; DeleteParticle 0x00AF5330,
  AllocateDecal 0x00AF5380, DeleteDecal 0x00AF5690 (calls ClearDecal out of line), DeleteAllParticles 0x00AF56E0,
  DeleteAllDecals 0x00AF5770 are neighbours.

## 6. Sibling landed rows (rebuild/src/compiled/00/af/)
- CParticleSystem_DeleteDecal_00af5690.cpp: local STLport list model (CRetailList, SListNode next/prev/data), no pragmas, no Flags line.
- NParticleEngineCParticleSystem_DeleteAllDecals_00af5770.cpp: `#pragma auto_inline(off)` / `(on)` around helpers.
- NParticleEngineCParticleSystem_DeleteAllParticles_00af56e0.cpp: `// Flags: none`, `#pragma auto_inline(off/on)` lines 88-94.
- NParticleEngineCParticleSystem_AllocateDecal_00af5380.cpp: includes engine/C3DVector.h, <new>.
- stdvector_erase_00af7910.cpp (the PrimIDList erase callee, same TU).
- Closest idiom sibling for handles: 00/b1/NParticleEngineCPSCDecalRenderer_ClearAllPrimitives_00b1dbe0.cpp (Clear() =
  `if(Data.GetRefCount()==1) Data->NotifyReleased(*this); Data.Reset(0);` FABLE_FORCEINLINE, real CIVCountedPointeeBase with
  IVRefCount, CIEngine slots by name) and the CPSCLight dtor 0x00B1AEB0 / Kill 0x00B19C20.
- None of the CParticleSystem siblings have a Flags line forcing /Oy- etc.; but THIS row has an ebp frame only because of the
  aligned frame (and esp,-8), not /Oy-.

## 7. Old draft model errors
1. Signature: `const CDecalIterator&` -> must be non-const `iterator&` (PDB AAV; landed caller declares non-const).
2. Slot names wrong (RemovePrimitive / OnLastHandleReleased / Destroy) -> ClearChildPrimitive / NotifyReleased / Delete; CIEngine
   slot 30 args are (CEnginePrimitiveHandle&, unsigned long) -- non-const handle.
3. Loops: retail outer & inner loops are unrotated with group index AND group*12 offset in frame slots; draft's `for` with a
   `GetPrimitiveID` helper got rotated loops. The inner test must yield materialised bools (setge) -> an inline bool-returning
   accessor for `id >= 0` (e.g. Ego CDecal::GetPrimitiveID-like) and setne for the handle test (inline IsNull()/IsValid()).
4. Missing 8-aligned frame: handle local must stay a real frame object (address passed to ClearChildPrimitive and to slot3).
5. Handle dtor in erase loop is a VIRTUAL call (`call [eax]` slot 0 with push 0) -- needs the virtual dtor called through vtable,
   i.e. STLport _Destroy calling `p->~T()` on a polymorphic type (draft did this; keep).
6. Clear() stores Data=0 twice (inside null test + after) -> use shared header opt-ins FABLE_RETAIL_CENGINEPRIMITIVEHANDLE +
   FABLE_RETAIL_CIVCOUNTEDPOINTER (Reset(0)), not the draft's single-store Release.
7. Local models of CDecal/CParticleSystem/CEnginePrimitiveHandle/CIEngine/CArray must be replaced by the shared headers.

## 0x00B68DA0

# DOSSIER 0x00B68DA0 CEngineLandscapeRenderer::RenderSortedPatches (607 bytes)

## 1. Retail shape (xdis 00b68da0 0x26f)
- __thiscall, no args, plain `ret` (two tail-duplicated epilogues `add esp,10h ; ret` at 0xB68FEA/0xB68FFE).
- NO frame pointer: `sub esp,10h ; push ebx ; ... push ebp/esi/edi` (ebp is a GPR). No SEH prologue (no /EHsc-visible unwind; RAII objects are trivially inlined). Default /O2 /Oy shape, no pragmas needed.
- Frame 0x10: [esp+11h] bool occluded temp, [esp+12h] old VS enable, [esp+13h] old PS enable (adjacent bytes), [esp+14h..1Bh] 8-byte T() temp for resize (zeroed `xor eax ; mov [14h],eax ; mov [18h],eax`) reused as `long queryHandle` in render loop; [esp+1Ch] = spilled `this`.
- Constant 1 is CSE'd in ebx after SetupCommonStates: used for the ShadowMode compare, `or [ecx+0E4h],ebx`, and `push ebx` (= `true` arg of Patch->Render). `or ebx,-1` hoisted for ZSorterHandle = -1 in loop 1.
- Order at end: PostRenderClearRenderStates -> VS restore -> PS restore -> RestoreCaptureBlock (0xA05840 via [0x13CB508]) LAST.

## 2. Identity
- Name: `?RenderSortedPatches@CEngineLandscapeRenderer@@AAEXXZ` (PRIVATE, void, no params, non-const). Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x009AA2B0), ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv (FableWin 0x02C87E20). Manifest label correct (rebuild/manifest/functions.tsv: __thiscall void).
- Only caller: 0x00B6B136 inside landed CEngineLandscapeRenderer::Render 0x00B6B0B0 (rebuild/src/compiled/00/b6/CEngineLandscapeRenderer_Render_00b6b0b0.cpp) -- that row declares it public locally; PDB says private (AAE).
- Not virtual.

## 3. Callees (landed row / PDB)
| addr | landed row | PDB name | draft spelling |
|---|---|---|---|
| 0x00987FE0 | 00/98/CShaderRenderManager_EnableVertexShaders_00987fe0.cpp | ?EnableVertexShaders@CShaderRenderManager@@QAEXXZ | ok |
| 0x009880E0 | .../CShaderRenderManager_DisableVertexShaders_009880e0.cpp | ?DisableVertexShaders@... | ok |
| 0x00988110 | .../CShaderRenderManager_EnablePixelShaders_00988110.cpp | ?EnablePixelShaders@... | ok |
| 0x00988190 | .../CShaderRenderManager_DisablePixelShaders_00988190.cpp | ?DisablePixelShaders@... | ok |
| (inline) | header CShaderRenderManager.h:83-84 | ?SetVertexShadersEnable@CShaderRenderManager@@QAEX_NPA_N@Z / ?SetPixelShadersEnable@... | DRAFT WRONG: SetVertexShadersEnabled / SetPixelShadersEnabled |
| (inline) | CRenderStateManager.h:143 | ?StartCaptureBlock@CRenderStateManager@@QAEXPAX@Z (via ??0CRenderStateScopedCaptureBlock@@QAE@XZ) | DRAFT WRONG: CreateCaptureBlock |
| 0x00A05840 | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ (via ??1CRenderStateScopedCaptureBlock) | ok |
| 0x00B6C840 | 00/b6/stdvector_Mfillinsert_00b6c840.cpp | ?_M_fill_insert@?$vector@UCSubPatchSorterEntry@CEngineLandscapeRenderer@@...@std@@QAEXPAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@IABU34@@Z | draft: on fake CSortedPatchVector (wrong element type name) |
| (inline) | stlport/fable_containers.h:555/561 | ?resize@?$vector@UCSubPatchSorterEntry@...@std@@QAEXI@Z (Ego_r; out-of-line copies 0x00B6CB80/0x00B6CC90 exist in OTHER TUs; here expanded) | draft hand-rolled |
| 0x00B67480 | 00/b6/CEngineLandscapeRenderer_SetupCommonStates_00b67480.cpp | ?SetupCommonStates@CEngineLandscapeRenderer@@AAEXXZ (private) | ok |
| (inline) | CEngineLightingManager.h:157 | ?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z | DRAFT WRONG: SetFadedShadowDiffuseFactorEnabled |
| (inline?) | not in header | ?PeekGlobalShadowMode@CEngineLightingManager@@QBE?AW4ELightingManagerShadowMode@@XZ (Ego_r/FableWin) - likely source of the ShadowMode read | draft reads ShadowMode directly |
| 0x00B46C80 | NOT landed (manifest functions.tsv: CEngineLightingManager::UpdateGlobalLightingStates); declared CEngineLightingManager.h:93 | ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ | ok |
| 0x00BE7BE0 | 00/be/CLandscapeBackgroundPatch_Render_00be7be0.cpp | ?Render@CLandscapeBackgroundPatch@@QAEX_NAAJAA_N@Z = Render(bool withOcclusion, long& queryHandle, bool& occluded) | DRAFT WRONG: pointer params (long*, bool*) and names sortKey/flag |
| 0x00B67510 | 00/b6/CEngineLandscapeRenderer_PostRenderClearRenderStates_00b67510.cpp | ?PostRenderClearRenderStates@CEngineLandscapeRenderer@@AAEXXZ (private) | ok |

Globals: 0x01436E14 g_pShaderRenderManager (CShaderRenderManager*; labels_*.tsv also has GPShaderRenderManager = ?GPShaderRenderManager@NEngineInternal; 42 landed rows use g_pShaderRenderManager); 0x013CB508 CRenderStateCaptureBlock::StateManager (static, CRenderStateCaptureBlock.h:24; draft wrongly calls it g_pRenderStateManager); 0x01436E9C g_pEngineLightingManager (settled spelling, CEngineLightingManager.h:6).
NOTE: this function goes through the POINTER 0x01436E14, whereas the shared CScopedShadersEnable (engine/CScopedShadersEnable.h) and the landed caller Render 0x00B6B0B0 use the singleton object 0x013BC470 (`mov al,[13BC800h]`, `mov ecx,13BC470h`). So the shared CScopedShadersEnable does NOT fit this row; use explicit SetVertexShadersEnable/SetPixelShadersEnable calls on g_pShaderRenderManager.

## 4. Members
- this+0x5E4: `CArray<CSubPatchSorterEntry> SortedSubPatchList` (struct_layouts_egor.tsv; engine/CEngineLandscapeRenderer.h under FABLE_RETAIL_CENGINELANDSCAPERENDERER). CArray : public std::vector<T> (stlport/fable_containers.h), so size(), operator[] (via begin()+n under FABLE_RETAIL_CARRAY), resize(n) all exist.
- CSubPatchSorterEntry (nested, CEngineLandscapeRenderer.h): `CLandscapeBackgroundPatch* Patch; long AddedThisFrame:1 (bit0); long IsOcclusionBox:1 (bit1); long OcclusionID:30 (bits 2..)`. Retail `sar eax,2` confirms signed OcclusionID. Draft names Registered/RenderFlag/SortKey are WRONG (and its unsigned bitfields).
- Header lacks declarations of RenderSortedPatches / SetupCommonStates / PostRenderClearRenderStates (grep finds none) -> a new opt-in is needed (e.g. a FABLE_RETAIL_CENGINELANDSCAPERENDERER_RENDER block, private per PDB), like the existing _ZSORT / _BLENDTABLES opt-ins.
- CLandscapeBackgroundPatch +0x100 `long ZSorterHandle` (CLandscapeBackgroundPatch.h:59, needs FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH; Render(bool,long&,bool&) under FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_RENDER).
- CShaderRenderManager +0x390 VertexShadersEnabled, +0x391 PixelShadersEnabled (CShaderRenderManager.h:279-280, no macro).
- CRenderStateManager +0x2808 StateStackSize, +0x2814 CurrentBookmarkMask, +0x3A24 BookmarkDummyState, StateStack +0x08 (CRenderStateManager.h, FABLE_RETAIL_CRENDERSTATEMANAGER; CRenderStateCaptureBlock.h defines it automatically).
- CEngineLightingManager +0xCE FadedShadowDiffuseFactorEnabled, +0xE4 StateUpdateFlags, +0x4694 `ELightingManagerShadowMode ShadowMode` (needs FABLE_RETAIL_CENGINELIGHTINGMANAGER + FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS). Enum has no enumerators: compare with `(ELightingManagerShadowMode)1`.
- Locally modelled in draft though shared headers exist: CRenderStateManager, CShaderRenderManager, CEngineLightingManager, CLandscapeBackgroundPatch, CEngineLandscapeRenderer, CShaderStatesScope (= PDB CScopedShadersEnable, header exists but wrong global for this row), CSortedPatchEntry (= CSubPatchSorterEntry), CSortedPatchVector (= CArray/std::vector).

## 5. Callee-body visibility
- Inline (expanded): SetVertexShadersEnable/SetPixelShadersEnable (header inline), StartCaptureBlock (FABLE_FORCEINLINE), SetEnableFadedShadowFactor (FABLE_FORCEINLINE), vector::resize(n) -> resize(n,T()) -> erase(range) (__copy loop, n always 0) / _M_fill_insert call. Do NOT define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE, _RESIZE_ONE_DECL, or _ERASE_RANGE_OUTOFLINE (retail expands them here).
- Out of line: _M_fill_insert 0x00B6C840 (declared only in fable_containers.h:564 - good), SetupCommonStates/PostRenderClearRenderStates (same TU, neighbouring addresses 0x00B67480/0x00B67510; called not inlined), UpdateGlobalLightingStates, RestoreCaptureBlock, Patch::Render.

## 6. Siblings
- 00/b6/CEngineLandscapeRenderer_RegisterSubPatchForZSorting_00b67e30.cpp: `#define FABLE_RETAIL_CENGINELANDSCAPERENDERER` + `_ZSORT`, include engine/CEngineLandscapeRenderer.h, uses SortedSubPatchList.size()/push_back/[] and the bitfield names. Closest idiom.
- 00/b6/CEngineLandscapeRenderer_CEngineLandscapeRenderer_00b67920.cpp: `// Flags: none`, FABLE_RETAIL_CENGINELANDSCAPERENDERER/CENGINEWORLDMAP/CENGINEMAP.
- 00/bb/CEnginePrimitiveManagerParticleGroup_RenderSubPrimitive_00bbc460.cpp: `// Flags: none`, FABLE_RETAIL_CENGINELIGHTINGMANAGER, includes CRenderStateCaptureBlock.h, CScopedShadersEnable.h; `CRenderStateScopedCaptureBlock capture;` idiom.
- 00/b3/CEnginePrimitiveRenderer_PreRenderLayer_00b32ad0.cpp: uses g_pShaderRenderManager (0x01436E14) shader enables (local model).
- 00/b6/CEngineLandscapeRenderer_Render_00b6b0b0.cpp: the caller; local models (no shared headers).

## 7. Draft model errors vs disassembly
- Scope order: retail restores shaders BEFORE RestoreCaptureBlock while starting the capture block AFTER the shader enables. Natural spelling: `bool oldVS, oldPS; g_pShaderRenderManager->SetVertexShadersEnable(true,&oldVS); ->SetPixelShadersEnable(true,&oldPS); CRenderStateScopedCaptureBlock capture; ... PostRenderClearRenderStates(); g_pShaderRenderManager->SetVertexShadersEnable(oldVS,0); ->SetPixelShadersEnable(oldPS,0);` (capture dtor runs last). Equivalent to draft's inner-block, but with real classes.
- Loop 1 counters: retail i=esi, count=ecx; `cmp esi,ecx ; jle` is a SIGNED i>count test, loop test `jb` unsigned vs size(). Use `long count = 0` and an index whose compare with size() is unsigned (e.g. `for (unsigned long i...)` with `(long)i > count`, or `long i` compared to size() -> unsigned). Draft's unsigned count likely caused count-in-eax.
- Loop 1 body: retail reloads begin() each iteration (`mov eax,[edi]`), copies Patch, then flags = (src & ~3) | (dst & 2) i.e. dst.OcclusionID = src.OcclusionID; dst.AddedThisFrame = 0 (IsOcclusionBox of dst kept), then `list[count].Patch->ZSorterHandle = count; ++count`. Else-branch: `entry.AddedThisFrame = 0; ++count`. Not-added: `if (entry.Patch) entry.Patch->ZSorterHandle = -1`.
- resize: `SortedSubPatchList.resize(count)` via the shared STLport header (T() zeroed temp at [esp+14h], erase copy-loop + finish store, else _M_fill_insert(end, n-size, tmp)).
- Shadow bool: retail `mov ebx,1 ; cmp edx,ebx ; mov al,1 ; je ; xor al,al` not sete. Try `SetEnableFadedShadowFactor(PeekGlobalShadowMode() == (ELightingManagerShadowMode)1)` with an inline const accessor (needs a header opt-in; PDB ?PeekGlobalShadowMode@CEngineLightingManager@@QBE?AW4ELightingManagerShadowMode@@XZ), or `? true : false`.
- Render loop: entry pointer esi = &list[j] (lea), `long q = entry.OcclusionID` (sar 2) stored to [esp+14h]; `bool occ = entry.IsOcclusionBox` extracted `and ecx,2 ; shr ecx,1` (retail; draft got shr/and -- note the 1-bit field is SIGNED long in PDB; `!= 0` / bool conversion gives and+shr); call Patch->Render(true, q, occ) by reference; write back `entry.OcclusionID = q; entry.IsOcclusionBox = occ;` (and 3|q*4 then xor/and 2/xor merge).

## 0x00941690

# DOSSIER 0x00941690 -- CAIStateGroup_KidPlayHopscotch::Update (609 bytes)

## 1. Retail shape (xdis 00941690 0x271)
- thiscall, `ret` (no args), NO frame pointer (`sub esp,0C0h ; push ebp ; push esi ; push edi`), no SEH/EH frame, no /Oy-.
- Frame 0xC0. Body-level esp offsets (after the 3 pushes): +0x0C float r (rsqrt estimate), +0x12 bool `moving` (MoveToPos bool&,
  `lea eax,[esp+2Ah]` after 6 pushes), +0x13 HasTC byte (materialised, never read), +0x14 squared length, +0x18 spilled X*r,
  +0x20 dz (fst), +0x24 the CCreatureAction_PlayHopscotch temporary (0xA8 bytes: 0x24+0xA8 = 0xCC-0xC = frame end).
  => sizeof(action) MUST be 0xA8. The draft's action model is 4+0x98 = 0x9C -> exactly the draft's "frame -0xc".
- Loop heads aligned: `mov edi,edi` @0x94169E pads loop 1 head to 0x9416A0; `lea ecx,[ecx]` @0x94176D pads loop 2 head to 0x941770.
- ebp = &PHopscotchBoard (this+0x15) for all GetPItem calls; esi = this.
- Control flow: loop1 { if !GetPItem -> T; if GetPItem->+0x91&1 -> T; HasTC(0x2d) (bit 13 of +0x24) -> byte store; if set:
  INLINE lower_bound loop over +0x44/+0x48 (8-byte entries, key const 0x2d, `cdq;sub;sar` halving, `jge` on mid->first<key)
  -- result in edi DISCARDED (no end/greater select, no store); MoveToPos(2,StartPos,moving,5,1,1,0,1,0.0f); ==4 -> loop1; !=2 -> T }
  then IS_THING_ALIVE (GetPItem twice, +0x91) -> T; loop2 { if !GetPItem -> T; LookAtPos(2,FacePos,1,0x3C638E39=1/72) ==4 loop2; !=2 T }
  then GetPItem==0 -> T; PerformActionAndWait(PlayHopscotch(PBrain->+0x18), false) (temp dtor 0x693EF0 after, result in edi) !=2 -> T;
  pos = creature->GetPos() (PBrain->+0x18, call 0x4C73D0); d = pos - StartPos; Normalise; dot with HopscotchForwardVec;
  `fcomp 0.9 ; test ah,41h ; jne DoNothing` ( <= 0.9 -> DoNothing ); else second PerformActionAndWait, !=2 -> T; DoNothing 0x97E200; T: Terminate 0x8FCF40.
  All failure exits share ONE `mov ecx,esi ; call Terminate` tail at 0x9418E0 (DoNothing falls into it).
- x87: d components loaded X,Y,Z (dx deepest); squared sum Z*Z + Y*Y then + X*X (= default GetSquaredMagnitude-ish Z,Y,X load
  order; dz kept via `fst [esp+20h]`). rsqrt = GFFastOneOverSqrt (table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C).
  Then scale: X*r spilled to [18], Y*r in reg, Z*r from [20]; dot formed Z*Fwd.Z(+0x44), Y*Fwd.Y(+0x40) add, then X*Fwd.X(+0x3C) add
  (member Fwd is the memory operand) -> candidate C3DVector opt-in FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX or default Dot with
  operands swapped (`offset.Dot(HopscotchForwardVec)` vs `HopscotchForwardVec.Dot(offset)`); test both.

## 2. Name / signature
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00965670) and FableWin.pdb (_fablewin_api.tsv 0x01847EE3):
  `?Update@CAIStateGroup_KidPlayHopscotch@@MAEXXZ` -> `protected: virtual void __thiscall CAIStateGroup_KidPlayHopscotch::Update(void)`.
- rtti-vtables.tsv: CAIStateGroup_KidPlayHopscotch vtable 0x0128BB90 slot 12 = 0x00941690 (slot 12 = Update per CAIStateGroupBase.h,
  pure in base). xref: only reference is the vtable (.rdata 0x0128BBC0 = 0x128BB90+0x30).
- Manifest label "CAIStateGroup_KidPlayHopscotch::CAIStateGroup_KidPlayHopscotch::Update" is a doubled-class typo; correct leaf above.
  Note: it is MAE (protected virtual), not QAE (siblings' Leaf lines say QAE for OnActivate -- they are MAE in the PDB too).

## 3. Callees
| addr | landed row | PDB name | draft spelling issue |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | draft returns CThing* (PDB: CBaseObject*); use CIntelligentPointer<CThing>::Get() (engine/CIntelligentPointer.h, static_cast of GetPItem) |
| 0x0097EC00 | 00/97/CAIStateGroupBase_AIActionMoveToPos_0097ec00.cpp | ?AIAction_MoveToPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@AA_NW4EMovementType@@_N4PBVCThing@@4M@Z | draft: long return, long repeat, long anim -> must be EAIActionReturn(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*, bool, float), protected |
| 0x0097D580 | 00/97/CAIStateGroupBase_AIActionLookAtPos_0097d580.cpp | ?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z | draft long/long -> EAIActionReturn / EAIActionRepeat |
| 0x00941B20 | 00/94/CCreatureActionPlayHopscotch_CCreatureActionPlayHopscotch_00941b20.cpp | ??0CCreatureAction_PlayHopscotch@@QAE@AAVCThingCreatureBase@@@Z | draft takes CThingCreatureBase* -- PDB takes a REFERENCE |
| 0x0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z | draft takes CCreatureActionBase* + long -> const CCreatureActionBase& , EAIActionReturn; pass the temporary by ref |
| 0x00693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ??1CCreatureActionBase (virtual dtor, Ego_r ??1CCreatureActionBase@@UAE@XZ) | draft declares ~CCreatureAction_PlayHopscotch at it; the derived dtor is implicit, base dtor called directly |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft "GetPosition" -> GetPos (file name is stale) |
| 0x0097E200 | 00/97/CAIStateGroupBase_AIActionDoNothing_0097e200.cpp | ?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ | draft void -> EAIActionReturn (CAIStateGroupBase.h already declares it) |
| 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ | ok |
Inline (no call): GetCreature (PBrain->+0x18), IS_THING_ALIVE/IsDead (+0x91 bit0), HasTC(0x2d)+lower_bound, operator-, Normalise/GFFastOneOverSqrt, Dot.

## 4. Members
- this: CAIStateGroup_KidPlayHopscotch (Ego_r sizeof 104): TookActionPoint bool +0x14, PHopscotchBoard CIntelligentPointer<CThing> +0x15
  (unaligned, pack(1) as landed Cleanup/OnActivate), HopscotchStartIndex long +0x20, StartPos C3DVector +0x24, FacePos +0x30,
  HopscotchForwardVec +0x3C, HopscotchRightVec +0x48, CreationFrame +0x54, FramesPerMin +0x58, Fatigued +0x5C, ActivationCount +0x60,
  ActivationFrequency +0x64. NO engine/CAIStateGroup_KidPlayHopscotch.h exists -> local class is allowed (siblings do it); use PDB names.
- base CAIStateGroupBase (engine/CAIStateGroupBase.h, sizeof 0x14): PBrain +0x04. Opt-ins: FABLE_RETAIL_CAISTATEGROUPBASE,
  _MEMBERS (virtual slots incl. Update slot 12 protected, Terminate, AIAction_DoNothing), _MEALACTIONS (AIAction_MoveToPos + AIAction_LookAtPos
  with PDB types), _PERFORMACTION (AIAction_PerformActionAndWait), _GETCREATURE (+ CAIBrain.h FABLE_RETAIL_CAIBRAIN_GETCREATURE: CThingAICreature&).
- CAIBrain (engine/CAIBrain.h): creature +0x18 (GetCreature() returns CThingAICreature& -> binds to the ctor's CThingCreatureBase&).
- CThing (engine/CThing.h): UsingComponents bitset +0x20 (tested word +0x24 bit 13 = 0x2d TCI_SINGLE_USER_ACTION_POINTS, cf. landed
  Cleanup 0x009415B0 CTCUserActionPointBase), Components CVectorMap +0x44 (start)/+0x48 (finish), dead bit +0x91 (IsDead()).
  Opt-ins: FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(id,&ptr)), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (forceinline HasTC -- NOT wanted
  here: retail materialises the byte), FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE (HasTC via `const bool& found=...` materialises the presence bool).
- engine/CVectorMap.h: FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (inline non-const LowerBound, `count/2` cdq-sar halving, compares on keys)
  -- retail here has the loop IN PLACE (no call 0x0040F020), unlike the same-TU Cleanup 0x009415B0 which calls 0x0040F020.
- C3DVector (engine/C3DVector.h): GFFastOneOverSqrt / Normalise / Dot opt-ins (DOT_ORDERED_ZYX, DOT_GROUPED, NORMALISE_* variants).
- CCreatureAction_PlayHopscotch.h: sizeof 0xA8 (flattened). CCreatureActionBase.h exists.
- Locally modelled in the draft though a shared header exists: C3DVector, CThing, CBaseIntelligentPointer, CCreatureActionBase,
  CCreatureAction_PlayHopscotch, CAIBrain, CAIStateGroupBase (and an ad-hoc CInterfaceMap instead of CVectorMap).

## 5. Callee-body visibility
- Same TU (retail neighbours, all CAIStateGroup_KidPlayHopscotch / Kid* AI file): InitStateGroup 0x00941520, OnActivate 0x00941550,
  Cleanup 0x009415B0 (earlier), IsRunnable 0x00941900 (pending, after), CCreatureAction_PlayHopscotch ctor 0x00941B20 + its methods
  0x00941B90..0x00941CC0 AFTER Update (emitted out of line though defined in the TU -> called, not inlined; declare only).
- LowerBound: out of line (0x0040F020) in Cleanup but expanded in Update -> Update needs the inline body (CVECTORMAP_INLINE_LOWERBOUND
  or a local forceinline lower_bound as in landed 0x009357F0 MinionFaceHero).
- HasTC: plain inline (stack-materialised byte `shr ecx,0Dh ; and cl,1 ; mov [esp+13h],cl`, exactly the CThing.h note on plain inline HasTC).
- Base dtor 0x00693EF0 out of line; GetPos 0x4C73D0 out of line; GetPItem out of line.

## 6. Siblings
- 00/94/CAIStateGroupKidPlayHopscotch_Cleanup_009415b0.cpp: GETTC_EXPANDED + CINTELLIGENTPOINTER, local pack(1) class with PHopscotchBoard at +0x15, inline IS_THING_ALIVE fn.
- 00/94/CAIStateGroupKidPlayHopscotch_OnActivate_00941550.cpp: local pack(1) layout, named locals before pushes.
- 00/94/CAIStateGroupKidReactToBlockage_Update_00941e00.cpp: same-file Update; LookAtPos((EAIActionRepeat)1, pos, 1, 0.013888889f),
  PerformActionAndWait(temp action by const&, false) != 2, `if (failed) {Terminate(); return;}` tail-merge idiom; CINTELLIGENTPOINTER.
- 00/93/CAIStateGroupSeekScriptTarget_Update_0093c030.cpp: full shared-header stack with GETTC_EXPANDED + CVECTORMAP_INLINE_LOWERBOUND +
  IS_THING_ALIVE (engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE) + CAISTATEGROUPBASE_MEMBERS/_GETCREATURE -- best macro template.
- 00/93/CAIStateGroupMinionFaceHero_Update_009357f0.cpp: local FABLE_FORCEINLINE lower_bound loop (`int len`, `len/2`) inlined in an AI Update.
- None of these carry `// Flags:` lines or optimize pragmas (default /O2-style lane flags); no /Oy- or /GX needed here.

## 7. Old draft model errors
1. Action temp too small (0x9C vs 0xA8) -> frame 0xB4 vs 0xC0 ("frame -0xc"). Use the 0xA8 header / sizeof.
2. GetInterface stores the result through an out pointer which is dead -> VC7.1 deleted the whole lookup (550 v 609). Retail keeps the
   HasTC byte store and the lower_bound LOOP but drops the end/greater select: the looked-up value is dead but the loop survived. Try a
   GetTC whose result is unused/dead after HasTC is materialised (`const bool&` style), inline LowerBound via CVECTORMAP_INLINE_LOWERBOUND,
   or a loop the optimizer cannot prove side-effect-free; the board pointer most likely was `CTCUserActionPointBase*` (0x2d), unused.
3. Wrong param/return types (long vs EAIActionReturn/EAIActionRepeat/EMovementType; action passed by pointer vs const&; ctor ptr vs ref).
4. LookAtPos loop in retail checks only GetPItem()!=0 (no dead bit) -- the draft has this right; first loop full IS_THING_ALIVE -- right.
5. Normalise sum order / Dot order must be checked against the x87 sequence above; the local C3DVector should be engine/C3DVector.h.

## 0x009C4440

# DOSSIER 0x009C4440 CFileInstaller::IsFileCopyNecessary (609 bytes)

## 1. Retail shape (xdis 009c4440 0x271)
- __thiscall, `ret 4` (one arg: the counted-pointer reference), no frame pointer (`sub esp,0x24`, ESP-relative), no SEH, no /GS.
- Pushes: esi, edi first; ebx, ebp pushed only after both PathExists succeed (early `return true` tail at 0x9C4697 pops only edi/esi).
- `mov esi,[arg]` = copy.Data; source = Data (SrcPath +0), dest = Data+4 (DstPath). this spilled [esp+8] (later [esp+0x10]),
  source spilled [esp+0x20]->[esp+0x28 after ebx/ebp], dest [esp+0x24]->[esp+0x2c]. source reloaded at 0x9C4604.
- CreateFileW (IAT 0x143FE2C) kept in ebx across both calls; args (src, GENERIC_READ 0x80000000, 1, 0, 3, 0x08000001, 0).
  sourceFile in ebp, destFile in ebx.
- sourceFile==-1: INLINED SetDirtyDiscError(true) (lock on Get().CriticalSection +4, `mov byte [0x13CA814],1`, Get(),
  DEAD LOAD `mov cl,[eax+0x7a]`, Get(), `mov byte [eax+0x7a],1`, lock dtor), then `if (destFile != -1) CloseHandle(destFile)`
  (compiled as `cmp ebx,ebp`), return false.
- destFile==-1: CloseHandle(sourceFile); return true.
- ext CWideString at [esp+0x38] (ctor 0x99AED0); SplitPath(source,0,0,0,&ext) (fastcall: ecx=source, edx=0, push &ext,0,0).
- `ext.ToCharString()` [esp+0x1c-temp] -> `.ToLower()` [esp+0x14-temp] -> `_M_find` 0x9AB5D0 with ecx=edi=this+0x94
  (CompressedFileTypes); node result spilled [esp+0x20]; both CCharString temps destroyed (ToLower one first);
  then `cmp edx,[edi]` (node vs header pointer = end()). `jne` -> close both, skip size check.
- Not found (uncompressed): GetFileSizeEx(sourceFile,&[esp+0x18]) (IAT 0x143FD5C kept in esi). fail -> CloseHandle both,
  OUT-OF-LINE `mov cl,1; call 0x9C3530` (SetDirtyDiscError(true)), ext dtor, return false.
  GetFileSizeEx(destFile,&[esp+0x20]) fail -> close both, ext dtor, return true; `repe cmpsd` ecx=2, esi=&sourceSize,
  edi=&destSize (memcmp intrinsic, 8 bytes; operand order memcmp(&sourceSize,&destSize) -- draft has them reversed) differ -> close both, return true.
- Then CloseHandle(src), CloseHandle(dst) (CloseHandle in edi), `if (this->[+0x81]) return false` (ext dtor shared tail 0x9C45AF).
- Times: GetFileModifiedTimeAsSomeWackyWin32BigNumber(source) (ebx:esi), (dest) (eax:edx), unsigned __int64 compares
  with 30000000 (0x1C9C380 = 3 s in 100ns): `if (dst>=src && dst-src > 3s) true; if (src>dst && src-dst > 3s) true; false`
  (the high-word subtraction is stored to [esp+0x30] = a dead/stack spill of the diff high word).
- Multiple return tails each duplicate ext dtor + epilogue (normal VC7.1 tail duplication).

## 2. Identity (two sources)
- FableWin.pdb (_fablewin_api.tsv 0x0304a880 / 0x01814548, naming_stragglers FableWin.exe.signatures.tsv) and
  Ego_r.pdb (egor_pdb_names.tsv 0x00b12e10) both:
  `?IsFileCopyNecessary@CFileInstaller@@IBE_NAAV?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@@Z`
  => protected: `bool CFileInstaller::IsFileCopyNecessary(CCountedPointer<const CFileInstaller::CFileInfo>&) const`
  (NON-const reference, CONST member function). Shared header rebuild/include/engine/CFileInstaller.h line ~138 declares exactly this.
- Manifest label is right. Only caller: 0x009C541F inside landed CFileInstaller::ProcessReads 0x009C53A0
  (rebuild/src/compiled/00/9c/CFileInstaller_ProcessReads_009c53a0.cpp has a noinline stand-in of this function).
- Draft errors: param type `const CCountedPointer<CFileCopy>&` (should be non-const ref to CCountedPointer<const CFileInfo>),
  method not const, CFileCopy::Source/Dest should be CFileInfo::SrcPath/DstPath.

## 3. Callees (all landed)
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00999230 | 00/99/CAFile_PathExists_00999230.cpp | CAFile::PathExists | ?PathExists@CAFile@@SI_NABVCWideString@@@Z | free `PathExists` WRONG (static member of CAFile) |
| 0x0099AF00 | 00/99/CWideString_operatorconstwcharptr_0099af00.cpp | operator const wchar_t* | ??BCWideString@@QBEPB_WXZ | `operator const unsigned short*` (fable_string.h uses wchar_t) |
| 0x00404440 | 00/40/CTBaseSingletonCFileInstaller_Get_00404440.cpp | CTBaseSingleton<CFileInstaller>::Get | ?Get@?$CTBaseSingleton@VCFileInstaller@@@@SIAAVCFileInstaller@@XZ | `GetCodeSectionManager()` WRONG (returns CFileInstaller&) |
| 0x00A62920 / 0x00A629A0 | 00/a6/CCriticalSectionLock_* | CCriticalSectionLock ctor/dtor | ??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z | draft takes pointer; PDB takes reference |
| 0x009C3530 | 00/9c/CFileInstaller_SetDirtyDiscError_009c3530.cpp | CFileInstaller::SetDirtyDiscError | ?SetDirtyDiscError@CFileInstaller@@KIX_N@Z (protected static fastcall) | draft made it a free __forceinline function |
| 0x0099AED0 | 00/99/CWideString_CWideString_0099aed0.cpp | CWideString() | ??0CWideString@@QAE@XZ | ok |
| 0x00997620 | 00/99/CAFile_SplitPath_00997620.cpp | CAFile::SplitPath (wide) | ?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z | free `SplitPath` WRONG |
| 0x0099B2C0 | 00/99/CWideString_ConvertToCharString_0099b2c0.cpp | ToCharString | ?ToCharString@CWideString@@QBE?AVCCharString@@XZ | `ConvertToCharString` WRONG |
| 0x0099ED10 | 00/99/CCharString_ToLower_0099ed10.cpp | CCharString::ToLower | (not in egor list; landed) | ok |
| 0x009AB5D0 | 00/9a/stdRbtree_Mfind_009ab5d0.cpp | _Rb_tree<CCharString,pair<const CCharString,CCharString>,...>::_M_find<CCharString> | ??$_M_find@VCCharString@@@?$_Rb_tree@...@@ABEPAU?$_Rb_tree_node@...@@ABVCCharString@@@Z (const member template) | draft `CIgnoredExtensionSet::find` (set) WRONG: it is the map CompressedFileTypes |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ??1CCharString@@QAE@XZ | ok |
| 0x0099B510 | 00/99/CWideString_Destructor_0099b510.cpp | ~CWideString | ??1CWideString@@QAE@XZ | ok |
| 0x00999320 | 00/99/CAFile_GetFileModifiedTimeAsSomeWackyWin32BigNumber_00999320.cpp | CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber | ?GetFileModifiedTimeAsSomeWackyWin32BigNumber@CAFile@@SI_KABVCWideString@@@Z (returns unsigned __int64) | `GetFileModifiedTime` returning CDateAndTime WRONG (that is a different PDB function) |
| IAT 0x143FE2C/0x143FE30/0x143FD5C | | CreateFileW / CloseHandle / GetFileSizeEx | import thunks 0x00CB55B8 / 0x00CB55B2 / 0x00CB56F6 | ok |

## 4. Members (retail layout; shared header rebuild/include/engine/CFileInstaller.h, sizeof 0xA0)
- +0x04 `_RTL_CRITICAL_SECTION CriticalSection` (via Get()).
- +0x7A `volatile bool ExitThread` (via Get(); header marks it volatile = retail cv flag). The draft calls it
  CCodeSectionManager::DirtyDiscError -- WRONG. The volatile is exactly what produces retail's dead load `mov cl,[eax+0x7a]`
  in the inlined `if (Get().ExitThread || error) error = true; Get().ExitThread = error;` (landed 0x9C3530 body shape).
- +0x81 `bool IgnoreTimeStampWhenCopying` (draft's "Cancelled" WRONG): `if (IgnoreTimeStampWhenCopying) return false;`.
- +0x94 `std::map<CCharString,CCharString> CompressedFileTypes` (draft's "SizeCheckedExtensions" set WRONG). end() = header ptr at +0.
  Size check runs only when the extension is NOT a compressed type.
- Global 0x013CA814: header has `static bool DirtyDisc;` (CFileInstaller.h line ~182); landed 0x9C3530 local model calls it
  `DirtyDiscError`; Pause 0x9C31E0 calls it g_installerNoPauseWait -- name inconsistency; no PDB name found here (check before naming).
- CFileInfo (header nested class): SrcPath +0, DstPath +4, InputSize, Started, Decompress, CreateMarkerFile.
- Header opt-ins seen used with it (CFileInstaller ctor 0x9C5890): FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CWIDESTRING,
  FABLE_RETAIL_CCOMPRESSORZLIB, FABLE_RETAIL_CACOMPRESSOR, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_STLP_LIST.
  Others needed: FABLE_RETAIL_CAFILE_SPLITPATH_WIDE, FABLE_RETAIL_CAFILE_MODIFIEDTIME_WACKY (engine/CAFile.h),
  FABLE_RETAIL_CCRITICALSECTIONLOCK (engine/CCriticalSectionLock.h, reference ctor), FABLE_RETAIL_STLP_TREE_MFIND_DECL
  (stlport/fable_map.h: out-of-line _M_find decl) + maybe FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (find -> iterator(_M_find)).
- Locally modelled in the draft though shared headers exist: CFileInstaller, CFileInfo (as CFileCopy), CCountedPointer,
  CCriticalSectionLock, CWideString/CCharString (fable_string.h), CAFile, CTBaseSingleton, CDateAndTime, std::map/set.

## 5. Callee visibility
- SetDirtyDiscError 0x9C3530 is in the same TU and defined earlier (lower address, CFileInstaller.cpp order) -> body
  visible; retail inlined it at the first site and called it out of line at the second (0x9C45AA). The draft's
  `#pragma inline_depth(0)` around the second site was the lever for that; keep something equivalent.
- CTBaseSingleton<CFileInstaller>::Get 0x404440 is called out of line (3x in the inlined setter) -- it is a COMDAT far away.
- _M_find 0x9AB5D0 out of line (COMDAT); map::find/end inline.
- CCountedPointer operator-> inline (just `mov esi,[eax]`).
- LESSONS.md (hand287): same-TU noinline stand-ins helped ProcessReads; here nothing in the body looks like a stand-in candidate except
  possibly the out-of-line setter.

## 6. Siblings
- 0x009C46B0 CFileInstaller::IsCompressedFilename (00/9c/CFileInstaller_IsCompressedFilename_009c46b0.cpp): same SplitPath /
  ToCharString().ToLower() / _M_find(CompressedFileTypes at +0x94) / `found != Head` idiom, local models, no pragmas.
- 0x009C3530 SetDirtyDiscError: body shape `if (Get().ExitThread || error) error = true; Get().ExitThread = error;`.
- 0x009C5890 CFileInstaller ctor: `// Flags: none` + the shared-header opt-ins listed above.
- 0x009C53A0 ProcessReads (caller): `#pragma auto_inline(off/on)` noinline stand-ins.
- 0x009C3590 StartReadingFile / 0x009C3660 StartWritingFile / 0x009C5690 ThreadMain: include engine/CCountedPointer.h and
  engine/CCriticalSectionLock.h.

## 7. Draft model errors
- Setter modelled as `x = x || error` on a NON-volatile member of a made-up class; retail = volatile ExitThread (shared header)
  -> the "dead load" residue is the volatile read, not a compiler mystery. Use the shared header / landed 0x9C3530 body shape.
- memcmp operands probably reversed (retail esi=&sourceSize, edi=&destSize).
- Wrong names/types listed in sec. 3/4 (byte-neutral mostly, but const member fn + CCountedPointer header may change codegen).
- "source/dest spill words swapped" residue: retail spills source ptr at the lower slot ([esp+0x20] base) and dest at +4 above;
  try declaring dest before source or taking `const CFileInfo* info = copy.Data` and using info->SrcPath / info->DstPath directly.
- "&CompressedFileTypes not kept in edi across temp dtors": retail computes `lea edi,[this+0x94]` AFTER ToLower and keeps it
  across both CCharString dtors to `cmp edx,[edi]` -- find() and end() on the same map object in one expression
  (`CompressedFileTypes.find(...) == CompressedFileTypes.end()`).

## 0x009F20B0 (not landed)

# DOSSIER 0x009F20B0 CMemoryAllocator::CMemoryAllocator(void* heap)  (598 bytes, 0x009F20B0..0x009F2305)

## 1. Retail shape (xdis.py 009f20b0 0x266)
- __thiscall, `ret 4` (one arg: heap). this -> ebp (`mov ebp,ecx`), returned in eax (`mov eax,ebp`) = ctor.
- NO frame pointer (sub esp,8 ; push ebx,ebp,esi,edi). No SEH, no /GS cookie. Default /O2 shape, no /Oy-.
- Frame: 8 bytes of locals: [esp+0x14] = loop-1 counter i (dword), [esp+0x13] = BYTE spill of the fixed-size bit
  (`mov [esp+0x13],dl`) that is NEVER read again (dead store). Arg heap at [esp+0x1c], re-read from the arg slot at
  each sub-allocator new (`mov ecx,[esp+0x2c]` after 4 pushes). Loop 2's outer counter `type` lives IN THE DEAD heap
  arg slot [esp+0x1c] (compiler overlay, not source).
- ebx = 0 from entry through loop 1 (xor ebx,ebx; used for MemoryAllocated/NoAllocations/CurrentTrackingID stores,
  `cmp [esi-0x14],ebx`, `cmp eax,ebx` new-null tests, `push ebx` = the 0 `void* base` arg of the variable ctor,
  `push ebx` = 0 arg of the vcall). Loop 2 reuses ebx as outer type index then inner allocator index a.
- Order: init-list stores +0x400, +0x40c, +0x414(byte) BEFORE InitializeCriticalSection; then Enter; call
  InitializeMemorySystem; rep stosd 15 dw @+0x424; rep stosd 15 dw @+0x460; inline 15-byte zero @+0x415
  (dword,dword,dword,word,byte via lea eax,[ebp+0x415]); rep stosd 0x100 dw @+0; MemoryAllocated=0 again (+0x400);
  NoAllocators=0x1e (+0x410). Heap (+0x408) is NEVER stored.
- Loop 1: esi = &g_AllocatorInfo[0].InternalFlags (0x0139687C), stride 0x1c, `i < NoAllocators` re-read each iter
  (unsigned jb). Skip if MaxAllocSize==0. edx=InternalFlags>>1&1 (spilled), edi=Flags(type);
  AllocatorExists[type]=1 written BEFORE the branch. Fixed: operator new(0xc4c) -> ctor 0x00A65FF0(heap, MinChunkSize,
  MaxAllocSize, Alignment, type, InternalFlags); store PAllocator (+4 from esi); vcall slot 3 (+0xc) with arg 0;
  result -> MaxAllocSize; `if (MaxAllocSize > MaxFixedAllocationSizes[type])` signed (jle) then store.
  Variable: operator new(0xc38) -> ctor 0x00A65930(heap, MinChunkSize, 0, InitialSize, Alignment, type,
  InternalFlags); store PAllocator. Both new-null arms produce `xor eax,eax` (normal new-expression null check).
- Loop 2: type 0..14 (`cmp ebx,0xf ; jl` signed), edi walks AllocSizeToAllocator[type], [edi-0x3c] =
  MaxFixedAllocationSizes[type] loaded before the AllocatorExists test. __allmul((__int64)(max+2), 8 (64-bit)); 64-bit
  `>=0 && <=0x7fffffff` check; failure jumps to EPILOGUE 0x9F22FA (skips MemoryAuditBuffer=0 and LeaveCriticalSection).
  malloc(max*8+0x10) (lea edx,[esi*8+0x10]); `inc esi; js skip` => size loop `for size=0; size<=max+1`.
  Inner: for a < NoAllocators (re-read, unsigned): info.Flags != type -> next; if !(InternalFlags>>1&1) -> found; else
  minSize = (a>0 ? table[a-1].MaxAllocSize+1 : 0) -- the a>0 test is strength-reduced to a SIGNED pointer compare
  `cmp eax,0x139687c ; jle`; `size<minSize || size>info.MaxAllocSize` (signed) -> next. Found: [size].PAllocator=
  info.PAllocator, [size].ID=(uchar)a, break. Not found: nothing stored.
- Tail: MemoryAuditBuffer(+0x404)=0 with an immediate (`mov dword [ebp+0x404],0` -- ebx no longer 0 here), Leave.
- Loop alignment padding: `nop` at 0x9F213F, `lea esp,[esp]; mov edi,edi` at 0x9F2207 (jmp over) -- normal /O2.

## 2. Name / signature (manifest label is CORRECT)
- `??0CMemoryAllocator@@IAE@PAX@Z` = protected: __thiscall CMemoryAllocator::CMemoryAllocator(void*). Sources:
  ghidra_out/egor_pdb_names.tsv (Ego_r 0x00B45C30) + ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv
  (0x030BCD20, 0x0181B21C). manifest functions.tsv: __thiscall, (CMemoryAllocator*, void*).
- Single caller: 0x009F2897 inside CMemoryAllocator::Create 0x009F2870 (landed
  rebuild/src/compiled/00/9f/CMemoryAllocator_Create_009f2870.cpp, `new CMemoryAllocator(GetProcessHeap())` through
  inline class `??2CMemoryAllocator@@KIPAXI@Z` = VirtualAlloc). Ctor is protected in the PDB.

## 3. Callees
| target | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| IAT 0x0143FEE4/0x0143FE0C/0x0143FE10 | - | Initialize/Enter/LeaveCriticalSection(g_allocatorLock 0x013CAA70) | inline bodies of `?InitSynchronisation@CMemoryAllocator@@KIXXZ`, `?EnterSynchronisation@..KIXXZ`, `?LeaveSynchronisation@..KIXXZ` (protected static __fastcall; out of line in Ego_r 0xB45B60/0xB45B80, inlined in TLC) | raw API calls -- consider calling inline Init/Enter/LeaveSynchronisation (header CMemoryAllocator.h already declares Enter/Leave as non-inline statics) |
| 0x009F1D20 | 00/9f/global_InitializeMemorySystem_009f1d20.cpp | InitializeMemorySystem | `?InitializeMemorySystem@?A0xcf4a15dd@@YIXXZ` (anonymous-namespace, __fastcall, same TU = MemoryAllocator.cpp) | `void __fastcall InitializeMemorySystem()` global -- OK at link level, PDB says anon namespace |
| 0x00A66730 | 00/a6/global_operatornew_00a66730.cpp | `op_new` (__fastcall, VirtualAlloc body) | `??2CMemoryAllocatorBase@@SIPAXI@Z` (public static __fastcall; Ego_r 0x00C085A0) | draft `CMemoryAllocatorBase::operator new` static __fastcall = PDB-correct; landed name `op_new` is WRONG (NAME_FIXES candidate) |
| 0x00A65FF0 | 00/a6/CMemoryAllocatorFixedSize_CMemoryAllocatorFixedSize_00a65ff0.cpp | CMemoryAllocatorFixedSize ctor | `??0CMemoryAllocatorFixedSize@@QAE@PAXKKKKK@Z` (void*, 5x unsigned long) | matches |
| 0x00A65930 | 00/a6/CMemoryAllocatorVariableSize_CMemoryAllocatorVariableSize_00a65930.cpp | ctor | `??0CMemoryAllocatorVariableSize@@QAE@PAXJ0JKKK@Z` (void*, long, void*, long, ulong, ulong, ulong) | matches |
| vslot 3 (+0xc) | FixedSize vtable 0x0129DB64 slot 3 = 0x00A65D40, 00/a6/CMemoryAllocatorFixedSize_GetAllocationSize_00a65d40.cpp | GetAllocationSize | `?GetAllocationSize@CMemoryAllocatorFixedSize@@UAEKPAX@Z` -> virtual unsigned long GetAllocationSize(void*) | draft `GetMaxAllocationSize(long reserved)` WRONG: it is `GetAllocationSize(0)` (NULL void*), returns unsigned long |
| 0x00BFEB50 | 00/bf/global_allmul_00bfeb50.cpp | __allmul (toolchain_library) | CRT | compiler-generated from __int64 multiply |
| 0x00BFEA0E | 00/bf/global_malloc_00bfea0e.cpp | malloc import thunk (toolchain_library) | msvcrt _malloc | OK |
- Note: the vcall's result is stored into a `long` MaxAllocSize and compared signed with MaxFixedAllocationSizes (long[15]).

## 4. Members (PDB struct_layouts_egor.tsv)
CMemoryAllocator (0x49c, no vtable): MemoryAllocatedPerTrackingID ulong[256] +0; MemoryAllocated ulong +0x400;
MemoryAuditBuffer void** +0x404; Heap void* +0x408 (not touched); NoAllocations ulong +0x40c; NoAllocators ulong +0x410;
CurrentTrackingID uchar +0x414; AllocatorExists bool[15] +0x415; MaxFixedAllocationSizes long[15] +0x424;
AllocSizeToAllocator CAllocSizeToAllocator*[15] +0x460.  Draft layout = PDB exactly.
CAllocatorInfo (0x1c): MaxAllocSize long, MinChunkSize long, InitialSize long, Alignment long, Flags ulong, InternalFlags ulong,
PAllocator CMemoryAllocatorBase*.  CAllocSizeToAllocator (8): PAllocator CMemoryAllocatorBase*, ID uchar.
- rebuild/include/engine/CMemoryAllocator.h is a DECLARATION-ONLY projection (no fields; opt-ins
  FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE = Get()/OnFailedAllocation, FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED =
  AllocateAligned/DeallocateAligned). No opt-in offers the fields, the ctor, Init/LeaveSynchronisation or
  CAllocatorInfo/CAllocSizeToAllocator. Every landed CMemoryAllocator row (dtor 0x9F2310, GetAllocationSize 0x9F2390,
  GetHeaderOverhead 0x9F2610, GetNoFreeAreas 0x9F2500, InternalDeallocate 0x9F2920...) models class/CAllocatorInfo
  LOCALLY with the same spellings `g_allocatorLock` (unsigned char[24], 0x013CAA70) and `g_AllocatorInfo[]`
  (0x01396868). No PDB name found for either global. Including CMemoryAllocator.h would conflict with a local class --
  siblings don't include it.
- CMemoryAllocatorBase.h: with FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE it is the real polymorphic class (15 pure
  virtuals, slot 3 = GetAllocationSize(void*)), but declares NO ctor and NO operator new. CMemoryAllocatorFixedSize.h is
  a generated pad-only struct (no ctor). CMemoryAllocatorVariableSize.h (needs the VARIABLESIZE macro) declares no ctor.
  So the ctors + `operator new` must be declared locally or via a #define-rename trick like the fixed ctor row does.
- Draft local models with a shared header: CMemoryAllocatorBase, CMemoryAllocatorFixedSize,
  CMemoryAllocatorVariableSize, CMemoryAllocator (header exists but has no layout).

## 5. Visibility
- InitializeMemorySystem (0x9F1D20) is in the same TU (anon namespace, defined earlier) but large -> called, not inlined.
- Init/Enter/LeaveSynchronisation: inline in retail (IAT calls appear directly) -> original header had inline bodies.
- PDB has `?IsAllocatorFixedSize@CMemoryAllocator@@QBE_NK@Z` (const, takes allocator index) -- likely the original
  spelling of the `InternalFlags>>1&1` test (used in both loops); inline member.
- No STLport calls. __allmul is compiler-generated.

## 6. Siblings (same TU, 0x009F1D20..0x009F2BE0)
- 00/9f/CMemoryAllocator_CMemoryAllocator_009f2310.cpp (dtor): no pragmas, no Flags line, local class + g_AllocatorInfo.
- 00/9f/CMemoryAllocator_Create_009f2870.cpp: local `class CMemoryAllocator{protected: unsigned char _body[0x49c]; ...}`,
  inline class operator new.
- 00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp: #include "fable_compiler.h", calls
  `g_AllocatorInfo[id].PAllocator->GetAllocationSize(...)` (slot 3), raw Enter/LeaveCriticalSection(g_allocatorLock).
- 00/9f/CMemoryAllocator_InternalAllocate_009f2ac0.cpp: `// Flags: /GS` (only TU flag seen); others default.
- 00/a6/CMemoryAllocatorFixedSize_CMemoryAllocatorFixedSize_00a65ff0.cpp: `#define CMemoryAllocatorBase
  CMemoryAllocatorBaseFields` include-rename trick + local true polymorphic base.

## 7. Old draft model errors / suspects (vs disassembly)
- Draft DIFFER 596v598 (2 bytes short): retail keeps ebx=0 and re-reads heap from the arg slot; ours pins heap in ebx.
- The DEAD byte spill [esp+0x13] of the fixed-size bit is the strongest clue: the source reads `fixed` AFTER the
  sub-allocator new (live across calls) and the optimiser threaded that second test away. Try:
  `bool fixed = IsAllocatorFixedSize(i)` (or InternalFlags test); `if (fixed) PAllocator=new Fixed(...); else
  PAllocator=new Variable(...); if (fixed) { MaxAllocSize = PAllocator->GetAllocationSize(0); if (...>MaxFixed[type]) ...}`
  -- this extra live local raises pressure so heap goes to the stack slot and ebx stays the zero register.
- vslot 3 misnamed (GetMaxAllocationSize(long)); real is `unsigned long GetAllocationSize(void*)` with 0.
- Lock calls should probably be inline Init/Enter/LeaveSynchronisation statics (bytes identical, possibly scheduling).
- Second loop `a > 0` compiled as signed pointer compare: keep `a` as a signed long index (draft does).
- Failure of the size check returns from the ctor without LeaveCriticalSection (retail jumps to epilogue) -- draft right.

