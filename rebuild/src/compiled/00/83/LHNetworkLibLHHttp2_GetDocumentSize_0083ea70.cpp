// LHNetworkLib::LHHttp2::GetDocumentSize() -- sums the BufferLength of every non-null received document part
// Leaf: GetDocumentSize
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EA70, 37 bytes (+ int3 padding). No callees. Ego_r.pdb ?GetDocumentSize@LHHttp2@LHNetworkLib@@QAEKXZ.
// Layout: engine/LHHttp2.h is the Ego_r.pdb layout (Dinkumware 0x1C-byte strings, 16-byte vector) and does not fit retail,
// whose STLport strings/vectors are 12 bytes: DocumentParts (vector<LHHttpDocumentParts*>) sits at +0x884 (the landed
// ResetDocumentRecv 0x0083EB90 has CurrentHeaderPart +0x70, DocumentLength +0x898). Local model with the PDB names.
// Shape: begin/end read once (no stores in the loop), the null-tested part adds its length.
struct LSocket;                                    // global, as the landed LSocket rows bind it

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

class LHHttp2 { public:                                   // retail offsets (Ego_r.pdb names; its 0x1C-byte strings shift every field)
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
    unsigned long GetDocumentSize();
};

unsigned long LHHttp2::GetDocumentSize()
{
    unsigned long size = 0;
    for (LHHttpDocumentParts** it = DocumentParts.begin(); it != DocumentParts.end(); ++it) {
        if (*it)
            size += (*it)->BufferLength;
    }
    return size;
}

}