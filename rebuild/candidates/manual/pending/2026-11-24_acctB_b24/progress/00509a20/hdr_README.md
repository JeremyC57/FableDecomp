# hdr/ overlay for 0x00509A20 (B24S1, 2026-11-24) -- each file = CURRENT repo header + one additive opt-in
- engine/CThingFilter_OverlappingArea.h: NEW header (no repo header exists; local models in landed 0x006437D0 / 0x00511710 / 0x00511A10).
  Ego_r 4 bytes, Area (C2DBoxI*) +0; FableWin ??0CThingFilter_OverlappingArea@@QAE@ABVC2DBoxI@@@Z (reference ctor), operator() 0x0050EF30.
- engine/C2DBoxI.h: FABLE_RETAIL_C2DBOXI_CONTAINSPOINT_BODY -- inline ContainsPoint body from FableWin 0x01C82100 (inline in retail).
- engine/CPhysicsMeshObject.h: FABLE_RETAIL_CPHYSICSMESHOBJECT_HELPERPOINTS -- GetAllHelperPointNames 0x00A11820 / GetHelperPointPositionStatic
  0x00A11760 declarations (Ego_r + FableWin mangled names).
- engine/CNavQuadTree.h: FABLE_RETAIL_CNAVIGATIONTRANSITIONPOINT_CTOR -- FableWin ??0CNavigationTransitionPoint@@QAE@ABVC2DVector@@JJ@Z inline.
- engine/CThingPhysical.h: FABLE_RETAIL_CTHINGPHYSICAL_NAVCOLLISIONLINES -- GetNoNavCollisionLineLists 0x006A4F10 / GetNavCollisionLineList
  0x006A4F60 declarations (Ego_r + FableWin).
- engine/CThingFilter_IsInQuestOrNullQuest.h: FABLE_RETAIL_CTHINGFILTER_ISINQUESTORNULLQUEST_INLINE_CTOR -- inline ctor body (retail copies the
  name with 0x0099EC30 and calls no ctor).
- engine/CNavigationAreaUpdate.h: FABLE_RETAIL_CSWITCHABLEBLOCKAGE_COPY_OUTOFLINE -- out-of-line copy ctor 0x005199E0 declaration (tried in b9/b10;
  NOT used by the best source: retail expands the copy at the in-place push_back and calls it only in the overflow path).
