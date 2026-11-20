// LHNetworkLib::LHHttpHeaderStatus::Reset() -- forgets a parsed response header: server code -1, no header lines, empty location / host / URI strings, port 0
// Leaf: ?Reset@LHHttpHeaderStatus@LHNetworkLib@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FB60, 88 bytes, __fastcall, plain `ret`.
// Identity: Ego_r.pdb ?Reset@LHHttpHeaderStatus@LHNetworkLib@@QAEXXZ; TLC keeps it next to LHHttpHeaders::Reset (landed
//   0x0083FBC0). The manifest label "CPatchTesselationEdgeStrip_Reset" is propagated and wrong.
// Callees: vector<basic_string<char> >::erase(iterator, iterator) 0x00841FC0 and basic_string<char>::assign(const char*,
//   const char*) 0x005693B0 with the empty literal 0x0122D70E as both bounds (the landed 0x0083FBC0 models).
// Layout: retail LHHttpHeaderStatus (Ego_r.pdb names, 12-byte STLport containers): hsServerCode +0, hsDocumentHeader +4
//   (LHHttpHeaders: HdrDocumentHeader vector +4, HeaderString +0x10), hsLocation +0x1c, hsDocumentSize +0x28, hsHost +0x2c,
//   hsURI +0x38, hsPort +0x44.
// Shape: only the header-line vector is cleared (the header string is left alone), hsDocumentSize is not touched.
// NF123 (2026-11-20, q4193): the local CRetailString / CRetailStringVector / LHHttpHeaders / LHHttpHeaderStatus models replaced by
//   the shared engine/LHHttpHeaderStatus.h (LOCATION branch, STLport std::string / std::vector<std::string>) with the NF123 RESET
//   opt-in; the erase call now binds the STLport spelling of 0x00841FC0 (was ?erase@CRetailStringVector@@...).
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION
#define FABLE_RETAIL_LHHTTPHEADERSTATUS_RESET
#include "engine/LHHttpHeaderStatus.h"

namespace LHNetworkLib {

void LHHttpHeaderStatus::Reset()
{
    hsServerCode = (LH_HTTP_SERVER_CODES)-1;
    hsDocumentHeader.HdrDocumentHeader.clear();
    hsLocation = "";
    hsHost = "";
    hsURI = "";
    hsPort = 0;
}
}
