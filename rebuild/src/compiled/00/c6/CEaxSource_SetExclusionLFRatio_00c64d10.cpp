// CEaxSource::SetExclusionLFRatio(float ratio) -- EAX 4.0 source property flExclusionLFRatio: accepts 0..1.0f, else the context's last error = -2 and result -3; on a change store it and set dirty bit 0x400000; returns 0
// Leaf: ?SetExclusionLFRatio@CEaxSource@@QAEJM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c64d10, 90 bytes, thiscall, `ret 4`.
// Identity (probable): the landed SetAllSourceProperties 0x00C67C00 feeds the EAXSOURCEPROPERTIES words in order through these setters and calls this one at 0x00C67ED6 for the word at source+0x44 (props+0x2c, flExclusionLFRatio; EAX 4.0 SDK range 0..1.0f: the bounds 0x0122DEDC = 0.0 and 0x0122DED8); the property dispatcher 0x00C68510 is the second caller (0x00C6877F). Class sweep1-descriptive (libpk, no PDB/RTTI).
// Shape (family L135 = the landed CEaxSource::SetRolloffFactor 0x00C64E20 without the deferred copy): one `||` range test
//   sends both failures to the shared error tail; the change test `value != member` is a fucompp.
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
    float         flOcclusionLFRatio;            // +0x1c
    float         flOcclusionRoomRatio;          // +0x20
    float         flOcclusionDirectRatio;        // +0x24
    long          lExclusion;
    float         flExclusionLFRatio;            // +0x2c
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
    SEAXSourceProperties m_current;              // +0x18
    unsigned char        _pad60[0x208 - 0x60];
    SEAXSourceProperties m_deferred;
    long SetExclusionLFRatio(float ratio);
};
long CEaxSource::SetExclusionLFRatio(float ratio)
{
    if (ratio < 0.0f || ratio > 1.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (ratio != m_current.flExclusionLFRatio) {
        m_current.flExclusionLFRatio = ratio;
        m_dirty |= 0x400000;
    }
    return 0;
}