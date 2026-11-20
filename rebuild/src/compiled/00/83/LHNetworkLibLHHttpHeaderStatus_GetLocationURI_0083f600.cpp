// LHNetworkLib::LHHttpHeaderStatus::GetLocationURI() -- parse the Location header data, then return a copy of hsURI (+0x38)
// Leaf: ?GetLocationURI@LHHttpHeaderStatus@LHNetworkLib@@QAE?AV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083F600, length 48 bytes.
// Identity: Ego_r.pdb ?GetLocationURI@LHHttpHeaderStatus@LHNetworkLib@@QAE?AV?$basic_string@...@std@@XZ (0x0084E7C0), in the Ego_r order
//   ParseLocationData 0x0084E5C0 / GetLocationHost 0x0084E780 / GetLocationURI 0x0084E7C0 that retail keeps (0x0083F420 /
//   0x0083F5C0 / 0x0083F600, the same 0x40 stride); the body calls ParseLocationData 0x0083F420 and copies the string at +0x38
//   (Ego_r hsURI). The earlier CHttpUrl_0083f420 / CNetString_Retail stand-ins and the manifest's "CColourConverter" label were
//   descriptive / propagated.
// Callees: LHHttpHeaderStatus::ParseLocationData 0x0083F420, the private STLport _M_range_initialize<char*> 0x00567D00.
// Shape: `ParseLocationData(); return hsURI;` -- the STLport copy constructor builds the result straight into the hidden return
//   slot: zero the three words, then the out-of-line range initialise reading hsURI._M_start / _M_finish (the two member loads
//   after the zero stores; `this` live in edi); the empty iterator tag is homed in the dead hidden-pointer slot (`lea eax,[esp+0Ch]`).
//   On the shared engine/LHHttpHeaderStatus.h retail struct (NF116 opt-in FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION).
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION
#include "engine/LHHttpHeaderStatus.h"

namespace LHNetworkLib {

std::string LHHttpHeaderStatus::GetLocationURI()
{
    ParseLocationData();
    return hsURI;
}

}
