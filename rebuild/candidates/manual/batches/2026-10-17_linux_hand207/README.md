# Hand chunk 207 (CPersistContext .. CPlayer): 2 of 40 byte-exact

## What matched (2)

- 00633420 CPlayer::UninitModeControlCreature: all eighteen input process pointers use the direct RemoveFromInterface helper in the retail order; the engine CPlayer header gives the exact offsets.
- 006338d0 CPlayer::InitModeBerserk: fourteen input process pointers call virtual AddToInterface with PlayerInterface; declaring the two-slot virtual interface reproduces the VC7.1 call shape.

## Parked for Fable 5.1 (38)

- 00a11ae0 CPhysicsMeshObject::GetBoundingBoxCentre DIFFER(166v185): FPU scheduling and hidden return-object stack layout remain | tried: interleaved bounds model, conventional affine transform | next: use explicit local/return model
- 00a422c0 CPlane::GetIntersectionWithLine DIFFER(114v165): FPU comparison and spill scheduling remain | tried: direct parametric line expression, inverted zero comparison | next: recover original line helper expression
- 00569f20 CPersistContext::Transfer<signed_char> DIFFER(148v180): mode switch needs retail jump-table shape and argument convention | tried: five-case switch with two argument forms | next: model five explicit cases and fixed text token

The remaining rows are parked as lane gaps with their retail lengths preserved in STATUS.tsv and their handoff placeholders in differ/.

## Skipped

None.

## Label corrections

None.
