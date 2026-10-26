// CDefinitionManager::GetPDefFromInstantiationName(const CCharString&) -- resolve the name to a global def index, load that def and return a counted pointer to it (null for an unknown name)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ADA90, length 80 bytes.
// Callees: CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CDefinitionManager::LoadDef(unsigned long)
// @ 0x009AD6E0. The CDefPointer<CDefClassBase> result is built through the hidden pointer (`ret 8`: result + name).
// PDB CDefinitionManager: InstantiatedDefs (CArray<CInstantiatedDefInfo*>) at +188, retail +0xa4 (six 4-byte-smaller
// containers before it); CInstantiatedDefInfo: PDef (CDefPointer) at +0xc; CDefPointeeBase keeps its reference count at +4.
// Shape: the copy constructor's increment goes through a register (`mov edx,[ecx+4] ; inc edx ; mov [ecx+4],edx`); the
// `index <= 0` branch default-constructs the result. 0x009ADA40 is the same body for a second instantiation.
class CCharString;

class CDefPointeeBase { public:
    virtual void v00();
    long RefCount;                                 // +4
};

struct CDefPointer {                               // CDefPointer<CDefClassBase>
    CDefPointeeBase* Object;                       // +0
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->RefCount = Object->RefCount + 1;
    }
};

struct CInstantiatedDefInfo {
    long        ClassName;                         // +0x00 CDefString
    long        InstantiationName;                 // +0x04 CDefString
    unsigned long ClassIndex;                      // +0x08
    CDefPointer PDef;                              // +0x0c
};

class CDefinitionManager { public:
    unsigned char          _pad[0xa4];             // +0x00 vptr, maps, symbol tables, compressed data
    CInstantiatedDefInfo** InstantiatedDefs;       // +0xa4 CArray<CInstantiatedDefInfo*>::m_begin
    void LoadDef(unsigned long globalIndex);                   // 0x009AD6E0
    CDefPointer GetPDefFromInstantiationName(const CCharString& name);
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

CDefPointer CDefinitionManager::GetPDefFromInstantiationName(const CCharString& name)
{
    long index = GetDefGlobalIndexFromName(name);
    if (index > 0) {
        LoadDef(index);
        return InstantiatedDefs[index]->PDef;
    }
    return CDefPointer();
}