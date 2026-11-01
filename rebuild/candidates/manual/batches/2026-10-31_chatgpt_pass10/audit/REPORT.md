# Naming audit: ChatGPT pass 10, 57 byte-exact rows (audit679, 2026-11-01)

Read-only audit. Inputs were the 57 sources in `w22/agent679/src`, which are byte-identical (sha256) to ChatGPT's `rows/<addr>/<addr>.cpp`. Each row's bound symbols come from ChatGPT's `reviews/<addr>/relocation_review.json`, whose source sha256 matches the current source; `ROW.json` bindings are incomplete for 14 rows. NAME_FIXES is the 181-row consolidated file. Files:

- `AUDIT.tsv`: 532 rows (57 leaves, 294 binding sites, 181 NAME_FIXES) with verdict, evidence and action.
- `LABELS.tsv`: 55 catalog or manifest labels proven wrong (address, correct symbol, evidence; the wrong label is quoted in the evidence).
- `NAMING_QUEUE.tsv`: 1,236 spellings that landed code binds at 170 of the audited addresses and that differ from the proven symbol (the stand-in debt). Each row lists its binders.
- Working data: `facts_*_c.json`, `perrow.json`, `callers.json`, `overrides.py` (every manual verdict with its evidence), `final.py`.

## Method

What counts as a source:
- **Spelling sources.** The exact mangled name is in the Ego_r, FableWin or Ego_d name set (161,621 mangled names). For data, the sets' ``dynamic initializer for '<name>'`` entries also count. For vtables, the retail RTTI type descriptor counts: it is read through the COL at vtable-4, and it is retail's own spelling.
- **Address sources.** The address already carries the same symbol in `reloc_bindings.tsv` (from landed code). Or `NAME_FIXES_QUEUE`/`ESTABLISHED_SPELLINGS` agree. Or the catalog label agrees on both class and member. Or there is an `rtti-vtables.tsv` slot. Or retail callers or the body semantics match, read with xref and capstone on our Fable.exe.

The Ego_r/FableWin builds used Dinkumware, not STLport. STLport-only members (`_M_find`, `_M_insert_overflow`, `__push_heap`, ...) are graded PROVEN_STL. That needs two things: the instance's element types appear in the PDB sets, and landed code already binds the same STLport member pattern (`reloc_bindings`: 22 to 448 sites per pattern).

Retail has no identical-code folding (GOTCHAS; naming/2026-10-29_wchar_held). So two spellings at one address means one of them is wrong.

## Result for the 57 rows

**No leaf and no binding is WRONG.** Every symbol ChatGPT emits is either proven or is a single-source or conflicting data/SDK name. Every leaf's class key, access, const-ness, calling convention and parameter types match the PDB spelling exactly. The exception is the GameCODA leaf of 00c35e30, which has no PDB to compare against.

- **45 rows fully proven** (leaf and every binding PROVEN, PROVEN_STL, or vtable/IAT proven):
  00462e78 00492fb4 004b0b90 004bc030 004bd3c0 004c7da0 004fc760 00525d90 0053dcd0 0053dd50 0055cb10 0057de57 0057f9e9 005bb190 005cbc36 005e38c0 0061aa80 0064ca15 006bc380 006e77f0 00701750 00705d40 00739360 0078c000 00835fd0 008559e0 0087c580 00894500 008cb250 0090c670 009b9c60 009ba5b0 009c8990 009fc170 00a3c1a0 00a533f0 00a53440 00a60fb0 00a65ff0 00a69c20 00a8f770 00ac8a60 00b66c10 00b93860 00cbaba0
- **12 rows carry a binding that is not fully proven.** All 12 leaves are proven except 00c35e30's. None of these is a wrong spelling; they are owner decisions:

