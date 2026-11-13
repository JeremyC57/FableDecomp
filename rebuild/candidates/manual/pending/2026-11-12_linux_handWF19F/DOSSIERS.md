## 0x00D17A80

# DOSSIER 0x00D17A80 NScript::CQ_BountyHuntScript::WatchForBoasts (785 bytes)

## 1. Retail shape (xdis 00d17a80 0x321)
- __thiscall, no args, plain `ret` (6 return blocks, each `pop edi/esi/ebp/ebx; add esp,1Ch; ret`).
- NO frame pointer (`sub esp,1Ch; push ebx,ebp,esi,edi`), NO SEH prologue despite CCharString/vector locals -> default lane build (no /EHsc); no optimize("s") idioms (uses `add esp,4` after cdecl calls, `lea` alignment pads before loop heads: speed/Ot).
- Frame: boastName CCharString @esp+0x14 (ctor 0x0099E4B0 at entry, lives across whole fn); boasts vector @esp+0x20 (start +0x20, finish +0x24, eos +0x28); allocator dummy @esp+0x13; spawn-name temp @esp+0x18, region "" temp @esp+0x1c; ebx = EH-style "temp constructed" flag bit 1 for the new-expression's name temp (`or ebx,1` .. `test bl,1; and ebx,-2`).
- Control flow: `CCharString s; while(!IsActiveThreadTerminating()) { { vector boasts; Game->MsgOnBoastsMade(boasts); for (i=0; i != size(); ++i) {...} if (IsActiveThreadTerminating()) return; } Game->NewScriptFrame(); }`
  * the vector is destroyed (inline element loop + free, 0x00D17C31..0x00D17C5D) BEFORE `NewScriptFrame` (slot 7 call at 0x00D17C65) -> vector must be in an inner `{}` block that closes before NewScriptFrame (the post-loop terminate test at 0x00D17C1E is inside that block).
  * while loop rotated: entry test at 0x00D17A9A, bottom test 0x00D17C6A `je 0xd17aa7`.
  * for loop: top guard `test ecx,0xFFFFFFF8` (size()!=0), bottom `inc ebp; sar eax,3; cmp ebp,eax; jne` -> `!=` compare (jne, not jb), index in ebp starting 0.
  * element read: `esi = boasts[i].first`, `boastName = boasts[i].second` (operator= 0x0099EFB0) every iteration, BEFORE the 15/16 compare.
  * `if (first==15) { if (!BoastActiveTimeLimit) { if(term) return; BoastActiveTimeLimit=true; spawn WatchForQuestInfoDisplayConditions } }` -- note retail stores `mov byte[edi+78h],1` interleaved after `push 3Ch` (the store happens before operator new: assignment precedes the spawn statement).
    After the spawn (and when BoastActiveTimeLimit was already set, jne 0xd17bcb skips the re-test) flow goes to the timer test.
  * `else if (first==16) { if (!BoastActiveDecapitation) { if(term) return; BoastActiveDecapitation=true; } }`
  * then `if (BoastActiveTimeLimit && BoastTime.GetTime() <= 0 && !MasterScriptData->BountyHuntTimeLimitExceeded) { if(term) return; MasterScriptData->BountyHuntTimeLimitExceeded = true; }` (`jg` on GetTimer result; GetTime is INLINE through the GLOBAL g_pGameScriptInterface 0x0143E8F8 slot 90 +0x168 with TimerIndex +0x80, not through Game).
