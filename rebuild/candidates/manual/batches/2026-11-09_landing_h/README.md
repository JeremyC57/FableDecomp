# Landing 2026-11-09h: chunk 895, six of the easiest parked rows

The user asked for the easier rows among the parked pool, worked by the same expert-agent brief as chunk 894. The coordinator
re-graded 358 small parked drafts (<= 600 bytes, within 4 bytes of retail) exactly as they sat, against today's headers, and gave
the agent the 31 with the smallest measured residue: 2 drafts that already graded RELOCATION_MATCH but were parked on a name,
23 near misses of one to five masked lines, and the six-row CTCVillage::RemoveThingFromTask family (optional).

Six landed: 5 RELOCATION_MATCH + 1 MATCH, every behaviour test PASS under VC7.1 (vland `/Od`) and g++ `-std=c++03`; no
`__asm` / `naked` / `_emit` / `volatile`, no bare `__forceinline`, no flags, no pragmas.

| Address | Row | Grade | Lever |
|---|---|---|---|
| 0x006F3570 | CThingSearchTools::GetAllThingsInRoughArea<CThingObject, vector<const CThing*>, CThingFilter_None> | RELOCATION_MATCH | the landed 0x00436D60 shape (chunk 894) with the instance swapped; identity from the type mask 0x807, the shared caller 0x006EB101 (same vector object for both searches) and the FableWin.pdb instance order |
| 0x006F37B0 | CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding, vector<const CThing*>, CThingFilter_None> | RELOCATION_MATCH | same; type mask 0x47 |
| 0x00807620 | std::__push_heap<pair<C3DVector,long>*, int, pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting> | RELOCATION_MATCH | the shared CVectorCompare_BestForTargeting header with `_BODIES`, as the landed `__median` 0x00807170 of the same TU (no capture of the comparator) |
| 0x007FA160 | std::__push_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2> | RELOCATION_MATCH | same lever |
| 0x00619560 | GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo> | RELOCATION_MATCH | `CPersistTraits<T> traits; out.resize(count, traits.DefaultVal);` (one object is the resize default and TransferBinaryIn's `this`); rewritten on the shared STLport vector, CDataInputStream, CPersistTraits and CTCInventoryMap headers (the October CRetailVector stand-in and bare `__forceinline` are gone) |
| 0x00BEBAA0 | CEngineInternalPrimitiveMeshBase::CalcObjectMatrix | MATCH | the shared `C3DVector::operator^` through `CRightHandedSet::GetRight` (cross-product z-term order) + three parenthesised negations (PAREN tuples keep `pop esi` in the epilogue) + the new `CMatrix3x4::SetTranslateOnly` |

Header: `rebuild/include/engine/CMatrix3x4.h` gains `SetTranslateOnly(const C3DVector&)` (Ego_r.pdb / FableWin.pdb
`?SetTranslateOnly@CMatrix3x4@@QAEXABVC3DVector@@@Z`, inline in retail) behind the new opt-in
`FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY`; `hdrcheck2.py` prints `default=same` and every other CMatrix3x4 opt-in `same`, and no
landed source defines the new macro, so no includer recheck was needed.

Names: catalog and manifest names for these six addresses only (`ghidra_out/labels_landing_2026-11-09h.tsv`). The catalog's
`GetPBestThingInRoughArea<...>` labels on 0x006F3570 / 0x006F37B0, `<CBrainUpdateZone>` on 0x00619560 and the Dinkumware
`_Push_heap` labels on the two heap helpers were wrong. New binder: `CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryIn`
0x00614DA0 (both PDB name sets; already the catalog name).

Warnings at landing (non-blocking):
- RELOC_NAME x2 on each search row: 0x0049C6F0 (`GetAllThingMapsInArea`) and 0x0049C700 (`ClipAndConvertWorldMapAreaToLocalArea`)
  are already queued (`c617`, `land0411s`), as for chunk 894's four instances.
- SHARED_HEADER_AVAILABLE on the two search rows (CVectorMap, CBaseClass, C2DBoxI, C2DExtentsI): the chunk-894 shape keeps its local
  models; header debt shared with 0x00436D60 and its siblings.
- `bindings_multi_check.py` prints 1 (CScriptThing::NullCoord over three addresses), unchanged from HEAD.

Parked (25, `pending/2026-11-09_linux_hand895/`, best sources in `differ/`, residues in STATUS.tsv / NOTES.md): 0x00B801F0
(x_copy load order Z,X,Y v X,Y,Z), 0x006D6CB0 / 0x006D68D0 (clamped store v `fmul [0.3]`), 0x0074E140 and 0x00A56670 (return copy
scheduling; extra parentheses move 0x00A56670 the wrong way), 0x006D8830 (`push 1` v `fst`), 0x0099BB80 (needs CWideString header
work), 0x00A358F0 / 0x00A22CA0 (cross-product z order; 0x00A22CA0 needs the full shared-header rebase), 0x00C3D030 (out-pointer
hoist), 0x007BC6FA, 0x00868E50 (local models first), 0x00E5E350 (dead zero store, likely another object's ctor), 0x00C6B9D0 (the
draft uses `__asm`: ASM_RULES), graded once only: 0x008EEBE0, 0x00BA48F0, 0x00A65720, 0x00D9EB30, 0x00826C90, the six
RemoveThingFromTask rows.
