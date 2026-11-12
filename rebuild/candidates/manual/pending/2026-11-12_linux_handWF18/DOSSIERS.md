## 0x008A7A70

# DOSSIER 0x008A7A70 CGameScriptInterface::TeleportAllFollowersToHeroPosition (738 bytes)

## 0. Most important: a NEWER draft exists
rebuild/candidates/manual/pending/2026-11-10_linux_hand1012c/differ/008a7a70.cpp -- shared-header rebase, DIFFER(654v738), uses
hdr overlay (FABLE_HDR_OVERLAY=$A/hdr; NEW hdr/engine/CTCFollowed.h in that dir). Start from it, NOT $W/draft_original.cpp
(hand288, local models, DIFFER 721v738). 1012c residue: retail INLINES CTCSummonSpell::GetSummonedCreature (copy ctor) and the
minion-loop push_back; 1012c calls both. /d2inlS no effect; /d2inlT85 over-inlines (702). Old draft reached 721 only by
__forceinline local models (not allowed now).

## 1. Retail shape (xdis 008a7a70 0x2f2)
- __thiscall, virtual, const, no args, plain `ret` (0x008A7D51). No frame pointer (`sub esp,30h` first, ebp used as GP reg) -> /Oy on.
- NO SEH prologue though locals with dtors (vector, CIntelligentPointer) exist -> TU had no EH unwinding for this (same as siblings: default flags).
- Register plan: hero in ebp, spilled immediately `mov [esp+0Ch],ebp` (0x008A7A87); final loop reloads it `mov ecx,[esp+14h]`
  (after push esi/edi) because ebp becomes the index. ebx never used. vector<CThing*> zeroed with immediates
  (`mov [esp+28h],0` x3 at 0x008A7ADD..AED, AFTER LowerBound(0xBC), right before GetFollowers) -> vector declared after the GetTC.
- push esi/edi only after the early-return tests (0x008A7AAB), shrink-wrapped.
- Early exits share one epilogue: !hero, Dead (+0x91 bit0), !HasTC(0xBC) (+0x34 bit 28).
- Block 2: HasTC(0x91) (+0x30 & 0x20000) -> const LowerBound 0x004365B0 -> comp+0x11 ; base ctor 0x00A01B10 on local [esp+18h],
  vptr 0x0123E7E0, GetPItem(src) , SetPItem  == inline CIntelligentPointer copy (GetSummonedCreature inline).
  then GetPItem; test; GetPItem; Dead test; GetPItem -> value homed at [esp+10h] (same slot as the TC key) -> push_back OUT OF LINE
  0x00436C80 ; dtor 0x00A01C10 called directly (non-virtual call of known type).
- Block 3: HasTC(0x9A) (+0x30 & 0x4000000) -> const LowerBound -> comp+0x10 -> vector copy ctor OUT OF LINE 0x008AC540 into
  local [esp+18h]; n = (finish-start)>>3 computed ONCE, signed (`sar 3; test; jle`), loop `dec edi; jne` with esi += 8:
  i.e. `for (long i=0;i<n;++i)` / count-down with hoisted size. push_back INLINE: `finish!=eos`, null test, store, +4; else
  _M_insert_overflow 0x00436AC0 (push 1; push 1; &tag [frame 0xF, 1 byte]; &value [frame 0x10]; finish). dtor 0x00742210.
- Final loop: unsigned index in ebp, size re-read each iteration (`sub; sar 2; cmp ebp,ecx; jb`) -> `for (unsigned i=0;i<followers.size();++i)`.
  Order: hero->GetPos() first (copied to [esp+18h]/[esp+1Ch] = 2D), then follower->GetPos(); squared 2D distance compared with
  4.0f global 0x0123088C (`fcomp; test ah,41h; jne` = skip unless > 4). HasTC(7) by `mov al,[esi+20h]; test al,al; jns`.
  GetTC(7) via const LowerBound 0x004365B0. Args pushed: false, &pos (frame 0x30), 0.8f (0x3F4CCCCD), hero->GetPos() (eval'd last).
  then `if (ok && follower->PhysicsTC) PhysicsTC->vslot31(&pos)` (`call [eax+7Ch]`).
- End: inline ~vector<CThing*>: `if (start) free(start)` 0x00BFEA14.

## 2. Identity (two+ sources)
- Name/leaf: ?TeleportAllFollowersToHeroPosition@CGameScriptInterface@@UBEXXZ (void, const, no params).
  Sources: rtti-vtables.tsv CGameScriptInterface 0x01260F0C slot 239 = 0x008A7A70; shared header CGameScriptInterface.h:280
  (slot 239 0x008A7A70); Ego_r egor_pdb_names (Ego addr 0x008E71B0); FableWin signatures (0x017EBA08 / 0x02A6BB00);
  FSE manifest "TeleportAllFollowersToHeroPosition" void(). Manifest label is CORRECT.
- Only xref: the vtable slot (0x012612C8).

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | PDB / correct spelling | draft_original spelling |
|---|---|---|---|
| 0x00449970 | 00/44/CPlayerManager_GetMainPlayer_00449970.cpp | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (returns CPlayer&) | `CPlayer* GetMainPlayer()` WRONG (pointer) |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ | `GetHeroCreature` WRONG |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (non-const) | local CInterfaceMap WRONG |
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | same, const (QBE) | local |
| 0x006D5850 | 00/6d/CTCFollowed_GetFollowers_006d5850.cpp (landed as GetFollowers(CRetailVector<CThing*>*) -- pointer, local model) | ?GetFollowers@CTCFollowed@@QAEXAAV?$vector@PAVCThing@@...@Z (Ego_r 0x0070A720): std::vector<CThing*>& | ref OK; landed row spelling wrong (1012c NAME_FIXES) |
| 0x00A01B10/50/90, 0x00A01C10 | 00/a0/CBaseIntelligentPointer_*.cpp | ctor / GetPItem() const -> CBaseObject* / SetPItem(CBaseObject*) / virtual ~ | GetPItem returning CThing* (minor) |
| 0x00436C80 | 00/43/global_PushBackdword00436c80_00436c80.cpp (generic folded body "CRetailVector_dword::PushBack_dword_00436c80") | vector<CThing*>::push_back (COMDAT, 13 identical bodies) | PushBack local |
| 0x00436AC0 | 00/43/stdvector_Minsertoverflow_00436ac0.cpp | std::vector<CThing*>::_M_insert_overflow(ptr,const&,const __true_type&,size_type,bool) | `InsertGrow` stand-in + fake GrowVectorStorage_s7 body WRONG |
| 0x008AC540 | 00/8a/stdvector_vector_008ac540.cpp | vector<CIntelligentPointer<CThingAICreature> >::vector(const vector&) | `CIntelligentPointerVector` of CThing WRONG elem type |
| 0x00742210 | 00/74/vectorCConversationstd_allocatorCConversation_00742210.cpp (mislabelled CConversation, folded body) | ~vector<CIntelligentPointer<CThingAICreature> > (COMDAT-folded) | local |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | `GetPosition` WRONG |
| 0x006CB6E0 | 00/6c/CTCCreatureNavigation_GetNearbyWanderPosition_006cb6e0.cpp (landed leaf QAE ... MPAV2 -- WRONG) | ?GetNearbyWanderPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@_N@Z (Ego_r 0x00701120 + shared header line 49): const, C3DVector& out | draft matches PDB (const, &) |
| vslot 31 (+0x7C) on PhysicsTC | 00/6b/CTCPhysicsBase_ResetPosition_006b01d0.cpp (rtti CTCPhysicsBase 0x0125D2C4 slot 31 = 0x006B01D0) | ?ResetPosition@CTCPhysicsBase@@UAEXABVC3DVector@@@Z | `SetPosition` WRONG (SetPosition is slot 30) |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free (import thunk) | ok |
Constants: 4.0f @0x0123088C, CIntelligentPointer<CThingAICreature> vptr 0x0123E7E0, 0.8f immediate.

## 4. Members / headers
- CGameScriptInterface::PlayerManager +0x14 (CGameScriptInterface.h, FABLE_RETAIL_NSCRIPT).
- CThing (engine/CThing.h): UsingComponents +0x20 (bitset; HasTC), Components +0x44 (CVectorMap), PhysicsTC +0x60 (GetPhysicsTC()),
  Dead +0x91 bit0 (IsDead()). Opt-ins: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC/PeekTC(id,&p) bodies as retail
  expands: LowerBound out of line, end-or-greater cond), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE.
- CTCSummonSpell (TC 145=0x91, engine/CTCSummonSpell.h): Minion CIntelligentPointer<CThingAICreature> +0x11 (pack 1, PDB +17),
  GetSummonedCreature() const inline {return Minion;} under FABLE_RETAIL_CTCSUMMONSPELL + _MINION (+ FABLE_RETAIL_CINTELLIGENTPOINTER_COPY).
- CTCGhostSword (TC 154=0x9A, engine/CTCGhostSword.h): Minions vector<CIntelligentPointer<CThingAICreature> > +0x10,
  GetSummonedCreatures() const (by value) under FABLE_RETAIL_CTCGHOSTSWORD; copy ctor out of line needs FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE.
- CTCFollowed (TC 188=0xBC): shared engine/CTCFollowed.h has no opt-ins (plain decl GetFollowers(std::vector<CThing*>&) line 26);
  1012c overlay hdr/engine/CTCFollowed.h adds FABLE_RETAIL_CTCFOLLOWED (StatGetInterfaceType). PDB layout Followers list +0x0C.
- CTCCreatureNavigation (TC 7 = TCI_NAVIGATION): engine/CTCCreatureNavigation.h, FABLE_RETAIL_CTCCREATURENAVIGATION.
- CTCPhysicsBase: engine/CTCPhysicsBase.h, FABLE_RETAIL_CTCPHYSICSBASE (ResetPosition virtual line 44).
- CIntelligentPointer: engine/CIntelligentPointer.h; relevant opt-ins FABLE_RETAIL_CINTELLIGENTPOINTER, _COPY, _FORCEINLINE_CTOR,
  _COPY_OUTOFLINE, _CTOR_OUTOFLINE.
- Locally modelled in draft_original though shared headers exist: CThing, C3DVector, C2DVector, CTCPhysicsBase, CBaseIntelligentPointer
  / CIntelligentPointer, CTCFollowed, CTCCreatureNavigation, CPlayer, CPlayerManager, CGameScriptInterface, the CVectorMap, std::vector
  (both vectors), and the fake "CTCLeader91"/"CTCFollowers9A" (really CTCSummonSpell / CTCGhostSword).

## 5. Callee-body visibility
- push_back (vector<CThing*>): stlport/fable_containers.h line 479-497: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (visible body,
  __declspec(noinline)) / _OUTOFLINE (decl only). Retail needs BOTH behaviours in one function (first site called 0x00436C80,
  loop site expanded) -> neither macro alone fits; default inline + budget is the only route; check what decides the first site
  (in hand288 a separate out-of-line PushBack produced it). 0x00436C80 is a COMDAT (folded, 13 bodies).
- vector copy ctor 0x008AC540: FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE (visible body kept out of line; documented for 0x008A8260).
- CIntelligentPointer copy: inline in retail (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY; FORCEINLINE_CTOR exists for budget cases).
- GetSummonedCreature / GetSummonedCreatures / StatGetInterfaceType: inline (FableWin-only names, no out-of-line copy).
- _M_insert_overflow 0x00436AC0, LowerBound 0x0040F020/0x004365B0, GetPos 0x004C73D0, GetPControlledCreature 0x00487DC0 (8 bytes,
  still called): all out of line, other TUs.
- Same-TU neighbours: CGameScriptInterface methods 0x008A78E0..0x008A9xxx are one TU (CGameScriptInterface.cpp); 0x008A8260
  GetHeroSummonedCreaturesList (unlanded, drafts in pending/2026-11-10_linux_hand1012{,b}/differ/008a8260.cpp, nd=7) uses the same
  GetSummonedCreature + GetSummonedCreatures idioms -- copy its spelling.

## 6. Sibling landed rows (same TU, 0x008A7xxx-0x008A9xxx)
- 00/8a/CGameScriptInterface_GetThingWithScriptName_008a7d60.cpp, _GetAllCreaturesExcludingHero_008a89d0.cpp, _CreateCreature_008a9100.cpp:
  `#define FABLE_RETAIL_NSCRIPT` + `#pragma auto_inline(off/on)` around same-TU stand-ins; no `// Flags:` line (default flags).
- 00/8a/CGameScriptInterface_EntityStopFollowing_008a7980.cpp (same hero idiom GetMainPlayer 0x00449970 + 0x00487DC0).
- 00/8a/CGameScriptInterface_GetFollowingEntityList_008a80f0.cpp (GetFollowers 0x006D5850 + vector<CThing*> local + inline
  `if(start) free(start)` dtor).
- None of these sibling rows use `// Flags:`; this row needs none either (no /Oy-, no SEH, no optimize("s") idioms seen).

## 7. Model errors in draft_original (hand288) vs disassembly
- Wrong callee names: GetHeroCreature, GetPosition, SetPosition (slot 31 is ResetPosition), InsertGrow, GetMainPlayer as pointer.
- Fake TC classes CTCLeader91/CTCFollowers9A -> CTCSummonSpell::Minion (+0x11) / CTCGhostSword::Minions (+0x10), element type
  CIntelligentPointer<CThingAICreature> not CThing.
- InsertGrow given a fake body (GrowVectorStorage_s7) in-TU: not a retail-visible body.
- Minion loop form: retail hoists n = size() once (signed) and counts down; 1012c's `i < (long)minions.size()` re-reads size?
  (check: retail does NOT re-read). Use `long n = minions.size(); for(long i=0;i<n;++i)` or pointer+count form as hand288 did.
- Final loop must re-read followers.size() each iteration (unsigned, jb) -- both drafts OK.
- Hero register: retail keeps hero as a local spilled to the frame (ebp reused as final-loop index); both drafts got ebx + edi zero
  register. Immediate zero stores for the vector -> vector constructed late (after GetTC lookup), maybe the hero should be const
  CThingPlayerCreature* / CThing* named local; not solved by hand288 attempts (aliased hero local, virtual-dtor model).

## 0x00724630

# DOSSIER 0x00724630 CTCPhysicsStandard::SetPhysicsMeshBankIndex (741 bytes)

## 1. Retail shape (xdis 00724630 0x2f5)
- thiscall, `ret 4` (one 4-byte arg `long`), frame `sub esp,0x48; push ebx/esi/edi`; NO frame pointer (no /Oy-), NO SEH prologue
  (counted-pointer temporaries die inside their full expression; nothing can throw while live, so /EHsc emits no frame).
- ebx = 0 for whole function (`xor ebx,ebx`), used for null stores / compares; esi = this; edi = index then meshInfo then mapwho.
- index==0 branch: delete PPhysicsMeshInfo (0xBFE9BC), store 0, Radius(+0x34)=CalculateRadius(), OffsetVector3D(+0x44/48/4c) = 0
  as int ebx stores, then `and byte [esi+0x85],0xef` (NavigationLayers=false, RMW, AFTER the zero stores), `jmp` to mapwho tail.
