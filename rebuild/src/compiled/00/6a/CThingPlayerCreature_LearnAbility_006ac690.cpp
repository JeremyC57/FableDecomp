// CThingPlayerCreature::LearnAbility(EHeroAbility ability, bool learn, long quickAccessSlot, bool silent) -- learning: unless already available, spawn the ability's inventory object at the hero, add it to the inventory, mark the ability available and give it a quick-access slot; unlearning: mark it unavailable and take the matching object out of the inventory
// Leaf: ?LearnAbility@CThingPlayerCreature@@QAE_NW4EHeroAbility@@_NJ1@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC690, length 628 bytes (`ret 0x10`).
// Callees: LowerBound @ 0x0040F020, CTCInventoryAbilities::IsAbilityAvailable @ 0x005EDF08 / GetAbilityPDef @ 0x005EDFD9
//   (CDefPointer by value) / SetAbilityAsAvailable @ 0x005F6BD6, CCharString(const char*, long) @ 0x0099EBF0 / dtor
//   @ 0x0099EAE0 ("" @ 0x0122D70E), CThing::GetPos @ 0x004C73D0, the __fastcall thing factory @ 0x00703210,
//   CTCInventoryItem::AddToInventory @ 0x005D8D50 / RemoveFromInventory @ 0x005D8E20, CTCInventory::
//   GetQuickAccessNextFreeSlot @ 0x00582027 / SetThingAsQuickAccessItem @ 0x00584157, the inventory's virtual slot
//   47 (+0xbc) finding the thing of a def.
// Layout (retail): CThing presence bitset +0x20, component map +0x44, def index +0x8c, player number +0x90, Dead +0x91
//   bit 0; CHeroAbilityDef inventory object +0x98; interfaces 0x11 inventory, 0x1a inventory item, 0x6f abilities.
// Shape: the def pointer returned by value is homed in the dead `learn` slot; its release is the null-tested
//   CDefPointer dtor calling the pointee's inline ReleaseTemporary() (DecRef / GetRefCount).
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long m_refCount;                            // +0x04
    void DecRef() const { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CHeroAbilityDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x98 - 0x08];
    long InventoryObject;                               // +0x98
};
template <class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
namespace NInventory { class CTCInventoryBase { public: virtual void v00(); }; }   // RTTI: CTCInventory's base @0
class CTCInventory : public NInventory::CTCInventoryBase { public:
    long GetQuickAccessNextFreeSlot(long item);                         // 0x00582027
    void SetThingAsQuickAccessItem(long item, long slot, bool a);       // 0x00584157
};
class CTCInventoryItem { public:
    bool AddToInventory(NInventory::CTCInventoryBase& inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
    void RemoveFromInventory();                                         // 0x005D8E20
};
class CTCInventoryAbilities { public:
    bool IsAbilityAvailable(EHeroAbility ability) const;                // 0x005EDF08
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    void SetAbilityAsAvailable(EHeroAbility ability, bool available, bool a);   // 0x005F6BD6
};
class CRightHandedSet;

class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x38];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned char _pad8e[0x90 - 0x8e];
    signed char PlayerNumber;                           // +0x90
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPos() const;               // 0x004C73D0
    FABLE_FORCEINLINE bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long type)
    {
        if (HasTC(type)) {
            *out = (T*)GetTC(type);
            return true;
        }
        return false;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        return (it == map->end() || type < it->first ? map->end() : it)->second;
    }
};
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* orientation,
                                           unsigned long flags, const CCharString& name);   // 0x00703210 (Ego_r/FableWin ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};
struct CTCInventoryFind : CTCInventory {
    // slot 47 (+0xbc): the first inventory thing of a def
};
FABLE_FORCEINLINE CThing* CreateThingObject(long defIndex, const CThing* creature, const CCharString& name)
{
    return CThingObject::Create(defIndex, creature->GetPos(), creature->PlayerNumber, 0, 0, name);
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
FABLE_FORCEINLINE CThing* FindThing(CTCBase* inventory, long object)
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
                CDefPointer<const CHeroAbilityDef> def = abilities->GetAbilityPDef(ability);
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
                        itemTC->AddToInventory(*inventory, false, true, 0, silent, false);
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
                CDefPointer<const CHeroAbilityDef> def = abilities->GetAbilityPDef(ability);
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