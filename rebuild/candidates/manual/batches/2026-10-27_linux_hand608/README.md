# Chunk 608 (sixth parked-row chunk from the sweep ranking, 10 same-length rows 219-238 bytes): 1 of 10 landed

Landed (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0): 0x0080FFB0
NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection (a named pointer local for the object of the virtual SetRHSet call
whose argument is a by-value CRightHandedSet temporary loads ecx before the 24-byte copy, so the copy runs in eax/edx like retail;
slot 75 = SetRHSet proven). Parked 9 (pending/.../differ/), three much closer: 00844600 CCreatureAction_Read::PerformAction 63 -> 17
lines (PDB inline accessors), 00864ff0 CTCCreatureHitNotification::DrawHitNotification 48 -> 9 (real STLport templates), 00808aa0
std::__linear_insert 44 -> 38 (PDB class keys); 007f8c20 AddQueuedAnimation has correct names but 233 v 221 because the header's
unsigned refcount makes the def release register-based; the rest are register swaps, a frame-slot share (00813010 0x64 v 0x5c) and
rep movsd scheduling (00881690). 9 of 10 parked files had wrong names (class keys, nesting, const/access/reference, inherited members
on the derived class) -- all re-proven. 19 NAME_FIXES + 5 review rows queued (prefix c608: CAIStateGroupBase::Terminate, CTCBase
GetDefinitionManager / PeekWorld, CWorld::GetFrame static fastcall, CAnimationSet / CAnimInit, CCompressorZlib, CAIStateGroup_UsesSeat
protected SetSeat / MoveToSeatAndSitDown, six catalog labels). Header debt: CIVCountedPointeeBase.h unsigned refcount; four headers
store the vptr as a data field; CGlobalDef::GraphicWeaponTrail retail +0x5C.
