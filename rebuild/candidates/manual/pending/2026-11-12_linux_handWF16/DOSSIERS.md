## 0x007E4E30

# DOSSIER 0x007E4E30 CTCBetting::FrameUpdate (645 bytes)

## 1. Retail shape (xdis 007e4e30 0x295)
- thiscall, plain `ret` (no args), NO frame pointer (`sub esp,0x40` first; /Oy on -> no optimize("y",off)). No SEH frame.
- /GS: cookie 0x0139C8A8 loaded at entry into [esp+0x40]; __security_check_cookie 0x00BFE9F9 on both exits. Frame 0x40.
- `edi`=this throughout. Early out `if (Active == g_bettingActive) return;` taken BEFORE ebx/ebp/esi are pushed (shrink-wrapped pushes).
- Speed idioms (/O2-ish): `lea esp,[esp]` loop-align pad at 0x7E502C; constant 1 CSE'd into ebp (`mov ebp,1` on both else-paths; `push ebp` = AddData size 1; `cmp [ecx+4],ebp` = GetRefCount()==1). 0 materialised by `xor ebx,ebx` only in the loops (and at 0x7E5067 when the first vector is empty). Not a size-optimised TU.
- True branch: def slot [E+0x14] zeroed (`xor eax,eax; mov g_bet,eax; mov [esp+14],eax`) -- the g_bet=0 store and def ctor share eax. After GetSubDef the def ptr goes to esi and the L"" CWideString is built INTO THE DEAD DEF SLOT E+0x14 before any SetText argument push. Font (MGC+0x1614C object word) in ebp and CPlayerGui::PClass in ebx are loaded before the pushes.
- SetText args (pushed right-to-left): 1,0,0(0.0f),0x404, GetText(Cancel +0x54)->E+0x1C, GetText(Continue +0x50)->E+0x20, &empty(E+0x14), GetText(Question +0x4C)->E+0x18, font; this = PClass+0x18. Dtors after: E+0x18,E+0x20,E+0x1C, then E+0x14 (empty last => constructed first: either a named local `CWideString empty(L"")` in a block, or ordering as retail shows).
- CGameEvent at E+0x24 (Type 9, Player -1 @+0x28, EndPos 0 @+0x49, Valid 1, Replacement 0); the bool byte at E+0x10, `AddData(1,&b)` 0x009F1760; then AddGameEvent(MGC) 0x0041649C.
- FrameToStart (+0x34) = GetCurrentServerFrame() + [0x01384050]*ConstantFPS: ConstantFPS(0x01375550) loaded into ebx BEFORE the call, `imul ecx(delay),ebx`, add -> store. Computed on BOTH paths (GetSubDef success and failure) -> it is after the `if (GetSubDef)` block, inside if(Active).
- The def is released AFTER the FrameToStart store, with a null test (`test esi,esi; dec [esi+4]; call [vtbl+4]`) -> CDefPointer<const CBettingDef> def declared at the top of the if(Active) block; its dtor runs at block end. Retail duplicates the epilogue there (tail).
- Else branch: `if (player.GetCurrentMode()==0x27) { RemoveMode(0x27 /*eax reused*/); event(9).AddData(1,&false); AddGameEvent; }` then two loops over BetHandles(+0x10..+0x14) and MoneyHandles(+0x1C..+0x20), stride 8, `end` reloaded each iteration, loop guard `first!=last` tested before entry (for-loop). Body = CEnginePrimitiveHandle::Clear(): `if (p && p->count==1) p->NotifyReleased(*this) /*slot3 +0xC*/; if (p){ if(--p->count==0) p->Delete() /*slot1 +4*/; p=0;} p=0;` -- exactly the double `mov [esi+4],ebx` of Clear() + CIVCountedPointer::Reset(0).
- Mode 0x27 = 39 (EPlayerMode betting). True branch: `if (GetCurrentMode()!=0x27) AddMode(0x27,false)` (push 0, push 0x27).

## 2. Name / signature (PROVEN)
- Retail vtable CTCBetting 0x01270624 slot 9 = 0x007E4E30 (rebuild/manifest/rtti-vtables.tsv; slot 8 = HasFrameUpdate 0x007E5910 landed). xref: only ref is the vtable word 0x01270648.
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x007e0220) and FableWin (_fablewin_api.tsv 0x017fd57d / 0x02799960): `?FrameUpdate@CTCBetting@@UAEXXZ`. Manifest label is RIGHT.
- Signature: `virtual void CTCBetting::FrameUpdate();` (public virtual, U). Leaf: ?FrameUpdate@CTCBetting@@UAEXXZ (draft wrote bare "FrameUpdate").
- CTCBetting.h has FABLE_RETAIL_CTCBETTING_VSLOTS (only HasFrameUpdate declared) -> add FrameUpdate there/declare (slot 9) or rely on CTCBase's virtual FrameUpdate.

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | PDB spelling | draft disagreement |
|---|---|---|---|
| 0x00686D40 | 00/68/CTCBase_GetPlayerManager_00686d40.cpp | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ (returns REF, protected) | draft: CTCBetting::GetPlayerManager returning pointer |
| 0x00449970 | 00/44/CPlayerManager_GetMainPlayer_00449970.cpp | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (CPlayer&) | draft returns pointer |
| 0x00633BE0 | 00/63/CPlayer_GetCurrentMode_00633be0.cpp | ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ | draft returns long |
| 0x006345C0 | 00/63/CPlayer_AddMode_006345c0.cpp | ?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z (mode, bool) | draft (long,long) |
| 0x00634190 | 00/63/CPlayer_RemoveMode_00634190.cpp | ?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z | draft long |
| 0x007E5820 | 00/7e/CDefClassBase_GetSubDefCBettingDef_007e5820.cpp | ??$GetSubDef@VCBettingDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCBettingDef@@@@@Z (const, CDefPointer<const CBettingDef>*) | draft non-const, non-const T, stand-in body |
| 0x0099B6B0 | 00/99/CWideString_CWideString_0099b6b0.cpp | CWideString(const wchar_t*) ; L"" @ 0x0122D70C | ok |
| 0x0099B510 | 00/99/CWideString_Destructor_0099b510.cpp | ??1CWideString@@QAE@XZ | ok |
| 0x007E4D70 | 00/7e/CTCBetting_GetText_007e4d70.cpp | ?GetText@CTCBetting@@QAE?AVCWideString@@K@Z (unsigned long) | draft `long id` |
| 0x00492F69 | 00/49/CSubtitleRenderer_SetText_00492f69.cpp | ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@111KM_N2@Z (const CFontBank&, 4x const CWideString&, unsigned long, float, bool click_past, bool pause_game) | draft passes CFontBank* |
| 0x009F1760 | 00/9f/CGameEvent_AddData_009f1760.cpp | ?AddData@CGameEvent@@QAEXJPBX@Z (long, const void*) | draft: CMemoryBuffer::Copy + AddBOOL (no AddBOOL in any PDB; inline PDB adder is AddUBYTE(unsigned char)) |
| 0x0041649C | 00/41/CMainGameComponent_AddGameEvent_0041649c.cpp | ?AddGameEvent@CMainGameComponent@@QAEXABVCGameEvent@@@Z | ok |
| 0x00416225 | 00/41/CWorld_DrawGetEnvironment_00416225.cpp (row named T::Get, mislabeled) | ?GetCurrentServerFrame@CMainGameComponent@@QBEJXZ (`mov eax,[ecx+48h]`, Ego_r CurrentServerFrame +0x48) | draft: GetFrame() non-const |
| 0x00BFE9F9 | 00/bf/global_securitycheckcookie_00bfe9f9.cpp | __security_check_cookie (/GS) | - |
| vslot 1 (+4) on def | CDefPointer release -> Delete | | |
| vslots 3 (+0xC) / 1 (+4) on CEnginePersistentPrimitiveBase | NotifyReleased(handle) / Delete | via CEnginePrimitiveHandle::Clear() | draft local RemoveHandle/Destroy |
Globals: 0x013B86A0 CMainGameComponent::PMainGameComponent (Get()); 0x01375550 CMainGameComponent::ConstantFPS (GetConstantFPS()); 0x013B8790 CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass (CPlayerGui::Get()); 0x0139C8A8 __security_cookie.
Betting globals (no PDB name found): 0x013BAE1D bool -- landed spellings CONFLICT: g_bettingActive (CGameScriptInterface_SetBettingActive_0088fb40, GetBetText_007e50c0), g_BettingActive (ProcessInput_007e5230), g_bBettingInputEnabled (DrawPreparePrimitivesForRendering_007e5680, CInputProcessBetting). Use g_bettingActive (script-interface accessor rows bind it; Ego_r ?SetBettingActive/GetBettingActive@CGameScriptInterface). 0x013BAE20 long: g_bettingAmount (GetBettingAmount_0088fb90, GetBetText) vs g_CurrentBet (ProcessInput) -> use g_bettingAmount. 0x01384050: initialised .data long = 1 (next to ProcessInput's g_BetRepeatDelay..g_BetMultiplierRate 0x01384040..4C), unnamed anywhere; descriptive name needed (e.g. g_BettingStartDelaySeconds), mark descriptive. Must be a non-const extern/global (retail loads it from memory).
NAME_FIXES candidates: 0x00416225 row named T::Get (should be CMainGameComponent::GetCurrentServerFrame); 0x007E50C0 landed "GetBetText" but Ego_r has ?GetBet@CTCBetting@@QAE?AVCWideString@@XZ (QAE, CWideString, no args -- same shape) -> likely CTCBetting::GetBet.

## 4. Members
- CTCBetting (rebuild/include/engine/CTCBetting.h, opt-in FABLE_RETAIL_CTCBETTING, sizeof 0x3C; base CTCBase -> ParentThing +4): Active bool +0x0C, BetHandles std::vector<CEnginePrimitiveHandle> +0x10, MoneyHandles +0x1C (12-byte vectors), FrameToStart long +0x34 (all private; Ego_r struct_layouts: Active@12, BetHandles@16, MoneyHandles@32 with 16-byte vectors -- retail is 12-byte). Draft models CTCBetting LOCALLY (and names layout fine).
- CThing +0x70 PDef/PBaseDef (CDefClassBase*); CTCTavernGame uses `ParentThing->PeekBaseDef().GetSubDef(def)` (CTCTavernGame.h:128) -- copy that idiom (FABLE_RETAIL_CTHING).
- CBettingDef (engine/CBettingDef.h, FABLE_RETAIL_CBETTINGDEF): Question +0x4C, Continue +0x50, Cancel +0x54 (unsigned long). Draft names InstructionText/Button2Text/Button3Text are WRONG.
- CMainGameComponent.h: CurrentServerFrame +0x48 (GetCurrentServerFrame out of line via FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE -- MUST be out of line, retail calls 0x00416225); PCutSceneMainFont +0x1614C CCountedPointer<CFontBank> (FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT -> PeekMainFont(), or FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK -> PeekCutSceneMainFont(), returns const CFontBank&); ConstantFPS via GetConstantFPS(); FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT.
- CPlayerGui.h (FABLE_RETAIL_CPLAYERGUI): SubtitleRenderer +0x18, GetSubtitleRenderer() inline, Get() via CTBaseSingletonNoDefaultAlloc. CSubtitleRenderer.h: FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT declares the 0x00492F69 overload.
- CGameEvent.h: FABLE_RETAIL_CGAMEEVENT_INLINE (inline ctor in member order + AddData decl + AddUBYTE/AddFloat inline). 
- CEnginePrimitiveHandle.h: FABLE_RETAIL_CENGINEPRIMITIVEHANDLE (+FABLE_RETAIL_CIVCOUNTEDPOINTER) gives the retail Clear() (notify at count 1, Reset(0)) -- matches the loop body.
- CDefPointer.h (Release on dtor). CPlayer.h: FABLE_RETAIL_PLAYER (GetCurrentMode/AddMode), FABLE_RETAIL_PLAYER_REMOVEMODE. CPlayerManager.h FABLE_RETAIL_CPLAYERMANAGER. CTCBase.h FABLE_RETAIL_CTCBASE (GetPlayerManager).
- Locally modelled in draft though shared headers exist: CWideString (fable_string.h), CSubtitleRenderer, CPlayerGui, CMemoryBuffer/CGameEvent, CMainGameComponent, CPlayer, CPlayerManager, CBettingDef, CDefPointer, CDefClassBase, CThing, CEnginePrimitiveHandle (+CEnginePrimitive), CTCBetting.

## 5. Callee visibility
- GetSubDef<CBettingDef> 0x007E5820 sits right after this TU's functions (0x007E4D70 GetText, 0x007E4E30, 0x007E50C0, 0x007E51D0, 0x007E5230, 0x007E5570, 0x007E5680, 0x007E57C0 ctor, 0x007E5820 GetSubDef, 0x007E58E0..) -> same TU, template instantiated there and its body visible (out of line). GetBetText landed only after putting the REAL body in the TU (noinline); TavernGame FrameUpdate defines `template<> bool CDefClassBase::GetSubDef<CTavernGameDef>(...) const` with the real body -- do the same for CBettingDef (body in 00/7e/CDefClassBase_GetSubDefCBettingDef_007e5820.cpp).
- GetText 0x007E4D70 is defined EARLIER in the same TU (visible, out of line, has a function-local static so not inlined).
- 0x00416225 GetCurrentServerFrame and 0x00492F69 SetText are out-of-line COMDATs (odd addresses) -- declare only.
- Inline in retail (expanded in place): CMainGameComponent::Get/GetConstantFPS/PeekMainFont, CPlayerGui::Get/GetSubtitleRenderer, CGameEvent ctor + AddUBYTE, CEnginePrimitiveHandle::Clear, vector begin/end, CDefPointer dtor, CThing::PeekBaseDef.

## 6. Siblings
- 00/5e/CTCTavernGame_FrameUpdate_005eb9e0.cpp: closest analogue (GetPlayerManager().GetMainPlayer(), GetCurrentMode()/AddMode((EPlayerMode)26,false), GetSubDef<T> real body in TU, CPlayerGui::Get().GetSubtitleRenderer()); defines FABLE_RETAIL_CCHARSTRING, CCLASS, DEF_CLASS_BASE, CDEFSTRINGTABLE, CRESOURCELIST, CDEFINITIONMANAGER, CTCBASE, CTHING, CINTELLIGENTPOINTER, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CTHINGAICREATURE, CPLAYERGUI, CSUBTITLERENDERER, CPLAYERMANAGER, PLAYER, CGAMECOMPONENT, CMAINGAMECOMPONENT, SCOPED_PTR.
- 00/7e/CTCBetting_GetBetText_007e50c0.cpp (local model; real GetSubDef body in TU with auto_inline(off) + noinline; no pragmas otherwise).
- 00/7e/CTCBetting_GetPlayersMoney_007e5570.cpp / CTCBetting_HasFrameUpdate_007e5910.cpp: header style with FABLE_RETAIL_CCHARSTRING, CWIDESTRING, CCLASS, C3DVECTOR, CTHING, CTCBASE, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CINTELLIGENTPOINTER, CTCHEROSTATS (+ CTCBETTING_VSLOTS for HasFrameUpdate).
- 00/7e/CTCBetting_ProcessInput_007e5230.cpp (local model, auto_inline(off) stand-in GetSubDef; same globals).
- No sibling uses optimize pragmas except the scalar-deleting dtor (`optimize("t",on)`). Default flags + /GS.

## 7. Old-draft model errors (vs disassembly)
- Residue was register allocation: zero cached in ebx for the whole function. Retail materialises 0 only via `xor eax,eax` in the true branch and `xor ebx,ebx` in the loop body/empty-vector path; ebx/ebp hold PlayerGui PClass/font across the GetText calls. Likely cause: the draft's local CEnginePrimitiveHandle::Release() and stand-in GetSubDef (pointer-escape behaviour differs) and pointer-typed GetPlayerManager/GetMainPlayer; use the real header Clear() (Data.Reset(0)) and the real GetSubDef body.
- Empty string: retail constructs L"" FIRST (into the dead def slot E+0x14) and destroys it LAST; the draft's implicit temp would be built in arg order (after Cancel/Continue). Try a named `CWideString empty(L"")` (or block-scoped) before the SetText call, as in the landed 0x00492F69 note.
- Font arg: retail passes the font object word (const CFontBank&: PeekMainFont / PeekCutSceneMainFont), draft passes a raw CFontBank* member (+0x1614C) -- same bytes probably, but use the header accessor.
- Event: use CGameEvent(9) + AddUBYTE(1)/AddUBYTE(0) (FABLE_RETAIL_CGAMEEVENT_INLINE), not CMemoryBuffer::Copy/AddBOOL.
- Loops: `for (iterator it = BetHandles.begin(); it != BetHandles.end(); ++it) it->Clear();` -- end reloaded each pass (matches). Retail tests ebp(=1) == count; ebp loaded `mov ebp,1` before the loops on both paths.
- Wrong names: CBettingDef fields (Question/Continue/Cancel), GetText param unsigned long, GetFrame -> GetCurrentServerFrame, Copy -> AddData, g_CurrentBet/g_BettingActive -> g_bettingAmount/g_bettingActive, g_framesPerSecond -> CMainGameComponent::GetConstantFPS().

## 0x007E8590

# DOSSIER 0x007E8590 CTCOwnedEntity::SetPOwner (650 bytes)

## 1. Retail shape (xdis 007e8590 0x29a)
- __thiscall, `ret 4` (one arg), returns void. NO frame pointer (`sub esp,0Ch; push ebx/ebp/esi/edi`), NO SEH (no fs:0) -> `// Flags: none` (no /EHsc), speed-optimised (no "s"); same as the 0x007E83xx-0x007E8820 CTCOwnedEntity siblings (none use optimize("s") or "y").
- Locals: [esp+10h] = conditional-temp destructor flag (init 0 in prologue, set 1 when the "Dress" CCharString temp is built in the && chain, tested at 0x7E87CE); [esp+14h/18h] shared by the first lookup key, the message temp, the CIntelligentPointer temp; [esp+20h] = the arg slot, re-used later as the 2nd lookup key and the CCharString temps.
- `mov bl,2` at 0x7E85AD (before the first je) = UsingComponents bit mask for TCI_THING_OWNER (0x21 -> word 1 bit 1 -> `test byte ptr [eax+24h],bl`); bl is reused for the 2nd lookup at 0x7E86AC. 0x21 is an immediate key store (`mov [esp+..],21h`, `cmp [eax],21h`).
- Uninitialised local: when the owner has no ThingOwner TC, first block uses `mov eax,[esp+20h]` (undefined value) -> source ignored GetTC's bool and called through the out-pointer unconditionally.
- Strings: 0x0123A5BC "CTCThingOwner", 0x0125D780 "Dress", 0x0123C010 "CTCSwitchableNavigation", 0x0129AAF4 "" (CCharString null-data literal).

## 2. Identity (proven, 2 sources)
- Ego_r.pdb (egor_pdb_names.tsv 0x00738B30) and FableWin.pdb (_fablewin_api.tsv 0x0258A350): `?SetPOwner@CTCOwnedEntity@@QAEXPAVCThing@@@Z` -> `public: void CTCOwnedEntity::SetPOwner(CThing*)` (non-const, void).
- fp_fable.tsv: 7e8590 matched to that mangled name (score 0.899, true). engine_api.tsv: 007e8590 __thiscall void (CTCOwnedEntity*, CThing*).
- Callers (xref, 13): InitialActivate 0x007E8820 (landed), CTCBuyableHouse::CreateRentMoney 0x006C0C90, CTCMeshAutomaticEntityCreator::AddOwnedEntityTC 0x0072DD60, CTCTavernBar::OnInitialActivate 0x007BA550, CTCTavernGame::CreateGamePiece 0x005EA9B0, CAIStateGroupServeFood::MakeFood 0x00923510 ... all call it as SetPOwner(CThing*). Manifest label is CORRECT.
- Declared already in rebuild/include/engine/CTCOwnedEntity.h: `void SetPOwner(CThing*);`

## 3. Callees (all landed)
| addr | landed file | landed/PDB name | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50 | CBaseIntelligentPointer::GetPItem() const (via CIntelligentPointer<T>::Get()) | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020 | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) (non-const) | draft local CInterfaceMap::LowerBound(const long*) + own noinline body -- WRONG, use shared CVectorMap.h |
| 0x0071B1B0 | 00/71/CTCThingOwner_RemoveOwnedObject_0071b1b0 | ?RemoveOwnedObject@CTCThingOwner@@AAEXAAVCThing@@@Z (PRIVATE, takes CThing&) | draft "RemoveDependentThing(CThing*)" -- WRONG name and type |
| 0x0071B300 | 00/71/CTCThingOwner_AddOwnedObject_0071b300 | ?AddOwnedObject@CTCThingOwner@@AAEXAAVCThing@@@Z (PRIVATE, CThing&) | draft "AddDependentThing(CThing*)" -- WRONG |
| 0x0099A2D0 / 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_* | CBaseClass ctor/dtor (message base) | ok |
| (inline ctor, vtable 0x01270BF0) | out-of-line copy 00/7e/CMessageOnSetThingOwner_CMessageOnSetThingOwner_007e8310 | NTCMessages::CMessageOnSetThingOwner (RTTI rtti-vtables.tsv 01270bf0; Ego_r struct: Owner CThing* +4; FableWin ?PeekOwner@CMessageOnSetThingOwner@NTCMessages@@QBEPAVCThing@@XZ) | draft "CMessageNewOwner" -- WRONG name |
| 0x004C7820 | 00/4c/CThing_SendMessageW_004c7820 | ?SendMessageW@CThing@@QAEXABVCMessageBase@NTCMessages@@@Z (const CMessageBase&) | draft took const CBaseClass& -- base class should be NTCMessages::CMessageBase |
| 0x004C9840 | 00/4c/CThing_RemoveTC_004c9840 | ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z, arg TCI_SWITCHABLE_NAVIGATION (121=0x79) | draft RemoveTC(long) 0x79 literal |
| 0x00A01B10 / 0x00A01C10 / 0x00A01B90 | 00/a0/CBaseIntelligentPointer_* | ctor / dtor / SetPItem; temp vtable 0x01238C6C = CIntelligentPointer<CThing> | ok |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0 / CCharString_CCharString_0099eae0 | CCharString(const char*, long=-1) / ~CCharString | ok |
| 0x004C9D60 | 00/4c/CThing_AddTC_004c9d60 | ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z -> AddTC(name, false, 0) | draft (name,long,long) -- types wrong (bool, const CTCParamsBase*) |
| 0x004C8CF0 | 00/4c/CThing_SetInLimbo_004c8cf0 | ?SetInLimbo@CThing@@QAEX_N@Z | ok |
| vslot +0x6C on owner | 00/4c/CThing_DrawIsDrawable_004c7790 | CThing slot 27 `bool DrawIsDrawable() const` | draft "GetPlayerOwnership" -- WRONG |
| vslot +0x70 on ParentThing | 0x004C8E50 | CThing slot 28 `void SetAsDrawable(bool)` | draft "SetPlayerOwnership(long)" -- WRONG |
| 0x00F3FB90 | 00/f3/global_strnicmp_00f3fb90 | _strnicmp (cdecl, add esp,0Ch) | inline of FableWin-only ?StartsWithCaseInsensitive@CCharString@@QBE_NABV1@@Z (0x0181F83A/0x01CF4EC0; no Ego_r copy -> inline). Draft "BeginsWithNoCase" -- WRONG name |

