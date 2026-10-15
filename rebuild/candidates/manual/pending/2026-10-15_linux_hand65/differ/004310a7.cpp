// CPersistContext::TransferWideString(CWideString& value) -- binary transfer of a null-terminated string: CRC check, then read it from the load stream (Mode 2) or write it to the save stream (Mode 3)
// Grade: DIFFER(ours v retail) -- 91v91, the two 4-byte string frame slots are swapped: retail keeps the default local at [ebp-4] and the read temporary at [ebp-8], the lane the reverse (tried: body local, case-block named local, argument temporary to an inlined helper, explicit operator=, const-ref bound temporary)
// Leaf: TransferWideString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004310A7, length 91 bytes.
// Callees: CPersistContext::CheckCRC(const char*) @ 0x00404500 with "" (0x0122D70E); CWideString::CWideString() @ 0x0099AED0;
// PLoadStream (+0x24) virtual slot 7 (+0x1c) returning a CWideString by value; CWideString::operator= @ 0x0099B7D0;
// CDataOutputStream::WriteNullTerminatedWString @ 0x00993F00 on PSaveStream (+0x28); CWideString::~CWideString() @ 0x0099B510.
// The TransferString @ 0x00431143 is the same body for the other string class.
// Shape: a frame-pointer size TU (`push ecx ; push ecx` frame, `push dword ptr [ebp+8]`, `leave`):
// `#pragma optimize("y", off)` + `("s", on)`. A default-constructed CWideString local (retail [ebp-4], never read: the generic
// binary transfer keeps a default value) is built after CheckCRC; the two-case `switch` on Mode lowers to
// `dec eax ; dec eax ; je` / `dec eax ; jne`; the read result is a by-value temporary (retail [ebp-8]) assigned and destroyed.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CWideString {
    void* m_pStringData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
    CWideString& operator=(const CWideString& other);          // 0x0099B7D0
};
struct CCharString;
struct CMemoryDataInputStream {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual CWideString ReadNullTerminatedWString();          // slot 7 (+0x1c), by value through the hidden pointer
};
struct CDataOutputStream {
    void WriteNullTerminatedWString(const CWideString& text);   // 0x00993F00
};
struct CDefinitionManager;
struct CSymbolMap;
struct CStringParser;
struct CCharString;

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
    void TransferWideString(CWideString& value);
};

void CPersistContext::TransferWideString(CWideString& value)
{
    CheckCRC("");
    CWideString defaultValue;
    switch (Mode) {
    case 2:
        value = PLoadStream->ReadNullTerminatedWString();
        break;
    case 3:
        PSaveStream->WriteNullTerminatedWString(value);
        break;
    }
}
