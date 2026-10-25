// Behaviour test for 00c64940: CEaxSource::SetExclusionParameters -- range check, change detection, dirty bit 0x400000
#include <cstdio>
#include <cstring>
struct EAXEXCLUSIONPROPERTIES { long lExclusion; float flExclusionLFRatio; };
struct SProps { long lExclusion; float flExclusionLFRatio; };
struct CEaxContext { long m_lastError; };
struct CEaxSource {
    unsigned long m_dirty; CEaxContext* m_pContext; SProps m_current; SProps m_deferred;
    long SetExclusionParameters(const EAXEXCLUSIONPROPERTIES* value)
    {
        long level = value->lExclusion;
        if (level < -10000 || level > 0 || value->flExclusionLFRatio < 0.0f || value->flExclusionLFRatio > 1.0f) { m_pContext->m_lastError = -2; return -3; }
        if (level != m_current.lExclusion || value->flExclusionLFRatio != m_current.flExclusionLFRatio) {
        m_current.lExclusion = level;
        m_current.flExclusionLFRatio = value->flExclusionLFRatio;
            m_dirty |= 0x400000;
        }
        return 0;
    }
};
int main()
{
    CEaxContext c; c.m_lastError = 0; CEaxSource s; std::memset(&s, 0, sizeof s); s.m_pContext = &c;
    EAXEXCLUSIONPROPERTIES bad = { 10, 0.5f }; EAXEXCLUSIONPROPERTIES bad2 = { -10, 1.5f }; EAXEXCLUSIONPROPERTIES good = { -2500, 0.25f };
    int ok = s.SetExclusionParameters(&bad) == -3 && c.m_lastError == -2 && s.SetExclusionParameters(&bad2) == -3 && s.m_dirty == 0;
    ok = ok && s.SetExclusionParameters(&good) == 0 && s.m_current.lExclusion == -2500 && s.m_current.flExclusionLFRatio == 0.25f && s.m_dirty == (unsigned long)0x400000;
    ok = ok && (0 == 0 || (s.m_deferred.lExclusion == -2500 && s.m_deferred.flExclusionLFRatio == 0.25f));
    s.m_dirty = 0; ok = ok && s.SetExclusionParameters(&good) == 0 && s.m_dirty == 0;
    std::printf(ok ? "00c64940_TEST PASS\n" : "00c64940_TEST FAIL\n");
    return ok ? 0 : 1;
}