- else: new 0x1c if null; BankIndex = (word)index; RenderSizeX = ParentThing(+4)->PTCGraphicAppearance(+0x64)->GetScale() (0x4BBFF0 call);
  PeekWorld() (0x686A80) ->[+0x3c] mesh bank ->PeekPhysicsMeshInfo(index) (0xA24250); BeginLoading (0xA25BD0);
  GetMesh (0xA262B0, hidden result in index's arg slot [esp+0x58]) x4, each temp released inline
  (`cmp eax,ebx; je; mov edx,[eax+4]; mov ecx,eax; add eax,4; dec edx; cmp edx,ebx; mov [eax],edx; jne; call [vtbl+4]`).
- Box centre: `fld [eax+0x90]` (Max.X) `add eax,0x84; fadd [eax]` (Min.X); Y,Z: Max first too -> (Max + Min) * 0.5f (0.5f @0x0122F59C).
- Matrix: `lea edx,[esi+0x50](RHSet); lea ecx,[esp+0x24](result)` call 0xA358F0 = static __fastcall returning CMatrix3x4 by value.
- Rotation (no translation): per row z*E3k, y*E2k faddp, x*E1k faddp; x kept on x87 for row Y (`fld st(2)`).
- ToCentre(+8 of info) = centre * (RenderSizeX * 0.01f) (0.01f @0x01230A5C); BoundingRadius(+0x14) = `fld info.RenderSizeX; fmul mesh[+0x80]; fmul 0.01`
  -> RenderSizeX * mesh->BoundingSphere.Radius * 0.01f; BoundingCircleRadius(+0x18) = `fld mesh[+0xa4]; fmul info.RenderSizeX; fmul 0.01`.
- OffsetVector3D = PPhysicsMeshInfo->ToCentre (reload [esi+0x40], copy 3 dwords via pointer regs); Radius = CalculateRadius();
- NavigationLayers bit4 of +0x85 = (mesh->NavigationContainmentVolumes first(+0x54) != last(+0x58)): setne/shl 4/xor/and 0x10/xor (bool:1 assign).
- Tail (both paths): ParentThing->UsingComponents test `[eax+0x24] & 0x4000000` (type 0x3a=58), key 0x3a stored in arg slot AFTER test,
  map=&Components (+0x44), LowerBound 0x40F020, `it==end || 0x3a < it->first ? end : it`, ->second; if not present, mapwho = stale slot.
  `test byte [mapwho+0x50],1` (IsInMap) -> RemoveFromMapwho 0x71A730, GetMapwhoPosition 0x6AFCB0 (hidden result [esp+0x18]), PlaceInMapwho 0x71A710.

## 2. Identity
- rtti-vtables.tsv: CTCPhysicsStandard vtable 0x01265F84 slot 85 = 0x00724630; same slot 85 = CTCPhysicsBase::SetPhysicsMeshBankIndex 0x006B1340
  (landed) and CTCPhysicsRigidBody::SetPhysicsMeshBankIndex 0x0075A530 (landed). xref: only the three vtable refs.
- PDB: Ego_r/FableWin `?SetPhysicsMeshBankIndex@CTCPhysicsStandard@@UAEXJ@Z` (egor_pdb_names 0x0073B6C0; FableWin api 0x017CF7E5/0x02525DB0)
  = public virtual void SetPhysicsMeshBankIndex(long). Manifest label CORRECT. Caller: landed SetScaleFromOwner 0x007236A0 calls it.
- Header: CTCPhysicsBase.h:160 declares `virtual void SetPhysicsMeshBankIndex(long);` but CTCPhysicsStandard.h (FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES,
  slots 84 SetAsPushableByHero / 86 SetScaleFromOwner) does NOT declare the slot-85 override -> author must add it (public) between them or locally.

## 3. Callees (landed file / landed name / PDB name)
- 0x00BFE9BC operator delete (global_operatordelete_00bfe9bc.cpp); 0x00BFEA1A operator new (global_operatornew_00bfea1a.cpp).
- 0x006AFE20 CTCPhysicsBase_CalculateRadius_006afe20.cpp; PDB ?CalculateRadius@CTCPhysicsBase@@IBEMXZ (protected const). Not in shared header.
- 0x004BBFF0 landed as CMap_DrawGetSeaLevel_004bbff0.cpp (WRONG landed name); PDB per RigidBody row NAME_FIXES: NGraphicAppearance::CTCGraphicAppearance::GetScale()const
  (?GetScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ). Draft spelled CTCGraphicAppearance::GetScale (no namespace, non-const) -> wrong.
  NOTE NGraphicAppearance_CTCGraphicAppearance.h:107 gives an INLINE GetScale; here it must be an out-of-line call -> declare non-inline.
- 0x00686A80 CTCBase_PeekWorld_00686a80.cpp; PDB ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (CTCBase.h:150). Draft says GetWorld -> WRONG (GetWorld is 0x00686A70).
- 0x00A24250 CMeshDataBank_PeekPhysicsMeshInfo_00a24250.cpp; PDB ?PeekPhysicsMeshInfo@CMeshDataBank@@QBEPBVCPhysicsMeshInfo@@K@Z (unsigned long, const, returns const*).
  Draft: CMeshBank::GetPhysicsMeshInfo(long) -> WRONG. Bank field CWorld +0x3c = PMainMeshBank (CCountedPointer<CMeshDataBank>::Data); CWorld.h FABLE_RETAIL_CWORLD_MESH_BANK/PEEKMESHBANK.
- 0x00A25BD0 CPhysicsMeshInfo_BeginLoading_00a25bd0.cpp; PDB ?BeginLoading@CPhysicsMeshInfo@@QBEXXZ (CONST; draft non-const).
- 0x00A262B0 CPhysicsMeshInfo_GetMesh_00a262b0.cpp; PDB ?GetMesh@CPhysicsMeshInfo@@QBE?AV?$CIVCountedPointer@$$CBVCPhysicsMesh@@@@XZ
  -> CIVCountedPointer<const CPhysicsMesh> (draft: non-const CPhysicsMesh).
- 0x00A358F0 C3DMeshObject_GetPositionMatrixFromRHSet_00a358f0.cpp; PDB ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z
  (static __fastcall, returns CMatrix3x4). Draft: free `GetMatrixFromRHSet` + local C3DMatrix -> WRONG name. Not declared in C3DMeshObject.h; copy the landed row's decl.
- 0x0040F020 CVectorMap_LowerBound_0040f020.cpp: CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (via CThing GetTC).
- 0x0071A730 landed as CTCMapwho_OnDie_0071a730.cpp (free fastcall wrapper; landed name WRONG): CTCMapwho.h:46 + FableWin ?RemoveFromMapwho@CTCMapwho@@QAEXXZ.
  OnDie is slot 13 0x0071A7C0. Draft spelled OnDie -> WRONG; use RemoveFromMapwho.
- 0x0071A710 CTCMapwho_PlaceInMapwho_0071a710.cpp; PDB ?PlaceInMapwho@CTCMapwho@@QAEXABVC3DVector@@@Z (CTCMapwho.h:45).
- 0x006AFCB0 CTCPhysicsBase_GetMapwhoPosition_006afcb0.cpp; PDB ?GetMapwhoPosition@CTCPhysicsBase@@QBE?AVC3DVector@@XZ. Do NOT use the inline body of
  FABLE_RETAIL_CTCPHYSICSBASE_MAPWHOPOSITION (retail calls it out of line).
- vtable slot 1 (+4) of the CPhysicsMesh pointee = CIVCountedPointeeBase::Delete() const (CIVCountedPointeeBase.h:29); refcount IVRefCount at +4.

## 4. Members
- this (engine/CTCPhysicsStandard.h with FABLE_RETAIL_CTCBASE, _CTCPHYSICSBASE, _CTCPHYSICSSTANDARD, _OVERRIDES, _FLAGS):
  +0x04 CTCBase::ParentThing (CThing*), +0x34 CTCPhysicsBase::Radius (float, protected), +0x40 PPhysicsMeshInfo (CThingPhysicsMeshInfo*),
  +0x44 OffsetVector3D (C3DVector), +0x50 RHSet (CRightHandedSet), +0x85 bit4 `bool NavigationLayers:1` (FLAGS opt-in; Ego_r bool:1).
- CThingPhysicsMeshInfo (no shared header; Ego_r 0x1c): BankIndex ushort +0, RenderSizeX float +4, ToCentre C3DVector +8, BoundingRadius +0x14, BoundingCircleRadius +0x18.
  SetScaleFromOwner row declares it locally (BankIndex only) -> local full class acceptable.
- CThing (engine/CThing.h): UsingComponents bitset<274> +0x20, Components CVectorMap +0x44, PTCGraphicAppearance +0x64 (GetTCGraphicAppearance() inline accessor at CThing.h:432).
  GetTC expanded form: FABLE_RETAIL_CTHING_GETTC_EXPANDED (`GetTC<CTCMapwho>(TC_MAPWHO/0x3a,&mapwho)`), matches retail key-store-after-test + pointer-local map.
- CTCMapwho (engine/CTCMapwho.h, FABLE_RETAIL_CTCMAPWHO): InMapwho uchar:1 +0x50, inline IsInMap().
- CPhysicsMesh (engine/CPhysicsMesh.h, retail offsets): NavigationContainmentVolumes +0x54 (FABLE_RETAIL_CPHYSICSMESH_NAVIGATION gives the CVectorMap),
  BoundingSphere C3DBoundingSphere +0x74 (Radius +0x80, GetBoundingSphere()), BoundingBox +0x84 (opaque bytes in header!), BoundingCircle +0x9c (CCircle, radius +0xa4; opaque).
- C3DBoundingBox.h: default GetCenter() = (MaxCoord + MinCoord) * 0.5f -- MATCHES retail Max-first; do NOT define _CENTER_MIN_FIRST. PDB spelling GetCenter (draft: GetCentre).
- CMatrix3x4.h: FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR TransformVector(C3DVector&) (xx/yy/zz copies, rotation only; FableWin ?TransformVector@CMatrix3x4@@QBEXAAVC3DVector@@@Z).
- CIVCountedPointer.h: FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT (one-level dtor, inline Release+Delete, no null store) = retail release pattern.
- Draft models LOCALLY classes with shared headers: C3DVector, C3DBoundingBox, CRightHandedSet, C3DMatrix(=CMatrix3x4), CPhysicsMesh, CIVCountedPointer,
  CPhysicsMeshInfo(only C3DMeshInfo.h exists), CMeshDataBank, CWorld, CTCGraphicAppearance, CVectorMap, CTCMapwho, CThing, CTCPhysicsStandard.

## 5. Visibility
- Out-of-line callees (must stay calls): all in sec.3. Same-TU neighbours defined earlier (CTCPhysicsStandard run 0x007234C0..0x00724D50):
  SetScaleFromOwner 0x007236A0 (caller), OnCreate 0x00724010, OnSerialise 0x00724290; GetPhysicsMeshInfo 0x00724D20 / HasNavigationLayers 0x00724BC0
  are out-of-line virtual COMDATs later -- retail does NOT call them here (reads members directly).
- Inline-expanded here: GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED), IsInMap, CIVCountedPointer dtor, GetCenter, TransformVector, bitfield set, C3DVector ops.

## 6. Siblings
- CTCPhysicsRigidBody_SetPhysicsMeshBankIndex_0075a530.cpp: SAME algorithm (no BeginLoading, no index==0 path). `// Flags: /EHsc`; its note:
  "at plain /O2 the BoundingRadius product loaded the mesh radius first" -> /EHsc fixed operand order. RB box centre is Min-first on X only (differs here).
- CTCPhysicsStandard_SetScaleFromOwner_007236a0.cpp / _EnableGravity_007235e0.cpp / _GetOffsetVector_00724d30.cpp / _CanCollideWithLandscape_00724cb0.cpp:
  #define FABLE_RETAIL_CTCBASE, _CTCPHYSICSBASE, _CTCPHYSICSSTANDARD, _CTCPHYSICSSTANDARD_OVERRIDES, _CTCPHYSICSSTANDARD_FLAGS; include ETCInterfaceType.h then CTCPhysicsStandard.h.
  No pragmas; only SetRHSetAsFlushOnGround has `// Flags: none`.

## 7. Old draft model errors
- Built with no /EHsc: hand304 residue (x87 operand order, bitfield RMW scheduling) is what RB sibling fixed with `// Flags: /EHsc`. Try /EHsc first.
- Box centre written (Min+Max) -> retail is (Max+Min) (shared header default GetCenter).
- BoundingRadius written mesh*RenderSizeX -> retail RenderSizeX*mesh->BoundingSphere.Radius; BoundingCircleRadius written RenderSizeX*mesh -> retail mesh radius loaded first.
- Wrong names: GetWorld->PeekWorld, CMeshBank::GetPhysicsMeshInfo->CMeshDataBank::PeekPhysicsMeshInfo, GetMatrixFromRHSet->C3DMeshObject::GetPositionMatrixFromRHSet,
  OnDie->RemoveFromMapwho, GetScale namespace/const, GetCentre->GetCenter; constness (BeginLoading/GetMesh const, CIVCountedPointer<const CPhysicsMesh>).
- Everything modelled locally instead of shared headers (strict verifier rejects local CTCPhysicsStandard).

## Open conflict: 0x004BBFF0
- Body: `fld [ecx+0x4c]; fmul [ecx+0x34]; ret` (thiscall, returns float product). NGraphicAppearance_CTCGraphicAppearance.h:178 says the inline
  GetScale is `fld [eax+34h]` only, so 0x4BBFF0 (a product of +0x4c and Scale +0x34) may NOT be GetScale; the RigidBody row (landed, byte-exact)
  spells it NGraphicAppearance::CTCGraphicAppearance::GetScale() const -- follow the landed sibling for consistency (one address, one name),
  but flag for the coordinator. The landed file name CMap_DrawGetSeaLevel is certainly wrong (thiscall on the appearance pointer).

## 0x009CE5E0

# DOSSIER 0x009CE5E0 CBankFile::AddEntryMultipleFiles (741 bytes)

## 1. Retail shape (xdis 009ce5e0 0x2f5)
- thiscall (this=ecx->esi), `ret 0x14` = 5 dwords: symbol& [esp+0x2c], type [0x30], filenames& [0x34], CCountedPointer<CBankStateBlock> BY VALUE (Data [0x38], Info [0x3c]) (offsets after the 4 pushes).
- NO frame pointer (`sub esp,0x18` then push ebx/ebp/esi/edi), NO SEH prologue although destructible locals exist -> TU built WITHOUT /EHsc (same as landed 0x009D47E0 note: "the TU builds without /EHsc"). Throw is a bare _CxxThrowException, temp never destroyed.
- Frame 0x18: [esp+0x10] = conditional-temporary flag word (zeroed at function entry, before the find call: `mov [esp+0x14],0` pre-push); arm1 filename local [esp+0x14]; arm2 filename local [esp+0x18]; arm2 zero value temps for resize [esp+0x14]; arm2 CRuntimeData blank [esp+0x1c..0x27].
- Dead-arg-slot homing: "Duplicate symbol: "+symbol result -> filenames slot [0x34]; the CBankCreatorException object -> stateBlock slot [0x38..0x3f]; the CCharString("") ternary temporary -> filenames slot [0x34] (both arms).
- Register roles: esi=this, ebp=index (both arms; pushed as InitEntry arg1), edi=update record ptr, ebx = symbol (only for the find), then &RuntimeData[index], then filenames (reloaded `mov ebx,[esp+0x34]`); arm2 keeps the temp-flag in bl (bit 2), arm1 reads it from memory (bit 1). So the old note "symbol in ebx" is only true for the first 6 instructions.
- Control flow: find-hit arm 0x9ce60b..0x9ce706 ends `lea ecx,[esp+0x14] ; jmp 0x9ce89b` = cross-jump into arm2's `call ~CCharString` (0x9ce89b) + inline ~CCountedPointer tail. So retail merges ONLY the filename dtor call + param dtor; the two InitEntry calls (0x9ce6fd, 0x9ce892) stay separate (two source statements, two separate filename locals, flag bits 1 and 2).
- Loop (find-miss): `ebp=1; if (1>=Size) goto grow; edi=0xc` strength-reduced index*12; body = inlined EntryExists: `test edi,edi` (index!=0), `cmp ebp,[esi+4]` jae->grow, PackedValid.Size [esi+0x6c] test -> call 0xA629F0 on [esi+0x68] with index-1 / `setne al`, else RuntimeData.m_pFirst[edi+9] (Valid). After loop `cmp ebp,[esi+4]; jb skipgrow`.
- Grow: blank CRuntimeData zeroed (dword,dword,byte,byte) -> RuntimeData.resize(index+1, blank) 0x9D4010; UpdateData.resize(index+1, (ptr)0) 0x9D3DF0; Symbols.resize(index+1) 0x49B760 (ONE-arg, out of line); Checksums.resize(index+1, 0UL) 0x464931; Size = (RuntimeData.last-first)/12 (signed: imul 0x2aaaaaab, sar 1, shr 31 add).
- After: ebx=&RuntimeData[index] computed BEFORE `operator new(0x18)`; inline ctor of the record (0,0,0,0, byte+0x10=0, +0x14=0, +0x15=1) with null test; UpdateData[index]=rec; Symbols[index]=symbol (operator= 0x99EFB0); Valid(+9)=1 then DataType(+8)=type; SetFilenames; [rec]=stateBlock->GetCRC() (vslot +8); Exists(+0x15)=1 then RequiresUpdate(+0x14)=1.
- Hit arm: index=node->_M_value_field.second (node+0x14); rec=UpdateData[index]; if rec->Exists throw CBankCreatorException("Duplicate symbol: "+symbol) [inline 1-arg ctor: EntrySymbol default 0x99E4B0, ErrorText copy 0x99EC30; ThrowInfo 0x013693B4]; rec->Exists=1; data=&RuntimeData[index]; `if (data.DataType(movzx) != type || !rec->Equals(filenames, stateBlock))` { DataType=(uchar)type; SetFilenames; CRC; RequiresUpdate=1 }.
- Ternary per arm: `CCharString filename(filenames.empty() ? CCharString("") : filenames[0])` -> `if first==last { CCharString("",-1) 0x99EBF0 into [0x34]; flag|=bit }` copy-ctor 0x99EC30 into the local; `if flag&bit ~temp`.
- End: inline ~CCountedPointer of the param: `if(Info && --RefCount==0){ DeleteFunc(Info->Data) [ecx=+8, call [+4]]; delete Info }` (no zero stores visible: dead).

## 2. Identity (two sources, PROVEN)
- Mangled leaf: `?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z`
  = `virtual void CBankFile::AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock)` (public virtual, by-value counted pointer, non-const).
- Sources: ghidra_out/egor_pdb_names.tsv:31574 (Ego_r 0x00B1C000); _fablewin_api.tsv:15659 + naming_stragglers/FableWin.exe.signatures.tsv:15666 (FableWin 0x017A3523); vtable: xref shows 0x0129B564 = CBankFile vtable 0x0129B54C + 0x18 = slot 6 (rtti-vtables.tsv lists only slots 0..5 for CBankFile, and slot 6 under CWADBank 0x012442DC); landed caller CBankFile::AddEntry 0x009CD620 calls `[edx+18h]` and names this PDB symbol. Manifest label is CORRECT.
- Engine header (FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS branch) declares it as `virtual void AddEntryMultipleFiles(const CCharString&,unsigned long,const CArray<CCharString>&,const CCountedPointer<CBankStateBlock>)` -- the trailing `const` on a by-value param does not change the mangling.
- Callers: only vtable refs (12 vtables: CBankFile + subclasses) and AddEntry 0x009CD620 via slot 6. CShaderDataBank has its own override in PDB.

