// CCreatureAction_UnsheatheWeapons::ShouldPlayUnsheatheWeaponAction(const CThingCreatureBase& creature, EUnsheatheWeaponType type, bool& play_animation, bool& sheathe_first) -- whether a weapon (un)sheathe action should play for the creature: types other than 0/1/3 need the creature def's UnsheatheWeaponsAutomatically; the wanted weapon def (default for 2/3, melee for 4, ranged for 5, none otherwise) is compared with the live thing in the primary carry slot; no wanted weapon plays STANDARD_SHEATHE when it exists (sheathe first) or reports STANDARD_UNSHEATHE; a wanted weapon not already in hand reports STANDARD_UNSHEATHE
// Leaf: ?ShouldPlayUnsheatheWeaponAction@CCreatureAction_UnsheatheWeapons@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7CF0, 480 bytes incl. the 4-entry jump table and int3 padding, /Gr
//   `__fastcall` static (creature in ecx, type in edx, the two bool& on the stack), `ret 8`.
// Identity: PDB name set `?ShouldPlayUnsheatheWeaponAction@CCreatureAction_UnsheatheWeapons@@SI_NABVCThingCreatureBase@@W4EUnsheatheWeaponType@@AA_N2@Z`;
//   callers 0x006EBDF9 and 0x008B7EF8 (the CCreatureAction_UnsheatheWeapons ctor next door). Enumerator names are not in
//   the PDB dumps (descriptive UNSHEATHE_WEAPON_TYPE_n).
// Callees: CThingCreatureBase::GetDefaultWeaponDef() const 0x00662860, GetMeleeWeaponDef() const 0x006623F0 and
//   GetRangedWeaponDef() const 0x00662400 (catalog "CPixelShader::CountReferences" is propagated for both: they read
//   PDef->PrimaryMeleeWeaponDef +0xD8 / RangedWeaponDef +0xE0), CVectorMap::LowerBound const 0x004365B0, CTCCarrying::
//   GetThingInPrimarySlot() const 0x006A1440, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, CThingCreatureBase::AnimExists(
//   const CCharString&, bool) const = virtual slot 73 (+0x124; the landed 0x00830F20 calls it "HasAnimation_slot73"), the
//   def's virtual slot 1 on the last release.
// Layout: CThing PBaseDef +0x70, UsingComponents +0x20, Components +0x44, DefIndex (word) +0x8C, Dead bit 0 of +0x91;
//   CThingCreatureDef flag byte +0x148 (Ego_r +0x150): bit 4 UseMeleeWeaponAsDefault, bit 5 UnsheatheWeaponsAutomatically.
//   TC interface type 0x46 CTCCarrying.
// Shape: the base def is a by-value CDefPointer temporary converted to a raw pointer (add-ref then immediate release, the
//   flag read afterwards: the landed 0x008CB780 model, pointee Release() through DecRef/GetRefCount); the weapon-def switch
//   (case 5 written before case 4: retail emits the ranged block first, and the relocation-masked compare cannot see a
//   swapped jump table -- check the $L case-label offsets; default 0 skips the `< 0` test); `weapon_def < 0 || !HasTC(0x46)` in ONE condition (two separate ifs put the shared
//   `return false` epilogue after the in-hand compare instead of at the end); `<= 0` for the `jg`; each animation name is a
//   named CCharString in its own block (built before the `push 0`, so no tail merge of the two STANDARD_UNSHEATHE reports).
class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long RefCount;                                  // +0x04
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CThingBaseDef : public CDefClassBase { };
class CThingCreatureDef : public CThingBaseDef {
public:
    unsigned char _pad08[0x148 - 0x08];
    bool LeaveDeadCreature : 1;                             // +0x148 bit 0
    bool AbleToStrafe : 1;                                  // bit 1
    bool CreateDummyOnDeath : 1;                            // bit 2
    bool ImmuneToAttack : 1;                                // bit 3
    bool UseMeleeWeaponAsDefault : 1;                       // bit 4
    bool UnsheatheWeaponsAutomatically : 1;                 // bit 5
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    operator T*() const { return Object; }
};

