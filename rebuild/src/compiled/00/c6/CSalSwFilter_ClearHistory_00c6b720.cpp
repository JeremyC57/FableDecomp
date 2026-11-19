// CSalSwFilter::ClearHistory() -- clear the filter history: zero both state words (+0x18 / +0x1c), snap the current parameters to the new ones (+0x0c -> +0x00) and zero the fade count (+0x20)
// Leaf: ?ClearHistory@CSalSwFilter@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6B720, length 37 bytes; 13 call sites in the 0x00C5xxxx voice module.
// Name (NF99, q11412; was the descriptive SBodyRecord_00c55030::Reset over a local three-vector record, earlier also bound as
//   SRampedVector::Reset / CVoiceFilter::Reset): GameCODA 2.10 CODAApi.lib SalSwFilter.obj public ?ClearHistory@CSalSwFilter@@QAEXXZ
//   (37 bytes, byte-identical to retail) + the SDK SalSw3DVoice.obj CalcParams relocations 0x255 / 0x280, the retail calls the
//   landed 0x00C54C10 already binds under this spelling (engine/CSalSwFilter.h, opt-in FABLE_RETAIL_CSALSW3D_CALCPARAMS).
// Members: the header's CodeView names (PARAMS m_Params +0x00, m_NewParams +0x0c, m_fZ0 +0x18, m_fZ1 +0x1c, m_nFadeSamples +0x20).
// Callees: none.
// Shape: the 12-byte PARAMS copy through a `lea edx,[ecx+0Ch]` source base and a `mov esi,ecx` destination with edi as the
// carrier (the implicit assignment of a POD struct), then one `xor eax,eax` feeding three ascending zero stores.
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#include "engine/CSalSwFilter.h"

void CSalSwFilter::ClearHistory()
{
    m_fZ0 = 0.0f;
    m_fZ1 = 0.0f;
    m_Params = m_NewParams;
    m_nFadeSamples = 0;
}
