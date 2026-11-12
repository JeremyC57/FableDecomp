## 0x00A114B0

# Header overlay for row 0x00A114B0 (wf1)
- engine/CMatrix3x4.h: `FABLE_RETAIL_CMATRIX3X4_SETIDENTITY` adds FableWin.pdb `?SetIdentity@CMatrix3x4@@QAEXXZ` (inline; chained zeros then the diagonal, the store order of 0x00A114F8..0x00A11520). Full copy of the current repo header plus this block; default path unchanged.
- engine/CPhysicsMeshObject.h: `FABLE_RETAIL_CPHYSICSMESHOBJECT_SETPOSITIONMATRIX` declares `void SetPositionMatrix(const C3DVector&, const CRightHandedSet&, float)` (Ego_r.pdb / FableWin.pdb, retail 0x00A114B0). Declaration only; default path unchanged.
- engine/C3DVector.h: `FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP` (wf1 attempt 2, row 0x00A114B0) is operator^ with all three components parenthesised, `C3DVector((Y*o.Z-Z*o.Y),(Z*o.X-X*o.Z),(X*o.Y-Y*o.X))`. Same instructions as the default; three no-code PAREN tuples ahead of VC7.1's first scheduling-region cut. Full copy of the current repo header plus this `#ifdef` branch (the existing `_CROSS_XY_GROUP` / `_CROSS_X_GROUP` branches follow as `#elif`); default path unchanged.

## 0x007FF190

engine/CTCNamedUserActionPoints.h: FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS -- the real class chain (CTCUserActionPointBase : CTCBase with the Ego_r member names and the PDB method spellings, then CTCNamedUserActionPoints with Names); default flattened layout unchanged. Row 0x007FF190.

## 0x00BEAB40

- engine/CEngineLandscapeRenderer.h: `FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME` adds the FableWin inline `const CLandscapeTheme& PeekTheme(unsigned long) const` (row 0x00BEAB40); default path unchanged.

## 0x00BFA920

engine/CEngineFadeDistance.h: FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR -- inline default and (float,float) constructors (retail inlines them at 0x00BFA939).
engine/CEnginePrimitiveRenderer.h: FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDSTATICMESH -- declares AddStaticMesh 0x00B32F20 (PDB signature) in both branches; no layout change.

## 0x009E3DF0

engine/CPixelFormat.h: FABLE_RETAIL_CPIXELFORMAT_PFDDATATORGB -- declaration of ?ConvertPFDDataToRGB@CPixelFormat@@QBEXPBEAAVCRGBColour@@@Z (0x009E3DF0); full copy of the repo header plus that branch only.

## 0x007F5150

No header overlay needed for 0x007F5150: the row compiles against the current repo headers (no opt-in added).

## 0x00863CA0

# Header overlay for row 0x00863CA0
No overlay headers: the row matches on the current repo headers (existing opt-ins only; see NOTES.md for the list).

## 0x00688D40

- engine/CAInputProcess.h: FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE -- declares `virtual bool AddToInterface(CInputProcessManager&)` (slot 1, 0x00A0D2B0); row 0x00688D40.
- engine/CInputProcessGameBase.h: same macro -- declares the override 0x00687A70 (called by name from 0x00688D40).
- engine/CInputProcessZTarget.h: same macro -- declares the override 0x00688D40.

## 0x006EA0C0

engine/CCombatManager.h: FABLE_RETAIL_CCOMBATMANAGER_DEFENDERATTACKPATTERN -- private decls of UpdateDefenderAttackPattern 0x006EA0C0 and ResetDefenderAttackPattern 0x006E8C30 (both PDBs); declarations only.

## 0x004B4260 (held back)

# Header overlay of row 0x004B4260 (full copies of the CURRENT repo headers plus opt-ins; default paths unchanged)
- engine/CQuestManager.h: `FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS` (needs FABLE_RETAIL_CQUESTMANAGER + _LOADGAMESTATE; the ActivateQuests body also needs _ACTIVEQUEST + _SAVEGAMESTATE) -- nested CActivateQuestInfo (Ego_r layout + inline ctor), ScriptInfoManager typed at +0x78, CurrentQuestID +0x84, QuestsWithResourcesLoadedThatAreNotActivated (std::set<CCharString>) +0x9C, CActiveQuest ctors / dtor / LoadFromGameState, CRuntimeQuestPersistence's two CVectorMaps typed with the inline GetQuestScriptPersistScript, declarations of IsQuestRegistered / IsQuestActive / GetIndexFromRegisteredQuest / private ActivateQuests / GetMasterScriptDataForScript / LoadMasterScriptDataFromGameState / LoadResourcesForQuests.
- engine/CScriptInfo.h: `FABLE_RETAIL_CSCRIPTINFO_FASTCALL_ALLOC` -- AllocFunc is called __fastcall in retail (0x004B3F3A `mov edx,[world+38h] ; mov eax,[info] ; mov ecx,<data> ; call eax`); the default keeps the old __cdecl spelling.

