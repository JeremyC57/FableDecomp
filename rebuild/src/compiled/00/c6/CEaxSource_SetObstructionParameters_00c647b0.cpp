// CEaxSource::SetObstructionParameters(const EAXOBSTRUCTIONPROPERTIES* value) -- EAX 4.0 source property EAXSOURCE_OBSTRUCTIONPARAMETERS: validate the pair (lObstruction -10000..0 mB, flObstructionLFRatio 0..1; else the context's last error -2 and result -3) and, when it changed, store it (current and deferred copy) and set dirty bit 1
// Leaf: ?SetObstructionParameters@CEaxSource@@QAEJPBUEAXOBSTRUCTIONPROPERTIES@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C647B0, 129 bytes (+ int3 padding), thiscall `ret 4`. Float pool: 0x0122DEDC = 0.0f,
//   0x0122DED8 = 1.0f.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 443 "0xC647B0/0xC64940 "CEntity::SetVectorProperty_XY/_Alt" = CEAXSource
//   obstruction/exclusion setters"): its only caller is the source's EAX property dispatcher 0x00C68510 at 0x00C68598 (`cmp size,8 ; jb`), the
//   switch that calls agent443's SetAllSourceProperties 0x00C67C00 and the single-field setters (SetObstruction 0x00C64AC0
//   etc.) on the same `this`; the pair lives at +0x28/+0x2c (EAXSOURCEPROPERTIES +0x10/+0x14) of the +0x18 property block. The class is agent442's
//   CEaxSource (agent443: CEAXSource): the dispatcher clears the +0x14 context's events (CEaxContext::ClearEvents
//   0x00C60410) and returns its +0x3c4 queue like CEaxSource::Set 0x00C61B40. EAX 4.0 SDK structure and field names; the
//   landed CEntity::SetVectorProperty_XY (SLevelRatioProperty) is a propagated label.
// Shape: one combined range test whose failure block sits at the end (`jl`/`jg` on the level, `test ah,5 ; jnp` = `< 0`,
//   `test ah,41h ; je` = `> 1`); the level is read once into a local while the ratio is re-read through the pointer;
//   the change test is `level != m || ratio != m` (fucompp, `test ah,44h ; jnp`); the stores are chained assignments `current = deferred = x` (deferred stored first, the float through `fst` + `fstp`).
struct EAXOBSTRUCTIONPROPERTIES {
    long  lObstruction;                                 // mB
    float flObstructionLFRatio;
};
struct CEaxContext {                           // the owning EAX context (landed 0x00C630D0 models its body)
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
struct CEaxSource {
    unsigned char        _pad00[0x4];
    unsigned long        m_dirty;                // +0x04 one bit per changed property
    unsigned char        _pad08[0x14 - 0x8];
    CEaxContext*         m_pContext;             // +0x14 (last error)
    SEAXSourceProperties m_current;              // +0x18
    unsigned char        _pad60[0x208 - 0x60];
    SEAXSourceProperties m_deferred;             // +0x208 (copy the update pass reads)
    long SetObstructionParameters(const EAXOBSTRUCTIONPROPERTIES* value);
};

long CEaxSource::SetObstructionParameters(const EAXOBSTRUCTIONPROPERTIES* value)
{
    long level = value->lObstruction;
    if (level < -10000 || level > 0 || value->flObstructionLFRatio < 0.0f || value->flObstructionLFRatio > 1.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (level != m_current.lObstruction || value->flObstructionLFRatio != m_current.flObstructionLFRatio) {
        m_current.lObstruction = m_deferred.lObstruction = level;
        m_current.flObstructionLFRatio = m_deferred.flObstructionLFRatio = value->flObstructionLFRatio;
        m_dirty |= 1;
    }
    return 0;
}
