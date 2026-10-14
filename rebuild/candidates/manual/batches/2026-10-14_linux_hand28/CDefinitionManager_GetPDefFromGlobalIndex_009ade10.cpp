// CDefinitionManager::GetPDefFromGlobalIndex(const unsigned long&, long) -- map the class key to its class info, take the global index at `index` of that class's def list, load the def and return a counted pointer to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ADE10, length 62 bytes.
// Callees: the class-info lookup @ 0x009AD2E0 (__thiscall, key by reference; the manifest label
// "CFactionDatabase::FindFactionBySymbolName" is propagated), CDefinitionManager::LoadDef(unsigned long) @ 0x009AD6E0.
// The CDefPointer<CDefClassBase> result is built through the hidden pointer (`ret 0Ch`: result + key + index).
// PDB CDefinitionManager: InstantiatedDefs at +188, retail +0xa4; CInstantiatedDefInfo: PDef at +0xc; the class info
// record keeps its def-index array pointer at +8. Shape: the copy constructor's increment is the single
// `inc dword ptr [ecx+4]` of an inline AddRef helper, scheduled after the pops.
struct CDefPointeeBase {
    virtual void v00();
    long RefCount;                                 // +4
    void AddRef() { ++RefCount; }
};

struct CDefPointer {                               // CDefPointer<CDefClassBase>
    CDefPointeeBase* Object;                       // +0
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
};

struct CInstantiatedDefInfo {
    long          ClassName;                       // +0x00 CDefString
    long          InstantiationName;               // +0x04 CDefString
    unsigned long ClassIndex;                      // +0x08
    CDefPointer   PDef;                            // +0x0c
};

struct CDefClassInfo {
    unsigned char  _pad[8];                        // +0x00
    unsigned long* GlobalIndices;                  // +0x08 the class's instantiated def indices
};

struct CDefinitionManager {
    unsigned char          _pad[0xa4];             // +0x00 vptr, maps, symbol tables, compressed data
    CInstantiatedDefInfo** InstantiatedDefs;       // +0xa4 CArray<CInstantiatedDefInfo*>::m_begin
    CDefClassInfo* FindClassInfo(const unsigned long& classKey);   // 0x009AD2E0
    void LoadDef(unsigned long globalIndex);                       // 0x009AD6E0
    CDefPointer GetPDefFromGlobalIndex(const unsigned long& classKey, long index);
};

CDefPointer CDefinitionManager::GetPDefFromGlobalIndex(const unsigned long& classKey, long index)
{
    unsigned long globalIndex = FindClassInfo(classKey)->GlobalIndices[index];
    LoadDef(globalIndex);
    return InstantiatedDefs[globalIndex]->PDef;
}
