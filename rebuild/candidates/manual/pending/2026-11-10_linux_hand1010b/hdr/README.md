# agent1010b header overlay (FABLE_HDR_OVERLAY=$A/hdr)

Copies of rebuild/include files with new opt-ins; every change is behind its own `#ifdef`, the default path is byte-identical
to the committed header. New files are only included by rows that ask for them.

| File | Opt-in / change | Row | Evidence |
|---|---|---|---|
| engine/CScriptGameResourceObjectScriptedThing.h | `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHING`: retail class (base CScriptGameResourceObjectScriptedThingBase, ScriptInterface +0x10, PThing CIntelligentPointer<CThing> +0x14, ScriptID +0x1C, NullResource +0x20, ActionPriority +0x24, size 0x28), both ctors, virtual dtor, static fastcall CreateNullHandle | 0x0089B5B0 | RTTI bases; Ego_r/FableWin ctor + CreateNullHandle names; `push 28h` |
| engine/CTCScriptedControl.h | `FABLE_RETAIL_CTCSCRIPTEDCONTROL_ISLOCKED`: inline `IsLocked()` (Locked +0x18) | 0x0089B5B0 | Ego_r/FableWin ?IsLocked@CTCScriptedControl@@QBE_NXZ; `mov al,[esi+18h]` |
| engine/CThingAICreature.h | `FABLE_RETAIL_CTHINGAICREATURE_SCRIPTPRIORITY`: GetCurrentStateGroupScriptPriority() const 0x006D5AD0 (decl) + MS forward enum EScriptAIPriority | 0x0089B5B0 | Ego_r name; body `mov ecx,[ecx+1A8h]; jmp 0x0088B760` |
| engine/CGameScriptInterface.h | `FABLE_RETAIL_CGAMESCRIPTINTERFACE_GETCURRENTSCRIPTID`: inline `GetCurrentScriptID()` | 0x0089B5B0 | FableWin ?GetCurrentScriptID@CGameScriptInterface@@QBEJXZ (debug only = inline in release) |
| engine/CCountedPointer.h | `FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR`: `__declspec(noinline)` on the default dtor (MSVC only); pair with the existing VISIBLE_CALLED_CTOR | 0x008A8C60 | retail calls 0x008AB3D0 yet packs/copies the counted pointer as if its body were visible |
| engine/CCreatureAction_SitDown.h | `FABLE_RETAIL_CCREATUREACTION_SITDOWN`: retail class on CCreatureActionBase, PSitMode CCountedPointer<NCreatureMode::CModeSitBase> +0xA8, size 0xB0 | 0x008A8C60 | RTTI, ctor/dtor PDB names, vtable slots |
| engine/CThingCreatureBase.h | `FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER`: GetRandomAnimFromNameWithFilter 0x00662EF0 (decl) | 0x008A8C60 | Ego_r/FableWin spelling |
| engine/CDefString.h | `FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION`: forwarder `operator CCharString()` forced inline, GetString with a visible noinline body | 0x008A8C60 | retail calls GetString 0x00415DD0 (not ??B 0x00415D70) after an implicit conversion (inline-budget device, row line 3) |
| engine/CCreatureAction_GetUpFromSitting.h | NEW: CCombatActionBase-derived, SittingMode +0x114, RemoveOnPerformAction +0x118, size 0x11C | 0x008A8C60 | RTTI, ctor PDB name, frame 0x12C |
| engine/CAnimFilterAnyGroup.h | NEW: CAnimFilterBase-derived, vtable 0x0127261C, inline ctor/dtor | 0x008A8C60 | RTTI + rtti-vtables.tsv |
| engine/NCreatureMode_CModeSitFloor.h | NEW: NCreatureMode::CModeSitBase (0x2C) / CModeSitFloor (0x38) on CTCCreatureModeManager::CModeBase | 0x008A8C60 | RTTI bases, Ego_r layouts, `push 38h` |
| engine/CTCQuestCompletionUI.h | NEW: retail layout (0xD8) with Active +0x0C / Quitting +0x1C / QuittingFadeOut +0xC0, Activate / SaveInitialValues, inline IsActive / IsQuitting / IsQuittingFadeOut | 0x00896B60 (parked) | Ego_r layout with STLport sizes, FableWin accessor names |
| engine/CQuestManager.h | `FABLE_RETAIL_CQUESTMANAGER_STARTSCREEN`: OnQuestStartScreenDisplayed 0x004B2FE0 (decl) | 0x00896B60 (parked) | PDB name; NAME_FIXES.tsv |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_AUTOSAVEQUESTSTART`: AutoSaveQuestStart 0x004A0CD0 (decl) | 0x00896B60 (parked) | Ego_r/FableWin ?AutoSaveQuestStart@CWorld@@QAE_NXZ |

Matched rows need: 0x0089B5B0 -> ScriptedThing.h, CTCScriptedControl.h, CThingAICreature.h, CGameScriptInterface.h;
0x008A8C60 -> CCountedPointer.h, CCreatureAction_SitDown.h, CThingCreatureBase.h, CDefString.h + the three new headers;
0x00896390 -> committed headers only.
