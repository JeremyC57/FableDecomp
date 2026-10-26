// CPersistContext::TransferSubComponentSaveText(const char* name, CPersistContext::TransferableSubComponentGenericType* subComponent) -- text save of a sub-component: "<name>." then the sub-component's own text
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BADA0, length 42 bytes (+ int3 padding).
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 twice on PStringToSaveTo (+0x20); the sub-component's
// virtual slot 0 (`call [edx]`) with this context. Literal "." @ 0x01238794.
// Shape: the name argument is loaded before `push esi`; PStringToSaveTo re-read from +0x20 per append; the virtual call
// takes only the context; `ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString { public:
    void* PStringData;
    const CCharString& operator+=(const char* text);      // 0x0099F100
};
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

    struct TransferableSubComponentGenericType {
        virtual void TransferSaveText(CPersistContext* context);   // slot 0
    };
    void TransferSubComponentSaveText(const char* name, TransferableSubComponentGenericType* subComponent);
};

void CPersistContext::TransferSubComponentSaveText(const char* name, TransferableSubComponentGenericType* subComponent)
{
    *PStringToSaveTo += name;
    *PStringToSaveTo += ".";
    subComponent->TransferSaveText(this);
}