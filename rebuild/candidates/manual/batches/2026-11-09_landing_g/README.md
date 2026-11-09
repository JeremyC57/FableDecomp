# Landing 2026-11-09g: chunk 894, landed-twin quick wins (14 rows)

Chunk 894 (expert agent; brief and hand-back `pending/2026-11-09_linux_hand894/`) took 14 unlanded rows whose retail bytes equal an
already-landed row's bytes modulo relocations (found by `lane_tools/coordinator/landed_twins.py`) and 9 one-line near misses. 14
match: 13 twins + 0x00C720C0. 13 RELOCATION_MATCH + 1 MATCH, every behaviour test PASS under VC7.1 and g++; no asm, no device; the
only flag is /GS on 0x00C6EEF0, as its twin. vland dry run and landing logs in `logs/`.

| Address | Row (proven identity) | Twin | Lever |
|---|---|---|---|
| 0x00436D60 | CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CThingFilter_OverlappingCircle> | 0x0096BCB0 | filter swapped (the only differing relocation is the filter call) |
| 0x00863E70 | ... <..., CThingFilter_HasPhysicsMesh> | 0x0096BCB0 | same |
| 0x008ADCC0 | ... <..., CThingFilter_HasTC> | 0x0096BCB0 | same |
| 0x00978250 | ... <..., CThingFilter_HasScriptName> | 0x0096BCB0 | same |
| 0x004AD810 | CThingSearchTools::GetAllThings<const CThing, vector<const CThing*>, CThingFilter_None> | 0x008093D0 | twin body on real STLport models |
| 0x00702D10 | CThingObject::DrawGetEditDrawInfo (vtable 0x01264DCC slot 25) | 0x0083D710 | twin |
| 0x00739030 | CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockUnarmedAttackDef> (GetSubDef 0x007380D0 pushes that class name) | 0x00739140 | twin |
| 0x00772490 | CTCShop::IsBarberShop (CShopDef +0x99) | 0x00772510 IsTattooShop | twin |
| 0x007921A0 | CUnprovenRbTree_007921A0::begin (14 B, MATCH; no reference in the exe; probably an STLport _Rb_tree::begin COMDAT, owner unproven) | 0x00CD2790 | twin |
| 0x00B84190 | CEngineClothSimulationAnimatedMesh `scalar deleting destructor' (vtable 0x012A39C0 slot 0, RTTI) | 0x00B84120 | out-of-class empty dtor + ctor in the TU |
| 0x00C6EEF0 | CVtable_012b8a8c::Process (libpk SSE2 gain-ramp mixer, descriptive) | 0x00C6FE10 | same-TU kernel body, /GS |
| 0x00F28990 | std::vector<CHeroSoulsWaveDef>::_M_fill_insert | 0x00F34D70 | twin |
| 0x00F3FB60 | std::vector<CArray<unsigned short> >::resize(size_type) | 0x00A4B1D0 | twin |
| 0x00C720C0 | CAudioSource::StartExponentialFade (stretch) | - | named float local + float quotient (x87 compare order) |

Parked (best drafts in the pending dir, residues and next ideas in its STATUS.tsv / NOTES.md): 0x009AA440 (second COMDAT copy of
the landed ~pair 0x009A8FE0 with no references -- needs an owner rule on unreferenced per-TU COMDAT twins), 0x00A17620, 0x00D259A0,
0x00492ED1, 0x00C11520, 0x008C7280, 0x009350C0, 0x009E1960 / 0x009E1A80.

Names: catalog + manifest set to the proven names for these 14 addresses only (readable form; labels for Ghidra in
`ghidra_out/labels_landing_2026-11-09g.tsv`). Queued PRIORITY `land0911g`: the 0x00C72E00 binder's `StartFade` spelling of 0x00C720C0,
and the 0x004A9EF0 binder's `CThingFilter_HasTC::operator()(Thing*)` spelling of 0x006A80A0.

Warnings at landing (non-blocking): SHARED_HEADER_AVAILABLE on rows that keep their landed twins' local models (CVectorMap,
CDefPointer, CDefString, C2DBoxI, ...); RELOC_NAME on the area-search rows for 0x0049C6F0 / 0x0049C700, whose catalog labels are
placeholders (`Forward_CThingMapManager_...`, `GetCellExtents`) while the rows bind the PDB names GetAllThingMapsInArea /
ClipAndConvertWorldMapAreaToLocalArea, as the landed twin 0x0096BCB0 does. Seven sources keep the twins' `#ifndef _MSC_VER` /
`#define __forceinline inline` g++ shim (check_portable_spellings: 0 bare). bindings_multi_check: 1, the pre-existing NullCoord entry.
