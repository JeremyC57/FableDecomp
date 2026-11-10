# Chunk 1013b notes

**Header overlay:** every grade (q.py, g.sh, gall.sh) ran with `FABLE_HDR_OVERLAY=$A/hdr`; see hdr/README.md for each opt-in.
Result: 1 RELOCATION_MATCH (0x00507650), 12 parked in differ/.

## Matched
- **0x00507650 ?LoadWorldFromBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z** (catalog: LoadFromFile; NAME_FIXES). The whole file goes
  into a CArray<char> buffer of length+1 (CDiskFileWin32 read, mode 2) and is read through a CMemoryDataInputStream. Then the map
  infos and the regions are loaded, entry 0 of each staying the default. Each map gets SetMapPlacement(Maps[m].Area, m) and
  CEngineManager::Get().AddMap(m). What made it match:
  1. the new shared CTBaseSingleton.h / CEngineManager.h. Retail expands Get here, with PClass at 0x013BA854 and the DeleteData
     deleter 0x004361E0;
  2. the CRegion / CMapInfo LoadBinary declarations;
  3. FABLE_RETAIL_CARRAY plus the new STLport `fill(char*)` / `fill_n(char*)` overloads (FABLE_RETAIL_STLP_FILL_CHAR). Without
     them vector<char>(n) is called out of line.
  Test: test/00507650.cpp (stub model of the loader, PASS).

## Parked (differ/)
- **0x00B43720 PeekThemeId, 0x00B43870 PeekThemeBlend, 0x00B439C0 PeekLandscapeHeight**: DIFFER 320v324 / 320v324 / 352v356, one
  block placement each.
  - All three are one shape. The `||` outer test gives a 0.75 then-frequency, so the inline CEngineWorldMap::GetMap(x, y) gets
    expanded. Inside it, MapHasDataAtPoint is inline and the self tail call becomes a loop. There is one GameMap call site.
  - Every instruction, register and slot is retail's, except that retail places the reload block for GetMap's x<0 / y<0 exits
    (`mov edi,[x] ; jmp`) before the other exits' reload block. Ours places it after the clamp's `x = 0 ; jmp`, so retail's two
    `jl` are long.
  - Inert (probes): GetMap forms (single return, nested tests), outer-test and clamp spellings, tail-return forms,
    inline helpers.
  - Next: the block-order decision for compensation blocks on split edges (c2 places them by creation order). Find what creates
    the x<0 split edge first in retail, for example GetMap's tests in another order inside a different predicate shape.
- **0x00B3FC40 PeekInterpolatedMapNormal**: DIFFER(528v528), frame slots only.
  - Every instruction, call and register matches. The frame is 0x60 against our 0x54. n11 / n01 are swapped (-0x30 / -0x24),
    and retail gives the term temporaries their own slots where we reuse dead corner slots.
  - The four terms evaluate right to left (n11 first), so the sum is one expression.
  - Needs a Retail-asm line for GFFloatToLongFloor (present).
  - Next: the frame packer's tie order (density sort). Try spellings that change the corner normals' reference counts.
- **0x00B41770 PeekInterpolatedWaterHeight**: DIFFER(402v777).
  - Retail expands the same-TU PeekLandscapeHeight into the averaging loop. VC7.1 will not do it with the plain or `inline`
    spelling, defined before or after.
  - Probe with FABLE_FORCEINLINE on the same-TU body: 26 hunks. Retail keeps MapWidth in [esp+2Ch] and the frame is 0x24 against
    0x20. That is not offered as the model, because retail's PeekLandscapeHeight also exists out of line at 0x00B439C0.
  - Next: an owner call on whether a forced inline of a same-TU body is acceptable. Otherwise this is an inline-budget question,
    like the /d2inlS / T rows in GOTCHAS.
- **0x0063AB90 GetThemeFaceNormalAt**: DIFFER(582v582), one store.
  - Fixed this chunk: the x87 pops and the frame. The per-branch fractional expressions are hoisted temporaries (faddp/fcompp),
    and the position is copied into a C2DVector.
  - Left: retail writes the first C2DCoordI's X right after the first _ftol (`mov [esp+1Ch],ebx` 0x0063ABBD); ours writes it
    after the second.
  - Inert: building the coordinate member by member or from the casts (both move the store early but create a long-lived cell
    object: 585-587 bytes), and a named cell after ix/iy.
- **0x004FEB70 HeightAreaChanged**: DIFFER(260v261).
  - Rebased onto the shared CEngineManager.h, so the local singleton model is gone.
  - c2prio: it2 (priority 54) takes ebx because ebp costs it 200; its neighbour `it` (48) has only ebp left. Retail colours
    `it` first and keeps const 0 in ebx (ours: priority -12).
  - Inert: see the differ header. Next: a spelling that makes const 0 hotter or `it` heavier without a reload.
- **0x0063B3F0 CreateLineSegment** (prior, not regraded).
  - Probe lead: `if ((long)end.Y == toY)` (end = *from) in the first Y compare stops the CSE. That gives retail's third from->Y
    _ftol, all 9 calls and the same instruction stream (845v856, 45 hunks).
  - The compare then loads end.Y from the frame where retail loads [esi+4]. The frame slots (bools, xHit / yHit / end) are still
    open.
  - Inert: an `(int)` cast on yHit, xHit through a pointer, an unnamed to->Y, a copy-constructed end (808).
- **0x0063D0A0 / 0x0063D4A0** Find{Ground,Theme}CollisionPointWithinCell: priors unchanged (x87 scheduling, 1017v1017), not
  regraded.
- **0x00500540 LoadRegion / 0x00507070 BuildStaticMapGenerationVisibilityInfo**: priors unchanged, not worked.
  - Header debt: the priors model CRegionDef, CGameTimeManager, CTCDParticleEmitter, CAsyncLoadingMemoryPool, CGlobalDef and
    CStaticMapGenerationVisibiilityInfo locally. No shared header exists for them yet (CGlobalDef only in _quarantine).

## Callee names (two sources each; LABELS.tsv)
- CWorldMap::SetMapPlacement 0x004FC9C0, CMapInfo::LoadBinary 0x004FB4F0, CRegion::LoadBinary 0x006BC510
- CEngineManager::AddMap 0x006403A0, CEngineManager::AddChangedArea 0x00640560
- CTBaseSingleton<CEngineManager>::Get 0x00436FB0 and its PClass 0x013BA854; CCountedPointer<CEngineManager>::DeleteData
  0x004361E0 (FableWin only, plus the byte match)
- CEngineMap::PeekMapNormal 0x00B3F5E0, PeekHasWater 0x00B412A0, PeekWaterDepth 0x00B40BD0
- g_pEngineWorldMap 0x01436E8C keeps the landed CEngine forwarders' spelling. It has several spellings in the bindings; the PDB
  name is not settled here.
