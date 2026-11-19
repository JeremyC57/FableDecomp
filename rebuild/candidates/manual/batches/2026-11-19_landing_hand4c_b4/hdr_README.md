# hand4c header overlay (2026-11-19)
Landed already (b2/b3, removed from the overlay since the repo copy is identical): engine/CThingFilter_OverlappingCircle.h
FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY; engine/CThingSearchTools.h FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_EGOD;
engine/CThingFilter_IsInQuestOrNullQuest.h (new header).

Open (batch 4, row 0x0066A1E0):
- engine/CThingSearchTools.h: FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD (new; appended at the end of the file after the
  TCSINROUGHAREA_EGOD block, inside its own `#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS) && ...`, so no assert line moves; active only with
  FOREACHINROUGHAREA + FOREACHINROUGHAREA_CONSTPROC + ROUGHAREA_EGOD and without FOREACHINROUGHAREA_BODY) -- the out-of-class
  ForEachInRoughArea<T,Filter,Proc> body, Ego_d locals in FableWin 0x01DAAC80's order, empty test on the edges. Users: 0x0066A1E0.
- engine/CThingFilter_SolidToThing.h: FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_BODY (new; appended after the class) -- the inline operator()
  body (FableWin 0x01DAEBB0); the same text grades MATCH at retail's out-of-line copy 0x00661640. Users: 0x0066A1E0.
- engine/CThingFilter_PhysicsFlagsSolidToThing.h: NEW header (no repo header) -- Ego_d class record 0x3925A (unary_function base,
  ctor (const CThing&), const CThing& SourceThing +0, const CTCPhysicsBase* SourceThingPhysicsTC +4), Ego_r layout; nested opt-in
  FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_BODY with the FableWin 0x01DAEB10 body. Users: 0x0066A1E0.
