# Hand chunk 153 (CPlayer .. CQ_HangingTreeEvilScript): 33 of 40 byte-exact

## What matched (33)
- **Quest scripts (CQ_*/CQR_*, 14)**: shared model = CScriptBase (0x40) + interface pointer +0x40 / owner +0x44, PDB offsets - 0x14 (and -4 per STLport vector before the member).
  - 00d66ee0 / 00f02100 EndMission: landed EndMission loop, then `SetQuestAsCompleted(GetActiveQuestName(), ...)` with the name temporary as argument (one vtable load shared).
  - 00f02170 / 00f0b780 PhaseOne: the loaded-test must be an inline helper with a NAMED CCharString local and `? false : true` (a temporary inside the call lets VC read the GSI vtable before the string is built).
  - 00d69cd0 BanditYells, 00f03560 WatchForBoastSuccess, 00d3a920 UpdateHistory: plain rotated `while (!terminating)` loops / if-else arms, compiled as written.
  - 00d254b0 Init, 00d17ea0 AddBoastQuestInfo: string arguments are NAMED locals built before the interface is read (two strings: the second argument declared first).
  - 00e24090 ForceDisableFollow: `SetEntityAsRegionFollowing(GSI->GetHero(), trader, false)` (both calls share ebx vtable).
  - 00cfa660 CQ_ArenaScript / 00d2a820 CQ_EndGameFocalSitesScript ctors: the landed novtable CScriptObject + STLport vector + CScriptThing + self-registering CTimer models.
  - 00f14270 GetFanfareMusic: an explicit `case 0` returning the default value keeps 0 in the jump-table range.
  - 005ea8e0 CTCTavernGame::OnSerialise (catalog said OnPersist of a quest manager): float default named local, long/bool defaults literal (dead arg slot).
