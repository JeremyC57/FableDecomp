# 2026-11-10 landing ar -- chunk 1014 (4 rows)

Source: `pending/2026-11-10_linux_hand1014/`. vland --land RELOCATION_MATCH, tests PASS:
- 0x00703210 ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (physics TC re-read per branch; the
  out-of-line CTCParamsPhysicsBase ctor visible from the new engine/CTCParamsPhysicsBase.h; inline default CRightHandedSet).
- 0x007EA2F0 ?GetTargetingPositionOnThing@NTargeting@@YI?AVC3DVector@@ABVCWorld@@ABVCThing@@M@Z (header bodies for the bullet-time
  getters and CTCPhysicsBase::GetPosition(float)).
- 0x009FA620 ?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z: the agent's draft made GFGetTopBit /
  GFGetNextPowerOfTwo FABLE_FORCEINLINE; the coordinator moved it down the owner ladder: plain `inline` in the new
  engine/GFGetNextPowerOfTwo.h plus the row flag /d2inlT180 (T160 leaves them called) -- re-graded RELOCATION_MATCH.
- 0x00B84720 ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@PAVCEngineSubPrimitiveBase@@MPAVCEnginePrimitiveManager@@@Z
  (scaled distance in a named float local; the fistp through the shared engine/GFFloatToLongNear.h).
Header opt-ins (hdrcheck2 default=same): CThingObject, CRightHandedSet, CEngineSubPrimitiveRenderer, CEngineSubPrimitiveBase,
CTCPhysicsBase, CWorld, CBulletTimeManager, stlport/fable_map.h (map::empty), CTCHeroExperience, CPixelFormat,
CAIStateGroup_ApprenticeHero, CAnimationSet; new CTCParamsPhysicsBase.h, GFGetNextPowerOfTwo.h, GFRandom.h.
Parked (differ/): 0x006D6FA0, 0x009E3F60 (real extent 189 bytes, not 576: BOUNDS_FIX.tsv), 0x00929FC0, 0x005DCC00, 0x00725F10.
Not attempted: 0x006A16A0, 0x006CBAC0 (moved to chunk 1016).
