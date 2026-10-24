// LHNetworkLib::LHHttp2::SetProxySettings(const string&, unsigned short) -- stores the proxy host name (self-checked string assignment) and the proxy port
// Leaf: ?SetProxySettings@LHHttp2@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083F870, 64 bytes, __fastcall, `ret 8`.
// Identity: Ego_r.pdb ?SetProxySettings@LHHttp2@LHNetworkLib@@QAEXABV?$basic_string@...@std@@G@Z (QAEX: void); TLC keeps the
//   Ego_r order SetConnectionDetails / SetProxySettings as 0x0083FB20 / 0x0083F870 neighbours. The manifest label
//   "LHNetworkLib_InsertStringRange" is an autoname.
// Callee: STLport basic_string<char>::_M_assign_dispatch(const char*, const char*, const __false_type&) 0x00569140 (the
//   inline operator=(const basic_string&), as the landed LAddress::operator= 0x00841F60).
// Layout: retail LHHttp2 (landed ResetRequest 0x0083F980): ProxyHost +0x874, ProxyPort +0x880 (unsigned long).
// Shape: _M_assign_dispatch carries a same-TU `__declspec(noinline)` STLport body that never touches its tag, so VC7.1
//   homes the empty __false_type tag on the LIVE port argument slot like retail (a bare declaration picks the dead
//   host slot). Retail homes the empty __false_type tag in the LIVE port argument slot (`lea edx,[esp+0Ch]`, the port re-read
//   from it after the call) and duplicates the port store into both arms of the self-assignment test.
#include <string.h>
struct LSocket;                                    // global, as the landed LSocket rows bind it

struct __false_type {                              // STLport empty tag; a user constructor keeps the temporary store-free
    __false_type() { }
};

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail (global, the landed 0x00841E90 name)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString& AppendRange(const char* first, const char* last);                               // append(first, last)
    __declspec(noinline) CRetailString& _M_assign_dispatch(const char* __f, const char* __l, const __false_type&)
    {                                              // 0x00569140: STLport body; never touches the tag
        char* __cur = _M_start;
        while (__f != __l && __cur != _M_finish) {
            *__cur = *__f;
            ++__f;
            ++__cur;
        }
        if (__f == __l) {                          // erase(__cur, end()): move the terminator down
            if (__cur != _M_finish) {
                *__cur = 0;
                _M_finish = __cur;
            }
        }
        else
            AppendRange(__f, __l);
        return *this;
    }
    CRetailString& Assign(const char* first, const char* last);                                    // 0x005693B0
    CRetailString& operator=(const CRetailString& other)
    {
        if (&other != this)
            _M_assign_dispatch(other._M_start, other._M_finish, __false_type());
        return *this;
    }
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

namespace LHNetworkLib {
struct LHHttp2 {                                   // retail offsets (Ego_r.pdb names)
    LSocket*       Host;                           // +0x000
    CRetailString  Hostname;                       // +0x004
    unsigned short Port;                           // +0x010
    unsigned char  _pad_0x12[0x874 - 0x12];
    CRetailString  ProxyHost;                      // +0x874
    unsigned long  ProxyPort;                      // +0x880
    void SetProxySettings(const CRetailString& host, unsigned short port);
};

void LHHttp2::SetProxySettings(const CRetailString& host, unsigned short port)
{
    ProxyHost = host;
    ProxyPort = port;
}
}