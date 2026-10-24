// LHNetworkLib::LHHttp2::Close() -- closes and deletes the host socket, clears the pointer, resets the connection state; returns LH_RETURN 0
// Leaf: Close
// Retail: Fable.exe (TLC, VC7.1) @ 0x00840070, 55 bytes (+ int3 padding). Ego_r.pdb ?Close@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@XZ.
// Callees: LSocket::Close @ 0x008F8C70, LSocket::~LSocket @ 0x008F9780 (landed; manifest "CWound::Kill" is propagated),
// operator delete @ 0x00BFE9BC, LHHttp2::Reset @ 0x0083FF80 (manifest "LHHttp2_Clear"; it deletes Host and empties the
// strings and headers, the PDB's Reset).
// Layout: Host is +0 in both the PDB and retail; the rest of the retail model as GetDocumentSize 0x0083EA70.
// Shape: `delete Host` re-reads the member after the Close call (null test + destructor + operator delete), the clear sits
// inside the outer if; the edi push is shrink-wrapped into that block.
struct LSocket {                                   // global, as the landed ~LSocket 0x008F9780 binds it
    bool Close();                                  // 0x008F8C70
    ~LSocket();                                    // 0x008F9780
};

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail (global, the landed 0x00841E90 name)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
};

namespace LHNetworkLib {


struct LHHttpDocumentParts {                       // Ego_r.pdb, 12 bytes (destructor 0x0083EA10)
    char*         Buffer;                          // +0x00
    unsigned long BufferLength;                    // +0x04
    unsigned long CurrBufPos;                      // +0x08
};

struct LDocumentPartVector {                       // STLport vector<LHHttpDocumentParts*>, 12 bytes in retail
    LHHttpDocumentParts** _M_start;
    LHHttpDocumentParts** _M_finish;
    LHHttpDocumentParts** _M_end_of_storage;
    LHHttpDocumentParts** begin() { return _M_start; }
    LHHttpDocumentParts** end() { return _M_finish; }
};

struct LHHttp2 {                                   // retail offsets (Ego_r.pdb names; its 0x1C-byte strings shift every field)
    LSocket*            Host;                      // +0x000
    CRetailString       Hostname;                  // +0x004
    unsigned short      Port;                      // +0x010
    CRetailString       PreparedRequest;           // +0x014
    unsigned long       RequestBytesSent;          // +0x020
    unsigned long       RequestLength;             // +0x024
    unsigned char       _pad028[0x48];             // +0x028 MyHeaders, ReceivedHeaders, TimeOutValue .. CurrForwarding
    char                CurrentHeaderPart[2048];   // +0x070
    unsigned long       CurrentHeaderPos;          // +0x870
    CRetailString       ProxyHost;                 // +0x874
    unsigned long       ProxyPort;                 // +0x880
    LDocumentPartVector DocumentParts;             // +0x884
    unsigned long       DocumentEncoding;          // +0x890
    unsigned long       DocumentEstimatedLength;   // +0x894
    unsigned long       DocumentLength;            // +0x898
    void Reset();                                  // 0x0083FF80
    long Close();
};

long LHHttp2::Close()
{
    if (Host) {
        Host->Close();
        delete Host;
        Host = 0;
    }
    Reset();
    return 0;
}

}