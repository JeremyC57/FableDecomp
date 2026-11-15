// CSalKsPin::SetConnectionState(KSSTATE) -- send the kernel-streaming connection state property.
// Leaf: ?SetConnectionState@CSalKsPin@@QAE_NW4KSSTATE@@@Z
// Retail: 0x00C5E0C0,22 bytes,thiscall; the assigned107-byte span includes two later functions.
// Flags: none
#define FABLE_RETAIL_CSAL_KS_PIN_PASS22_PROPERTIES
#include "engine/CSalOutputKS.h"
bool CSalKsPin::SetConnectionState(KSSTATE state)
{
#ifdef _MSC_VER
    return SetProperty(__uuidof(KSPROPSETID_Connection), KSPROPERTY_CONNECTION_STATE, &state, sizeof(state));
#else
    // Public KS GUID value, equivalent to the compiler's UUID metadata on other compilers.
    const GUID connection = {0x1d58c920,0xac9b,0x11cf,{0xa5,0xd6,0x28,0xdb,0x04,0xc1,0x00,0x00}};
    return SetProperty(connection, KSPROPERTY_CONNECTION_STATE, &state, sizeof(state));
#endif
}