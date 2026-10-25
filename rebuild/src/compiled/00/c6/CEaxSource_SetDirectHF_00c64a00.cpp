// CEaxSource::SetDirectHF(long directHF) -- EAX 4.0 source property lDirectHF: accepts -10000..0 mB, else the context's last error = -2 and result -3; on a change store it (current and deferred copy) and set dirty bit 0x10; returns 0
// Leaf: ?SetDirectHF@CEaxSource@@QAEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C64A00, 59 bytes (+ int3 padding), `ret 4`.
// Naming (sweep1, 2026-10-25): agent443's landed CEAXSource / CEAXContext respelled to agent442's CEaxSource /
//   CEaxContext: the same object (context +0x14 = CEaxContext, whose ClearEvents 0x00C60410 and event queue +0x3c4 the
//   source's property dispatcher 0x00C68510 uses exactly like CEaxSource::Set 0x00C61B40).
// Module: libpk's EAX 4.0 property layer (0x00C63xxx..0x00C69xxx; no PDB/RTTI, no trace strings). The caller
//   0x00C67C00 (SetAllSourceProperties) feeds the EAXSOURCEPROPERTIES words in order through these setters: +0 lDirect
//   0x00C649C0, +4 lDirectHF 0x00C64A00, +8 lRoom 0x00C64A40, +0x10 lObstruction 0x00C64AC0, +0x30 lOutsideVolumeHF
//   0x00C64D70, +0x44 ulFlags 0x00C64F70 -- the field offsets (+0x18 + index*4) and the EAX ranges fix the names.
//   Landed siblings on the same block: 0x00C647B0 (obstruction pair) and 0x00C64940 (exclusion pair).
// Layout: local model (no PDB class): dirty mask +0x04, context +0x14, current properties +0x18, deferred copy +0x208.
// Callees: none.
// Shape: one `||` range test moves the shared error block to the function end (`jl`/`jg`, `test eax,eax ; jg` for
//   an upper bound of 0); the change test is `value != member` (retail `cmp eax,[ecx+..]`). The chained store
//   `current = deferred = value` writes the deferred copy first.
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
    long SetDirectHF(long directHF);
};

long CEaxSource::SetDirectHF(long directHF)
{
    if (directHF < -10000 || directHF > 0) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (directHF != m_current.lDirectHF) {
        m_current.lDirectHF = m_deferred.lDirectHF = directHF;
        m_dirty |= 0x10;
    }
    return 0;
}