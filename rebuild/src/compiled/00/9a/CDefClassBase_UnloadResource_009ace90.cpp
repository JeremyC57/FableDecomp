// CDefClassBase::UnloadResource() -- drop the definition manager's instantiated pointer to this def unless something else still holds it (more than two references)
// Leaf: ?UnloadResource@CDefClassBase@@UAE_NXZ
// Grade: MATCH (wave 2 sec5) -- shared engine/CDefClassBase.h + CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE) with the PDB
//   nested CInstantiatedDefInfo added to CDefinitionManager by the define trick; the null-safe count (IsNull() ? 0 :
//   GetRefCount()) inside an inverted if (release block first, return false last) gives retail's ja to the trailing
//   false block; CDefPointer::Reset(T*) (declared in the header, body unproven) is expanded here as Release + store.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ACE90, 68 bytes (+ int3 padding), thiscall virtual (slot 2), `ret`.
// Identity (proven): Ego_r.pdb ?UnloadResource@CDefClassBase@@UAE_NXZ (0x00B04CE0); labels_rtti_port.tsv puts it in
//   CDefClassBase's RTTI vtable; the label agrees.
// Callees: the def's Destroy (virtual slot 1) when the release drops the count to zero.
// Layout: Ego_r.pdb CDefClassBase PDefManager +0x1C / GlobalIndex +0x20; CDefinitionManager InstantiatedDefs (CArray of
//   CInstantiatedDefInfo*) +0xA4 (engine/CDefinitionManager.h); CInstantiatedDefInfo PDef CDefPointer<CDefClassBase> +0x0C;
//   CIVCountedPointeeBase IVRefCount (unsigned long) +4. Header debt: engine/CDefClassBase.h and CDefinitionManager.h are
//   flat (vptr as a data member, InstantiatedDefs opaque), so the classes are local models.
// Shape: the CDefPointer null assignment releases through a local copy (`dec [ecx+4] ; jne`) and stores 0 twice.
// Header model (wave 2 sec5): engine/CDefClassBase.h + engine/CDefPointer.h retail chain (FABLE_RETAIL_DEF_CLASS_BASE);
//   CDefinitionManager's flat header keeps InstantiatedDefs opaque, so the define trick adds the nested PDB
//   CInstantiatedDefInfo (Ego_r.pdb, 16 bytes) and types the CArray start pointer.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CDefString { public: void* PData; };
class CDefinitionManager : public CDefinitionManagerLayout {
public:
    class CInstantiatedDefInfo {                        // Ego_r.pdb: 16 bytes
    public:
        CDefString ClassName;                           // +0x00
        CDefString InstantiationName;                   // +0x04
        unsigned long ClassIndex;                       // +0x08
        CDefPointer<CDefClassBase> PDef;                // +0x0C
    };
    CInstantiatedDefInfo* GetInstantiatedDef(unsigned long index) const
    { return (*reinterpret_cast<CInstantiatedDefInfo** const*>(InstantiatedDefs))[index]; }
};

// The header declares Reset(T*) with an unproven body; this row expands it as Release + store (+ AddRef of a null object).
template <class T> inline void CDefPointer<T>::Reset(T* object)
{
    Release();
    Object = object;
    if (Object) Object->AddRef();
}

bool CDefClassBase::UnloadResource()
{
    CDefinitionManager::CInstantiatedDefInfo* info = PDefManager->GetInstantiatedDef(GlobalIndex);
    if ((info->PDef.IsNull() ? 0 : info->PDef.GetRefCount()) <= 2) {
        info->PDef.Reset(0);
        return true;
    }
    return false;
}