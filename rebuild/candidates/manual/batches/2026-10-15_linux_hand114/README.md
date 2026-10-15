# Hand chunk 114 (CUIMiniMapMarker .. CV_FisticuffsClubScript): 37 of 40 byte-exact

## What matched (37)
- **CUIMiniMapMarker** 00613510 ctor: PDB-typed init list (Region, UIDefName("", -1), Position(0,0), DefNameLength, GraphicIndex, Angle); float zeros share the integer zero register.
- **CUIState** 0052e5d0 ctor (NUISystem::CUIState): every member is a BODY assignment in retail store order (Zoom, Position, Colour R/G/B/A, UpdateTime, StateChangeType, StateChangeFlag=7, LinearChange) with empty default ctors on C2DVector/CRGBColour; init-list or (x,y)-ctor forms put Position before Zoom.
- **CUndeadRisingZombieAttackerBoss** 00e0d640 Init: virtual float call (`fstp` into +0x28) then a POD 3-float copy from the target's slot-6 reference getter.
- **CUserProfileManager** (11 rows, 10 matched):
  - 00406820 ConvertFloatToVideoOptionQuality: four independent `if (v >= K) q = n;` (the dead `>= 0.0f` test survives as `jne +0`).
  - 00406890 GetMaxRefreshRate: the known-maximum case is the `if` body so `return 3` lands last; `best == -1 ? 0 : best` = `sete ; dec ; and`.
  - 0040b690 SetAntialiasing (really the resolution setter +0x48/+0x104) and 0040d2f0 SetCameraBowYZInverted (two inlined single-flag setters): new-profile copy under UseNewProfileValues, live value, ApplyValues(), optional Save().
  - 0040d2a0 CTBaseSingleton<CUserProfileManager>::Get: `if (!holder.object) holder = new T; return holder.object;` (the null-checked new duplicates the assignment).
  - 0040d400 SetProfileName: if/else Load/Save with duplicated `pop esi ; ret 8` epilogues.
  - 0041a0c0 / 0041a4c0: static `__fastcall` CWideString getters (hidden result in ecx), the second `return GetDocumentsFolder() + L"My Games\\Fable\\";` through the `__fastcall` operator+.
  - 007d7c10: `__thiscall` by-value `CWideString(L"")` return (hidden pointer on the stack, `ret 4`).
  - 00913960: STLport `deque<T(0x30)>::back()` -- `iterator tmp = _M_finish; --tmp; return *tmp;` with the IMPLICIT iterator copy (a user memberwise copy constructor drops retail's 0x10-byte frame).
- **Quest-script constructors** (00d12cb0, 00d7c700, 00e00690, 00e2ab60, 00e32350, 00e46bd0): the landed 0x00E68F70 model (CScriptBase() + `__declspec(novtable)` intermediate base storing interface +0x40 / owner +0x44 before the vtable); `CTimer() : Index(g_pGameScriptInterface->RegisterTimer())` member constructors; CCharString arrays as inline counted loops; 00e46bd0 needed the real STLport vector (`_Vector_base(const allocator&)` + `_STLP_alloc_proxy` end-of-storage) and the CScriptThing PImp counted pointer as a member OBJECT with its own inline constructor.
- **Script watch / wait bodies** (00d66e90, 00e50100, 00ea3490, 00e7a580, 00e2ba50): the WAIT macro `while (!(cond)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` + trailing terminate test; 00e2ba50 is `while (!terminating) { if (cond) { check; QuestFinished = true; check; return; } NewScriptFrame(); }` with the last check as a tail jump.
- **CV_AmbushScamScript** 00e1feb0 ResetRandomSpeechTime: def pointer read into a local BEFORE rand(), the maximum re-read through the global, the difference in a named local; 00e2bac0/00e2bb10 the good/evil ending rewards (morality float pushed via integer moves, Game re-read per call).
- **CV_ArcheryCompetitionScript** 00e32ef0 RemoveArcheryQuestInfo / 00e33000 UpdateHighScore: plain GSI virtual calls (slots 321/338, 335/725), `(float)score` = `fild` from the argument slot.
- **CV_AssassinAttacksScript** 00e37cf0 (counted `for` with per-arm terminate check, `*(begin()+i)`), 00e38290 (by-value CCharString parameter, `!(name != region)` break, callee-destroyed argument).
- **CV_BanditTollScript** 00b2fa40 (engine-module record reset): STLport `vector<unsigned char>::erase(begin(), end())` inlining `__copy_trivial` -- `last == first ? result : memmove(...) + n` keeps retail's `cmp eax,eax` and the dead memmove arm.
- **CV_BodyGuardScript** 00e5d1f0 Init, **CV_ExposeMayorScript** 00e75680 Init: statement order = store order around one call (SetTimer / CCharString operator=).
- **CV_BordelloScript** 00c12040 (engine module): `if (pending && timer.HasElapsed()) { handler->Handle(&req); pending = false; timer.Reset(); }` (tail jump).
- **CV_ChapelOfEvilScript** 00e6da40 (`TortureType = TortureCount++` / `rand() % 7`), **CV_FisticuffsClubScript** 00e86e10 (nested ifs, `inc dword ptr`).

## Parked for Fable 5.1 (3)
- 00406d90 CUserProfileManager::ClampResolution DIFFER(89v84): retail width walk is un-rotated (`cmp [eax],esi ; jle ; dec edx ; sub eax,10h ; jmp back`, args in edi/esi) where the lane rotates it with a lookahead load (`mov edi,[ecx-10h] ; ... ; jg`) and allocates esi/edx | tried: while, for(;;)+break, pointer walk, optimize("s"), inline and __forceinline helper holding the walk | next: lane gap: loop rotation (GOTCHAS 23f)
- 00be24d0 CVO_OracleMarker::Init DIFFER(36v54): retail keeps a 0x10-byte frame temporary (`sub esp,10h`) whose four zero stores survive after the member copy through `mov esi,eax` | tried: brace/memberwise/memset local, (x,y,z,w) ctor temporary, implicit/user copy ctor, user operator=, long[4] array member, const-ref inline setter with temporary or named local, float fields | next: lane gap: dead stores of an address-taken 16-byte temporary (or an unknown source form; class unidentified, no references)
- 00be2540 CVO_OracleMarker::Init DIFFER(38v56): constructor twin of 00be24d0 (`mov eax,ecx`, returns this), same frame-temporary shape | tried: as 00be24d0 | next: same as 00be24d0

## Skipped
- none (no row covers two functions).

## Label corrections
- 0040b690 "SetAntialiasing" stores +0x48/+0x104 = Resolution / ResolutionNewProfile (PDB, retail new-profile block = PDB - 4): the resolution setter. (Same family: the landed 0x0040B6D0 "SetAntialiasing" is RefreshRate +0x4c/+0x108, the landed 0x0040B750 "SetCameraResetting" is VerticalSyncEnabled +0x50/+0x10c.)
- 0041a0c0 "GetAutoSaveFileName" returns L"Data\\Misc\\" (a folder getter, static __fastcall); 0041a4c0 "GetAutoSaveCheckPointFileName" returns Documents + L"My Games\\Fable\\".
- 007d7c10 "CUserProfileManager::GetAutoSaveFileName" is slot 42 of CTCTavernGameSpotTheAddition (vtable 0x0126F814) returning L"".
- 00913960 "GetAssignedInputsPassive" is an STLport deque<T(0x30 bytes)>::back() instance (no callers).
- 00d12cb0 / 00d7c700 / 00e00690 / 00e2ab60 "CV_AmbushScamScript::CV_AmbushScamScript" are four different quest-script constructors (vtables 0x012C9E68, 0x012D3994, 0x012DF16C, 0x012E2428; no RTTI) -- named CVtable_<va> like the landed 0x00E68F70.
- 00d66e90 / 00e50100 "CV_AmbushScamScript::WatchForQuestFinished" belong to other script classes (flag layouts differ); 00e2ba50 fits the real CV_AmbushScamScript layout.
- 00e2bac0 "AssignMoralityEtcForEvilEnding" reads MoralityGood (+0x2b4 retail) -- the good-ending twin; 00e2bb10 is the evil one.
- 00b2fa40 "CV_BanditTollScript::Init" is an engine-module member (ecx = manager @ 0x01436E58) called from CEnginePrimitiveManagerOcclusionBase::RenderBoundingBox.
- 00c12040 "CV_BordelloScript::HeroWorksInBrothel" (and its callee labels C3DMesh2::CalculateTextureChangeFlags, CTCOnHeroPush::IsAngry, CBankFileAsyncData::FreeBuffer) are propagated; the body is an engine-module update step.
- 00be24d0 / 00be2540 "CVO_OracleMarker::Init" do not fit the PDB CVO_OracleMarker layout (no references found).
- 00cb8110 "CHeroMorphDef" (callee of the script ctors) is CScriptBase::CScriptBase (already in GOTCHAS).
