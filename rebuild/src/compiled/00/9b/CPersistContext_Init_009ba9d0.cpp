// Grade: MATCH (2026-10-24, fixnames; test PASS)
// Fix (2026-10-24): `Mode = 0` written first keeps eax as the zero register so every pointer copy goes through edx (register-tie park resolved).
// CPersistContext::Init(CStringParser*, ErrorCallback, bool useDefaults, CDefinitionManager*, CSymbolMap*, CCharString* stringToClean) -- reset to a text-load context: Mode 0 with the parser, error callback, managers and the string to clean; the flags cleared
// Grade (parked, before the fix): DIFFER(ours v retail) -- 58v58/60v60 same instructions; retail copies each pointer argument through ONE register (edx: load, store, load, store), the lane interleaves two (eax/edx or ecx/edx) and schedules the zero register after the +0x10 store
// Leaf: Init
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BA9D0, length 58 bytes (+ int3 padding).
// Callees: none. The catalog label "CPersistContext::CPersistContext" is propagated: the body does not return `this`
// (no `mov eax,ecx`; eax is the zero register), so it is the void Init that the real constructor @ 0x009BADD0 inlines.
// Shape: the five pointer members stored in layout order through edx, then one zero register (eax) for Mode, the
// useDefaults byte, and the bytes +0x15, +0x17, +0x16 (Finished before SingleTransfer); `ret 18h`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
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

    void Init(CStringParser* parser, void (__cdecl* errorCallback)(CCharString*), bool useDefaults,
              CDefinitionManager* definitionManager, CSymbolMap* symbolMap, CCharString* stringToClean);
};

void CPersistContext::Init(CStringParser* parser, void (__cdecl* errorCallback)(CCharString*), bool useDefaults,
                           CDefinitionManager* definitionManager, CSymbolMap* symbolMap, CCharString* stringToClean)
{
    Mode = 0;
    PDefinitionManager = definitionManager;
    PSymbolMap = symbolMap;
    PParser = parser;
    ErrorCallback = errorCallback;
    PStringToClean = stringToClean;
    UseDefaults = useDefaults;
    UseCRCCheck = false;
    Finished = false;
    SingleTransfer = false;
}