// CTCCreatureNavigation::GetNearestFleePositionBehindCover(const C3DVector& flee_from, C3DVector& flee_pos) const -- find the nearest thing that can be used as cover on the far side from the threat, then put the flee position just behind it (along the threat-to-cover direction, or in a random direction when they coincide), clamped to the map region and dropped to the ground height
// Grade: DIFFER(ours v retail) -- not compiled: a structural draft. Needs the CWorldMap region lookup (slot 15, +0x3C, by C2DVectorI) and its region bounds (+0x1D0C / +0x1D10 / +0x1D24 / +0x1D28), the cover filter / nearest-compare types of CThingSearchTools::GetPBestThing 0x006CD3B0 (functor 0x006CCF50) and the inline fast inverse square root (table 0x013CE558) in shared headers; see NOTES.md.
// Leaf: ?GetNearestFleePositionBehindCover@CTCCreatureNavigation@@QBE_NABVC3DVector@@AAV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CBE80, 739 bytes (752 with padding); thiscall, `ret 8`, returns bool.
// Placeholder names (descriptive, unproven -- do not land): GetParentCreature, CWorldMapRegion / PeekRegion / MinX..SizeY, CoverFilter,
//   CompareNearest, FastInvSqrt, Clamp, GetPhysicsRadius (CTCPhysics +0x34 through +0x60), GetGroundHeight.
// Identity: Ego_r.pdb ?GetNearestFleePositionBehindCover@CTCCreatureNavigation@@QBE_NABVC3DVector@@AAV2@@Z (0x00701A60).
// Callees: CTCBase::PeekWorldMap 0x00686D20, CThingCreatureBase::GetPhysicsPosition 0x006A4D00, __ftol 0x00BFEA70 (Y then X),
//   CTCBase::PeekThingSearchTools 0x00686D90, CThingSearchTools::GetPBestThing 0x006CD3B0 (filter radius 4.0f), CTCBase::GetPRandomSeed
//   0x00686DA0, GFFloatRandom 0x004978A0, the ground-height getter 0x006A4DD0 (catalog label NUISystem::CSpellContainerList::AddChild is
//   wrong; it returns a float for a C3DVector on the creature -- name unproven).
bool CTCCreatureNavigation::GetNearestFleePositionBehindCover(const C3DVector& flee_from, C3DVector& flee_pos) const
{
    const CThingCreatureBase& creature = GetParentCreature();
    const CWorldMapRegion* region = PeekWorldMap().PeekRegion(C2DVectorI((long)creature.GetPhysicsPosition().X,
                                                                          (long)creature.GetPhysicsPosition().Y));
    const CThing* cover = 0;
    if (!PeekThingSearchTools().GetPBestThing(CoverFilter(creature.GetPhysicsPosition(), flee_from, 4.0f), CompareNearest(), &cover))
        return false;
    C2DVector away(cover->GetPhysicsPosition().X - flee_from.X, cover->GetPhysicsPosition().Y - flee_from.Y);
    C3DVector direction;
    if (away.GetLength() < 0.0001f) {                         // 0x0129BA3C
        float angle = GFFloatRandom(1.0f, GetPRandomSeed()) * 6.2831855;      // heading in [0, 2pi), elevation 0.0
        direction = C3DVector(sin(angle) * cos(0.0), cos(angle) * cos(0.0), sin(0.0));
    } else {
        float inv = FastInvSqrt(away.GetSquaredLength());                     // table 0x013CE558, one Newton step (3 - x*y*y) * y * 0.5
        direction = C3DVector(away.X * inv, away.Y * inv, 0.0f * inv);
    }
    direction *= cover->GetPhysicsRadius() + creature.GetPhysicsRadius() + 0.5f;
    C3DVector pos = cover->GetPhysicsPosition() + direction;
    pos.X = Clamp(pos.X, region->MinX, region->MinX + region->SizeX);
    pos.Y = Clamp(pos.Y, region->MinY, region->MinY + region->SizeY);
    pos.Z = creature.GetGroundHeight(pos);
    flee_pos = pos;
    return true;
}