## 3. Callees (landed file / landed name / PDB)
| addr | landed file | landed name | PDB / note |
|---|---|---|---|
| 0x009D2DD0 | 00/9d/stdRbtree_Mfind_009d2dd0.cpp | _Rb_tree<CCharString,pair<const CCharString,ulong>...>::_M_find<CCharString> const, node* in eax | STLport _M_find; source is `OldSymbolIndices.find(symbol) != OldSymbolIndices.end()` (end = [map+0] header). Draft calls it "SymbolIndices.find" -- WRONG MEMBER NAME (see 4) |
| 0x0099F690 | 00/99/global_operator_0099f690.cpp | operator+(const char*, const CCharString&) __fastcall | ??H@YI?AVCCharString... |
| 0x0099E4B0 | 00/99/CCharString_CCharString_0099e4b0.cpp | CCharString::CCharString() | ok |
| 0x0099EC30 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp | CCharString(const CCharString&) | ok |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) (fable_string.h) | ok |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| 0x0099EFB0 | 00/99/CCharString_operator_0099efb0.cpp | ??4CCharString@@QAEABV0@ABV0@@Z returns const CCharString& | draft returns non-const `CCharString&` (no byte effect; fix spelling) |
| 0x00BFEB84 | 00/bf/global_CxxThrowException_00bfeb84.cpp | _CxxThrowException | ThrowInfo 0x013693B4 = CBankCreatorException |
| (inline) | -- | CBankCreatorException(const CCharString&) | PDB ??0CBankCreatorException@@QAE@ABVCCharString@@@Z (FableWin 0x01817946); EntrySymbol default, ErrorText copy. NO shared header for CBankCreatorException; landed siblings 0x009CB8F0 (2-arg ctor) and 0x009D47E0 (copy ctor) model it locally (EntrySymbol +0, ErrorText +4). |
| 0x009CC750 | 00/9c/CBankFileEntryUpdateData_Equals_009cc750.cpp | Equals(const CArray<CCharString>&, CCountedPointer<CBankStateBlock>) const | PDB is PRIVATE `ABE_N...` (landed made it public) |
| 0x009CC840 | 00/9c/CBankFileEntryUpdateData_SetFilenames_009cc840.cpp | SetFilenames(const CArray<CCharString>&) __thiscall ret 4 | PDB PRIVATE `AAEX` |
| vslot +8 | -- | CBankStateBlock slot 2 = GetCRC (per landed Equals 0x9CC750) | |
| 0x009D4010 | 00/9d/stdvectorCBankFileCRuntimeData_resize_009d4010.cpp | vector<CBankFile::CRuntimeData>::resize(unsigned, const CRuntimeData&) | |
| 0x009D3DF0 | 00/9d/global_ResizeCRetailVector009d3df0_009d3df0.cpp | descriptive `CRetailVector_009d3df0::Resize_...` (4-byte resize(n,const T&)) | real: vector<CBankFileEntryUpdateData*>::resize(n, const T&) |
| 0x0049B760 | 00/49/STLvectorCCharString_resize_0049b760.cpp | ?resize@CCharStringVector@@QAEXI@Z (one-arg resize(n)) | vector<CCharString>::resize(unsigned) out of line |
| 0x00464931 | 00/46/CArray_resize_00464931.cpp (#pragma optimize("s",on)) | CArray::resize (4-byte, resize(n,const T&)) | unaligned address -> COMDAT; Checksums is CArray<unsigned long> |
| 0x00BFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new | size 0x18 |
| (inline) | 0x009CC6A0 (not landed; xdis: zero +0..+0xc, byte +0x10, +0x14=0, +0x15=1) | CBankFileEntryUpdateData::CBankFileEntryUpdateData() | PDB ??0CBankFileEntryUpdateData@@AAE@XZ (PRIVATE). Retail emitted it out of line too yet INLINED here -> same-TU visible body |
| 0x00A629F0 | 00/a6/CPackedUIntArray_operator_00a629f0.cpp | CPackedUIntArray::operator[](ulong) const | via inline EntryExists |
| (inline) | 00/9c/CBankFile_EntryExists_009cc190.cpp | CBankFile::EntryExists(ulong) const | PDB ?EntryExists@CBankFile@@QBE_NK@Z; out of line at 0x9CC190 but INLINED here (same TU, defined earlier) |
| 0x009CE050 | 00/9c/CBankFile_InitEntry_009ce050.cpp | CBankFile::InitEntry(ulong, const CCharString&, const CCharString&) | PDB PRIVATE ?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z; draft has it public (no byte effect) |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | count block |

## 4. Members
- this = CBankFile, shared header rebuild/include/engine/CBankFile.h (retail layout, sizeof 0x110, pack(1)). Retail offsets used: Size +0x04 (ulong), Symbols +0x08 (CArray<CCharString>), Checksums +0x14 (CArray<unsigned long>), RuntimeData +0x20 (CArray<CBankFile::CRuntimeData>), UpdateData +0x2c (CArray<CBankFileEntryUpdateData*>), PackedValid +0x68 (CPackedUIntArray; +0x6c = PackedValid.Size), OldSymbolIndices +0xb8 (map<CCharString,unsigned long>).
- **+0xb8 is OldSymbolIndices, NOT SymbolIndices** (header static asserts SymbolIndices==0xac, OldSymbolIndices==0xb8; struct_layouts_egor.tsv order SymbolIndices then OldSymbolIndices; landed InitEntry 0x9CE050 says SymbolIndices +0xac). Draft misnames it.
- Header opt-ins: FABLE_RETAIL_CBANKFILE_FILENAMEINDICES types Symbols (CArray<CCharString>), RuntimeData (CArray<CRuntimeData>, nested CRuntimeData DataOffset/DataSize/DataType/Valid), PackedDataType, FilenameIndices and pulls in CArray.h, CPackedUIntArray.h, fable_string.h, stlport/fable_map.h. Checksums, UpdateData, PackedValid, OldSymbolIndices are still OPAQUE `unsigned char[]` -> author needs a new opt-in (e.g. FABLE_RETAIL_CBANKFILE_ADDENTRY) typing those four + declaring AddEntryMultipleFiles/InitEntry and an inline EntryExists body (FABLE_RETAIL_CBANKFILE_ENTRYQUERY only DECLARES EntryExists out of line -- wrong for this row). The FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS branch gives the full virtual table (slot 6 = AddEntryMultipleFiles) but swaps __vftable for a CBaseClassNonCopyable base.
- CRuntimeData has NO user ctor in PDB (only DeallocateMemory / resize / _Uninit_def_fill_n): the blank is `_Tp()` value-init from STLport inline `resize(n){resize(n,_Tp());}` -> source is likely `RuntimeData.resize(index+1)`, `UpdateData.resize(index+1)`, `Symbols.resize(index+1)`, `Checksums.resize(index+1)`; retail calls the 2-arg resize for RuntimeData/UpdateData/Checksums out of line but the 1-arg resize for vector<CCharString>.
- CPackedUIntArray (engine/CPackedUIntArray.h, PDB): PackedInts +0, Size +4, Bits +8 (uchar), Bias +0xc; inline GetSize(). Draft names m_bitsPerValue/m_pData/IsPacked are WRONG -- the test at +0x6c is `PackedValid.GetSize()` (header's GetEntryDataType uses exactly `if (PackedDataType.GetSize()) return PackedDataType[index-1];`).
- CBankFileEntryUpdateData: shared engine/CBankFileEntryUpdateData.h is the PDB 0x20 layout WITH StateBlock +0x18; retail `new` is 0x18 bytes (record has no StateBlock: landed dtor 0x9CC6C0 says so). Members: StateBlockCRC +0, InfoSize +4, InfoBuffer +8, Filenames +0xc (CSmallVector<CCharString,8>: Start +0, Size signed char +4), RequiresUpdate +0x14, Exists +0x15. Header has no FABLE_RETAIL opt-in and keeps members private (friend CBankFile) -- needs a retail-size opt-in (drop StateBlock) + ctor/Equals/SetFilenames decls before it can be used.
- CCountedPointer: engine/CCountedPointer.h (Data +0, Info +4; CCPPointerInfo RefCount +0, DeleteFunc +4, Data +8). Default (#else) dtor body matches retail; copy ctor matches (`if(Info)++RefCount`).
- CArray: engine/CArray.h (public STLport vector, 12 bytes).
- Map: stlport/fable_map.h; FABLE_RETAIL_STLP_TREE_MFIND_DECL makes _M_find declaration-only (call 0x9D2DD0, node in eax) -- matches retail.
- Locally modelled in the draft though a shared header exists: CCharString (fable_string.h), CCountedPointer + CCPPointerInfo (engine/CCountedPointer.h), CArray (engine/CArray.h), CPackedUIntArray (engine/CPackedUIntArray.h), CBankFileEntryUpdateData (engine/CBankFileEntryUpdateData.h, size mismatch), CBankFile (engine/CBankFile.h), map (stlport/fable_map.h). CBankStateBlock: header list has no CBankStateBlock.h (only subclasses' StateBlock headers) -- local stub OK. CBankCreatorException: no header.

## 5. Callee-body visibility
- Inlined (visible, same TU = Bank file .cpp, defined earlier): EntryExists 0x9CC190, CBankFileEntryUpdateData ctor 0x9CC6A0, CBankCreatorException(const CCharString&), CCountedPointer copy ctor/dtor, CArray/vector empty()/operator[]/size().
- Called out of line though STLport-inline: _M_find 0x9D2DD0 (FABLE_RETAIL_STLP_TREE_MFIND_DECL), vector::resize(n,x) for CRuntimeData/ptr/ulong (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE = __declspec(noinline) visible body), vector<CCharString>::resize(n) 0x49B760 (FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL declares resize(n) for ALL types -- would break the 4-byte ones, so the CCharString one needs per-type handling or a model declaration).
- Same-TU neighbours (CBankFile .cpp, ascending): EntryExists 0x9CC190, Equals 0x9CC750, SetFilenames 0x9CC840, AddEntry 0x9CD620, InitEntry 0x9CE050, Close 0x9CE360, WriteFileInformation 0x9CE370 (parked), THIS 0x9CE5E0, BeginUpdateEntries 0x9CE8D0. Equals/SetFilenames/InitEntry are called, not inlined (VC7.1 heuristic) -- keep them declared only.

## 6. Siblings to copy idioms from
- 00/9c/CBankFile_FindIndicesByFilename_009ccef0.cpp: `#define FABLE_RETAIL_CCHARSTRING`, `..._CCHARSTRING_SUBSTRING/STARTSWITH/GETASLOWERCASE`, `FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE`, `FABLE_RETAIL_STLP_MULTIMAP_EQUAL_RANGE`, `FABLE_RETAIL_CBANKFILE_FILENAMEINDICES`, includes stlport/fable_containers.h, stlport/fable_map.h, fable_string.h, engine/CArray.h, engine/CPackedUIntArray.h (the header-based CBankFile model).
- 00/9c/CBankFile_AddEntry_009cd620.cpp: by-value CCountedPointer via engine/CCountedPointer.h; CBankFile vtable modelled with slot 6 = AddEntryMultipleFiles; no Flags line.
- 00/9c/CBankFile_InitEntry_009ce050.cpp: local retail CBankFile view, fable_compiler.h, RELOCATION_MATCH.
- 00/9d/CBankFile_UpdateEntry_009d1320.cpp: fable_compiler.h, `#pragma intrinsic(memcpy, memset)`, engine/CArray.h.
- 00/9c/CBankFile_EntryExists_009cc190.cpp: local CBankFile_Retail; body to copy as the inline EntryExists.
- None of the CBankFile rows carry a `// Flags:` line (default flags, no /EHsc) -- consistent with no SEH frame here. Do NOT add /EHsc.

## 7. Old draft model errors
- Names: `SymbolIndices` at +0xb8 must be `OldSymbolIndices`; CPackedUIntArray member names wrong (PackedInts/Size; test is GetSize()); operator= return type; Equals/SetFilenames/InitEntry/ctor are private in PDB; map node `find` returning node* instead of `_M_find` + `!= end()`.
- Local struct models for 7 classes that have shared headers (list in 4).
- resize: draft declares resize(n, value) with explicit `blank` local and literal 0s; retail more likely `X.resize(index+1)` through STLport's inline 1-arg resize (value-init _Tp() temporaries in the frame); the CCharString vector's 1-arg resize is out of line.
- Tail merge: retail keeps two InitEntry calls and cross-jumps only the ~filename + ~stateBlock tail; filename locals at different slots (arm1 0x14, arm2 0x18) and different temp-flag bits (1 / 2). The draft's two arms are already separate; the lane over-merges (726 v 741). Distinct per-arm objects / scopes are the lever (e.g. each arm's `{ CCharString filename(...); InitEntry(...); }` block, or arm2's filename temp evaluated differently) -- untested.
- `index` must be one function-scope variable (ebp in both arms), loaded from node->second in arm1.
- CBankFileEntryUpdateData size: `new` must be 0x18 (shared header is 0x20).

## 0x00627400

# DOSSIER 0x00627400 CTCQuickAccessMenu::ConstructExpressionCategoryList (742 B)

## 1. Retail shape (xdis 00627400 0x2f6)
- __thiscall, `ret 0x10` (4 stack args). NO frame pointer (sub esp,0x1c; push ebx/ebp/esi/edi), NO SEH prologue (no fs:0) -> /O2 /Oy, no EH frame, like sibling 0x626E40 (sub esp,0x40, also no SEH, `ret 0x14`).
- Registers: ebp = this (mov ebp,ecx) for the whole body; ebx = CManager::GetInstance() result; edi = &items; esi = zero (xor esi,esi at 0x6274B6) in the CreateMenuEntry/then part, then esi = plist_cat in else; ebx re-zeroed (xor ebx,ebx 0x62760A) in else; tail `xor esi,esi` 0x6276A8.
- Frame: +0x10 menu_title (CWideString), +0x14/+0x18 pmenu_entry_cat CCountedPointer (inline ctor: new(0xC) {1,0x428B10,Data}), +0x1c/+0x20 pactions_list, +0x24/+0x28 plist_cat_component (both via out-of-line 0x429C15), +0x30 temps (CCharString/CWideString).
- Literals (verified from Fable.exe): 0x1258D24 "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW_FOR_QUICK_ACCESS", 0x1236ED0 "PC_BUTTON_BASTARD", 0x122D70E "", 0x1237150 "UI_SELECTION_ARROW_RIGHT", 0x1258C20 "UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", 0x1258CD4 "UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS", 0x122DE44 L"%d".
  OLD DRAFT USES MADE-UP STRINGS ("UI_QAM_CATEGORY_TEMPLATE", "PC_BUTTON", "ARROW", "UI_QAM_SUBLIST", "UI_QAM_ENTRIES", L"(%d)") -- all wrong.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @0x0066BB10) and FableWin.pdb (_fablewin_api.tsv 0x017CCC57/0x0211C870, naming_stragglers signatures) agree:
  `?ConstructExpressionCategoryList@CTCQuickAccessMenu@@AAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@@Z`
  = private: void __thiscall CTCQuickAccessMenu::ConstructExpressionCategoryList(CWideString&, NUISystem::CList*, std::vector<std::pair<long,long> >&, NUISystem::CComponent*). Manifest label is RIGHT.
- Draft errors: `const CWideString&` (PDB: non-const `CWideString&`, AAV); method not declared in engine/CTCQuickAccessMenu.h (header has ConstructItemCategoryList + both ContructItemLists, but NOT this one -> must be added (private) or declared in the row; sibling header line 25 is the model). (CTCInventoryAbilities has a 5-arg overload -- not ours.)
- Callers: 4 calls at 0x627E7A/ED0/F26/F7C, all in GetExpressionsListUI 0x00627BE0 (not landed). Same TU as 0x626E40/0x6276F0.
- Param names (sibling style): menuparam, plist, items, ptop_list.

## 3. Callees (landed file / name)
| addr | landed | PDB/landed name | draft spelling |
|---|---|---|---|
| 0x41E5F2 | 00/41/NUISystemCManager_GetInstance_0041e5f2.cpp | static NUISystem::CManager::GetInstance() __fastcall | WRONG: CFrontEndManager::GetInstance |
| 0x428494 | not landed | NUISystem::CManager::CreateMenuEntryComponent(const CWideString&,float,float,const CCharString&,float,const CCharString,float,CCharString) (NUISystem_CManager.h:97) | args ok, strings wrong |
| 0x41DB1D | 00/41/NUISystemCManager_CreateComponent_0041db1d.cpp | CManager::CreateComponent(const CCharString&,bool) | ok |
| 0x429C15 | 00/42/CCountedPointerNUISystemCComponent_CCountedPointer_00429c15.cpp | CCountedPointer<NUISystem::CComponent>::CCountedPointer(CComponent*) (size TU, out of line) | WRONG: draft fakes CCountedPointer<CList>/<CSubList> specialisations |
| 0x428B10 | 00/42/global_DeleteData_00428b10.cpp | CCountedPointer<CComponent>::DeleteData (static) | draft global DeleteData |
| 0x5CC772 | 00/5c/CTCInventoryStats_GetXString_005cc772.cpp | static CTCInventoryStats::GetXString() __fastcall | WRONG: free GetXString() |
| 0x99BA70 | 00/99/CWideString_Sprintf_0099ba70.cpp | CWideString::Sprintf(const wchar_t*,...) __cdecl | WRONG: Format |
| 0x99AED0/0x99B720/0x99B8D0/0x99B510 | 00/99/... | CWideString() / copy / operator+= / ~ | ok |
| 0x99EBF0/0x99EAE0 | 00/99/... | CCharString(const char*,long=-1) / ~ | ok |
| 0x42CD84 | 00/42/NUISystemCDeletion_CDeletion_0042cd84.cpp | NUISystem::CDeletion::CDeletion(const CDeletion&) | WRONG: "SUIActionData" copy ctor |
| 0x42ABCA | 00/42/stdlistNUISystemCComponent_list_0042abca.cpp | std::list<NUISystem::CComponent*>::~list() (= inline ~CDeletion's member dtor) | WRONG: ~SUIActionData |
| 0xBFEA0E | global_malloc | malloc (STLport list sentinel, inline default list ctor) | ok |
| 0xBFEA1A / 0xBFE9BC | global operator new / delete | | ok |
| 0x626750 | not landed | CTCQuickAccessMenu::ContructItemLists(vector<pair<long,long>>&, CList*, CComponent*, CCountedPointer<CComponent>&) (in header) | WRONG: draft passes CCountedPointer<CSubList>* |
| inline | GFIntToWideString(long) __fastcall (Ego_r ?GFIntToWideString@@YI?AVCWideString@@J@Z, FableWin too) = CWideString t; t.Sprintf(L"%d",v); | draft "CountString" WRONG |
Vtable slots (engine/NUISystem_CComponent.h, NUISystem_CList.h; confirmed by landed rows' comments):
- +0x0C slot 3 CComponent::FindChild(CCharString) (draft ok)
- +0xEC slot 59 CComponent::AddChild(CCountedPointer<CComponent>&)
- +0xF8 slot 62 CComponent::SetDeletion(NUISystem::CDeletion) by value (draft "SetAction" WRONG)
- +0x170 slot 92 CComponent::MakeUnavailable() (draft "Disable" WRONG)
- +0x260 CList::AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>&) (draft "AddEntry" WRONG)
- +0x270 slot 156 CList::InitialiseOffsets(unsigned long) (draft "SetNumEntries" WRONG)
- +0x278 slot 158 CList::SetDefaultChild(long) (draft "SetSelection" WRONG; call as SetDefaultChild(0L))

## 4. Members / layouts
- No `this` member read (this only passed to ContructItemLists). vector<pair<long,long>> _M_start/_M_finish (+0,+4): items.empty()/size() via stlport.
- CDeletion (engine/NUISystem_CDeletion.h): {EDeletionMethod Method +0; list<CComponent*> AssociatedParents +4}; Method=CDeletion::UI_DELETE_THIS_INSTANCE (1). Opt-ins: FABLE_RETAIL_CDELETION (declares default+copy ctors out of line -- retail here INLINES the default ctor (malloc 0xC + self-links) and CALLS copy 0x42CD84), FABLE_RETAIL_CDELETION_BODIES. Sibling 0x626E40 uses NO CDeletion macro and gets the same shape (implicit ctors; the auto_inline(off) list specialisations make ~list a call).
- CCountedPointer (engine/CCountedPointer.h): {Data, Info}; macros FABLE_RETAIL_CCOUNTEDPOINTER (ctor tests stored Data; written for 0x626E40), _CALLED_CTOR, _VISIBLE_CALLED_CTOR, _DTOR_DECL_ONLY, _VISIBLE_CALLED_DTOR, _RELEASEDATA(_DECL_ONLY).
- Strings: fable_string.h with FABLE_RETAIL_CCHARSTRING / FABLE_RETAIL_CWIDESTRING (Sprintf at 0x99BA70).
- Locally-modelled classes in the draft that HAVE shared headers: CCharString, CWideString (fable_string.h), CCountedPointer (engine/CCountedPointer.h), NUISystem::CComponent (NUISystem_CComponent.h), CList (NUISystem_CList.h), CManager (NUISystem_CManager.h), CFrontEndManager (CFrontEndManager.h -- not needed), SUIActionData -> NUISystem::CDeletion (NUISystem_CDeletion.h), CTCQuickAccessMenu (CTCQuickAccessMenu.h), vector (stlport), CTCInventoryStats (CTCInventoryStats.h).

## 5. Callee visibility
- Same TU (retail order): ContructItemLists 0x626110 (map) / 0x626750 (vector) / ConstructItemCategoryList 0x626E40 (LANDED) / this 0x627400 / GetItemsListUI 0x6276F0 (LANDED, separately) / GetExpressionsListUI 0x627BE0.
- Out-of-line COMDATs at odd addresses (size-TU instances): 0x429C15 CCountedPointer ctor, 0x42CD84 CDeletion copy ctor, 0x42ABCA ~list (+ clear 0x42A18F) -- sibling forces these with `#pragma auto_inline(off)` + explicit `template<> list<CComponent*>::~list() {}` and `_List_base<...>::clear()` specialisations.
- Inline in retail: first CCountedPointer ctor (new 0xC inline), all CCountedPointer dtors (dec/cmp/call [eax+4]/delete), CDeletion default ctor + list ctor, GFIntToWideString, vector size/empty.
- First counted ptr inline but the two CreateComponent ones call 0x429C15: same as sibling 0x626E40 (it gets it with the shared header + FABLE_RETAIL_CCOUNTEDPOINTER; copy its include set exactly).

## 6. Sibling landed rows
- rebuild/src/compiled/00/62/CTCQuickAccessMenu_ConstructItemCategoryList_00626e40.cpp -- NEAR-TWIN (same TU, same strings, same idioms). Macros: FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _CCOUNTEDPOINTER, _CTCBASE; includes fable_string.h, CCountedPointer.h, CTCBase.h, CThing.h, CTCTargetingBase.h, NUISystem_CList.h, NUISystem_CManager.h, NUISystem_CDeletion.h, CTCInventoryStats.h, CTCQuickAccessMenu.h; `#pragma auto_inline(off)` list specialisations; no Flags line (default /O2 /Oy); inline __fastcall GFIntToWideString.
- rebuild/src/compiled/00/62/CTCQuickAccessMenu_GetItemsListUI_006276f0.cpp (local models, older style; not the idiom to copy).
- 00/62/CTCQuickAccessMenu_OnCreate_00625910.cpp, _DrawPreparePrimitivesForRendering_00625950.cpp, _GlobalUpdate_00625ed0.cpp, _CTCQuickAccessMenu_006259e0.cpp.

## 7. Old-draft model errors
- Everything modelled locally; wrong names (see table) and wrong string literals; `const CWideString&` param; CountString/Format instead of GFIntToWideString/Sprintf; SUIActionData(1) ctor instead of `CDeletion d; d.Method=CDeletion::UI_DELETE_THIS_INSTANCE;` (retail stores Method after the list malloc/self-link, matching default-ctor-then-assign); `entries` typed CCountedPointer<CSubList> and passed by pointer.
- Flow/order in retail else branch: CCountedPointer<CComponent> plist_cat_component(pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS",false)); CList* plist_cat = static_cast<CList*>(plist_cat_component.Get()) (esi loaded BEFORE AddChild); pmenu_entry_cat->AddChild(plist_cat_component); plist_cat->InitialiseOffsets(items.size()); CCountedPointer<CComponent> pactions_list(CManager::GetInstance()->CreateComponent("UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS",false)); static_cast<CList*>(pactions_list.Get())->SetDefaultChild(0L); ContructItemLists(items, plist_cat, ptop_list, pactions_list).
- then-branch: CComponent* arrow = pmenu_entry_cat->FindChild("UI_SELECTION_ARROW_RIGHT"); if (arrow) { CDeletion delete_arrow; delete_arrow.Method=UI_DELETE_THIS_INSTANCE; arrow->SetDeletion(delete_arrow); } pmenu_entry_cat->MakeUnavailable();
- Head: pmanager = CManager::GetInstance(); CWideString menu_title(menuparam); if(!items.empty()){ menu_title += CTCInventoryStats::GetXString(); menu_title += GFIntToWideString(items.size()); } (size computed before the GFIntToWideString CWideString ctor, as sibling).
- Residual noted in hand305: fe/zero register swap (retail fe=ebx, zero=esi) and one release `cmp [eax],0` immediate. The draft's pmanager-as-CFrontEndManager + local models likely contributed; re-try with the sibling's exact headers/macros first (sibling 0x626E40 has ebp=pmanager because it spills this; here ebp=this).

## 0x007EB6A0

# DOSSIER 0x007EB6A0 (748 B) -- CTCActionUseSearch::OnActionUse(CThing&)

## 1. Retail shape (xdis 007eb6a0 0x2fc)
- __thiscall, `ret 4` (one arg). NO frame pointer (`sub esp,58h` first; ebp used as a GPR = this) -> TU built with /Oy (no optimize("y",off)). No SEH prologue, no /GS cookie.
- Frame 0x58. Prologue order: `mov eax,[esp+4] ; sub esp,58h ; push ebp,esi,edi ; push eax ; mov ebp,ecx ; call 0x7EAB90` (base OnActionUse(user) first).
- ebx pushed LATE (0x7EB7AC, right before `xor bl,bl` = hadReward) and popped at 0x7EB93D before the stealth block -> bool hadReward lives only in the container..theft region.
- Frame slots: [esp+0xC] CCharString animation; [esp+0x10] float height, LATER REUSED for the hero pointer (`mov [esp+10h],edi` at 0x7EB744, reloaded `mov edi,[esp+14h]` at 0x7EB864 with ebx pushed); [esp+0x14] (= [esp+0x18] with ebx pushed) single shared LowerBound key slot for all 6 GetTC lookups; [esp+0x18] C3DVector temp, [esp+0x24] CPhysicsMeshObject temp.
- Height: `GetPhysicsMeshInfo()` vslot +0x174 (slot 93) tested; true -> `PhysicsTC->GetPhysicsMeshObject().GetBoundingBoxZEdge()` (hidden-ret ptrs pushed both before the first call), mesh-object temp dtor 0xA127D0, then `.Z` ([esp+0x20]) -> height; else height = 2.0f (0x40000000). Height compared `fcomp [0x1270F98]` (1.15f), `test ah,41h ; jne` -> `if (height > 1.15f)` "SEARCH_HIGH"@0x1270FA8 else "SEARCH_LOW"@0x1270F9C; single CCharString::operator=(const char*) call with pushed-arm idiom.
- Early exit: tutorial true -> `jne 0x7EB97A` = straight to ~CCharString + epilogue (a `return` inside the CCharString scope).
- Every GetTC is the FABLE_RETAIL_CTHING_GETTC_EXPANDED shape (presence test in place, key stored after the test, `lea esi,[x+44h]`, LowerBound 0x40F020, `mov esi,[esi+4]` end, conditional-expression choice, `mov reg,[eax+4]`).

## 2. Name / signature (two sources)
- vtable: rebuild/manifest/rtti-vtables.tsv `CTCActionUseSearch 0123f4dc 28 007eb6a0`; slot 28 of every CTCActionUse* class is OnActionUse (base 0x007EAB90 landed as `CTCActionUseBase::OnActionUse(CThing&)`, header CTCActionUseBase.h line 52 "slot 28").
- Ego_r.pdb names (egor_pdb_names.tsv / egor_signatures.tsv): `?OnActionUse@CTCActionUseSearch@@UAEXAAVCThing@@@Z` (Ego address 0x007E5430, FableWin 0x0181B190 / 0x025428B0).
- => **Name: `void CTCActionUseSearch::OnActionUse(CThing& user)`**, public virtual, non-const, void. Manifest label `vfunc_28` is a placeholder; draft's `Use(CThing* user)` is WRONG (pointer, wrong name). Mangled leaf: `?OnActionUse@CTCActionUseSearch@@UAEXAAVCThing@@@Z`.
- Only xref: vtable data 0x0123F54C. No CTCActionUseSearch.h shared header exists.

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | landed name | PDB name | draft mismatch |
|---|---|---|---|---|
| 0x7EAB90 | 00/7e/CTCActionUseBase_OnActionUse_007eab90.cpp | CTCActionUseBase::OnActionUse(CThing&) | ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z | draft `Use(CThing*)` WRONG |
| vslot +0x174 | CTCPhysicsBase.h:190 | `virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() const` | same | draft void*, ok-ish |
| 0x6B0850 | 00/6b/CTCPhysicsBase_GetPhysicsMeshObject_006b0850.cpp | CPhysicsMeshObject CTCPhysicsBase::GetPhysicsMeshObject() const | same | ok |
| 0xA11BA0 | 00/a1/CPhysicsMeshObject_GetBoundingBoxZEdge_00a11ba0.cpp | C3DVector GetBoundingBoxZEdge() const | ?GetBoundingBoxZEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ | draft `GetWorldCentre` WRONG |
| 0xA127D0 | 00/a1/CPhysicsMeshObject_CPhysicsMeshObject_00a127d0.cpp | ~CPhysicsMeshObject() | | ok |
| 0x99E4B0 / 0x99EFE0 / 0x99EAE0 | 00/99/CCharString_* | CCharString ctor / operator=(const char*) / dtor | | use fable_string.h (FABLE_RETAIL_CCHARSTRING), not local class |
| 0x686D40 | 00/68/CTCBase_GetPlayerManager_00686d40.cpp | CPlayerManager& CTCBase::GetPlayerManager() | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ | draft returns pointer WRONG |
| 0x449970 | 00/44/CPlayerManager_GetMainPlayer_00449970.cpp | CPlayer& GetMainPlayer() | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ | draft pointer WRONG |
| 0xA01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseObject* GetPItem() const | | reached via inline CPlayer::GetPPlayerCharacter() (+0x34) and inline CTCOwnedEntity::GetPOwner() (+0xD) |
| 0x40F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap::LowerBound (non-const) | | via GetTC expanded |
| 0x6C9FA0 | 00/6c/CTCHero_GiveTutorialForCategory_006c9fa0.cpp | bool CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) | ?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z | draft `long category` -> use (ETutorialCategory)0x21 |
| 0x74E500 | 00/74/CTCContainerRewardHero_HasReward_0074e500.cpp | `long CTCContainerRewardHero::HasReward()` | ?HasReward@CTCContainerRewardHero@@QBE_NXZ (const, bool) | LANDED SPELLING DISAGREES WITH PDB (const bool) -> NAME_FIXES candidate; draft put it on CTCSearchableContainer (inherited, ok as call) |
| 0x812340 | 00/81/CTCSearchableContainer_Search_00812340.cpp | `void Search(long frames)` | ?Search@CTCSearchableContainer@@QAEXAAVCThing@@@Z | LANDED PARAM WRONG (PDB: CThing&); draft `Search(CThing*)` also wrong -> `Search(CThing&)` |
| 0x686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | CWorld& CTCBase::GetWorld() const | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ | draft pointer WRONG; +0x60 = message event manager ptr |
| 0x4C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | const C3DVector& CThing::GetPos() const | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft `GetPosition` WRONG |
| 0x687540 | 00/68/CMessageEventManager_AddMessage_00687540.cpp | CMessageEvent* AddMessage(EMessageEventType, unsigned long lifespan, const C3DVector&, CThing* originator, CThing* target, float, float, long) | ?AddMessage@...W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z | draft has 7 args, WRONG (see 7) |
| 0x6C6990 | NOT LANDED; declared in CTCHeroOpinionDeedLog.h (FABLE_RETAIL_CTCHEROOPINIONDEEDLOG) | long PostDeed(EOpinionDeedType, const C3DVector&, const CThing*, CThingAICreature*, EOpinionPostDeedType, bool, bool, const CFaction*, bool, long) | ?PostDeed@CTCHeroOpinionDeedLog@@QAEJ... | draft arg order right |
| 0x7EACB0 | 00/7e/CTCActionUseBase_IsThisTheft_007eacb0.cpp | bool IsThisTheft() const | ?IsThisTheft@CTCActionUseBase@@QBE_NXZ | ok |
| 0x6869D0 | 00/68/CTCBase_GetDefinitionManager_006869d0.cpp | const CGameDefinitionManager& GetDefinitionManager() const | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ | draft `GetDefinitions` WRONG |
| 0x57C2E6 | 00/57/CTCHeroStats_SetMorality_0057c2e6.cpp | void CTCHeroStats::SetMorality(long) | ?SetMorality@CTCHeroStats@@QAEXJ@Z | ok |
| 0x747030 | 00/74/CDrawEnvironmentUser00747030_SetWorld_00747030.cpp | `CDrawEnvironmentUser_00747030::SetWorld(CWorld*)` | **?SetActionPermittedToSneak@CTCStealth@@QAEXPBVCCreatureActionBase@@@Z** | LANDED NAME WRONG (NAME_FIXES): body = `ActionPermittedToSneakID(+0x2c) = action->GetActionID()` (0x692620 = CCreatureActionBase::GetActionID, landed); Ego CTCStealth +0x2c = ActionPermittedToSneakID; called on TCI_STEALTH(54) TC. Draft `CTCShopper::BoughtSomething` WRONG |

## 4. Members / interface ids
- this (CTCBase): ParentThing +4 (CTCBase.h).
- CThing (engine/CThing.h, retail size 0x98): UsingComponents bitset +0x20 (words +0x24 / +0x28 tested in place), Components CVectorMap +0x44, PhysicsTC +0x60, Dead bit +0x91. Opt-ins: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(id,&p) form = retail), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE, IsDead()/IsAlive().
- Interface ids (ETCInterfaceType.h, Ego_r values, masks verified): TCI_HERO=41 (`test ah,2` on +0x24), TCI_CONTAINER=60 (+0x24 0x10000000; NOT "searchable container" id; component cast to CTCSearchableContainer), TCI_OWNED_ENTITY=51 (+0x24 0x80000), TCI_HERO_OPINION_DEED_LOG=81 (+0x28 0x20000), TCI_HERO_STATS=4 (`test byte [edi+20h],10h`), TCI_STEALTH=54 (+0x24 0x400000; draft called it CTCShopper 0x36 WRONG).
- `[user+0x100]` = CThingCreatureBase::PCurrentAction (CCountedPointer data, CThingCreatureBase.h:445/193 static asserts); accessor `PeekPCurrentAction() const` (FableWin ?PeekPCurrentAction@CThingCreatureBase@@QBEPBVCCreatureActionBase@@XZ, inline, `mov eax,[creature+100h]`) under FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION (line 269-276). So `static_cast<CThingCreatureBase&>(user).PeekPCurrentAction()`; draft's `user->ShopValue` WRONG.
- 0x013B8790 = CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass; byte +0xF6 = SubtitleRenderer(+0x18).InCutscene(+0xDE): `CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()` -- FableWin ?IsInCutscene@CSubtitleRenderer@@QBE_NXZ + ?GetSubtitleRenderer@CPlayerGui@@QAEAAVCSubtitleRenderer@@XZ; opt-in FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE (CSubtitleRenderer.h:111, precedent row 0x0056AD40 CTCBoastUI::Activate). Draft `g_pGameState->TutorialsDisabled` WRONG.
- CPlayer +0x34 = PlayerCharacter; inline `GetPPlayerCharacter()` (CPlayer.h:217-220, FableWin, `lea ecx,[eax+34h] ; call GetPItem`), returns CThingPlayerCreature* (needs engine/CThingPlayerCreature.h). Precedent: CTCFishing_FrameUpdate_007b0cd0.cpp line 231 `GetPlayerManager().GetMainPlayer().GetPPlayerCharacter()`.
- CTCOwnedEntity (engine/CTCOwnedEntity.h): POwner +0x0D packed; inline `GetPOwner()` -- retail calls GetPItem TWICE (0x7EB82D test, 0x7EB848 arg) = two GetPOwner() calls. Header defines FABLE_RETAIL_CTCBASE/CINTELLIGENTPOINTER itself.
- CTCHeroStats +0x28 = Morality (read in place, `add ecx,[esi+28h]`), header CTCHeroStats.h (FABLE_RETAIL_CTCHEROSTATS). GetDefinitionManager() +0xDC ptr -> +0xE0 long (morality change for searching/theft; name unproven, check CGameDefinitionManager.h / hero-stats def).
- CPhysicsMeshObject.h (FABLE_RETAIL_CPHYSICSMESHOBJECT), CTCPhysicsBase.h (FABLE_RETAIL_CTCPHYSICSBASE), CMessageEventManager.h, CWorld.h, CTCHero.h, CTCActionUseBase.h (FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS for virtual OnActionUse), CTCContainerRewardHero.h, CTCStealth.h, CThingCreatureBase.h, CPlayer.h/CPlayerManager.h, EMessageEventType.h (MESSAGE_EVENT_THEFT=28 = 0x1c), EOpinionDeedType.h (OPINION_DEED_TYPE_CRIME_THEFT=6).
- Draft models LOCALLY classes that HAVE shared headers: CCharString (fable_string.h), C3DVector, CThing, CInterfaceMap(CVectorMap.h), CPhysicsMeshObject, CTCPhysicsBase, CBaseIntelligentPointer (CIntelligentPointer.h), CTCOwnedEntity, CPlayer, CPlayerManager, CTCHero, CTCHeroOpinionDeedLog, CTCHeroStats, CMessageEventManager, CWorld, CTCBase, CTCActionUseBase. (No shared header: CTCActionUseSearch, CTCSearchableContainer.)

## 5. Visibility
- All callees are called out of line in retail; none inlined. GetTC/HasTC/GetPPlayerCharacter/GetPOwner/IsInCutscene/GetSubtitleRenderer/PeekPCurrentAction/CTBaseSingleton::Get are inline (folded).
- IsThisTheft 0x7EACB0 and base OnActionUse 0x7EAB90 are in the same TU (CTCActionUse* sources 0x7EAB90..0x7EFE70, same retail neighbourhood); IsThisTheft is still CALLED here (not inlined) -- declare it out of line, no body.
- LowerBound 0x40F020 out of line (non-const) via GETTC_EXPANDED; CCharString members out of line.

## 6. Sibling landed rows (same TU neighbourhood)
- 00/7e/CTCActionUseMapTable_OnActionUse_007eb570.cpp: `#define FABLE_RETAIL_CINTELLIGENTPOINTER`, CIntelligentPointer.h, CThing.h; `OnActionUse(CThing& user)`.
- 00/7e/CTCActionUseBlowHorn_OnActionUse_007eb9b0.cpp and 00/7e/CTCActionUsePutInInventory_OnActionUse_007eb390.cpp: same include set.
- 00/7e/CTCActionUseBase_OnActionUse_007eab90.cpp: FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS, _CTCBASE, _CTHING, _CCHARSTRING, _CGAMEDEFINITIONMANAGER ..., includes fable_string.h, engine/CTCActionUseBase.h.
- 00/7b/CTCFishing_FrameUpdate_007b0cd0.cpp: hero + GetTC(TCI_HERO,&heroTC)->GiveTutorialForCategory idiom, FABLE_RETAIL_CTHING_GETTC_EXPANDED, CThing_MOREACCESS.
- None of these use #pragma optimize; no frame pointer, consistent with this row.

## 7. Old draft model errors
1. AddMessage args: retail pushes 8 args `(MESSAGE_EVENT_THEFT 0x1c, 0x32, user.GetPos(), &user, owned->GetPOwner(), 0.0f, 0.0f, 0)` -- the `push ecx` (user) at 0x7EB852 is the ORIGINATOR arg; draft passed owner as 4th and only 7 args.
2. Last block is `user.GetTC(TCI_STEALTH,&stealth) -> stealth->SetActionPermittedToSneak(static_cast<CThingCreatureBase&>(user).PeekPCurrentAction())`, not shop/BoughtSomething.
3. Gate is `!CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()`, not a game-state tutorial flag.
4. Height = `GetBoundingBoxZEdge().Z`, not world centre.
5. Signature `OnActionUse(CThing& user)`; references for GetPlayerManager/GetMainPlayer/GetWorld/GetDefinitionManager.
6. Register residue (ebp=this, hero in edi then spilled into the dead height slot; ebx pushed late for hadReward): declare `float height` first in its own scope, hero (`CThingPlayerCreature* hero`) after the animation string; keep `bool hadReward=false` declared just before the container test so ebx is pushed late; GetTC must be the expanded out-param form (one key slot).

## 0x00959F80

# DOSSIER 0x00959F80 CAIStateGroup_HoverKeepInArea::Update (752 B)

## 1. Retail shape (xdis 00959f80 0x300)
- thiscall, plain `ret` (no args), NO frame pointer (`sub esp,54h; push ebx,ebp,esi,edi`), no SEH, no /GS. ebp is a scratch reg (`mov ebp,64h` divisor). Default /O2-style, x87 sin/cos inline (`fcos`/`fsin`/`fsqrt` = /Oi intrinsics).
- ebx = &this->StartPos (lea ebx,[esi+1Ch]) kept live from the GetNearbyWanderPosition call to the end loop.
- Flow: seed=GetWorld().GetWorldSeed() FIRST (edi) -> GetTC(7) nav (bitset bit7 of creature+0x20 stored as byte [esp+13h]; LowerBound 0x40F020; on miss nav = uninitialised frame word [esp+24h]) -> if(!nav->GetNearbyWanderPosition(StartPos,20.0f,target,false)) return (NO Terminate, jumps straight to epilogue 0x95A268)
  -> f = GFRandom(100,seed)*0.003f+0.1f (unsigned->float via fild + `jge`/fadd 2^32 fix-up) ; if(GFRandom(2,seed)==0) f *= -1.0f ; d = GFRandom(100,seed)*0.02f+3.0f
  -> pos=GetCreature().GetPos(); dir = (target.X-pos.X, target.Y-pos.Y, 0) (Y difference computed first, then X; Z stored as 0 imm) ; dir.AccurateNormalise() (result ignored)
  -> a = f * TWO_PI (double const 0x0124F2B8 = (double)6.2831855f, `fmul qword`) ; look = target + perp*sin(a)*d + dir*cos(a)*d, perp = (dir.Y, -dir.X, 0.0f)  [decoded below]
  -> if(AIAction_LookAtPos(1, look /*by value*/, 0x11, 0.05f) == 2) { movement = GFRandom(3, GetWorld().GetWorldSeed()) + 8;  do { p=GetCreature().GetPos(); dx=p.X-StartPos.X; dy=p.Y-StartPos.Y; r = vcall slot22(this, 2, target, look, reached, movement); } while (r==4 && !((float)sqrt(dy*dy+dx*dx) < 20.0f)); }  Terminate();
- Look-vector x87 decode (exact): X = (T.X + dY*s*d) + dX*c*d ; Y = (T.Y + (-dX)*s*d) + dY*c*d ; Z = (T.Z + 0.0f*s*d) + dZ*c*d. Each term is ((v*trig)*d) (two separate fmuls: operator*(float) chained), and the + chain is (target + perpTerm) + dirTerm. -dX is fchs'd before the angle multiply; the 0.0f*s is a real fmul by const 0x0122DEDC (perp.Z literal 0.0f, not folded). cos part computed before sin part (scheduling).
- `reached` bool& argument of the slot-22 call is the SAME frame byte [orig esp+13h] that held the GetTC bitset-test byte (frame-slot sharing).
- Constants: 0x01294EAC 0.003f, 0x0122EE58 0.1f, 0x0122DEE0 -1.0f, 0x01294EA8 0.02f, 0x01294EB8 3.0f, 0x0122DEDC 0.0f, 0x01294EB0 20.0f, 0x0122DCB4 2^32, 0x3D4CCCCD 0.05f imm, 0x41A00000 20.0f imm.

## 2. Identity
- Name CONFIRMED: ?Update@CAIStateGroup_HoverKeepInArea@@UAEXXZ (virtual void Update(), public). Sources: Ego_r.pdb (egor_pdb_names.tsv @0x0097E7B0, a different build address), FableWin.pdb (_fablewin_api.tsv 0x0182FF41), vtable 0x0128C6F4 slot 12 = 0x00959F80 (rtti-vtables.tsv; slot 12 is Update in every sibling: HoverWander 0x959BF0, HoverWanderAroundThing 0x959A60, HornetQueen 0x95C1F0). Manifest label correct.
- Class chain: CAIStateGroup_HoverKeepInArea : CAIStateGroup_HoverMovementBase (no data, 0x1c) : CAIStateGroup_EnemyCreatureBase (PTargetThing CIntelligentPointer<CThingCreatureBase> +0x14, size 0x1c) : CAIStateGroupBase. Sizeof 0x28.
- xref: only reference is the vtable (.rdata 0x0128C724).

## 3. Callees (address | landed row | PDB name | draft spelling)
- 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (protected, const, returns CWorld&) | draft `GetPWorld()` returning CWorld* -- WRONG.
- 0x0049D810 | 00/49/CWorld_GetWorldSeed_0049d810.cpp | ?GetWorldSeed@CWorld@@QBEPAKXZ (unsigned long* const) | draft `GetRandomSeed()` returning unsigned long& -- WRONG.
- 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) | draft local CInterfaceMap::LowerBound(const long*) -- use engine/CVectorMap.h + CThing::GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED, as in sibling 0x00959A60).
- 0x006CB6E0 | 00/6c/CTCCreatureNavigation_GetNearbyWanderPosition_006cb6e0.cpp (landed leaf ?..@@QAE_NABVC3DVector@@MPAV2@_N@Z) | PDB (Ego_r) ?GetNearbyWanderPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@_N@Z = bool (const C3DVector&, float, C3DVector& result, bool) const | FLAG: PDB has const method + C3DVector& out; landed row/draft use non-const + pointer. Same codegen (pointer push), but name/sig mismatch -> NAME_FIXES candidate.
- 0x00497890 | 00/49/global_GFROR13_00497890.cpp | ?GFROR13@@YIJK@Z (long __fastcall(unsigned long)) | draft returns unsigned long. Use engine/GFRandom.h GFRandom(range, seed) (?GFRandom@@YIKKPAK@Z, inline): 4 inline expansions here (ranges 100, 2, 100, 3); draft's hand-written NextRandom should be replaced.
- 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft `GetPosition()` -- WRONG.
- 0x00A14440 | 00/a1/C3DVector_AccurateNormalise_00a14440.cpp | ?AccurateNormalise@C3DVector@@QAEAAV1@XZ (returns C3DVector&) | draft `Normalise()` void -- WRONG.
- 0x0097D580 | 00/97/CAIStateGroupBase_AIActionLookAtPos_0097d580.cpp | ?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z (protected; EAIActionReturn(EAIActionRepeat, C3DVector by value, long, float)) | draft long(long,...) -- types wrong; header decl under FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS (CAIStateGroupBase.h:81).
- vtable slot 22 (+0x58) = 0x0091D7F0 | 00/91/CAIStateGroupHoverMovementBase_AIActionMoveToPosAndFacePos_0091d7f0.cpp | ?AIAction_MoveToPosAndFacePos@CAIStateGroup_HoverMovementBase@@MAE?AW4EAIActionReturn@@W4EAIActionRepeat@@ABVC3DVector@@1AA_NW4EMovementType@@@Z (protected virtual; (EAIActionRepeat, const C3DVector& pos, const C3DVector& face, bool& reached, EMovementType)) | draft `AIAction_MoveToPosProximityAndFacePos` -- WRONG NAME (that is the non-virtual 0x0091D510).
- 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ | ok.

## 4. Members
- this+0x04 PBrain (CAIBrain*), brain+0x18 PCreature -> GetCreature(): FableWin ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ inline; opt-ins FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS + _GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE (engine/CAIStateGroupBase.h:130, CAIBrain.h).
- this+0x1C StartPos (C3DVector) -- struct_layouts_egor.tsv `CAIStateGroup_HoverKeepInArea 40 28 C3DVector StartPos`; engine/CAIStateGroup_HoverKeepInArea.h names it only as flattened StartPos_X/Y/Z with NO FABLE_RETAIL_* branch (vptr as data). Draft calls it `Centre` -- WRONG. Landed sibling IsRunnable 0x00959F30 models the class locally with `C3DVector StartPos; // +0x1c` over a local EnemyCreatureBase (_enemy[8]).
- this+0x14 PTargetThing (CIntelligentPointer<CThingCreatureBase>) not touched; engine/CAIStateGroup_EnemyCreatureBase.h offers FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE; engine/CAIStateGroup_HoverMovementBase.h (a real polymorphic class, needs that macro) declares NO virtuals -- slot 22 must be declared somewhere (sibling 0x959A60 declares it locally on a local HoverMovementBase with v01..v21 placeholders).
- creature+0x20 bitset (bit 7 = TC 7 navigation present), creature+0x44 interface CVectorMap (first +0x44, last +0x48) -> engine/CThing.h with FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- Draft local models with a shared header: C3DVector, CThing, CTCCreatureNavigation, CWorld, CAIBrain, CAIStateGroupBase, CAIStateGroup_HoverMovementBase, CAIStateGroup_HoverKeepInArea, CInterfaceMap (-> CVectorMap.h).

## 5. Visibility
- GFRandom: inline body in engine/GFRandom.h (all 4 sites expand inline here).
- GetTC / bitset test / LowerBound: GetTC inline, LowerBound out of line (FABLE_RETAIL_CTHING_GETTC_EXPANDED).
- Same TU neighbours (HoverKeepInArea/Hover* TU, retail order): 0x00959800 WanderAroundThing::IsRunnable, 0x00959A60 WanderAroundThing::Update, 0x00959BF0 HoverWander::Update (pending DIFFER, same rotation x87 issue), 0x00959EE0 KeepInArea slot1, 0x00959F30 KeepInArea::IsRunnable (landed), 0x00959F80 this, 0x0095A270 HoverAttackArea slot1. Nothing in those is called here, so no noinline stand-ins needed.
- TWO_PI: `const double TWO_PI=6.283185482025146484375; // ?TWO_PI@@3NB` spelled locally in 00/87/NMaths_GetRootsNumericallyCTanMethod_0087aa10.cpp (no shared header defines it).
- sin/cos: `#pragma intrinsic(cos, sin)` with extern "C" decls (see 00/84/NCameraModesCBase_UpdateFixLookDirection_0084cdf0.cpp).
- C3DVector ops in engine/C3DVector.h: operator+ / operator*(float) / operator- with paren opt-ins FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP, _SCALE_X_PAREN, _SUBTRACT_XY_GROUP.

## 6. Siblings
- 00/95/CAIStateGroupHoverKeepInArea_IsRunnable_00959f30.cpp: no Flags/pragmas; local models; `(float)sqrt(dy*dy+dx*dx) > 20.0f` with dx then dy computed (same dist idiom as this loop).
- 00/95/CAIStateGroupHoverWanderAroundThing_Update_00959a60.cpp: no pragmas; `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED`, CThing.h + CVectorMap.h + C3DVector.h; local CAIStateGroupBase with v01..v21 + slot 22 AIAction_MoveToPosAndFacePos; GetTC(TCI_NAVIGATION,&navigation); same "GetNearbyWanderPosition / LookAt==2 / Random(3)+8 / do-while(result==4)" skeleton.
- 00/95/CAIStateGroupHoverWanderAroundThing_IsRunnable_00959800.cpp: `// Flags: none`; full shared-header retail opt-in set (FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _GETCREATURE, CAIBRAIN_GETCREATURE, CWORLD, CAISTATEGROUP_ENEMYCREATUREBASE, CAISTATEGROUPBASE_GOTOBED_WORLD for GetWorld, ...) + engine/GFRandom.h GFRandom(n, GetWorld().GetWorldSeed()).
- 00/95/CAIStateGroupHoverHornetQueen_GoToSpawnPoint_0095b020.cpp (/GS) shows the same "GetWorldSeed fetched at top" pattern.

## 7. Draft model errors
1. Rotation formula wrong: draft angle uses the 3..5 value and look = T + (rot(-dX,dY))*d. Retail: angle a = f*TWO_PI with f the FIRST draw (+-0.1..0.397), d = SECOND draw (3.0..4.98); look = target + perp*sin(a)*d + dir*cos(a)*d, perp=C3DVector(dir.Y,-dir.X,0.0f). (draft's distance/turn names are swapped).
2. 2*pi must be a double constant (`fmul qword`), e.g. TWO_PI double: `cos(f*TWO_PI)`.
3. Random: replace NextRandom with GFRandom; seed pointer `unsigned long* seed = GetWorld().GetWorldSeed();` hoisted to function entry (before GetTC); final draw uses a fresh GetWorld().GetWorldSeed().
4. Names/types (section 3): GetWorld, GetWorldSeed, GetPos, AccurateNormalise, AIAction_MoveToPosAndFacePos (slot 22), StartPos, EAIActionRepeat/EAIActionReturn/EMovementType enums.
5. Draft's HoverMovementBase skips EnemyCreatureBase (uses _pad); fine for layout but use shared headers.
6. `reached` reuses the GetTC present byte slot: draft reused the `has` variable to force it; keep in mind if frame differs.
7. Uninitialised nav on GetTC miss is retail-faithful ([esp+24h]); keep GetTC(...,&nav) form.

## 0x0094F180

# DOSSIER 0x0094F180 CAIStateGroup_BalverineCornered::IsRunnable (754 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 0094f180 0x302)
- thiscall, no args, plain `ret` (0x94F467 / 0x94F471), returns bool in al. FPO: NO `push ebp; mov ebp,esp` (ebp = target), no SEH.
- Prologue `sub esp,0x38 ; push ebx ; push ebp ; push esi ; mov esi,ecx` ... `push edi` is scheduled AFTER the first HasTC test (0x94F194).
- TWO epilogues: true exit 0x94F45E (`mov al,1`) and ONE shared false exit 0x94F468 (`xor al,al`) that every early-out jumps to
  (first HasTC(8) miss, null/dead target, !IsAwareOfThing, range test, LOS tests). Old draft: 737 v 754, frame 0x3c v 0x38.
- ebx = 8 held as a constant: used as TCI_TARGETING key (0x94F1A3) AND for `test [attacker+0x6c], bl` (ThingTypeFlags & 8).
- bool hitByMelee lives in memory at [esp+0x11] (set 0 at 0x94F1D4 before the 0x42 HasTC test, set 1 at 0x94F24D).
- Stack slot [esp+0x14] is shared: the LowerBound key temp (8 / 0x42 / 7), the GetEvent out-param, and the interface-7 nav pointer
  (nav stays UNINITIALISED when HasTC(7) fails -- retail has no else).

## 2. Identity (PROVEN, 2 sources) -- manifest label is CORRECT
- rebuild/manifest/rtti-vtables.tsv: vtable 0x0128C0BC (CAIStateGroup_BalverineCornered) slot 11 = 0x0094F180; base CAIStateGroupBase
  vtable 0x01288C60 slot 11 = IsRunnable (engine/CAIStateGroupBase.h comment, `mov al,1` base 0x008FD282). xref: only ref is the vtable.
- Ego_r.pdb (ghidra_out/egor_signatures.tsv 0x00973A70) and FableWin.pdb (_fablewin_api.tsv 0x0184BEC1 / 0x02BD63D0):
  `?IsRunnable@CAIStateGroup_BalverineCornered@@MAE_NXZ` = protected virtual `bool IsRunnable()` (non-const, no params).
- Donor order Ego_r InitStateGroup 0x973960, GetLandPos 0x973980, IsRunnable 0x973A70, Update 0x974000 == retail
  0x94F080 / 0x94F090 / 0x94F180 / (0x94F640) -> same TU, GetLandPos defined just before.

## 3. Callees (landed file / landed name / PDB name; FLAG = old draft spelling wrong)
| addr | landed file | correct name (PDB) | draft spelling |
|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (non-const), from inline CThing::GetTC for types 8 and 0x42 | local CInterfaceMap::LowerBound -- FLAG (local model; also draft DEFINES the body: must not) |
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | const LowerBound, type 7 (const thing) | FLAG same |
| vslot 29 (+0x74) of interface 8 | -- | CTCTargetingBase::GetPBestEnemyCreatureForAttack() const -> CThingCreatureBase* (CTCTargetingAI slot29 = Ego ?GetPBestEnemyCreatureForAttack@CTCTargetingAI@@UBEPAVCThingCreatureBase@@XZ; sibling 0x0094F480 uses it) | GetPTarget -- FLAG |
| 0x0094F890 | 00/94/CTCEntityEvents_GetEvent_0094f890.cpp | CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, CFilter_Bind2<NEntityEvents::CIsHitEventHitByMelee,NEntityEvents::CIsHitEventBlocked> >(E**, const F&) const, `ret 8` (header opt-in FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_DECL declares `template<class E,class F> bool GetEvent(E**,const F&)const`) | non-const, non-template -- FLAG (should be const member template) |
| 0x00810F40 | 00/81/global_DrawGetEnvironment_00810f40.cpp (ICF-folded `mov eax,[ecx+8];ret`, landed name unrelated) | Ego_r ?GetHitParams@CEventHitBase@NEntityEvents@@QBEABVCHitParameters@@XZ (returns const CHitParameters&) | CEventHitBy::GetHitInfo -- FLAG |
| +0x50 of hit params then 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CHitParameters::PThingHitting (CIntelligentPointer<CThing>; Ego_r +0x58, retail +0x50) .GetPItem(); NB CEventHitBase::GetPHitBy 0x00810FC0 exists but is NOT called -> source spells GetHitParams().PThingHitting.GetPItem() | `Attacker` -- FLAG |
| 0x008645E0 | 00/86/CMainGameComponent_BeginInputLoading_008645e0.cpp (bogus manifest label) | NCreatureAI::IsAwareOfThing, Ego_r+FableWin ?IsAwareOfThing@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z (__fastcall: ecx creature, edx thing, push false) | CanCreatureSeeCreature -- FLAG (name-fixed 2026-11-10 in siblings) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const (engine/CThing.h line 411) | GetPosition -- FLAG |
| 0x00A14440 | 00/a1/C3DVector_AccurateNormalise_00a14440.cpp | C3DVector::AccurateNormalise() -> C3DVector& (?AccurateNormalise@C3DVector@@QAEAAV1@XZ) opt-in FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE | Normalise -- FLAG |
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | CAIStateGroupBase::GetWorld() const -> CWorld& (?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ) | GetPWorld -- FLAG |
| vslot 12 (+0x30) of CWorld | 00/4a/CWorld_GetWorldMap_004ae8b0.cpp | virtual CWorldMap& CWorld::GetWorldMap() (engine/CWorld.h line 111, retail 0x004AE8B0); result UNUSED | GetDebugGraphics -- FLAG |
| 0x00666D10 | 00/66/CThingCreatureBase_GetCombatTypeDef_00666d10.cpp | CThingCreatureBase::GetCombatTypeDef() const -> const CCombatTypeDef& (Ego_r 0x540EE0 ?GetCombatTypeDef@CThingCreatureBase@@QBEABVCCombatTypeDef@@XZ); opt-in FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE | returns pointer, non-const -- FLAG |
| 0x006CBA20 | 00/6c/CTCCreatureNavigation_IsPosNavLineOfSightTo_006cba20.cpp | CTCCreatureNavigation::IsPosNavLineOfSightTo(const C3DVector&) const; opt-in FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS | ok |
| 0x006CB610 | 00/6c/CTCCreatureNavigation_IsPosNavLineOfSightToPos_006cb610.cpp | IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const; opt-in FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT | ok |
| 0x0094F090 | 00/94/CAIStateGroupBalverineCornered_GetLandPos_0094f090.cpp (landed on a stand-in CBalverineLandMethods, takes CGeometryThing*) | PDB ?GetLandPos@CAIStateGroup_BalverineCornered@@ABE?AVC3DVector@@ABVCThing@@@Z = private const `C3DVector GetLandPos(const CThing&) const` | ok (draft correct; landed row's signature is the stand-in) |
| 0x0049D870 | 00/49/CWorld_GetFrame_0049d870.cpp | static long __fastcall CWorld::GetFrame() (?GetFrame@CWorld@@SIJXZ) | GetWorldFrame, unsigned long -- FLAG |
| 0x00668B60 | 00/66/CIntelligentPointer_CThingCreatureBase_CIntelligentPointer_00668b60.cpp | CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase*) out of line; opt-in FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE | `CIntelligentPointer<const CThing>` -- FLAG (header comment: members are CIntelligentPointer<CThingCreatureBase>) |
| 0x00A01B90 / 0x00A01C10 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp / ..._CBaseIntelligentPointer_00a01c10.cpp | CBaseIntelligentPointer::SetPItem / ~CBaseIntelligentPointer | ok |
The two `PX = target` assignments are temp ctor 0x668B60 + INLINE operator= (GetPItem 0xA01B50 then SetPItem 0xA01B90) + dtor -> FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (inline), NOT ..._ASSIGN_OUTOFLINE (sibling 0x94F480 uses out-of-line 0x668B80; this row does not call 0x668B80).

## 4. Members
- this: CAIStateGroupBase (engine/CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS + _GETCREATURE; GetWorld under
  FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD or AIACTIONS+SPEECH) PBrain +0x04 -> CAIBrain creature +0x18 (engine/CAIBrain.h
  FABLE_RETAIL_CAIBRAIN_GETCREATURE: GetCreature() returns CThingAICreature&).
- engine/CAIStateGroup_BalverineCornered.h (generated, NO FABLE_RETAIL_ overlay macro, flattened, vptr+pads): PTarget +0x14,
  PLastJumpTarget +0x1c (both CIntelligentPointer<CThingCreatureBase>), TimeOfLastJump +0x24 (unsigned long), LandPos +0x28 (C3DVector,
  flattened LandPos_X/Y/Z). Author needs an overlay like FABLE_RETAIL_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY in
  engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h (real class over CAIStateGroupBase).
- CThing (engine/CThing.h, retail = Ego_r - 0x10 from ThingTypeFlags on): +0x20 UsingComponents bitset, +0x44 Components map,
  +0x60 PhysicsTC (GetPhysicsTC()), +0x6c ThingTypeFlags (GetTypeFlags(); bit 8 = creature), +0x91 bit0 Dead (IsDead()).
- CTCPhysicsBase (engine/CTCPhysicsBase.h) Radius +0x34; `fld [r+0x34]; fadd st,st` = GetDiameter() (`GetRadius()*2.0f`) likely.
- CCombatTypeDef (engine/CCombatTypeDef.h, FABLE_RETAIL_CCOMBATTYPEDEF) +0x70 = FrontMiddleLimit (draft invented "CorneredRange").
- CHitParameters (engine/CHitParameters.h) PThingHitting retail +0x50; CEventHitBase (engine/NEntityEvents_CEventHitBase.h) PHitParams +8;
  header declares GetPHitBy/GetPWeaponUsed but NOT GetHitParams -- needs adding (opt-in).
- Locally modelled classes in the draft that HAVE shared headers: C3DVector, CVectorMap(CInterfaceMap), CThing, CThingCreatureBase,
  CCombatTypeDef, CBaseIntelligentPointer/CIntelligentPointer, CTCTargetingBase, CTCEntityEvents, NEntityEvents::CEventHitBy, CTCCreatureNavigation,
  CWorld, CAIBrain, CAIStateGroupBase, CAIStateGroup_BalverineCornered, CTCPhysicsBase (as CThingDef -- wrong class).

## 5. Visibility
- GetLandPos 0x94F090 is same TU, defined earlier, but retail CALLS it (not inlined; hidden return ptr esp+0x30).
- GetEvent<...> 0x94F890 is a template COMDAT instantiated in this TU, called out of line.
- LowerBound 0x40F020/0x4365B0 out of line inside inline CThing::GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED, as sibling).
- GetHitParams 0x810F40, GetPos 0x4C73D0, GetWorld 0x8FCFC0, GetCombatTypeDef, AccurateNormalise, CIntelligentPointer ctor: out of line.

## 6. Siblings
- 00/94/CAIStateGroupBalverineJumpAtFleeingEnemy_IsRunnable_0094f480.cpp (SAME TU, retail neighbour, RELOCATION_MATCH): copy its
  #define list (FABLE_RETAIL_CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CTHING_ORIGINAL_VTABLE, CTCPHYSICSBASE_LASTPOSITION,
  STLP_BITSET_INDEX, CTHING_GETTC_EXPANDED, CINTELLIGENTPOINTER, CINTELLIGENTPOINTER_CTOR_OUTOFLINE, CTHINGPHYSICAL, CTHINGGAMEOBJECT,
  CTHINGCREATUREBASE, CTHINGAICREATURE, CTCPHYSICSBASE, CAISTATEGROUPBASE(+_MEMBERS,+_GETCREATURE), CAIBRAIN_GETCREATURE) and idioms:
  `CTCTargetingBase* targeting; if (GetCreature().GetTC(TCI_TARGETING,&targeting)) {...} return false;` single nested condition / single false,
  GetCreature() re-called at every use (no creature local), enemy->IsDead(), NCreatureAI::IsAwareOfThing(&GetCreature(), enemy, false).
