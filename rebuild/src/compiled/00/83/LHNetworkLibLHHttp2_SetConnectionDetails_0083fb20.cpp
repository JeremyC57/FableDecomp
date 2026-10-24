// LHNetworkLib::LHHttp2::SetConnectionDetails(const string&, unsigned short) -- stores the server host name (self-checked string assignment) and port, then returns the result of re-opening the connection details step
// Leaf: ?SetConnectionDetails@LHHttp2@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FB20, 51 bytes, __fastcall, `ret 8`, LH_RETURN in eax.
// Identity: Ego_r.pdb ?SetConnectionDetails@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@ABV?$basic_string@...@std@@G@Z. The
//   manifest label "LHNetwork_BuildLoginRequestString" is an autoname.
// Callees: STLport basic_string<char>::_M_assign_dispatch 0x00569140 (inline operator=, as the landed LAddress::operator=
//   0x00841F60); LHHttp2 member 0x0083F9B0 (`this` only, LH_RETURN result: the Ego_r SetConnectionDetails tail is Open()).
// Layout: retail LHHttp2 (landed ResetRequest 0x0083F980): Hostname +0x04, Port +0x10 (unsigned short).
// Shape: _M_assign_dispatch carries a same-TU `__declspec(noinline)` STLport body that never touches its tag, so VC7.1
//   homes the empty __false_type tag on the LIVE port argument slot like retail (a bare declaration picks the dead
//   host slot). Retail homes the empty __false_type tag in the LIVE port argument slot (`lea edx,[esp+0Ch]`).
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
enum LH_RETURN { };
struct LHHttp2 {                                   // retail offsets (Ego_r.pdb names)
    LSocket*       Host;                           // +0x000
    CRetailString  Hostname;                       // +0x004
    unsigned short Port;                           // +0x010
    LH_RETURN Open();                              // 0x0083F9B0
    LH_RETURN SetConnectionDetails(const CRetailString& host, unsigned short port);
};

LH_RETURN LHHttp2::SetConnectionDetails(const CRetailString& host, unsigned short port)
{
    Hostname = host;
    Port = port;
    return Open();
}
}