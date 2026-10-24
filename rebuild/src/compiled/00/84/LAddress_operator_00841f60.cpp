// LAddress::operator=(const LAddress& other) -- memberwise assignment: the server name string (self-checked range assign), IP, port, protocol and flags; returns *this
// Leaf: ??4LAddress@@QAEAAU0@ABU0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00841F60, 64 bytes, `ret 4`. Ego_r.pdb ??4LAddress@LHNetworkLib@@QAEAAU01@ABU01@@Z
// (compiler-generated; written out here so the TU emits it). Called by LSocketServer::RequestListener 0x0090B0B9.
// Callee: STLport basic_string<char>::_M_assign_dispatch(const char* first, const char* last, const __false_type&) @ 0x00569140
// (manifest "std::string_InsertRange"): the inline operator=(const basic_string&) skips self-assignment and passes an
// empty tag temporary homed in the dead argument slot (`lea eax,[esp+0Ch]`).
// Layout: retail LAddress is 0x1C bytes (PDB 44 with a Dinkumware string), as the landed LSocket::Attach 0x008F8C00 and
// the LAddress constructor 0x00841E90 show. Global namespace, as the landed LSocket rows.
struct __false_type {                              // STLport empty tag; a user constructor keeps the temporary store-free
    __false_type() { }
};

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail (global, the landed 0x00841E90 name)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString& _M_assign_dispatch(const char* first, const char* last, const __false_type&);   // 0x00569140
    CRetailString& operator=(const CRetailString& other)
    {
        if (&other != this)
            _M_assign_dispatch(other._M_start, other._M_finish, __false_type());
        return *this;
    }
};

struct LAddress {                                  // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    CRetailString  ServerName;                     // +0x00
    unsigned long  ServerIP;                       // +0x0C
    unsigned short ServerPort;                     // +0x10
    long           TransportProtocol;              // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;                 // +0x18 LSOCKET_FLAGS
    LAddress& operator=(const LAddress& other);
};

LAddress& LAddress::operator=(const LAddress& other)
{
    ServerName = other.ServerName;
    ServerIP = other.ServerIP;
    ServerPort = other.ServerPort;
    TransportProtocol = other.TransportProtocol;
    TransportFlags = other.TransportFlags;
    return *this;
}