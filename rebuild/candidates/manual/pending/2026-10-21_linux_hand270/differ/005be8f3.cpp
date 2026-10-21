// Grade: DIFFER(483v482) -- register rotation: retail thing=ebx (its 0 doubles as the pushed zero args), this=esi (spilled [ebp-0Ch]), ParentThing loaded into ecx after the name temp; ours hoists ParentThing into esi before the CCharString ctor (pushes literal 0s), this=edi, one extra frame slot. The IS_THING_ALIVE temp copy needs the CThingObject* -> CThing* conversion (fixed 489 -> 483). Tried: early return (484).
// NInventory::CTCInventoryBase::CreateInventoryObject(long defIndex) -- when the inventory holds an item of `defIndex`,
// create the thing at the owner's position and, if it lives, restore its bought record (the 0x48 component's two values,
// the augmentation slots -- grown to and filled up to the smaller of the record's and the component's count -- and the
// 0x39 component's value) and attach this inventory to its inventory-item component; returns the thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE8F3, 480 bytes, `ret 4` (frame pointer, `leave`).
// Callees: GetNumberOfItemsOfTypeInInventory 0x005BDF08, CThing::GetPosition 0x004C73D0, the thing factory 0x00703210,
// CCharString 0x0099EBF0 / 0x0099EAE0 on 0x0122D70E, IS_THING_ALIVE 0x0040DCE0, the bought-record lookup 0x005BE8D3
// (landed as GetBoughtPrice), bitset::test 0x00410DE0 / LowerBound 0x0040F020, CTCObjectAugmentations slot count
// 0x00765D90 / current count 0x00765D80 / add slot 0x00766D70 / AttachAugmentationToSlot 0x00768120 /
// RemoveAugmentationFromSlot 0x007681D0, the inventory item's owner setter 0x005D8E40.
// Layout: retail; the bought record: augmentation vector +0x00, values +0x0C/+0x10/+0x14 (names descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
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
    unsigned char _pad4c[0x90 - 0x4c];
    signed char   PlayerNumber;                             // +0x90
    const C3DVector* GetPosition() const;                   // 0x004C73D0
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
class CThingObject : public CThing {};
CThingObject* __fastcall CreateThing(long defIndex, const C3DVector* position, long player, CRightHandedSet* rhSet,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
bool __fastcall IS_THING_ALIVE(CThing* const& thing);      // 0x0040DCE0

struct SBoughtRecord {
    long* Augmentations_first;                              // +0x00 vector<long>
    long* Augmentations_last;
    long* Augmentations_end;
    long  ValueA;                                           // +0x0C
    long  ValueB;                                           // +0x10
    long  ValueC;                                           // +0x14
    long AugmentationCount() const { return Augmentations_last - Augmentations_first; }
};
struct CTCType48 {                                          // interface type 0x48
    unsigned char _pad00[0x1c];
    long ValueA;                                            // +0x1C
    long ValueB;                                            // +0x20
};
struct CTCType39 {                                          // interface type 0x39
    unsigned char _pad00[0x18];
    long ValueC;                                            // +0x18
};
struct CTCObjectAugmentations {                             // interface type 0xBF
    long GetNumberOfSlots();                                // 0x00765D90
    long GetNumberOfAugmentations();                        // 0x00765D80
    void AddSlot();                                         // 0x00766D70
    void AttachAugmentationToSlot(long augmentation, long slot);   // 0x00768120
    void RemoveAugmentationFromSlot(long slot);             // 0x007681D0
};
namespace NInventory { class CTCInventoryBase; }
struct CTCInventoryItem {                                   // interface type 0x1A
    void SetOwnerInventory(NInventory::CTCInventoryBase* inventory);   // 0x005D8E40
};

namespace NInventory {
class CTCInventoryBase {
public:
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    long GetNumberOfItemsOfTypeInInventory(long defIndex);  // 0x005BDF08
    SBoughtRecord* GetBoughtRecord(long defIndex);          // 0x005BE8D3
    CThing* CreateInventoryObject(long defIndex);
};

CThing* CTCInventoryBase::CreateInventoryObject(long defIndex)
{
    CThingObject* thing = 0;
    if (GetNumberOfItemsOfTypeInInventory(defIndex) > 0) {
        thing = CreateThing(defIndex, ParentThing->GetPosition(), ParentThing->PlayerNumber, 0, 0, CCharString(g_EmptyName, -1));
        if (IS_THING_ALIVE(thing)) {
            SBoughtRecord* record = GetBoughtRecord(defIndex);
            if (record) {
                if (thing->HasTC(0x48)) {
                    CTCType48* values = thing->FindTC<CTCType48>(0x48);
                    values->ValueA = record->ValueA;
                    values->ValueB = record->ValueB;
                }
                if (thing->HasTC(0xbf)) {
                    CTCObjectAugmentations* augmentations = thing->FindTC<CTCObjectAugmentations>(0xbf);
                    long slots = augmentations->GetNumberOfSlots();
                    long count = Min(record->AugmentationCount(), slots);
                    while (augmentations->GetNumberOfAugmentations() < count)
                        augmentations->AddSlot();
                    for (long i = 0; i < count; ++i) {
                        if (record->Augmentations_first[i] > 0)
                            augmentations->AttachAugmentationToSlot(record->Augmentations_first[i], i);
                        else
                            augmentations->RemoveAugmentationFromSlot(i);
                    }
                }
                if (thing->HasTC(0x39))
                    thing->FindTC<CTCType39>(0x39)->ValueC = record->ValueC;
            }
            CTCInventoryItem* item = 0;
            if (thing->HasTC(0x1a))
                item = thing->FindTC<CTCInventoryItem>(0x1a);
            item->SetOwnerInventory(this);
        }
    }
    return thing;
}
}
