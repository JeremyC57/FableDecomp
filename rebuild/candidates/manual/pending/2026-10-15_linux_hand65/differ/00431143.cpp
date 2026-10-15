// CPersistContext::TransferString(CCharString& value) -- binary transfer of a null-terminated string: CRC check, then read it from the load stream (Mode 2) or write it to the save stream (Mode 3)
// Grade: DIFFER(ours v retail) -- 91v91, the two 4-byte string frame slots are swapped: retail keeps the default local at [ebp-4] and the read temporary at [ebp-8], the lane the reverse (tried: body local, case-block named local, argument temporary to an inlined helper, explicit operator=, const-ref bound temporary)
// Leaf: TransferString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00431143, length 91 bytes.
// Callees: CPersistContext::CheckCRC(const char*) @ 0x00404500 with "" (0x0122D70E); CCharString::CCharString() @ 0x0099E4B0;
// PLoadStream (+0x24) virtual slot 6 (+0x18) returning a CCharString by value; CCharString::operator= @ 0x0099EFB0;
// CDataOutputStream::WriteNullTerminatedString @ 0x00995AE0 on PSaveStream (+0x28); CCharString::~CCharString() @ 0x0099EAE0.
// The TransferWideString @ 0x004310A7 is the same body for the other string class.
// Shape: a frame-pointer size TU (`push ecx ; push ecx` frame, `push dword ptr [ebp+8]`, `leave`):
// `#pragma optimize("y", off)` + `("s", on)`. A default-constructed CCharString local (retail [ebp-4], never read: the generic
// binary transfer keeps a default value) is built after CheckCRC; the two-case `switch` on Mode lowers to
// `dec eax ; dec eax ; je` / `dec eax ; jne`; the read result is a by-value temporary (retail [ebp-8]) assigned and destroyed.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* m_pStringData;
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const CCharString& other);          // 0x0099EFB0
};

struct CMemoryDataInputStream {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual CCharString ReadNullTerminatedString();          // slot 6 (+0x18), by value through the hidden pointer
};
struct CDataOutputStream {
    void WriteNullTerminatedString(const CCharString& text);   // 0x00995AE0
};
struct CDefinitionManager;
struct CSymbolMap;
struct CStringParser;

// CPersistContext (0x2c bytes, PDB layout Ego_r.pdb; no engine header, HAND_OWNED in engine/INDEX.tsv)
struct CPersistContext {
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18 EMode: 0 text load, 1 text save, 2 binary load, 3 binary save
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28

    void CheckCRC(const char* name);                     // 0x00404500
    void TransferString(CCharString& value);
};

void CPersistContext::TransferString(CCharString& value)
{
    CheckCRC("");
    CCharString defaultValue;
    switch (Mode) {
    case 2:
        value = PLoadStream->ReadNullTerminatedString();
        break;
    case 3:
        PSaveStream->WriteNullTerminatedString(value);
        break;
    }
}
