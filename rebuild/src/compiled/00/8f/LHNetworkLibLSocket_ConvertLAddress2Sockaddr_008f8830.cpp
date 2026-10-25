// LHNetworkLib::LSocket::ConvertLAddress2Sockaddr(const LAddress&, sockaddr_in&) -- AF_INET sockaddr from an LAddress: resolve the IP in place, then the network-order port
// Leaf: ?ConvertLAddress2Sockaddr@LSocket@LHNetworkLib@@
// Retail 0x008F8830, 0x3E bytes, static __fastcall (address in ecx, sockaddr in edx), plain `ret`, bool. PDB name set:
// `?ConvertLAddress2Sockaddr@LSocket@LHNetworkLib@@CI_NABULAddress@2@AAUsockaddr_in@@@Z`; the manifest name agrees.
// Callees: LSocket::ResolveHost(LAddress&, unsigned long&) @ 0x008F86F0 (address still in ecx, &sin_addr in edx; its bool
// result is ignored), htons (WSOCK32 #9, IAT 0x01440544).
// Layout: retail LAddress (0x1C bytes): ServerPort +0x10; winsock sockaddr_in {family +0, port +2, addr +4, zero +8}.
// Shape: the port is cleared before the family is set (source order); the failure test re-reads sin_addr through
// its address register (`cmp dword ptr [edi],-1`), i.e. the address written by ResolveHost is compared with
// INADDR_NONE rather than the call's result.
extern "C" __declspec(dllimport) unsigned short __stdcall htons(unsigned short hostshort);   // WSOCK32 #9

struct in_addr {
    unsigned long S_addr;
};

struct sockaddr_in {                            // winsock sockaddr_in, 16 bytes
    short          sin_family;                  // +0x00
    unsigned short sin_port;                    // +0x02
    in_addr        sin_addr;                    // +0x04
    char           sin_zero[8];                 // +0x08
};

namespace std {
template <class _CharT> class char_traits { };
template <class _Tp> class allocator { };
template <class _CharT, class _Traits, class _Alloc>
class basic_string {                            // STLport basic_string, 12 bytes
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;
}

namespace LHNetworkLib {

struct LAddress {                               // Ego_r.pdb names; retail 0x1C bytes
    std::string   ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0C
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14
    long           TransportFlags;              // +0x18
};

class LSocket {
public:
    static bool __fastcall ResolveHost(LAddress& address, unsigned long& ip);          // 0x008F86F0
    static bool __fastcall ConvertLAddress2Sockaddr(const LAddress& address, sockaddr_in& sa);
};

bool __fastcall LSocket::ConvertLAddress2Sockaddr(const LAddress& address, sockaddr_in& sa)
{
    sa.sin_port = 0;
    sa.sin_family = 2;                          // AF_INET
    ResolveHost(const_cast<LAddress&>(address), sa.sin_addr.S_addr);
    if (sa.sin_addr.S_addr == (unsigned long)-1)
        return false;
    sa.sin_port = htons(address.ServerPort);
    return true;
}

}