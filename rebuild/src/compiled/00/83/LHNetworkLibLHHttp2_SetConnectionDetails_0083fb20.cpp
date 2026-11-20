// LHNetworkLib::LHHttp2::SetConnectionDetails(const string&, unsigned short) -- stores the server host name (self-checked string assignment) and port, then returns the result of re-opening the connection details step
// Leaf: ?SetConnectionDetails@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@ABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@G@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FB20, 51 bytes, __fastcall, `ret 8`, LH_RETURN in eax.
// Identity: Ego_r.pdb ?SetConnectionDetails@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@ABV?$basic_string@...@std@@G@Z. The
//   manifest label "LHNetwork_BuildLoginRequestString" is an autoname.
// Callees: STLport basic_string<char>::_M_assign_dispatch 0x00569140 (inline operator=, as the landed LAddress::operator=
//   0x00841F60); LHHttp2 member 0x0083F9B0 (`this` only, LH_RETURN result: the Ego_r SetConnectionDetails tail is Open()).
// Layout: retail LHHttp2 (landed ResetRequest 0x0083F980): Hostname +0x04, Port +0x10 (unsigned short).
// Shape: the shared engine/LHHttp2.h class (FABLE_RETAIL_LHHTTP2_SCOREBOARD declares the member) on stlport/fable_string.h; the
//   inline operator=(const string&) calls the private _M_assign_dispatch<char*> 0x00569140, whose STLport body is in the TU
//   (stlport/fable_string_bodies.h, out of line, never touches its tag), so VC7.1 homes the empty __false_type tag in the LIVE port
//   argument slot (`lea edx,[esp+0Ch]`, the port re-read after the call) as retail does.
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#define FABLE_RETAIL_LHHTTP2_OPEN
#include "engine/LHHttp2.h"
#include "stlport/fable_string_bodies.h"

namespace LHNetworkLib {

LH_RETURN LHHttp2::SetConnectionDetails(const std::string& host, unsigned short port)
{
    Hostname = host;
    Port = port;
    return Open();
}

}
