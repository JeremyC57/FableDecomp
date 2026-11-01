// CEaxContext::SetContextProperty(unsigned long property, const void* value, unsigned long size, SEaxEvent** ppEvents) -- EAX 4.0 context property dispatcher
// Leaf: ?SetContextProperty@CEaxContext@@
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