- 00/94/CAIStateGroupBalverineCornered_Update_0094f640.cpp, _GetLandPos_0094f090.cpp, _InitStateGroup_0094f080.cpp (same class; stand-in styles).
- 00/93/CAIStateGroupMinionReactToEnemy_IsRunnable_00937f70.cpp (slot 29 + IsAwareOfThing + PEnemy = enemy).

## 7. Old draft model errors
- Wrapped body / early returns were tried (NOTES.md hand277 line 107); retail = one shared false exit -> nested-condition form like 0x94F480.
- Retail calls GetCombatTypeDef UNCONDITIONALLY before `if(!hitByMelee)` (draft matches) but reads FrontMiddleLimit; test is
  `!(dist2 < range*range)` -> false (fcompp; test ah,0x41; jne false); dist2 computed Y*Y first then X*X (GetSquaredMagnitude2D default branch).
- 2D offset: creature.Pos - target.Pos with only X,Y kept (stores esp+0x18/0x1c) -- use full C3DVector diff + GetSquaredMagnitude2D like sibling.
- away = GetCreature().GetPos() - target->GetPos() (creature first); AccurateNormalise(); jumpBack = away*GetDiameter() + GetCreature().GetPos().
- GetWorld().GetWorldMap(); statement with discarded result sits between jumpBack and the nav lookup.
- hit-info: `lea ecx,[eax+0x50]` wants the member reached through a const reference (GetHitParams().PThingHitting.GetPItem()), not pointer+offset.
- Attacker condition: attacker && !attacker->IsDead() && (attacker->GetTypeFlags() & 8) (Thing type creature) -> target = (CThingCreatureBase*)attacker.
- Order at the end: LandPos = land; TimeOfLastJump = CWorld::GetFrame(); PLastJumpTarget = target; PTarget = target; return true.
- engine/CHitParameters.h opt-ins FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP / _AUGMENTATIONS change the map/vector sizes: pick the combination that puts PThingHitting at +0x50 (retail lea ecx,[eax+0x50]); Ego_r default is +0x58.

