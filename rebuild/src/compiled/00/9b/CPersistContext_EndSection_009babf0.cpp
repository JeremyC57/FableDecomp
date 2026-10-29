// CPersistContext::EndSection(CPersistContextWriteSection& section) -- close a section; binary save measures the bytes written since StartSection, seeks back to patch the length placeholder and returns to the end, text save appends "END_<name>\r\n\r\n"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BABF0, length 142 bytes.
// Callees: the save stream's virtual GetPosition (slot 2, `call [eax+8]`) and Seek (slot 1, `call [edx+4]` / `[eax+4]`),
// CDataOutputStream::WriteSLONG @ 0x00993EB0 (catalogued "WriteCBYTE"; writes a long), operator+(const char*, const CCharString&)
// @ 0x0099F690 and operator+(const CCharString&, const char*) @ 0x0099F600 (both __fastcall: hidden result ecx, left edx,
// right on the stack), CCharString::operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString @ 0x0099EAE0 (x2).
// Literals: "END_" @ 0x0129B324, "\r\n\r\n" @ 0x0129AA38.
// Layout: CPersistContext (Ego_r.pdb, 0x2c bytes; as the landed TransferObjectSaveText 0x009BAD60); Ego_r.pdb
// CPersistContextWriteSection {CCharString Name; long StartPosition}.
// Shape: the end position kept in ebx, the length (end - start) in edi computed before the seek to start-4; the save
// stream re-read from +0x28 for every call; the text temporaries live in the `push ecx` frame word and the dead section
// argument slot; `ret 4` on both exits.
class CCharString { public:
    void* PStringData;
    const CCharString& operator+=(const CCharString& other);      // 0x0099F0A0
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

class CDataOutputStream { public:
    virtual void v00();
    virtual void Seek(long position);                       // slot 1
    virtual long GetPosition();                             // slot 2
public:
    void WriteSLONG(long);   // 0x00993EB0
};
class CMemoryDataInputStream;

class CPersistContextWriteSection { public:
    CCharString Name;                                       // +0x00
    long        StartPosition;                              // +0x04
};

class CPersistContext { public:
    void*                   PDefinitionManager;             // +0x00
    void*                   PSymbolMap;                     // +0x04
    void*                   PParser;                        // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*);   // +0x0c
    CCharString*            PStringToClean;                 // +0x10
    bool                    UseDefaults;                    // +0x14
    bool                    UseCRCCheck;                    // +0x15
    bool                    SingleTransfer;                 // +0x16
    bool                    Finished;                       // +0x17
    long                    Mode;                           // +0x18 (1 text save, 3 binary save)
    bool                    Binary;                         // +0x1c
    CCharString*            PStringToSaveTo;                // +0x20
    CMemoryDataInputStream* PLoadStream;                    // +0x24
    CDataOutputStream*      PSaveStream;                    // +0x28
    void EndSection(CPersistContextWriteSection& section);
};

void CPersistContext::EndSection(CPersistContextWriteSection& section)
{
    if (Mode == 3) {
        long end = PSaveStream->GetPosition();
        long length = end - section.StartPosition;
        PSaveStream->Seek(section.StartPosition - 4);
        PSaveStream->WriteSLONG(length);
        PSaveStream->Seek(end);
    } else if (Mode == 1) {
        *PStringToSaveTo += "END_" + section.Name + "\r\n\r\n";
    }
}