| row | address | symbol (source line) | status |
|---|---|---|---|
| 0041db84 | 013b871c | `?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A` (src line 21) | PROVISIONAL: the identity is proven (the only writer is 0042f761 in CNewFrontendGameComponent::Init, vtable 01230ca0 by RTTI). The identifier and its private access come only from ChatGPT's raw Ego_r record. |
| 0065a075, 00699b00, 008f7860 | 013b86a0 | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` (0065a075:63, 00699b00:105, 008f7860:29) | CONFLICT_EST. Codex (codex50) and ChatGPT read raw Ego_r independently and got the same spelling, and landed code already binds it. But ESTABLISHED_SPELLINGS still says `?g_pMainGameComponent@@3PAVCMainGameComponent@@A`. Recommendation: keep PMainGameComponent and fix ESTABLISHED. |
| 006b3030 | 013ba8dc | `static unsigned long seed;` in UpdateWind (line 33) | PROVISIONAL (raw S_LDATA local; single source) |
| 006b3030, 007af0f0 | 013ce558 | `extern unsigned long GFastOneOverSqrtTable[128];` (006b3030:7, 007af0f0:44) | PROVISIONAL. The PDB has only the consumer `?GFFastOneOverSqrt@@YIMM@Z`. |
| 0076ef40 | 013bada0 | `static bool WeaponTrailAlphaAddedToConsole;` (line 42) | PROVISIONAL |
| 008f7860 | 013756f5 | `namespace NGlobalConsole{extern bool CombatParticleEffects;}` (line 30) | PROVISIONAL (the NGlobalConsole namespace itself is in both PDBs) |
| 009e3640 | 0129ba40 | `extern const CD3DFormatInfo GD3DFormats[];` (line 14) | PROVISIONAL |
| 00a87260 | 013986c4 | `static float min_height_diff=1.5f;` (line 51) | PROVISIONAL (raw S_LDATA local) |
| 00bc9800 | 01396fb0, 01436e18 | `D3DPolyCullModeTable` (line 19), `NEngineInternal::GPRenderManager` (line 32) | PROVISIONAL |
| 00c35e30 | leaf + 00c3c4a0, 00c3d090, 00c438f0, 0143bb58 | `_CODA_SourceSetPlayCursor@8`, `CSalBufferBase::GetDesc`, `CCodaContext::SetError`, `CCodaSource::SetPlayCursor`, `CODA` (lines 14-62) | PROVISIONAL. These are GameCODA SDK 2.10 names (ChatGPT's SDK header and COFF; the SDK is not in this repo). Retail is a Fable 1.5 fork and its strings carry no CODA_ API names; only the RTTI `CCodaFrameStreamer` corroborates the library family. |
| 00cef3b0 | 0143e8f8 | `extern CGameScriptInterfaceBase*pDSTGame;` (line 15) | CONFLICT_EST. The global's only writer is 00cbe0c0 (`mov [0143e8f8],ecx; ret`), which is the PDB `?SetDSTGame@@YIXAAVCGameScriptInterfaceBase@@@Z`, so the type and the "DSTGame" naming are proven. The `p` prefix comes from ChatGPT's raw record. ESTABLISHED has `?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A` (only one landed binder: 00cbe716). |
| 00cef3b0 | 0143e90c | `extern CDefPointer<const CScriptDef>pGlobalScriptDef;` (line 16) | CONFLICT_EST, and ChatGPT is right. The PDB dynamic initializer `pGlobalScriptDef` puts it at global scope. Retail 00cbf65a passes 0143e90c to GetDef<CScriptDef> (00cd2994), whose PDB parameter is `CDefPointer<const CScriptDef>*`, and neither PDB has a non-const `CDefPointer<CScriptDef>`. ESTABLISHED's `?pGlobalScriptDef@@3V?$CDefPointer@VCScriptDef@@@@A` lacks `$$CB` and no landed code binds it. ESTABLISHED should be corrected to `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A`. |

No source line needs to change for correctness. If the owner keeps the ESTABLISHED spellings instead, these lines change:
- 0065a075:63, 00699b00:105 and 008f7860:29 (`g_pMainGameComponent`).
- 00cef3b0:15 (`g_pGameScriptInterface`, with type `CGameScriptInterface*`).

For pGlobalScriptDef, keep ChatGPT's spelling.

Manual identity checks done in this audit:
- **0x00705170**: the const `GetPCarriedByThing`. It has a byte twin at 0x00705180, which is the non-const overload; the 16-byte spacing matches the Ego_r donor order.
- **0x009ADA10**: the protected mutable `GetPDefFromGlobalIndex`. The Ego_r donor has the const overload then the mutable one, 0x30 apart, the same spacing as retail 009ad9e0/009ada10.
- **0x0057F9E9**: the `void(stream&, T&)` overload; arg 2 is filled.
- **0x00A3D330 and 0x0055CF50**: copy constructors; they read a source object.
- **0x009110F0**: the `(CThingCreatureBase&)` ctor.
- **0x0099A300**: `~CBaseClassNonCopyable`. The order is ctor/dtor pairs at 2d0/2e0/2f0/300, and the dtor's own vtable store is elided.
- **0x00739360**: `<CCombatAbilityBlockProjectileWeaponAttackDef>`, taken from its callee GetSubDef 0x00738370.
- **0x00CEF3B0**: registered under the literal "CoreQuestReminder" (012c5d40).
- **0x0041CFB2**: the file-local `GetTextBySymbol`. Ego_r lists the undecorated name in the CManager TU with a 0x8a extent, which equals retail's 138 bytes.
- **Seven vtables**: each RTTI type descriptor matches exactly.

## NAME_FIXES (181 rows): 155 ACCEPT, 21 UNPROVEN, 5 CONFLICT, 0 REJECT

- **REJECT: none.** No row's evidence contradicts it.
- **CONFLICT (5), all with ESTABLISHED_SPELLINGS**:
  - 0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB` vs ESTABLISHED `?EPSILON@@3MB`. ChatGPT is a single raw source; keep EPSILON until the owner decides.
  - 0x013B8390 `?GPDisplayManager@@3PAVCDisplayManager@@A` vs ESTABLISHED `?PDisplayManager@@3PAVCDisplayManager@@A`. ESTABLISHED's evidence was the CSystemManager member name, not the global's name.
  - 0x013B86A0 PMainGameComponent, 0x0143E8F8 pDSTGame and 0x0143E90C pGlobalScriptDef, as in the table above.
