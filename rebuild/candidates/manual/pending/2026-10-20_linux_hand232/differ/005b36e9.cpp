// Grade: DIFFER(280v270) -- the starting-item loop: retail keeps the list start (CSE'd from the size test) in edi across GetPosition and reads the element after it (`mov ecx,[edi+eax*4]`), ours reads the element first and holds it in edi; the final `--def->m_refCount` release is the long eax form (retail `mov ecx,[ebp-8] ; dec [ecx+4]`). Tried: by-value accessor, named position reference (276v270, register map worse), direct start index (276v270).
// CTCInventoryClothing::OnCreate() -- registers the clothing screen, points the inventory at the clothing sub-def's inventory def, runs the base OnCreate, then creates every starting clothing item at the owner and adds it to this inventory
// Leaf: ?OnCreate@CTCInventoryClothing
// Retail 0x005B36E9, 270 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0 (global player GUI 0x013B8790),
// NHeroInformationScreens::CManager::RegisterScreen(const CBase*) 0x0066EB11, CDefClassBase::GetSubDef<T> 0x00485815,
// CTCInventoryBase::SetPDef(long) 0x005BDB4D, CTCInventoryBase::OnCreate 0x005C0121, this's slot 86 (+0x158, the
// starting item def list; name descriptive), CThing::GetPosition() const 0x004C73D0, the thing factory 0x007FF7D0
// (__fastcall (def index, const C3DVector&); manifest "Create"), std::bitset::test 0x00410DE0 + CVectorMap::LowerBound
// 0x0040F020 (inline GetInterface for the inventory-item component 0x1A), CTCInventoryItem::AddToInventory(
// CTCInventoryBase*, bool, bool, long, bool, bool) 0x005D8D50, the defs' slot 1 on release.
// Shape: the def dance of the landed CTCInventoryWeapons::OnCreate 0x005C3057 (owner def add-ref'd/released by hand
// around GetSubDef into a zeroed out slot); the starting list is walked by index with the size re-read every pass;
// each created item is flagged (+0x20) and added silently; the sub-def is released at the end straight from its slot.
// Layout (retail): NHeroInformationScreens::CBase secondary base +0x130; the clothing def's inventory-def index +0x7C;
// CTCInventoryItem flag +0x20.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct C3DVector { float X, Y, Z; };
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CInventoryClothingDef : CDefPointeeBase {
    unsigned char _pad08[0x7c - 0x08];
    long InventoryDefIndex;                     // +0x7c
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CInventoryClothingDef>* out);   // 0x00485815
};
struct CPresenceBits {
    bool test(long bit);                        // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                    // +0x20
    unsigned char _pad21[0x23];
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position);   // 0x007FF7D0

namespace NHeroInformationScreens {
struct CBase {                                  // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
};
struct CManager {
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
struct CPlayerGui {
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
struct CTCInventoryItem {                       // interface type 0x1A
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
        CThing* thing = CreateThing(startingItems[i], ParentThing->GetPosition());
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
