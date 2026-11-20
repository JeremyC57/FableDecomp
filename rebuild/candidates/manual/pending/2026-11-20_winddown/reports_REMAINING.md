# Shared-header re-port backlog -- what remains after reports b1..b5 (Account B, 2026-11-20, wind-down)

Done: b1 CGenericException (9, landed), b2+b3 IDirect3D9 (13, landed), b4 CEngineSubPrimitiveSprite (2, ready), b5 CPSCSingleSprite
part 1 (8, ready). Tools: $W/tools (mkrp.py, chk.sh [STRICT=0|1], port.py, swapblk.py, cpsrc.sh, tcc.sh, fin.sh). Baseline grades of
all 79 listed rows: $W/base/grades.txt.

## Held for Account A (NF136 "handed-back", not "done")
0x009BE870 GetBestCompressedAlphaTextureFormat (IDirect3D9); 0x009BFB30 ResetDevice (IDirect3D9 + CVsyncCallbackTimer).

## CPSCSingleSprite / CEnginePrimitiveSpriteTrail -- 13 addresses left (b5 header opt-ins carry over; work from batches/b5/hdr)
- 0x00B0A140 ??_G (snapshot in batches/b6/src, landed text only). It is a free fastcall stand-in. The real
  ??_GCPSCSingleSprite@NParticleEngine@@UAEPAXI@Z (calls ??1 0x00B0A160, then global operator delete) is emitted with the vtable. The
  ctor 0x00B09360 and dtor 0x00B0A160 are out of line, so this row's TU only emits it if it holds the destructor's definition. Two
  options for main: (a) grade it from the dtor TU text with `// Leaf: ??_G...` (GOTCHAS line 163 precedent; the source duplicates
  0x00B0A160); (b) leave it until 0x00B0A160 is ported and land both from one text.
- Not started:
  - CPSCSingleSprite: 00af8170 RegisterPSCs (also local CPSCDecalRenderer / CPSCEmitterGeneric / CPSCLight / CPSCRenderSprite /
    CPSCUpdateNormal), 00b09360 ctor, 00b0a160 dtor, 00b094b0 Initialise, 00b062a0 InterpolateTo, 00b068f0 ReadBinary, 00b06060
    WriteBinary (no Leaf line), 00b087c0 PreparePrimitives, 00b081a0 AddTrailPosition, 00b08250 SetTrailPositionsFromSpline.
  - CEnginePrimitiveSpriteTrail: 00b09f90 ctor, 00b09e40 _List_base<CSpriteTrailEntry> dtor, 00bcbc00
    CEngineInternalPrimitiveSpriteTrail::Update.
  - Rows flagged for both classes must be ported for both in one go.
- Expect more stand-ins like those in b5 (see each row's model). The FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS /
  FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS declarations should cover the member rows. 00b05c60 GetComponentCost already includes
  CPSCSplineBase.h and still has a local CPSCSingleSprite.

## CVsyncCallbackTimer -- 27 rows + hidden 0x009C0E50 (+ held 0x009BFB30); header-contract decision needed from main
Facts:
- Ego_r layout: Device +0, FreeQueoryObjects +4, PendingQueories, PendingFrame, FinishedFrame; 36 bytes with 12-byte VC lists.
  Retail has 4-byte STLport lists, so the offsets are +4 / +8 / +0xC / +0x10 and sizeof is 0x14 (every row's model agrees; the
  ctor's caller does new(0x14)).
- CPendingQuery {IDirect3DQuery9* Query; long Frame; double TimeStamp} (0x10).
- engine/CVsyncCallbackTimer.h models only Device + the inline SetDevice and says "must not be constructed or embedded".
- The 0x009F7xxx timer TU is unoptimised (`#pragma optimize("", off)`). Its rows carry their own `namespace std` STLport models,
  which are not stlport/fable_containers.h: e.g. _List_iterator_base out of line at 0x0041C780.., distance returning int, and
  __malloc_alloc::allocate with a `__result` temporary.
Rows by need:
- (1) Nested CPendingQuery type only. These are the STL instantiations: 009f7a30 009f7bb0 009f7830 009f7810 009f77f0 009f7780
  009f78f0 009f7890 009f7b60 009f7d70 009f76f0 009f7ad0 009f79d0 009f7c50 009f7ec0 009f7e40 009f7dc0 009f7760 009f77b0 009f7d10.
- (2) Members including the lists: 009f70e0 ctor, 009f7330 dtor, 009f7250 ClearAllQueries, 009f73a0 OnPreDeviceReset.
- (3) Method declarations / Device only: 009f7050 GetInfo (plus a CVsyncCallbackTimerInfo struct: no header yet; Ego_r 48 bytes),
  the CDisplayManager rows 004166e2 009bf2d0 009bf500 009beeb0 009c01c0, and 009c0e50 (named CDynamicTextureList there; NAME_FIXES
  in b3, landed). That covers ctor/dtor/GetInfo/OnPreDeviceReset/ClearAllQueries/SetDevice.
Options:
- A. Opt-in FABLE_RETAIL_CVSYNCCALLBACKTIMER_PENDINGQUERY: the nested struct only (add-only, no layout claim), plus a declarations
  opt-in for the methods. This covers groups (1) and (3) without touching their local std models, so 25+ rows port as pure model
  swaps. Group (2) still needs B or C.
- B. Opt-in FABLE_RETAIL_CVSYNCCALLBACKTIMER_LAYOUT that includes stlport/fable_containers.h and declares
  `std::list<CPendingQuery> FreeQueoryObjects, PendingQueories; long PendingFrame, FinishedFrame;` with a 0x14 size assert. Group (2)
  rows then drop their local std models for the shared STLport. This is the cleanest end state, but the /Od TU's inline bodies
  (allocate temporaries, out-of-line iterator base) must match; that needs a calibration probe on 009f70e0 first, and new STLport
  opt-ins are likely.
- C. The LAYOUT opt-in names `std::list<CPendingQuery>` without including any STL header, so the includer must have defined
  std::list first (the row's own model). Least work, but the header then depends on its includer: a fragile contract I would not
  recommend.
My recommendation is A now (mechanical, ~25 rows), then B for the four member rows after a calibration probe.

## Coming with hand3c3 (not started)
GFFloatAbs.h-flagged rows; the 7 CThingFilter_CanBeHitByEpicSpell rows (hand3c3 b12).