## 0x00AC53F0

# DOSSIER 0x00AC53F0 -- C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk&)

## 1. Retail shape (python3 $S/w17/xdis.py 00ac53f0 0x303)
- thiscall, `ret 4` (one arg: const C3DMeshFileMaterialChunk&). Returns this in eax (ctor).
- Prologue `push ebp; mov ebp,esp; and esp,-8; sub esp,0x24; push ebx/esi/edi` = VC7.1 8-byte-ALIGNED frame (ebp is the frame
  pointer because of the alignment, not /Oy-). Arg read as [ebp+8] into edi (and RELOADED at 0xac5562 `mov edi,[ebp+8]` at loop top).
  No SEH frame (no /EHsc unwinding state). Locals esp-relative (+0xc..+0x18 four CCharString temps, +0x20/+0x24 the map pointer,
  +0x28/+0x2c the ext-properties temp, later reused for FindNextMap's result).
- Same aligned frame in the landed same-family loops C3DMeshFileMaterialChunk::GetMap 0x00AC1020 and const GetMap 0x00AC1160
  (both `push ebp; mov ebp,esp; and esp,-8`), whereas FindNextMap 0x00AC0EE0 / ReadFromFile 0x00AC5770 / default ctor 0x00AC56F0 do not.
  => the alignment comes from the CCountedPointer (8-byte) loop local + while/FindNextMap shape; copy GetMap's model for the loop.
- Layout of the code: init -> ext-properties -> new CMapFilenames + Reset -> FindFirstMap -> `if (Data==0)` falls into the EXIT
  block (bank zeros with an ecx zero register, CalculateMapFlags, texture dtor, ret) placed BEFORE the loop body; loop body (switch,
  jump table 0xAC56C0 + byte index table 0xAC56D8) after the `ret`; bottom test `jne 0xac5562` / `jmp 0xac5522`. A plain `while`.

## 2. Name / signature (two sources)
- FableWin.pdb (ghidra_out/fablewin_pdb_names.tsv 0347f260) and Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00c3ed00):
  `??0C3DMeshFileEngineMaterialChunk@@QAE@ABVC3DMeshFileMaterialChunk@@@Z`. Manifest label is RIGHT (naming_stragglers Fable.exe sig).
