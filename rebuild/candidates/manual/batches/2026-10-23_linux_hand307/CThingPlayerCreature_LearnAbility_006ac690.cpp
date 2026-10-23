// CThingPlayerCreature::LearnAbility(EHeroAbility ability, bool learn, long quickAccessSlot, bool silent) -- learning: unless already available, spawn the ability's inventory object at the hero, add it to the inventory, mark the ability available and give it a quick-access slot; unlearning: mark it unavailable and take the matching object out of the inventory
// Leaf: ?LearnAbility@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC690, length 628 bytes (`ret 0x10`).
// Callees: LowerBound @ 0x0040F020, CTCInventoryAbilities::IsAbilityAvailable @ 0x005EDF08 / GetAbilityPDef @ 0x005EDFD9
//   (CDefPointer by value) / SetAbilityAsAvailable @ 0x005F6BD6, CCharString(const char*, long) @ 0x0099EBF0 / dtor
//   @ 0x0099EAE0 ("" @ 0x0122D70E), CThing::GetPosition @ 0x004C73D0, the __fastcall thing factory @ 0x00703210,
//   CTCInventoryItem::AddToInventory @ 0x005D8D50 / RemoveFromInventory @ 0x005D8E20, CTCInventory::
//   GetQuickAccessNextFreeSlot @ 0x00582027 / SetThingAsQuickAccessItem @ 0x00584157, the inventory's virtual slot
//   47 (+0xbc) finding the thing of a def.
// Layout (retail): CThing presence bitset +0x20, component map +0x44, def index +0x8c, player number +0x90, Dead +0x91
//   bit 0; CHeroAbilityDef inventory object +0x98; interfaces 0x11 inventory, 0x1a inventory item, 0x6f abilities.
// Shape: the def pointer returned by value is homed in the dead `learn` slot; its release is the null-tested
//   CDefPointer dtor calling the pointee's inline ReleaseTemporary() (DecRef / GetRefCount).
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CHeroAbilityDef : CDefPointeeBase {
    unsigned char _pad08[0x98 - 0x08];
    long InventoryObject;                               // +0x98
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing;
struct CTCInventory {
    virtual void v00();
    long GetQuickAccessNextFreeSlot(long item);                         // 0x00582027
    void SetThingAsQuickAccessItem(long item, long slot, bool a);       // 0x00584157
};
struct CTCInventoryItem {
    bool AddToInventory(CTCInventory* inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
    void RemoveFromInventory();                                         // 0x005D8E20
};
struct CTCInventoryAbilities {
    bool IsAbilityAvailable(EHeroAbility ability);                      // 0x005EDF08
    CDefPointer<CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability);  // 0x005EDFD9
    void SetAbilityAsAvailable(EHeroAbility ability, bool available, bool a);   // 0x005F6BD6
};
struct CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210

class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned char _pad8e[0x90 - 0x8e];
    signed char PlayerNumber;                           // +0x90
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (HasTC(type)) {
            *out = (T*)GetTC(type);
            return true;
        }
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CTCInventoryFind : CTCInventory {
    // slot 47 (+0xbc): the first inventory thing of a def
};
__forceinline CThing* CreateThingObject(long defIndex, const CThing* creature, const CCharString& name)
{
    return CreateThingObject(defIndex, &creature->GetPosition(), creature->PlayerNumber, 0, 0, name);
}
struct CTCInventoryVirtual {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual CThing* GetThingOfType(long defIndex);      // slot 47 (+0xbc)
};
__forceinline CThing* FindThing(CTCBase* inventory, long object)
{
    return ((CTCInventoryVirtual*)inventory)->GetThingOfType(object);
}
class CThingPlayerCreature : public CThing {
public:
    bool LearnAbility(EHeroAbility ability, bool learn, long quickAccessSlot, bool silent);
};

bool CThingPlayerCreature::LearnAbility(EHeroAbility ability, bool learn, long quickAccessSlot, bool silent)
{
    CTCInventoryAbilities* abilities;
    if (GetInterface(&abilities, 0x6f)) {
        if (learn) {
            if (!abilities->IsAbilityAvailable(ability)) {
                CDefPointer<CHeroAbilityDef> def = abilities->GetAbilityPDef(ability);
                CThing* item;
                {
                    CCharString name("");
                    item = CreateThingObject(def->InventoryObject, this, name);
                }
                if (item) {
                    CTCInventory* inventory = 0;
                    if (HasTC(0x11)) {
                        inventory = (CTCInventory*)GetTC(0x11);
                        CTCInventoryItem* itemTC = 0;
                        if (item->HasTC(0x1a))
                            itemTC = (CTCInventoryItem*)item->GetTC(0x1a);
                        itemTC->AddToInventory(inventory, false, true, 0, silent, false);
                    }
                    abilities->SetAbilityAsAvailable(ability, true, true);
                    if (quickAccessSlot != -1 || (quickAccessSlot = inventory->GetQuickAccessNextFreeSlot(item->m_defGlobalIndex)) != -1)
                        inventory->SetThingAsQuickAccessItem(item->m_defGlobalIndex, quickAccessSlot, false);
                    return true;
                }
            }
        } else {
            if (abilities->IsAbilityAvailable(ability)) {
                abilities->SetAbilityAsAvailable(ability, false, true);
                CDefPointer<CHeroAbilityDef> def = abilities->GetAbilityPDef(ability);
                if (HasTC(0x11)) {
                    CTCBase* inventory = GetTC(0x11);
                    CThing* item = FindThing(inventory, def->InventoryObject);
                    if (item && !item->Dead) {
                        CTCInventoryItem* itemTC = 0;
                        if (item->HasTC(0x1a))
                            itemTC = (CTCInventoryItem*)item->GetTC(0x1a);
                        itemTC->RemoveFromInventory();
                    }
                }
            }
        }
    }
    return false;
}
