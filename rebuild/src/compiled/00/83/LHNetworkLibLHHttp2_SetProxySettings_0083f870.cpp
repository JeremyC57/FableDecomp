// LHNetworkLib::LHHttp2::SetProxySettings(const string&, unsigned short) -- stores the proxy host name (self-checked string assignment) and the proxy port
// Leaf: ?SetProxySettings@LHHttp2@LHNetworkLib@@QAEXABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@G@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083F870, 64 bytes, __fastcall, `ret 8`.
// Identity: Ego_r.pdb ?SetProxySettings@LHHttp2@LHNetworkLib@@QAEXABV?$basic_string@...@std@@G@Z (QAEX: void); TLC keeps the
//   Ego_r order SetConnectionDetails / SetProxySettings as 0x0083FB20 / 0x0083F870 neighbours. The manifest label
//   "LHNetworkLib_InsertStringRange" is an autoname.
// Callee: STLport basic_string<char>::_M_assign_dispatch(const char*, const char*, const __false_type&) 0x00569140 (the
//   inline operator=(const basic_string&), as the landed LAddress::operator= 0x00841F60).
// Layout: retail LHHttp2 (landed ResetRequest 0x0083F980): ProxyHost +0x874, ProxyPort +0x880 (unsigned long).
// Shape: the shared engine/LHHttp2.h class (FABLE_RETAIL_LHHTTP2_SCOREBOARD declares the member) on stlport/fable_string.h; the
//   inline operator=(const string&) calls the private _M_assign_dispatch<char*> 0x00569140, whose STLport body is in the TU
//   (stlport/fable_string_bodies.h, out of line, never touches its tag), so VC7.1 homes the empty __false_type tag in the LIVE port
//   argument slot (`lea edx,[esp+0Ch]`, the port re-read after the call) as retail does.
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#include "engine/LHHttp2.h"
#include "stlport/fable_string_bodies.h"

namespace LHNetworkLib {

void LHHttp2::SetProxySettings(const std::string& host, unsigned short port)
{
    ProxyHost = host;
    ProxyPort = port;
}

}