- Spawn expression: `operator new(0x3C)` -> if non-null: CCharString("WatchForQuestInfoDisplayConditions" 0x012CA7A4, -1) temp, CSpawnedFuncBase(name,0) 0x00CDD450, vtable 0x012C9EDC, Func 0x00D17DA0 @+0x34, PClass=this @+0x38; else esi=0. THEN CCharString("" 0x0122D70E) temp, push it, push esi, AddSpawnedFunction 0x00CB7E50; dtor "" temp, then conditional dtor of name temp. Same expression form as landed Deputy2 Main line 149: `AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("WatchForQuestInfoDisplayConditions"), 0, this, &CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions), CCharString(""));` (without the "ParentClass." concat -- this is the class's own SpawnFunction form).
- THE RESIDUE (6 exits destroy the vector differently):
  | exit | site | element destroy | storage free |
  |---|---|---|---|
  | for-top terminate (1st in source) | 0x00D17C88 | out-of-line `??_G?$pair@JVCCharString@@@std@@` 0x004DA68A with flag 0 (push 0) | out-of-line 0x004D9C5D (~_Vector_base, folded) |
  | time-limit arm terminate | 0x00D17CBE | inline ~CCharString(+4) loop | out-of-line 0x004D9C5D |
  | decap arm terminate | 0x00D17CF9 | inline loop | out-of-line 0x004D9C5D |
  | expiry-flag terminate | 0x00D17D2E | inline loop | inline free (tail shared with next) |
  | post-loop terminate | 0x00D17D51 | inline loop | inline free 0x00BFEA14 |
  | normal end-of-block | 0x00D17C31 | inline loop | inline free |
  Inline-ness grows toward the LATER source sites -> looks like VC7.1 inline-budget exhaustion with sites processed bottom-up; the draft only tried in-body inline_depth pragmas. Untried lever: TU flag sweep `// Flags: /d2inlSNN` (sibling 0x00D18DF0 uses `/d2inlS15` for exactly this kind of per-site inline/out-of-line split), and/or modelling pair's implicit dtor as non-inline only at -- not possible per site; so budget is the realistic lever. Also the out-of-line ??_G(0) means the inner `_Destroy(pair*)` was called as `p->~pair()` through the scalar-deleting dtor (row 0x004DA68A notes: VC7.1 emits ??_G flag 0 for a non-inlined implicit dtor).

## 2. Name / identity (manifest label CORRECT)
- Leaf `?WatchForBoasts@CQ_BountyHuntScript@NScript@@QAEXXZ` -> `public: void __thiscall NScript::CQ_BountyHuntScript::WatchForBoasts(void)`, non-const, void.
- Source 1: Ego_r.pdb (ghidra_out/egor_pdb_names.tsv:38675, @0x00D1F110 in Ego_r build) and FableWin.pdb (ghidra_out/_fablewin_api.tsv:312277 / naming_stragglers FableWin.exe.signatures.tsv:317938 @0x036B1F10).
- Source 2: only xref is retail CQ_BountyHuntScript::Main 0x00D13580 (not landed): at 0x00D138B0 it builds CCharString(0x012CA45C = "WatchForBoasts") and stores Func=0x00D17A80 into a CSpawnedFunc<CQ_BountyHuntScript> (vtable 0x012C9EDC) -> AddSpawnedFunction.
- Not virtual; no vtable slot.

## 3. Callees (landed row | landed name | PDB/real name)
- 0x0099E4B0 CCharString() | 00/99/CCharString_CCharString_0099e4b0.cpp | ??0CCharString@@QAE@XZ -- ok
- 0x00CB7940 | CScriptBase_IsActiveThreadTerminating_00cb7940.cpp | CScriptBase::IsActiveThreadTerminating -- ok (engine/CScriptBase.h:35)
- 0x0074F680 vector ctor | manifest label CIndexBuffer::CIndexBuffer (FOLDED COMDAT, wrong for us) | real: STLport `vector<pair<long,CCharString> >::vector(const allocator&)` (or _Vector_base(const _Alloc&)) emitted out of line: `ret 4`, three zero stores. Note it is CALLED out of line here (lea of dummy allocator esp+0x13 pushed). Draft models it as an out-of-line ctor: right idea. Shared stlport `vector(const _Alloc& a=_Alloc())` is inline -- need a row-side/opt-in out-of-line ctor declaration (no FABLE_RETAIL_STLP_* macro exists for the default vector ctor; only VECTOR_N/FILL/COPY_OUTOFLINE).
- vtable Game slot 34 +0x88 MsgOnBoastsMade(vector<pair<long,CCharString> >&) const 0x008A9AE0 (engine/CGameScriptInterfaceBase.h:57) -- ok
- 0x0099EFB0 | CCharString_operator_0099efb0.cpp | CCharString::operator=(const CCharString&) -- ok
- 0x00BFEA1A | global_operatornew_00bfea1a.cpp | operator new
- 0x0099EBF0 | CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1)
- 0x00CDD450 | CSpawnedFuncBase_CSpawnedFuncBase_00cdd450.cpp | CSpawnedFuncBase(const CCharString&, ulong)
- 0x00CB7E50 | CScriptBase_AddSpawnedFunction_00cb7e50.cpp | CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&)
- 0x0099EAE0 | ~CCharString
- g_pGameScriptInterface 0x0143E8F8 slot 90 +0x168 GetTimer(long) const 0x006E7710 (inline CTimer::GetTime)
- 0x00BFEA14 free (MSVCR71) | global_free_00bfea14.cpp
- 0x004DA68A | 00/4d/stdpairlongCCharString_scalardeletingdestructor_004da68a.cpp | ??_G?$pair@JVCCharString@@@std@@QAEPAXI@Z (optimize("s") TU) -- ok
- 0x004D9C5D | 00/4d/_unattributed_ReleaseFirstMember_004d9c5d_004d9c5d.cpp | landed as DESCRIPTIVE placeholder `CFirstMemberOwner_004d9c5d::ReleaseFirstMember_004d9c5d`; by this call site it is the folded STLport `~_Vector_base<pair<long,CCharString>,allocator<...> >` (free of _M_start). NAME_FIX candidate (one source only: shape + this call site; no PDB hit checked).
- Game slot 7 +0x1c NewScriptFrame() const 0x006E7410 -- ok
- 0x00D17DA0 (address-only) | NScriptCQBountyHuntScript_WatchForQuestInfoDisplayConditions_00d17da0.cpp | ?WatchForQuestInfoDisplayConditions@CQ_BountyHuntScript@NScript@@QAEXXZ; retail string 0x012CA7A4 confirms.
- Draft spelling disagreements: draft calls 0x004D9C5D "base dealloc" (fine), names the +0xDA flag `BoastTimeLimitExpired` (WRONG -- real `BountyHuntTimeLimitExceeded`), names master-data pointer `Data` and type `CQ_BountyHuntMasterData` (WRONG -- `MasterScriptData`, `NScript::CQ_SunnyvaleMasterData`), interface type `CGameScriptInterface` (shared header uses `CGameScriptInterfaceBase* Game`).

