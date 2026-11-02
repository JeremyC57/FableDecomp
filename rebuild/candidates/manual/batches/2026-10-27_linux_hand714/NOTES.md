# Hand chunk 714 (CTCActionUseScriptedHook .. CDarkwoodTrader): 10 of 10 byte-exact

All ten rows are RELOCATION_MATCH on the first source shape, with tests PASS under g++ and VC7.1.
- Read-only vland: 10/10 RELOCATION_MATCH PASS, no warnings (`tmp/vland.log`).
- Each `// Leaf:` was confirmed with `objdump -t`, and every call target was checked against retail.
- Every extent ends in its tail `jmp` followed by int3 padding, so there are no extent fixes.

## Rows
| addr | symbol | bytes | shape |
|---|---|---|---|
| 00729A50 | ??1CTCActionUseScriptedHook@@UAE@XZ | 54 | user dtor `{ RemoveFromGlobalTCs(); }`, full Ego_r members (0x50) |
| 00729CA0 | ?GetRegionEntranceDisplayName@CTCActionUseScriptedHook@@QBEABVCCharString@@XZ | 75 | PeekWorldMap / GetMapNumberAt / GetRegionNumberMapIsIn / GetRegion(...).GetDisplayName() |
| 007A66F0 | ??1CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@UAE@XZ | 79 | user dtor: `if (!ParentThing->IsDead()) ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);` |
| 007CF0B0 | ??1CTCCreatureTintEffect@@UAE@XZ | 86 | empty user dtor; STLport vector<CIntelligentPointer<CThing> > |
| 007E1510 | ??1CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UAE@XZ | 95 | as 007A66F0 plus vector<CPrimitive*> |
| 007ADAA0 | ??1CTCAIScratchpad@@UAE@XZ | 73 | implicit dtor (PDB ctor defined in the TU) |
| 009BF500 | ?PreDeviceReset@CDisplayManager@@AAEXXZ | 66 | scoped_ptr + STLport list walk, tail OnPreDeviceReset |
| 00A628D0 | ??1CShaderBankEntry@@UAE@XZ | 79 | implicit dtor (PDB ctor defined) |
| 00AA4170 | ??1C3DAnimationManager@@UAE@XZ | 75 | implicit dtor (PDB ctor defined) on the CSimpleMemoryPool retail branch |
| 00E01710 | ??1CDarkwoodTrader@CQ_TraderEscortScript@NScript@@UAE@XZ | 65 | implicit entity-script dtor (CBriarRose recipe) |

## Names and evidence (two sources each; detail in each source header)
- **Destructors.** Each one is proven by two things:
  - the vtable it stores (user dtors) or the slot-0 ??_G caller (implicit dtors), with RTTI;
  - the Ego_r.pdb / FableWin.pdb symbol.
- **??_G names.** Each TU's emitted ??_G graded RELOCATION_MATCH against the retail slot-0 caller: 0x007A6DF0, 0x007CF390, 0x007E1AA0, 0x007ADA80, 0x00A628B0, 0x00AA4150 and 0x00E016F0. That proves their names.

## Label corrections (catalog / manifest)
| Address | Wrong label | Correct identity |
|---|---|---|
| 0x007CF0B0 | ~CTCGraphicAppearanceReplaceableMesh (propagated across byte twins) | ~CTCCreatureTintEffect |
| 0x007E1510 | ~CTCGraphicAppearanceReplaceableMesh (propagated across byte twins) | ~NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes |
| 0x007ADAA0 | ~CTCTargetingBase | ~CTCAIScratchpad |
| 0x00A628D0 | CEnginePrimitiveRenderer::ResetVisibilityRatios | ~CShaderBankEntry |
| 0x007A66F0 | class without its namespace | ~NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh |
| 0x00E01710 | bare CDarkwoodTrader | NScript::CQ_TraderEscortScript::CDarkwoodTrader |
| 0x009A52B0 | sub_ | ?OnPreDeviceReset@CSystemManager@@QAEXXZ |
| 0x009A52C0 | CThingFilter_IsUnconscious::operator() / ReturnTrue | ?OnPostDeviceReset@CSystemManager@@QAE_NXZ |
| all 7 ??_G callers | manifest "CLandscapeBackgroundPatch vector_deleting_destructor" | the ??_G of each row's class |

