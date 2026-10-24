// LHNetworkLib::LHHttp2::ResetDocumentParts() -- deletes every received document part and empties the DocumentParts vector
// Leaf: ResetDocumentParts
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EB00, 130 bytes (+ int3 padding). Ego_r.pdb ?ResetDocumentParts@LHHttp2@LHNetworkLib@@QAEXXZ.
// Callees: operator delete[] @ 0x00BFEB1C (the part's buffer, inside the inlined ~LHHttpDocumentParts 0x0083EA10),
// operator delete @ 0x00BFE9BC (the part), memmove @ 0x00BFEAE6 (STLport erase's __copy_trivial).
// Layout: retail LHHttp2 offsets (engine/LHHttp2.h is the PDB layout and does not fit retail; see GetDocumentSize 0x0083EA70).
// Shape: `delete *it` = null test + inline destructor + operator delete; then STLport clear() = erase(begin(), end()):
// `_M_finish = __copy_trivial(end, _M_finish, begin)` -- the retail `cmp eax,eax ; jne` on the twice-read finish pointer
// (same form as the landed CTCLight::OnAppearanceClear 0x00641210).
void __cdecl operator delete[](void* block);       // 0x00BFEB1C
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
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
    ~LHHttpDocumentParts()
    {
        if (Buffer) {
            delete[] Buffer;
            Buffer = 0;
        }
        BufferLength = 0;
    }
};

struct LDocumentPartVector {                       // STLport vector<LHHttpDocumentParts*>, 12 bytes in retail
    LHHttpDocumentParts** _M_start;
    LHHttpDocumentParts** _M_finish;
    LHHttpDocumentParts** _M_end_of_storage;
    LHHttpDocumentParts** begin() { return _M_start; }
    LHHttpDocumentParts** end() { return _M_finish; }
    void clear()
    {
        LHHttpDocumentParts** i = (LHHttpDocumentParts**)CopyTrivial(end(), _M_finish, begin());
        _M_finish = i;
    }
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
    void ResetDocumentParts();
};

void LHHttp2::ResetDocumentParts()
{
    for (LHHttpDocumentParts** it = DocumentParts.begin(); it != DocumentParts.end(); ++it)
        delete *it;
    DocumentParts.clear();
}

}