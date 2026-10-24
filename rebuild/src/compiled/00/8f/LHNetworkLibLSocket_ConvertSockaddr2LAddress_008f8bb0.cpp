// LHNetworkLib::LSocket::ConvertSockaddr2LAddress(const sockaddr_in&, LAddress&) -- LAddress from a sockaddr: dotted name, raw IP and host-order port
// Leaf: ?ConvertSockaddr2LAddress@LSocket@LHNetworkLib@@
// Retail 0x008F8BB0, 0x47 bytes, static __fastcall (sockaddr in ecx, address in edx), plain `ret`, always true. PDB name
// set: `?ConvertSockaddr2LAddress@LSocket@LHNetworkLib@@CI_NABUsockaddr_in@@AAULAddress@2@@Z`; the manifest name agrees.
// Callees: inet_ntoa (WSOCK32 #11, IAT 0x01440528), basic_string::assign(const char*, const char*) @ 0x005693B0
// (landed; STLport operator=(const char*) inlined with the intrinsic strlen), ntohs (WSOCK32 #15, IAT 0x01440538).
// Layout: retail LAddress (0x1C bytes): ServerName +0x00, ServerIP +0x0C, ServerPort +0x10.
// Shape: STLport `operator=(const _CharT* __s) { return assign(__s, __s + _Traits::length(__s)); }` -- the length is the
// inline `mov dl,[ecx] ; inc ecx ; test dl,dl` scan; in_addr is passed by value (one dword push).
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

struct in_addr {
    unsigned long S_addr;
};

struct sockaddr_in {                            // winsock sockaddr_in, 16 bytes
    short          sin_family;                  // +0x00
    unsigned short sin_port;                    // +0x02
    in_addr        sin_addr;                    // +0x04
    char           sin_zero[8];                 // +0x08
};

extern "C" __declspec(dllimport) char* __stdcall inet_ntoa(in_addr in);                     // WSOCK32 #11
extern "C" __declspec(dllimport) unsigned short __stdcall ntohs(unsigned short netshort);   // WSOCK32 #15

namespace _STL {
template <class _CharT> class char_traits {
public:
    static unsigned int length(const _CharT* __s) { return strlen(__s); }
};
template <class _Tp> class allocator { };
template <class _CharT, class _Traits, class _Alloc>
class basic_string {                            // STLport basic_string, 12 bytes
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
    basic_string& assign(const _CharT* __f, const _CharT* __l);                  // 0x005693B0
    basic_string& operator=(const _CharT* __s) { return assign(__s, __s + _Traits::length(__s)); }
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;
}

namespace LHNetworkLib {

struct LAddress {                               // Ego_r.pdb names; retail 0x1C bytes
    _STL::string   ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0C
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14
    long           TransportFlags;              // +0x18
};

class LSocket {
public:
    static bool __fastcall ConvertSockaddr2LAddress(const sockaddr_in& sa, LAddress& address);
};

bool __fastcall LSocket::ConvertSockaddr2LAddress(const sockaddr_in& sa, LAddress& address)
{
    address.ServerName = inet_ntoa(sa.sin_addr);
    address.ServerIP = sa.sin_addr.S_addr;
    address.ServerPort = ntohs(sa.sin_port);
    return true;
}

}