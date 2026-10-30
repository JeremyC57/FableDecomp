# Chunk 588 (2026-10-30): 9 of 10 engine-range released pass 9 rows landed

1 MATCH (zlib updatewindow) + 8 RELOCATION_MATCH, all PASS (g++ and VC7.1). Rows: zlib 1.2.1 updatewindow, libvorbis
mdct_bitreverse, three STLport _M_fill_insert instances (C3DQuad2 /GS, pair<CCharString,unsigned long>,
CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator), CEngineScene::RenderSceneOcclusion (real STLport map model),
CReverbDiffuser::ProcessRamped (descriptive, libpk), strmbase CMemAllocator::SetProperties, ImeUi CheckInputLocale (/GS).
Parked (../../pending/2026-10-27_linux_hand588/differ/): 0x00C071B0 LZO 1.08 swd_accept (279v279, prologue shrink-wrap
order only: retail tests n before the pushes; 4 forms tried; next: copy n to a local before the test).
Several TUs carry same-TU helper bodies that map to their own retail addresses and are not landed (z_inflateSetDictionary,
the head of mdct_backward, static _strtoul, noinline STLport helpers / map::operator[]); the lander graded the leaf only.
NAME_FIXES.tsv: 19 rows queued, incl. respellings of landed rows (0x009B71B0 QAE->IAE, 0x00B97B60 key EEngineRenderLayer,
0x00B93F40 RenderScene, 0x00B94060 RenderSceneShadowBuffer), 0x0099EC30 CCharString copy ctor (~85 bound spellings), and the
ImeUi globals 0x0143E8C0 / 0x013AC74C whose names rest on the SDK source from memory only (UNCONFIRMED: second source
needed before the naming wave applies them). Header gaps (NOTES.md): no headers for CEngineScene*, CCountedPointer, CArray,
C3DQuad2, CCharString; CEngineScene's Ego_r layout has Dinkumware map sizes (members after MapList sit 4 lower in retail);
CEngineInternalPrimitiveParticleGroup.h uses struct (PDB: class), lacks the nested CPrimitiveIterator, flattens
PrimitiveIDToIterator. Landed CEngineScene siblings 0x00B93F40 / 0x00B94060 / 0x00B948E0 still use flattened tree models and
an invented CRenderStatsMap; +0x44 is CurrentOcclusionMask, not a "current-cell marker".