## 4. Members
- this (CTCOwnedEntity, shared engine/CTCOwnedEntity.h, auto-defines FABLE_RETAIL_CTCBASE + FABLE_RETAIL_CINTELLIGENTPOINTER): ParentThing +0x04 (CTCBase.h; GetParentThing() inline returns CThing&), SwitchableNavigationTCAdded bool +0x0C, POwner CIntelligentPointer<CThing> +0x0D (packed), OwnerUID unsigned __int64 +0x18 (written as two dwords from owner UID). Members are private: SetPOwner is a member, fine. GetPOwner() inline = POwner.Get().
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING; CClass via FABLE_RETAIL_CCLASS):
  - +0x10 CClass::Type (unsigned char; GetType()==5, `cmp byte ptr [ecx+10h],5`) on ParentThing.
  - +0x18 UniqueID (unsigned __int64) of owner -> OwnerUID.
  - +0x20 UsingComponents std::bitset<274>; HasTC(type). Tested: owner TCI_THING_OWNER 33 (byte +0x24 mask 2); ParentThing `test dword [eax+2Ch],2000000h` = bit 121 TCI_SWITCHABLE_NAVIGATION (must be NOT present); owner `test dword [eax+38h],200000h` = bit 213 TCI_BUYABLE_HOUSE (must be present). The draft's "Flags2C"/"Flags38" CThingFlags cast is a model error: these are HasTC() tests.
  - +0x44 Components CVectorMap (LowerBound + end at +0x48).
  - +0x78 ScriptName CCharString (GetScriptName() under FABLE_RETAIL_CTHING_GETSCRIPTNAME; data ptr +0,len +4 of CCharStringData via fable_string.h FABLE_RETAIL_CCHARSTRING).
  - +0x91 bit0 Dead (POwner liveness: `test byte [eax+91h],1`), bit5 InLimbo (`movzx; shr al,5; and eax,0FFFFFF01h` -> bool arg to SetInLimbo; IsInLimbo() exists).
- Opt-ins: FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(id,&ptc) form: key local in test, map pointer local, conditional end-or-greater -- exactly retail's shape here), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (HasTC in place), FABLE_RETAIL_CTHING_DRAWABLE (+ needs FABLE_RETAIL_CTHING_ISPLACED) for slots 27/28, FABLE_RETAIL_CTHING_GETSCRIPTNAME, FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (operator= as GetPItem then SetPItem -- retail 0x7E8655..0x7E8661), FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CCLASS, FABLE_RETAIL_CTCTHINGOWNER (StatGetInterfaceType).
- MISSING in shared headers (author must add as opt-ins): CTCThingOwner.h has no AddOwnedObject/RemoveOwnedObject decls (private in PDB -> needs `friend class CTCOwnedEntity` or public decl in an opt-in); no NTCMessages::CMessageOnSetThingOwner header (CTCBase.h only forward-declares NTCMessages::CMessageBase); fable_string.h has no StartsWithCaseInsensitive.
- Classes the old draft models LOCALLY that have shared headers: CCharString (fable_string.h), CBaseClass (engine/CBaseClass.h), CVectorMap/"CInterfaceMap" (engine/CVectorMap.h), bitset (stlport), CTCThingOwner (engine/CTCThingOwner.h), CThing (engine/CThing.h), CBaseIntelligentPointer/CIntelligentPointer (engine/CIntelligentPointer.h), CTCOwnedEntity (engine/CTCOwnedEntity.h).

## 5. Callee visibility
- CVectorMap::LowerBound 0x0040F020: out of line in retail everywhere (declared in CVectorMap.h) -- do NOT define it in the TU (draft defined a noinline body: wrong).
- CMessageOnSetThingOwner ctor: out-of-line COMDAT exists at 0x007E8310 (27 bytes, earlier in the same TU neighbourhood) but retail INLINES it here (CBaseClass() call then `mov [esp+18h],01270BF0h ; mov [esp+1Ch],ebp`) -> inline ctor body; virtual dtor of the temp inlined to ~CBaseClass 0x0099A2E0.
- CIntelligentPointer<CThing>(T*) ctor inline (base ctor 0x00A01B10, vptr store, SetPItem); operator= inline (ASSIGN form); temp dtor -> 0x00A01C10 directly.
- StartsWithCaseInsensitive inline (_strnicmp with PeekData()/""-fallback and length / 0).
- GetTC<T>/HasTC: __forceinline per owner decision 2026-11-01 (CThing.h).

## 6. Sibling landed rows
- 00/7e/CTCOwnedEntity_InitialActivate_007e8820 (caller, same TU; local pack(1) model, no pragmas/flags), _OnForceRecreateUID_007e8370 (FABLE_RETAIL_CINTELLIGENTPOINTER), _OnInitialActivate_007e83b0, _OnSerialise_007e8460 -- the TU is plain /O2, no optimize pragma.
- 00/7d/CTCTavernGameCoinGolf_EnableCourse_007d2cc0: idiom for DrawIsDrawable/SetAsDrawable: FABLE_RETAIL_CCHARSTRING, _CTHING, _CTHING_ORIGINAL_VTABLE, _CTHING_STATEACCESS, _CCLASS, _CTHING_ISPLACED, _CTHING_DRAWABLE, _CTHING_MOREACCESS, _CTHING_GETSCRIPTNAME, _CINTELLIGENTPOINTER.
- 00/65/CTCVillage_AddThingToVillage_0065b854: GetTC of CTCThingOwner via FABLE_RETAIL_CTCTHINGOWNER (but that TU is optimize("y",off)+("s",on) -- do not copy those pragmas).
- 00/6c/CTCBuyableHouse_CreateRentMoney_006c0c90: FABLE_RETAIL_CCHARSTRING + FABLE_RETAIL_CTHING_GETTC_EXPANDED; 00/6c/CTCBuyableHouse_SetupDressingAtLevel_006c0330 does the same "Dress"-prefix _strnicmp inline (local model of GetPString/Length) -- reuse its spelling.
- 00/5e/CTCTavernGame_FrameUpdate_005eb9e0 uses engine/CTCOwnedEntity.h (defines list at lines 28-43).

## 7. Old-draft model errors
1. Constant CSE residue (70 difflines): the draft's GetInterface took `long interfaceType` and its own word/bit test; retail keeps mask 2 in bl and 0x21 as immediates. Use the shared GETTC_EXPANDED GetTC(TCI_THING_OWNER,&tc) (ETCInterfaceType key) + bitset test.
2. Wrong names: CMessageNewOwner -> NTCMessages::CMessageOnSetThingOwner; RemoveDependentThing/AddDependentThing -> RemoveOwnedObject/AddOwnedObject(CThing&) (pass *ParentThing / GetParentThing()); Get/SetPlayerOwnership -> DrawIsDrawable/SetAsDrawable(bool); BeginsWithNoCase -> StartsWithCaseInsensitive.
3. Wrong types: AddTC(name,long,long) -> (const CCharString&, bool, const CTCParamsBase*); RemoveTC(long) -> ETCInterfaceType; SendMessage(const CBaseClass&) -> SendMessageW(const NTCMessages::CMessageBase&).
4. Flags2C/Flags38 casts -> !ParentThing->HasTC(TCI_SWITCHABLE_NAVIGATION) && POwner->HasTC(TCI_BUYABLE_HOUSE) && ParentThing->GetType()==5 && ScriptName.StartsWithCaseInsensitive(CCharString("Dress")).
5. Draft defines LowerBound in the TU and models every class locally; switch to shared headers.
6. Control flow matches: if(POwner.Get()){GetTC; tc->RemoveOwnedObject(*ParentThing);} SendMessageW(CMessageOnSetThingOwner(owner)); if(SwitchableNavigationTCAdded){RemoveTC(121); =false;} POwner=CIntelligentPointer<CThing>(owner); if(POwner.Get() && !POwner->Dead){OwnerUID=UID; if(!GetTC) tc=(CTCThingOwner*)AddTC(CCharString("CTCThingOwner"),false,0); tc->AddOwnedObject(*ParentThing); ParentThing->SetAsDrawable(POwner->DrawIsDrawable()); ParentThing->SetInLimbo(POwner->IsInLimbo()/InLimbo); bool add=...; if(add){AddTC(CCharString("CTCSwitchableNavigation"),false,0); SwitchableNavigationTCAdded=true;}}. Note GetPItem is re-called for each POwner access (8 calls) -> no cached owner pointer.

## 0x00922010

# DOSSIER 0x00922010 CAIStateGroup_MinionWalkToMarker::Update (651 bytes)

## 1. Retail shape (xdis.py 00922010 0x29b)
- thiscall, no args, plain `ret` at 0x0092229A; `sub esp,4Ch` then push ebx/ebp/esi/edi. NO frame pointer, NO SEH frame
  (despite CCharString / vector locals) -> default lane flags, no /EHsc, no optimize("y",off). No size idioms.
- Registers: this=edi; PBrain=ebx (`mov ebx,[edi+4]`, reused `mov ecx,[ebx+18h]` for PeekPos -> a brain local, LESSONS.md line 7);
  component=ebp, spilled to [esp+18h] (the uninitialised GetTC out-var: miss path `mov ebp,[esp+18h]`).
- HasTC byte-homed: `shr ecx,0Ah ; and cl,1 ; mov [esp+13h],cl ; je` (bit 170 = dword +0x34 bit 10) = plain-inline HasTC via
  FABLE_RETAIL_CTHING_GETTC_EXPANDED (NOT HasTC_FORCEINLINE). Same slot [esp+13h] later is the `bool& reached` arg (lea [esp+2Bh] after 6 pushes).
- Loop vars: target [esp+14h] (ebx in loop), previous [esp+1Ch] (zeroed before loop), beforePrevious esi (zeroed).
- Loop head 0x9220D1: `cmp 8; je MODE8; cmp 9; jne EXIT; cmp 8; jne MODE9` -> `while(m==8||m==9){ if(m==8){fallthrough block} else {out-of-line block at 0x9221F6} ...}`.
- Mode 8 (fallthrough): vector<CThing*> {0,0,0} at [esp+38h], name copy [esp+24h], filter copy [esp+20h], GetAllThings(filter, vec);
  2 dtors; size=(finish-start)>>2 computed BEFORE GetPWorldSeed; GFRandom expanded (`imul 24A1h; add 24DFh; mov [ebx],ecx; call GFROR13; xor edx,edx; test esi; mov [ebx],eax; je; div esi`);
  target=start[idx]; `if(start) free(start)`; then reload ebp=[esp+18h], 16-byte-aligned inner loop (`lea ebx,[ebx]` pad).
- Inner loop 0x922190: `m==8||m==9` test, then MoveToThingProximity(2, CIntelligentPointer<const CThing>(target) [ctor 0xA01B10 inline, vptr 0x01237F1C, SetPItem], 0.0f, reached, 3, true,false,true,true, 0.0f); `cmp eax,4; je loop`.
  = `while ((m==8||m==9) && AIAction_MoveToThingProximity(...)==AI_ACTION_IN_PROGRESS) ;`
- Then `if(IsStopping()) break;` -> Terminate; else esi=previous; previous=target; ecx=0 (dead), jmp head.
- Mode 9 (0x9221F6): RELOADS `[edi+4]->+18h` (no brain local here: GetCreature() inline), PeekPos -> Nearest pos copy [esp+44h];
  name [esp+2Ch], filter [esp+28h]; IsNot#1 [esp+30h]=previous, IsNot#2 [esp+34h]=beforePrevious (stored LAST, after arg pushes);
  Bind3 {&IsNot1,&IsNot2,&filter} at [esp+50h]; GetPBestThing 0x9223A0(bind3, nearest, &target); 2 dtors; ebx=target; jmp inner loop.
- Before loop: GetTC(170) ; Nearest(PeekPos) [esp+38h] ; name [esp+20h], filter [esp+1Ch] ; GetPBestThing 0x922320(filter,nearest,&target).
  Note the Nearest compare is built BEFORE the name copies in both search blocks (draft order right).

## 2. Name / identity (manifest label is CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x009450A0) and FableWin.pdb (_fablewin_api.tsv 0x0179E433 / 0x02C374C0):
  `?Update@CAIStateGroup_MinionWalkToMarker@@MAEXXZ` = protected virtual void Update() (MAE).
- rtti-vtables.tsv: vtable 0x0128A84C slot 12 = 0x00922010 (slot 11 = landed IsRunnable 0x00921630); xref.py: only ref is .rdata 0x0128A87C (=vtable+0x30).
  CAIStateGroupBase.h: slot 12 = `virtual void Update() = 0` (protected). Leaf: ?Update@CAIStateGroup_MinionWalkToMarker@@MAEXXZ.

