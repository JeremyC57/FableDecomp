// CEaxSource::SetAirAbsorptionFactor(float factor) -- EAX 4.0 source property flAirAbsorptionFactor: accepts 0..10.0f, else the context's last error = -2 and result -3; on a change store it (current and deferred copy) and set dirty bit 0x80000; returns 0
// Leaf: ?SetAirAbsorptionFactor@CEaxSource@@QAEJM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C64F00, 102 bytes, thiscall, `ret 4`.
// Identity (probable): the landed SetAllSourceProperties 0x00C67C00 feeds the EAXSOURCEPROPERTIES words in order through these
//   setters and calls this one at 0x00C67F1B for the word at +0x40 (flAirAbsorptionFactor, EAX 4.0 SDK range 0..10.0f: the bounds
//   0x0122DEDC = 0.0 and 0x01237F0C = 10.0f); the property dispatcher 0x00C68510 is the second caller. Class sweep1-descriptive
//   (libpk, no PDB/RTTI).
// Shape (family K064, representative 0x00C64B00): the landed CEaxSource::SetObstruction 0x00C64AC0 with a float: one `||`
//   range test sends both failures to the shared error tail; the change test `value != member` is a fucompp; the chained
//   store `current = deferred = value` moves the float through eax/edx.
struct CEaxContext {
    unsigned char _pad000[0x3d4];
    long          m_lastError;                   // +0x3d4
};
struct SEAXSourceProperties {
    long          lDirect;
    long          lDirectHF;
    long          lRoom;
    long          lRoomHF;
    long          lObstruction;
    float         flObstructionLFRatio;          // +0x14
    long          lOcclusion;
    float         flOcclusionLFRatio;
    float         flOcclusionRoomRatio;
    float         flOcclusionDirectRatio;
    long          lExclusion;
    float         flExclusionLFRatio;
    long          lOutsideVolumeHF;
    float         flDopplerFactor;
    float         flRolloffFactor;
    float         flRoomRolloffFactor;
    float         flAirAbsorptionFactor;
    unsigned long ulFlags;
};
struct CEaxSource {
    unsigned char        _pad00[0x4];
    unsigned long        m_dirty;
    unsigned char        _pad08[0x14 - 0x8];
    CEaxContext*         m_pContext;
    SEAXSourceProperties m_current;
    unsigned char        _pad60[0x208 - 0x60];
    SEAXSourceProperties m_deferred;
    long SetAirAbsorptionFactor(float factor);
};
long CEaxSource::SetAirAbsorptionFactor(float factor)
{
    if (factor < 0.0f || factor > 10.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (factor != m_current.flAirAbsorptionFactor) {
        m_current.flAirAbsorptionFactor = m_deferred.flAirAbsorptionFactor = factor;
        m_dirty |= 0x80000;
    }
    return 0;
}