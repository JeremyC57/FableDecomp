// Behaviour test for 0x0085EE50 NCreatureMode::GetStunnedEffectCreationPos (chunk 402): the stunned effect sits 0.75
// above the creature's head position (GetHeadPos(true)); X and Y pass through.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CThingCreatureBase {
public:
    C3DVector Head;
    bool      LastIncludeOffset;
    CThingCreatureBase() : Head(0.0f, 0.0f, 0.0f), LastIncludeOffset(false) {}
    C3DVector GetHeadPos(bool includeOffset) const
    {
        const_cast<CThingCreatureBase*>(this)->LastIncludeOffset = includeOffset;
        return Head;
    }
};
namespace NCreatureMode {
C3DVector __fastcall GetStunnedEffectCreationPos(const CThingCreatureBase& creature);
}

C3DVector __fastcall NCreatureMode::GetStunnedEffectCreationPos(const CThingCreatureBase& creature)
{
    C3DVector head = creature.GetHeadPos(true);
    return C3DVector(head.X, head.Y, head.Z + 0.75f);
}

int main()
{
    int failures = 0;
    CThingCreatureBase creature;
    creature.Head = C3DVector(10.0f, -4.5f, 2.25f);
    C3DVector pos = NCreatureMode::GetStunnedEffectCreationPos(creature);
    if (pos.X != 10.0f || pos.Y != -4.5f || pos.Z != 3.0f) { printf("  wrong position\n"); ++failures; }
    if (!creature.LastIncludeOffset) { printf("  head position asked without the offset\n"); ++failures; }
    creature.Head = C3DVector(0.0f, 0.0f, -0.75f);
    pos = NCreatureMode::GetStunnedEffectCreationPos(creature);
    if (pos.X != 0.0f || pos.Y != 0.0f || pos.Z != 0.0f) { printf("  wrong position at the origin\n"); ++failures; }
    if (failures == 0) {
        printf("0085ee50_TEST PASS\n");
        return 0;
    }
    printf("0085ee50_TEST FAIL\n");
    return 1;
}