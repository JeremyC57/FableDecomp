// LAddress::LAddress(const string& serverName, unsigned short serverPort) -- an address for a named server: the default state (empty name, IP / port / protocol / flags zero), then the name (self-checked string assignment) and the port
// Leaf: ??0LAddress@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00841EE0, 124 bytes, __fastcall, `ret 8`, returns this.
// Identity: Ego_r.pdb ??0LAddress@LHNetworkLib@@QAE@ABV?$basic_string@...@std@@G@Z, next to the default constructor (landed
//   0x00841E90 as "CNamedRecord_00841e90") and operator= (landed 0x00841F60); global namespace, as the landed LSocket rows.
//   The manifest label "LHHttp_InitializeRequest" is an autoname.
// Callees: malloc 0x00BFEA0E (the STLport string's 8-byte initial block), basic_string<char>::assign(const char*, const char*)
//   0x005693B0 (the "" literal 0x0122D70E as both bounds), _M_assign_dispatch(const char*, const char*, const __false_type&)
//   0x00569140 (the inline operator=(const basic_string&)).
// Layout: retail LAddress 0x1C bytes (landed 0x00841E90 / 0x00841F60): ServerName +0, ServerIP +0xc, ServerPort +0x10,
//   TransportProtocol +0x14, TransportFlags +0x18.
// Shape: _M_assign_dispatch carries a same-TU `__declspec(noinline)` STLport body that never touches its tag, so VC7.1
//   homes the empty __false_type temporary on the LIVE port argument slot (`lea edx,[esp+10h]`) like retail (a bare
//   declaration puts it on the dead name slot). Then: the landed default-constructor body (string = "", then port, IP, protocol, flags zeroed in the body), then
//   ServerName = serverName and ServerPort = serverPort (the port store duplicated into both arms of the self test).
#include <string.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct __false_type {                           // STLport empty tag; a user constructor keeps the temporary store-free
    __false_type() { }
};

struct SRetailAllocProxy {                      // STLport _STLP_alloc_proxy<char*, char, allocator>: the end-of-storage pointer
    char* m_data;
    SRetailAllocProxy(char* data) : m_data(data) { }
};

struct CRetailStringBase {                      // STLport _String_base
    char* _M_start;
    char* _M_finish;
    SRetailAllocProxy _M_end_of_storage;
    CRetailStringBase(size_t n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = static_cast<char*>(malloc(n));
        _M_finish = _M_start;
        _M_end_of_storage.m_data = _M_start + n;
    }
};

struct CRetailString : CRetailStringBase {      // STLport basic_string<char>: 8-byte initial block, terminated
    CRetailString() : CRetailStringBase(8)
    {
        *_M_finish = 0;
    }
    CRetailString& Assign(const char* first, const char* last);   // 0x005693B0
    CRetailString& AppendRange(const char* first, const char* last);                               // append(first, last)
    __declspec(noinline) CRetailString& _M_assign_dispatch(const char* __f, const char* __l, const __false_type&)
    {                                                   // 0x00569140: STLport body; never touches the tag
        char* __cur = _M_start;
        while (__f != __l && __cur != _M_finish) {
            *__cur = *__f;
            ++__f;
            ++__cur;
        }
        if (__f == __l) {                               // erase(__cur, end()): move the terminator down
            if (__cur != _M_finish) {
                *__cur = 0;
                _M_finish = __cur;
            }
        }
        else
            AppendRange(__f, __l);
        return *this;
    }
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
    CRetailString& operator=(const CRetailString& other)
    {
        if (&other != this)
            _M_assign_dispatch(other._M_start, other._M_finish, __false_type());
        return *this;
    }
};

struct LAddress {                               // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    CRetailString  ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0C
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;              // +0x18 LSOCKET_FLAGS
    LAddress(const CRetailString& serverName, unsigned short serverPort);
};

LAddress::LAddress(const CRetailString& serverName, unsigned short serverPort)
    : ServerName()
{
    ServerName = "";
    ServerPort = 0;
    ServerIP = 0;
    TransportProtocol = 0;
    TransportFlags = 0;
    ServerName = serverName;
    ServerPort = serverPort;
}