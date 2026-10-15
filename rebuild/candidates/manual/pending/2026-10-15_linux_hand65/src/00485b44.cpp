// CPersistContext::Transfer(const char* name, C3DVector& value) -- transfer a 3D vector with a default of (0, 0, 0)
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485B44, length 40 bytes.
// Callees: CPersistContext::Transfer<C3DVector>(const char*, C3DVector&, const C3DVector&) @ 0x004859AD (CheckCRC, the
// mode switch with a three-`movsd` default copy, CDataOutputStream::Write3DVector @ 0x00993FD0; labelled
// "Transfer<CFloatRange>", propagated).
// Shape: a frame-pointer size TU (`push dword ptr [ebp+N]`, `leave`): `#pragma optimize("y", off)` + `("s", on)`; the
// 12-byte temporary C3DVector(0.0f, 0.0f, 0.0f) is filled with `fldz ; fstp` (the size form of a float zero store)
// interleaved with the pushes; `this` stays in ecx; `ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
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

    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue);   // 0x004859AD
    void Transfer(const char* name, C3DVector& value);
};

void CPersistContext::Transfer(const char* name, C3DVector& value)
{
    Transfer(name, value, C3DVector(0.0f, 0.0f, 0.0f));
}
