// LHNetworkLib::LAddress::LAddress() -- the default network address: an empty STLport ServerName string
// (default-constructed inline: the three pointers nulled by the _String_base part, then an 8-byte malloc block,
// empty and terminated) that is then assigned the empty literal in the body, followed by ServerPort, ServerIP,
// TransportProtocol and TransportFlags cleared in the body (init-list zeros would be stored before the assign call)
// Leaf: ??0LAddress@LHNetworkLib@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00841E90, length 72 bytes (+ int3 padding).
// Callees: malloc @ 0x00BFEA0E (the string's 8-byte initial block), basic_string::assign(const
// char* first, const char* last) @ 0x005693B0 (the manifest's "Vector_Insert" label is
// propagated) with the empty literal @ 0x0122D70E as both bounds (strlen("") folded to 0, so the
// same pointer is pushed twice).
// Naming: Ego_d.pdb ??0LAddress@LHNetworkLib@@QAE@XZ (LAddress is a struct: `AAULAddress@2@` in every LSocket
//   signature). Ego_r.pdb LAddress: basic_string ServerName +0, ulong ServerIP, ushort ServerPort,
//   LSOCKET_PROTOCOL TransportProtocol, LSOCKET_FLAGS TransportFlags (44 bytes with the 28-byte debug string; retail
//   string is 12 bytes, so +0x0c / +0x10 / +0x14 / +0x18 and 0x1c bytes, the shape this body clears). Callers are the
//   network code (LHHttp2 send / LHHttp connect / bank-file network send), which declare LAddress locals. FINDINGS
//   chunk 401. Was landed as the stand-in "CNamedRecord_00841e90" (manifest "CContainedBank" is propagated: the PDB
//   CContainedBank is a 20-byte POD of unsigned longs with no string). The two enum members are modelled as long
//   (their enumerators are not in the PDB dumps); the ctor's mangling does not depend on member types.
// The end-of-storage pointer is STLport's _STLP_alloc_proxy member (its own inline constructor),
// which is why the `push 8` of the allocation is scheduled between the second and third null
// store; the four scalars are assigned in the body in the order ServerPort, ServerIP, TransportProtocol, TransportFlags.
#include <string.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct SRetailAllocProxy {                      // STLport _STLP_alloc_proxy<char*, char, allocator>: the end-of-storage pointer
    char* m_data;
    SRetailAllocProxy(char* data) : m_data(data) { }
};

struct CRetailStringBase {                      // STLport _String_base: start / finish nulled, the proxy member nulled, then the block allocated
    char* m_start;
    char* m_finish;
    SRetailAllocProxy m_endOfStorage;
    CRetailStringBase(size_t n) : m_start(0), m_finish(0), m_endOfStorage(0)
    {
        m_start = static_cast<char*>(malloc(n));
        m_finish = m_start;
        m_endOfStorage.m_data = m_start + n;
    }
};

struct CRetailString : CRetailStringBase {      // STLport basic_string<char>: 8-byte initial block, terminated
    CRetailString() : CRetailStringBase(8)
    {
        *m_finish = 0;
    }
    CRetailString& Assign(const char* first, const char* last);   // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

namespace LHNetworkLib {
struct LAddress {
    CRetailString  ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0c
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;              // +0x18 LSOCKET_FLAGS
    LAddress();
};

LAddress::LAddress()
    : ServerName()
{
    ServerName = "";
    ServerPort = 0;
    ServerIP = 0;
    TransportProtocol = 0;
    TransportFlags = 0;
}
}
