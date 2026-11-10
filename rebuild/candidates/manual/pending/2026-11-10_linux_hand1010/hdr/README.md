# Chunk 1010 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every file here is a copy of `rebuild/include/engine/<file>` at repo HEAD with additions behind NEW `FABLE_RETAIL_*`
opt-ins (default path byte-identical: nothing outside the new `#ifdef` blocks changed), or a new header. All grades of
this chunk ran with the overlay exported. Copy the files over `rebuild/include/engine/` at landing.

| Header | Opt-in | Adds | Evidence | Rows |
|---|---|---|---|---|
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_MAPNUMBERFROMSCRIPTNAME` | `long GetMapNumberFromScriptName(const CCharString&) const` 0x004FB880 | FableWin + Ego_r `?GetMapNumberFromScriptName@CWorldMap@@QBEJABVCCharString@@@Z` | 0x008930F0 |
| CThingSearchTools.h | `FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING` (declaration) / `_FIRSTTHING_BODY` (same-TU `__declspec(noinline)` body; needs `_ALLTHINGSBODY` for PeekClassList) | `template<class Filter,class T> bool GetPFirstThing(const Filter&, T**) const` (instance 0x008ABDF0) | FableWin `??$GetPFirstThing@V?$CFilter_Bind2@VCThingFilter_HasScriptName@@VCThingFilter_InMap@@@@VCThingGameObject@@@CThingSearchTools@@QBE_N...`; the same-TU body fixes 0x008930F0's scheduling | 0x008930F0 |
| CThingGameObject.h | `FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS` | `static unsigned long __fastcall StatGetTypeFlags() { return 7; }` | FableWin `?StatGetTypeFlags@CThingGameObject@@SIKXZ`; mask 7 in 0x008ABDF0 | 0x008930F0 |
| CThingFilter_HasScriptName.h | NEW | `CThingFilter_HasScriptName : unary_function<CThing,bool>` { CCharString Name; ctor; `operator()` 0x006A7E30 } | Ego_r.pdb 4 bytes, member Name; FableWin `??RCThingFilter_HasScriptName@@QBE_NABVCThing@@@Z` | 0x008930F0 |
| CGameScriptThing.h | `FABLE_RETAIL_CGAMESCRIPTTHING_CREATENULLTHING` | `static CScriptThing __fastcall CreateNullThing()` 0x008CFF40 | FableWin `?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ` | 0x008930F0 |
| NScreenEffect_CScreenEffectPriorityStack.h | `FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER` | `enum NScreenEffect::ScreenEffectPriority` (SCRIPT = 1) and pack(1) `CThingGraphicArgPasser { CThingGraphicArgPasser(CThing*, bool); CIntelligentPointer<CThing> thing; bool bypassSet; }` (9 bytes) | FableWin 0x006B85D0 signature; retail 9-byte vector stride | 0x008A9880 |
| CEnvironmentSpellEffectInterpolator.h | `FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO` | `void* ScreenFilterFadeTo(float, ScreenEffectPriority, float x4, const CRGBFloatColour&, const ThingGraphicArgPassers&)` 0x006B85D0 (+ `class CRGBFloatColour;`) | FableWin + Ego_r leaf (see NAME_FIXES) | 0x008A9880 |
| CScreenFilterSThingByPass.h | NEW | `CScreenFilterSThingByPass { CScreenFilterSThingByPass(CScriptThing&, bool); CScriptThing thing; bool bypassSet; }` | Ego_r.pdb layout (+0x00 / +0x0C); CGameScriptInterface::ScreenFilterFadeTo leaf | 0x008A9880 |
| CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYACCESS` | `GetInventoryType() const` 0x005D9030, `GetInventoryOwner() const` 0x005D8E90 | FableWin + Ego_r (NAME_FIXES) | 0x00897FA0 |
| CTCCarrying.h | `FABLE_RETAIL_CTCCARRYING_SLOTFREE` / `_SLOTACCESS` | `IsCarrySlotFree(long) const` 0x006A1230; `IsSlotPrimarySlot(long) const` 0x006A1330, `RemoveThingInCarrySlot(long, bool)` 0x006A16A0 | FableWin + Ego_r | 0x00897FA0, 0x00895190 |
| CTCCreatureOpinionOfHero.h | `FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM_HANDLE` (nested in `_INFORM`) | `void InformOfDeed(long handle, bool flag)` 0x00683590 | FableWin `?InformOfDeed@CTCCreatureOpinionOfHero@@QAEXJ_N@Z` | 0x0089FEB0 |
| CTCHero.h | `FABLE_RETAIL_CTCHERO_TUTORIAL` | `bool GiveTutorialForCategory(ETutorialCategory, bool)` 0x006C9FA0 (+ include of fable_nscript_forward.h) | FableWin `?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z` | 0x00896390 |
| CThingCreatureBase.h | `FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION` | `void FinishCurrentAction()` 0x00662650 | FableWin `?FinishCurrentAction@CThingCreatureBase@@QAEXXZ` | 0x00896390 |
| CCreatureAction_OpenChest.h | `FABLE_RETAIL_CCREATUREACTION_OPENCHEST` (needs `FABLE_RETAIL_CCREATUREACTIONBASE`) | retail class on CCreatureActionBase (0xB0): ctor `(CThingCreatureBase&, CThing&, const CCharString&)` 0x007EE7E0, PerformAction 0x008448E0 / FinishAction 0x00842B60 / Clone 0x007EEA50 declared, `CIntelligentPointer<CThing> PChest` +0xA8, implicit dtor | FableWin ctor spelling; landed ctor/Clone/PerformAction rows; vtable 0x0127173C | 0x00896390 |
| CTCChest.h | NEW | `CTCChest : CTCBase` { GetOpenAnimNameForCreature() const 0x00750240, Open() 0x00750540, Close() 0x007506C0 } | FableWin `?GetOpenAnimNameForCreature@CTCChest@@QBE?AVCCharString@@XZ`, `?Open@CTCChest@@QAEXXZ`, `?Close@CTCChest@@QAEXXZ` | 0x00896390 |

Default-path check: `diff` of each modified header against HEAD shows only added `#ifdef FABLE_RETAIL_<new>` blocks
(CCreatureAction_OpenChest.h wraps the generated body in the `#else` of the new opt-in).