## 3. Callees (landed file / landed name / PDB) -- draft spelling issues flagged (!)
- 0x0040F020 00/40/CVectorMap_LowerBound_0040f020.cpp CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (via CThing::GetTC expanded).
- 0x006A4D00 00/6a/CThingPhysical_PeekPos_006a4d00.cpp ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ.
  (!) draft says CThingCreatureBase::GetPosition -- WRONG; use GetCreature().PeekPos().
- 0x0099EC30 CCharString copy ctor / 0x0099EAE0 ~CCharString (fable_string.h, FABLE_RETAIL_CCHARSTRING).
- 0x008FD000 CAIStateGroupBase::PeekThingSearchTools ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (const, returns const&; draft returns pointer (!)). Header: FABLE_RETAIL_CAISTATEGROUPBASE_PEEK.
- 0x00922320 00/92/CThingSearchTools_GetPBestThing_00922320.cpp ??$GetPBestThing@VCThingFilter_HasScriptName@@VCThingCompare_Nearest@@VCThing@@@CThingSearchTools@@QBE_N...@Z (const member (!) draft non-const).
- 0x005ED860 00/5e/CThingSearchTools_GetAllThings_005ed860.cpp GetAllThings<CThing, std::vector<CThing*>, CThingFilter_HasScriptName> (template order T,Container,Filter; const).
- 0x009223A0 00/92/CThingSearchTools_GetPBestThingCFilterBind3CThingFilterHasTCCThing_009223a0.cpp: landed comment says the instance is
  GetPBestThing<CFilter_Bind3<CThingFilter_IsNot,CThingFilter_IsNot,CThingFilter_HasScriptName>,CThingCompare_Nearest,CThing> (naming wave 24);
  manifest functions.tsv still says <HasTC,InRegion,IsRegionEntranceActive> (stale). Draft's IsNot,IsNot,HasScriptName is right.
- 0x008FCFD0 CAIStateGroupBase::GetPWorldSeed ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ (const).
- 0x00497890 GFROR13 (Ego_r ?GFROR13@@YIJK@Z, returns long) -- engine/GFRandom.h; the expansion is GFRandom(range, seed) (Ego_r ?GFRandom@@YIKKPAK@Z). (!) draft hand-wrote `Random`.
- 0x00BFEA14 free (toolchain_library) -- STLport vector dealloc.
- 0x00A01B10 ??0CBaseIntelligentPointer@@QAE@XZ ; 0x00A01B90 ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z (draft: SetPItem(const CThing*) (!)).
  The by-value temp is CIntelligentPointer<const CThing>(T*) inline (vtable 0x01237F1C) -- engine/CIntelligentPointer.h.
- 0x0097E380 (not landed) ?AIAction_MoveToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@MAA_NW4EMovementType@@_N444M@Z
  -> (EAIActionRepeat, CIntelligentPointer<const CThing>, float, bool&, EMovementType, bool,bool,bool,bool, float) returning EAIActionReturn.
  (!) draft: long/bool*/long. Args here: ACTION_REPEAT_CALLBACK(2), target, 0.0f, reached, AI_MOVEMENT_WALKING(3), true,false,true,true, 0.0f; compare AI_ACTION_IN_PROGRESS(4) (engine/fable_ai_action_enums.h).
- 0x008FD010 ?IsStopping@CAIStateGroupBase@@IBE_NXZ (const; draft non-const (!)). 0x008FCF40 ?Terminate@CAIStateGroupBase@@QAEXXZ.

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (engine/CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS + _AIACTIONS + _PEEK + _GETCREATURE).
- brain+0x18 CAIBrain::Creature (CThingAICreature&, private, under FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE; GetCreature() inline under FABLE_RETAIL_CAIBRAIN_GETCREATURE).
- creature+0x20 UsingComponents, +0x44 Components (engine/CThing.h, FABLE_RETAIL_CTHING_GETTC_EXPANDED; GetTC(TCI_AI_SCRATCHPAD,&p)).
- (!) Component 0xAA = CTCAIScratchpad (ETCInterfaceType.h TCI_AI_SCRATCHPAD=170; CTCAIScratchpad::StatGetInterfaceType). struct_layouts_egor.tsv:
  +0x38 (56) `EScriptingStateGroups ScriptingStateGroup` (8 = ESSG_WALK_TO_RANDOM, 9 = ESSG_WALK_TO_NEAREST_DIFFERENT, engine/EScriptingStateGroups.h),
  +0x50 (80) `CCharString TargetName`. Shared header engine/CTCAIScratchpad.h (sizeof 0x84). Draft's local CTCWalkToMarker{Mode,MarkerName} is a made-up model;
  landed IsRunnable 0x00921630 uses the same local struct -> NAME_FIXES candidate (respell Mode->ScriptingStateGroup, MarkerName->TargetName, class->CTCAIScratchpad).
- Filters: engine/CThingFilter_HasScriptName.h (member Name, ctor from const CCharString&), engine/CThingFilter_IsNot.h (`const CThing& Thing`, ctor from const CThing& -> `CThingFilter_IsNot(*previous)`),
  engine/CThingCompare_Nearest.h (C3DVector Pos; FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE), engine/CFilter_Bind3.h (F1/F2/F3 refs; FABLE_RETAIL_FILTER_BIND3_FUNCTION gives GFFilter_Bind3).
- Locally modelled in draft but with shared headers: CCharString, C3DVector, CThing, CThingCreatureBase, CTCWalkToMarker(=CTCAIScratchpad), CThingFilter_HasScriptName,
  CThingFilter_IsNot, CThingCompare_Nearest, CFilter_Bind3, CThingSearchTools, CBaseIntelligentPointer/CIntelligentPointer, CAIBrain, CAIStateGroupBase. Vector: use STLport std::vector<CThing*>.

## 5. Callee visibility
- 0x00922320 and 0x009223A0 directly follow this function (0x0092229B end; 0x009222A0 small unnamed fn between) -> the two GetPBestThing
  instances are COMDATs emitted from THIS TU, bodies visible: FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY (+ _ALLTHINGSBODY for PeekClassList) gives the
  same-TU __declspec(noinline) body (caller knows temporaries aren't captured -> stack-slot sharing like [esp+13h]).
- 0x005ED860 GetAllThings is a far, folded COMDAT, but its instance would also be emitted by this TU: FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY offers the noinline body.
- Same TU earlier: IsRunnable 0x00921630 (and the MinionFollowThing rows 0x00921270..0x00921990).
- CIntelligentPointer(T*) inline in retail: FABLE_RETAIL_CINTELLIGENTPOINTER (+ _FORCEINLINE_CTOR if VC7.1 budget calls it out of line, row 0x009341C0 precedent).
- STLport vector dtor inlined (`if(start) free(start)`); ~CCharString / copy ctor out of line.

## 6. Sibling idioms
- 00/93/CAIStateGroupSeekScriptTarget_Update_0093c030.cpp: GFRandom + GetTC expanded + MoveToThingProximity(…, reached, …) on shared headers; macro set to copy
  (CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CTHING_GETTC_EXPANDED, CVECTORMAP_INLINE_LOWERBOUND, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE,
  CTHINGAICREATURE, CINTELLIGENTPOINTER, C3DVECTOR, CAISTATEGROUPBASE, _MEMBERS, _GETCREATURE, _AIACTIONS, CAIBRAIN_GETCREATURE); includes engine/fable_ai_action_enums.h, engine/GFRandom.h.
- 00/93/CAIStateGroupMinionKidGroupTag_TagTarget_009341c0.cpp: CIntelligentPointer<const CThing>(T*) by-value arg, PeekPos, CAIBRAIN_CREATURE_REFERENCE, CINTELLIGENTPOINTER_FORCEINLINE_CTOR.
- 00/92/CAIStateGroupMinionWalkToMarker_IsRunnable_00921630.cpp (same class/TU): named CCharString copy + filter copy in inner block; GETTC_EXPANDED; still local models.
- 00/92/CAIStateGroupMinionFollowThing_IsExpressionPerfomedTo_00921440.cpp (same TU neighbour): `#pragma auto_inline(on)` after an off section.
- None of these use `// Flags:` lines or optimize pragmas.

## 7. Old draft's likely model errors
- Wrong names: GetPosition (is CThingPhysical::PeekPos), Random (is GFRandom), CTCWalkToMarker/Mode/MarkerName (is CTCAIScratchpad/ScriptingStateGroup/TargetName),
  bool* (bool&), long enums, non-const PeekThingSearchTools/IsStopping/GetPBestThing/GetAllThings.
- Residue (STATUS.tsv): 648v651, this in ebx not edi, frame 0x50 v 0x4c, mode-8 placement. Fixes to try: brain local only for the pre-loop part
  (retail keeps PBrain in ebx before the loop but reloads it via GetCreature() in mode 9); noinline same-TU search bodies (slot sharing reduces frame);
  inner loop as `while((m==8||m==9) && Move(...)==AI_ACTION_IN_PROGRESS);`; read mode as an int local per test (retail rereads [ebp+38h] at each head).
- `IsNot#2` (beforePrevious) is the last store before the call -- declare IsNot filters as in Bind3 order; size computed before GetPWorldSeed (GFRandom(things.size(), GetPWorldSeed())).

## 0x00AF5090

# DOSSIER 0x00AF5090 NParticleEngine::CParticleSystem::ClearParticle (657 B)

## 1. Retail shape (xdis 00af5090 0x2a1)
- thiscall, `ret 4` (one arg: the iterator by reference). this -> spilled to [esp+0x14] pre-push (= [esp+0x1c] after push esi/edi); never in a register.
- `push ebp; mov ebp,esp; and esp,-8; sub esp,1Ch` = 8-byte-aligned frame. NOT an /Oy- pragma: it is VC7.1's alignment for an 8-byte
  object local (the CEnginePrimitiveHandle local at [esp+0x20], vptr 0x0122F598 / Data [esp+0x24]). Same cause documented in landed
  rows 0x005671C0 std::__reverse ("8-byte pair temporary gives the and esp,-8 frame") and 0x007AF490 CTCBoss::KillHelpers.
- No SEH prologue (no EH state despite the handle dtor) -> no /EHsc unwinding in this TU or nothrow; ClearAll..dtor end at 0xAF52CB..0xAF5313.
- Frame slots: [esp+0x14] group index (long), [esp+0x18] group*12 (PrimIDList byte offset, strength-reduced), [esp+0x13] 1-byte
  random_access_iterator_tag temp for the __copy call, [esp+0x1c] = this, later reused for ParentHandleList._M_finish.
- Control flow: outer loop top-tested, `jmp` back (0xAF526A -> 0xAF50D0); exits at size check AND at the setne-materialised
  `!handle.IsNull()` test. Inner loop also top-tested (0xAF5170..0xAF51D1 jmp back), with `setge dl; test dl,dl` = a materialised
  `return id >= 0`. Materialised bools = inline member functions returning bool (see 2).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @0x00AB7B20) and FableWin.pdb (_fablewin_api.tsv @0x02F18C50 / 0x01804959 thunk):
  `?ClearParticle@CParticleSystem@NParticleEngine@@QAEXAAV?$_List_iterator@V?$_List_val@PAVCParticle@NParticleEngine@@V?$allocator@PAVCParticle@NParticleEngine@@@std@@@std@@@std@@@Z`
  => `public: void __thiscall NParticleEngine::CParticleSystem::ClearParticle(std::list<CParticle*>::iterator&)` -- NON-const reference
  (AAV). The old draft's `const iterator&` is wrong. Manifest label CParticleSystem::ClearParticle is correct (namespace NParticleEngine).
- Callers (xref): 0x00AF5339 DeleteParticle (landed rebuild/src/compiled/00/af/NParticleEngineCParticleSystem_DeleteParticle_00af5330.cpp,
  models the iterator as std::_List_iterator<std::_List_val<...>>), 0x00AF58B9 ClearAllParticles (landed 00af58a0), 0x00AF56FA,
  0x00AFB727, 0x00AFE0A5, 0x00B02733, 0x00B055A1.
- Shared header already declares it: rebuild/include/engine/CParticleSystem.h under FABLE_RETAIL_CPARTICLESYSTEM_UPDATE:
  `typedef std::list<CParticle*>::iterator ParticleIterator; void ClearParticle(ParticleIterator&);`

## 3. KEY FINDING: the body is four inline CParticle accessors (FableWin.pdb COMDATs right after ClearParticle 0x02F18C50)
  0x02F18DA0 ?GetPrimID@CParticle@NParticleEngine@@QBE_NJJAAJ@Z          bool GetPrimID(long list,long index,long& id) const
  0x02F18E30 ?ClearParentHandle@CParticle@NParticleEngine@@QAEXJ@Z        void ClearParentHandle(long list)
  0x02F18EA0 ?ClearAll@CParticle@NParticleEngine@@QAEXXZ                  void ClearAll()
  0x02F18ED0 ?GetParentHandle@CParticle@NParticleEngine@@QBE_NJAAVCEnginePrimitiveHandle@@@Z  bool GetParentHandle(long list, CEnginePrimitiveHandle& h) const
  (twin CDecal::ClearParentHandle 0x02F196E0 / GetParentHandle 0x02F19770 follow ClearDecal 0x02F19500 -> row 0x00AF5430 same shape.)
  None are called in retail (all inlined). Engine slot +0x78 = CIEngine slot 30 `ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long)`
  (rebuild/include/engine/CIEngine.h line 104; FableWin ?ClearChildPrimitive@CIEngine@@UAEXAAVCEnginePrimitiveHandle@@K@Z). Draft's
  "RemovePrimitive" is WRONG.
  Likely source (matches every retail block):
    CParticle* particle = *it;  CEnginePrimitiveHandle handle;
    for (long i = 0; particle->GetParentHandle(i, handle); ++i) {
        if (PEngine && !handle.IsNull()) { long id; for (long j = 0; particle->GetPrimID(i, j, id); ++j) PEngine->ClearChildPrimitive(handle, id); }
        particle->ClearParentHandle(i);
    }
    particle->ClearAll();
  Inline bodies inferred from bytes:
    GetParentHandle: if (ParentHandleList.size() > (unsigned long)list) { handle = ParentHandleList[list]; return !handle.IsNull(); } return false;
       (0xAF50D0 size via sar 3 + jbe exit; operator= = Clear()+Data assign (0xAF50ED..0xAF513E: notify slot3 if count==1, dec/Delete slot1,
        Data=0, then copy + inc); then `setne al; test al,al`.)
    GetPrimID: exactly the existing CParticle.h body (size()>list via imul 0x2AAAAAAB /12, then inner size()>index, id=..., return id>=0).
    ClearParentHandle: if (ParentHandleList.size() > (unsigned long)list) { ParentHandleList[list].Clear(); PrimIDList[list].clear(); }
       (0xAF51D3; NO PrimIDList size check; Clear = FABLE_RETAIL_CENGINEPRIMITIVEHANDLE form: null-safe count==1 notify, `dec [ecx+4]`/Delete,
        store 0 inside + unconditional second store 0xAF521D/0xAF5220; clear() = STLport erase(begin,end) -> __copy_trivial with the
        unfolded `cmp eax,eax` + memmove 0x00BFEAE6 = rebuild/include/stlport/fable_containers.h:145 __copy_trivial verbatim.)
    ClearAll: ParentHandleList.clear(); PrimIDList.clear();  (0xAF526F: handle vector erase INLINE -- __copy 0x00440260 fastcall + virtual
       dtor loop `push 0; call [eax]` via _Destroy; then PrimIDList erase OUT OF LINE: thiscall 0x00AF7910.)
  CParticle.h currently has GetPrimID + SetPrimitiveInfo only (inside FABLE_RETAIL_CPARTICLE_SPRITE, lines 72-112); GetParentHandle,
  ClearParentHandle, ClearAll are MISSING -> author must add them as a new opt-in (e.g. FABLE_RETAIL_CPARTICLE_CLEARPARTICLE) in the shared header.
  Members are private, so the accessors are how CParticleSystem reaches them (no friend in header).

## 4. Callees
| addr | landed file | landed name | PDB name | draft spelling |
| 0x0099A2D0 | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp | CBaseClass::CBaseClass | ??0CBaseClass | ok (via handle ctor) |
| 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | CBaseClass::~CBaseClass | ??1CBaseClass@@UAE@XZ | ok |
| 0x00440260 | 00/44/std_copyCEnginePrimitiveHandleCEnginePrimitiveHandle_00440260.cpp | std::__copy<CEnginePrimitiveHandle*,..,int> (__fastcall, ret 0xC) | STLport __copy | draft ok |
| 0x00AF7910 | 00/af/stdvector_erase_00af7910.cpp | vector<CArray<long> >::erase(iterator,iterator) thiscall ret 8 | ?erase@?$vector@V?$CArray@J@@ | draft: local CVector::erase ok-ish |
| 0x00BFEAE6 | 00/bf/global_memmove_00bfeae6.cpp | memmove import thunk (toolchain_library) | - | ok |
| vslot +0x78 | CIEngine.h slot 30 | ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) | FableWin CIEngine | draft "RemovePrimitive" WRONG |
| pointee +0x0C | CEnginePersistentPrimitiveBase slot 3 | NotifyReleased(const CEnginePrimitiveHandle&) | FableWin | draft "OnLastHandleReleased" WRONG |
| pointee +0x04 | CIVCountedPointeeBase slot 1 | Delete() const | FableWin | draft "Destroy" WRONG |
| handle slot 0 | CEnginePrimitiveHandle virtual dtor (scalar deleting, flag 0) | via _Destroy | | |

## 5. Members / headers
- CParticleSystem +0x08 PEngine (CIEngine*) -- rebuild/include/engine/CParticleSystem.h (protected), PDB +8 same. Particles list +0x1c
  (retail one-word STLport list; PDB +32) needs FABLE_RETAIL_CPARTICLESYSTEM_UPDATE (which declares ClearParticle(ParticleIterator&)).
