# Wave 7: remaining class-model debt, the engine value-class header scope, and the asm_bake list

Counts are taken from the wave 6 worktree (base c122befff plus the wave 6 edits). A row counts when it defines
`class|struct <X> {` (template prefix allowed) under `rebuild/src/compiled`, excluding `*.pre.cpp`
(`scripts/count.py`).

| class | wave 6 start | after wave 6 | header | what is left |
|---|---:|---:|---|---|
| CGameScriptInterface | 412 | **220** | CGameScriptInterface.h | 77 named slots at an index the header names otherwise (invented names / overload order); 65 the script edits but mcheck rejected (64 in REVERTED.tsv `CGSI-m6`: one-parameter std templates, `CScriptThingRuntime` / `CScriptThingVector` argument types, PDB enum params, `ESex` typedef clashes, local list/vector/set use); 14 `m_handLampLit` at +0x1c; 64 one-offs (local CBaseClass, `#define` blocks, local CDefinitionManager naming, members the header lacks, `CScriptThing Hero` / timer-map members) |
| CGameScriptInterfaceBase | 58 | 58 | CGameScriptInterfaceBase.h | 24 on the CScriptBase chain (local CBaseClassNonCopyable, see SB), 18 named slots at the wrong index; unchanged |
| CScriptBase | 1,704 | **456** | CScriptBase.h | 185 non-template `CLevelScriptBase` blocks (102 bare, 78 with an inline ctor, 5 `CGameScriptInterface* Game`); 90 local CBaseClassNonCopyable (47 with inline ctor/dtor bodies and full STLport member models, 42 the header's declaration-only shape); 41 `CGameScriptInterface* Game` in the template; 26 inline CLevelScriptBase ctor; 16 retail-0x48 pads (the quest-script base folded in); 15 no pad; 17 reverted (template clashes, `SetPredicate(CIsThingConscious)` without a CPredicateBase base) |
| CLevelScriptBase | 951 | **303** | CLevelScriptBase.h | the same rows as above (the non-template local class is the largest shape) |
| CScriptGameResourceObjectScriptedThingBase / Base / MovieBase | 264 / 87 / 86 | **220 / 80 / 85** | GRO headers | 85 rows define MovieBase (residue: LESSONS 1); 60 STB blocks with no base (filler vtables, `char pad[8]; Inner*`, `_body[0xb0-4]`); 9 other CCPPointerInfo models; ~50 reverted DIFFER / SRC_FAIL (REVERTED.tsv `GRO-full`) |
| CScriptThing / CEntityScriptBase / CTimer | 2,507 / 1,295 / 200 | unchanged | NScript headers | not started (wave 1 NEXT (c): the CScriptThing copy rule; chunk 583: wrappers after the header need `__forceinline`) |
| NScript::CIsThingConscious | 48 | 48 | CIsThingConscious.h | needs CScriptThing first |
| CPlayerManager | 487 | 487 | CPlayerManager.h (flattened + 5 methods) | a retail branch (vtable 0x01231CD0, `??1` UAE) is unwritten |
| CBaseObject, CSpawnedFunc(1), CSpawnedFuncBase, CBaseClass, CParentDefClassBase, CDefClassBase | 210, 2/15, 16, 21, 67, 650 | unchanged | existing | wave 4 NEXT items 3-5 (PDCB 67 with 7 fixable, BC 21, DCB clusters, the 16 SFB hand edits): not reached in wave 6 |

## Ready next (in order)

1. **The 185 non-template CLevelScriptBase rows.** The derived level script names its master data type in the row
   (`CQ_XMasterData* PMasterData` in the entity scripts, or the PDB `CLevelScriptBase<T>` base of the level script's
   Ego_r class key). Rewrite `: public CLevelScriptBase` as `: public CLevelScriptBase<T>` with the PDB T, then
   mig_sb.py. The 78 rows with an inline `CLevelScriptBase(data, game) : Game(&game), MasterScriptData(data) {}`
   need the retail ctor first: find whether retail inlines it (look for the two stores after `call 0x00CB8110` in the
   level-script ctors) and declare it in the header accordingly.
2. **`CGameScriptInterface* Game` (41 + 5 rows).** The PDB member is `CGameScriptInterfaceBase* Game`; the rows call
   CGSI slots through it. Change the type and cast at the call (`static_cast<CGameScriptInterface*>(Game)`), or give
   the rows' own base a typed accessor; mcheck decides.
3. **SB rows with a local CBaseClassNonCopyable (42 of the declaration-only shape).** Drop the local block when its
   shape equals the header's (ctor + virtual dtor declared only), then mig_sb.py.
4. **CScriptThing, then CEntityScriptBase, then CIsThingConscious** (wave 1 NEXT (c)). The CEntityScriptBase blocks in
   the SB rows (e.g. the Alloc rows' `CEntityScriptBase(CGameScriptInterfaceBase&, const CScriptThing&) : Game(&game),
   Me(me) {}`) are the next bulk family once CScriptThing is shared.
5. **GRO residue.** MovieBase rows and argument temporaries (LESSONS 1): find what makes retail call the
   CCountedPointer dtor COMDAT 0x006E7AB0 inside an expanded MovieBase dtor. Ideas not yet tried: the Ego_r
   `CCountedPointer<T>` member set (`ReleaseData`, `DeleteData` static, `ShareData`, `Reset`) in full, since the
   inliner's budget counts the callee's own call sites; `#pragma inline_depth`; EH (/EHsc) only for the temporaries.
6. **CGSI remainder:** the 77 slot-index rows need per-slot evidence (overload order); the 2 GetTextIDFromSymbol rows
   (NAME_FIXES).
7. **Wave 4 leftovers:** unchanged since wave 5 (see wave 4 / 5 NEXT).

## Engine value-class headers (scope only, no edits)

Rows modelling each class locally, the commonest local shapes, the PDB layout (Ego_r `struct_layouts_egor.tsv`), and
the proposed header shape. Every proposal needs its inline members proven against retail call sites before rows move
(the rows' copy/ctor forms are byte-relevant: an inline copy ctor and an implicit one emit different code).

| class | rows | commonest local shapes | Ego_r layout | proposed header (evidence to collect) |
|---|---:|---|---|---|
| C3DVector | 3,213 | `float X, Y, Z;` 1,273; lower-case `x, y, z` 185; with inline copy ctor 120; one-per-line 94 | 0xC: X, Y, Z (float) | `engine/C3DVector.h`: POD `float X, Y, Z;` and only the members retail calls out of line (1,474 C3DVector symbols in Ego_r; check which have retail bodies). Keep inline copy ctors out of the header until a call-site test shows retail's copy is the implicit one. The `x,y,z` rows need a member respelling |
| C2DVector | 863 | (not clustered) | 0x8: X, Y | as C3DVector |
| C3DBoundingBox | 92 | `C3DVector Min, Max;` 16; `float Min[3], Max[3];` 14; `MinCoord, MaxCoord` 6 | 0x18: MinCoord, MaxCoord | PDB names MinCoord / MaxCoord; the `Min` / `Max` rows need respelling |
| CMatrix3x4 | 299 | `float M[12]` 50; `float m_row[3][4]` 46; `float m[12]` 32; `C3DVector XAxis, YAxis, ZAxis, Position` 30 | 0x30, one anonymous union member | the union is the PDB form: model it (`union { float M[12]; struct { C3DVector ...; }; }`) after reading the Ego_r union fields |
| CRightHandedSet | 315 | `C3DVector Up; C3DVector Forward;` 121 (+29 one line); `float m[6]` 19; Forward-first 9 (probably wrong) | 0x18: Up, Forward | POD Up / Forward; check the 9 Forward-first rows against their retail offsets |
| CArray<T> | 1,193 | `template <class T> class CArray : public std::vector<T> { };` 579 (three spellings); own three-pointer model 30 | (template) | needs a shared STLport vector model first (fable_nscript_forward.h declares `vector<T, A>`; one-parameter local vectors clash with it, the commonest SB / CGSI compile failure) |
| CCountedPointer<T> | 3,451 | (template, many forms) | Data, Info (CCPPointerInfo*, 12 bytes: RefCount, DeleteFunc, Data) | full Ego_r member set; the dtor must be the retail release (LESSONS 1). Which members are inline in retail is the open question of LESSONS 1 |
| CCPPointerInfo | 2,214 | | 0xC | header with the PDB members; trivial |
| CCharString / CBasicString | 6,283 / 294 | `void* PStringData; CCharString(const char*, long = -1); ~CCharString();` 1,402; bare `void* PStringData` 358; `m_pData` 258; dtor only 175 | 0x4: CCharStringData* PStringData | `rebuild/include/fable_string.h` exists (1,126 includers) but is a different (flattened) model; a shared `engine/CCharString.h` with the PDB members and out-of-line ctor / dtor (0x0099EBF0 / 0x0099EAE0 / 0x0099E4B0) is the natural target; declaration-only rows move mechanically |
| CWideString | 1,019 | | 0x4: CWideStringData* | as CCharString |
| CEngineScene* | CEngineScene 35, ..Grid 31, ..GridCell 25, ..Map 19, ..RenderStats 9, others 15 | | CEngineScene 0x64, Grid 0x24, GridCell 0x48 (Ego_r; retail sizes need the STLport container correction) | generated headers exist only for CEngineSceneEnumeratorResetVisibilityRatio; generate + retail-correct the five main ones |
| C3DGroup2 / C3DQuad2 | 25 / 4 | | 0x14: BoneIndex, CArray<C3DVertexBlend2> VertexBlends / 0x8: ushort VertexIndex[4] | after CArray |

## asm_bake rows (landed `__declspec(naked)` / `__asm` / `_emit` without a `// Retail-asm:` line): 27

162 landed rows carry asm; 135 have a `// Retail-asm:` line. The 27 below do not (26 `__declspec(naked)`; 00a5b850 has
a bare `__asm` block). They are graded `asm_bake` (oracles, not reconstruction) and need C++ bodies, or a
`// Retail-asm:` line where retail itself ran the instruction.

| addr | counts | file |
|---|---|---|
| 0099bf30 | naked=1 asm=1 emit=0 | CWideString_AddWideStringAndLiteral_0099bf30.cpp |
| 0099be70 | naked=1 asm=1 emit=0 | CWideString_AddWideStrings_0099be70.cpp |
| 0042df9e | naked=1 asm=1 emit=0 | CNewFrontendGameComponent_Draw_0042df9e.cpp |
| 0042e204 | naked=1 asm=1 emit=0 | CNewFrontendGameComponent_InitialiseEngine_0042e204.cpp |
| 0042f722 | naked=1 asm=1 emit=60 | CNewFrontendGameComponent_InitialiseDefs_0042f722.cpp |
| 005bd993 | naked=1 asm=1 emit=0 | NInventoryCCategory_GetMaxItemsPerSlot_005bd993.cpp |
| 00899ef0 | naked=1 asm=1 emit=0 | CGameScriptInterface_GiveHeroMorality_00899ef0.cpp |
| 007022c0 | naked=1 asm=1 emit=0 | CTCQuestCard_IsOptional_007022c0.cpp |
| 00bfe9f9 | naked=2 asm=2 emit=0 | global_securitycheckcookie_00bfe9f9.cpp |
| 009bef80 | naked=1 asm=1 emit=0 | CDisplayManager_SetViewportInteger_009bef80.cpp |
| 009a8840 | naked=1 asm=45 emit=1 | CBankFileManager_OpenRetailBank_009a8840.cpp |
| 009a8170 | naked=1 asm=113 emit=1 | CBankFileManager_OpenIniFile_009a8170.cpp |
| 00536b4f | naked=1 asm=5 emit=1 | CList_InitialiseOffsets_00536b4f.cpp |
| 0053c332 | naked=1 asm=31 emit=1 | CList_DoRecomputeOffsets_0053c332.cpp |
| 00551340 | naked=1 asm=13 emit=1 | CTable_GetDataFromDef_00551340.cpp |
| 005dacf0 | naked=1 asm=1 emit=0 | CTCWeapon_IsAbleToBeUsedToBlock_005dacf0.cpp |
| 00597bf2 | naked=1 asm=1 emit=0 | CFrontEndManager_GotoPreviousScreen_00597bf2.cpp |
| 009c85a0 | naked=1 asm=1 emit=0 | NGameText_CDataBank_Constructor_009c85a0.cpp |
| 004167a7 | naked=1 asm=1 emit=33 | CMainGameComponent_GetFrameDifferenceFromCurrent_004167a7.cpp |
| 00413c50 | naked=1 asm=404 emit=1 | Global_InitialiseConsoleVariables_00413c50.cpp |
| 0066ff20 | naked=1 asm=1 emit=0 | CTCCoopSpirit_SwapToHero_0066ff20.cpp |
| 00c8c730 | naked=1 asm=2 emit=3 | global_BatchInvokeWithStackArgsc8c730_00c8c730.cpp |
| 00c8db10 | naked=1 asm=1 emit=0 | std_DestvalCFlasha_00c8db10.cpp |
| 004013c7 | naked=1 asm=1 emit=0 | Global_SEHEpilog_004013c7.cpp |
| 0040138c | naked=1 asm=1 emit=0 | Global_SEHProlog_0040138c.cpp |
| 00401284 | naked=1 asm=1 emit=0 | Global_Exit_00401284.cpp |
| 00a5b850 | naked=0 asm=1 emit=0 | global_GFInitVectorMath_00a5b850.cpp |
