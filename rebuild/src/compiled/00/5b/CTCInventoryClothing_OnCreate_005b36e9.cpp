// CTCInventoryClothing::OnCreate() -- registers the clothing screen, points the inventory at the clothing sub-def's inventory def, runs the base OnCreate, then creates every starting clothing item at the owner and adds it to this inventory
// Leaf: ?OnCreate@CTCInventoryClothing
// Retail 0x005B36E9, 270 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0 (global player GUI 0x013B8790),
// NHeroInformationScreens::CManager::RegisterScreen(const CBase*) 0x0066EB11, CDefClassBase::GetSubDef<T> 0x00485815,
// CTCInventoryBase::SetPDef(long) 0x005BDB4D, CTCInventoryBase::OnCreate 0x005C0121, this's slot 86 (+0x158, the
// starting item def list; name descriptive), CThing::GetPos() const 0x004C73D0, the thing factory 0x007FF7D0
// (__fastcall (def index, const C3DVector&); manifest "Create"), std::bitset::test 0x00410DE0 + CVectorMap::LowerBound
// 0x0040F020 (inline GetInterface for the inventory-item component 0x1A), CTCInventoryItem::AddToInventory(
// CTCInventoryBase*, bool, bool, long, bool, bool) 0x005D8D50, the defs' slot 1 on release.
// Shape: the def dance of the landed CTCInventoryWeapons::OnCreate 0x005C3057 (owner def add-ref'd/released by hand
// around GetSubDef into a zeroed out slot); the starting list is walked by index with the size re-read every pass;
// each created item is flagged (+0x20) and added silently; the sub-def is released at the end straight from its slot.
// Per pass the owner's position is fetched first (a named reference) and the def index is read from the list start
// cached by the size test (`_M_start[i]`): CThing::GetPos is LTCG-known to write nothing, so the start stays in
// edi across it. GetSubDef is a template instantiation in retail's TU, so the def slot does not escape and the final
// release is `dec [ecx+4] ; jne`. Same-TU stand-ins for both reproduce that.
// Layout (retail): NHeroInformationScreens::CBase secondary base +0x130; the clothing def's inventory-def index +0x7C;
// CTCInventoryItem flag +0x20.
#pragma optimize("y", off)
#pragma optimize("s", on)

class C3DVector { public: float X, Y, Z; };
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
struct CInventoryClothingDef : CDefPointeeBase {
    unsigned char _pad08[0x7c - 0x08];
    long InventoryDefIndex;                     // +0x7c
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CInventoryClothingDef>* out);   // 0x00485815
};
CInventoryClothingDef* FindSubDef_00485815(CDefClassBase* defClass);   // the sub-def, add-ref'd (0 when absent)
// Stand-in for CDefClassBase::GetSubDef<T> @ 0x00485815: same TU, writes the out slot only on success, so the caller's
// def slot does not escape.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CInventoryClothingDef>* out)
{
    CInventoryClothingDef* def = FindSubDef_00485815(this);
    if (def == 0)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
struct CPresenceBits {
    bool test(long bit);                        // 0x00410DE0 (std::bitset::test)
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                    // +0x20
    unsigned char _pad21[0x23];
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    struct CPlacement { unsigned char _pad00[0xc]; C3DVector Position; }* PPlacement;   // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)((void*)((it == map->last || wanted < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateThing_007ff7d0(long defIndex, const C3DVector& position);   // 0x007FF7D0
// Stand-in for CThing::GetPos() const @ 0x004C73D0 (the retail body; same TU so the caller sees it writes nothing).
#pragma auto_inline(off)
__declspec(noinline) const C3DVector& CThing::GetPos() const
{
    return PPlacement->Position;
}
#pragma auto_inline(on)

namespace NHeroInformationScreens {
class CBase { public:                                  // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
};
class CManager { public:
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long operator[](unsigned long n) const { return *(_M_start + n); }
};

struct CTCInventoryBase_Clothing;
class CTCInventoryItem { public:                       // interface type 0x1A
    unsigned char _pad00[0x20];
    bool StartingItem;                          // +0x20 (descriptive)
    bool AddToInventory(CTCInventoryBase_Clothing* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
};

struct CTCInventoryBase_Clothing {              // retail 0x130 bytes
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                    // slot 4
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual const CLongVector& GetStartingItems();   // slot 86 (+0x158)
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
class CTCInventoryClothing : public CTCInventoryBase_Clothing, public NHeroInformationScreens::CBase {
public:
    virtual void OnCreate();
};

void CTCInventoryClothing::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CInventoryClothingDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    SetPDef(def->InventoryDefIndex);
    CTCInventoryBase_Clothing::OnCreate();
    const CLongVector& startingItems = GetStartingItems();
    for (unsigned long i = 0; i < startingItems.size(); ++i) {
        const C3DVector& pos = ParentThing->GetPos();
        CThing* thing = CreateThing_007ff7d0(startingItems._M_start[i], pos);
        if (thing) {
            CTCInventoryItem* item;
            if (thing->GetInterface(&item, 0x1a)) {
                item->StartingItem = true;
                item->AddToInventory(this, false, true, 0, true, false);
            }
        }
    }
    if (--def->m_refCount == 0)
        def->Destroy();
}