## NAME_FIXES (NAME_FIXES.tsv, 22 rows)
- 0x009BF500 / 0x009BF550 CDisplayManager::Pre/PostDeviceReset are private (`AAE`); the landed ResetDevice 0x009BFB30 spells them `QAE`.
- 0x00686D20 ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ, spelled three wrong ways by landed callers (GetWorld, GetRegionManager, ComputeFlushGround).
- 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ, which has 8 stand-in spellings.
- 0x009A52C0 ?OnPostDeviceReset@CSystemManager@@QAE_NXZ (landed caller 0x009BF550 binds `?OnDisplayDeviceReset@CSystemManager@@QAEXXZ`).
- 0x009A52B0 ?OnPreDeviceReset@CSystemManager@@QAEXXZ.
- The seven destructor rows, whose landed ??_G callers use stand-ins (?Dtor@..., @X_dtor@4, @CLandscapeBackgroundPatch_dtor_00e01710@4).
- The seven ??_G rows themselves.
- 0x00A5C570 ??0CSimpleMemoryPool@@QAE@KK@Z and 0x0099A310 ??0CInitBaseClass@@QAE@XZ (stand-ins in landed 0x00B260B0).
- First bindings with no conflict: 0x00A0A9D0 ?Uninit@CRenderManagerCore@@QAEXXZ and 0x009F73A0 ?OnPreDeviceReset@CVsyncCallbackTimer@@QAEXXZ.

## Header corrections (hdr/, see hdr/README.md)
- **engine/CTCBase.h:** `PeekWorldMap` added to the retail branch. This is additive, and the overlay is rebased on chunk 712's `RemoveFromGlobalTCs` line.
- **engine/CSimpleMemoryPool.h:** new opt-in `FABLE_RETAIL_CSIMPLEMEMORYPOOL` branch with the polymorphic retail class (0x1C).
- **Includer recheck:** all 52 includers still MATCH/RELOCATION_MATCH + PASS with the overlay.

## Header debt (local models used, all at retail offsets / sizeof)
- **CWorldMap.h** is flattened. 00729CA0 uses a local CWorldMap (RTTI bases, slots 0..19, 0xD8).
- **CRegion.h** has no member functions. 00729CA0 uses a layout-rename plus GetDisplayName.
- **CRenderManagerCore.h** is a flattened `struct`. 009BF500 uses a layout-rename.
- **No header exists** for:
  - CDisplayManager, CThing (retail 0x98, used opaque), CIntelligentPointer / CBaseIntelligentPointer and CAGraphicAppearanceComponent;
  - the TC classes CTCActionUseScriptedHook, CTCGraphicAppearance*, CTCCreatureTintEffect and CTCAIScratchpad;
  - CShaderBankEntry and C3DAnimationManager;
  - the STLport vector / list and boost::scoped_ptr. Each TU carries the minimal STLport 4.6 members it expands.
- **fable_string.h** CCharString is usable as is.

## Skipped
None.

## Decisions for the coordinator
1. Merge hdr/engine/CTCBase.h together with chunk 712's hand-back of the same file. The repo copy changed mid-session (05:56, RemoveFromGlobalTCs), and this overlay already contains that line.
2. Land the CSimpleMemoryPool retail branch. 00AA4170 requires it, and it is invisible to existing includers because it is opt-in.
3. Respell the landed callers listed in NAME_FIXES.tsv. The ones that touch these rows directly are 0x009BFB30, 0x009BF550, 0x007E27F0 and the seven ??_G sources.
4. The ??_G rows use stand-in names in landed code. Re-landing them from these TUs would retire those stand-ins.