- CParticle +0x84 PrimIDList CArray<CArray<long> >, +0x90 ParentHandleList CArray<CEnginePrimitiveHandle> -- rebuild/include/engine/CParticle.h
  (static assert sizeof 0x9c; PDB 132/148 -> retail 132/144; struct_layouts_egor.tsv CParticle). CArray = STLport vector (engine/CArray.h).
- CEnginePrimitiveHandle (engine/CEnginePrimitiveHandle.h): opt-ins FABLE_RETAIL_CENGINEPRIMITIVEHANDLE (Clear with Reset(0)),
  FABLE_RETAIL_CIVCOUNTEDPOINTER (Reset/Release forms, copy ctor), FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN (operator= with Clear() when
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED; also gives IsNull()), FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA (IsNull),
  FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DTOR / _DTOR (dtor = Clear(); retail end 0xAF52CB..0xAF5313 = Clear then one `mov [..],0` then ~CBaseClass).
- CIEngine.h: FABLE_RETAIL_CIENGINE (slot 30 ClearChildPrimitive).
- STLport (rebuild/include/stlport/fable_containers.h): vector::clear()/erase(first,last) inline by default (lines 535-548);
  FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE / _CLEAR_OUTOFLINE are PER-TU (all vector types) -- but retail needs erase OUT of line only
  for vector<CArray<long> > while vector<long> and vector<CEnginePrimitiveHandle> erase are inline. Probably VC7.1's inline budget (ClearAll
  inlined into ClearParticle, the third-level erase<CArray<long>> with its free loop emitted as COMDAT); try plain inline first, else a
  type-specific declaration/specialization for vector<CArray<long> >::erase.
- Locally modelled in old draft though shared headers exist: CBaseClass, CEngineInternalPrimitiveBase/CIVCountedPointer, CEnginePrimitiveHandle,
  CVector (=CArray/std::vector), CIEngine, NParticleEngine::CParticle, CParticleSystem, list node/iterator.

## 6. Visibility
- 0x00440260 __copy is far away (another TU's COMDAT) -> separate tag temp at [esp+0x13] (contrast 0x00AF7910 notes: same-TU __copy body
  lets VC home the tag in a live slot). 0x00AF7910 erase sits after ClearParticle in the same 0xAF2xxx-0xAF8xxx particle-system region.
- All four CParticle accessors inline (no calls); CEnginePrimitiveHandle Clear/operator=/IsNull inline.

## 7. Siblings (no #pragma / Flags lines in any of them; plain default flags)
- 00/af/CParticleSystem_ClearAllParticles_00af58a0.cpp (caller; local CRetailList model), NParticleEngineCParticleSystem_DeleteParticle_00af5330.cpp
  (caller, _List_iterator<_List_val> model, PDB spelling), NParticleEngineCParticleSystem_AllocateParticle_00af5010.cpp,
  CParticleSystem_ClearAllPrimitives_00af4320.cpp (PEngine re-read per call), CParticleSystem_DeleteDecal_00af5690.cpp.
- 00/af/CParticle_ClearChildPrimitive_00afe9b0.cpp = actually CParticle::SetPrimitiveInfo (FableWin ?SetPrimitiveInfo@CParticle@...@QAEXJJABVCEnginePrimitiveHandle@@J@Z;
  landed name "SetPrimitiveID" is descriptive/wrong -> NAME_FIXES candidate) -- shows handle Clear/RemoveReference spelling.
- Handle model also in landed CPSCLight::Kill 0x00B19C20 / ~CPSCLight 0x00B1AEB0 (FABLE_RETAIL_CENGINEPRIMITIVEHANDLE + FABLE_RETAIL_CIVCOUNTEDPOINTER).

## 8. Old draft model errors
- Wrong control flow: wrote ParentHandleList loop with `size()` + separate `if(!IsValid()) break;` and a second `group < size()` test;
  retail is GetParentHandle()-driven loop (size test + materialised !IsNull()) and ClearParentHandle (one size test). Rotated loops came from that.
- Index types: retail accessors take `long` (J) compared as unsigned; draft unsigned long.
- const iterator& (PDB: non-const), wrong vslot names (RemovePrimitive/OnLastHandleReleased/Destroy), local types instead of headers,
  hand-written erase_inline/clear instead of STLport clear().

## 0x00EB2140

# DOSSIER 0x00EB2140 (658 bytes) -- NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Main

## 1. Retail shape (xdis 00eb2140 0x2a2)
- thiscall, plain `ret` (0x00EB23D1), no frame pointer (`sub esp,14h; push esi; mov esi,ecx; push edi`), no SEH, no pragmas needed.
- Frame 0x14 = CIsThingAlive temp 0x10 (vptr 0x012C32F0 stored at [esp+10h] = local+4, CScriptThing copy 0x004ABE90 at local+8)
  + ONE extra dword local+0 which gets the dead store `mov dword ptr [esp+8],0` at 0x00EB214A (before `push edi`). Same slot is
  later reused by the CCharString("BRAIN_MELEE_GUARD" 0x012EFA28, -1) temp. This dead store is the ONLY residue of the old
  draft (DIFFER 651v658, hand291 STATUS.tsv).
- Callee-saved: esi=this, edi=&Me (this+8), ebx=&MyEntityHandle (this+0x1C) then reused as constant 2, ebp=4 (priority, then reused
  as AssassinState constant 4), edi later reused as 1 and 3. Epilogue pops ebp/ebx/edi/esi; early exit before `push ebx` at 0x00EB23CC.

## 2. Identity (two sources)
- Leaf: `?Main@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@EAEXXZ` (private virtual, void, no params).
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x00F1EE80) + egor_signatures.tsv; only xref = .rdata 0x012EFEEC = slot 1 of the
  RTTI-less entity vtable 0x012EFEE8 (the landed Init 0x00EB2070 proves slot 2 of the same vtable; ctor 0x00EB2000 stores it).
  Manifest label is CORRECT.

## 3. Callees (landed file under rebuild/src/compiled/00/<xx>/ ; draft spelling issues flagged)
| addr | landed | note |
|---|---|---|
| 0x004ABE90 | CScriptThing_CScriptThing_004abe90.cpp (copy ctor) | inside inline CIsThingAlive ctor |
| 0x00F35B10 | CEntityScriptBase_SetPredicate_00f35b10.cpp | header: `SetPredicate(const NScript::CPredicateBase&)` (draft took CIsThingAlive& -- wrong) |
| 0x00E241B0 | NScriptCIsThingAlive_CIsThingAlive_00e241b0.cpp (~CIsThingAlive, out of line) | header engine/CIsThingAlive.h virtual dtor |
| 0x00F35B30 | CEntityScriptBase_IsActiveThreadTerminating_00f35b30.cpp | const |
| 0x0099EBF0 / 0x0099EAE0 | CCharString_Constructor / CCharString_CCharString | fable_string.h `CCharString(const char*, long length=-1)` |
| 0x00CD23B9 | CScriptGameResourceObjectScriptedThingBase_IsValid_00cd23b9.cpp | **draft says IsScripting -- WRONG, landed/header name IsValid() const** |
| 0x00CD2770 | ..._Release_00cd2770.cpp | Release (PDB ?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ) |
| 0x008AB980 | CScriptThing_operator_008ab980.cpp | CScriptThing::operator= (header declares it out of line by default) |
| 0x008ABD10 | CScriptGameResourceObjectScriptedThingBase_operator_008abd10.cpp | needs opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN |
| slot 7 +0x1C | CGameScriptInterface_NewScriptFrame_006e7410.cpp | via Game (CGameScriptInterfaceBase*) |
| slot 8 +0x20 | CGameScriptInterface_StartScriptingEntity_0089b5b0.cpp | priority 4 = SCRIPT_AI_PRIORITY_HIGHEST (Assassin Main row enum) |
| slot 589 +0x934 | CGameScriptInterface_SetCreatureBrain_00894b90.cpp | (const CScriptThing&, const CCharString&) const |
| 0x00EB23E0 | ..SheriffGuard_DoAskingHeroToSpeakToBoss_00eb23e0.cpp | draft OK |
| 0x00EB2900 | ..SheriffGuard_DoRemindingHeroToKillAssassin_00eb2900.cpp | **draft says DoAskingHeroToSpeakToBossAgain -- WRONG** |
| 0x00EB2DC0 | ..SheriffGuard_DoAskingHeroToSpeakToBossAgain_00eb2dc0.cpp | **draft DoStateHandler2 -- WRONG** |
| 0x00EB3280 | ..SheriffGuard_DoCongratulatingHero_00eb3280.cpp | **draft DoStateHandler3 -- WRONG** |
| 0x00EB3740 | ..SheriffGuard_DoSayingThankYouToHero_00eb3740.cpp | draft OK |
| 0x00EB3BA0 | ..SheriffGuard_AttackHero_00eb3ba0.cpp | **draft DoFightHero -- WRONG** |
Ego_r order (Alloc, DoAskingHeroToSpeakToBoss, DoRemindingHeroToKillAssassin, DoAskingHeroToSpeakToBossAgain, DoCongratulatingHero,
DoSayingThankYouToHero, AttackHero, Init, Main) agrees with these landed names. All declared in engine/CV_MurderTwistFinalScript.h.

## 4. Members
- this: engine/CV_MurderTwistFinalScript.h `CMurderTwistSheriffGuard : CEntityScriptBase` (no opt-in needed; always present):
  Game +0x04 (CGameScriptInterfaceBase*), Me +0x08 (CScriptThing), ParentClass +0x14, MyEntityHandle +0x1C, ID +0x30 (unsigned long).
- parent (same header): SheriffState +0x48 (ESheriffState), AssassinState +0x4C (EAssassinState), Guards +0x60 std::vector<CScriptThing>
  (12-byte elts: `lea ecx,[ID*3]; mov eax,[p+60h]; lea ecx,[eax+ecx*4]`), seh_Guards +0x7C std::vector<ScriptedThingBase> (`shl ecx,4`).
- Header enum EAssassinState stops at ASSASSIN_STATE_3, but retail compares AssassinState with 4 (cmp [eax+4Ch],ebp, ebp=4).
  Landed Sheriff Main 0x00EAEF30 uses ASSASSIN_STATE_4 via a local enum; with the shared header use `(EAssassinState)4` or ask to add ASSASSIN_STATE_4=4.
- Header contract: define FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CLEVELSCRIPTBASE, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCHARSTRING
  (+ FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CDEFSTRINGTABLE as the Assassin Main row does) before including.
- Old draft LOCALLY models CCharString, CScriptThing, CScriptGameResourceObjectScriptedThingBase, CGameScriptInterface (589 dummy slots),
  CIsThingAlive, CEntityScriptBase, CVector, CV_MurderTwistFinalScript, CMurderTwistSheriffGuard -- ALL have shared headers; replace them.

## 5. Visibility / opt-ins
- Inline CIsThingAlive ctor: define it in the row like Assassin Main 0x00EB3F40:
  `inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}`; dtor 0x00E241B0 stays out of line.
- ScriptedThingBase operator= called out of line -> FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN.
- CScriptThing::operator= out of line by default (do NOT define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT).
- vector::operator[] inline (STLport fable_containers.h); no STL out-of-line calls.
- Same TU (CV_MurderTwistFinalScript source): Sheriff entity 0x00EAED10..0x00EB1EC0, Guard 0x00EB1F80..0x00EB3BA0, Assassin 0x00EB3C60...

## 6. Siblings (all `// Flags: none`, no pragmas)
- **NScriptCVMurderTwistFinalScriptCMurderTwistAssassin_Main_00eb3f40.cpp** (rebuild/src/compiled/00/eb) -- shared-header template;
  same dead `mov dword ptr [esp+0Ch],0` in the same frame slot, reconstructed with `TRetVoid _Fail_Val;`, `SetPredicate(CIsThingAlive(Me));`
  (temporary, not a named local), `return _Fail_Val.Return();`, and dead `if (trust && false && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { }`
  marked `Inferred: RECONSTRUCTED DEAD CODE`. Opt-ins: FABLE_RETAIL_NSCRIPT, _CLEVELSCRIPTBASE, _BASE_OBJECT, _CCHARSTRING, _DEF_CLASS_BASE,
  _CDEFSTRINGTABLE, _CV_MURDERTWISTFINALSCRIPT_ASSASSIN; includes CV_MurderTwistFinalScript.h, CScriptGameResourceObjectScriptedThingBase.h,
  CGameScriptInterfaceBase.h, TRetVoid.h, CIsThingAlive.h.
- **NScriptCVMurderTwistFinalScriptCMurderTwistSheriff_Main_00eaef30.cpp** (00/ea) -- byte-identical twin structure (SetCreatureBrain
  "BRAIN_MELEE_GUARD" same string 0x012EFA28, priority 4, Sheriff/seh_Sheriff registration, final `do{NewScriptFrame}while(!term)`),
  but WITHOUT the dead flag store (its prologue has no `mov [esp+..],0`). Uses local models + named `CIsThingAlive filter(Me)` block.
- NScriptCVMurderTwistFinalScriptCMurderTwistSheriffGuard_Init_00eb2070.cpp, ..._DoSayingThankYouToHero_00eb3740.cpp (same class).
- GOTCHAS.md line 230: dead entry `mov [esp+N],0` with frame one dword bigger = folded conditional-destruction flag; reproduce with
  `true ? (void)0 : (void)f();` (destructible temp in an arm) or `x && false && f("s")` (optimiser-proved false); a front-end
  constant `false && ...` does NOT work. Examples 0x00E2EAC0 (TricksterApproach line 41) and 0x00EB3F40.

## 7. Old draft vs retail
- Control flow of the draft is right (it matched all but 7 bytes): loops
  L1 while SheriffState==0 -> DoAskingHeroToSpeakToBoss; L2 while AssassinState==0 && SheriffState!=2 -> DoRemindingHeroToKillAssassin;
  L3 while AssassinState!=4 && SheriffState!=2 -> DoAskingHeroToSpeakToBossAgain; L4 while AssassinState==4 && SheriffState==1 -> DoCongratulatingHero;
  L5 while SheriffState==3 -> DoSayingThankYouToHero; if SheriffState==2 {term check; AttackHero}; then NO term check before the final
  `do { NewScriptFrame(); } while (!IsActiveThreadTerminating());` (retail 0x00EB23B7 is reached directly from both arms).
- Missing: the dead flag store -> add one reconstructed dead conditional with a destructible temporary (CCharString/CScriptThing) per
  GOTCHAS 230; position unknown (retail schedules the store first, before `push edi`; in Assassin Main it lands after `lea ecx`). Try it
  at the top (before SetPredicate) and right after the StartScriptingEntity loop; mark `Inferred:`.
- Wrong names (IsScripting, DoStateHandler2/3, DoFightHero, 2900 name), wrong SetPredicate parameter type, local models instead of headers,
  `(EScriptAIPriority)4` vs SCRIPT_AI_PRIORITY_HIGHEST.

## 0x009F3B00

# DOSSIER 0x009F3B00 -- CSurface::CopyFromMemory (4-arg overload), 664 bytes

