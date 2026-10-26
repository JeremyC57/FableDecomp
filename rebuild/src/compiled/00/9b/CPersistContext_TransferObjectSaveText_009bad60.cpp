// CPersistContext::TransferObjectSaveText(const char* name, CPersistContext::TransferableComponentGenericType* component) -- text save of a component: "<name> ", the component's own text, then ";\r\n"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BAD60, length 59 bytes (+ int3 padding).
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 three times on PStringToSaveTo (+0x20); the component's
// virtual slot 0 (`call [edx]`) with (this context, PStringToSaveTo). Literals " " @ 0x01258C1C, ";\r\n" @ 0x012353B8.
// Shape: the name argument is loaded before `push esi`; PStringToSaveTo is re-read from +0x20 for every append (the
// appends go through a member that a call may change); the virtual call pushes the string first, then `this`; `ret 8`.
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

    struct TransferableComponentGenericType {
        virtual void TransferSaveText(CPersistContext* context, CCharString* text);   // slot 0
    };
    void TransferObjectSaveText(const char* name, TransferableComponentGenericType* component);
};

void CPersistContext::TransferObjectSaveText(const char* name, TransferableComponentGenericType* component)
{
    *PStringToSaveTo += name;
    *PStringToSaveTo += " ";
    component->TransferSaveText(this, PStringToSaveTo);
    *PStringToSaveTo += ";\r\n";
}