- Vtable stored: 0x0129EFC8 = C3DMeshFileEngineMaterialChunk (shared header comment, RTTI). Base: CChunkedFileSuperChunk (not "C3DMeshFileChunk").
- xref.py: 0 direct callers.

## 3. Callees (landed file / landed name / PDB name)  -- DRAFT SPELLINGS MOSTLY WRONG
| addr | landed row | PDB name | draft spelling |
|---|---|---|---|
| 0x00AA8360 | 00/aa/CChunkedFileSuperChunk_CChunkedFileSuperChunk_00aa8360.cpp | CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) | WRONG: "C3DMeshFileChunk(const char*)" |
| 0x0099E4B0 / 0x0099EC30 / 0x0099EFB0 / 0x0099EAE0 | 00/99/CCharString_* | CCharString() / copy / operator= / ~CCharString | ok |
| 0x00ABFCD0 | 00/ab/C3DMeshFileMaterialChunk_HasExtendedProperties_00abfcd0.cpp | ?HasExtendedProperties@C3DMeshFileMaterialChunk@@QBE_NXZ (const, bool) | ok |
| 0x00AC0B70 | 00/ac/C3DMeshFileMaterialChunk_GetExtendedProperties_00ac0b70.cpp | ?GetExtendedProperties@C3DMeshFileMaterialChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMaterialExtChunk@@@@XZ | WRONG class: draft "C3DMeshFileExtendedMaterialChunk" -> C3DMeshFileMaterialExtChunk; returns CCountedPointer<const C3DMeshFileMaterialExtChunk> |
| 0x00ABFDC0 | 00/ab/CWorld_DrawGetEnvironment_00abfdc0.cpp (MISLABEL, propagated) | ?GetSelfIllumination@C3DMeshFileMaterialExtChunk@@QBEKXZ (Ego_r 00c3c8e0) | class wrong in draft; body `mov eax,[ecx+0x10]; ret`. Put in NAME_FIXES.tsv (landed name CWorld::DrawGetEnvironment is wrong) |
| 0x00AC0E50 | 00/ac/C3DMeshFileMaterialChunk_FindFirstMap_00ac0e50.cpp | ?FindFirstMap@C3DMeshFileMaterialChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMapChunk@@@@XZ | WRONG: draft "FindFirstTexture" / C3DMeshFileTextureChunk |
| 0x00AC0EE0 | 00/ac/C3DMeshFileMaterialChunk_FindNextMap_00ac0ee0.cpp | ?FindNextMap@C3DMeshFileMaterialChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMapChunk@@@@ABV2@@Z (arg const CCountedPointer<const C3DMeshFileMapChunk>&) | WRONG: "FindNextTexture" |
| 0x00AC5260 | 00/ac/C3DMeshFileEngineMaterialChunk_CalculateMapFlags_00ac5260.cpp | ?CalculateMapFlags@C3DMeshFileEngineMaterialChunk@@AAEXXZ (private) | ok |
| 0x00BFEA1A / 0x00BFE9BC | operator new / operator delete | | ok |
| vcall [vtbl+4] on CMapFilenames | CIVCountedPointeeBase::Delete (slot 1) | | draft "DeleteThis" -> Delete |
| [Info+4] | CCPPointerInfo::DeleteFunc (count block) | | draft SCountBlock/destroy -> CCPPointerInfo {RefCount,DeleteFunc,Data} |
- No call to CMapFilenames::CMapFilenames 0x00AC5190 here: it is EXPANDED inline (0xac54bc: +4=1, vtable 0x0129EFBC, 4x CCharString()).
- Texture chunk class is C3DMeshFileMapChunk (header engine/C3DMeshFileMapChunk.h; MapType is EMaterialMapType, private; Filename private).

