// CPersistContext::Init(CMemoryDataInputStream& stream, bool) -- sets the context up to load from a memory input stream
// Retail: Fable.exe @ 0x009BAA30, 27 bytes, `ret 8`. Catalog label CPersistContext::CPersistContext is wrong: the body
// does not return `this` in eax (a VC7.1 ctor always does), so this is Ego_r.pdb's void
// ?Init@CPersistContext@@QAEXAAVCMemoryDataInputStream@@_N@Z (same argument list as the ctor overload). Stores PLoadStream (+0x24) = stream, Mode (+0x18) = 2,
// PSaveStream (+0x28) = 0, PDefinitionManager (+0x00) = 0, UseCRCCheck (+0x15) = false; the bool argument is not read.
// Layout: Ego_r.pdb CPersistContext (44 bytes); no engine header exists, so the class is modelled locally.
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
class CPersistContext {
public:
    enum EMode { MODE_0, MODE_1, MODE_2 };
    CDefinitionManager*     PDefinitionManager;          // +0x00
    CSymbolMap*             PSymbolMap;                  // +0x04
    CStringParser*          PParser;                     // +0x08
    void (*ErrorCallback)(const CCharString&);           // +0x0c
    CCharString*            PStringToClean;              // +0x10
    bool                    UseDefaults;                 // +0x14
    bool                    UseCRCCheck;                 // +0x15
    bool                    SingleTransfer;              // +0x16
    bool                    Finished;                    // +0x17
    EMode                   Mode;                        // +0x18
    bool                    Binary;                      // +0x1c
    CCharString*            PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    void Init(CMemoryDataInputStream& stream, bool);
};
void CPersistContext::Init(CMemoryDataInputStream& stream, bool)
{
    PLoadStream = &stream;
    Mode = MODE_2;
    PSaveStream = 0;
    PDefinitionManager = 0;
    UseCRCCheck = false;
}