#define V8(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
              virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7();

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    // slots 0..72 (AnimExists is CThingCreatureBase's slot 73, +0x124)
    V8(0) V8(1) V8(2) V8(3) V8(4) V8(5) V8(6) V8(7) V8(8) virtual void v90();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
    unsigned char _pad74[0x8c - 0x74];
    unsigned short DefIndex;                                // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    bool Dead : 1;                                          // +0x91 bit 0

    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* PeekTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};

class CThingCreatureBase : public CThing {
public:
    virtual bool AnimExists(const CCharString& name, bool current_mode_only) const;   // slot 73 (+0x124)
    long GetDefaultWeaponDef() const;                       // 0x00662860
    long GetMeleeWeaponDef() const;                         // 0x006623F0
    long GetRangedWeaponDef() const;                        // 0x00662400
};

class CTCCarrying {                                         // interface type 0x46
public:
    CThing* GetThingInPrimarySlot() const;                  // 0x006A1440
};

enum EUnsheatheWeaponType {
    UNSHEATHE_WEAPON_TYPE_0, UNSHEATHE_WEAPON_TYPE_1, UNSHEATHE_WEAPON_TYPE_2,
    UNSHEATHE_WEAPON_TYPE_3, UNSHEATHE_WEAPON_TYPE_4, UNSHEATHE_WEAPON_TYPE_5
};

class CCreatureAction_UnsheatheWeapons {
public:
    static bool __fastcall ShouldPlayUnsheatheWeaponAction(const CThingCreatureBase& creature, EUnsheatheWeaponType type,
                                                           bool& play_animation, bool& sheathe_first);
};

bool __fastcall CCreatureAction_UnsheatheWeapons::ShouldPlayUnsheatheWeaponAction(const CThingCreatureBase& creature,
    EUnsheatheWeaponType type, bool& play_animation, bool& sheathe_first)
{
    if (type != UNSHEATHE_WEAPON_TYPE_0 && type != UNSHEATHE_WEAPON_TYPE_1 && type != UNSHEATHE_WEAPON_TYPE_3) {
        const CThingCreatureDef* def = static_cast<const CThingCreatureDef*>((const CThingBaseDef*)creature.GetPBaseDef());
        if (!def->UnsheatheWeaponsAutomatically)
            return false;
    }
    long weapon_def;
    switch (type) {
    case UNSHEATHE_WEAPON_TYPE_2:
    case UNSHEATHE_WEAPON_TYPE_3:
        weapon_def = creature.GetDefaultWeaponDef();
        break;
    case UNSHEATHE_WEAPON_TYPE_5:
        weapon_def = creature.GetRangedWeaponDef();
        break;
    case UNSHEATHE_WEAPON_TYPE_4:
        weapon_def = creature.GetMeleeWeaponDef();
        break;
    default:
        weapon_def = 0;
        break;
    }
    if (weapon_def < 0 || !creature.HasTC(0x46))
        return false;
    const CThing* in_hand = ((const CTCCarrying*)creature.PeekTC(0x46))->GetThingInPrimarySlot();
    if (weapon_def <= 0) {
        if (in_hand == 0 || in_hand->Dead)
            return false;
        bool can_sheathe;
        {
            CCharString sheathe_name("STANDARD_SHEATHE");
            can_sheathe = creature.AnimExists(sheathe_name, false);
        }
        if (can_sheathe) {
            play_animation = true;
            sheathe_first = true;
            return true;
        }
        {
            CCharString unsheathe_name("STANDARD_UNSHEATHE");
            play_animation = creature.AnimExists(unsheathe_name, false);
        }
        sheathe_first = false;
        return true;
    } else {
        if (in_hand != 0 && !in_hand->Dead && in_hand->DefIndex == weapon_def)
            return false;
        {
            CCharString unsheathe_name("STANDARD_UNSHEATHE");
            play_animation = creature.AnimExists(unsheathe_name, false);
        }
        sheathe_first = false;
        return true;
    }
}