- **UNPROVEN (21)**, all single-source data or SDK names:
  - GameCODA/SAL: 00c35e30, 00c3c4a0, 00c3d090, 00c438f0, 00c52e40 `CSalCpuInfo::Initialize`, 00c6b240 `sal_round`, 00c6b260 `_salFrameFloatToInt`, 0143bb48 `CODA_Error`, 0143bb58 `CODA`, 0143bc78/7c/7d `CSalCpuInfo::m_b*`.
  - Globals: 0129ba40 GD3DFormats, 013756f5 CombatParticleEffects, 01396fb0 D3DPolyCullModeTable, 013986c4 min_height_diff, 013b871c m_pNewFrontendGameComponent, 013ca814 `CFileInstaller::DirtyDisc` (protected), 013ce558 GFastOneOverSqrtTable, 01436e18 GPRenderManager, 01436e98 GPResourceManager.
- The rest are ACCEPT, including several data names that this audit lifted to two sources:
  - `PClass@CTBaseSingletonNoDefaultAlloc<CPlayerGui>` and `CWideString::NullString`: PDB dynamic initializers.
  - `CRenderStateCaptureBlock::StateManager`: the only setter, 00a04450, is the PDB `SetStateManager`.
  - `CThing::PCurrentThingManager`: PDB Get/Peek accessors plus the only writer, 004c74f0.
  - `DEFAULT_CAMERA_MODE_DEF`: PDB anonymous-namespace initializer.
  - `ConstantFPS`: Codex, ChatGPT and the PDB `GetConstantFPS`.
- The `??0CWideString@@QAE@PBG@Z` row is ACCEPT under the owner's 2026-10-29 wchar decision: retail's wide character is unsigned short, so the PDB `_W` spelling becomes `G`.

## Catalog labels proven wrong (`LABELS.tsv`, 55)

The most consequential, as address: wrong -> right:
- 00449b40: CEngine::GetEditorAnimation -> `?RemovePlayerCharacter@CPlayerManager@@QAEXJ@Z`
- 007f2100: CAIStateGroup_BuyFromSeller::BoughtSomething -> `?SetLifeTime@CTCLightningOrb@@QAEXM@Z`
- 004c9c10: CThing::SetAsGameStatePersisted -> `?SetThingScanInclusion@CThing@@QAEX_N@Z`
- 00705170: CTCCarryable::GetCarrier -> const `GetPCarriedByThing`; 00705180: CGuiControlListBox::GetCurrentSelectedEntryData -> non-const `GetPCarriedByThing`
- 00739360: ResetBlockPercentageChance<...HeavyWeapon...> -> `<CCombatAbilityBlockProjectileWeaponAttackDef>`
- 00cef3b0: CNostro::Main -> `NScript::CGameflowScript::CoreQuestReminder`
- 00b66c10: CGraphicInfo::GetFrame -> `CGraphicInfoFrame::GetTexture`
- 005e38c0: CAnimationSet::Add -> `CVectorMap<ulong,CAnimationEntry>::Insert`
- 005bb190: CNavigatorQueueManager::FrameUpdate -> `std::_List_base<CDefPointer<const CAppearanceModifierDef>>::clear`
- 0099a300: CBaseClass::~CBaseClass -> `??1CBaseClassNonCopyable@@MAE@XZ`
- 00525d90: AddTask -> `CThingManager::OnLevelUnload`
- 006e77f0: GetIDFromSymbol -> `GetScriptSpriteBySymbol`
- 009ad390: COptimisedPrimitive::ActualToUniqueIndex -> `CDefinitionManager::GetGlobalIndexFromClassIndex`
- 006e2500: Catch@0040d88c -> `~CParticleUpdateParameters<ulong,ulong>`
- 004aa840: ~C3DClothPrimitive -> `??1CScriptThing@@UAE@XZ`
- 00cbe0c0: SetCurrentThingManager -> `?SetDSTGame@@YIXAAVCGameScriptInterfaceBase@@@Z`
- Three `_Cons_val<...>` labels: 00487dd0 PeekPControlledCreature, 00643d5d ScaleAlpha, 00686d90 PeekThingSearchTools.

