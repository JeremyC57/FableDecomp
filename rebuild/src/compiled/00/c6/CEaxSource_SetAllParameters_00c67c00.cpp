// CEaxSource::SetAllParameters(const EAXSOURCEPROPERTIES* value) -- EAX 4.0 source property EAXSOURCE_ALLPARAMETERS: validate every field of the record against the SDK ranges (last error -2 / result -3 on any failure), then apply it field by field through the single-property setters, stopping at the first negative result, and flag the all-parameters update
// Leaf: ?SetAllParameters@CEaxSource@@QAEJPBUEAXSOURCEPROPERTIES@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C67C00, 854 bytes, thiscall `ret 4`.
// Identity (descriptive; libpk's EAX 4.0 property layer has no PDB, RTTI or trace strings): sole caller 0x00C68553 is the
//   source property dispatcher 0x00C68510 (the EAXSOURCE_* switch whose other cases call the landed single-field setters with
//   the same `this`); this row validates the 18 EAXSOURCEPROPERTIES fields (SDK ranges: mB levels -10000..1000 / ..0, ratios
//   0..1, factors 0..10, flags bits 0..2; float pool 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 10.0f 0x01237F0C) and feeds the words in
//   order through SetDirect 0x00C649C0 .. SetFlags 0x00C64F70 (all landed), then sets dirty bit 21. It is the source-side
//   twin of the landed CEaxContext::SetAllParameters 0x00C67AE0 (EAXCONTEXT_ALLPARAMETERS), spelled the same way; the landed
//   setter rows' comments call it "SetAllSourceProperties" (agent443's working name, never bound). The catalog label
//   "ScriptCommand_SetCharacterAttributes" is a propagated game name.
// Callees (CEaxSource, thiscall `ret 4`): SetDirect 0x00C649C0, SetDirectHF 0x00C64A00, SetRoom 0x00C64A40, SetRoomHF
//   0x00C64A80, SetObstruction 0x00C64AC0, SetObstructionLFRatio 0x00C64B00, SetOcclusion 0x00C64B70, SetOcclusionLFRatio
//   0x00C64BB0, SetOcclusionRoomRatio 0x00C64C10, SetOcclusionDirectRatio 0x00C64C70, SetExclusion 0x00C64CD0,
//   SetExclusionLFRatio 0x00C64D10, SetOutsideVolumeHF 0x00C64D70, SetDopplerFactor 0x00C64DB0, SetRolloffFactor 0x00C64E20,
//   SetRoomRolloffFactor 0x00C64E90, SetAirAbsorptionFactor 0x00C64F00, SetFlags 0x00C64F70.
// Shape: one `||` chain of range tests sends the shared error block to the function end; flExclusionLFRatio (+0x2c) is the
//   one field retail does not range-check here. Retail reloads ecx before some setters and not others: VC7.1 only knows
//   which callee preserves ecx when the callee bodies are in the TU (chunk 686 / landed 0x00C67AE0 recipe), so the 18
//   landed setter bodies sit here under __declspec(noinline). The last setter's result is returned as is.
struct CEaxContext {                           // the owning EAX context (landed 0x00C630D0 models its body)
    unsigned char _pad000[0x3d4];
    long          m_lastError;                   // +0x3d4
};
struct EAXSOURCEPROPERTIES {                     // EAX 4.0 EAXSOURCEPROPERTIES, 0x48 bytes
    long          lDirect;                       // +0x00 -10000..1000 mB
    long          lDirectHF;                     // +0x04 -10000..0
    long          lRoom;                         // +0x08 -10000..1000
    long          lRoomHF;                       // +0x0c -10000..0
    long          lObstruction;                  // +0x10 -10000..0
    float         flObstructionLFRatio;          // +0x14 0..1
    long          lOcclusion;                    // +0x18 -10000..0
    float         flOcclusionLFRatio;            // +0x1c 0..1
    float         flOcclusionRoomRatio;          // +0x20 0..10
    float         flOcclusionDirectRatio;        // +0x24 0..10
    long          lExclusion;                    // +0x28 -10000..0
    float         flExclusionLFRatio;            // +0x2c 0..1
    long          lOutsideVolumeHF;              // +0x30 -10000..0
    float         flDopplerFactor;               // +0x34 0..10
    float         flRolloffFactor;               // +0x38 0..10
    float         flRoomRolloffFactor;           // +0x3c 0..10
    float         flAirAbsorptionFactor;         // +0x40 0..10
    unsigned long ulFlags;                       // +0x44 EAXSOURCEFLAGS_* (bits 0..2)
};
struct CEaxSource {
    unsigned char        _pad00[0x4];
    unsigned long        m_dirty;                // +0x04 one bit per changed property
    unsigned char        _pad08[0x14 - 0x8];
    CEaxContext*         m_pContext;             // +0x14 (last error)
    EAXSOURCEPROPERTIES  m_current;              // +0x18
    unsigned char        _pad60[0x208 - 0x60];
    EAXSOURCEPROPERTIES  m_deferred;             // +0x208 (copy the update pass reads)
    long SetDirect(long direct);                              // 0x00C649C0
    long SetDirectHF(long directHF);                          // 0x00C64A00
    long SetRoom(long room);                                  // 0x00C64A40
    long SetRoomHF(long value);                               // 0x00C64A80
    long SetObstruction(long obstruction);                    // 0x00C64AC0
    long SetObstructionLFRatio(float ratio);                  // 0x00C64B00
    long SetOcclusion(long value);                            // 0x00C64B70
    long SetOcclusionLFRatio(float ratio);                    // 0x00C64BB0
    long SetOcclusionRoomRatio(float ratio);                  // 0x00C64C10
    long SetOcclusionDirectRatio(float ratio);                // 0x00C64C70
    long SetExclusion(long value);                            // 0x00C64CD0
    long SetExclusionLFRatio(float ratio);                    // 0x00C64D10
    long SetOutsideVolumeHF(long outsideVolumeHF);            // 0x00C64D70
    long SetDopplerFactor(float factor);                      // 0x00C64DB0
    long SetRolloffFactor(float factor);                      // 0x00C64E20
    long SetRoomRolloffFactor(float factor);                  // 0x00C64E90
    long SetAirAbsorptionFactor(float factor);                // 0x00C64F00
    long SetFlags(unsigned long flags);                       // 0x00C64F70
    long SetAllParameters(const EAXSOURCEPROPERTIES* value);
};

