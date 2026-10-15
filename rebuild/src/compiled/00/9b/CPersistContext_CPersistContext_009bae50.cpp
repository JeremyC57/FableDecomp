// CPersistContext::CPersistContext(CDataOutputStream* saveStream, bool) -- binary-save context: Mode 3, the output stream, no input stream or definition manager, CRC check off
// Leaf: ??0CPersistContext
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BAE50, length 29 bytes (+ int3 padding).
// Callees: none -- the out-of-line Init(CDataOutputStream*, bool) @ 0x009BAA50 (the same five stores, `ret 8`) is
// inlined; the bool argument is not read by the release body.
// Shape: `mov eax,ecx`, the stream stored first, one zero register (ecx) for +0x24, +0x00 and the byte +0x15, the
// Mode immediate scheduled between; `ret 8`.
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

    void Init(CDataOutputStream* saveStream, bool)
    {
        PSaveStream = saveStream;
        Mode = 3;
        PLoadStream = 0;
        PDefinitionManager = 0;
        UseCRCCheck = false;
    }
    CPersistContext(CDataOutputStream* saveStream, bool useCRCCheck);
};

CPersistContext::CPersistContext(CDataOutputStream* saveStream, bool useCRCCheck)
{
    Init(saveStream, useCRCCheck);
}