## 4. Members
this = C3DMeshFileEngineMaterialChunk (rebuild/include/engine/C3DMeshFileEngineMaterialChunk.h, opt-in FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK,
retail sizeof 0x54; needs FABLE_RETAIL_CCHUNKEDFILECHUNK, _CCHUNKEDFILESUPERCHUNK, _DEF_CLASS_BASE, _CIVCOUNTEDPOINTER, _CCHARSTRING):
+0x2c long MaterialIndex, +0x30 CCharString Name, +0x34 CIVCountedPointer<CMapFilenames> MapFilenames, +0x38..+0x44 Diffuse/Bump/Reflection/
IlluminationMapBankIndex (ulong), +0x48 MapFlags, +0x4c SelfIllumination, +0x50 TwoSided, +0x51 Transparent, +0x52 BooleanAlpha, +0x53 DegenerateTriangles
(names = struct_layouts_egor.tsv at PDB offsets +8). Nested CMapFilenames : CIVCountedPointeeBase, DiffuseMap +8, BumpMap +0xc, ReflectionMap +0x10, IlluminationMap +0x14.
Arg = C3DMeshFileMaterialChunk (engine/C3DMeshFileMaterialChunk.h, FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK, retail 0x50): MaterialIndex +0x2c (read),
TwoSided +0x30 (read), Name +0x4c (copied). Members PRIVATE -> inline accessors needed (GetMaterialIndex/GetName/GetTwoSided -- FableWin has
?GetMaterialIndex@... for the engine class; the material getters are inline); the retail header declares neither the accessors nor
HasExtendedProperties/GetExtendedProperties/FindFirstMap/FindNextMap -- author must add declarations (Methods-subclass trick or header opt-in).
C3DMeshFileMapChunk (FABLE_RETAIL? -- header is unconditional retail, 0x3c): MapType +0x2c (EMaterialMapType), MapDescription +0x30, Filename +0x34, CRC +0x38; private.
C3DMeshFileMaterialExtChunk: SelfIllumination +0x10 (FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK).
CIVCountedPointeeBase: IVRefCount +4, slot1 Delete; opt-in FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR (IVRefCount(1)).
CCountedPointer (engine/CCountedPointer.h): Data +0, Info +4; CCPPointerInfo RefCount +0, DeleteFunc +4, Data +8. Opt-ins: FABLE_RETAIL_CCOUNTEDPOINTER,
_RELEASEDATA(_DECL_ONLY), _DTOR_DECL_ONLY, _VISIBLE_CALLED_DTOR/_CTOR, _CALLED_CTOR, _RESET_CALLS_DTOR.
Draft models LOCALLY (all have shared headers): CCharString (fable_string.h), CCountedPointer (engine/CCountedPointer.h), CIVCountedObject/CIVCountedPointer
(engine/CIVCountedPointeeBase.h, engine/CIVCountedPointer.h), C3DMeshFileChunk (-> CChunkedFileSuperChunk.h), C3DMeshFileMaterialChunk, C3DMeshFileTextureChunk
(-> C3DMeshFileMapChunk.h), C3DMeshFileExtendedMaterialChunk (-> C3DMeshFileMaterialExtChunk.h), C3DMeshFileEngineMaterialChunk.

## 5. Visibility
- Same TU (EngineMaterialChunk, 0xAC5190..0xAC62xx), defined earlier: CMapFilenames ctor 0x00AC5190 (inline in header; expanded here), GetChunkInfo 0xAC51D0,
  CalculateMapFlags 0xAC5260 (called, not inlined -- fine), WriteToFile 0xAC52F0. Later: default ctor 0xAC56F0, ReadFromFile 0xAC5770.
- Other TU (material chunk, 0xABFxxx-0xAC1xxx): HasExtendedProperties, GetExtendedProperties, GetSelfIllumination, FindFirstMap, FindNextMap -> declare only (called out of line).
- MapFilenames assignment at 0xac54ed = CIVCountedPointer::Reset ADOPT form: `if(Object){ if(--IVRefCount==0) Delete(); Object=0;} Object=new` (no AddRef).
  ReadFromFile 0xAC5770 uses exactly: FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR + FABLE_RETAIL_CIVCOUNTEDPOINTER + FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT,
  `MapFilenames.Reset(new CMapFilenames);`. Note retail here zero-tests `mov ecx,[esi+0x34]; test ecx` then `dec [ecx+4]` (pointer in ecx before dec) -> default
  Release branch (or _RELEASE_INNER) -- check which one ReadFromFile's 0xAC5A86 block matches.
- Loop pointer assignment (0xac5629..0xac5674) = inline CCountedPointer ShareData (`Info != info` compare, release old, store, inc) = operator=; the
  FindNextMap result temp's dtor keeps its zero stores (`xor ecx,ecx; mov [esp+28],ecx; mov [esp+2c],ecx` 0xac56a6) => zeroing dtor/ReleaseData visible.
- No STLport helpers.

## 6. Siblings to copy idioms from
- 00/ac/C3DMeshFileEngineMaterialChunk_ReadFromFile_00ac5770.cpp: shared-header model, defines FABLE_RETAIL_CCHARSTRING, _CDATAINPUTSTREAM, _CCHUNKEDFILECHUNK,
  _CCHUNKEDFILESUPERCHUNK, _DEF_CLASS_BASE, _CIVCOUNTEDPOINTEEBASE_CTOR, _CIVCOUNTEDPOINTER, _CIVCOUNTEDPOINTER_RESET_ADOPT, _C3DMESHFILEENGINEMATERIALCHUNK. BEST template.
- 00/ac/C3DMeshFileMaterialChunk_GetMap_00ac1020.cpp / _00ac1160.cpp: same aligned frame, FindFirstMap/FindNextMap while loop (`while (map.Object && ...) map = FindNextMap(map);`),
  stand-in finder bodies under `#pragma auto_inline(off)` (local CCountedPointer model -- pre-wave; the draft copied this).
- 00/ac/C3DMeshFileEngineMaterialChunk_C3DMeshFileEngineMaterialChunk_00ac56f0.cpp: default ctor init list order (local model).
- 00/ac/C3DMeshFileMaterialChunk_FindFirstMap_00ac0e50.cpp / FindNextMap_00ac0ee0.cpp: shared engine/CCountedPointer.h with SetStaticCast.
- 00/ac/C3DMeshFileEngineMaterialChunk_CalculateMapFlags_00ac5260.cpp.

## 7. Draft model errors vs disassembly
1. SWITCH CASES WRONG. Index table 0xAC56D8 (MapType+1, 0..10) = 00 05 00 05 05 05 01 02 05 03 04; jump table 0xAC56C0 = [0xac5589 Diffuse(+8),
   0xac55f2 Illumination(+0x14), 0xac5580 Transparent, 0xac55ac Bump(+0xc), 0xac55cf Reflection(+0x10), 0xac5618 default].
   => case -1,1: DiffuseMap; case 5: IlluminationMap; case 6: Transparent=true; case 7: BumpMap; case 8: ReflectionMap; 0,2,3,4,9: nothing.
   Draft had 8=Bump, 9=Reflection (off by one) -- this also changes the table range (`cmp ecx,0xa` = values -1..9).
   Code order of the case blocks: Transparent, Diffuse, Bump, Reflection, Illumination; CCharString temp slots in order Diffuse(0xc) Bump(0x10) Reflection(0x14) Illum(0x18).
2. Each case is `MapFilenames->X = map->GetFilename();` (copy ctor of [map+0x34] into a temp, operator=, dtor) -- an expression temporary.
3. Ext props: `SelfIllumination = material.GetExtendedProperties()->GetSelfIllumination();` single expression (temp at esp+0x28, getter called on Data
   directly, temp released, no zero stores) inside `if (material.HasExtendedProperties())`. SelfIllumination = 0 stored before the if.
4. Statement order in retail: base ctor("EMTL" @0x0129DED4), vptr, Name(), MapFilenames=0, DegenerateTriangles=false (init list), then MaterialIndex =, Name = GetName()
   (temp at +0xc), TwoSided =, SelfIllumination = 0, ext block, Transparent=false, BooleanAlpha=false, MapFilenames.Reset(new CMapFilenames), map = FindFirstMap(),
   while(map) {switch; map = FindNextMap(map);} bank indices = 0 (x4), CalculateMapFlags(), ~map. Matches draft order.
5. Map pointer type must be CCountedPointer<const C3DMeshFileMapChunk> (const FindFirstMap/FindNextMap) -- FindNextMap is passed &map directly (no converting temp, 0xac5618).
6. Base is CChunkedFileSuperChunk, not a fake C3DMeshFileChunk with pad; CIVCountedObject/DeleteThis -> CIVCountedPointeeBase/Delete; SCountBlock -> CCPPointerInfo.
7. The previous residue (703v755): ours used ebp for `material` with a zero register; retail's aligned frame -- the switch's four CCharString temps changed VC's frame decision
   in the old local model; the wrong case values (range 1..9 vs -1..9) and wrong pointer constness (extra converting temp) are untested causes.

## 0x007E6450

# DOSSIER 0x007E6450 CTCOracleMinigame::UpdateTints (756 B incl. 4-entry jump table at 0x007E6734)

## 1. Retail shape (xdis 007e6450 0x304)
- thiscall, `void`, plain `ret` (no args). NO frame pointer (sub esp,0x44; push ebx/ebp/esi/edi) -> default /Oy, no optimize("y",off).
- /GS cookie: `mov eax,[0x139C8A8]` -> [esp+0x4c], `call 0x00BFE9F9` (__security_check_cookie) before ret -> `// Flags: /GS`.
  Cause: the local CGameEvent (unsigned char Data[32]) -> char array in frame. No SEH prologue.
- Loop alignment padding inside the body (`lea esp,[esp]` at 0x7E646C, `nop` at 0x7E65DF): ordinary /O2 loop alignment, not size-opt. Not optimize("s").
- Register plan: esi=this, ebx=&SymbolsFiring[i] (walk pointer; Symbols[i]=[ebx-0x10], FramesFired[i]=[ebx+0x10]),
  edi=input (front()), ebp=expression (switch result) and later the appearance pointer; loop counter 4 in [esp+0x1c] counting down.
- Floats (read from Fable.exe): 0x0122F59C=0.5f, 0x0124F2B8=6.2831854820251465 (double = (double)6.2831855f), 0x0122DED8=1.0f,
  0x01230014=255.0f, 0x0122DCB4=4294967296.0f (unsigned->float fixup). Strings 0x01270860 "SND_MM_EXPRESSION_SEQUENCE_INCORRECT",
  0x0127083C "SND_MM_EXPRESSION_SEQUENCE_CORRECT".

## 2. Identity (two sources) -- manifest label is CORRECT
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv): `?UpdateTints@CTCOracleMinigame@@QAEXXZ` at Ego_r 0x007E1910, between Quit 0x7E1780 and
  FrameUpdate 0x7E1C60 -- same order as retail Quit 0x7E62D0 < 0x7E6450 < FrameUpdate 0x7E6750 (vtable 0x012707B4 slot 9).
- Only caller: 0x007E676B inside landed FrameUpdate (rebuild/src/compiled/00/7e/CTCOracleMinigame_FrameUpdate_007e6750.cpp line 45 `UpdateTints();`).
- Shared header rebuild/include/engine/CTCOracleMinigame.h (FABLE_RETAIL_CTCORACLEMINIGAME_CLASS) already declares `void UpdateTints(); // 0x007E6450`.
- Leaf: `?UpdateTints@CTCOracleMinigame@@QAEXXZ` -> public, non-const, void(void).

## 3. Callees (landed file | PDB / proven name | old draft spelling)
| addr | landed row | real name | draft |
|---|---|---|---|
| 0x00449960 | 00/44/CPlayerManager_GetMainPlayer_00449960.cpp | `const CPlayer& CPlayerManager::GetMainPlayer() const` (?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ) | returned pointer -- WRONG type (ref) |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | `CThingPlayerCreature* CPlayer::GetPControlledCreature() const` | `GetCharacterThing` -- WRONG NAME |
| 0x009F1760 | 00/9f/CGameEvent_AddData_009f1760.cpp | `void CGameEvent::AddData(long, const void*)` (?AddData@CGameEvent@@QAEXJPBX@Z) | `CMemoryBuffer::Copy(unsigned long,..)` -- WRONG |
| (inline) | engine/CGameEvent.h FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE | `AddSLONG(long)` (FableWin ?AddSLONG@CGameEvent@@QAEXJ@Z) | `AddLONG` -- WRONG |
| 0x0041649C | 00/41/CMainGameComponent_AddGameEvent_0041649c.cpp | `CMainGameComponent::AddGameEvent(const CGameEvent&)` | ok |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | `free` (STLport allocator deallocate of the list node) | ok (but should come from list::pop_front) |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | CCharString(const char*, long=-1) / ~CCharString | ok |
| 0x007E60F0 | 00/7e/CTCOracleMinigame_PlaySoundW_007e60f0.cpp | `unsigned long PlaySoundW(const CCharString&)` (PDB spelling, <windows.h> rename) | `PlaySound` -- use PlaySoundW |
| 0x007E62D0 | 00/7e/CTCOracleMinigame_Quit_007e62d0.cpp | `void Quit()` | ok |
| 0x0049D870 | 00/49/CWorld_GetFrame_0049d870.cpp | `static long __fastcall CWorld::GetFrame()` (?GetFrame@CWorld@@SIJXZ) | free fn `GetWorldFrame` -- WRONG |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound | local CInterfaceMap -- use CThing::GetTC |
| 0x00686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | `CWorld& CTCBase::GetWorld() const` (?GetWorld@CTCBase@@IBEAAVCWorld@@XZ) | returned CWorld* -- ref |
| (global) 0x01375550 | -- | `CMainGameComponent::ConstantFPS` via inline `CMainGameComponent::GetConstantFPS()`; retail calls GetWorld() then reads the static: spell `GetWorld().GetFramesPerSecond()` with `static long GetFramesPerSecond(){return CMainGameComponent::GetConstantFPS();}` (idiom: 00/6c/CTCWife_IsOfferingSex_006ceea0.cpp line 39/49) | g_FramesPerSecond -- rename |
| 0x004BFAE0 | 00/4b/CParticleSystem_Kill_004bfae0.cpp (file misnamed; labels_chunk711 + functions.tsv say RemoveTintColour) | `NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour(void*)` (Ego_r ?RemoveTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAX@Z, FableWin 0x01D0A000) | `ClearTintColour` -- WRONG NAME |
| 0x004C00B0 | 00/4c/NGraphicAppearanceCTCGraphicAppearance_SetTintColour_004c00b0.cpp | `SetTintColour(const CRGBColour&, void*)` | ok |
| 0x00BFEA70 | 00/bf/global_ftol2_00bfea70.cpp | __ftol2 (compiler) | -- |
| 0x00BFE9F9 | global_securitycheckcookie | /GS | -- |
| (global) 0x013B86A0 | -- | `CMainGameComponent::PMainGameComponent` via inline `CMainGameComponent::Get()`; member PlayerManager +0x1c | g_pMainGameComponent -- rename |
- NOTE: RemoveTintColour is declared in NO shared header (grep rebuild/include). NGraphicAppearance_CTCGraphicAppearance.h
  FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT declares only HasDummyObject + SetTintColour; author must add a declaration
  (new opt-in or extend TINT) -- landed rows CTCFlashOnHit_FrameUpdate_007e33b0 / CTCCreatureTintEffect_FinalCleanup_007cf040 declare it locally.
- NAME_FIXES candidate: rebuild/src/compiled/00/4b/CParticleSystem_Kill_004bfae0.cpp models it as CParticleSystem::Kill.

## 4. Members
- this (CTCOracleMinigame.h, FABLE_RETAIL_CTCORACLEMINIGAME_CLASS: CTCBase base, retail layout 0xEC):
  ParentThing +0x04 (CTCBase; GetParentThing() under CTCBase opt-in), TimeBetweenOutputs float +0x20, NumOutputsFired long +0x24,
  Output[32] long +0x28, Symbols[4] CThing* +0xb0, SymbolsFiring[4] long +0xc0, FramesFired[4] long +0xd0,
  InputsPending std::list<EOracleMinigameInput> +0xe0 (retail one-pointer STLport list), PDef const COracleMinigameDef* +0xe8.
- ParentThing +0x30 bit 15 (`mov ecx,[eax+30h]; test ch,ch; js skip`): this is CThing::UsingComponents (std::bitset<274> at +0x20, CThing.h
  static assert line 405) word 4 bit 15 = component 143 = TCI_EXPRESSION (ETCInterfaceType.h:148). So the test is
  `!ParentThing->HasTC(TCI_EXPRESSION)` (CThing.h:298/309), NOT a "paused" flag as the draft says.
- Symbols[i] +0x28 bit 27 = UsingComponents bit 91 = TCI_GRAPHIC_APPEARANCE_NEW (ETCInterfaceType.h:96); then Components (+0x44) LowerBound:
  exactly CThing::GetTC<T>(id, T**) under FABLE_RETAIL_CTHING_GETTC_EXPANDED (CThing.h:340; map pointer local, end via [map+4], ternary end-or-greater).
- hero thing +0x90: CThing::PlayerNumber (signed char, CThing.h:85, protected) via inline `GetPlayerNumber()` (CThing.h:435, returns unsigned long, under FABLE_RETAIL_CTHING?).
  Retail does `mov al,[eax+90h]` then byte store -> `event.SetPlayer(creature->GetPlayerNumber())` (SetPlayer(long) truncates to signed char).
- CGameEvent (engine/CGameEvent.h): Type +0, Player +4, Data[32] +5, EndPos +0x25, Valid +0x26, Replacement +0x27; inline ctor under
  FABLE_RETAIL_CGAMEEVENT_INLINE (needs FABLE_RETAIL_CGAMEEVENT_CTOR? see lines 14-36), AddSLONG under FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE.
  Retail stores Player=-1 first, then Type 0x27, EndPos/Valid/Replacement, then Player=al (SetPlayer after ctor). Event type 0x27.
- COracleMinigameDef.h: TintRed +0x38, TintGreen +0x3c, TintBlue +0x40 (unsigned long; Ego_r struct_layouts agree) -- draft TintR/G/B spellings wrong.
- CRGBColour.h: storage B,G,R,A; ctor `CRGBColour(r,g,b,a){R=r;G=g;B=b;A=a;}` (body assigns). Args evaluated right-to-left -> b first.
- Draft LOCAL models that have shared headers: CCharString (fable_string.h), CRGBColour, CThing (+CInterfaceMap -> CVectorMap.h),
  CTCGraphicAppearance (NGraphicAppearance_CTCGraphicAppearance.h), CPlayer/CPlayerManager (CPlayerManager.h), CGameEvent,
  CMainGameComponent, CWorld, COracleMinigameDef, CTCOracleMinigame, the list node (stlport/fable_containers.h list).

