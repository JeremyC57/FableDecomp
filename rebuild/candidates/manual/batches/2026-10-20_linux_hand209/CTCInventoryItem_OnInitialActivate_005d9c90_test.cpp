// Behaviour test for CTCInventoryItem::OnInitialActivate @ 0x005D9C90 (model copy; stub thing manager, def and inventory lookup).
#include <cstdio>
static long g_aborts = 0;
static int puts_stub(const char*) { return 0; }
static void abort_stub() { ++g_aborts; }
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++destroyed; }
    long m_refCount; static long destroyed;
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
long CDefPointeeBase::destroyed = 0;
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* p) : Object(p) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
struct CInventoryItemDef : CDefPointeeBase { unsigned char _pad08[0x50 - 0x08]; long InventoryType; };
struct CPresenceBits {
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) { puts_stub("bitset"); abort_stub(); return false; }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present; CInterfaceMap m_interfaces;
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char Dead : 1; unsigned char _91_1 : 7;
    template<class T> bool GetInterface(T** out, long wanted) {
        if (m_present.test(wanted)) {
            long type = wanted; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component; return true;
        }
        return false;
    }
};
struct CThingManager { CThing* things[4]; CThing* GetThingWithUID(unsigned long long uid) { return uid < 4 ? things[(long)uid] : 0; } };
struct CTCInventoryBase { long id; };
struct CTCBase_Item {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void OnInitialActivate() { ++baseActivates; }
    CThing* ParentThing; long field8; CThingManager* manager; long baseActivates;
    CThingManager* GetThingManager() { return manager; }
};
struct CTCInventoryItem : CTCBase_Item {
    CDefPointer<CInventoryItemDef> PDef; CTCInventoryBase* PInventory; long call; unsigned long long InventoryUIDToAttachTo; bool DefaultItem;
    CTCInventoryBase* added; long adds; bool addArgs[5]; long addC;
    CTCInventoryItem(CInventoryItemDef* def) : PDef(def), adds(0), added(0), baseActivatesDummy(0) {}
    long baseActivatesDummy;
    CDefPointer<CInventoryItemDef> GetPDef() const { return PDef; }
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool levelPersistent) { ++adds; added = inventory; addArgs[0] = a; addArgs[1] = b; addC = c; addArgs[2] = d; addArgs[3] = levelPersistent; return true; }
    virtual void OnInitialActivate();
};
void CTCInventoryItem::OnInitialActivate()
{
    CThing* owner;
    long inventoryType;
    if (PInventory == 0 && InventoryUIDToAttachTo > 0) {
        owner = GetThingManager()->GetThingWithUID(InventoryUIDToAttachTo);
        if (owner && !owner->Dead) {
            inventoryType = GetPDef()->InventoryType;
            CTCInventoryBase* inventory;
            if (owner->GetInterface(&inventory, inventoryType))
                AddToInventory(inventory, false, true, 0, true, false);
        }
    }
    CTCBase_Item::OnInitialActivate();
}
int main()
{
    CInventoryItemDef def; def.m_refCount = 1; def.InventoryType = 0x40;
    CTCInventoryBase inventory; inventory.id = 1;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x40, &inventory } };
    CThing owner; owner.Dead = 0;
    for (int i = 0; i < 9; ++i) owner.m_present.words[i] = 0;
    owner.m_present.words[0x40 >> 5] |= 1UL << (0x40 & 31);
    owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 2;
    CThingManager manager; manager.things[0] = 0; manager.things[1] = &owner; manager.things[2] = 0; manager.things[3] = &owner;
    CTCInventoryItem item(&def); item.manager = &manager; item.baseActivates = 0; item.PInventory = 0; item.InventoryUIDToAttachTo = 1;
    bool ok = true;
    item.OnInitialActivate();                                   // attaches to the owner's inventory
    if (item.adds != 1 || item.added != &inventory || item.addArgs[0] || !item.addArgs[1] || item.addC != 0 || !item.addArgs[2] || item.addArgs[3]) ok = false;
    if (item.baseActivates != 1 || def.m_refCount != 1 || CDefPointeeBase::destroyed != 0) ok = false;
    item.PInventory = &inventory; item.OnInitialActivate();     // already in an inventory: only the base hook
    if (item.adds != 1 || item.baseActivates != 2) ok = false;
    item.PInventory = 0; item.InventoryUIDToAttachTo = 0; item.OnInitialActivate();   // no owner UID
    if (item.adds != 1 || item.baseActivates != 3) ok = false;
    item.InventoryUIDToAttachTo = 2; item.OnInitialActivate(); // UID of a missing thing
    if (item.adds != 1 || item.baseActivates != 4) ok = false;
    owner.Dead = 1; item.InventoryUIDToAttachTo = 1; item.OnInitialActivate();   // dead owner
    if (item.adds != 1 || item.baseActivates != 5) ok = false;
    owner.Dead = 0; def.InventoryType = 0x10; item.OnInitialActivate();          // type not present on the owner
    if (item.adds != 1 || item.baseActivates != 6 || g_aborts != 0) ok = false;
    def.InventoryType = 0x40; item.InventoryUIDToAttachTo = 0x100000000ull + 3; item.OnInitialActivate();   // high word set counts as a UID; thing 3 (index out of the stub) -> none
    if (item.adds != 1 || item.baseActivates != 7) ok = false;
    if (!ok) { std::printf("005d9c90_TEST FAIL\n"); return 1; }
    std::printf("005d9c90_TEST PASS\n");
    return 0;
}
