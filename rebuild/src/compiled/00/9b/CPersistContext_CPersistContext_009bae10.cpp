// CPersistContext::CPersistContext(CCharString* stringToSaveTo) -- text-save context: Mode 1, the target string, no definition manager, CRC check off
// Leaf: ??0CPersistContext
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BAE10, length 29 bytes (+ int3 padding).
// Callees: none -- the out-of-line Init(CCharString*, CDefinitionManager*) @ 0x009BAA10 (same four stores, `ret 8`) is
// inlined with a null definition manager.
// Shape: `mov eax,ecx` (constructor returns this), the argument loaded into ecx, then Mode=1, +0x20, the immediate
// zero store to +0x00 and the byte clear of +0x15 in Init's statement order; `ret 4`.
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

    void Init(CCharString* stringToSaveTo, CDefinitionManager* definitionManager)
    {
        Mode = 1;
        PStringToSaveTo = stringToSaveTo;
        PDefinitionManager = definitionManager;
        UseCRCCheck = false;
    }
    CPersistContext(CCharString* stringToSaveTo);
};

CPersistContext::CPersistContext(CCharString* stringToSaveTo)
{
    Init(stringToSaveTo, 0);
}