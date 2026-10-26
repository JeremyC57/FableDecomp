// CTCInventoryItem::OnInitialActivate() -- an item created outside an inventory but tagged with an inventory owner's UID attaches itself: finds the live owner thing, looks up its inventory component of the def's InventoryType and adds the item to it; then the CTCBase hook
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D9C90, length 215 bytes; speed TU (no frame pointer). CTCInventoryItem vtable 0x01240004 slot 6 (OnInitialActivate).
// Callees: CTCBase::GetThingManager() @ 0x00686AE0, CThingManager::GetThingWithUID(unsigned __int64) @ 0x0051ED80 (manifest "CStopWatch::GetTicks"
// is propagated), the inlined std::bitset<274>::test with its range check (`puts("bitset")` @ 0x00BFEAF2 then `abort` @ 0x00BFEAEC, the
// exception-less STLport throw) over the thing's presence words at +0x20, CVectorMap::LowerBound @ 0x0040F020 on the interface map at
// +0x44, CTCInventoryItem::AddToInventory(CTCInventoryBase*, bool, bool, long, bool, bool) @ 0x005D8D50, CTCBase::OnInitialActivate
// @ 0x00686870 (manifest "$E2" is propagated), the def's virtual slot 1 on release.
// Layout: engine/CTCInventoryItem.h (PDef_Object +0x0C, PInventory +0x10, InventoryUIDToAttachTo +0x18); CInventoryItemDef::InventoryType
// at +0x50 (PDB offsets); CThing Dead is +0x91 bit 0 (PDB +0xA1).
// Shape: the owner thing and the inventory type are function-scope locals (esi/edi are pushed at entry, not lazily inside
// the blocks); the 64-bit UID is tested as `> 0` (hi word `ja`, low word `jbe`) and pushed as two words; the def's inventory type is read
// through a by-value def temporary (add-ref, read, `mov/dec/mov ; jne` release) into edi, which is both the bitset position and the
// map key homed in the frame word; the found component is pushed straight into AddToInventory's first slot.
extern "C" int  __cdecl puts(const char* text);    // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC (noreturn as in <stdlib.h>: the throw path tail-jumps)
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
class CInventoryItemDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x50 - 0x08];
    long InventoryType;                            // +0x50 ETCInterfaceType
};
struct CPresenceBits {                             // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                 // +0x00
    SEntry* last;                                  // +0x04
    SEntry* LowerBound(const long* type);          // 0x0040F020
};
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                       // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
class CThingManager { public:
    CThing* GetThingWithUID(unsigned __int64 uid);   // 0x0051ED80
};
struct CTCInventoryBase;
struct CTCBase_Item {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void OnInitialActivate();              // slot 6 @ 0x00686870
    CThing*       ParentThing;                     // +0x04
    long          field8;                          // +0x08
    CThingManager* GetThingManager();              // 0x00686AE0
};
#pragma pack(push, 4)
class CTCInventoryItem : public CTCBase_Item { public:
    CDefPointer<CInventoryItemDef> PDef;           // +0x0c
    CTCInventoryBase* PInventory;                  // +0x10
    long              call;                        // +0x14
    unsigned __int64  InventoryUIDToAttachTo;      // +0x18
    bool              DefaultItem;                 // +0x20
    CDefPointer<CInventoryItemDef> GetPDef() const { return PDef; }
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool levelPersistent);   // 0x005D8D50
    virtual void OnInitialActivate();
};
#pragma pack(pop)
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