- **CPlayer (7)**: engine/CPlayer.h is right up to +0x208; PlayerModes is 8 bytes narrower in retail, so flags from +0x216 are PDB-8.
  - 006337c0 InitModeParalysed, 006359c0 InitModeClickPastText: landed InitMode pattern; the special-abilities lookup is the chunk-141 HasTC/Find model with the key in the dead mode slot.
  - 006335e0 InitModeControlFreeCamera: the world's slot 6 returns a REFERENCE to the camera it filled into a caller scratch local (`mov esi,eax` for each of the three `rep movsd`).
  - 00488ab0 Update: landed InitialiseIfUninitialised inlined + `World->GetWorldMap()->SetPlayerPos(Number, creature->GetPosition(), 0)`.
  - 0048a1a0 InitPlayerHeroLogBook: STLport vector + _Rb_tree (alloc_proxy header built with 0 then malloc'd, `_M_empty_initialize` through reference accessors) inline in `new CHeroLogBook`.
  - 00a67ea0 CVolumeTexture::GetVolumeLevel (catalog CPlayer::UninitModeQuestCompletionUI): COM slot 18 out-pointer homed in the level slot; `CVolume r(volume); return r;` via an inline converting ctor.
  - 00c90cb0 InitializeDrawing (catalog CPlayer::InitializeDrawing): a linked media player's `__fastcall` routine; the backend hook is a member-function-pointer table call `(p->*g_table[3])()`.
- **CPlayerGui (5)**: retail offsets = engine/CPlayerGui.h - 8 for the counted draw pointers and AbilityButtons, -0x14 from DrawGui, -0x10 from HeroCentreUID.
  - 00438230 ClearGui / 00438460 PrepareForLoad: `for_each(begin, end, mem_fun(&CDrawBase::Virtual))` with the `__fastcall` SGI for_each (functor result in the push-ecx frame word, vcall thunk immediate).
  - 00438990 / 00438a20 Get/SetAbilityButton: lazily loaded def in a forced-inline helper, `(unsigned long)index < size()` (divide by 0x5c), `*(begin()+n)`.
  - 0043a4d0 SetGuildSealLevelUpPos: STLport push_back/_M_insert_overflow and clear() as erase(begin(), end()); the __true_type tag needs a user ctor.
- **CPlayerManager (2)**: 00449730 GetSpiritDefName (same explicit-last-case trick), 00449cb0 SwapToHero (MainPlayer +0x1c in retail).
- **Others (5)**: 008198c0 SwapKeyedRecords (pair-style user copy ctor over a POD value with IMPLICIT copy gives the lea'd copy); 00885a80 CIDrawEnvironmentRadialBlurParameters copy ctor (C3DVector user copy ctor, CRGBFloatColour implicit); 00a69830 CPreallocTexturePool::Allocate; 00499c50 CProgressDisplay::DisplayProgress (the 0.0001f is a float GLOBAL read from memory, `g_fEpsilon > LastUpdateTime`); 006e9140 CProng::SetBlocked (landed SetPTarget temporary-pointer shape, `__forceinline` helper).

## Parked for Fable 5.1 (7)
- 00e24260 CQR_EscortTraderScript::GetTraderRandomSpeechEnabled DIFFER(95v97): retail `mov eax,[esi+eax*4] ; sub eax,4 ; mov eax,[eax]`, ours `mov edx,[esi+eax*4] ; mov eax,[edx-4]` | tried: back() as *(end()-1), decremented end copy, named reference, reverse_iterator | next: an out-of-line-looking iterator class with user copy ctor for vector<CCharString>::iterator, or back() through `end()` returning by value
- 00c70040 CopyBlock128MMX (catalog CPreTransposedBoneMatrix::Set) DIFFER(144v141): retail push esi/edi + mm0..mm7 rotated movq pairs + emms; intrinsics give an aligned ebp frame and reuse mm0 | tried: __m64 intrinsics | next: lane gap: inline-asm body
- 006f17a0 CProng::SetPOccupyingAttacker DIFFER(127v127): retail forms `add esi,14h` after the first temporary's GetPItem, ours before | tried: forced-inline helper, item local + member reference, inline base operator= over a converting temporary | next: lane gap: scheduling only
- 00487fb0 CPlayer::InitPlayerGui DIFFER(97v99): CPlayerGui ctor argument loads: retail loads bank/manager/interface into edi/ecx/ebx (push ebx) before pushing, ours interleaves loads and pushes | tried: member compare, number local (fixed the compare), optimize("s"), inline accessors | next: lane gap: argument scheduling
- 0087c580 CPlayer::GlobalEventCreatureDied DIFFER(99v102): non-main arm retail `mov eax,[ebp+28h] ; push eax`, ours `push esi` (value loaded before GetMainPlayerNumber) | tried: plain compare, 64-bit id local (fixed id compare), inline IsMainPlayer, inline else helper | next: lane gap: member re-read after a call (CSE of a pre-call load)
- 004384f0 CPlayerGui::PeekGlobalDef DIFFER(117v123): inline counted release `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx` (ours `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`), and retail calls CGameDefinitionManager::Get before pushing &name | tried: hand-written Reset (one try, known gap) | next: lane gap: inline counted release; put the Get call in a GetWorld-style inline helper
- 00438ff0 CPlayerGui::DrawSaveScreen DIFFER(118v116): component kept in ecx by retail (`mov ecx,[eax+4]`), eax + `mov ecx,eax` by ours; same blocks and dead-slot read | tried: chunk-141 unset accessor, inline test, condition declaration, entry-returning Find | next: lane gap: register choice

## Skipped
- none (no row holds two functions; the two jump tables, 00449730 and 00f14270, belong to their function).

## Label corrections
- 0x005EA8E0 is CTCTavernGame::OnSerialise (vtable 0x012560BC slot 1 + four derived vtables), not CQR_EscortTrader_ManagerScript::OnPersist.
- 0x00D66EE0 is CQ_GuildTrainingWoodsMeleeScript::EndMission (only caller 0x00D66E7C inside its DoMission 0x00D66CA0), not CQ_CinemaTestScript.
- 0x008198C0 (reached via `jmp` thunk 0x0081A8A0) is a 24-byte record swap; 0x00885A80 is the CIDrawEnvironmentRadialBlurParameters copy constructor; 0x00C70040 is an MMX 128-byte block copy -- none is CPreTransposedBoneMatrix::Set.
- 0x00A67EA0 is CVolumeTexture::GetVolumeLevel-like (CVolume vtable 0x0129C840, D3D volume texture slot 18), not CPlayer::UninitModeQuestCompletionUI (that is the landed 0x00633B40).
- 0x00C90CB0 is a linked media-player `__fastcall` routine, not a game CPlayer member.
- Callees: 0x00A67770 is CVolume init (not CSurface::InitialiseFromPreallocatedSurface), 0x00A673E0 the CVolume copy ctor, 0x0099E900 CCharString::operator!= (not less<>), 0x009F9D50 a load-request test (not CThingFilter_IsUnconscious), 0x0043FF30 GetDef<CPlayerGuiDef>, 0x00449B40 a CPlayerManager per-player death handler calling CPlayer::UninitCharacter, 0x00410620/0x00410BE0/0x004045C0 are Transfer<float>/<long>/<bool>.
- 0x006359C0 (catalog InitModeClickPastText) drives InputProcessYesNoQuestion_ptr (+0x144), so it may be InitModeYesNoQuestion; name kept (only reference is the mode table initialiser).
