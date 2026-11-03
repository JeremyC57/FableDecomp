void CTCVillage::CalculateSecuritySweepTasks()
{
    long numberOfGuards;
    CThingAICreature* firstGuard;
    {
        const HasGuardProperty& isGuard = HasGuardProperty();
        numberOfGuards = std::count_if(VillagePeople.begin(), VillagePeople.end(), isGuard);
        std::list<CIntelligentPointer<CThingAICreature> >::iterator guard =
            std::find_if(VillagePeople.begin(), VillagePeople.end(), isGuard);
        if (guard == VillagePeople.end())
            return;
        firstGuard = guard->Get();
    }
    CTCCreatureNavigation* navigation = 0;
    firstGuard->GetTC(&navigation);
    CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!player)
        return;
    float radius;
    const C3DVector* exitPosition;
    {
    const C3DVector& playerPosition = player->GetPos();
    CThing* exit = 0;
    PositionPlayerWasInWhenSecuritySweepCalculated = playerPosition;
    CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    GetWorld().GetThingSearchTools().GetPBestThing(
        std::compose2(std::logical_or<bool>(), CThingFilter_HasTC(TCI_REGION_EXIT), CThingFilter_HasTC(TCI_TELEPORTER)),
        CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &exit);
    if (!exit)
        return;
    radius = InitialSecuritySweepRadius -
        (float)((PeekWorld().GetFrame() - FrameStartedSecuritySweep) / CMainGameComponent::GetConstantFPS()) * 0.4f;
    {
    CThingAICreature* nearestGuard;
    if (GetWorld().GetThingSearchTools().GetPBestThing(CThingFilter_HasTC(TCI_GUARD),
            CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &nearestGuard)) {
        float distance = (PositionPlayerWasInWhenSecuritySweepCalculated - nearestGuard->GetPos()).GetMagnitude2D();
        if (radius > distance) {
            InitialSecuritySweepRadius -= radius - distance;
            radius = distance;
        }
    }
    }
    const float minimumRadius = 2.0f;
    if (SecuritySweepUrgent)
        radius = minimumRadius;
    if (radius < minimumRadius)
        radius = minimumRadius;
    exitPosition = &exit->GetPos();
    }
    C3DVector offset = *exitPosition - PositionPlayerWasInWhenSecuritySweepCalculated;
    float distanceSquared = offset.GetSquaredMagnitude2D();
    if (distanceSquared < radius * radius)
        radius = (float)sqrt(distanceSquared) * 0.8f;
    offset.Normalise();
    offset *= radius;
    float step = 1.0f / numberOfGuards;
    for (long i = 0; i < numberOfGuards; ++i) {
        float angle = 0.0f;
        C3DVector rotated = offset;
        C3DVector position;
        while (!navigation->GetNearestNavigablePosTo(rotated + PositionPlayerWasInWhenSecuritySweepCalculated, 5.0f, position)) {
            rotated = offset;
            if (angle >= 0.0f) {
                angle += step * 0.2f;
                if (angle >= step)
                    goto nextGuard;
            }
            else
                angle *= -1.0f;
            rotated.RotateXY(angle);
        }
        SecuritySweepTasks.push_back(CCountedPointer<CVillageTaskSecuritySweep>(
            new CVillageTaskSecuritySweep(position, 5.0f, PositionPlayerWasInWhenSecuritySweepCalculated, 0.25f)));
    nextGuard:
        offset.RotateXY(step);
    }
}
