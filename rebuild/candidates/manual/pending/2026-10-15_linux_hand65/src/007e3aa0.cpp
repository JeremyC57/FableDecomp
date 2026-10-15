// CPersistContext::Transfer(const char* name, long& value) -- transfer a long with a default of -1
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E3AA0, length 32 bytes.
// Callees: CPersistContext::Transfer<long>(const char*, long&, const long&) @ 0x007E39D0 (CheckCRC, then the mode switch
// with dword copies / WriteLong; its label "Transfer<signed char>" is propagated).
// Shape: a one-dword frame slot (`push ecx`) holds the literal default -1, stored only after the three pushes; `this`
// stays in ecx; `pop ecx ; ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
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

    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x007E39D0
    void Transfer(const char* name, long& value);
};

void CPersistContext::Transfer(const char* name, long& value)
{
    Transfer(name, value, -1L);
}
