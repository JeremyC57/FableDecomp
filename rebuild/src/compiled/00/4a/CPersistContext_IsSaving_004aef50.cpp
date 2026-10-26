// CPersistContext::IsSaving() -- true (as a full-register long) when the context writes: Mode 1 (text save) or Mode 3 (binary save)
// Leaf: IsSaving
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AEF50, length 22 bytes.
// Callees: none. The catalog label "IsBinary" is propagated/misread: the constructors (0x009BAE10 Mode 1 + PStringToSaveTo,
// 0x009BAE30 Mode 2 + PLoadStream, 0x009BAE50 Mode 3 + PSaveStream, 0x009BADD0 Mode 0 + PParser) make modes 1 and 3 the two
// SAVING modes (text and binary), so the test is "is saving", not "is binary".
// Shape: `mov eax,[ecx+18h] ; cmp eax,1 ; je ; cmp eax,3 ; je ; xor eax,eax ; ret ; mov eax,1 ; ret` -- the `||` of two
// compares returned as a long (a bool result would be `mov al,1` / `xor al,al`).
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;

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

    long IsSaving() const;
};

long CPersistContext::IsSaving() const
{
    return Mode == 1 || Mode == 3;
}