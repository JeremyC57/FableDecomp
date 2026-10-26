// CPersistContext::Transfer(const char* name, CCharString& value) -- transfer a string with an empty default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00411010, length 76 bytes (+ int3 padding).
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::CCharString(const CCharString&) @ 0x0099EC30, CCharString::~CCharString() @ 0x0099EAE0 (twice),
// CPersistContext::Transfer<CCharString>(const char*, CCharString&, const CCharString&) @ 0x004109A0 (labelled
// "Transfer<CIntelligentPointer<CThing>>", propagated). The CWideString twin @ 0x00411060 has the same shape.
// Shape: the default is copy-initialised from a default-constructed temporary through an inline helper that returns a NAMED
// local (`T value; return value;`): VC7.1 has no NRVO, so the helper's local is built in the low frame slot, copied
// into the caller's named default (the return slot) and destroyed at once,
// then pushes &default, value, name (right to left) with `this` restored into ecx, and destroys the default; `ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
class CCharString { public:
    void* m_pStringData;
    CCharString();                                   // 0x0099E4B0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CMemoryDataInputStream;
class CDataOutputStream;

// CPersistContext (0x2c bytes, PDB layout Ego_r.pdb; no engine header, HAND_OWNED in engine/INDEX.tsv)
class CPersistContext { public:
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

    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);   // 0x004109A0
    void Transfer(const char* name, CCharString& value);
    static CCharString DefaultValue()
    {
        CCharString value;
        return value;
    }
};

void CPersistContext::Transfer(const char* name, CCharString& value)
{
    CCharString defaultValue = DefaultValue();
    Transfer(name, value, defaultValue);
}