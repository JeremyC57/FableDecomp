#include <cstdio>
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float a, float b, float c) : x(a), y(b), z(c) { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
struct CThingCreatureBase {
    C3DVector m_head; bool m_lastExact;
    C3DVector GetHeadPos(bool exact) { m_lastExact = exact; return m_head; }
};
static bool g_bQueryCreatureHeadPos;
struct CTCLook {
    CThingCreatureBase* PParentCreature; C3DVector CachedHeadPos;
    C3DVector GetHeadPos()
    {
        if (g_bQueryCreatureHeadPos) return PParentCreature->GetHeadPos(false);
        return CachedHeadPos;
    }
};
int main()
{
    CThingCreatureBase c; c.m_head = C3DVector(1, 2, 3); c.m_lastExact = true;
    CTCLook l; l.PParentCreature = &c; l.CachedHeadPos = C3DVector(7, 8, 9);
    g_bQueryCreatureHeadPos = false;
    C3DVector v = l.GetHeadPos();
    if (v.x != 7 || v.y != 8 || v.z != 9 || !c.m_lastExact) { std::printf("00751b30_TEST FAIL\n"); return 1; }
    g_bQueryCreatureHeadPos = true;
    v = l.GetHeadPos();
    if (v.x != 1 || v.y != 2 || v.z != 3 || c.m_lastExact) { std::printf("00751b30_TEST FAIL\n"); return 1; }
    std::printf("00751b30_TEST PASS\n");
    return 0;
}