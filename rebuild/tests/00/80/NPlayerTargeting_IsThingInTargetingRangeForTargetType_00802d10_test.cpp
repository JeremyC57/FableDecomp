// Behavioral fixture for NPlayerTargeting::IsThingInTargetingRangeForTargetType.
// The collaborators supply component availability, slot contents, and targeting range.
// The vector operations and squared range come from the same headers as the retail row.
#include <cstdio>
#include <cstdlib>
#include "engine/C3DVector.h"
#include "engine/GFSqr.h"

struct RangeActor {
    C3DVector position;
    bool hasCarryingComponent;
    const RangeActor* primarySlot;
    float baseRange;
};

struct RangeQuery {
    const RangeActor* expectedTarget;
    const RangeActor* expectedSlot;
    unsigned long expectedType;
    unsigned int slotReads;
    unsigned int rangeReads;

    const RangeActor* readSlot(const RangeActor& actor) {
        ++slotReads;
        return actor.primarySlot;
    }

    float readRange(unsigned long type, float baseRange, const RangeActor& target,
                    const RangeActor* carried) {
        ++rangeReads;
        if (type != expectedType || &target != expectedTarget || carried != expectedSlot)
            std::abort();
        const float range = baseRange + static_cast<float>(type);
        return carried ? range * 2.0f : range;
    }
};

static bool inTargetingRange(const RangeActor& actor, const RangeActor& target,
                             unsigned long type, RangeQuery& query) {
    const RangeActor* carried = 0;
    if (actor.hasCarryingComponent)
        carried = query.readSlot(actor);
    return (target.position - actor.position).GetSquaredMagnitude2D() <
        GFSqr(query.readRange(type, actor.baseRange, target, carried));
}

int main() {
    RangeActor actor, target, weapon;
    actor.position = C3DVector(13.0f, -7.0f, 300.0f);
    actor.baseRange = 3.0f;
    target.position.Z = -300.0f;
    for (int mode = 0; mode < 3; ++mode) {
        actor.hasCarryingComponent = mode != 0;
        actor.primarySlot = mode == 2 ? &weapon : 0;
        for (unsigned long type = 0; type < 3; ++type) {
            const float range = (3.0f + static_cast<float>(type)) * (mode == 2 ? 2.0f : 1.0f);
            for (int x = -6; x <= 6; ++x) {
                for (int y = -6; y <= 6; ++y) {
                    target.position.X = actor.position.X + x;
                    target.position.Y = actor.position.Y + y;
                    RangeQuery query = {&target, actor.primarySlot, type, 0, 0};
                    if (inTargetingRange(actor, target, type, query) != (x*x + y*y < range*range) ||
                        query.rangeReads != 1 || query.slotReads != (mode != 0))
                        return 1;
                }
            }
        }
    }
    std::puts("00802d10_TEST PASS");
    return 0;
}
