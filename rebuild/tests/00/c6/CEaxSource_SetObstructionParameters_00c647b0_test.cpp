// Behaviour test for 00c647b0: CEaxSource::SetObstructionParameters -- range check, change detection, dirty bit 1
#include <cstdio>
#include <cstring>
struct EAXOBSTRUCTIONPROPERTIES { long lObstruction; float flObstructionLFRatio; };
struct SProps { long lObstruction; float flObstructionLFRatio; };
struct CEaxContext { long m_lastError; };
struct CEaxSource {
    unsigned long m_dirty; CEaxContext* m_pContext; SProps m_current; SProps m_deferred;
    long SetObstructionParameters(const EAXOBSTRUCTIONPROPERTIES* value)
    {
        long level = value->lObstruction;
        if (level < -10000 || level > 0 || value->flObstructionLFRatio < 0.0f || value->flObstructionLFRatio > 1.0f) { m_pContext->m_lastError = -2; return -3; }
        if (level != m_current.lObstruction || value->flObstructionLFRatio != m_current.flObstructionLFRatio) {
        m_current.lObstruction = m_deferred.lObstruction = level;
        m_current.flObstructionLFRatio = m_deferred.flObstructionLFRatio = value->flObstructionLFRatio;
            m_dirty |= 1;
        }
        return 0;
    }
};
int main()
{
    CEaxContext c; c.m_lastError = 0; CEaxSource s; std::memset(&s, 0, sizeof s); s.m_pContext = &c;
    EAXOBSTRUCTIONPROPERTIES bad = { 10, 0.5f }; EAXOBSTRUCTIONPROPERTIES bad2 = { -10, 1.5f }; EAXOBSTRUCTIONPROPERTIES good = { -2500, 0.25f };
    int ok = s.SetObstructionParameters(&bad) == -3 && c.m_lastError == -2 && s.SetObstructionParameters(&bad2) == -3 && s.m_dirty == 0;
    ok = ok && s.SetObstructionParameters(&good) == 0 && s.m_current.lObstruction == -2500 && s.m_current.flObstructionLFRatio == 0.25f && s.m_dirty == (unsigned long)1;
    ok = ok && (1 == 0 || (s.m_deferred.lObstruction == -2500 && s.m_deferred.flObstructionLFRatio == 0.25f));
    s.m_dirty = 0; ok = ok && s.SetObstructionParameters(&good) == 0 && s.m_dirty == 0;
    std::printf(ok ? "00c647b0_TEST PASS\n" : "00c647b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
