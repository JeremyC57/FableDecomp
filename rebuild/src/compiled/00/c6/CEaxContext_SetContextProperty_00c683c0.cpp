// CEaxContext::SetContextProperty(unsigned long property, const void* value, unsigned long size, SEaxEvent** ppEvents) -- EAX 4.0 context property dispatcher: clear the event queue, set the property named by `property & 0x7FFFFFFF` through its single-field setter (size-checked), then unless the deferred bit 0x80000000 is set commit the deferred settings and hand back the event queue
// Leaf: ?SetContextProperty@CEaxContext@@QAEJKPBXKPAPAUSEaxEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C683C0, 328 bytes incl. the 7-entry jump table at 0x00C684EC; thiscall `ret 10h`.
// Identity (descriptive: the EAX emulation layer next to libpk has no PDB or RTTI): `this` is the CEaxContext of the landed
//   rows -- ClearEvents 0x00C60410 first (as CEaxSource::Set 0x00C61B40 does on its context), the event queue +0x3c4 handed
//   back, last error +0x3d4, and the cases call the landed context setters in EAX 4.0 EAXCONTEXT_* enum order: 1 ALLPARAMETERS
//   SetAllParameters 0x00C67AE0 (size 0x1c = sizeof EAXCONTEXTPROPERTIES), 2 PRIMARYFXSLOTID SetPrimaryFXSlotID 0x00C64420
//   (16-byte GUID by value), 3 DISTANCEFACTOR 0x00C645D0, 4 AIRABSORPTIONHF 0x00C64630, 5 HFREFERENCE 0x00C646E0, 6 LASTERROR
//   (read-only: last error = -1, result -3). Siblings of the same shape: the source dispatcher 0x00C68510 and the FX-slot
//   dispatcher 0x00C68F90 (both call the same commit 0x00C67F60). No callers or absolute references (unused entry).
//   The catalog label "CDrawGuildSeal_ApplyEffect" is a propagated game name.
// Callees: ClearEvents 0x00C60410, the five setters above, CommitDeferredSettings 0x00C67F60 (descriptive: flushes the
//   dirty bits +0x3bc 0x2000000 / 0x1000000 to the renderer; catalog "CTCHeroStats_UpdateAttributeEvents" is propagated).
// Shape: case 6 falls into the default's `result = -3; break;` -- retail's re-assigned `mov esi,-3` in the duplicated
//   epilogue; a `return -3` folds to `mov eax,-3` (probe log). Null/size failures `break` with result -3, which VC threads
//   straight to the exit.
struct SGuid {
    unsigned long  Data1;
    unsigned short Data2, Data3;
    unsigned char  Data4[8];
};
struct EAXCONTEXTPROPERTIES {
    SGuid guidPrimaryFXSlotID;
    float flDistanceFactor;
    float flAirAbsorptionHF;
    float flHFReference;
};
struct SEaxEvent;
class CEaxContext {
public:
    unsigned char _pad000[0x3c4];
    SEaxEvent*    m_pEvents;                       // +0x3c4
    unsigned char _pad3c8[0x3d4 - 0x3c8];
    long          m_lastError;                     // +0x3d4
    void ClearEvents();                            // 0x00C60410
    long SetPrimaryFXSlotID(SGuid id);             // 0x00C64420
    long SetDistanceFactor(float value);           // 0x00C645D0
    long SetAirAbsorptionHF(float value);          // 0x00C64630
    long SetHFReference(float value);              // 0x00C646E0
    long SetAllParameters(const EAXCONTEXTPROPERTIES* value);   // 0x00C67AE0
    long CommitDeferredSettings();                 // 0x00C67F60
    long SetContextProperty(unsigned long property, const void* value, unsigned long size, SEaxEvent** ppEvents);
};

long CEaxContext::SetContextProperty(unsigned long property, const void* value, unsigned long size, SEaxEvent** ppEvents)
{
    long result = -3;
    ClearEvents();
    switch (property & 0x7fffffff) {
    case 0:
        result = 0;
        break;
    case 1:
        if (value && size >= sizeof(EAXCONTEXTPROPERTIES))
            result = SetAllParameters((const EAXCONTEXTPROPERTIES*)value);
        break;
    case 2:
        if (value && size >= sizeof(SGuid))
            result = SetPrimaryFXSlotID(*(const SGuid*)value);
        break;
    case 3:
        if (value && size >= sizeof(float))
            result = SetDistanceFactor(*(const float*)value);
        break;
    case 4:
        if (value && size >= sizeof(float))
            result = SetAirAbsorptionHF(*(const float*)value);
        break;
    case 5:
        if (value && size >= sizeof(float))
            result = SetHFReference(*(const float*)value);
        break;
    case 6:
        if (!value || size < sizeof(long))
            break;
        m_lastError = -1;
    default:
        result = -3;
        break;
    }
    if (result >= 0 && !(property & 0x80000000)) {
        result = CommitDeferredSettings();
        if (result >= 0)
            *ppEvents = m_pEvents;
    }
    return result;
}