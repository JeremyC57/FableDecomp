// Behaviour test for CAIStateGroup_RecoverStealableItem::IsRunnable (0x00965320): same logic over mocks.
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CThing { bool dead; C3DVector pos; float range; CThing() : dead(false), range(0) { pos.x = pos.y = pos.z = 0; } };
static bool IS_THING_ALIVE(CThing* p) { return p != 0 && !p->dead; }
struct CState {
    bool canRecover; CThing* targetCandidate; CThing* item; float lastRange;
    CThing* PStealableItem; CThing* PTargetPoint;
    bool GotActionPoint; long ActionPointIndex; bool ReachedDestination; long PickUpActionID; bool NeedsCleanup;
    CThing* FindPTargetPoint() { return targetCandidate; }
    CThing* FindPStealableItemOutsideTargetPointRange(const C3DVector&, float range) { lastRange = range; return item; }
    bool IsRunnable()
    {
        if (!NeedsCleanup) {
            if (canRecover) {
                ActionPointIndex = -1; GotActionPoint = false; ReachedDestination = false; PickUpActionID = -1;
                PStealableItem = 0;
                PTargetPoint = FindPTargetPoint();
                if (IS_THING_ALIVE(PTargetPoint)) {
                    CThing* point = PTargetPoint;
                    PStealableItem = FindPStealableItemOutsideTargetPointRange(point->pos, point->range);
                }
                return IS_THING_ALIVE(PStealableItem) && IS_THING_ALIVE(PTargetPoint);
            }
        }
        return false;
    }
};
int main()
{
    CThing target, item; target.range = 7.5f;
    CState s; s.canRecover = true; s.targetCandidate = &target; s.item = &item; s.lastRange = 0;
    s.PStealableItem = 0; s.PTargetPoint = 0; s.GotActionPoint = true; s.ActionPointIndex = 3; s.ReachedDestination = true;
    s.PickUpActionID = 5; s.NeedsCleanup = false;
    bool ok = s.IsRunnable() && s.lastRange == 7.5f && s.ActionPointIndex == -1 && s.PickUpActionID == -1 && !s.GotActionPoint && !s.ReachedDestination;
    item.dead = true; ok = ok && !s.IsRunnable();
    item.dead = false; target.dead = true; ok = ok && !s.IsRunnable() && s.PStealableItem == 0;
    target.dead = false; s.NeedsCleanup = true; s.ActionPointIndex = 4; ok = ok && !s.IsRunnable() && s.ActionPointIndex == 4;
    s.NeedsCleanup = false; s.canRecover = false; ok = ok && !s.IsRunnable() && s.ActionPointIndex == 4;
    if (!ok) { std::printf("00965320_TEST FAIL\n"); return 1; }
    std::printf("00965320_TEST PASS\n");
    return 0;
}