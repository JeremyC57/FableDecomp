// CTCDExplosion::UpdateHitThings(const C3DVector& start_pos, const C3DVector& end_pos) -- damage every thing within the damage radius of the explosion line that is not immune or already hit (only the specified things when a list is set), then push every physics thing within the physics radius away (impulse frames from its distance along the line) and kill or damage the breakable objects there
// Grade: DIFFER(ours v retail) -- not compiled: a structural draft. The shared engine/CTCDExplosion.h models HitThings / ImmuneThings / SpecifiedThingsToHit as padding, there is no header for the two CThingSearchTools filter types (GetAllThingsInRoughArea instantiations 0x0080E670 / 0x0080E8E0) nor for the vector<CIntelligentPointer<CThing> > -> vector<CThing*> copy 0x0080EC50; see NOTES.md.
// Leaf: ?UpdateHitThings@CTCDExplosion@@AAEXABVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080CD40, 876 bytes (880 with padding); thiscall, `ret 8`, no EH frame.
// Identity: Ego_r.pdb ?UpdateHitThings@CTCDExplosion@@AAEXABVC3DVector@@0@Z (0x00808AF0, between SetSpecificThingToHit 0x00808AA0 and
//   CreateStaticExplosion 0x00808F30; retail SetSpecificThingToHit 0x0080CCF0 and CreateStaticExplosion 0x0080D0B0 are landed neighbours).
// Callees: NPhysicsHelpers::GetLineThingSearchArea 0x008F58C0 (__fastcall, C2DBoxI by hidden pointer), the vector copy 0x0080EC50 (three
//   times: HitThings +0x54 and ImmuneThings +0x60 into the ignore list, SpecifiedThingsToHit +0x6C into its own), malloc 0x00BFEA0E (the
//   list heads), CTCBase::GetThingSearchTools 0x00686D80, CThingSearchTools::GetAllThingsInRoughArea 0x0080E670 (damage-radius filter) and
//   0x0080E8E0 (physics-radius filter), std::find 0x0080D970 / 0x0080D9E0 (the specified-things test), CTCDExplosion::GiveDamageToThing
//   0x0080CA20, CThing::GetPos 0x004C73D0, NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine 0x008F5BA0, __ftol 0x00BFEA70, the
//   interface-0xDE component's impulse call 0x00750BA0, CThing::Kill(bool) 0x004C9B80, free 0x00BFEA14.
// Placeholder names (descriptive, unproven -- do not land): CopyThings, CThingFilter_InRangeOfLine, CThingFilter_NotInList,
//   CTCPhysicsImpulse::ApplyForFrames, TCI_OBJECT_BREAKABLE, IsDamageable.
// Layout (Ego_r.pdb, retail STLport sizes): DamageRadius +0x0C, PhysicsRadius +0x10, StartPos +0x20, EndPos +0x2C, HitThings +0x54,
//   ImmuneThings +0x60, SpecifiedThingsToHit +0x6C; CMainGameComponent::ConstantFPS 0x01375550.
void CTCDExplosion::UpdateHitThings(const C3DVector& start_pos, const C3DVector& end_pos)
{
    C2DBoxI search_area = NPhysicsHelpers::GetLineThingSearchArea(start_pos, end_pos, DamageRadius);
    std::vector<CThing*> things_to_ignore;
    CopyThings(HitThings, things_to_ignore);              // 0x0080EC50 (owner unproven)
    CopyThings(ImmuneThings, things_to_ignore);
    std::vector<CThing*> specified_things;
    CopyThings(SpecifiedThingsToHit, specified_things);
    {
        std::list<CThing*> things;
        GetThingSearchTools().GetAllThingsInRoughArea<CThing>(search_area,
            CFilter_Bind2<CThingFilter_InRangeOfLine, CThingFilter_NotInList>(
                CThingFilter_InRangeOfLine(start_pos, end_pos, DamageRadius), CThingFilter_NotInList(things_to_ignore)), things);
        for (std::list<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            if (specified_things.empty() || std::find(specified_things.begin(), specified_things.end(), thing) != specified_things.end())
                GiveDamageToThing(*static_cast<CThingGameObject*>(thing));
        }
    }
    std::list<CThing*> things;
    GetThingSearchTools().GetAllThingsInRoughArea<CThing>(search_area,
        CFilter_Bind2<CThingFilter_InRangeOfLine, CThingFilter_NotInList>(
            CThingFilter_InRangeOfLine(start_pos, end_pos, PhysicsRadius), CThingFilter_NotInList(things_to_ignore)), things);
    for (std::list<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        CThing* thing = *it;
        if (!specified_things.empty() && std::find(specified_things.begin(), specified_things.end(), thing) == specified_things.end())
            continue;
        CTCPhysicsImpulse* impulse;                       // interface 0xDE (class unproven)
        if (thing->GetTC(&impulse)) {
            float distance = NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine(StartPos, EndPos, thing->GetPos());
            impulse->ApplyForFrames((long)((distance / DamageRadius * 0.75f + 0.25f) * CMainGameComponent::GetConstantFPS()));
        } else if (thing->HasTC(TCI_OBJECT_BREAKABLE)) {   // interface 0x4E
            if (thing->IsDamageable())                     // +0x6C bit 2
                GiveDamageToThing(*static_cast<CThingGameObject*>(thing));
            else
                thing->Kill(true);
        }
    }
}
