// CTCInventory::UseQuickAccessItem(long) -- uses the item in a quick-access slot: the carried thing if it is that item, else the inventory item
// Leaf: UseQuickAccessItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005840A6, 177 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//          CTCCarrying::GetThingInPrimarySlot() @ 0x006A1440, IS_THING_ALIVE(CBaseObject* const&) @ 0x0040DCE0
//          (__fastcall), and this class's own virtuals OnItemSelectedInHand(CThing*, bool*) (slot 34, 0x0058400A) and
//          OnItemSelected(long, bool*) (slot 33, 0x00583E26) from vtable 0x01250DE4.
// Shape: the carrying component (interface 0x46) comes from the `__forceinline` GetTC helper with a zeroed out
// pointer (`and [ebp-4],0`, the key `push 46h ; pop ebx` reused for the compare), fetched before the slot test.
// The slot is read with the STLport `*(begin() + slot)` form (scaled index kept in edi, begin re-read). The
// thing in hand stays in ebx: IS_THING_ALIVE takes `CBaseObject* const&`, so the CThing* binds through a converted
// temporary homed in the dead `slot` argument slot (as in the landed CTCVillage::DetachFromDummyVillager 0x0065B2C4) and matched on
// its 16-bit def index (+0x8C). Each branch declares its own `handled` out-flag (both in that slot's high byte): one
// flag declared above the `if` overlaps the condition's temporary, which then takes the dead component word [ebp-4].
// Layout: retail CTCInventory quick-access vector<long> at +0x148 (PDB +0x168; engine/CTCInventory.h carries the
// 16-byte PDB containers, so `this` is a local model as in the landed IsQuickAccessItemUsable 0x005849EA).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CBaseObject { public:
};

class CThing : public CBaseObject { public:
    unsigned char  _pad_0x00[0x20];
    CPresenceBits  m_present;                        // +0x20
    unsigned char  _pad_0x21[0x23];
    CInterfaceMap  m_interfaces;                     // +0x44
    unsigned char  _pad_0x4c[0x40];
    unsigned short DefIndex;                         // +0x8C
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component = 0;
        GetInterface(&component, wanted);
        return component;
    }
};

bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0

class CTCCarrying { public:                                 // interface type 0x46
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

struct CLongVector {                                 // std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    const long& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInventory { public:                                // vtable 0x01250DE4
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void OnItemSelected(long item, bool* handled);           // slot 33 (0x00583E26)
    virtual void OnItemSelectedInHand(CThing* thing, bool* handled); // slot 34 (0x0058400A)

    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x140];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)

    void UseQuickAccessItem(long slot);
};

void CTCInventory::UseQuickAccessItem(long slot)
{
    CTCCarrying* carrying = ParentThing->GetTC<CTCCarrying>(0x46);
    if (QuickAccessItems[slot] > 0) {
        CThing* inHand = carrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(inHand) && inHand->DefIndex == QuickAccessItems[slot]) {
            bool handled;
            OnItemSelectedInHand(inHand, &handled);
        }
        else {
            bool handled;
            OnItemSelected(QuickAccessItems[slot], &handled);
        }
    }
}