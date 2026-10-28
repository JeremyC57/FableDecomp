// CTCInventoryItem::CacheDef(long defIndex) -- static: make the def cache hold defIndex: true at once when it already does; else fetch the thing def from the game definition manager and its CInventoryItemDef sub-def, and when both exist store the index and both counted pointers in the cache and return true (false otherwise)
// Leaf: ?CacheDef@CTCInventoryItem@@CI_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D94B0, 270 bytes, private static `__fastcall` (defIndex in ecx), `ret`.
// Identity (proven): Ego_r.pdb ?CacheDef@CTCInventoryItem@@CI_NJ@Z (private static __fastcall bool(long)); its callers are
//   the static CTCInventoryItem getters of this TU (IsWeapon 0x005D97F0, GetContextSensitiveType 0x005D9B90, ...), all
//   of which pass their def index straight through in ecx, and ClearCachedDefs 0x005D9320 empties the same three globals.
// Callees: CGameDefinitionManager::Get() 0x0044C6B0 (static __fastcall, Ego_r.pdb ?Get@CGameDefinitionManager@@SIAAV1@XZ),
//   CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const 0x0042AF3C,
//   CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<const CInventoryItemDef>*) const 0x005BADE1 (both Ego_r.pdb
//   template spellings), the pointee's virtual Delete (slot 1) on the last release.
// Globals: CTCInventoryItem's static def cache -- 0x013B8BC8 the cached def index, 0x013B8C14 the cached thing def,
//   0x013B8C18 the cached inventory-item def (descriptive member names, as the landed ClearCachedDefs binds them; data
//   symbols are not in the PDB name sets).
// Shape: the two out-pointer locals share the one `push ecx` frame word: the GetDef / GetSubDef stand-in bodies below
//   (same TU, noinline, real names) only fill their out slot, so VC keeps the thing def in edi once read and reuses the
//   word for the sub-def. The cache stores are the CDefPointer assignment (null-safe release, then add-ref).
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1
    mutable unsigned long IVRefCount;                       // +0x04
    void AddRef() const { ++IVRefCount; }
    long GetRefCount() const { return (long)IVRefCount; }
    long Release() const
    {
        long result = (long)--IVRefCount;
        if (result == 0)
            Delete();
        return result;
    }
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResourceList* ResourceList;
    CResource*     PrevResource;
    CResource*     NextResource;
    unsigned long  ResourceSize;
    unsigned long  LastUsedFrame;
    virtual ~CResource();
};
class CDefPointeeBase : public CResource {
public:
};
enum ENullDef { NULL_DEF = 0 };
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer(ENullDef = NULL_DEF) : Object(0) { }
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
    T* operator->() const { return (T*)Object; }
};
class CDefClassBase : public CDefPointeeBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005BADE1 for CInventoryItemDef
};
class CThingBaseDef : public CDefClassBase { };
class CInventoryItemDef : public CDefClassBase { };
class CDefinitionManager {
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C for CThingBaseDef
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
};
// Same-TU stand-ins for the two retail template COMDATs (0x0042AF3C, 0x005BADE1): they only fill the out slot, which
// lets VC see that the out-pointer locals are not captured (layout-only bodies; the retail ones are linked).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    out->Object = (CDefPointeeBase*)(this + index);
    return out->Object != 0;
}
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    out->Object = (CDefPointeeBase*)this;
    return out->Object != 0;
}
#pragma auto_inline(on)

class CTCInventoryItem {
private:
    static bool __fastcall CacheDef(long defIndex);
public:
    static long                                  s_CachedDefIndex;      // 0x013B8BC8
    static CDefPointer<const CThingBaseDef>      s_CachedThingDef;      // 0x013B8C14
    static CDefPointer<const CInventoryItemDef>  s_CachedItemDef;       // 0x013B8C18
};

bool __fastcall CTCInventoryItem::CacheDef(long defIndex)
{
    if (defIndex > 0 && defIndex == s_CachedDefIndex)
        return true;
    CDefPointer<const CThingBaseDef> thingDef;
    if (CGameDefinitionManager::Get().GetDef(defIndex, &thingDef)) {
        CDefPointer<const CInventoryItemDef> itemDef;
        if (thingDef->GetSubDef(&itemDef)) {
            s_CachedDefIndex = defIndex;
            s_CachedThingDef = thingDef;
            s_CachedItemDef = itemDef;
            return true;
        }
    }
    return false;
}