## 1. Retail shape (python3 $S/w17/xdis.py 009f3b00 0x2a8)
- __thiscall, `ret 0x10` (4 dword args), NO frame pointer (`sub esp,9Ch ; push ebx,ebp,esi,edi`), no SEH, no /GS. Plain /O2 style (not size-optimised: `rep movsd/rep movsb` memcpy intrinsic, `lea esp,[esp]` loop-alignment pad at 0x9F3D49).
- Frame 0x9C: [+10] scratch (Height in else-arm, then the down-counter), dst box TLX/TLY at [+14]/[+18], dst.BRX in esi, dst.BRY at [+20]; `this` spilled at [+24]; src box TLX/TLY at [+28]/[+2C], src.BRX in ebp, src.BRY in ebx; locked rect at [+48] (Dimensions +48/+4C, Pitch +50, PMem +54); D3D descs at [+6C] and [+8C]; both-null arm uses descs at [+48] and [+28] (overlapping locked rect / src box).
- `xor ebx,ebx` at entry: 0 is cached in EBX (pointer tests `cmp ecx,ebx`, zero stores `mov [..],ebx`), then ebx is reused as src.BRY. Old draft had 0 in ebp (its stated residue).
- Flow: dst = dstBox ? *dstBox : srcBox ? {0,0,src.BRX-src.TLX,src.BRY-src.TLY} : {0,0,GetWidth(),GetHeight()} (both-null arm: inline GetHeight first via desc[+48]->Height, then inline GetWidth via desc[+28]->Width). srcBox-only arm tail-jumps back into the `if (srcBox)` copy (0x9F3B6F -> 0x9F3B37). src = srcBox ? *srcBox : {0,0,dst.BRX-dst.TLX,dst.BRY-dst.TLY}.
- Lock(0) 0x009F33E0 via hidden result ptr (&[+48]); `if (!locked.PMem) return;` (no Unlock on that path, jumps straight to epilogue 0x9F3D8B).
- `if (srcPitch == 0) srcPitch = locked.Pitch;` (stored back into the arg slot [+BC]).
- Inline GetPixelFormat(): GetDesc into [+6C], `CPixelFormat::Initialise(desc.Format)` with this=&[+6C] (temp shares desc's words; result unused -- an unused `CPixelFormat` local/temporary).
- Full-copy test, order: srcPitch==Pitch, src.TLX==0, src.TLY==0, src.BRX==GetWidth() [INLINE: GetDesc [+6C], cmp ebp,[+84]], src.BRY==GetHeight() [CALL 0x415BF0], dst.TLX==0, dst.TLY==0, dst.BRX==GetWidth() [CALL 0x415BD0], dst.BRY==GetHeight() [CALL 0x415BF0]. -> memcpy(locked.PMem, data, GetByteLength() [CALL 0x9F2FC0]) then jmp to unlock.
- Else: inline GetWidth (desc [+6C] -> esi), inline GetHeight (desc [+8C] -> [+10]), then GetByteLength() call; `bpp = (len*8) / (W*H)` UNSIGNED div (`shl 3 ; xor edx,edx ; div`); then SIGNED /8 (cdq/and 7/sar 3) for: rowBytes=(src.BRX-src.TLX)*bpp/8 (ebp), from=data+bpp*src.TLX/8 (ecx), to=PMem+bpp*dst.TLX/8 (eax) -- so bpp is a signed long after the unsigned division. Note dst.TLY is never used (rows offset by src.TLY on BOTH sides -- original bug, keep it).
- Row loop: guard `src.TLY < src.BRY` (jge skip), starts computed after the guard: to=src.TLY*Pitch+to, from=src.TLY*srcPitch+from; count=BRY-TLY in [+10], dec/jne; body memcpy(to,from,rowBytes) as rep movsd/movsb; steps from+=srcPitch ([+BC]), to+=Pitch ([+50]). Looks like a strength-reduced `for (y = src.TLY; y < src.BRY; ++y)`.
- Unlock: reload this from [+24], `PD3DSurface->UnlockRect()` (vtable +0x38) inline -> inline `Unlock()`.

## 2. Name / signature (two sources)
- Ego_r.pdb ghidra_out/egor_pdb_names.tsv: 00b477f0 `?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0J@Z`; FableWin.pdb ghidra_out/fablewin_pdb_names.tsv 030c8780 + _fablewin_api.tsv 01808db0 same mangled name. Ego_r order (CopyFromSurface 0xb474e0, CopyToMemory 0xb476b0, CopyFromMemory 0xb477f0, CopyToMemoryB8G8R8A8 0xb47aa0) mirrors retail (0x9F37E0, 0x9F39A0, 0x9F3B00, 0x9F3DA0). Ego_r body 680 bytes (naming_stragglers/ego_r.exe.signatures.tsv) vs retail 664.
- Caller: 0x009F44FA inside landed CSurface::CopyFromMemory(6-arg) 0x009F4390 (rebuild/src/compiled/00/9f/CSurface_CopyFromMemory_009f4390.cpp) calls `CopyFromMemory(destRect, source, sourceRect, sourcePitch)`.
- Manifest label CSurface::CopyFromMemory is CORRECT. Signature: `void CSurface::CopyFromMemory(const C2DBoxI* destRect, const void* source, const C2DBoxI* sourceRect, long sourcePitch)` (public, non-const). Param names as in header decl (engine/CSurface.h) / 6-arg sibling: destRect, source, sourceRect, sourcePitch. Leaf for row: `?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0J@Z`.

## 3. Callees
| retail | landed row | landed name | PDB | draft spelling |
|---|---|---|---|---|
| 0x009F33E0 | 00/9f/CSurface_Lock_009f33e0.cpp | CSurface::Lock | ?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z (Ego_r 0xb470e0) | Lock(unsigned long) OK; draft's CSurfaceLockedRect {DimX,DimY,Pitch,PMem} WRONG: PDB is {C2DExtentsI Dimensions; long Pitch; uchar* PMem} with user copy ctor |
| 0x009E3830 | 00/9e/CPixelFormat_InitialiseD3DFormat_009e3830.cpp | CPixelFormat::Initialise(_D3DFORMAT) | ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z | draft takes `unsigned long` -- should be `_D3DFORMAT` |
| 0x00415BD0 | 00/41/CSurface_GetWidth_00415bd0.cpp | CSurface::GetWidth() const | ?GetWidth@CSurface@@QBEKXZ (Ego_r 0x414F70 + FableWin) | OK, but draft's inline GetWidth reads GetDesc directly; real inline is `GetDimensions().X` |
| 0x00415BF0 | 00/41/CSurface_GetHeight_00415bf0.cpp | CSurface::GetHeight() const | ?GetHeight@CSurface@@QBEKXZ | same (`GetDimensions().Y`) |
| 0x009F2FC0 | 00/9f/CSurface_GetByteLength_009f2fc0.cpp | leaf `?GetByteLength@CSurface_Methods@@QBEKXZ` | ?GetByteLength@CSurface@@QBEKXZ (Ego_r 0xb46ca0) | landed LEAF IS WRONG (CSurface_Methods) -> NAME_FIXES candidate; draft OK |
| vtbl +0x30 | IDirect3DSurface9::GetDesc(_D3DSURFACE_DESC*) | engine/Direct3DSurface9.h | | draft local struct |
| vtbl +0x38 | IDirect3DSurface9::UnlockRect() | via inline CSurface::Unlock() | | draft calls PD3DSurface->UnlockRect() directly (same bytes) |
| (inline) 0x009F2F90 GetDimensions | 00/9f/CSurface_GetDimensions_009f2f90.cpp | ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ | expanded in place here | draft lacks it |
| (inline) 0x009F2F60 GetPixelFormat | 00/9f/CSurface_GetPixelFormat_009f2f60.cpp | ?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ | expanded in place | draft builds `CPixelFormat format(GetD3DFormat())` -- invented GetD3DFormat; use GetPixelFormat() |

## 4. Members
- CSurface (engine/CSurface.h, struct_layouts_egor.tsv 34704-34708): +0 vptr (virtual dtor), +4 `IDirect3DSurface9* PD3DSurface` (only member touched), +8 AllocationSource, +C PAllocatedMemory.
- C2DBoxI (engine/C2DBoxI.h; egor 7388-7391): TLX, TLY, BRX, BRY (long). Draft spells left/top/right/bottom -- WRONG. Opt-ins: FABLE_RETAIL_C2DBOXI (GetExtents etc.), _COPY (user copy ctor), _CTORS (4-long ctor via Initialise).
- CSurfaceLockedRect (engine/CSurfaceLockedRect.h, no macro; egor 38711-38713): Dimensions (C2DExtentsI X,Y), Pitch, PMem; user default+copy ctors.
- C2DExtentsI (engine/C2DExtentsI.h): X, Y.
- CSurface.h opt-ins relevant: FABLE_RETAIL_CSURFACE (base), _COPYFROMMEMORY (inline virtual dtor + 6-arg decl), _COPYFROMMEMORY_BODY (inline GetPixelFormat + THIS 4-arg decl 0x009F3B00 + private CanDo/CopyFromUnsigned), _BITMAP (GetDimensions + inline GetWidth/GetHeight), _DIMENSIONS_INLINE (inline GetDimensions body), _SIZE_OUTOFLINE (GetWidth/GetHeight decl only -- do NOT use: retail inlines some), _GETWIDTH_FORCEINLINE / _GETHEIGHT_OUTOFLINE (per-TU), _CLEAR (Lock decl, inline Unlock, but ALSO an INLINE GetByteLength body -- retail calls GetByteLength out of line, so _CLEAR is a hazard; no opt-in yet that declares Lock + out-of-line GetByteLength: author needs a new macro, e.g. split Lock/Unlock/GetByteLength decl, or watch whether VC7.1 inlines the _CLEAR body).
- Engine/Direct3DSurface9.h supplies IDirect3DSurface9 / _D3DSURFACE_DESC (used by 0x009F4390).
- Draft models LOCALLY: IDirect3DSurface9, D3DSURFACE_DESC_Local, C2DBoxI, C2DExtentsI, CSurfaceLockedRect, CPixelFormat, CSurface -- ALL have shared headers (Direct3DSurface9.h, C2DBoxI.h, C2DExtentsI.h, CSurfaceLockedRect.h, CPixelFormat.h, CSurface.h).

## 5. Callee visibility (same TU = retail CSurface.cpp, Ego_r order)
- GetPixelFormat 0x9F2F60, GetDimensions 0x9F2F90, GetByteLength 0x9F2FC0, Lock 0x9F33E0 are DEFINED EARLIER in the same TU -> bodies visible. GetPixelFormat/GetDimensions inline (opt-ins exist). Lock: landed sibling CopyFromMemoryAutoSwizzle 0x9F3470 defines a same-TU `#pragma auto_inline(off)` stand-in body for Lock (so VC7.1 knows the result address doesn't escape) -- same trick in CopyToMemoryAutoUnswizzle 0x9F3520, CopyFromUnsignedDataToSignedSurface 0x9F36E0, Clear 0x9F3FF0. GetByteLength called out of line here (twice); its body may also want a same-TU noinline stand-in.
- GetWidth/GetHeight: header inlines; COMDAT copies at 0x415BD0/0x415BF0 (far address = emitted by another TU, linker kept one) -> retail CSurface.cpp hit VC7.1's per-caller inline budget: first 2 GetHeight/GetWidth + 1 in condition expanded, the next three called. Old notes say draft reproduced this "budget-limited inline then out-of-line" pattern already with plain inline bodies.
- No STLport involved.

## 6. Siblings
- 0x009F4390 CSurface::CopyFromMemory (6-arg): macros FABLE_RETAIL_CPIXELFORMAT, _CPIXELFORMAT_SIGNED, _C2DBOXI, _CSURFACE, _CSURFACE_COPYFROMMEMORY, _COPYFROMMEMORY_BODY, _BITMAP, _DIMENSIONS_INLINE; includes Direct3DSurface9.h, C2DExtentsI.h, C2DBoxI.h, CPixelFormat.h, CSurface.h. No pragmas. Best template.
- 0x009F3470 CopyFromMemoryAutoSwizzle: methods struct `CSurface_Methods : CSurface`, local CSurfaceLockedRect, same-TU noinline Lock under `#pragma auto_inline(off/on)`, memcpy row loop `for (y=0;y<Y;++y) memcpy(PMem+y*Pitch, data+y*pitch, X*depth)` (index form, matched).
- 0x009F37E0 CopyFromSurface: whole class modelled locally (reason: generated header vptr), lock in own `{}` block.
- 0x009F2F90 GetDimensions, 0x00415BF0 GetHeight (_SIZE_OUTOFLINE).
- No `// Flags:` lines with values in this neighbourhood (Lock has an empty `// Flags:`); default /O2 flags.
- Mirror function CopyToMemory 0x009F39A0 (manifest `CSurface_CopySurfaceData`, unlanded; Ego_r ?CopyToMemory@CSurface@@QAEXPBVC2DBoxI@@PAX0J@Z) -- likely same box-default code, useful cross-check.

## 7. Old draft model errors
- Local classes everywhere (header rule violation), C2DBoxI fields left/top/right/bottom (PDB TLX..BRY), CSurfaceLockedRect DimX/DimY (PDB Dimensions.X/Y), invented GetD3DFormat(), Initialise(unsigned long) instead of _D3DFORMAT, GetWidth/GetHeight read desc directly instead of GetDimensions().X/.Y.
- No same-TU Lock (and GetByteLength) body visible -- siblings needed it for register/frame allocation.
- Row loop: draft pre-adds `src.top*Pitch` before the loop with pointer stepping; retail computes start AFTER the guard -> try index form `for (long y = src.TLY; y < src.BRY; ++y) memcpy(to + y*locked.Pitch, from + y*sourcePitch, rowBytes);` (as AutoSwizzle).
- Zero cached in ebx (retail) vs ebp (draft) and two descs at distinct frame words: per NOTES next step was separate `{}` blocks; real inline GetDimensions temporaries (C2DExtentsI) may give that naturally.
- Unused CPixelFormat: spell as `CPixelFormat format = GetPixelFormat();` (or bare `GetPixelFormat();`) after the srcPitch default.

## 0x005C71AA

# DOSSIER 0x005C71AA  CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight (680 B)

## 1. Retail shape (xdis 005c71aa 0x2b8)
- thiscall, `ret 10h` (4 stack args: CWideString& text [ebp+8], long itemDefIndex [ebp+0Ch], float damage [ebp+10h] UNUSED, bool fromDef [ebp+14h]).
- `push ebp ; mov ebp,esp ; sub esp,38h ; push ebx/esi/edi` -> frame pointer: `#pragma optimize("y",off)` + `optimize("s",on)`; NO SEH frame.
- ebx = 0 for whole function. Frame: [ebp-4] def, [ebp-8] miscDef, [ebp-0Ch] CCharString temp then weaponDef (slot reused),
  [ebp-38h..-1Ch] CPerItemData (vector +0..+8, kills/time/witnesses zeroed, QuestPersistent=1 [-20h], RefCount=-1 [-1Ch]).
  [ebp+14h] (dead fromDef slot) reused for augDef out-ptr, slot count, CCharString temp, GetDetails result.
- Early-out path: `GetNumberOfItemsOfTypeInInventory(idx)==0` -> jumps to 0x5C740F, SKIPPING the data vector free (compiler knows data is still empty).
- Releases at end in order weaponDef(-0Ch), def(-4, `add eax,4;dec[eax];cmp[eax],0` form + `mov [ebp-4],0`), miscDef(-8).

## 2. Identity
- Name CONFIRMED (manifest label right): Ego_r.pdb `?ConstructWeaponDescriptionNoDamageNorWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z`
  (egor_pdb_names.tsv + egor_signatures.tsv donor 0x00626839, between ConstructWeaponDescription 0x006261B8 and ...Damage 0x00626ACE);
  retail order matches landed neighbours ConstructWeaponDescription 0x005C6B26 (1668 B = gap) and ConstructWeaponDescriptionDamage 0x005C7452
  (its header comment already names 0x005C71AA). Header rebuild/include/engine/CTCInventoryWeapons.h:61 declares it (bool param named `pc`; siblings use `fromDef`).
- Signature: `void ConstructWeaponDescriptionNoDamageNorWeight(CWideString& text, long itemDefIndex, float damage, bool fromDef) const`.
- Caller: 0x005C7CD4 only (inside landed GetItemDescriptionNoDamageNorWeight 0x005C7B10). Not a vtable slot.

## 3. Callees (landed file / landed name / PDB name; DRAFT spelling disagreements marked !!)
- 0x005C30D3 CTCInventoryWeapons_RefreshActiveWeaponData_005c30d3.cpp  RefreshActiveWeaponData() const  OK
- 0x0099EBF0 / 0x0099EAE0 CCharString ctor(const char*, long=-1) / dtor  OK
- 0x0044C6B0 CGameDefinitionManager::Get()  OK
- 0x009AD390 !! draft `GetDefGlobalIndexFromName(name,bool)`; sibling/manifest: `CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long)` called as `GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1)` (Ego_r ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z). Landed file is the ICF name COptimisedPrimitive_ActualToUniqueIndex_009ad390.cpp.
- 0x0042B312 !! draft `CGameDefinitionManager::GetDef(long, CDefPointer<CMiscThingsDef>&)`; real: template `CDefinitionManager::GetDef<NUISystem::CUIMiscThingsDef>(long, CDefPointer<const T>*) const` (pointer out param, `&miscDef`). Draft's `CMiscThingsDef` is wrong class name.
- 0x006869D0 CTCBase::GetDefinitionManager() const -> `const CGameDefinitionManager&` (?..@IBEABV..). !! draft returns pointer.
- 0x009AD9E0 CDefinitionManager::GetPDefFromGlobalIndex(long) const -> CDefPointer<const CDefClassBase> by value.  !! draft `CDefBasePointer`.
- 0x0042B1A6 `??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z`  OK-ish (draft non-const, non-template).
- 0x0042AF9E !! draft `GetSubDef(CDefPointer<CAugmentationDef>&)`; real `GetSubDef<CObjectAugmentationsDef>(CDefPointer<const CObjectAugmentationsDef>*) const` (CAugmentationDef is a DIFFERENT def, used by GetAugmentationName).
- 0x004428F0 vector<long>::resize(n, const long&) out of line -> FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE.
- 0x005BDF08 NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const (?..@QBEJJ@Z) landed OK.
- 0x005BE8D3 !! draft `GetItemSlotValues`; Ego_r `?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z` returning `const CPerItemData*` (header NInventory_CTCInventoryBase.h:108). NOTE: landed row file is named CTCInventoryBase_GetBoughtPrice_005be8d3.cpp (name conflict -> NAME_FIXES candidate; siblings 0x005C6B26/0x005C90FC/0x005BE8F3 all call it GetPerItemDataForItem).
- 0x009C95E0 NGameText::CDataBank::GetTextBySymbol(const CCharString&) const -> CWideString.
- 0x0099BE70 free `operator+(const CWideString&, const CWideString&)` __fastcall (??H@YI?AVCWideString@@ABV0@0@Z).
- 0x0099B8D0 CWideString::operator+=(const CWideString&);  0x0099B510 ~CWideString.
- 0x00765F90 !! draft free `GetAugmentationName`; real static `CTCObjectAugmentations::GetAugmentationName(long)` __fastcall (?..@SI?AVCWideString@@J@Z).
- 0x005D9630 !! draft `GetItemDefDescription`; real static `CTCInventoryItem::GetDetails(long)` __fastcall (?GetDetails@CTCInventoryItem@@SI?AVCWideString@@J@Z), header opt-in FABLE_RETAIL_CTCINVENTORYITEM_DETAILS.
- 0x00BFEA14 free (STLport vector dtor).
- Globals: 0x013B86A0 !! draft `g_pMainGameComponent`; real CMainGameComponent::Get() (static PMainGameComponent) ->GetTextBank() (+0x14), FABLE_RETAIL_CMAINGAMECOMPONENT(_TEXTBANK).
  0x013B8BA8 !! draft `g_NewLine`; real `namespace { extern const CWideString NEWLINE; }` (this TU's anon NEWLINE, as siblings).
- Strings: 0x01230018 "UI_MISC_THINGS_DEF", 0x01254568 "TEXT_GUI_MENU_AUGMENTATIONS", 0x01254554 "TEXT_GUI_MENU_NONE".

## 4. Members
- miscDef +0x3C `SpaceSeparator`, +0x40 `CommaSeparator` (engine/NUISystem_CUIMiscThingsDef.h:22-23; static asserts). !! draft names Colon/Separator on local CMiscThingsDef.
- CObjectAugmentationsDef +0x28 `NumberOfSlots`, +0x40 `std::vector<long> InitialAugmentations` (engine/CObjectAugmentationsDef.h). !! draft local CAugmentationDef/SlotValues.
- NInventory::CPerItemData: +0 `std::vector<long> Augmentations`, NumberOfKills/TimeWielded/BestTrophyWitnessesAheadToDate, QuestPersistent(true), QuestPersistentRefCount(-1)
  (engine/NInventory_CTCInventoryBase.h:41-49; FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA + _PERITEMDATA_CTOR, the latter's comment names THIS row).
- CTCBase ParentThing +0x04 (via GetDefinitionManager). CDefClassBase PDefManager +0x1C (inside GetSubDef bodies).
- Draft-local classes that have shared headers: CCharString, CWideString, CDataBank (NGameText_CDataBank.h), CMainGameComponent, vector (STLport),
  CDefBase/CDefPointer/CDefClassBase, CMiscThingsDef(->NUISystem_CUIMiscThingsDef.h), CWeaponDef, CAugmentationDef(->CObjectAugmentationsDef.h),
  CGameDefinitionManager, CDefinitionManager, CPerItemData (NInventory_CTCInventoryBase.h), CTCInventoryWeapons. ALL must go.

## 5. Callee visibility
- GetDef<T> 0x0042B312, GetSubDef<CWeaponDef> 0x0042B1A6, GetSubDef<CObjectAugmentationsDef> 0x0042AF9E: same-TU template bodies under
  `#pragma auto_inline(off)` (copy verbatim from 0x005C7452 / 0x005C6B26) so the CDefPointer locals stay unaliased.
- vector resize out of line: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE. CPerItemData ctor inline: _PERITEMDATA_CTOR.
- CDefPointer release forms: FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (siblings define it), FABLE_RETAIL_CINTELLIGENTPOINTER.

## 6. Siblings (same TU, same idioms) -- copy their header block VERBATIM
- 0x005C6B26 CTCInventoryWeapons_ConstructWeaponDescription_005c6b26.cpp: SUPERSET of this function (same def lookups, same fromDef/augmentation copy loop,
  same augmentation-names loop over `const std::vector<long>& augmentations = itemData->Augmentations;`, same NONE text). Strip damage/value/weight parts.
- 0x005C7452 ...ConstructWeaponDescriptionDamage_005c7452.cpp: pragmas y off + s on, defines: CCHARSTRING CWIDESTRING DEF_CLASS_BASE CTCBASE CTHING
  CTHING_GETTC_EXPANDED STLP_BITSET_TEST_DECL STLP_VECTOR_RESIZE_OUTOFLINE CTCINVENTORYBASE CTCINVENTORYBASE_SELECTEDOWNED (needed for
  GetNumberOfItemsOfTypeInInventory) CTCINVENTORYBASE_PERITEMDATA(_CTOR) CTCINVENTORYWEAPONS_METHODS CDEFSTRINGTABLE CRESOURCELIST CDEFINITIONMANAGER
  CGAMEDEFINITIONMANAGER CMAINGAMECOMPONENT(_TEXTBANK) CTCINVENTORYITEM(_DETAILS) CINTELLIGENTPOINTER CDEFPOINTER_RELEASE_POINTEE (+CTCWEAPON* not needed);
  includes ETCInterfaceType.h, CThing.h, CTCInventoryWeapons.h, CTCInventoryItem.h, CTCObjectAugmentations.h, CMainGameComponent.h,
  NGameText_CDataBank.h, NUISystem_CUIMiscThingsDef.h, CGameDefinitionManager.h, CWeaponDef.h, CObjectAugmentationsDef.h.
- 0x005C770C ConstructWeaponDescriptionWeight, 0x005C90FC ConstructWeaponDescriptionContainer, 0x005C7B10 GetItemDescriptionNoDamageNorWeight (caller).

## 7. Old-draft model errors vs disassembly
- Residue (681v680): 2nd loop `cmp [eax+esi*4],ebx ; mov edx,[eax+esi*4]` (re-reads `mov eax,[edi]` each pass) -- landed 0x005C6B26 gets this with
  `const std::vector<long>& augmentations = itemData->Augmentations;` + `augmentations[i]` and the first loop via the same ref (pointer walk `add ecx,4`).
- "Two push/lea swaps": the draft's `bool none` + single `if (none)` join schedules in source order; retail's single NONE block at 0x5C73A1 has
  `push -1 ; lea ecx,[ebp+14h] ; push str` and `lea eax,[ebp+14h]; push eax; lea eax,[ebp-14h]; mov ecx,esi; push eax` = the cross-jumped
  copy of TWO `else { text += bank.GetTextBySymbol("TEXT_GUI_MENU_NONE"); }` branches (exactly as landed 0x005C6B26 explains). Use that form.
- Bank: `const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();` held in esi (loaded once at 0x5C72D5).
- Else branch: `else { if (GetNumberOfItemsOfTypeInInventory(itemDefIndex) == 0) return; itemData = GetPerItemDataForItem(itemDefIndex); }` (draft had this right).
- Tail: `text += NEWLINE; text += CTCInventoryItem::GetDetails(itemDefIndex);`. Head: `text += bank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + miscDef->SpaceSeparator;`; separator `miscDef->CommaSeparator`.
- Wrong types: vector element copy uses unsigned index (`jae`), counts loops signed (`jl`/`jle`): keep `unsigned long i` copy loop, `long` count loops (as sibling).

## 0x009735D0

# DOSSIER 0x009735D0 -- CAIStateGroup_JackOfBladesBase::RushToPos (690 bytes)

## 1. Retail shape (xdis 009735d0 0x2c2)
- __thiscall, `ret 4` (one arg: const C3DVector& at [esp+0x170] after prologue). Returns eax = AIAction_PerformActionAndWait result.
- NO frame pointer: `sub esp,15Ch ; push ebx ; push ebp ; push esi ; push edi` (edi pushed late, after `mov [esp+10h],ebx`
  and the creature load). No `push ebp; mov ebp,esp` -> no optimize("y",off). NO SEH prologue (no fs:0) although the
  body has CCharString temps and a CCreatureAction_CastAssassinRush temp with dtors -> TU built without EH unwinding
  (same as the other JackOfBlades rows in rebuild/src/compiled/00/97, which carry `// Flags: none`).
- Speed idioms (/O2, not optimize("s")): `mov ebx,1 ; push ebx` reused as the CCharString-constructed flag (`test bl,1`),
  inlined CCountedPointer(T*) ctor (`push 0Ch ; call new` at 0x97377B), inlined copy ctor (`inc [edi]` 0x9737BA).
- Frame 0x15C: CastAssassinRush temp at esp+0x34 (sizeof 0x138 -> its FinalAction CCountedPointer at esp+0x160/0x164),
  dir at 0x28/0x2C/0x30, distance at 0x18, squares X^2@0x14 Y^2@0x20 Z^2@0x18, sum@0x24, CCharString temp@0x14,
  key 0x4D @0x14, [esp+0x10] = solid (init 0 at 0x9735DD, reused for the rsqrt estimate/r, then solid again 0x9736E8/0x973716,
  read 0x973845).

## 2. Identity (two sources) -- manifest label is RIGHT
- FableWin.pdb (_fablewin_api.tsv 017b68c1 + naming_stragglers/FableWin.exe.signatures.tsv) and Ego_r.pdb
  (egor_pdb_names.tsv 00998690; Ego_r addresses are not retail):
  `?RushToPos@CAIStateGroup_JackOfBladesBase@@IAE?AW4EAIActionReturn@@ABVC3DVector@@@Z`
  -> protected, non-virtual, non-const: `EAIActionReturn CAIStateGroup_JackOfBladesBase::RushToPos(const C3DVector&)`.
- Shared header already declares it: rebuild/include/engine/CAIStateGroup_JackOfBladesBase.h line 24, under
  FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE + FABLE_RETAIL_JACK_STAGE1_UPDATE. Draft returns `long` (wrong type, same bytes).
- Callers: 0x009765B5 and 0x00976C7E (xref.py), both inside CAIStateGroup_JackOfBladesStage1::Update 0x00975BB0 (unlanded).
  No vtable slot (not virtual).

## 3. Callees (all landed)
| addr | landed file | PDB name / landed spelling | draft spelling issue |
|---|---|---|---|
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft says GetPosition -> use GetPos |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound(const ETCInterfaceType&) | draft local CInterfaceMap; use CThing.h GetTC |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | CCharString(const char*, long=-1) / ~CCharString | local model; use fable_string.h / FABLE_RETAIL_CCHARSTRING |
| 0x004C9D60 | 00/4c/CThing_AddTC_004c9d60.cpp | ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z | draft param non-const CTCParamsBase* |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | draft returns CThing* (real: CBaseObject*) |
| 0x00770FA0 | 00/77/CTCSolidToThing_AddThingNotSolidTo_00770fa0.cpp | ?AddThingNotSolidTo@CTCSolidToThing@@QAEXPBVCThing@@@Z | draft takes non-const CThing* |
| 0x00770F20 | 00/77/CTCSolidToThing_RemoveThingNotSolidTo_00770f20.cpp | ?RemoveThingNotSolidTo@CTCSolidToThing@@QAEXPBVCThing@@@Z | same const issue |
| 0x00BFEA1A / 0x00BFE9BC | 00/bf/global_operatornew_00bfea1a.cpp / global_operatordelete_00bfe9bc.cpp | operator new / delete | ok |
| 0x008B7FF0 | 00/8b/CCreatureActionPlayCombatAnimation_..._008b7ff0.cpp | CCreatureAction_PlayCombatAnimation(CThingCreatureBase&, const CCharString&, bool) (header engine/CCreatureAction_PlayCombatAnimation.h:7) | draft takes pointer; real is reference |
| 0x00668610 | 00/66/global_DeleteData_00668610.cpp | CCountedPointer<T>::DeleteData (static __fastcall void(void*); landed as `DeleteData`) | draft invented `CountedDestroy` |
| 0x008CE1B0 | 00/8c/CCreatureActionCastAssassinRush_..._008ce1b0.cpp | ??0CCreatureAction_CastAssassinRush@@QAE@PAVCThingCreatureBase@@ABVC3DVector@@M_NJUCCountedPointer_CCreatureActionBase@@@Z, `ret 1Ch`: (CThingCreatureBase*, const C3DVector& dir, float distance, bool useCollision, long level, CCountedPointer<CCreatureActionBase> finalAction) | draft passes `false,false` -> 5th is `long level` = 0 |
| 0x0062E900 | 00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp | ~CCombatActionBase (virtual in engine/CCombatActionBase.h:130/157; called directly here = end of the inlined derived dtor) | ok |
| 0x0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z (`ret 8`); header engine/CAIStateGroupBase.h:109 under FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION | draft takes pointer + returns long; real is const ref, EAIActionReturn |
Arg order check (0x9737A9..0x9737DB): `push 0` (wait=false, PerformActionAndWait's 2nd arg, evaluated FIRST), then the 8-byte by-value
CCountedPointer copy, then 0 (level), 0 (useCollision), distance, &dir, creature -> ctor on temp esp+0x34, `push eax` -> call 0x97DB70.
Other indirect: `call [eax+4]` with ecx=[eax+8] = CCPPointerInfo::DeleteFunc(Data) (0x9737FD, 0x973867).

## 4. Members
- this+0x04 PBrain (CAIStateGroupBase, CAIBrain*), brain+0x18 Creature: engine/CAIStateGroupBase.h + engine/CAIBrain.h
  (GetCreature(): FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE; PDB returns CThingAICreature&).
  Reloaded every use (`mov eax,[esi+4]; mov eax,[eax+18h]` at 0x9735E1, 0x97369C, 0x9736FE, 0x97375A, 0x9737BC) -> call GetCreature() each time.
- this+0x24 PTargetThing: CIntelligentPointer<CThingCreatureBase> (struct_layouts_egor.tsv offset 36), header
  CAIStateGroup_JackOfBladesBase.h (FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE, needs FABLE_RETAIL_CINTELLIGENTPOINTER).
- creature+0x28 (bit 13) = CThing TC presence bitset word for TCI_SOLID_TO_THING 77 (0x4D); creature+0x44 Components
  (CVectorMap, end at +0x48) -> engine/CThing.h HasTC / GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED).
- target+0x91 bit0 = dead flag read by IS_THING_ALIVE -> engine/ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE), as in sibling 0x00971ED0.
- CastAssassinRush temp: FinalAction CCountedPointer at +0x12C/+0x130 (layout in 008ce1b0 row, byte-packed; header
  engine/CCreatureAction_CastAssassinRush.h exists). CCPPointerInfo {RefCount, DeleteFunc(fastcall), Data} = engine/CCountedPointer.h.
- Locally modelled in draft but shared header exists: C3DVector, CCharString (fable_string.h), CThing, CThingCreatureBase,
  CTCSolidToThing, CBaseIntelligentPointer (CIntelligentPointer.h), CCountedPointer (+CCPPointerInfo), CCreatureActionBase,
  CCreatureAction_PlayCombatAnimation, CCombatActionBase, CCreatureAction_CastAssassinRush, CAIBrain, CAIStateGroupBase,
  CAIStateGroup_JackOfBladesBase, CVectorMap. Verifier warns on all of these.

## 5. Visibility / opt-ins
- GFFastOneOverSqrt: expanded (table 0x013CE558 `GFastOneOverSqrtTable`, 3.0f 0x0122DED4, 0.5f 0x0122F59C); FableWin
  ?GFFastOneOverSqrt@@YIMM@Z in C3DVector.h under FABLE_RETAIL_C3DVECTOR with NEWTON_* paren variants. Retail step
  = fld r; fmul r; fmul sum; fsubr 3.0; fmul r; fmul 0.5 -> default `(3.0f-r*r*value)*r*0.5f`.
- C3DVector Normalise / GetMagnitude variants: FABLE_RETAIL_C3DVECTOR_NORMALISE_{LOCALS_XYZ,LOCALS,SQUARE_HELPER(+GROUPED_SUM),UNPAREN},
  FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF (GFRoot), GFSqr (`FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER`, n-spelling under JACK_STAGE1_UPDATE),
  GetSquaredMagnitude orders (C3DVector.h 196-218). FABLE_RETAIL_JACK_STAGE1_UPDATE is the Jack-TU switch (also in CCombatActionBase.h,
  CThingCreatureBase.h, CCreatureAction_PlayCombatAnimation.h, GFAngles.h, CAIStateGroup_JackOfBladesStage1.h).
- CCountedPointer(T*) ctor is INLINED here (default CCountedPointer.h body: `if (data) Info=new CCPPointerInfo(1,DeleteData,Data); else Info=0`);
  do NOT use FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR (sibling 0x008C2550 calls 0x00695880 out of line; this row does not).
  Destructor inlined: for the temp's member it zeroes Data+Info after release (0x973814/0x97381B) = shared header default dtor.
- CVectorMap::LowerBound, GetPItem, AddTC, Add/RemoveThingNotSolidTo, CCharString, ~CCombatActionBase: called out of line.
- IS_THING_ALIVE expanded inline (3 GetPItem calls: test, +0x91 flag, arg) -> do not use _NOINLINE.

## 6. Siblings
- 0x00971ED0 CAIStateGroupJackOfBladesStage1_GetRushToPosForLunge (00/97): `// Flags: none`; defines FABLE_RETAIL_CAISTATEGROUPBASE,
  _MEMBERS, _GOTOBED_WORLD, FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE, FABLE_RETAIL_CINTELLIGENTPOINTER(+_GET_ORIGINAL_CHAIN,
  _COMPARE_NULL_INT), FABLE_RETAIL_CTHING, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CWORLD, FABLE_RETAIL_IS_THING_ALIVE.
- 0x009718F0 GetIsEnemyInJacksWay, 0x00971B00 IsStillRunnable, 0x00972830 SetupArena (same TU, earlier).
  NOTE 0x00971B00 Leaf says `QAE` but PDB is `?IsStillRunnable@CAIStateGroup_JackOfBladesBase@@IAE_NXZ` (name fix candidate).
- 0x0087E860 CCombatAction_KickThingOnGround::Initialise: FABLE_RETAIL_CCHARSTRING + FABLE_RETAIL_CTHING_GETTC_EXPANDED
  for the HasTC/GetTC/AddTC("CTCSolidToThing") + AddThingNotSolidTo idiom.
- 0x008C2550 CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction: same RUSH_OUTOF follow-up +
  CastAssassinRush(creature, direction, distance, false, 0, finalAction) idiom; uses free `Normalise(const C3DVector&)` model.
- Pending in hand278 (same TU, not landed): 0x00973DC0 SummonPillars, 0x00972990/0x00972CC0/0x00974770 Stage1.

## 7. Old draft model errors vs disassembly
1. Local stand-ins everywhere (see 4) -> must move to shared headers.
2. Return type `long` -> EAIActionReturn; PerformActionAndWait takes `const CCreatureActionBase&`.
3. CastAssassinRush 5th arg is `long level` = 0, not bool.
4. Solid lookup: retail keeps `solid` in MEMORY [esp+10h] (init 0 first instruction after `xor ebx,ebx`; AddTC result stored then
   reloaded `mov edi,[esp+10h]`) and copies key 0x4D into a stack local AFTER the bit test -> matches CThing.h
   GetTC_EXPANDED `GetTC(id,&solid)` (address-taken out pointer), e.g.
   `CTCSolidToThing* solid = 0; if (!creature.GetTC(TCI_SOLID_TO_THING,&solid)) solid=(CTCSolidToThing*)creature.AddTC(CCharString("CTCSolidToThing"),false,0);`
   Draft's `solid = GetTC<>()` returns by value -> register, likely part of the 20-byte deficit.
5. x87: retail computes the three squares ONCE from the fresh difference (Z^2, Y^2, X^2 order, spilled to 0x18/0x20/0x14),
   sums Y^2+Z^2+X^2 into 0x24 for the rsqrt bits, keeps X,Y,Z diff on the x87 stack, scales dir = (X*r,Y*r,Z*r), then RE-SUMS the
   spilled squares (Y^2+Z^2+X^2) and fsqrt for distance (no call to sqrt, intrinsic). I.e. two identical squared-length expressions
   CSE'd at the product level -> e.g. `C3DVector delta = pos - creature.GetPos(); C3DVector dir = delta; dir.Normalise();
   float distance = delta.GetMagnitude();` with a Y,Z,X-order GetSquaredMagnitude / a GFSqr helper variant; try the
   NORMALISE_* / MAGNITUDE_SQRTF opt-ins. Difference computed Z,Y,X (fld [ecx+8] first) = operator- inline.
6. PlayCombatAnimation takes CThingCreatureBase& (GetCreature() re-read), the CCharString("RUSH_OUTOF") temp is conditionally
   destroyed via ebx flag (new-expression arg) -- draft form `new X(GetCreature(), CCharString("RUSH_OUTOF"), true)` is right in shape.
7. Temp dtor: release FinalAction then zero Data AND Info, then call ~CCombatActionBase -- draft only zeroes m_pObject.

## 0x00803D60

# DOSSIER 0x00803D60 CTCTargetingPlayer::UpdateProjectileTargeting (699 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 00803d60 0x2cb)
- __thiscall, no args, plain `ret` (0x0080401A). NO frame pointer (sub esp,0A4h; push ebx/ebp/esi/edi; ebp used as a GPR) -> default /Oy, no optimize("y",off).
- NO SEH prologue (no fs:0) even though an intelligent-pointer temp is destroyed -> the temp dtor runs with no unwind state: either the TU has no /EHsc, or (more likely) every object with a dtor in scope is trivially-nothrow per VC7.1... note: the std::vector local is freed by an inlined `if(start) free(start)` at the end (0x00803FFF..0x00804011) with no EH state.
- Frame 0xA4. Stack map (esp-relative at function body, after the 4 pushes):
  +0x10 dir C3DVector (AccurateNormalise'd, then *= MaxTargetingDistance) | +0x1C first: 2D offset pos-eye (x@1C,y@20); later CThingFilter_In2DRange{Pos@1C,Range@28}; later reused as LOS `to` vector (1C..24)
  +0x2C CGameCameraManager* pcam_man | +0x33 CThingFilter_ViableProjectileAimingTarget (empty, 1 byte) | +0x34 const CThing* hit (=0)
  +0x38 fov float spill, then CThingFilter_IsNot{Thing=ParentThing} | +0x3C unary_negate<CThingFilter_ShotFiredByThing>{Thing=ParentThing}; later the CIntelligentPointer<CThing> temp
  +0x44 C2DBoxI {x-r, y-r, x+r, y+r} | +0x54 std::vector<const CThing*> (3 words zeroed at 0x00803DF7..0x00803E07)
  +0x60 C3DVector int_pos (LOS out #1; copied to TargetedPos) | +0x6C CFilter_Bind5 (5 const& = pointers: &fov@90,&isnot@38,&in2drange@1C,&shotfired@3C,&viable@33)
  +0x80 float& (LOS out, cam_rad) | +0x84 C3DVector normal_vec (LOS out #2) | +0x90 CThingFilter_IsInFOV (0x24 bytes).
- Sequence: GetWorld (0x686A70) -> [+0x34] PCurrentGameCamera -> vslot +0x24 (slot 9 CAIGameCameraBase::GetCameraManager(CGameCameraManager**)) ;
  GetParentThing().GetPos() (0x4C73D0, called before the dir math; ebx keeps the result) ;
  dir = pcam_man->CurrentMode(+0x98, CCountedPointer Data)->LookAt.Position (+0x10) - pcam_man->Camera.WorldPosition (+0x04): Z computed first (right-to-left ctor args, all three on the x87 stack, stored X,Y,Z) ;
  dir.AccurateNormalise() (0xA14440, out of line, result ignored) ; dir *= MaxTargetingDistance(+0x48) (fld once, fmul st1 x3, fstp st0) ;
  2D offset X = pos.X-eye.X, Y = pos.Y-eye.Y (X first, stored to 1C/20, NO Z) ;
  fov = pcam_man->Camera.GetFOV() (0xA0BEB0 with ecx = pcam_man+4) ;
  CThingFilter_IsInFOV(eye, dir, fov, 0.0f, false) ctor 0x6A7150 (ret 14h) into +0x90 ;
  In2DRange: Pos = eye (X,Y via fld, Z via integer copy), Range = sqrt(Y*Y + X*X) (retail loads Y*Y first: fld [20];fmul [20];fld [1C];fmul [1C];faddp;fsqrt) -- see C2DVector macros (FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX) ;
  ftol x3 (0xBFEA70): eye.X, eye.Y, MaxTargetingDistance -> box (x-r,y-r,x+r,y+r) ;
  PeekThingSearchTools() (0x686D90, ecx=this) -> GetAllThingsInRoughArea (0x80A550; push &vec, &bind5, &box) ;
  TargetedPos(+0x84) = dir + eye (X first: fld dir.X; fadd eye.X ...) via a temp then 3-word copy ; to = dir + eye recomputed (same order) into +0x1C ;
  pcam_man->CamTools(+0x78).IsLineOfSight(vec, eye, to, int_pos, normal_vec, cam_rad, &hit, TEST_ALL(0xF), false) (0x877840) ;
  if (LOS true) goto end ; if (!fov(int_pos)) goto end (0x6A7270) ;
  if (hit && hit->IsAlive() [test byte [hit+91h],1 = Dead] && PProjectileTargetedThing.GetPItem() != hit [0xA01B50 out of line] && hit->HasTC(TCI_TARGETED) [test dword [hit+20h] bit 9])
      PProjectileTargetedThing = CIntelligentPointer<CThing>(hit)  [ctor 0x4A96F0, operator= 0x4A9760, dtor 0xA01C10 all out of line] ;
  TargetedPos = int_pos (inside the fov-true arm) ; end: vector free.

## 2. Name / signature (two+ sources agree)
- Ego_r.pdb 0x007FF010 and FableWin.pdb 0x024CF520: `?UpdateProjectileTargeting@CTCTargetingPlayer@@IAEXXZ` (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, _fablewin_api.tsv); BSim port audit maps retail 0x803d60 to it (ghidra_out/bsim_port_audit_final.tsv, 0.87). Manifest label is CORRECT.
- Signature: `protected: void CTCTargetingPlayer::UpdateProjectileTargeting()` -- non-const, non-virtual, no params.
- Mangled leaf: ?UpdateProjectileTargeting@CTCTargetingPlayer@@IAEXXZ
- Only caller: 0x0080580D inside CTCTargetingPlayer::FrameUpdate (retail 0x00805600, NOT landed).
- PDB order (FableWin): UpdateProjectileTargeting 0x24CF520 < UpdateTargetList 0x24CFE90; anon-namespace filter ctors ShotFiredByThing 0x24CFDE0, ViableProjectileAimingTarget 0x24CFE00 in same TU.

## 3. Callees
| retail | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | CTCBase::GetWorld() const -> CWorld& | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ | draft returns CWorld* and accesses raw +0x34 -- WRONG; use GetWorld().GetGameCamera() (CWorld.h:217) |
| vslot +0x24 | CAIGameCameraBase.h:37 slot 9 0x006970C0 | CAIGameCameraBase::GetCameraManager(CGameCameraManager**) | bool return | draft "CCameraManager::GetCurrentCamera(CCamera**)" -- WRONG class/name/out type |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft "GetPosition" -- WRONG |
| 0x00A14440 | 00/a1/C3DVector_AccurateNormalise_00a14440.cpp | C3DVector::AccurateNormalise() returns C3DVector& | ?AccurateNormalise@C3DVector@@QAEAAV1@XZ | draft "Normalise" -- WRONG (Normalise is a different inline) |
| 0x00A0BEB0 | 00/a0/CCamera_GetFOV_00a0beb0.cpp | CCamera::GetFOV() const | ?GetFOV@CCamera@@QBEMXZ | ok name; but called on pcam_man->Camera (+4), not on the manager |
| 0x006A7150 | 00/6a/CThingFilterIsInFOV_CThingFilterIsInFOV_006a7150.cpp | CThingFilter_IsInFOV(const C3DVector& fromPos,const C3DVector& direction,float fov,float radius,bool angleTest3D) | ok |
| 0x006A7270 | 00/6a/CThingFilterIsInFOV_operator_006a7270.cpp | operator()(const C3DVector&) const | ??RCThingFilter_IsInFOV@@QBE_NABVC3DVector@@@Z | draft takes C3DVector* non-const -- WRONG |
| 0x00BFEA70 | 00/bf/global_ftol2_00bfea70.cpp | _ftol2 (compiler helper from (long) casts) | - |
| 0x00686D90 | NOT landed (body `mov ecx,[0x013B8A1C]; jmp 0x0051E480`) | labels: CTCBase::PeekThingSearchTools | ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ (Ego_r 0x6B2180 / FableWin 0x21915D0, CTCBase.h:155-158) | draft "GetThingSearchTools" -- WRONG (that is 0x00686D80) |
| 0x0080A550 | 00/80/GetAllThingsInRoughArea...CThingFilterIsAffectedByExplosions_0080a550.cpp (file name/manifest label propagated & wrong; body correct) | CThingSearchTools::GetAllThingsInRoughArea | Ego_r 0x00803C50 / FableWin 0x024D7AF0: ??$GetAllThingsInRoughArea@$$CBVCThing@@V?$vector@PBVCThing@@...@std@@V?$CFilter_Bind5@VCThingFilter_IsInFOV@@VCThingFilter_IsNot@@V?$unary_negate@VCThingFilter_In2DRange@@@std@@V?$unary_negate@VCThingFilter_ShotFiredByThing@?A0x58ed6966@@@4@VCThingFilter_ViableProjectileAimingTarget@?A0x58ed6966@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV...@AAV?$vector@PBVCThing@@...@Z | draft "FindCreatures" with CThingCreatureBase list -- WRONG: template <const CThing, std::vector<const CThing*>, Bind5<...>>, const member |
| 0x00877840 | NOT landed; declared NCameraModes_CTools.h:52 | NCameraModes::CTools::IsLineOfSight(const std::vector<const CThing*>&, const C3DVector& from, const C3DVector& to, C3DVector& int_pos, C3DVector& normal, float&, const CThing**, ELOSTest, bool) const | Ego_r ?IsLineOfSight@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@...ABVC3DVector@@1AAV5@2AAMPAPBVCThing@@W4ELOSTest@12@_N@Z | draft args WRONG (passed &fov as arg 5; real arg 5 = C3DVector& normal, arg 6 = float&) |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | ok |
| 0x004A96F0 | 00/4a/CIntelligentPointer_CThing_CIntelligentPointer_004a96f0.cpp | CIntelligentPointer<CThing>::CIntelligentPointer(T*) | ok |
| 0x004A9760 | 00/4a/CIntelligentPointerCThingCreatureBase_operator_004a9760.cpp (file name says CThingCreatureBase; body/comment = CIntelligentPointer<CThing>::operator=) | operator=(const CIntelligentPointer&) | ok |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ??1CBaseIntelligentPointer@@UAE@XZ (virtual dtor, called directly on the temp) | ok |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free (STLport vector dealloc, inlined ~vector) | ok |
Possible NAME_FIX (not this row): the 0x80A550 row names 0x00809210 as CThingFilter_ShotFiredByThing::operator(), but 0x00809210 is landed as CThingFilter_NoiseCameFromThing_operator_00809210.cpp; Ego_r has ShotFiredByThing op() at 0x00802AB0 (anon ns ?A0x58ed6966, this TU) vs NoiseCameFromThing op() at 0x005A3FD0 -> 0x00809210 is probably ShotFiredByThing (verify). Also 0x008077B0 leaf lacks the `?A0x58ed6966` anon namespace.

## 4. Members (this = CTCTargetingPlayer; header rebuild/include/engine/CTCTargetingPlayer.h)
- +0x04 ParentThing (CTCBase) -> use GetParentThing() / CTCBase.h (FABLE_RETAIL_CTCBASE).
- +0x48 MaxTargetingDistance float (header line 50; PDB +72) -- draft called it "ProjectileRange": WRONG name.
- +0x84 TargetedPos C3DVector (line 61; PDB +140, retail -8 because Targets/RejectedTargetPoints are 12-byte STLport vectors).
- +0xDC PProjectileTargetedThing CIntelligentPointer<CThing> (PDB +228). Typed under FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST (line 71) or _VSLOTS3 (line 82); otherwise unsigned char[8].
- Header macros: FABLE_RETAIL_CTCTARGETINGPLAYER, _UPDATE_LIST, _VSLOTS, _VSLOTS3, _MELEE, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_DEF_CLASS_BASE.
- CWorld +0x34 PCurrentGameCamera: CWorld.h:217 GetGameCamera() (FABLE_RETAIL_CWORLD).
- CGameCameraManager (CGameCameraManager.h): Camera CCamera +0x04 (WorldPosition +0 inside, GetFOV), CamTools NCameraModes::CTools +0x78 (GetCameraTools(), line 80/220), CurrentMode CCountedPointer<NCameraModes::CBase> +0x98; NCameraModes::CBase (NCameraModes_CBase.h) LookAt CPhysicsState +0x10 (typed under FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES), CPhysicsState.Position +0. Macros: FABLE_RETAIL_CGAMECAMERAMANAGER*, FABLE_RETAIL_CAIGAMECAMERABASE, FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND (used by sibling 0x804020).
- CThing (CThing.h): +0x20 UsingComponents bitset, bit 9 = TCI_TARGETED -> HasTC(TCI_TARGETED) (line 298/309); +0x91 bit0 Dead (line 88) -> IsAlive().
- Filters: CThingFilter_IsInFOV {FromPos,Direction,CosFOV,FOV,AngleTest3D} 0x24 (CThingFilter_IsInFOV.h, macro UPDATE_LIST); CThingFilter_IsNot{CThing* Thing} (CThingFilter_IsNot.h); CThingFilter_In2DRange{C3DVector Pos; float Range} (CThingFilter_In2DRange.h); CThingFilter_ShotFiredByThing{CThing* Thing} and CThingFilter_ViableProjectileAimingTarget{} are anonymous-namespace classes of THIS TU (no header). CFilter_Bind5 + GFFilter_Bind5 (FASTCALL) in engine/TargetingPlayerSupport.h (stores const& refs = the 5 pointers retail builds).
- Classes the old draft models LOCALLY that have shared headers: C3DVector, C2DBoxI, CThing, CThingFilter_IsInFOV, CThingFilter_IsNot, CThingFilter_In2DRange, CFilter_Bind5 (TargetingPlayerSupport.h), CThingSearchTools, NCameraModes::CTools, CCamera, CGameCameraManager/CAIGameCameraBase (draft: CCameraManager), CWorld, CBaseIntelligentPointer/CIntelligentPointer, CTCTargetingPlayer, plus CRetailVector (use std::vector from stlport).

## 5. Callee-body visibility
- Out of line in retail although inline-able: CThing::GetPos (0x4C73D0), C3DVector::AccurateNormalise, CCamera::GetFOV, CTCBase::GetWorld/PeekThingSearchTools, GetPItem, CIntelligentPointer ctor/operator= (FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE / _ASSIGN_OUTOFLINE / _ASSIGN_ORIGINAL_CHAIN, _COMPARE_RAW_NOT_EQUAL exist in CIntelligentPointer.h), IsInFOV ctor & operator(), GetAllThingsInRoughArea (template instantiated out of line; FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA / _ROUGHAREABODY / _ALLTHINGSBODY in CThingSearchTools.h -- sibling uses ORIGINAL_ROUGHAREA).
- Inlined in retail: CWorld::GetGameCamera (raw [eax+34h]), C3DVector operators -,+,*=, In2DRange/IsNot/ShotFired/Viable ctors, not1, GFFilter_Bind5 + CFilter_Bind5 ctor (pointer stores), std::vector ctor (3 zero stores) and dtor (`if(p) free(p)` -> FABLE_RETAIL_STLP_VECTOR_C used by sibling), CThing::IsAlive, HasTC.
- The vector dtor is inlined but no SEH: copy sibling 0x804020's setup (it has std::vector locals too) and check its EH flags.

## 6. Siblings
- rebuild/src/compiled/00/80/CTCTargetingPlayer_UpdateTargetList_00804020.cpp (immediate next function, same TU, RELOCATION_MATCH): `// Flags: /d2inlT68`; macro set incl. FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST, _CTCTARGETINGPLAYER, _CTCBASE, _C3DVECTOR(_PACKED,_SUBTRACT_ASSIGN), _CTHING, _CINTELLIGENTPOINTER(+_COPY,_ASSIGN,_COMPARE_RAW_NOT_EQUAL,_EQUAL_RAW, *_ORIGINAL_CHAIN), _CWORLD, _CAIGAMECAMERABASE, _CGAMECAMERAMANAGER_LOOKAROUND, _CTHINGSEARCHTOOLS(_ORIGINAL_ROUGHAREA), _STLP_VECTOR_C, _FILTER_BIND3_FUNCTION; includes engine/TargetingPlayerSupport.h, CGameCameraManager.h, CThingSearchTools.h, C2DBoxI.h. Idioms to copy verbatim: `CGameCameraManager* pcam_man; GetWorld().GetGameCamera()->GetCameraManager(&pcam_man);`, `pcam_man->GetCameraTools().IsLineOfSight(..., int_pos, normal_vec, cam_rad, &phit_thing, NCameraModes::CTools::TEST_..., bool)`, `GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area, GFFilter_Bind5(...), list)` (here: PeekThingSearchTools, <const CThing>, vector<const CThing*>).
- 0x00802420 GetTargetedPos (FABLE_RETAIL_CTHING, _CTHING_PEEKPLAYER), 0x008039A0 IsTargetedThingOfTypeInRange (auto_inline off/on), 0x00803B20 PostProcessTargetedThing (plain), 0x00802610 GetPTargetedThing (pack push 1).
- 0x0080A550 (the callee) documents the exact Bind5 operator order: F0(t)&&F1(t)&&!F2(t)&&!F3(t)&&F4(t).

## 7. Old draft model errors
- Wrong camera model: treated vslot-0x24 result as a CCamera with Position at +4, FOV getter on the whole object; really CGameCameraManager* (Camera member at +4, CamTools +0x78, CurrentMode +0x98 -> CBase::LookAt.Position at +0x10).
- Wrong filter set: retail is Bind5<IsInFOV, IsNot(parent), not1(In2DRange(eye, 2D dist)), not1(ShotFiredByThing(parent)), ViableProjectileAimingTarget()>; draft had IsNot twice + "Empty", no negations.
- Wrong search: GetAllThingsInRoughArea<const CThing> into std::vector<const CThing*> via PeekThingSearchTools (const); draft used CThingCreatureBase list + GetThingSearchTools.
- Wrong LOS args: (vec, eye, to, int_pos, normal_vec, cam_rad, &hit, TEST_ALL=0xF, false); draft passed &fov and hitPos, CThing** non-const.
- Hit test: hit is `const CThing*`; retail order hit!=0, IsAlive (Dead bit +0x91), PProjectileTargetedThing.GetPItem()!=hit, HasTC(TCI_TARGETED) (+0x20 bit 9 -- draft read m_presentBits).
- TargetedPos is assigned twice: `TargetedPos = dir + eye` before the LOS (via temp), and `TargetedPos = int_pos` only when LOS is blocked/false AND fov(int_pos) is true.
- Retail adds as `dir + eye` (dir loaded first) both times; subtract `LookAt.Position - eye` with Z first (ordinary C3DVector(x,y,z) ctor arg order).
- Range = sqrt of the 2D offset, Y*Y loaded first (C2DVector macros / FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX); the offset is X,Y only (C2DVector or two floats), not pos.Z.
- Frame 0xBC vs 0xA4 in draft: caused by separate locals (notMe twice, Empty, by-value range etc.) and a separate `end` + `hitPos`; retail overlaps slots (offset/In2DRange/to share +0x1C; fov spill/IsNot share +0x38; ShotFired/intelligent-pointer temp share +0x3C) -> scope temporaries inside the call expression (filters as temporaries in GFFilter_Bind5(...) call), as sibling 0x804020 does.

## 0x006D8F10 (not landed)

# DOSSIER 0x006D8F10 -- CHeroExperienceDef::Transfer (692 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 006d8f10 0x2c4)
- __thiscall virtual, `ret 4` (one arg: CPersistContext&). No frame pointer, no SEH, no `sub esp`. Prologue push ebx/esi/edi;
  esi = context (arg), edi = this. **ebp is shrink-wrapped**: `push ebp` at 0x6D9064 (just before the 8th transfer, the
  first StatIncreasePerHitType) and `pop ebp` at 0x6D91A3 (inside the last transfer, between `sub eax,2` and `mov edx,edi`).
  Two epilogues (pop edi/esi/ebx ; ret 4) -- one per arm of the final vector transfer (tail duplicated).
- 14 inline transfers, each `push 0x0122D70E ("") ; mov ecx,esi ; lea ebx,[edi+off] ; call CheckCRC 0x404500`, then
  `mov eax,[esi+0x18] ; sub eax,2`:
  - vectors (+0x28,+0x34,+0x40 vector<long>; +0xa8 vector<CAttackHistoryCombo>): `mov edx,ebx` BEFORE `je` (common to both
    arms), je -> In(ecx=[esi+0x24]) else Out(ecx=[esi+0x28]) => two-way `Mode==2 ? In : Out` (TransferVector body).
  - maps and floats: `je load ; dec eax ; jne done` => switch {case 2 load; case 3 save}; edx loaded inside each arm.
  - floats (+0x64,+0x68): save = `mov eax,[ebx] ; mov ecx,[esi+0x28] ; push eax ; call WriteFloat 0x993EE0` (integer copy of
    the float, no x87); load = `push ebx(&value) ; push [esi+0x24](stream) ; lea ecx,[esp+0x18] ; call 0x40EFB0` -- the
    CPersistTraits<float> traits temporary lives in the dead `context` argument slot.
- Transfer order: Thresholds +28, HeroLevelTitles +34, FallbackMultipliers +40, SoundIntensityThresholds +4c, ComboMultiplierMap +58,
  float +64, float +68, StatIncreasePerHitType +9c (ebp), ComboMultiplierIncrease +6c, Lock +78, Lost +84,
  RunningHitsIncrease +90, StatIncreasePerHitType AGAIN (reuses ebp, 0x6D915E: no lea), AttackHistoryCombos +a8 (`add edi,0xa8`).
  The double transfer of +0x9c is real (same register, same address); keep it.
- Size idioms: none of the `optimize("s")` kind (per-member `lea ebx,[edi+off]` reused across arms) -- plain /O2. No /Oy-.

## 2. Name / signature (manifest label `CHeroExperienceDef::CHeroExperienceDef::Transfer` has a doubled class; real name below)
- Vtable: rebuild/manifest/rtti-vtables.tsv `CHeroExperienceDef 0124390c 18 006d8f10`; xref.py: only reference is the .rdata
  slot 0x01243954 (= 0x0124390C + 18*4). CDefClassBase.h: slot 18 = `virtual void Transfer(CPersistContext&) = 0`.
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0058a900) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 017a57c4/01ef6b20,
  naming_stragglers FableWin.exe.signatures.tsv 01ef6b20, 317 bytes): `?Transfer@CHeroExperienceDef@@UAEXAAVCPersistContext@@@Z`
  => `public: virtual void __thiscall CHeroExperienceDef::Transfer(class CPersistContext &)`. Two sources + vtable slot.

## 3. Callees (landed file / landed name / PDB name)
| addr | landed file | PDB name (egor_pdb_names) | draft spelling |
|---|---|---|---|
| 0x00404500 | 00/40/CPersistContext_CheckCRC_00404500.cpp | CPersistContext::CheckCRC(const char*) | ok |
| 0x00466A75 | 00/46/global_GFSerialiseVectorBinaryOutfloat_00466a75.cpp (shared <float>/<long> folded body) | ??$GFSerialiseVectorBinaryOut@M@@YIXAAVCDataOutputStream@@AAV?$vector@M...@Z (no <J> Out in PDB: COMDAT-folded) | draft `GFSerialiseVectorBinaryOutLong(CDataOutputStream*,CLongVector*)` WRONG: template GFSerialiseVectorBinaryOut<long>(CDataOutputStream&, vector<long>&) |
| 0x00466AA7 | 00/46/global_GFSerialiseVectorBinaryIn_00466aa7.cpp, Leaf ??$GFSerialiseVectorBinaryIn@J@@YIXAAVCMemoryDataInputStream@@AAV?$vector@JV?$allocator@J@std@@@std@@@Z | same | draft `...InLong(ptr,ptr)` WRONG (refs, template) |
| 0x006DAE40 | 00/6d/global_GFSerialiseMapBinaryOut_006dae40.cpp | ??$GFSerialiseMapBinaryOut@VCDefString@@JU?$less@J@std@@@@YIXAAVCDataOutputStream@@ABV?$map@JVCDefString@@...@Z (map by CONST ref) | draft `WriteDefStringMap` WRONG |
| 0x006DBA50 | 00/6d/global_GFSerialiseMapBinaryIn_006dba50.cpp | ??$GFSerialiseMapBinaryIn@VCDefString@@JU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@JVCDefString@@...@Z | draft `ReadDefStringMap` WRONG |
| 0x006DC350 | 00/6d/global_GFSerialiseMapBinaryOut_006dc350.cpp | ??$GFSerialiseMapBinaryOut@VCComboMultiplierData@@JU?$less@J@std@@@@YIXAAVCDataOutputStream@@ABV?$map@... | draft `WriteComboMultiplierMap` WRONG |
| 0x006DCFA0 | 00/6d/global_GFSerialiseMapBinaryIn_006dcfa0.cpp | ??$GFSerialiseMapBinaryIn@VCComboMultiplierData@@JU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@... | draft `ReadComboMultiplierMap` WRONG |
| 0x006DC450 | 00/6d/GFSerialiseMapBinaryOutCHeroStatIncreaseDataEDamageAttribute_WriteToStream_006dc450.cpp (file says "Actual GFSerialiseMapBinaryOut<CHeroStatIncreaseData,EDamageAttribute>, not manifest WriteToStream") | ??$GFSerialiseMapBinaryOut@VCHeroStatIncreaseData@@W4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCDataOutputStream@@ABV?$map@... | draft `WriteStatIncreaseMap` WRONG; landed filename leaf also wrong (NAME_FIXES candidate) |
| 0x006DD1C0 | 00/6d/CHeroExperienceDef_ReadFromStream_006dd1c0.cpp (Leaf: ReadFromStream -- WRONG) | ??$GFSerialiseMapBinaryIn@VCHeroStatIncreaseData@@W4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@... | draft `ReadStatIncreaseMap` WRONG; landed name wrong too (NAME_FIXES: CHeroExperienceDef::ReadFromStream -> GFSerialiseMapBinaryIn<CHeroStatIncreaseData,EDamageAttribute,less<EDamageAttribute>>) |
| 0x006DC550 | 00/6d/global_GFSerialiseVectorBinaryOutCAttackHistoryCombo_006dc550.cpp | ??$GFSerialiseVectorBinaryOut@VCAttackHistoryCombo@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCAttackHistoryCombo@@...@Z | draft `...OutCombo(ptr,ptr)` WRONG |
| 0x006DD580 | 00/6d/global_GFSerialiseVectorBinaryInCAttackHistoryCombo_006dd580.cpp | ??$GFSerialiseVectorBinaryIn@VCAttackHistoryCombo@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@... | draft `...InCombo` WRONG |
| 0x00993EE0 | 00/99/CDataOutputStream_WriteFloat_00993ee0.cpp | ?WriteFloat@CDataOutputStream@@QAEXM@Z | ok |
| 0x0040EFB0 | 00/40/CPersistTraitsfloat_TransferBinaryIn_0040efb0.cpp | ?TransferBinaryIn@?$CPersistTraits@M@@QAEXAAVCMemoryDataInputStream@@AAM@Z (refs!) | draft takes pointers (stream*, float*) -- PDB is (CMemoryDataInputStream&, float&) |
All GFSerialise* are `__fastcall` (YI) free function templates taking references (ecx=stream, edx=container).

### PDB names of the INLINE transfer templates the source called (egor_pdb_names.tsv) -- retail inlined all of them:
- `??$TransferVector@J@CPersistContext@@QAEXPBDAAV?$vector@J...@Z` and `??$TransferVector@VCAttackHistoryCombo@@@CPersistContext@@QAEXPBDAAV?$vector@...@Z`
  => `context.TransferVector("", vec)` (named 2-arg), not `Transfer`.
- `??$TransferMap@VCDefString@@J@CPersistContext@@QAEXPBDAAV?$map@...@ABVCDefString@@@Z`, same for CComboMultiplierData and
  CHeroStatIncreaseData,EDamageAttribute: ONLY the 3-arg form (name, map&, const V& default) exists for these V -> source was
  `TransferMap(name, map, default)` or an inline 2-arg wrapper forwarding a default (cf. CPersistContext.h NAMED_DEFAULTS
  `Transfer(name,value){Transfer(name,value,GetDefaultVal<T>());}`).
- `??$Transfer@M@CPersistContext@@QAEXPBDAAM@Z` and `...AAMABM@Z` (float, 2- and 3-arg).
- Draft uses `context.Transfer("", x)` overloads for everything and local CPersistTraits<map> specialisations: wrong spellings.

## 4. Members (Ego_r struct_layouts_egor.tsv offsets are PDB +0x10-per-container; retail re-based)
Shared header: rebuild/include/engine/CHeroExperienceDef.h (EXISTS -- the draft's "No generated engine header" is stale).
`class CHeroExperienceDef : public CSubDefClassBase` (CSubDefClassBase.h needs `#define FABLE_RETAIL_DEF_CLASS_BASE`; base 0x25
packed -> first member at 0x28), sizeof 0xb4. Members: ExperienceLevelThresholds +28, HeroLevelTitles +34, FallbackMultipliers +40
(vector<long>), SoundIntensityThresholds +4c (map<long,CDefString>), ComboMultiplierMap +58 (map<long,CComboMultiplierData>),
ExperienceMultiplierParamPerUnitComboMultiplier +64, ExperienceMultiplierRegionChangeMultiplier +68 (float),
ComboMultiplierIncrease +6c, ComboMultiplierLock +78, ComboMultiplierLost +84, ComboMultiplierRunningHitsIncrease +90
(map<long,CDefString>), StatIncreasePerHitType +9c (map<EDamageAttribute,CHeroStatIncreaseData>), AttackHistoryCombos +a8
(vector<CAttackHistoryCombo>). Names match the draft's.
- Header models all containers as `unsigned char X[12]` except opt-in FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT which makes
  StatIncreasePerHitType a real std::map<EDamageAttribute,CHeroStatIncreaseData> (includes CHeroStatIncreaseData.h,
  EDamageAttribute.h). No opt-in for real vector/map types of the other members, and NO Transfer declaration in the
  header: the author must add an opt-in (e.g. FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER declaring `virtual void Transfer(CPersistContext&)`
  and real STLport member types) -- not a local struct.
- CPersistContext: rebuild/include/engine/CPersistContext.h (opt-in FABLE_RETAIL_CPERSISTCONTEXT; fields Mode +0x18 (EMode,
  MODE_LOAD_BINARY=2, MODE_SAVE_BINARY=3), PLoadStream +0x24, PSaveStream +0x28; CheckCRC(const char*)). Opt-ins relevant:
  FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY (bodies of one-arg Transfer<T>(T&) [traits local, switch 2/3] and
  TransferVector<T>(vector&) [2 / default] -- exactly retail's arm shapes, but unnamed one-arg forms; no landed user yet),
  FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (named Transfer -> 3-arg; declares 3-arg TransferMap, declaration only),
  FABLE_RETAIL_GFSERIALISEMAPBINARY_DECL (map serialiser declarations; In body under FABLE_RETAIL_GFSERIALISEMAPBINARYIN in
  engine/CPersistTraits.h). Streams: FABLE_RETAIL_CMEMORYDATAINPUTSTREAM / CDATAINPUTSTREAM / CDATAOUTPUTSTREAM.
- Classes the draft models LOCALLY that have shared headers: CPersistContext, CHeroExperienceDef (+ base CSubDefClassBase /
  CDefClassBase), CDataOutputStream (engine/CDataOutputStream.h), CMemoryDataInputStream, CPersistTraits (engine/CPersistTraits.h),
  CHeroStatIncreaseData; the fake CLongVector / CDefStringMap / ... should be std::vector / std::map (stlport/fable_containers.h).

## 5. Callee visibility
- Every callee is out of line in retail (calls, no inlined bodies). Template bodies GFSerialise*<...> sit right after this
  function (0x6DAE40..0x6DD580, same TU COMDATs) -- they are instantiated in this TU, so their bodies are visible; they stay
  called because they're big. CheckCRC (0x404500), WriteFloat (0x993EE0), CPersistTraits<float>::TransferBinaryIn (0x40EFB0)
  live in other TUs (low addresses / CDataOutputStream module).
- TU = CTCHeroExperience's .cpp: neighbours 0x6D7120..0x6D8E30 are CTCHeroExperience members (OnSerialise 0x6D8E30 precedes),
  plus CDefClassBase::GetSubDef<CHeroExperienceDef> 0x6DAA90 and the CPersistTraits<CAttackHistoryCombo/CComboMultiplierData/
  CHeroStatIncreaseData> TransferBinaryIn/Out rows (0x6D9EE0, 0x6DB060, 0x6DB0F0, 0x6DBE00, 0x6DC070, 0x6DC6A0) after it.
- Per GOTCHAS Q24: visible same-TU noinline callee bodies taking a container by reference tell c2 the address does not
  escape -- declaring the GFSerialise* templates with bodies (or not) may change register/save decisions; worth a try for the ebp
  shrink-wrap.

## 6. Siblings (idioms)
- rebuild/src/compiled/00/74/CCreatureGeneratorDef_Transfer_0074c5f0.cpp -- the draft's template: local CPersistContext with
  inline `Transfer(name,T&)` switch 2/3 using `CPersistTraits<T>().TransferBinaryIn(...)` temp, vector overload 2/default. No pragmas.
- rebuild/src/compiled/00/47/CHeroStatsDef_Transfer_00479cba.cpp -- `#pragma optimize("s", on)`, local models, one-arg Transfer.
- rebuild/src/compiled/00/46/CArmourDef_Transfer_004615fb.cpp -- `#pragma optimize("s", on)`, `#define FABLE_RETAIL_DEF_CLASS_BASE`,
  `#include "engine/CParentDefClassBase.h"`, `context.TransferMap(member)` (out-of-line there).
- rebuild/src/compiled/00/6d/CDefClassBase_GetSubDefCHeroExperienceDef_006daa90.cpp -- same TU; `#define FABLE_RETAIL_DEF_CLASS_BASE`,
  engine/CSubDefClassBase.h, engine/CDefPointer.h.
- Shrink-wrap precedents (comment text): 00/84/CCreatureActionDrink_FinishAction_00847c90.cpp (goto shared tail),
  00/54/NUISystemCTextSlider_Initialise_00549230.cpp ("flattened push_back shrink-wraps the ebp push").

## 7. Old draft's likely model errors
- Only diff (693v692) is ebp saved in the prologue instead of around transfers 8..13. Ideas not yet tried (NOTES tried only the
  CCreatureGenerator template and ordering): (a) PDB-true spellings -- TransferMap(name, map, default) 3-arg with a real default
  (GetDefaultVal / static), TransferVector(name, vec), Transfer<float>(name, f) -- the extra (dead) default-value argument and real
  std::map/std::vector types change c2's flow graph; (b) the float load traits temp: retail homes it in the dead arg slot (the
  draft's `CPersistTraits<T>()` temp reproduces that already); (c) visible GFSerialise* bodies (Q24); (d) the second +0x9c
  transfer is a real source line -- keep it.
- Wrong callee spellings / pointer-vs-reference parameter types (all GFSerialise* take refs; CPersistTraits<float>::TransferBinaryIn
  takes (CMemoryDataInputStream&, float&)); local structs instead of shared headers; header comment "no engine header" stale.
- Control flow and member order in the draft are already right (vectors 2/default, maps+floats 2/3).