// The landed setter bodies (same TU: VC7.1 reads their register facts; never graded here).
__declspec(noinline) long CEaxSource::SetDirect(long direct)
{
    if (direct < -10000 || direct > 1000) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (direct != m_current.lDirect) {
        m_current.lDirect = m_deferred.lDirect = direct;
        m_dirty |= 0x8;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetDirectHF(long directHF)
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
__declspec(noinline) long CEaxSource::SetRoom(long room)
{
    if (room < -10000 || room > 1000) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (room != m_current.lRoom) {
        m_current.lRoom = room;
        m_dirty |= 0x400000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetRoomHF(long value)
{
    if (value < -10000 || value > 0) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (value != m_current.lRoomHF) {
        m_current.lRoomHF = value;
        m_dirty |= 0x400000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetObstruction(long obstruction)
{
    if (obstruction < -10000 || obstruction > 0) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (obstruction != m_current.lObstruction) {
        m_current.lObstruction = m_deferred.lObstruction = obstruction;
        m_dirty |= 0x80;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetObstructionLFRatio(float ratio)
{
    if (ratio < 0.0f || ratio > 1.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (ratio != m_current.flObstructionLFRatio) {
        m_current.flObstructionLFRatio = m_deferred.flObstructionLFRatio = ratio;
        m_dirty |= 0x100;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetOcclusion(long value)
{
    if (value < -10000 || value > 0) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (value != m_current.lOcclusion) {
        m_current.lOcclusion = value;
        m_dirty |= 0x400000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetOcclusionLFRatio(float ratio)
{
    if (ratio < 0.0f || ratio > 1.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (ratio != m_current.flOcclusionLFRatio) {
        m_current.flOcclusionLFRatio = ratio;
        m_dirty |= 0x400000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetOcclusionRoomRatio(float ratio)
{
    if (ratio < 0.0f || ratio > 10.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (ratio != m_current.flOcclusionRoomRatio) {
        m_current.flOcclusionRoomRatio = ratio;
        m_dirty |= 0x400000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetOcclusionDirectRatio(float ratio)
{
    if (ratio < 0.0f || ratio > 10.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (ratio != m_current.flOcclusionDirectRatio) {
        m_current.flOcclusionDirectRatio = ratio;
        m_dirty |= 0x400000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetExclusion(long value)
{
    if (value < -10000 || value > 0) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (value != m_current.lExclusion) {
        m_current.lExclusion = value;
        m_dirty |= 0x400000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetExclusionLFRatio(float ratio)
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
__declspec(noinline) long CEaxSource::SetOutsideVolumeHF(long outsideVolumeHF)
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
__declspec(noinline) long CEaxSource::SetDopplerFactor(float factor)
{
    if (factor < 0.0f || factor > 10.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (factor != m_current.flDopplerFactor) {
        m_current.flDopplerFactor = m_deferred.flDopplerFactor = factor;
        m_dirty |= 0x10000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetRolloffFactor(float factor)
{
    if (factor < 0.0f || factor > 10.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (factor != m_current.flRolloffFactor) {
        m_current.flRolloffFactor = m_deferred.flRolloffFactor = factor;
        m_dirty |= 0x20000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetRoomRolloffFactor(float factor)
{
    if (factor < 0.0f || factor > 10.0f) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    if (factor != m_current.flRoomRolloffFactor) {
        m_current.flRoomRolloffFactor = m_deferred.flRoomRolloffFactor = factor;
        m_dirty |= 0x40000;
    }
    return 0;
}
__declspec(noinline) long CEaxSource::SetAirAbsorptionFactor(float factor)
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
__declspec(noinline) long CEaxSource::SetFlags(unsigned long flags)
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

long CEaxSource::SetAllParameters(const EAXSOURCEPROPERTIES* value)
{
    if (value->lDirect < -10000 || value->lDirect > 1000 ||
        value->lDirectHF < -10000 || value->lDirectHF > 0 ||
        value->lRoom < -10000 || value->lRoom > 1000 ||
        value->lRoomHF < -10000 || value->lRoomHF > 0 ||
        value->lObstruction < -10000 || value->lObstruction > 0 ||
        value->flObstructionLFRatio < 0.0f || value->flObstructionLFRatio > 1.0f ||
        value->lOcclusion < -10000 || value->lOcclusion > 0 ||
        value->flOcclusionLFRatio < 0.0f || value->flOcclusionLFRatio > 1.0f ||
        value->flOcclusionRoomRatio < 0.0f || value->flOcclusionRoomRatio > 10.0f ||
        value->flOcclusionDirectRatio < 0.0f || value->flOcclusionDirectRatio > 10.0f ||
        value->lExclusion < -10000 || value->lExclusion > 0 ||
        value->lOutsideVolumeHF < -10000 || value->lOutsideVolumeHF > 0 ||
        value->flDopplerFactor < 0.0f || value->flDopplerFactor > 10.0f ||
        value->flRolloffFactor < 0.0f || value->flRolloffFactor > 10.0f ||
        value->flRoomRolloffFactor < 0.0f || value->flRoomRolloffFactor > 10.0f ||
        value->flAirAbsorptionFactor < 0.0f || value->flAirAbsorptionFactor > 10.0f ||
        (value->ulFlags & ~7UL)) {
        m_pContext->m_lastError = -2;
        return -3;
    }
    long result;
    if ((result = SetDirect(value->lDirect)) < 0) return result;
    if ((result = SetDirectHF(value->lDirectHF)) < 0) return result;
    if ((result = SetRoom(value->lRoom)) < 0) return result;
    if ((result = SetRoomHF(value->lRoomHF)) < 0) return result;
    if ((result = SetObstruction(value->lObstruction)) < 0) return result;
    if ((result = SetObstructionLFRatio(value->flObstructionLFRatio)) < 0) return result;
    if ((result = SetOcclusion(value->lOcclusion)) < 0) return result;
    if ((result = SetOcclusionLFRatio(value->flOcclusionLFRatio)) < 0) return result;
    if ((result = SetOcclusionRoomRatio(value->flOcclusionRoomRatio)) < 0) return result;
    if ((result = SetOcclusionDirectRatio(value->flOcclusionDirectRatio)) < 0) return result;
    if ((result = SetExclusion(value->lExclusion)) < 0) return result;
    if ((result = SetExclusionLFRatio(value->flExclusionLFRatio)) < 0) return result;
    if ((result = SetOutsideVolumeHF(value->lOutsideVolumeHF)) < 0) return result;
    if ((result = SetDopplerFactor(value->flDopplerFactor)) < 0) return result;
    if ((result = SetRolloffFactor(value->flRolloffFactor)) < 0) return result;
    if ((result = SetRoomRolloffFactor(value->flRoomRolloffFactor)) < 0) return result;
    if ((result = SetAirAbsorptionFactor(value->flAirAbsorptionFactor)) < 0) return result;
    if ((result = SetFlags(value->ulFlags)) < 0) return result;
    m_dirty |= 0x200000;
    return result;
}