## 4. Members
- this+0x40 Game: CLevelScriptBase<CQ_SunnyvaleMasterData>::Game (CGameScriptInterfaceBase*; or reference under FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS) -- engine/CLevelScriptBase.h.
- this+0x44 MasterScriptData (T* = CQ_SunnyvaleMasterData*) -- engine/CLevelScriptBase.h.
- this+0x78 bool BoastActiveTimeLimit, +0x79 bool BoastActiveDecapitation, +0x80 CTimer BoastTime (TimerIndex +0) -- engine/CQ_BountyHuntScript.h:90-93; Ego_r struct_layouts 140/141/148 (-0x14 retail shift). (+0x7c BoastTimerID unused here.)
- MasterScriptData+0xDA bool BountyHuntTimeLimitExceeded -- engine/CQ_SunnyvaleMasterData.h:167; struct_layouts_egor CQ_SunnyvaleMasterData 218.
- Opt-ins: CQ_BountyHuntScript.h offers FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_{GANGDEPUTY1,GANGDEPUTY2,HOSTAGE1,HOSTAGE2,GANGLEADER}; NONE declares WatchForBoasts or WatchForQuestInfoDisplayConditions -> the shared header needs a new opt-in (e.g. FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS) declaring both (coordinator to add). Needs FABLE_RETAIL_NSCRIPT (CLevelScriptBase/CTimer/CScriptBase #error without it), FABLE_RETAIL_SUSPENDABLE_PROCESS for engine/CSpawnedFunc.h (__forceinline ctor, Func +0x34, PClass +0x38), CTimer::GetTime defined inline after the include (CTimer.h wave-8 convention: `inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }`).
- Classes the draft models LOCALLY that have shared headers: CCharString (fable_string.h / FABLE_RETAIL_CCHARSTRING), std::pair/vector (stlport/fable_containers.h), CGameScriptInterface (engine/CGameScriptInterface.h / CGameScriptInterfaceBase.h), CTimer (engine/CTimer.h), CSpawnedFuncBase/CSpawnedFunc (engine/CSpawnedFuncBase.h, CSpawnedFunc.h), CScriptBase (engine/CScriptBase.h), CQ_BountyHuntScript (engine/CQ_BountyHuntScript.h), master data (engine/CQ_SunnyvaleMasterData.h).

## 5. Callee visibility
- Vector ctor 0x0074F680: called out of line (COMDAT, aligned) -> declare without body.
- ~vector: retail mixes; at the fully-inline sites the body is STLport `_Destroy(start,finish)` (inline __destroy_aux loop calling ~CCharString(+4)) + `_Vector_base` dtor (if start free). The pair dtor and ~_Vector_base bodies were visible (inlined at later sites) but emitted out of line at earlier ones (0x004DA68A / 0x004D9C5D are UNALIGNED addresses = COMDAT copies from other TUs, folded) -> budget effect, not invisibility.
- Relevant stlport macros: FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE / _DESTROY_AUX_FASTCALL_DECL (wrong: retail has no __destroy_aux call here), nothing for _Vector_base dtor out-of-line or default ctor out-of-line. ~vector at 0x004E34B2 (used out of line by landed CHeroBoastsScript::Main 0x00CE1AF0) is NOT called here.
- WatchForQuestInfoDisplayConditions 0x00D17DA0 (next function, same TU) only address-taken.
- Same-TU neighbours (retail order): WatchForAllGruntsKilled 0x00D17340, SpawnBanditGangGrunts 0x00D17470, [0x00D179B0 CBH_GangLeader::OnPersist], WatchForBoasts 0x00D17A80, WatchForQuestInfoDisplayConditions 0x00D17DA0, RemoveBoastQuestInfo 0x00D17E70, AddBoastQuestInfo 0x00D17EA0, CBH_GangDeputy1::Main 0x00D17F20.

## 6. Sibling landed rows
- rebuild/src/compiled/00/d1/NScriptCQBountyHuntScriptCBHGangDeputy2_Main_00d18df0.cpp: `// Flags: /d2inlS15` (inline-budget for per-site dtor inline split), defines FABLE_RETAIL_CCHARSTRING, _NSCRIPT, _SUSPENDABLE_PROCESS, _CLEVELSCRIPTBASE, _STLP_VECTOR_PUSHBACK_OUTOFLINE, _CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2; includes engine/CQ_BountyHuntScript.h, CSpawnedFuncBase.h, CSpawnedFunc.h; spawn idiom line 149.
- rebuild/src/compiled/00/d1/NScriptCQBountyHuntScript_SpawnBanditGangGrunts_00d17470.cpp: FABLE_RETAIL_NSCRIPT + FABLE_RETAIL_CCHARSTRING, shared CQ_BountyHuntScript.h, local std::vector, `#pragma auto_inline(off/on)` around one template specialization; no Flags.
- rebuild/src/compiled/00/d1/CQBountyHuntScript_WatchForAllGruntsKilled_00d17340.cpp: local vector whose dtor is expanded at 4 returns (notes: plain `inline` dtor got emitted out of line and called once; __forceinline expanded all).
- 0x00D17DA0 / 0x00D17E70 / 0x00D17EA0 rows: older local class models (CScriptBase-derived local CQ_BountyHuntScript), FABLE_RETAIL_NSCRIPT, no flags.
- rebuild/src/compiled/00/ce/NScriptCHeroBoastsScript_Main_00ce1af0.cpp: only other MsgOnBoastsMade caller landed; boast vector in its own `{}` block destroyed before the frame call; local vector model with out-of-line ~vector 0x004E34B2 and out-of-line _STLP_alloc_proxy ctor 0x0074F4B0.

## 7. Old draft model errors
- Vector scope: draft keeps `boasts` alive across `Game->NewScriptFrame()`; retail destroys it first -> put `{ vector; ...; if(term) return; }` then NewScriptFrame (draft notes claim body matched to post-loop test, so check this ordering in the differ).
- Wrong member names/types (Data/BoastTimeLimitExpired/CQ_BountyHuntMasterData; local CGameScriptInterface with padded vslots) -- use shared headers.
- Local vector ~BoastVector modelled as one inline dtor with custom _Destroy -> cannot produce the ??_G(0) out-of-line call nor the out-of-line 0x004D9C5D; need STLport-shaped vector (vector : _Vector_base, ~vector = _Destroy(range); ~_Vector_base frees) so the two levels can independently inline / not inline, then tune budget (/d2inlSNN sweep).
- `free` vs deallocate: inline sites call free 0x00BFEA14 directly (node allocator = malloc/free).
- Everything else (15/16 arms, re-tests, GetTime via global, spawn temp ordering) matches the disassembly.

## 8. Scout addendum (2026-11-13, re-verified with xdis/xref)
- Re-disassembled 0x00D17A80..0x00D17D90: section 1-7 above confirmed (no frame ptr, no SEH, 6 `ret` exits, `!=` for loop, inner-block vector destroyed before Game slot 7).
- Xrefs: 0x00D17A80 only `abs` ref at 0x00D138DB (Main 0x00D13580, not landed). 0x0074F680 (vector ctor, mislabeled CIndexBuffer) has ONE caller: 0x00D17AB0. 0x004D9C5D (folded ~_Vector_base, landed as `_unattributed ReleaseFirstMember_004d9c5d`) has exactly THREE callers, all in this function (0x00D17CA8/CE3/D18). So both are COMDATs kept alive only by this function.
- ??_G pair<long,CCharString> 0x004DA68A callers: 0x004DBED4 (tiny `push 0; call; ret` thunk), 0x004DD782 __destroy_aux<pair<long,CCharString>*> (landed std_destroyauxstdpairlongCCharString_004dd782.cpp, optimize("s"), __fastcall, ret 4), and 0x00D17C98 (this function, for-top exit).
- The for-top exit loop at 0x00D17C88..0x00D17CA2 (`push 0; mov ecx,esi; call 0x4DA68A; add esi,8; cmp esi,edi; jne`) is byte-for-byte the BODY of __destroy_aux 0x004DD782 inlined: i.e. at that site ~vector -> _Destroy(range) -> __destroy_aux -> _Destroy(T*) all inlined, only the implicit ~pair (as ??_G flag 0) and ~_Vector_base 0x004D9C5D called.
- Reference shape: out-of-line ~vector<pair<long,CCharString> > 0x004E34B2 (landed stdvector..._vector_004e34b2.cpp) = call __destroy_aux 0x004DD782 (tag at `lea [esp+7]`) + inline `if(start) free(start)` -- NOT called here, but shows the TU that owns these COMDATs is a size-optimised STLport-instantiating TU.
- Shared stlport (rebuild/include/stlport/fable_containers.h): `_Vector_base::~_Vector_base` (l.378) and `vector(const _Alloc&)` are plain inline; __destroy_aux has opt-ins FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL (declaration only, out-of-line call -- WRONG here, no __destroy_aux call in retail), FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE (WRONG here). No opt-in exists for an out-of-line default vector ctor or out-of-line ~_Vector_base; the differ source declares the ctor as an explicit specialization (row-side), which is acceptable; ~_Vector_base per-site mix cannot come from a declaration (some sites inline it).
- Net: the residue is purely the inliner's per-site budget order (reverse of ours). Everything else (names, members, callees, flow) is solved in $W/differ/00d17a80.cpp (705v785, 4 hunks). Recommend parking as DIFFER unless the author has a new budget-order lever (real same-TU Main 0x00D13580 reconstruction).

