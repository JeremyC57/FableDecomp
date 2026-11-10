# 2026-11-10 landing be -- chunk 1022: 4 of 8 fresh rows (one phantom, one parked, two not graded)

Source: `pending/2026-11-10_linux_hand1022/` (NOTES, STATUS, LESSONS, LABELS, NAME_FIXES, SPLITS, ATTEMPTS, differ/).
vland --land: 4 wins (RELOCATION_MATCH), tests PASS.
- 0x00A02DF0 ?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z (585 bytes): same-TU callee bodies defined before the row.
- 0x00418289 ?Update@CMainGameComponent@@MAEXXZ (flags /O1 /Oy- /GS, size-TU pragmas): calls the new shared retail-asm helper
  engine/GFFPUSetDoublePrecision.h (FableWin.pdb ?GFFPUSetDoublePrecision@@YIXXZ; `fnstcw` / `fldcw` at 0x0041829C..0x004182AF, which
  VC7.1 never emits from C++) with a FABLE_PORTABLE twin; dual-path test rebuild/tests/portable/test_GFFPUSetDoublePrecision.cpp
  (written at landing) passes vc71-asm / vc71-dual / vc71-portable and the g++ portable steps (portable_twins_check.py).
- 0x0078C210 ??0CTCBerserk@@QAE@AAVCThing@@@Z, 602 bytes (catalog 688 split; BOUNDS_FIX).
- 0x00691D70 ?ProcessInput@CInputProcessCreatureMovement@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z (/GS), 528 bytes incl. two switch
  tables (catalog 624 split; BOUNDS_FIX).
Not landed: 0x00417747 UpdateForcedTickSpeed DIFFER(600v596) (retail's lone `fld [m]; fld1; fdivrp` reciprocal pair, 7 grades);
0x004CA019 phantom (inside landed 0x004CA010; BOUNDS_FIX skip); 0x0061A6A0 CTCInventoryQuests::OnCreate and 0x00B67270
CEngineLandscapeRenderer::BuildBlendingTables not graded (agent budget; decoded outlines in differ/).
Headers: base 061e531f7, clean 3-way merge, hdrcheck2 default=same; new CInterpolationInfo.h, CRoughFrameCounter.h,
GFFPUSetDoublePrecision.h; 22 opt-ins (hdr/README.md). Queued land1011be: 9 name fixes.
