// LAddress::LAddress(const string& serverName, unsigned short serverPort) -- an address for a named server: the default state (empty name, IP / port / protocol / flags zero), then the name (self-checked string assignment) and the port
// Leaf: ??0LAddress@LHNetworkLib@@QAE@ABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@G@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00841EE0, 124 bytes, __fastcall, `ret 8`, returns this.
// Identity: Ego_r.pdb ??0LAddress@LHNetworkLib@@QAE@ABV?$basic_string@...@std@@G@Z, next to the default constructor (landed
//   0x00841E90 as "CNamedRecord_00841e90") and operator= (landed 0x00841F60). The namespaced spelling is what the landed caller 0x0083F9B0 binds.
//   The manifest label "LHHttp_InitializeRequest" is an autoname.
// Callees: malloc 0x00BFEA0E (the STLport string's 8-byte initial block), basic_string<char>::assign(const char*, const char*)
//   0x005693B0 (the "" literal 0x0122D70E as both bounds), _M_assign_dispatch(const char*, const char*, const __false_type&)
//   0x00569140 (the inline operator=(const basic_string&)).
// Layout: retail LAddress 0x1C bytes (landed 0x00841E90 / 0x00841F60): ServerName +0, ServerIP +0xc, ServerPort +0x10,
//   TransportProtocol +0x14, TransportFlags +0x18.
// Shape: the shared engine/LSocket.h LAddress (opt-ins FABLE_RETAIL_LSOCKET_LADDRESS for ClearData, NF116's
//   FABLE_RETAIL_LSOCKET_LADDRESS_NAMEPORT_CTOR for this constructor) on stlport/fable_string.h: the string's default constructor
//   (8-byte block, terminator), ClearData() inline (ServerName = "" through the out-of-line assign 0x005693B0, the scalars zeroed),
//   then the inline operator=(const string&) to the private _M_assign_dispatch<char*> 0x00569140, whose STLport body is in the TU
//   (stlport/fable_string_bodies.h, out of line, never touches its tag), and the port.

#define FABLE_RETAIL_LSOCKET_LADDRESS
#define FABLE_RETAIL_LSOCKET_LADDRESS_NAMEPORT_CTOR
#include "engine/LSocket.h"
#include "stlport/fable_string_bodies.h"

namespace LHNetworkLib {

LAddress::LAddress(const std::string& serverName, unsigned short serverPort)
{
    ClearData();
    ServerName = serverName;
    ServerPort = serverPort;
}

}
