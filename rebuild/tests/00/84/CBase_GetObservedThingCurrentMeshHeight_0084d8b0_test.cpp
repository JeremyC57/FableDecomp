#include <cstdio>
#include <cstdlib>
#include <cstring>
class CThing { public:
    unsigned char m_meshFlags; unsigned char m_lifeFlags; float m_height;
    float GetCurrentMeshHeight() { return m_height; }
};
struct CBaseIntelligentPointer { CThing* m_p; CThing* GetPItem() { return m_p; } };
struct CCameraManager { CBaseIntelligentPointer m_observedThing; CThing* GetObservedThing() { return m_observedThing.GetPItem(); } };
struct CBase {
    CCameraManager* m_pManager;
    float GetObservedThingCurrentMeshHeight()
    {
        CThing* thing = m_pManager->GetObservedThing();
        if (thing != 0 && (thing->m_lifeFlags & 1) == 0) {
            if (m_pManager->GetObservedThing()->m_meshFlags & 8) {
                float height = m_pManager->GetObservedThing()->GetCurrentMeshHeight();
                return height;
            }
        }
        return 1.0f;
    }
};
static bool Check()
{
    CThing t; t.m_meshFlags = 8; t.m_lifeFlags = 0; t.m_height = 2.5f;
    CCameraManager m; m.m_observedThing.m_p = 0;
    CBase b; b.m_pManager = &m;
    if (b.GetObservedThingCurrentMeshHeight() != 1.0f) return false;    // nothing observed
    m.m_observedThing.m_p = &t;
    if (b.GetObservedThingCurrentMeshHeight() != 2.5f) return false;    // live thing with a mesh
    t.m_lifeFlags = 1;
    if (b.GetObservedThingCurrentMeshHeight() != 1.0f) return false;    // dead
    t.m_lifeFlags = 0; t.m_meshFlags = 0;
    if (b.GetObservedThingCurrentMeshHeight() != 1.0f) return false;    // no mesh
    return true;
}
int main()
{
    if (!Check()) { std::printf("0084d8b0_TEST FAIL\n"); return 1; }
    std::printf("0084d8b0_TEST PASS\n");
    return 0;
}