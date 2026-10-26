// Behaviour test for 007483e0: body copied from src/007483e0.cpp (CCompare_FurthestFromInitPos2D::operator()), GetPosition stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public:
    float X, Y, Z;
};
class C2DVector { public:
    float X, Y, Z;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { float r = Y * Y; r += X * X; return r; }
};
static int g_posCalls;
class CThing { public:
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;
    C3DVector     m_position;
    const C3DVector& GetPosition() const { ++g_posCalls; return m_position; }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad92[0xe8 - 0x91 - 1 - sizeof(C3DVector)];
    float InitialPosX;
    float InitialPosY;
    C2DVector GetOffsetFromInitialPos2D() const
    {
        const C3DVector& position = GetPosition();
        return C2DVector(InitialPosX - position.X, InitialPosY - position.Y);
    }
};
inline CThingCreatureBase* AsCreature(CThing* thing)
{
    CThingCreatureBase* creature = 0;
    if (thing && (thing->m_flags6c & 8))
        creature = (CThingCreatureBase*)thing;
    return creature;
}
inline bool IsAlive(const CThing* thing)
{
    return thing && !(thing->m_flags91 & 1);
}
struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const;
};
bool CCompare_FurthestFromInitPos2D::operator()(CThing* a, CThing* b) const
{
    if (a == b)
        return false;
    CThingCreatureBase* creatureA = AsCreature(a);
    CThingCreatureBase* creatureB = AsCreature(b);
    if (IsAlive(creatureA) || IsAlive(creatureB)) {
        if (IsAlive(creatureA)) {
            if (!IsAlive(creatureB))
                return true;
            C2DVector offsetA = creatureA->GetOffsetFromInitialPos2D();
            C2DVector offsetB = creatureB->GetOffsetFromInitialPos2D();
            if (offsetA.GetLengthSquared() > offsetB.GetLengthSquared())
                return true;
        }
    }
    return false;
}
static void setup(CThingCreatureBase& c, float ix, float iy, float px, float py, bool creature, bool dead)
{
    memset(&c, 0, sizeof(c));
    c.m_flags6c = creature ? 8 : 0; c.m_flags91 = dead ? 1 : 0;
    c.InitialPosX = ix; c.InitialPosY = iy; c.m_position.X = px; c.m_position.Y = py; c.m_position.Z = 50.0f;
}
int main()
{
    CCompare_FurthestFromInitPos2D cmp;
    CThingCreatureBase a, b;
    // 1. same thing: false
    setup(a, 0, 0, 0, 0, true, false); CHECK(cmp(&a, &a) == false);
    // 2. neither a creature: false
    setup(a, 0, 0, 0, 0, false, false); setup(b, 0, 0, 0, 0, false, false); CHECK(cmp(&a, &b) == false);
    // 3. null a, live creature b: false (a not alive)
    setup(b, 0, 0, 0, 0, true, false); CHECK(cmp(0, &b) == false);
    // 4. live a, null b: true
    setup(a, 0, 0, 0, 0, true, false); CHECK(cmp(&a, 0) == true);
    // 5. live a, dead creature b: true
    setup(b, 0, 0, 0, 0, true, true); CHECK(cmp(&a, &b) == true);
    // 6. dead a, live b: false
    setup(a, 0, 0, 0, 0, true, true); setup(b, 0, 0, 0, 0, true, false); CHECK(cmp(&a, &b) == false);
    // 7. both live: a further from its initial position (3,4 -> 25) than b (1,1 -> 2): true; positions read once each
    setup(a, 10, 10, 13, 14, true, false); setup(b, 10, 10, 11, 11, true, false);
    g_posCalls = 0; CHECK(cmp(&a, &b) == true); CHECK(g_posCalls == 2);
    // 8. both live, b further: false
    CHECK(cmp(&b, &a) == false);
    // 9. equal distances: false (strict greater)
    setup(b, 0, 0, 4, 3, true, false); CHECK(cmp(&a, &b) == false);
    // 10. z is ignored (only XY offset): a (3,4,z=50) vs b (0,5): equal -> false
    setup(b, 0, 0, 0, 5, true, false); CHECK(cmp(&a, &b) == false);
    if (fails == 0) printf("007483e0_TEST PASS\n");
    else printf("007483e0_TEST FAIL (%d)\n", fails);
    return fails;
}