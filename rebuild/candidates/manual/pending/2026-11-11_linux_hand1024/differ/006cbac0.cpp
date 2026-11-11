// CTCCreatureNavigation::GetNextNavPosOnRoute(const CNavigationInput& target, bool a, bool b, bool c, bool d, bool e) -- pick the creature's navigation speed for the target's movement type, ask the navigator (slot 3 / 4 / 5 by straight-line vs proximity case) for the next position on its route, remember whether a route is still being calculated, and hand back the navigator's output
// Grade: DIFFER(ours v retail) -- not compiled: a structural draft. Needs the CThingCreatureBase movement-speed virtuals (slots 79..86, +0x13C..+0x158) and the CNavigatorBase GetNextNavPos family (slots 3..5) in shared headers, CNavigationInput (+0x0C proximity, +0x10 movement type, +0x14 explicit speed) and CCreatureNavOutput (16 bytes) layouts; see NOTES.md.
// Leaf: ?GetNextNavPosOnRoute@CTCCreatureNavigation@@QAE?AVCCreatureNavOutput@1@ABVCNavigationInput@@_N1111@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CBAC0, 825 bytes of code (`ret 1Ch`, the result by hidden pointer) then its 11-entry movement-type
//   case table 0x006CBDFC; 880 with padding.
// Placeholder names (descriptive, unproven -- do not land): GetParentCreature, the six Get*Speed virtual names, MovementType / Speed
//   of CNavigationInput.
// Identity: Ego_r.pdb ?GetNextNavPosOnRoute@CTCCreatureNavigation@@QAE?AVCCreatureNavOutput@1@ABVCNavigationInput@@_N1111@Z (0x00701580),
//   `ret 1Ch` = hidden result + target + five bools.
// Callees: CNavigationInput::operator== 0x006CCF70 (the GotNavigationTarget test; owner unproven), NCreatureMode::IsAbleToMove 0x0085F3E0,
//   the creature's speed virtuals +0x13C (walk) / +0x144 (run) / +0x14C / +0x150 / +0x154 / +0x158, CTCBase::GetPRandomSeed 0x00686DA0,
//   GFFloatRandom 0x004978A0 / GFRandom LCG step (imul 24A1h, add 24DFh) and 0x00497890, CTCBase::GetCurrentTimeFrame-like 0x00686AE0 /
//   0x00686A80 (world / game-time getters; names unproven), CThing::GetPos 0x004C73D0, CThingCreatureBase::GetPhysicsPosition 0x006A4D00,
//   the navigator's slots +0x0C / +0x10 / +0x14, CTCBase::GetWorld 0x00686A70, CWorld frame counter 0x0049D870.
CTCCreatureNavigation::CCreatureNavOutput CTCCreatureNavigation::GetNextNavPosOnRoute(const CNavigationInput& target, bool a, bool b,
                                                                                       bool c, bool d, bool e)
{
    CThingCreatureBase& creature = GetParentCreature();
    bool had_target = GotNavigationTarget;
    float speed = 0.0f;
    if (had_target && !(NavigationTarget == target))
        GotNavigationTarget = false;
    if (!had_target || !GotNavigationTarget)
        CalculatingNewRoute = true;
    if (NCreatureMode::IsAbleToMove(creature)) {
        switch (target.MovementType) {                                        // case table 0x006CBDFC (11 entries)
        case 0: speed = target.Speed; break;                                  // +0x14
        case 1: speed = creature.GetWalkSpeed(target) * 0.5f; break;          // slot 79 (+0x13C)
        case 2: speed = creature.GetWalkSpeed(target) * 0.5f; break;
        case 3: speed = creature.GetWalkSpeed(target); break;
        case 5: speed = creature.GetRunSpeed(target); break;                  // slot 81 (+0x144)
        case 6: {                                                             // run with a random +-40% wobble
            float wobble = GFFloatRandom(creature.GetRunSpeed(target) * 0.4f, GetPRandomSeed());
            if (!(GFRandom(GetPRandomSeed()) & 1))
                wobble = -wobble;
            speed = creature.GetRunSpeed(target) + wobble;
            break;
        }
        case 7: speed = creature.GetSprintSpeed(target); break;               // slot 83 (+0x14C)
        case 8: speed = creature.GetSneakSpeed(target); break;                // slot 84 (+0x150)
        case 9: speed = creature.GetCombatWalkSpeed(target); break;           // slot 85 (+0x154)
        case 10: speed = creature.GetCombatRunSpeed(target); break;           // slot 86 (+0x158)
        default: speed = 0.0f; break;                                         // 4 and out of range
        }
    }
    Speed = speed;
    // ... the navigator call (slot 3, 4 or 5 on the proximity / CanMoveThroughSolidObjects tests), FrameLastNavigated, NextUpdate as
    // retail 0x006CBC42..0x006CBDF8; see NOTES.md.
}
