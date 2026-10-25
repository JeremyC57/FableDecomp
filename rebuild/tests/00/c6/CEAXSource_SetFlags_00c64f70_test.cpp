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
    long SetFlags(unsigned long flags);
};

long CEAXSource::SetFlags(unsigned long flags)
{
    if (flags & ~7UL) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (flags != m_current.ulFlags) {
        m_current.ulFlags = m_deferred.ulFlags = flags;
        m_dirty |= 0x100000;
    }
    return 0;
}
static int fail() { std::printf("00c64f70_TEST FAIL\n"); return 1; }
int main()
{
    static CEAXContext ctx;
    static CEAXSource s;
    std::memset(&ctx, 0, sizeof(ctx));
    std::memset(&s, 0, sizeof(s));
    s.m_pContext = &ctx;
    // out of range: context error -2, result -3, nothing stored
    if (s.SetFlags(8UL) != -3 || ctx.m_lastError != -2 || s.m_dirty != 0 || s.m_current.ulFlags != 0) return fail();
    ctx.m_lastError = 0;
    // in range and changed: stored, dirty bit set, result 0
    if (s.SetFlags(5UL) != 0 || ctx.m_lastError != 0) return fail();
    if (s.m_current.ulFlags != 5UL || s.m_dirty != 0x100000) return fail();
    if (s.m_deferred.ulFlags != 5UL) return fail();
    // unchanged value: no new dirty bit
    s.m_dirty = 0;
    if (s.SetFlags(5UL) != 0 || s.m_dirty != 0) return fail();
    // the bounds themselves are accepted
    if (s.SetFlags(7UL) != 0 || s.m_current.ulFlags != 7UL || s.m_dirty != 0x100000) return fail();
    std::printf("00c64f70_TEST PASS\n");
    return 0;
}