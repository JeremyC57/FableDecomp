// CTCInventoryBase::GetResurrectionItemInInventory() -- walk every inventory item; for each whose def has a resurrection sub-def, create its object and return it when it carries interface 0xE1, otherwise poke its interface-0x1A component and go on; null when none
// Leaf: GetResurrectionItemInInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF77D, 334 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)`.
// Callees: CTCInventoryBase::GetAllItems(vector<NInventory::CItem>&) @ 0x005BF482 (landed), CTCBase::GetDefinitions
//   @ 0x006869D0, CDefinitionManager::GetDef<CThingDef> @ 0x0042AF3C, CDefClassBase::GetSubDef<CResurrectionItemDef>
//   @ 0x005C0CFB (landed), CTCInventoryBase::CreateInventoryObject(long) @ 0x005BE8F3, std::bitset<>::test @ 0x00410DE0
//   and CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x1A), the 0x1A
//   component's 0x005D8E50, the CRT free thunk @ 0x00BFEA14 (the item vector's storage), the defs' virtual slot 1.
// Shape: same-TU `auto_inline(off)` stand-ins for GetDef / GetSubDef (as 0x00801A50) keep the thing def in edi, and one
//   for GetAllItems (retail 0x005BF482 is a TU-mate, landed) tells VC7.1 the item vector does not escape, so the element
//   address is formed once per pass and homed; the count is read once before the loop; the def-pointer releases are
//   `__forceinline`; the item vector's destructor frees its storage with free().
// Layout: NInventory::CItem is 8 bytes (def index first); retail CThing bitset +0x20, interface map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CTCBase;
struct CPresenceBits { bool test(long kind); };            // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) { return present.test(type); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInterface1A {
    void Notify();                                          // 0x005D8E50
};

struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() { Object = 0; }
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CResurrectionItemDef : CDefClassBase { };
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CResurrectionItemDef>* out);    // 0x005C0CFB
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingDef>* out);   // 0x0042AF3C
};

// Same-TU stand-ins (see the Shape note).
CDefClassBase* LookUpDef(long index);
CDefClassBase* LookUpSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out)
{
    CThingDef* def = (CThingDef*)LookUpDef(index);
    out->Object = def;
    return def != 0;
}
bool CThingDef::GetSubDef(CDefPointer<CResurrectionItemDef>* out)
{
    CResurrectionItemDef* def = (CResurrectionItemDef*)LookUpSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

namespace NInventory {
struct CItem {
    long DefIndex;                                          // +0x00
    long Count;                                             // +0x04
};
}
struct CItemVector {                                        // STLport vector<NInventory::CItem> (12 bytes)
    NInventory::CItem* _M_start;
    NInventory::CItem* _M_finish;
    NInventory::CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    NInventory::CItem& operator[](unsigned long n) { return _M_start[n]; }
};

struct CTCInventoryBase {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase

    CDefinitionManager* GetDefinitions() const;             // 0x006869D0 (CTCBase)
    void GetAllItems(CItemVector& items);                   // 0x005BF482
    CThing* CreateInventoryObject(long defIndex);           // 0x005BE8F3
    CThing* GetResurrectionItemInInventory();
};

NInventory::CItem* LookUpItems(const CTCInventoryBase* inventory, unsigned long* count);
#pragma auto_inline(off)
void CTCInventoryBase::GetAllItems(CItemVector& items)
{
    unsigned long count;
    items._M_start = LookUpItems(this, &count);
    items._M_finish = items._M_start + count;
}
#pragma auto_inline(on)

CThing* CTCInventoryBase::GetResurrectionItemInInventory()
{
    CItemVector items;
    GetAllItems(items);
    unsigned long count = items.size();
    for (unsigned long i = 0; i < count; ++i) {
        NInventory::CItem& item = items[i];
        CDefPointer<CThingDef> thingDef;
        GetDefinitions()->GetDef(item.DefIndex, &thingDef);
        CDefPointer<CResurrectionItemDef> resurrectionDef;
        if (thingDef->GetSubDef(&resurrectionDef)) {
            CThing* object = CreateInventoryObject(item.DefIndex);
            if (object->HasTC(0xe1))
                return object;
            CTCInterface1A* component = 0;
            object->GetInterface(&component, 0x1a);
            component->Notify();
        }
    }
    return 0;
}
