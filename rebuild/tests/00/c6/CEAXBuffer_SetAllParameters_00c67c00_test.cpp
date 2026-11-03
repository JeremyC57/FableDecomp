// Behaviour test for 0x00C67C00 CEAXBuffer::SetAllParameters: reject any of the 17 EAX 4.0 source properties outside its
// range or a flag outside bits 0..2 (context last error -2, result -3); otherwise apply the eighteen setters in property
// order, stopping at the first negative result.
#include <cstdio>
struct Props {
    long lDirect, lDirectHF, lRoom, lRoomHF, lObstruction; float flObstructionLFRatio;
    long lOcclusion; float flOcclusionLFRatio, flOcclusionRoomRatio, flOcclusionDirectRatio;
    long lExclusion; float flExclusionLFRatio; long lOutsideVolumeHF;
    float flDopplerFactor, flRolloffFactor, flRoomRolloffFactor, flAirAbsorptionFactor; unsigned long ulFlags;
};
struct Ctx { long m_lastError; };
struct Src {
    Ctx* m_pContext; int calls; long failAt;
    long Step() { ++calls; return calls == failAt ? -5 : 0; }
    long SetAllParameters(const Props* v)
    {
        if (v->lDirect < -10000 || v->lDirect > 1000 || v->lDirectHF < -10000 || v->lDirectHF > 0 ||
            v->lRoom < -10000 || v->lRoom > 1000 || v->lRoomHF < -10000 || v->lRoomHF > 0 ||
            v->lObstruction < -10000 || v->lObstruction > 0 || v->flObstructionLFRatio < 0.0f || v->flObstructionLFRatio > 1.0f ||
            v->lOcclusion < -10000 || v->lOcclusion > 0 || v->flOcclusionLFRatio < 0.0f || v->flOcclusionLFRatio > 1.0f ||
            v->flOcclusionRoomRatio < 0.0f || v->flOcclusionRoomRatio > 10.0f || v->flOcclusionDirectRatio < 0.0f || v->flOcclusionDirectRatio > 10.0f ||
            v->lExclusion < -10000 || v->lExclusion > 0 || v->lOutsideVolumeHF < -10000 || v->lOutsideVolumeHF > 0 ||
            v->flDopplerFactor < 0.0f || v->flDopplerFactor > 10.0f || v->flRolloffFactor < 0.0f || v->flRolloffFactor > 10.0f ||
            v->flRoomRolloffFactor < 0.0f || v->flRoomRolloffFactor > 10.0f || v->flAirAbsorptionFactor < 0.0f || v->flAirAbsorptionFactor > 10.0f ||
            (v->ulFlags & ~7UL)) { m_pContext->m_lastError = -2; return -3; }
        for (int n = 0; n < 18; ++n) { long r = Step(); if (r < 0) return r; }
        return 0;
    }
};
static int fail(const char* why) { std::printf("00c67c00_TEST FAIL %s\n", why); return 1; }
int main()
{
    Props p = { 0, -100, 0, -100, -200, 0.5f, -300, 0.25f, 1.5f, 1.0f, -400, 1.0f, -500, 1.0f, 1.0f, 0.0f, 1.0f, 7 };
    Ctx c; c.m_lastError = 0; Src s; s.m_pContext = &c; s.calls = 0; s.failAt = 0;
    if (s.SetAllParameters(&p) != 0 || s.calls != 18 || c.m_lastError != 0) return fail("accept");
    s.calls = 0; s.failAt = 5;
    if (s.SetAllParameters(&p) != -5 || s.calls != 5) return fail("stop at first failing setter");
    s.calls = 0; s.failAt = 0;
    p.lDirect = 1001;
    if (s.SetAllParameters(&p) != -3 || c.m_lastError != -2 || s.calls != 0) return fail("lDirect high");
    p.lDirect = 1000; c.m_lastError = 0; p.flOcclusionRoomRatio = 10.5f;
    if (s.SetAllParameters(&p) != -3 || c.m_lastError != -2) return fail("room ratio high");
    p.flOcclusionRoomRatio = 10.0f; c.m_lastError = 0; p.ulFlags = 8;
    if (s.SetAllParameters(&p) != -3 || c.m_lastError != -2) return fail("flag bit 3");
    p.ulFlags = 0; p.lRoomHF = -10001; c.m_lastError = 0;
    if (s.SetAllParameters(&p) != -3) return fail("lRoomHF low");
    p.lRoomHF = -10000; p.flExclusionLFRatio = 2.0f; c.m_lastError = 0; s.calls = 0;
    if (s.SetAllParameters(&p) != 0 || s.calls != 18) return fail("exclusion LF ratio is not range-checked by retail");
    std::printf("00c67c00_TEST PASS\n");
    return 0;
}