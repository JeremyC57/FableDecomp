# Chunk Q20 (20 parked near misses whose drafts used local models of header classes, Opus 5.5)

2 matched and landed in landing dk:
- 0x007B0360 CTCFishingSpot::FrameUpdate (no CTCFishingSpot header exists: a local class on the shared CTCBase / CThing /
  CIntelligentPointer / CTCDParticleEmitter headers; the per-member block is a FABLE_FORCEINLINE helper with the descriptive
  made-up name CreateParticle, noted in NOTES)
- 0x008B7040 CCreatureAction_JumpOutFromCover::Initialise (new CCreatureAction_JumpOutFromCover.h; C3DVector NORMALISE_LOCALS_XYZ)

4 ported but still DIFFER (differ/, residue on line 2): 0x009DDC60 Draw2DBoxWithZ (two values swap stack slots), 0x007BCF7F
MultiArrow InitMovement and 0x007BD845 DrainLife InitMovement (x87 operand order plus slot packing), 0x009FB750
LoadFromDataStreamToPreallocatedSurface (one hoisted load, eax/edx swap in the row copy).
14 were not reached (line 2 `DIFFER(not regraded in Q20)`); chunk Q21 takes them.
