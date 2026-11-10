# agent1022 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every change is a new opt-in behind a new `#ifdef FABLE_RETAIL_*` (or a new header); with none of the new macros defined each
modified header preprocesses to the repo's current text, so landed rows are untouched. Copy these into `rebuild/include/` at landing.

| Header | New opt-in | Row(s) | What it adds |
|---|---|---|---|
| engine/CHandleBasedMemoryPool.h | `FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_DEFRAG` (new first branch, ahead of `FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL`) | 0x00A02DF0 | the real class (CBaseClassNonCopyable base; nested CHeader / CHandleInfo / CScopedCritical; members to 0x464 incl. `_RTL_CRITICAL_SECTION` +0x44C), inline BeginCritical (TryEnterCriticalSection path) / EndCritical, GetListIndex as the ternary `(size >> 9) < 0xff ? (size >> 9) : 0xff`, private decls of the defrag helpers with their Ego_r spellings |
| engine/GFFPUSetDoublePrecision.h | NEW shared retail-asm helper | 0x00418289, 0x00417747 | `fnstcw` / `(cw & ~0x100) \| 0x200` / `fldcw` (Retail-asm 0x004182B2..0x004182B9), `FABLE_PORTABLE` twin via `_controlfp(_PC_53, _MCW_PC)` / glibc `fpu_control.h` / no-op. Needs a dual-path test in `rebuild/tests/portable/` at landing (same pattern as GFFPUSetRoundingNearest's) |
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_UPDATE`, `FABLE_RETAIL_CMAINGAMECOMPONENT_FORCEDTICK` | 0x00418289 / 0x00417747 | typed EventPackageSet (CGameEventPackageSet), IsControllerDisconnected, SignalFrameUpdate (0x00416047), IsTimeForServerUpdate, GetEventPackageSetFromSave, AddEventPackageSetToSave; LastInterpolationInfo / RoughFPSCounter typed, UpdateForcedTickSpeed, protected GetPredictedTimeSinceLastRenderFrame |
| engine/CInterpolationInfo.h, engine/CRoughFrameCounter.h | NEW | 0x00417747 | Ego_r layouts used by UpdateForcedTickSpeed |
| engine/CNetworkClient.h | `FABLE_RETAIL_CNETWORKCLIENT_GAMEEVENTPACKAGESET` | 0x00418289 | GetGameEventPackageSet (0x004AEBA0), inline IsLocal (LocalGame) |
| engine/CInputManager.h | `FABLE_RETAIL_CINPUTMANAGER_EVENTFLAGS` | 0x00418289 | Loading +0x38 / Saving +0x39 and their inline accessors |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_UPDATEINPUT` (both branches) | 0x00418289 / 0x00417747 | IsPaused, UpdateInputRelatedInterpolation (0x0049D9E0), RenderUpdate(const CInterpolationInfo*) (0x0049E080) |
| engine/CSubtitleRenderer.h | `FABLE_RETAIL_CSUBTITLERENDERER_INTERACTIVE` | 0x00418289 | IsInInteractiveState (0x00490A22) |
| engine/NUISystem_CManager.h | `FABLE_RETAIL_CMANAGER_ERRORMESSAGE` | 0x00418289 | ClearErrorMessage (0x0041CE6A), HasErrorMessage, the free `ShowErrorMessage(ERRORMESSAGE_TYPE)` __fastcall (0x0041E61B) |
| engine/CThing.h | `FABLE_RETAIL_CTHING_SETCURRENTTHINGMANAGER` | 0x00417747 | static SetCurrentThingManager(CThingManager&) (0x004C74F0) |
| engine/CDisplayEngine.h | `FABLE_RETAIL_CDISPLAYENGINE_DORENDER` | 0x00417747 | protected DoRender(const CInterpolationInfo*, bool, bool) (0x00435F70) + friend CMainGameComponent |
| engine/CSystemManager.h | `FABLE_RETAIL_CSYSTEMMANAGER_APPACTIVE` | 0x00417747 | IsApplicationActive (0x009A57B0) |
| engine/CInputProcessGameBase.h | `FABLE_RETAIL_INPUTPROCESS_MOVEMENT` | 0x00691D70 | GetControlMovementFromGameActionEvent (0x00687D00), protected IsEventButtonReleased (0x00687B30) |
| engine/CInputProcessCreatureMovement.h | `FABLE_RETAIL_INPUTPROCESS_CREATUREMOVEMENT_PROCESSINPUT` | 0x00691D70 | virtual ProcessInput declaration |
| engine/CInputEvent.h | `FABLE_RETAIL_CINPUTEVENT_DEVICETYPE` | 0x00691D70 | GetDeviceType (0x00A03B50) |
| engine/CGameEvent.h | `FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE` (nested in `_INLINE`) | 0x00691D70 | inline AddUHUGE / AddSLONG (FableWin spellings) |
| engine/CGamePlayerInterface.h | `FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_SNEAK` | 0x00691D70 | IsToggleSneak (0x00445BA0) |
| engine/CProcessedInput.h | `FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED` | 0x00691D70 | SetAsPerformedEvent (0x00A0D390) |
| engine/CTCStealth.h | `FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE` | 0x00691D70 | IsSneaking / SetSneakingToggleSet as inline bodies (retail expands them) instead of out-of-line decls |
| engine/CTCBerserk.h | `FABLE_RETAIL_CTCBERSERK_CTOR` (nested in `_HIT_EFFECT`) | 0x0078C210 | ctor `CTCBerserk(CThing&)` and private SetLevel() (0x0078BF00) |
| engine/CTCHeroExperience.h | `FABLE_RETAIL_CTCHEROEXPERIENCE_TOTALSTAT` | 0x0078C210 | GetTotalStatLevel / GetTotalStatMaxLevel (0x006D6660 / 0x006D66E0) |
| engine/CTCCombat.h | `FABLE_RETAIL_CTCCOMBAT_FLOURISH` | 0x0078C210 | AddMeleeFlourishAbility(long) (0x00736640) |
