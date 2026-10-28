// CDefClassBase::UnloadResource() -- drop the definition manager's instantiated pointer to this def unless something else still holds it (more than two references)
// Grade: DIFFER(68v68) -- block placement only: retail lays the `return false` block (refcount > 2) last behind `ja`; every spelling (early return, inverted if, bool local, result local, else, reference, goto) puts it right after the `jbe`; all instructions otherwise identical
// Leaf: ?UnloadResource@CDefClassBase@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ACE90, 68 bytes (+ int3 padding), thiscall virtual (slot 2), `ret`.
// Identity (proven): Ego_r.pdb ?UnloadResource@CDefClassBase@@UAE_NXZ (0x00B04CE0); labels_rtti_port.tsv puts it in
//   CDefClassBase's RTTI vtable; the label agrees.
// Callees: the def's Destroy (virtual slot 1) when the release drops the count to zero.
// Layout: Ego_r.pdb CDefClassBase PDefManager +0x1C / GlobalIndex +0x20; CDefinitionManager InstantiatedDefs (CArray of
//   CInstantiatedDefInfo*) +0xA4 (engine/CDefinitionManager.h); CInstantiatedDefInfo PDef CDefPointer<CDefClassBase> +0x0C;
//   CIVCountedPointeeBase IVRefCount (unsigned long) +4. Header debt: engine/CDefClassBase.h and CDefinitionManager.h are
//   flat (vptr as a data member, InstantiatedDefs opaque), so the classes are local models.
// Shape: the CDefPointer null assignment releases through a local copy (`dec [ecx+4] ; jne`) and stores 0 twice.
class CIVCountedPointeeBase {                           // Ego_r.pdb: vptr, IVRefCount +4
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Destroy() const;                       // slot 1
    mutable unsigned long IVRefCount;                   // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer& operator=(T* object)
    {
        T* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Destroy();
            Object = 0;
        }
        Object = object;
        if (Object)
            ++Object->IVRefCount;
        return *this;
    }
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {        // Ego_r.pdb: 28 bytes
public:
    virtual bool UnloadResource();                      // slot 2
    CResourceList* ResourceList;                        // +0x08
    CResource* PrevResource;                            // +0x0C
    CResource* NextResource;                            // +0x10
    unsigned long ResourceSize;                         // +0x14
    unsigned long LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource { };
class CDefString { public: void* PData; };
class CDefinitionManager;
class CDefClassBase : public CDefPointeeBase {
public:
    virtual bool UnloadResource();
    CDefinitionManager* PDefManager;                    // +0x1C
    unsigned long GlobalIndex;                          // +0x20
};
class CDefinitionManager {                              // only InstantiatedDefs is read here (engine/CDefinitionManager.h: +0xA4)
public:
    class CInstantiatedDefInfo {                        // Ego_r.pdb: 16 bytes
    public:
        CDefString ClassName;                           // +0x00
        CDefString InstantiationName;                   // +0x04
        unsigned long ClassIndex;                       // +0x08
        CDefPointer<CDefClassBase> PDef;                // +0x0C
    };
    unsigned char _pad00[0xA4];
    CInstantiatedDefInfo** InstantiatedDefs;            // +0xA4 CArray<CInstantiatedDefInfo*> (STLport vector start)
};

bool CDefClassBase::UnloadResource()
{
    CDefinitionManager::CInstantiatedDefInfo* info = PDefManager->InstantiatedDefs[GlobalIndex];
    if (info->PDef.Object && info->PDef.Object->IVRefCount > 2)
        return false;
    info->PDef = 0;
    return true;
}
