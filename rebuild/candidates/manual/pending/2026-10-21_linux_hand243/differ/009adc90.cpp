// CDefinitionManager::InstantiateDef(const CCharString& name, const CCharString& className, bool recompiled) -- look the class up in DefClassInfo, create a def through the class factory, flag it as recompiled, name it when the class says so, record it (global index, class slot) in a new SInstantiatedDefInfo appended to InstantiatedDefs, add the name to both symbol maps, and drop the creation reference
// Grade: DIFFER(367v382) -- retail spills the class-info pointer to the frame and keeps the two counts in ebx/edi before the factory call; ours keeps the info in a register and rebuilds the vector base through lea; everything after the factory call is in the same order
// Leaf: InstantiateDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ADC90, length 371 bytes (+ int3 padding), `ret 0xc`.
// Callees: CVectorMap<..., CDefClassInfo>::Find @ 0x009AD580 (label "CVectorMap_FindByKey"), the class info's factory function pointer (+4),
// the def's virtual slots 0x40 (NeedsName) and 0x2c (SetName), CDefString::CDefString(const CCharString&) @ 0x009D5060, operator new
// @ 0x00BFEA1A (16-byte SInstantiatedDefInfo), the two vector<long> push_back grow paths @ 0x009B4BD0 / 0x00464ACE, CSymbolMap::
// AddSymbolDirty(const CCharString&, long) @ 0x009E5420 and (prefix, name, long) @ 0x009E54E0, the def's virtual slot 1 (Destroy) on the
// last reference.
// Layout: retail CDefinitionManager (engine header minus 4 per 16-byte vector): PSymbolMap +0x58, PDefinitionSymbolMap +0x60, InstantiatedDefs
// +0xa4 (12-byte vector); CDefClassInfo {?, factory +4, Instances vector +8}; def ref count +4, flags byte +0x24 (bit 1 = recompiled),
// manager +0x1c, global index +0x20.
// Shape: the global index is the InstantiatedDefs count and the class slot the Instances count, both taken before the factory call; two
// inline push_backs with the grow path; the recompiled flag is a one-bit bitfield assignment (`shl / xor / and / xor`); one shared `return false` exit.
#include <new>
struct CCharString { void* PStringData; };
struct SScalarTag { SScalarTag() { } };
template<class T1, class T2> inline void Construct(T1* p, const T2& value) { new (p) T1(value); }
struct CRetailVector_dword {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);
    long size() const { return (long)(last - first); }
    void push_back(const long& value)
    {
        if (last != end) {
            Construct(last, value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CRetailVector_ptr {
    void** first;
    void** last;
    void** end;
    void InsertGrow(void** where, void* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x009B4BD0
    long size() const { return (long)(last - first); }
    void push_back(void* const& value)
    {
        if (last != end) {
            Construct(last, value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CDefString {
    long m_position;
    CDefString(const CCharString& name);           // 0x009D5060
};
struct CDefinitionManager;
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a();
    virtual void SetName(CDefString name);         // slot 0x2c / 4
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual bool NeedsName();                      // slot 0x40 / 4
    long               RefCount;                   // +0x04
    unsigned char      _pad08[0x1c - 0x08];
    CDefinitionManager* Manager;                   // +0x1c
    long               GlobalIndex;                // +0x20
    unsigned char      Flag0 : 1;                  // +0x24 bit 0
    unsigned char      Recompiled : 1;             // +0x24 bit 1
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
struct CDefClassInfo {
    long               ClassIndex;                 // +0x00
    CDefClassBase*     (__cdecl* Create)();        // +0x04
    CRetailVector_dword Instances;                 // +0x08
};
struct SInstantiatedDefInfo {                      // 16 bytes
    CDefString     Name;                           // +0x00
    CDefString     ClassName;                      // +0x04
    long           ClassSlot;                      // +0x08
    CDefClassBase* Def;                            // +0x0c
    SInstantiatedDefInfo(const CCharString& name, const CCharString& className, long classSlot, CDefClassBase* def)
        : Name(name), ClassName(className), ClassSlot(classSlot), Def(def) { ++def->RefCount; }
};
struct CSymbolMap {
    void AddSymbolDirty(const CCharString& name, long value);                              // 0x009E5420
    void AddSymbolDirty(const CCharString& prefix, const CCharString& name, long value);   // 0x009E54E0
};
struct CDefinitionManager {
    unsigned char      _pad00[0x58];
    CSymbolMap*        PSymbolMap;                 // +0x58
    unsigned char      _pad5c[4];
    CSymbolMap*        PDefinitionSymbolMap;       // +0x60
    unsigned char      _pad64[0xa4 - 0x64];
    CRetailVector_ptr  InstantiatedDefs;           // +0xa4
    CDefClassInfo* FindClassInfo(const CCharString& className);   // 0x009AD580
    bool InstantiateDef(const CCharString& name, const CCharString& className, bool recompiled);
};

bool CDefinitionManager::InstantiateDef(const CCharString& name, const CCharString& className, bool recompiled)
{
    CDefClassInfo* info = FindClassInfo(className);
    if (info) {
    long globalIndex = InstantiatedDefs.size();
    long classSlot = info->Instances.size();
    CDefClassBase* def = info->Create();
    def->Recompiled = recompiled;
    if (def->NeedsName())
        def->SetName(CDefString(name));
    def->Manager = this;
    def->GlobalIndex = globalIndex;
    SInstantiatedDefInfo* entry = new SInstantiatedDefInfo(name, className, classSlot, def);
    InstantiatedDefs.push_back(entry);
    info->Instances.push_back(globalIndex);
    PSymbolMap->AddSymbolDirty(name, globalIndex);
    PDefinitionSymbolMap->AddSymbolDirty(className, name, globalIndex);
    def->Release();
    return true;
    }
    return false;
}