## 5. Callee visibility / inline helpers
- **OutputsFiring() 0x007E5D00** (same TU, defined earlier, landed 00/7e/CTCOracleMinigame_OutputsFiring_007e5d00.cpp): body is
  `for(i<4) if(SymbolsFiring[i]!=0) return true; return false;` -- byte-for-byte the first loop of UpdateTints (xor eax; cmp [ecx],0; jne; inc; add 4; cmp 4; jl).
  Retail inlined it: write `if (!OutputsFiring() && !ParentThing->HasTC(TCI_EXPRESSION))` with OutputsFiring visible/inline in the TU (header declares it out-of-line; provide an inline body in-row or a forceinline twin). The draft's anyFiring flag is a stand-in.
- STLport list (FABLE_RETAIL_STLP_LIST): size() (counting loop; FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE alternative swaps head/walker regs --
  retail keeps head in ecx, walks eax: `mov eax,[ecx] ... mov eax,[eax]; inc edx; cmp eax,ecx` = the default for-loop form, try it first),
  front(), pop_front() -> erase(begin()) inline (unlink next/prev, deallocate). Retail calls free with NO null test, while
  allocator::deallocate has `if(p) free(p)` -- check this (fable_containers.h:336); landed rows using pop_front: CTCWeaponTrail_UpdateWeaponTrails_0076efa0, NUISystemCTextSlider_ProcessEvent_00549440.
- Draft's "count then reload InputsPending for front()" error: retail re-reads [esi+0xe0] after counting (size() then front() as separate calls).
- GFSin (engine/GFSin.h, PDB ?GFSin@@YINM@Z): `inline double GFSin(float a){return sin(a*(double)6.2831855f);}` -> matches
  `fmul 0.5f; fmul qword 2pi; fsin`. So s = GFSin(t*0.5f). Draft used sin(t*0.5f*PI) -- WRONG constant (pi vs 2pi).
- CWorld::GetFramesPerSecond / CMainGameComponent::GetConstantFPS / Get(): inline statics.

## 6. Siblings (same class, same TU)
- 00/7e/CTCOracleMinigame_FrameUpdate_007e6750.cpp: no Flags; macros CCHARSTRING, STLP_LIST, CENGINEPERSISTENTPRIMITIVEBASE, DEF_CLASS_BASE,
  CINTELLIGENTPOINTER, CTCBASE, CTCORACLEMINIGAME_CLASS, CGAMECOMPONENT, CMAINGAMECOMPONENT, ... includes engine/CTCOracleMinigame.h, CMainGameComponent.h, CWorld.h. Uses CWorld::GetFrame(), InputsPending.size().
- 00/7e/CTCOracleMinigame_PlaySoundW_007e60f0.cpp, _Quit_007e62d0.cpp: FABLE_RETAIL_CCHARSTRING + FABLE_RETAIL_CTHING_GETTC_EXPANDED + engine/CThing.h + CVectorMap.h (Quit still local models; CMainGameComponent::Get/GetConstantFPS idioms).
- 00/7e/CTCOracleMinigame_OutputsFiring_007e5d00.cpp (inline candidate above).
- Tint idioms: 00/7c/CTCBurntEffect_FrameUpdate_007cd9a0.cpp (FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT + engine/CRGBColour.h; notes "4-arg CRGBColour(r,g,b,a) ctor, VC evaluates channels right to left"), 00/7e/CTCFlashOnHit_FrameUpdate_007e33b0.cpp (RemoveTintColour/SetTintColour local decls).
- /GS + CGameEvent: 00/7e/CTCFireheartMinigame_Answer_007e7c00.cpp (`// Flags: /GS`, inline CGameEvent ctor). AddSLONG under INLINE_ADDMORE: 00/69/CInputProcessCreatureMovement_ProcessInput_00691d70.cpp.

## 7. Old draft model errors vs disassembly
1. Tint channel order: retail ftols +0x40 (TintBlue) -> bl, +0x3c (TintGreen) -> stack, +0x38 (TintRed) -> al; colour bytes B=blue,G=green,R=red,A=0xff.
   Spell `CRGBColour colour(255.0f - PDef->TintRed*s, 255.0f - PDef->TintGreen*s, 255.0f - PDef->TintBlue*s, 0xff)` with the SHARED RGBA ctor
   (right-to-left arg eval gives Blue first). Each channel: fild (unsigned fixup +2^32), fmul st(1) (s kept on FPU, double from GFSin), fsubr 255.0f.
2. sin constant: GFSin(t*0.5f) (2pi as (double)float), not t*0.5f*3.14159.
3. First loop = inlined OutputsFiring(); condition 2 = !ParentThing->HasTC(TCI_EXPRESSION), not a Flags30 paused bit.
4. Appearance pointer: retail keeps it in ebp, initialised from an uninitialised stack slot at 0x7E65CF and only reassigned when HasTC passes
   (declare `NGraphicAppearance::CTCGraphicAppearance* appearance;` before the loop and use GetTC(TCI_GRAPHIC_APPEARANCE_NEW,&appearance) whose result is ignored; maybe loop via pointer/index).
5. fps read (`mov edx,[ConstantFPS]` after GetWorld call, stored to [esp+0x18]) happens BEFORE GetFrame(): named local `long fps = GetWorld().GetFramesPerSecond();` then `(float)(CWorld::GetFrame()-FramesFired[i]) / (fps*TimeBetweenOutputs)`; t compared `> 1.0f`.
6. Clear branch: `if(--SymbolsFiring[i]) FramesFired[i]=CWorld::GetFrame(); appearance->RemoveTintColour(this);`.
7. Pending block: `if (InputsPending.size()) { EOracleMinigameInput input = InputsPending.front(); switch(input){0..3 -> 0..3, default -1}; ... InputsPending.pop_front(); if (Output[NumOutputsFired++] != input) { PlaySoundW("..INCORRECT"); Quit(); } PlaySoundW("..CORRECT"); FramesFired[input]=CWorld::GetFrame(); ++SymbolsFiring[input]; }`.
   The hero: `CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()` then `->GetPlayerNumber()`.

## 0x00959BF0 (not landed)

# DOSSIER 0x00959BF0 -- CAIStateGroup_HoverWander::Update (739 bytes)

## 1. Retail shape (xdis 00959bf0 0x2f3)
- thiscall virtual, plain `ret` (0x00959EC3 / 0x00959ED2), no args. NO frame pointer (`sub esp,70h; push ebx,ebp,esi,edi`) -> default /Oy, no SEH.
- Two epilogues: loop-exit (counter==3) and the shared Terminate tail at 0x00959EC4/0x00959EC6 (`mov ecx,esi; call 0x008FCF40`).
- Order of evaluation at entry: GetWorld() 0x008FCFC0 -> GetWorldSeed 0x0049D810 FIRST (seed ptr kept in edi for the whole function),
  then the GetTC(7) on GetCreature() (bitset byte bit 7 of +0x20 stored to [esp+13h]; on a miss `nav` stays the uninitialised [esp+28h]).
  => source: `unsigned long* seed = GetWorld().GetWorldSeed();` as the first statement, GetTC hoisted ABOVE the loop (unlike 0x00959A60).
- Loop counter [esp+1Ch] (also the GetTC key slot) 0..3, `inc; cmp 3; jne` at the bottom. Wander-position miss -> `continue`.
- [esp+13h] (the GetTC presence byte) is reused as the `bool& reached` argument of slot 22 (lea [esp+17h] after one push).
- Constants: 4294967296.0f 0x0122DCB4 (unsigned->float fix-up), 0.003f 0x01294EAC, 0.1f 0x0122EE58, -1.0f 0x0122DEE0 (`fmul`, so `*= -1.0f`
  not unary minus), 0.02f 0x01294EA8, 3.0f 0x01294EA4, 0.0f 0x0122DEDC, double 6.2831854820251465 0x0124F2B8 = ?TWO_PI@@3NB
  (= (double)6.2831855f, the GFSin/GFCos scale), 5.0f pushed imm 0x40A00000, 0.05f imm 0x3D4CCCCD, 0x11.

## 2. Identity
- Name: CAIStateGroup_HoverWander::Update, leaf `?Update@CAIStateGroup_HoverWander@@UAEXXZ` (void, no params, public virtual).
  Sources: (a) rebuild/manifest/rtti-vtables.tsv: CAIStateGroup_HoverWander vtable 0x0128C5D4 slot 12 = 0x00959BF0 (xref: only ref is
  .rdata 0x0128C604 = 0x0128C5D4+0x30); (b) CAIStateGroupBase.h slot 12 = `virtual void Update() = 0`; (c) Ego_r.pdb (egor_pdb_names
  0x0097E390) and FableWin (_fablewin_api 0x02C09EB0) both have ?Update@CAIStateGroup_HoverWander@@UAEXXZ. Manifest label CORRECT.
- Class: CAIStateGroup_HoverWander : CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase : ... CAIStateGroupBase; sizeof 0x1C (no own data).

## 3. Callees (landed file / landed name / PDB) -- draft spellings that are WRONG marked [FIX]
- 0x008FCFC0 rebuild/src/compiled/00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp  CAIStateGroupBase::GetWorld() const -> CWorld&
  (?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ, protected). [FIX] draft `GetPWorld()` returning CWorld*.
- 0x0049D810 .../00/49/CWorld_GetWorldSeed_0049d810.cpp  CWorld::GetWorldSeed() const -> unsigned long* (?GetWorldSeed@CWorld@@QBEPAKXZ).
  [FIX] draft `GetRandomSeed()` returning unsigned long&.
- 0x0040F020 .../00/40/CVectorMap_LowerBound_0040f020.cpp  CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (via CThing::GetTC,
  FABLE_RETAIL_CTHING_GETTC_EXPANDED). [FIX] draft's local CInterfaceMap/SEntry/m_presentBits/m_interfaces model.
- 0x004C73D0 .../00/4c/CThing_GetPosition_004c73d0.cpp  CThing::GetPos() const (?GetPos@CThing@@QBEABVC3DVector@@XZ). [FIX] draft `GetPosition`.
- 0x006CB6E0 .../00/6c/CTCCreatureNavigation_GetNearbyWanderPosition_006cb6e0.cpp  ?GetNearbyWanderPosition@CTCCreatureNavigation@@QAE_NABVC3DVector@@MPAV2@_N@Z (OK; flag arg here = true).
- 0x00497890 .../00/49/global_GFROR13_00497890.cpp  GFROR13 (?GFROR13@@YIJK@Z, returns long) -- declared in engine/GFRandom.h; draft returned unsigned long.
- 0x00A14440 .../00/a1/C3DVector_AccurateNormalise_00a14440.cpp  ?AccurateNormalise@C3DVector@@QAEAAV1@XZ (returns C3DVector&). [FIX] draft `Normalise`.
  Opt-in FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE (used by sibling 0x0091DDD0).
- 0x0097D580 .../00/97/CAIStateGroupBase_AIActionLookAtPos_0097d580.cpp  ?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z
  (`ret 18h`; enums EAIActionRepeat/EAIActionReturn, engine/EAIAction.h). Declared in CAIStateGroupBase.h under FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS (line 81).
- vtable slot 22 (+0x58) = 0x0091D7F0 .../00/91/CAIStateGroupHoverMovementBase_AIActionMoveToPosAndFacePos_0091d7f0.cpp
  ?AIAction_MoveToPosAndFacePos@CAIStateGroup_HoverMovementBase@@MAE?AW4EAIActionReturn@@W4EAIActionRepeat@@ABVC3DVector@@1AA_NW4EMovementType@@@Z
  (protected virtual; (EAIActionRepeat, const C3DVector& pos, const C3DVector& facePos, bool&, EMovementType)).
  [FIX] draft calls it AIAction_MoveToPosProximityAndFacePos -- that is the NON-virtual 0x009412b0-in-Ego_r one with an extra float (`M1`).
  NOTE engine/CAIStateGroup_HoverMovementBase.h declares NO members (empty class) -- the virtual must be modelled (sibling 0x00959A60 does it locally).
- 0x008FCF40 .../00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp  ?Terminate@CAIStateGroupBase@@QAEXXZ (OK).
- Inline (no call): GFRandom (?GFRandom@@YIKKPAK@Z, engine/GFRandom.h: `*seed*=0x24a1; *seed+=0x24df; *seed=GFROR13(*seed); if(range) return *seed%range; return 0;`)
  x4: GFRandom(100,seed), GFRandom(2,seed) (`and 1`), GFRandom(100,seed), GFRandom(3, GetWorld().GetWorldSeed())+8.
  GFSin/GFCos (?GFSin@@YINM@Z / ?GFCos@@YINM@Z, engine/GFSin.h / engine/GFCos.h: `sin(angle*(double)6.2831855f)`, needs #pragma intrinsic(sin,cos)).

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*) -- engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS]).
- PBrain+0x18 CAIBrain creature (CThingAICreature) -- engine/CAIBrain.h GetCreature() under FABLE_RETAIL_CAIBRAIN_GETCREATURE;
  CAIStateGroupBase::GetCreature() const -> CThingAICreature& under FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE (inline, ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ).
- creature+0x20 bitset (UsingComponents) bit 7 and +0x44 Components CVectorMap -- engine/CThing.h, FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(TCI_NAVIGATION,&nav)), engine/CVectorMap.h.
- C3DVector X/Y/Z -- engine/C3DVector.h (operator+, operator*(float) const, FABLE_VECTOR_INLINE = __forceinline).
- Draft models LOCALLY (all have shared headers): C3DVector, CInterfaceMap/CVectorMap, CThing, CThingCreatureBase, CTCCreatureNavigation,
  CWorld, CAIBrain, CAIStateGroupBase, CAIStateGroup_HoverMovementBase (header empty -> virtual slot 22 needs a model).

## 5. Callee visibility
- GetWorld / GetWorldSeed / GetPos / AccurateNormalise / LookAtPos / Terminate / GetNearbyWanderPosition / LowerBound / GFROR13: out of line (calls).
- Inline in retail (bodies must be visible): GFRandom (engine/GFRandom.h), GFSin/GFCos (engine/GFSin.h, GFCos.h), CThing::GetTC
  (FABLE_RETAIL_CTHING_GETTC_EXPANDED), CAIStateGroupBase::GetCreature + CAIBrain::GetCreature (both _GETCREATURE macros), C3DVector ops.
- Do NOT use CAIStateGroupBase::GetPWorldSeed (0x008FCFD0, out of line) -- retail calls GetWorld+GetWorldSeed explicitly here.

## 6. Siblings (copy idioms)
- 0x0091DDD0 rebuild/src/compiled/00/91/CAIStateGroupHoverAllAroundTest_Update_0091ddd0.cpp (RELOCATION_MATCH) -- THE closest idiom:
  `C3DVector dir(tp.X-mp.X, tp.Y-mp.Y, 0.0f); dir.AccurateNormalise(); C3DVector perp(dir.Y, -dir.X, 0.0f);`
  `target = start + A*(float)GFSin(frac)*dist + perp*(float)GFCos(frac)*dist;` Macros: FABLE_RETAIL_CTHING, _CINTELLIGENTPOINTER,
  _CAISTATEGROUPBASE, _CAISTATEGROUPBASE_MEMBERS, _GETCREATURE, _AIACTIONS, _SPEECH, _MEALACTIONS, FABLE_RETAIL_CAIBRAIN_GETCREATURE,
  FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE, _CWORLD, _CTHINGPHYSICAL/_CTHINGGAMEOBJECT/_CTHINGCREATUREBASE/_CTHINGAICREATURE; `#pragma intrinsic(fmod, sin, cos)`;
  includes engine/GFSin.h, engine/GFCos.h, engine/EAIAction.h, engine/EMovementType.h.
- 0x00959A60 .../00/95/CAIStateGroupHoverWanderAroundThing_Update_00959a60.cpp (same 3-iteration loop, same slot-22 call, GetTC via
  FABLE_RETAIL_CTHING_GETTC_EXPANDED + CVectorMap.h, Random(3)+8 before the do/while; models HoverMovementBase locally with slot 22).
- 0x0096E830 .../00/96/CAIStateGroupMeleeCombatNearby_IsEnemyCreatureNearCombatPosition_0096e830.cpp (TWO_PI 0x0124F2B8 = ?TWO_PI@@3NB, GFCos).
- 0x00959800 HoverWanderAroundThing::IsRunnable, 0x00959F30 HoverKeepInArea::IsRunnable (`// Flags: none`, no pragmas).
- Neighbour rows use no #pragma optimize; Flags none.

## 7. Draft model errors vs disassembly
1. SWAPPED randoms: retail v1 = GFRandom(100)*0.003f+0.1f, `*= -1.0f` when GFRandom(2)==0 -> stored [esp+18h] is the ANGLE in turns
   (fld [esp+20h] after 8 pushed bytes, `fmul qword TWO_PI`); v2 = GFRandom(100)*0.02f+3.0f [esp+14h] is the DISTANCE (fmul [esp+28h]
   after +0x14). Draft computes angle from the 3..5 value and scales by the ±0.1..0.4 one.
2. Rotation formula wrong. Retail (decoded x87): x = (T.X + dir.Y*s*d) + dir.X*c*d; y = (T.Y + (-dir.X)*s*d) + dir.Y*c*d;
   z = (T.Z + 0.0f*s*d) + dir.Z*c*d, cos term evaluated first (VC right-to-left operator+ arg), `-dir.X` (fchs) computed before the angle.
   => `C3DVector perp(dir.Y, -dir.X, 0.0f); C3DVector look = target + perp*(float)GFSin(v1)*v2 + dir*(float)GFCos(v1)*v2;`
   (exact sibling 0x0091DDD0 form; one `angle*TWO_PI` shared by fcos/fsin).
3. Movement type: retail computes GFRandom(3, GetWorld().GetWorldSeed())+8 ONCE after LookAtPos succeeds, before the do/while (loop
   head 0x00959E87 re-pushes ebx); draft recomputes it inside the loop.
4. Seed: `unsigned long* seed = GetWorld().GetWorldSeed();` first statement; then GFRandom(100,seed) etc. (dead store of the first rotate result is retail's own).
5. dir = C3DVector(T.X-here.X, T.Y-here.Y, 0.0f) with here = GetCreature().GetPos() re-fetched (Y subtracted first = right-to-left ctor args) then AccurateNormalise().
6. Names/types per section 3 (GetWorld, GetWorldSeed, GetPos, AccurateNormalise, AIAction_MoveToPosAndFacePos, EAIActionRepeat/EMovementType enums).

