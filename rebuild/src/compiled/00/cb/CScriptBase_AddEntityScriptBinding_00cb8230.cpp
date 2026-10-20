// CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* binding) -- take ownership of an entity-script binding through a counted pointer and file it under its name in EntityScriptBindings
// Leaf: AddEntityScriptBinding
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB8230, length 189 bytes (`ret 4`).
// PDB: ?AddEntityScriptBinding@CScriptBase@@QAEXPAVCEntityScriptBindingBase@@@Z.
// Callees: the binding's virtual slot 0 (its name, by reference), operator new @ 0x00BFEA1A (the 12-byte count block),
// CCountedPointer<CEntityScriptBindingBase>::DeleteData @ 0x00CB8D30 (the block's destroy member pointer; manifest
// "operator_new" is propagated -- the body is `operator delete(this)`), CCharString(const CCharString&) @ 0x0099EC30,
// the 12-byte {CCharString, CCountedPointer} vector push_back @ 0x00CBD310 (landed as std::vector<CScriptThing>::push_back
// and called as CNamedChunkVector::push_back by C3DAnimFile::AddNamedChunk 0x00CBD670), operator delete @ 0x00BFE9BC,
// ~CCharString @ 0x0099EAE0.
// Layout: retail CScriptBase sits 0x10 below Ego_r.pdb (Threads list one word at +4, CVectorMaps 16 bytes), so
// EntityScriptBindings is +0x18 (vector +0x18, comparator +0x24, Dirty +0x25).
// Shape: the AddAction 0x007137D0 counted pointer built from the raw binding (new-expression null test), then the
// CVectorMap's inline Add -- push_back of a {name, pointer} temporary (string copy out of line, count bump inline,
// destroyed with the re-read `--count; if (count == 0)` release and both words cleared) and Dirty = true -- as in the
// landed C3DAnimFile::AddNamedChunk (Add must be __forceinline, else VC emits it out of line); the local counted
// pointer's release keeps the block in esi.
#include <stddef.h>
void* __cdecl operator new(size_t size);            // 0x00BFEA1A
void __cdecl operator delete(void* block);          // 0x00BFE9BC

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

class CEntityScriptBindingBase {
public:
    virtual const CCharString& GetName() const;     // slot 0
    void DeleteData();                              // 0x00CB8D30 (CCountedPointer<CEntityScriptBindingBase>::DeleteData)
};

template <class T> struct SCountBlock {
    long m_references;                              // +0x00
    void (T::*m_pfnDestroy)();                      // +0x04
    T*   m_pItem;                                   // +0x08
    SCountBlock(T* item) : m_references(1), m_pfnDestroy(&T::DeleteData), m_pItem(item) { }
};
template <class T> struct CCountedPointer {
    T* Data;                                        // +0x00
    SCountBlock<T>* Info;                           // +0x04
    CCountedPointer(T* item) : Data(item), Info(new SCountBlock<T>(item)) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info) ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct SBindingEntry {                              // pair<CCharString, CCountedPointer<CEntityScriptBindingBase>>
    CCharString                               first;    // +0x00
    CCountedPointer<CEntityScriptBindingBase> second;   // +0x04
    SBindingEntry(const CCharString& name, const CCountedPointer<CEntityScriptBindingBase>& binding)
        : first(name), second(binding) { }
};
struct CBindingVector {                             // STLport vector<SBindingEntry>
    SBindingEntry* _M_start;
    SBindingEntry* _M_finish;
    SBindingEntry* _M_end_of_storage;
    void push_back(const SBindingEntry& entry);     // 0x00CBD310
};
struct CBindingMap {                                // CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<..>>
    CBindingVector m_vector;                        // +0x00
    unsigned char  m_compare;                       // +0x0c (empty comparator)
    bool           Dirty;                           // +0x0d
    __forceinline void Add(const CCharString& name, const CCountedPointer<CEntityScriptBindingBase>& binding)
    {
        m_vector.push_back(SBindingEntry(name, binding));
        Dirty = true;
    }
};

class CScriptBase {
public:
    void*         __vftable;                        // +0x00
    unsigned char _threadsAndScripts[0x14];         // +0x04 Threads (+0x04), EntityScripts (+0x08)
    CBindingMap   EntityScriptBindings;             // +0x18
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);
};

void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* binding)
{
    const CCharString& name = binding->GetName();
    CCountedPointer<CEntityScriptBindingBase> pointer(binding);
    EntityScriptBindings.Add(name, pointer);
}