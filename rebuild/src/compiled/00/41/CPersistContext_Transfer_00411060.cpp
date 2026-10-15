// CPersistContext::Transfer(const char* name, CWideString& value) -- transfer a string with an empty default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00411060, length 76 bytes (+ int3 padding).
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::~CWideString() @ 0x0099B510 (twice),
// CPersistContext::Transfer<CWideString>(const char*, CWideString&, const CWideString&) @ 0x00410AC0 (labelled
// "Transfer<CIntelligentPointer<CThing>>", propagated). The CCharString twin @ 0x00411010 has the same shape.
// Shape: the default is copy-initialised from a default-constructed temporary through an inline helper that returns a NAMED
// local (`T value; return value;`): VC7.1 has no NRVO, so the helper's local is built in the low frame slot, copied
// into the caller's named default (the return slot) and destroyed at once,
// then pushes &default, value, name (right to left) with `this` restored into ecx, and destroys the default; `ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
struct CWideString {
    void* m_pStringData;
    CWideString();                                   // 0x0099AED0
    CWideString(const CWideString& other);                 // 0x0099B720
    ~CWideString();                                  // 0x0099B510
};
struct CDefinitionManager;
struct CSymbolMap;
struct CStringParser;
struct CCharString;
struct CMemoryDataInputStream;
struct CDataOutputStream;

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

    void Transfer(const char* name, CWideString& value, const CWideString& defaultValue);   // 0x00410AC0
    void Transfer(const char* name, CWideString& value);
    static CWideString DefaultValue()
    {
        CWideString value;
        return value;
    }
};

void CPersistContext::Transfer(const char* name, CWideString& value)
{
    CWideString defaultValue = DefaultValue();
    Transfer(name, value, defaultValue);
}