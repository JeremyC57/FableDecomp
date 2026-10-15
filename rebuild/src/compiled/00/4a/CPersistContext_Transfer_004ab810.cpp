// CPersistContext::Transfer(const char* name, double& value) -- transfer a double with a default of 0.0
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AB810, length 37 bytes (+ int3 padding).
// Callees: CPersistContext::Transfer<double>(const char*, double&, const double&) @ 0x004AB710 (CheckCRC, then the
// mode switch with `fld qword ptr` copies; its label "Transfer<signed char>" is propagated). The catalog label
// "Transfer<long>" is propagated too: the default is the double 0.0 from the pool @ 0x0122ED70.
// Shape: an 8-byte frame temporary for the literal default (`fld [0.0] ; fstp qword ptr [esp]`), its address pushed,
// then the value and name slots; `this` stays in ecx; `add esp,8 ; ret 8`.
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

    void Transfer(const char* name, double& value, const double& defaultValue);   // 0x004AB710
    void Transfer(const char* name, double& value);
};

void CPersistContext::Transfer(const char* name, double& value)
{
    Transfer(name, value, 0.0);
}