The rest are descriptive or vfunc_N labels.

## Landed sources binding a spelling this audit proves wrong (for the naming queue)

Priority items, where the landed spelling is a different real function or a wrong signature:
- **0072f170** binds `??$GetDef@VCDefClassBase@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCDefClassBase@@@@@Z` at 0x0042AF3C. It should be `??$GetDef@VCThingBaseDef@@...` (queue-settled; the GetDef<T> body does not depend on T, so the template argument comes from the callers).
- **0083b7f0** binds `??1CTCParamsPhysicsBase@@UAE@XZ` at 0x0099A300. It should be `??1CBaseClassNonCopyable@@MAE@XZ`.
- **00847ac0** binds `??1?$CIntelligentPointer@VCThing@@@@UAE@XZ` at 0x00A01C10. It should be `??1CBaseIntelligentPointer@@UAE@XZ`.
- **004917b5** binds `??0CWideString@@QAE@PB_W@Z` at 0x0099B6B0. It should be `PBG` (owner wchar decision).
- **006e7740** binds `?SetGameScriptInterface@@YIXPAVCGameScriptInterface@@@Z` at 0x00CBE0C0. It should be `?SetDSTGame@@YIXAAVCGameScriptInterfaceBase@@@Z`.
- **0041e61b** binds `?ShowErrorMessage@CManager@NUISystem@@QAEXJ@Z` at 0x0041DB84. It should be `...@@QAEXW4ERRORMESSAGE_TYPE@@@Z` (the PDB's only overload).
- **008918c0** binds `?GetTextIDFromSymbol@CGameScriptInterface@@QBEJABVCCharString@@@Z` at 0x006E77F0. It should be `GetScriptSpriteBySymbol` (already queued as mig6).
- **005fd410** binds `?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@2V...` (public). Two independent raw-Ego_r readings (Codex and ChatGPT) give private `0V`, and template static access is uniform across instances. Queue c674's "both PDB name sets" claim is unfounded, because data symbols are not in the name sets. Owner decision.
- **00d15a40 and 0122ae7e** bind `?pGlobalScriptDef@@3PAVCScriptDef@@A` and `?pGlobalScriptDef@NGlobalScriptDefs@@3V?$CDefPointer@VCScriptDef@@@@A`. Both should be `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A`.

`NAMING_QUEUE.tsv` holds the full mechanical list: 1,236 stand-in spellings at 170 proven addresses.

## Owner decisions needed

1. **ESTABLISHED_SPELLINGS vs outside raw-PDB readings**:
   - 0x013B86A0: PMainGameComponent vs g_pMainGameComponent. Recommend PMainGameComponent.
   - 0x0143E8F8: pDSTGame vs g_pGameScriptInterface.
   - 0x0143E90C: correct ESTABLISHED to `$$CB`.
   - 0x0129BA3C: EPSILON vs DELTA_NOTIONAL_ZERO.
   - 0x013B8390: PDisplayManager vs GPDisplayManager.
2. **Single-source names.** Should data and local-static names read only from an outside pass's raw Ego_r records, plus native identity, count as landable? The Codex precedent (VIRTUAL_SCREEN_HEIGHT, codex50) accepted them. This decides 10 bindings in 8 rows.
3. **GameCODA SDK names (00c35e30)**: these are external vendor identifiers with no Fable-PDB coverage.
4. **Tooling quirk (not naming).** vland warns "leaf 'New<C3DAnimationSequence>' matches no symbol" for 00a533f0. It matches line 1's demangled name; the mangled `// Leaf:` symbol is present in the object (ChatGPT symbols.txt).
