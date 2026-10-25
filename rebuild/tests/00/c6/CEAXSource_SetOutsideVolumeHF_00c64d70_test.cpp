#include <cstdio>
#include <cstring>
struct CEAXContext {                           // the owning EAX context (landed 0x00C630D0 models its body)
    unsigned char _pad000[0x3d4];
    long          m_lastError;                   // +0x3d4
};
struct SEAXSourceProperties {                    // EAX 4.0 EAXSOURCEPROPERTIES, 0x48 bytes
    long          lDirect;                       // +0x00 -10000..1000 mB
    long          lDirectHF;                     // +0x04 -10000..0
    long          lRoom;                         // +0x08 -10000..1000
    long          lRoomHF;                       // +0x0c -10000..0
    long          lObstruction;                  // +0x10 -10000..0
    float         flObstructionLFRatio;          // +0x14
    long          lOcclusion;                    // +0x18
    float         flOcclusionLFRatio;            // +0x1c
    float         flOcclusionRoomRatio;          // +0x20
    float         flOcclusionDirectRatio;        // +0x24
    long          lExclusion;                    // +0x28
    float         flExclusionLFRatio;            // +0x2c
    long          lOutsideVolumeHF;              // +0x30 -10000..0
    float         flDopplerFactor;               // +0x34
    float         flRolloffFactor;               // +0x38
    float         flRoomRolloffFactor;           // +0x3c
    float         flAirAbsorptionFactor;         // +0x40
    unsigned long ulFlags;                       // +0x44 EAXSOURCEFLAGS_* (bits 0..2)
};
struct CEAXSource {
    unsigned char        _pad00[0x4];
    unsigned long        m_dirty;                // +0x04 one bit per changed property
    unsigned char        _pad08[0x14 - 0x8];
    CEAXContext*         m_pContext;             // +0x14 (last error)
    SEAXSourceProperties m_current;              // +0x18
    unsigned char        _pad60[0x208 - 0x60];
    SEAXSourceProperties m_deferred;             // +0x208 (copy the update pass reads)
    long SetOutsideVolumeHF(long outsideVolumeHF);
};

long CEAXSource::SetOutsideVolumeHF(long outsideVolumeHF)
{
    if (outsideVolumeHF < -10000 || outsideVolumeHF > 0) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (outsideVolumeHF != m_current.lOutsideVolumeHF) {
        m_current.lOutsideVolumeHF = m_deferred.lOutsideVolumeHF = outsideVolumeHF;
        m_dirty |= 0x8000;
    }
    return 0;
}
static int fail() { std::printf("00c64d70_TEST FAIL\n"); return 1; }
int main()
{
    static CEAXContext ctx;
    static CEAXSource s;
    std::memset(&ctx, 0, sizeof(ctx));
    std::memset(&s, 0, sizeof(s));
    s.m_pContext = &ctx;
    // out of range: context error -2, result -3, nothing stored
    if (s.SetOutsideVolumeHF(1) != -3 || ctx.m_lastError != -2 || s.m_dirty != 0 || s.m_current.lOutsideVolumeHF != 0) return fail();
    ctx.m_lastError = 0;
    if (s.SetOutsideVolumeHF(-10001) != -3 || ctx.m_lastError != -2) return fail();
    ctx.m_lastError = 0;
    // in range and changed: stored, dirty bit set, result 0
    if (s.SetOutsideVolumeHF(-9995) != 0 || ctx.m_lastError != 0) return fail();
    if (s.m_current.lOutsideVolumeHF != -9995 || s.m_dirty != 0x8000) return fail();
    if (s.m_deferred.lOutsideVolumeHF != -9995) return fail();
    // unchanged value: no new dirty bit
    s.m_dirty = 0;
    if (s.SetOutsideVolumeHF(-9995) != 0 || s.m_dirty != 0) return fail();
    // the bounds themselves are accepted
    if (s.SetOutsideVolumeHF(-10000) != 0 || s.m_current.lOutsideVolumeHF != -10000 || s.m_dirty != 0x8000) return fail();
    std::printf("00c64d70_TEST PASS\n");
    return 0;
}