# hand4e corrections to rows already landed from batches 1-3 (2026-11-19). Each checked with q.py (overlay as noted) + the behaviour test
# (mt.sh, VC7.1 /Od). vland skips landed rows, so it cannot recheck these: the coordinator's landing run must.
1. 0x006379D0 NThingMap::InitLayerInfo -- calling convention. Both PDBs spell ?InitLayerInfo@NThingMap@@YIXXZ (__fastcall); the landed
   row used YAXXZ (cdecl) only because the no-argument body cannot show it. Fix: FABLE_FASTCALL on the landed opt-in declaration
   (hdr/engine/NThingMap_CThingMap.h, one line: FABLE_RETAIL_CTHINGMAP_INITLAYERINFO) and on the definition (src/006379d0.cpp, Leaf
   ?InitLayerInfo@NThingMap@@YIXXZ; test updated). RELOCATION_MATCH + PASS. The caller 0x00638540's binding should then read YIXXZ
   (NAME_FIXES.tsv updated).
2. 0x00AAC5E0 CSoundEmitterLH::GetActiveSampleInfo -- the shared engine/CSoundManagerLH.h exists (SoundSystem +0x40); the landed row (like
   every landed CSoundEmitterLH sibling) declared a local CSoundManagerLH with m_pSoundSystem. Corrected src/test include the shared header
   and read PManager->SoundSystem: RELOCATION_MATCH + PASS. Also adds the `// Inline-budget:` line for STLP_VECTOR_PUSHBACK_OUTOFLINE
   (without it VC7.1 expands push_back: DIFFER(267v136)).
3. 0x009B9D70 (no change made, coordinator's call): the landed nested opt-in FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS_EGOD copies
   FABLE_FORCEINLINE from the pre-existing MOVESTRINGPOS helpers; with plain `inline` helpers (hdr/engine/CStringParser.h.plain_inline_proposal)
   the row still MATCHes, so the forcing is not required by this row. Either drop FABLE_FORCEINLINE from the EGOD opt-in or keep it and
   note it as inherited.
