// CThingCreatureBase::CreateWeaponForThingFromDef(CThing& thing, long weaponDefIndex) -- create a weapon object at the thing's position for its player; a thing with a weapons inventory gets it as a default inventory item made the active weapon, any other thing has it put in its carrying component's active carry slot (the secondary one when a living thing already holds the primary), after clearing the passive slot
// Grade: DIFFER(414v381) -- the weapons-inventory branch is retail's bar frame offsets (one extra 4-byte frame slot, 0xC vs 8, no ebx save); the carrying branch reloads the CDefPointer<const CCarryableDef> from its frame home after every call and the release keeps its null test and zeroing (retail holds the def in esi, releases with a bare `dec [esi+4]` and no null test, and keeps the carrying TC in ebx set by `xor ebx,ebx` + if, not a ternary), and the CreatedByOwnerThingCreation bit is a split load/or/store. Next: a same-TU GetSubDef<CCarryableDef> stand-in as in the landed DropWeapon 0x00845CE0, carrying as `= 0; if (HasTC) ...`. Tried: Find() inline (key reloaded, not inlined for the weapon), the landed copy-key GetTC (constant compares, retail's), a map-pointer local (retail's `add esi, 0x44` map register).
// Leaf: ?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00663990, 381 bytes, static __fastcall (ecx = thing, edx = def index), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z; the landed
//   CCreatureAction_BanditKingChangeWeapons::PerformAction 0x008C8960 calls 0x00663990 with (creature, def) twice.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E, -1) / ~CCharString 0x0099EAE0, CThing::GetPos() const
//   0x004C73D0, CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*, unsigned long, const CCharString&)
//   0x00703210, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline CThing::HasTC / GetTC: interfaces 0x13
//   weapons inventory, 0x1A inventory item, 0x46 carrying), CTCInventoryItem::AddToInventory(NInventory::CTCInventoryBase&,
//   bool, bool, long, bool, bool) 0x005D8D50, CTCInventoryWeapons::SetThingAsActiveWeapon(long, NUISystem::CComponent*)
//   0x005CB2B0, CDefClassBase::GetSubDef<CCarryableDef>(CDefPointer<const CCarryableDef>*) const 0x005CC10E,
//   CTCCarrying::RemoveThingInCarrySlot(long, bool) 0x006A16A0 / GetThingInCarrySlot(long) const 0x006A11F0 /
//   PutThingInCarrySlot(CThing&, long, bool) 0x006A1F80.
// Label correction: catalogued as "_global::CreateWeaponForThingFromDef" (a free function); it is the static member.
// Layout: CThing retail offsets (Ego_r.pdb names, retail is 0x10 below the debug-STL donor from PBaseDef on): UsingComponents
//   +0x20, Components +0x44, PBaseDef +0x70, DefGlobalIndex +0x8C, PlayerNumber +0x90, Dead / CreatedByOwnerThingCreation
//   bits 0 / 1 of +0x91; CTCInventoryItem::DefaultItem +0x20; CCarryableDef from engine/CCarryableDef.h offsets
//   (ActiveCarrySlot +0x28, SecondaryActiveCarrySlot +0x2C, PassiveCarrySlot +0x30). No engine/ header for CThing,
//   CThingObject, CTCCarrying or CTCInventoryWeapons; engine/CThingCreatureBase.h is a flattened POD without members.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector {
public:
    float X, Y, Z;
};
class CRightHandedSet;

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1
    mutable unsigned long IVRefCount;                       // +0x04
    void AddRef() const { ++IVRefCount; }
    long Release() const
    {
        long result = (long)--IVRefCount;
        if (result == 0)
            Delete();
        return result;
    }
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResourceList* ResourceList;
    CResource*     PrevResource;
    CResource*     NextResource;
    unsigned long  ResourceSize;
    unsigned long  LastUsedFrame;
    virtual ~CResource();
};
class CDefPointeeBase : public CResource {
public:
};
enum ENullDef { NULL_DEF = 0 };
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer(ENullDef = NULL_DEF) : Object(0) { }
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return (T*)Object; }
};
class CDefClassBase : public CDefPointeeBase {
public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005CC10E for CCarryableDef
};
class CThingBaseDef : public CDefClassBase { };
class CCarryableDef : public CDefClassBase {                // retail 0x50 bytes
public:
    unsigned char _pad_0x1c[0x28 - 0x1c];
    long ActiveCarrySlot;                                   // +0x28
    long SecondaryActiveCarrySlot;                          // +0x2C
    long PassiveCarrySlot;                                  // +0x30
};

namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const { return (_M_w[__pos / 32] & (1UL << (__pos % 32))) != 0; }
};
}

class CThing;
enum ETCInterfaceType { };
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* LowerBound(const K& key);                   // 0x0040F020
    value_type* end() { return Data._M_finish; }
    std::vector<value_type> Data;
    Cmp  Compare;
    bool Dirty;
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> UsingComponents;                       // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char _pad_0x54[0x70 - 0x54];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
    unsigned char _pad_0x74[0x8c - 0x74];
    unsigned short DefGlobalIndex;                          // +0x8C
    short          CurrentMapNumber;                        // +0x8E
    signed char    PlayerNumber;                            // +0x90
    unsigned char  Dead : 1;                                // +0x91
    unsigned char  CreatedByOwnerThingCreation : 1;
    const C3DVector& GetPos() const;                        // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    __forceinline CTCBase& GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        std::pair<ETCInterfaceType, CTCBase*>* end = map->end();
        return *(it == end || type < it->first ? end : it)->second;
    }
};
class CThingObject : public CThing {
public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& pos, long playerNumber,
                                           const CRightHandedSet* rhSet, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};
namespace NUISystem { class CComponent; }
namespace NInventory {
class CTCInventoryBase : public CTCBase { };
}
class CTCInventoryWeapons : public NInventory::CTCInventoryBase {
public:
    void SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* component);   // 0x005CB2B0
};
class CTCInventoryItem : public CTCBase {
public:
    bool AddToInventory(NInventory::CTCInventoryBase& inventory, bool, bool, long, bool, bool);   // 0x005D8D50
    unsigned char _pad_0x0c[0x20 - 0x0c];
    bool DefaultItem;                                       // +0x20
};
class CTCCarrying : public CTCBase {
public:
    CThing* RemoveThingInCarrySlot(long slot, bool);        // 0x006A16A0
    CThing* GetThingInCarrySlot(long slot) const;           // 0x006A11F0
    void PutThingInCarrySlot(CThing& thing, long slot, bool);   // 0x006A1F80
};
class CThingCreatureBase {
public:
    static void __fastcall CreateWeaponForThingFromDef(CThing& thing, long weaponDefIndex);
};

void __fastcall CThingCreatureBase::CreateWeaponForThingFromDef(CThing& thing, long weaponDefIndex)
{
    if (weaponDefIndex == 0)
        return;
    CThingObject* weapon = CThingObject::Create(weaponDefIndex, thing.GetPos(), thing.PlayerNumber, 0, 0, "");
    if (thing.HasTC((ETCInterfaceType)0x13)) {
        CTCInventoryWeapons& inventory = (CTCInventoryWeapons&)thing.GetTC((ETCInterfaceType)0x13);
        if (weapon->HasTC((ETCInterfaceType)0x1a)) {
            CTCInventoryItem& item = (CTCInventoryItem&)weapon->GetTC((ETCInterfaceType)0x1a);
            item.DefaultItem = true;
            item.AddToInventory(inventory, false, true, 0, false, false);
            inventory.SetThingAsActiveWeapon(weapon->DefGlobalIndex, 0);
        }
    }
    else {
        CTCCarrying* carrying = thing.HasTC((ETCInterfaceType)0x46) ? (CTCCarrying*)&thing.GetTC((ETCInterfaceType)0x46) : 0;
        CDefPointer<const CCarryableDef> carryableDef;
        weapon->PBaseDef->GetSubDef(&carryableDef);
        carrying->RemoveThingInCarrySlot(carryableDef->PassiveCarrySlot, false);
        long slot = carryableDef->ActiveCarrySlot;
        CThing* occupant = carrying->GetThingInCarrySlot(slot);
        if (occupant && !occupant->Dead)
            slot = carryableDef->SecondaryActiveCarrySlot;
        carrying->PutThingInCarrySlot(*weapon, slot, false);
        weapon->CreatedByOwnerThingCreation = true;
    }
}
