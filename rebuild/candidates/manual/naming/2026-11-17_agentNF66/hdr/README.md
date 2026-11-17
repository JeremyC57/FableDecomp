# NF66 header overlay (round 7)

Every file is a copy of the CURRENT repo header (re-diffed at hand-back: no repo line removed or changed) with `#ifdef` opt-ins
added. Each body or declaration is new text inside its own macro; without the macro the header is token-identical to the repo.

| Overlay file | Repo header | Opt-in added | Used by |
|---|---|---|---|
| engine/NUISystem_CList.h | rebuild/include/engine/NUISystem_CList.h | `FABLE_RETAIL_CLIST_GETDATAFROMDEF`: declares Ego_r/FableWin `?GetDataFromDef@CList@NUISystem@@QAEXXZ` (public, 0x0053822B) | 0x0053822B (ported) |
| engine/CThingCreatureBase.h | rebuild/include/engine/CThingCreatureBase.h | `FABLE_RETAIL_CTHINGCREATUREBASE_WORLDUPDATE`: `virtual bool WorldUpdate()` (Ego_r `?WorldUpdate@CThingCreatureBase@@UAE_NXZ`, slot-19 override, no new slot) and protected `UpdateBodyReorientation()` (Ego_r `IAEXXZ`, 0x006622D0) | 0x006666A0 (ported) |
| engine/CThingGameObject.h | rebuild/include/engine/CThingGameObject.h | `FABLE_RETAIL_CTHINGGAMEOBJECT_WORLDUPDATE`: `virtual bool WorldUpdate()` (`?WorldUpdate@CThingGameObject@@UAE_NXZ`, 0x00838BA0, slot-19 override) | 0x006666A0 |
| engine/CTCPhysicsBase.h | rebuild/include/engine/CTCPhysicsBase.h | `FABLE_RETAIL_CTCPHYSICSBASE_GETTURNSPEEDANGLEXY`: `float GetTurnSpeedAngleXY() const` (`QBEMXZ`, 0x006AFF40) | 0x006666A0 |
| engine/NThingShot_CTCShotDefinitionBase.h | rebuild/include/engine/NThingShot_CTCShotDefinitionBase.h | `FABLE_RETAIL_CTCSHOTDEFINITIONBASE_UPDATE`: `void Update()` (Ego_r `?Update@CTCShotDefinitionBase@NThingShot@@QAEXXZ`, 0x007C21A0) | 0x006666A0 |
| engine/CQuestManager.h | rebuild/include/engine/CQuestManager.h | `FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_CONTAINSREGION`: declares `?ContainsRegion@CActiveQuest@CQuestManager@@QBE_NABVCRegion@@@Z` (0x004AFD80) on the full nested CActiveQuest (the REGIONLOCK block has an inline body on its partial record) | 0x004B11A0 (ported) |
| engine/CAnimSet.h | rebuild/include/engine/CAnimSet.h | `FABLE_RETAIL_CANIMSET_SETASFINISHED_DECL`: `SetAsFinished(long)` declared only (Ego_r `QAEXJ`, out of line 0x00668A50) in place of the inline body (the `#else` arm is the repo text, token-identical); nested `FABLE_RETAIL_CANIMSET_SETUPDATEMODE`: FableWin inline `SetUpdateMode(EAnimUpdateMode)` / `SetAsFinished()` | 0x00694430 (ported) |
| engine/CTCCarrying.h | rebuild/include/engine/CTCCarrying.h | `FABLE_RETAIL_CTCCARRYING_GETALLCARRIEDTHINGS`: `GetAllCarriedThings(std::vector<CThing*>&) const` (Ego_r `QBEXAAV?$vector@PAVCThing@@..@Z`, 0x006A2220); the includer must include stlport/fable_containers.h first | 0x00694430 |
| engine/CThingCreatureBase.h (second opt-in) | same | `FABLE_RETAIL_CTHINGCREATUREBASE_SETPITEMTOUNSHEATHE`: `SetPItemToUnsheatheAfterCutscene(CThing*)` 0x00662450, DESCRIPTIVE name (no PDB / FSE / Ego_d name; NAME_FIXES.tsv) | 0x00694430 |
| engine/CFrontEndManager.h | rebuild/include/engine/CFrontEndManager.h | `FABLE_RETAIL_CFRONTENDMANAGER_REFRESHAVAILABLEPROFILES`: protected `RefreshAvailableProfiles()` / `RefreshAvailableProfilesForDelete()` (Ego_r/FableWin `IAEJXZ`) inside the SAVEDGAMES block | blocked drafts of 0x00596CCE / 0x00597006 only |
| engine/CUserProfileManager.h | rebuild/include/engine/CUserProfileManager.h | `FABLE_RETAIL_CUSERPROFILEMANAGER_GETPROFILEDIR`: `CWideString GetProfileDir(const CWideString&, bool)` (`?GetProfileDir@CUserProfileManager@@QAE?AVCWideString@@ABV2@_N@Z`, 0x004082D0) | blocked drafts of 0x00596CCE / 0x00597006 only |

The CTCPhysicsBase.h opt-in opens with `public:`; the access in force before it is already public (repo line 280), so nothing leaks.
Macro names are member-specific so the coordinator can merge them with NF64 / NF65 overlays. No default path changed.
