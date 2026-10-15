// CPersistContext::Transfer(const char* name, C2DVector& value) -- transfer a 2D vector with a default of (0, 0)
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800D30, length 44 bytes.
// Callees: CPersistContext::Transfer<C2DVector>(const char*, C2DVector&, const C2DVector&) @ 0x00800B90 (CheckCRC, the
// mode switch, CDataOutputStream::Write2DVector @ 0x00993F90; its label "Transfer<signed char>" is propagated).
// Shape: an 8-byte frame temporary C2DVector(0.0f, 0.0f) whose two float members are stored as IMMEDIATES after the
// pushes (float zeroes do not claim a register); `this` stays in ecx; `add esp,8 ; ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
struct C2DVector {
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
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

    void Transfer(const char* name, C2DVector& value, const C2DVector& defaultValue);   // 0x00800B90
    void Transfer(const char* name, C2DVector& value);
};

void CPersistContext::Transfer(const char* name, C2DVector& value)
{
    Transfer(name, value, C2DVector(0.0f, 0.0f));
}
