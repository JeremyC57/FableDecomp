// Behaviour test for CQuestManager::ActivateFeat(long) @ 0x004AF300
#include <cstdio>

struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CMessageEventManager {
    long m_type; unsigned long m_delay; float m_px, m_py, m_pz; CThing* m_from; CThing* m_to; float m_v1, m_v2; long m_data; int m_calls;
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data)
    {
        m_type = type; m_delay = delay; m_px = position.X; m_py = position.Y; m_pz = position.Z;
        m_from = from; m_to = to; m_v1 = value1; m_v2 = value2; m_data = data; ++m_calls;
        return 0;
    }
};

struct CWorld { CMessageEventManager* PMessageEventManager; };

struct CQuestManager {
    CWorld* World;
    bool ActivateFeat(long featIndex)
    {
        World->PMessageEventManager->AddMessage(0x38, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, featIndex);
        return true;
    }
};

int main()
{
    CMessageEventManager m; m.m_calls = 0;
    CWorld w; w.PMessageEventManager = &m;
    CQuestManager q; q.World = &w;
    bool ok = q.ActivateFeat(17);
    if (!ok || m.m_calls != 1 || m.m_type != 0x38 || m.m_delay != 0x32 || m.m_data != 17) { std::printf("004af300_TEST FAIL\n"); return 1; }
    if (m.m_px != 0.0f || m.m_py != 0.0f || m.m_pz != 0.0f || m.m_from != 0 || m.m_to != 0 || m.m_v1 != 0.0f || m.m_v2 != 0.0f) { std::printf("004af300_TEST FAIL\n"); return 1; }
    std::printf("004af300_TEST PASS\n");
    return 0;
}
