# NF37 header overlay (round 3)

Each file is a copy of the CURRENT repo header with `#ifdef` opt-ins added. Nothing is removed or changed outside the new blocks
(checked with `diff`: 0 lines removed in every file).

| Overlay file | Copies | Opt-in added | Declares | Used by |
|---|---|---|---|---|
| `engine/CFaction.h` | `rebuild/include/engine/CFaction.h` | `FABLE_RETAIL_CFACTION_ISALLIEDWITHFACTION` | `bool IsAlliedWithFaction(const CFaction&) const;` 0x006B9460 (Ego_r.pdb `?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z`, Ego_r 0x006F4EC0; catalog CFaction::IsAlliedWithFaction) | 0x0076C7F0 |
| `engine/CGamePlayerInterface.h` | `rebuild/include/engine/CGamePlayerInterface.h` | `FABLE_RETAIL_CGAMEPLAYERINTERFACE_GAMEACTIONQUERIES` (generated-layout class) | `IsEventGameAction(EGameAction, const CInputEvent&) const` 0x00445BD0, `GetControlMovementFromGameActionEvent(EGameAction, const CInputEvent&, C2DVector&) const` 0x00445C30, `IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long, float&) const` 0x00445CB0 (Ego_r.pdb 0x004439F0 / 0x00443A60 / 0x00443AF0 and FableWin.pdb; Ego_r order IsToggleSneak..IsGameActionEventInQueue(3) = retail 0x00445BA0..0x00446B10) | 0x00687CF0, 0x00687D00, 0x00687D80 |
| `engine/CInputProcessGameBase.h` | `rebuild/include/engine/CInputProcessGameBase.h` | `FABLE_RETAIL_INPUTPROCESS_GAMEACTIONS` (generated-layout struct; the retail class already has ZTARGET / MOVEMENT / EVENTINQUEUE) | the same three base queries the retail opt-ins declare: `IsEventGameAction` 0x00687CF0, `GetControlMovementFromGameActionEvent` 0x00687D00, protected `IsGameActionEventInQueue` 0x00687D80 | 0x0068AE30, 0x0068B2E0, 0x0068C880, 0x0068F320, 0x00690480, 0x00690830, 0x00690B20, 0x00690E70, 0x00690FD0, 0x00691130, 0x00691680, 0x00691870 |
| `engine/CRenderManagerCore.h` | `rebuild/include/engine/CRenderManagerCore.h` | `FABLE_RETAIL_CRENDERMANAGERCORE_WAITWHILEGPUISBUSY` inside the flattened Ego_r layout (the retail class already carries it under this macro) | `void WaitWhileGPUIsBusy();` 0x00A0AF80 | 0x00F37170 |

All other rows use opt-ins that already ship in the repo headers.
