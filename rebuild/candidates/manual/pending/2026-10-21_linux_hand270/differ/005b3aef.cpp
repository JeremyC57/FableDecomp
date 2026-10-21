// Grade: DIFFER(438v422) -- retail keeps this memory-only ([ebp-10h]) and caches the starting-items def in edi (reloaded only after AddToInventory), and skips the sub-def release when GetDef fails; ours keeps this in edi plus a spill and reloads the def each pass. Tried: member Release (dec/jne form), named raw def pointer (453, worse).
// CTCInventoryClothing::OnInitialActivate() -- on a fresh (not save-loaded) start, give the owner every starting item of
// its def that is a clothing item: create the thing at the owner's position, add it to this inventory (flagged) and mark
// it, or kill it when it has no inventory-item component; then the base activation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B3AEF, 422 bytes, plain `ret` (frame pointer, `leave`).
// Callees: the owner def's GetSubDef (starting-items def) 0x00485815, CTCBase::GetDefinitions 0x006869D0,
// CDefinitionManager::GetDef (item def) 0x0042B040, the item def's GetSubDef 0x0057FF92, CThing::GetPosition 0x004C73D0,
// the thing factory 0x00703210 (__fastcall (def index, position, player, rh set, flags, name)), CCharString 0x0099EBF0 /
// 0x0099EAE0 on 0x0122D70E, bitset::test 0x00410DE0 / LowerBound 0x0040F020 (interface 0x1A), CTCInventoryItem::
// AddToInventory 0x005D8D50, CThing::Kill(bool) 0x004C9B80, CTCInventoryBase::OnInitialActivate 0x005BD1EB.
// Layout: retail CTCInventoryClothing LoadedFromSaveGame +0x15D (as the landed clothing bodies); CThing def +0x70, player
// +0x90 (signed char), flag bits +0x91; the starting-items def's item vector +0x54; the item sub-def's kind +0x50;
// CTCInventoryItem flag +0x20 (retail offsets, names descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long m_refCount;                                        // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CStartingItemsDef : CDefPointeeBase {
    unsigned char _pad08[0x54 - 0x08];
    long* Items_first;                                      // +0x54 vector<long>
    long* Items_last;
    long* Items_end;
    unsigned long ItemCount() const { return Items_last - Items_first; }
};
struct CItemSubDef : CDefPointeeBase {
    unsigned char _pad08[0x50 - 0x08];
    long Kind;                                              // +0x50
};
struct CItemDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CItemSubDef>* out) const;    // 0x0057FF92
};
struct CThingDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CStartingItemsDef>* out) const;   // 0x00485815
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CItemDef>* out);    // 0x0042B040
};

class CTCBase;
struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
extern const char g_EmptyName[];                            // 0x0122D70E
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CThingDef> PDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char   PlayerNumber;                             // +0x90
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char Persistent : 1;                           // +0x91 bit 1
    __forceinline CDefPointer<CThingDef> GetDef() const { return PDef; }
    const C3DVector* GetPosition() const;                   // 0x004C73D0
    void Kill(bool removeImmediately);                      // 0x004C9B80
    __forceinline bool HasTC(long type) { return present.test(type); }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector* position, long player, CRightHandedSet* rhSet,
                               unsigned long flags, const CCharString& name);   // 0x00703210

namespace NInventory { class CTCInventoryBase; }
struct CTCInventoryItem {                                   // interface type 0x1A
    unsigned char _pad00[0x20];
    bool          GivenAtStart;                             // +0x20
    bool AddToInventory(NInventory::CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
};

namespace NInventory {
class CTCInventoryBase {
public:
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    CDefinitionManager* GetDefinitions() const;             // 0x006869D0 (CTCBase)
    void OnInitialActivate();                               // 0x005BD1EB
};
}

class CTCInventoryClothing : public NInventory::CTCInventoryBase {
public:
    unsigned char _pad08[0x15d - 0x08];
    bool          LoadedFromSaveGame;                       // +0x15D
    void OnInitialActivate();
};

void CTCInventoryClothing::OnInitialActivate()
{
    if (!LoadedFromSaveGame) {
        CDefPointer<CStartingItemsDef> startingItems;
        if (ParentThing->GetDef()->GetSubDef(&startingItems)) {
            for (unsigned long i = 0; i < startingItems->ItemCount(); ++i) {
                long item = startingItems->Items_first[i];
                CDefPointer<CItemDef> itemDef;
                CDefPointer<CItemSubDef> subDef;
                if (GetDefinitions()->GetDef(item, &itemDef)) {
                    if (itemDef->GetSubDef(&subDef) && subDef->Kind == 2) {
                        CThing* thing = CreateThing(item, ParentThing->GetPosition(), ParentThing->PlayerNumber, 0, 0, CCharString(g_EmptyName, -1));
                        if (thing->HasTC(0x1a)) {
                            CTCInventoryItem* inventoryItem = thing->FindTC<CTCInventoryItem>(0x1a);
                            inventoryItem->GivenAtStart = true;
                            inventoryItem->AddToInventory(this, true, true, 0, true, false);
                            if (!thing->Dead)
                                thing->Persistent = true;
                        } else
                            thing->Kill(false);
                    }
                }
            }
        }
    }
    NInventory::CTCInventoryBase::OnInitialActivate();
}
