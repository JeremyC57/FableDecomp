// CPersistContext::StartSection(CPersistContextWriteSection& section, const CCharString& name) -- name the section; binary save writes the name and a zero length placeholder and remembers the stream position, text save appends "START_<name>\r\n"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BAB60, length 132 bytes (+ int3 padding).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, CDataOutputStream::WriteNullTerminatedString @ 0x00995AE0,
// CDataOutputStream::WriteSLONG @ 0x00993EB0 (catalogued "WriteCBYTE"; writes a long), the save stream's virtual
// GetPosition (slot 2, `call [eax+8]`), operator+(const char*, const CCharString&) @ 0x0099F690 and
// operator+(const CCharString&, const char*) @ 0x0099F600 (both __fastcall: hidden result ecx, left edx, right on the
// stack), CCharString::operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString @ 0x0099EAE0 (x2).
// Literals: "START_" @ 0x0129B31C, "\r\n" @ 0x012397F8.
// Layout: CPersistContext (Ego_r.pdb, 0x2c bytes; as the landed TransferObjectSaveText 0x009BAD60); Ego_r.pdb
// CPersistContextWriteSection {CCharString Name; long StartPosition}.
// Shape: the save stream re-read from +0x28 for every call; the two string temporaries live in the dead argument slots
// (section's, then name's); `ret 8` on both exits.
class CCharString { public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    const CCharString& operator+=(const CCharString& other);      // 0x0099F0A0
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

class CDataOutputStream { public:
    virtual void v00();
    virtual void v01();
    virtual long GetPosition();                             // slot 2
    void WriteNullTerminatedString(const CCharString& value);   // 0x00995AE0
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
    void StartSection(CPersistContextWriteSection& section, const CCharString& name);
};

void CPersistContext::StartSection(CPersistContextWriteSection& section, const CCharString& name)
{
    section.Name = name;
    if (Mode == 3) {
        PSaveStream->WriteNullTerminatedString(name);
        PSaveStream->WriteSLONG(0);
        section.StartPosition = PSaveStream->GetPosition();
    } else if (Mode == 1) {
        *PStringToSaveTo += "START_" + section.Name + "\r\n";
    }
}