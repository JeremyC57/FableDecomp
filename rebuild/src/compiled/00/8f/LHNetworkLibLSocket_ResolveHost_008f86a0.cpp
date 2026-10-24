// LHNetworkLib::LSocket::ResolveHost(const string&) -- dotted address through inet_addr, else the first address gethostbyname returns
// Leaf: ?ResolveHost@LSocket@LHNetworkLib@@SIK
// Retail 0x008F86A0, 0x47 bytes, static __fastcall (host in ecx), plain `ret`, returns the IPv4 address
// (INADDR_NONE when unresolved). Manifest label "Socket_ResolveHostname" (_global) is descriptive. PDB name set:
// `?ResolveHost@LSocket@LHNetworkLib@@SIKABV?$basic_string@...@@@Z` (static, __fastcall, unsigned long, const string&);
// its only caller is the other overload ResolveHost(LAddress&, unsigned long&) 0x008F86F0.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (MSVCR71 thunks), inet_addr (WSOCK32 #10, IAT 0x0144054C),
// gethostbyname (WSOCK32 #52, IAT 0x01440550).
// Shape: a one-byte scratch block is malloc'ed and terminated before the lookup and freed WITHOUT a null test after
// it -- not an STLport string (its deallocate tests the pointer, as the "ANY" temporary in 0x008F86F0 does): plain
// malloc/free reproduce it. The host's c_str() is the string's _M_start, read once per call (`mov eax,[esi]`, then
// `mov esi,[esi]`). `*(u_long*)entry->h_addr_list[0]` is the classic h_addr read.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14

struct hostent {                                // winsock hostent
    char*  h_name;                              // +0x00
    char** h_aliases;                           // +0x04
    short  h_addrtype;                          // +0x08
    short  h_length;                            // +0x0a
    char** h_addr_list;                         // +0x0c
};
extern "C" __declspec(dllimport) unsigned long __stdcall inet_addr(const char* cp);      // WSOCK32 #10
extern "C" __declspec(dllimport) hostent* __stdcall gethostbyname(const char* name);     // WSOCK32 #52

namespace _STL {
template <class _CharT> class char_traits { };
template <class _Tp> class allocator { };
template <class _CharT, class _Traits, class _Alloc>
class basic_string {                            // STLport basic_string, 12 bytes
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
    const _CharT* c_str() const { return _M_start; }
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;
}

namespace LHNetworkLib {

class LSocket {
public:
    static unsigned long __fastcall ResolveHost(const _STL::string& host);
};

unsigned long __fastcall LSocket::ResolveHost(const _STL::string& host)
{
    char* scratch = (char*)malloc(1);
    scratch[0] = 0;
    unsigned long ip = inet_addr(host.c_str());
    if (ip == (unsigned long)-1) {
        hostent* entry = gethostbyname(host.c_str());
        if (entry)
            ip = *(unsigned long*)entry->h_addr_list[0];
    }
    free(scratch);
    return ip;
}

}