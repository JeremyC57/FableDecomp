# 2026-11-16 landing nf46: name-fix round 7, NF46 (74 rows ported onto the engine-data global headers, plus one caller)

Owner: "Continue the naming fix queue with two agents until complete".

Round 7 cuts the queue by class family. NF46 took the 84 landed rows that read five `NEngineInternal` engine-data globals through
stand-ins:
- GPPrimitiveRendererSharedData 0x01436E78;
- GPPrimitiveTypeInfo 0x01436E84;
- GPEngineWorldMap 0x01436E8C;
- GPResourceManager 0x01436E98;
- GPCamera 0x01436EA0.

NF45 (landed 8872d59ed4) had the render globals; NF47 runs beside NF46 on other families. Brief, worklist and records:
`naming/2026-11-16_agentNF46/`.

## Rows (84, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 74 |
| not-a-binder | 1 (0x00B6B0B0 already binds GPEngineWorldMap) |
| blocked | 9 (one grade each; rows unchanged) |

- **Blocked, authoring (7):** July behaviour lifts on `candidates/*_compiled.h` that grade DIFFER unchanged.
  - Entry 35: 0x00B687D0, 0x00BE7D70, 0x00BF4130 and 0x00BFE050.
  - Entries 37 / 79: 0x00BDC510, 0x00BF4220 and 0x00BF8810.
- **Blocked, port (2, entry 34):**
  - 0x00B7C510 needs CRGBFloatColour, C3DVector, C4DVector, CMatrix3x4 and CArray moved to their headers.
  - 0x00B7FA20 needs those plus CArray and CShaderRenderManager.
  - The GP spelling for both is written in STATUS.tsv.

## Entries (APPLIED.tsv)
- **done:** 32 (GPPrimitiveRendererSharedData) and 33 (GPPrimitiveTypeInfo).
- **blocked:** 34, 35 and 37 / 79. Only the 9 rows above are left in them.
- **deferred:** 36 / 78 (GPLightingManager). Their remaining binders were NF45 rows.

## Rows and checks
- **Self renames (RENAMES section 1, 6 rows; 2 file moves):**
  - 0x00BC5A20 RenderPrimitiveToShadowBuffer (now virtual, returns `ERendered`);
  - 0x00B51F60 CEngineWeatherRenderer::ClipBoxToCamera (was the stand-in CWeatherRendererMethods);
  - 0x00B676A0 OnReleaseDefaultPoolResources (now virtual);
  - 0x00B23F60 / 0x00B25C80 / 0x00B25CC0: `CEngine` overrides of CIEngine slots 71-73.
- **Caller fixed by the coordinator (section 2):** 0x00B52120 ClipLockedBoxToCamera declared its callee as public
  `long ClipBoxToCamera(C3DVector*, float)`. That declaration, inside the row's pre-existing local CEngineWeatherRenderer model,
  now spells the PDB private const form returning `EWeatherBoxVisible`, with the header's enumerators.
  - The row's own comment explains why it keeps a local layout: the lock byte at +0x11A differs from the Ego PDB layout.
  - Its object now references `?ClipBoxToCamera@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@M@Z`. This is
    the only spelling reloc_bindings has.
- **Recheck (`logs/recheck_part0..2.log`):** all 75 changed rows (NF46's 74 plus 0x00B52120) graded from the repo after landing.
  All 75 are RELOCATION_MATCH, every test PASS.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 186 binding lines and added 139;
  - bindings_multi_check is at its baseline (`NullCoord`).
- **Fitted spelling:** 0x00BAD670 converts the frame width signed, `(float)(long)GetWidth()`, because retail converts it with
  `fild dword`. Its neighbour 0x00BAD8A0 keeps the plain unsigned conversion its retail uses.

## Headers (opt-ins in 9 files; default path unchanged)
- **Opt-ins added:**
  - CEngine.h: `_GETSCRATCHBUFFERTEXTUREMANAGER`, `_GETMEMORYPOOLS`;
  - CEngineResourceManager.h: `_GETLOADINGMEMORYPOOL`, `_PEEKANIMATIONMEMORYPOOL`;
  - CHandleBasedMemoryPool.h: `_ALLOCATE`;
  - CGPUScratchBufferTextureManager.h: `_ALLOCATEDEPTHBUFFERSURFACE`;
  - CEngineWorldMap.h: `_VISIBLEMAPLIST`, `_GAMEWORLDMAP`;
  - CEnginePrimitiveTypeInfo.h: `_GETPRIMITIVEMANAGER`;
  - CEnginePrimitiveRendererSharedData.h: `_DTOR`;
  - CMeshDataBank.h: `_PEEKPARTICLEMESH`, `_PEEKREPEATEDMESH`;
  - CEngineLandscapeRenderer.h: `_ONRELEASEDEFAULTPOOLRESOURCES`.
- **Existing lines edited (3):**
  - one `#if` condition line in CEnginePrimitiveTypeInfo.h (the only line `land_naming_prep.sh` reports removed);
  - padding lines in CEngineWorldMap.h.
  - hdrcheck2 shows no default-path change in any of the nine files.

## Name fixes queued (`land1116fu`)
- 0x00B401E0 is CEngineWorldMap::OnGameMapLoaded (landed as CEngineMapManager::MapStateSetLoaded).
- The three InitialiseTextures shells: 0x00B5BF10, 0x00B5ECE0 and 0x00B86A70.
- 0x00B7C510: settle Background vs Midground.
- 0x00B42750 still binds 0x00B3E820 as `CStaticMapBankFile::SetMemoryPool`.

## Notes
- **Renaming macros:** five rows still carry renaming macros that NF46 did not add: 0x00B90270, 0x00BCA770, 0x00BCC5F0, 0x00BBF380
  and 0x00BC3E10. That is row debt.
- **Raw offset:** 0x00B90270 still reads +0x29C8 by raw offset.
- **Rule slips:** two `ls` probes without `; true`, which exited 2.
