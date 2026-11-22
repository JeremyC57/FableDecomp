// CTCContainer::InsertItem(long defIndex, const CCharString& name) -- append (def index, name) to the container's item list; a container whose thing does not persist its game state yet starts doing so when the item's def asks for it
// Leaf: ?InsertItem@CTCContainer@@QAEXJABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED1F0, 184 bytes (`ret 8`).
// Callees: CCharString(const CCharString&) @ 0x0099EC30 (the pair, then the placement copy), the item vector's
// _M_insert_overflow @ 0x0074FCE0 (manifest "std::vector_InsertRangeWithCopy"), ~CCharString @ 0x0099EAE0,
// CThing::IsGameStatePersisted() const @ 0x004C8280, CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDef<T>
// (long, CDefPointer*) @ 0x0042AF3C (manifest "GetDef<CSpecialAbilitiesDrunkennessDef>", byte twin), CThing::
// SetAsGameStatePersisted(bool) @ 0x004C7880, the def's virtual slot 1 on the last release.
// Layout: CTCContainer (Ego_r.pdb) ContainerItems vector<pair<long, CCharString>> +0x0c (8-byte elements); the item def's flag
// byte +0x42 bit 1 (named descriptively).
// Shape: a pair<long, CCharString> local (index stored next to the string copy) handed to the inline STLport push_back
// (placement memberwise copy / _M_insert_overflow with the __false_type tag in the dead index slot); the def out pointer also
// reuses the dead index slot, the def taken out of it into a holder whose release loses its null test after the flag read.
// NF254 (2026-11-22, CODEXP27 L4 / L5 family): the item vector and its element were the row-local CContainerItemVector / SContainerItem /
//   CCharString stand-ins (bound 0x0074FCE0 as ?_M_insert_overflow@CContainerItemVector@@...); they now come from the shared headers
//   (fable_string.h CCharString, stlport/fable_containers.h vector<pair<long, CCharString> > with the overflow declaration only:
//   FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY), so the call binds the protected STLport spelling the self row 0x0074FCE0 defines.
//   The other local models are as at HEAD.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"
#include "fable_string.h"
struct CDefPointeeBase {
    virtual void v00();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
class CDefClassBase : public CDefPointeeBase { public: };
class CThingBaseDef : public CDefClassBase { public:
    unsigned char _pad08[0x42 - 0x08];
    unsigned char Flags42;                              // +0x42 (bit 1: persist the container's game state)
    bool PersistsGameState() const { return (Flags42 & 2) != 0; }
};
template <class T> class CDefPointer { public:     // the out slot (released through the holder below)
    CDefPointeeBase* Object;                            // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def taken out of the out pointer, released at the end
    CDefPointeeBase* Object;
    CDefHolder(CDefPointeeBase* object) : Object(object) { }
    ~CDefHolder() { if (Object && --Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef>
};
class CGameDefinitionManager : public CDefinitionManager
{
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingBaseDef>(long index, CDefPointer<const CThingBaseDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    bool IsGameStatePersisted() const;                  // 0x004C8280
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
};
class CTCBase
{
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};

class CTCContainer : public CTCBase { public:
    unsigned char _pad08[0x0c - 0x08];
    std::vector<std::pair<long, CCharString> > ContainerItems;   // +0x0c (Ego_r.pdb)
    FABLE_FORCEINLINE CDefPointeeBase* GetItemDef(long defIndex) const
    {
        CDefPointer<const CThingBaseDef> out;
        GetDefinitionManager().GetDef(defIndex, &out);
        return out.Object;
    }
    void InsertItem(long defIndex, const CCharString& name);
};

void CTCContainer::InsertItem(long defIndex, const CCharString& name)
{
    ContainerItems.push_back(std::pair<long, CCharString>(defIndex, name));
    if (!ParentThing->IsGameStatePersisted()) {
        CDefHolder<const CThingBaseDef> def(GetItemDef(defIndex));
        if (def->PersistsGameState())
            ParentThing->SetAsGameStatePersisted(true);
    }
}