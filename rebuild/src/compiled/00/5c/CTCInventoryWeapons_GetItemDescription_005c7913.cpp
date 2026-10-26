// CTCInventoryWeapons::GetItemDescription(CWideString& description, long defIndex, bool flag) const -- for a weapon item: the live active melee weapon's damage and the live active ranged weapon's damage (its ability def's damage def value times the object-augmentations damage multiplier), then ConstructWeaponDescription with the one matching the item's weapon class; other items go to the base description
// Leaf: GetItemDescription
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C7913, 509 bytes, `ret 0Ch`; virtual (CTCInventoryWeapons vtable 0x01253FF4 slot 78). Ego_r.pdb: ?GetItemDescription@CTCInventoryWeapons@@UBEXAAVCWideString@@J_N@Z.
//   Frame-pointer size TU: optimize("y",off) + ("s",on) (`fldz ; fstp`, `leave`). One of four byte-twins (0x005C7913,
//   0x005C7B10, 0x005C7D0D, 0x005C7F07) that differ only in the final Construct* callee and the bool argument.
// Callees: IsWeaponItem @ 0x005D97F0 (manifest "IsAugmentation", `__fastcall`), IS_THING_ALIVE @ 0x004CC340, GetPItem
//   @ 0x00A01B50, bitset::test @ 0x00410DE0 + CVectorMap::LowerBound @ 0x004365B0 (inline GetInterface 0x48 weapon,
//   0xBF object augmentations), CTCWeapon damage getter @ 0x005DAAA0, CDefClassBase::GetSubDef @ 0x0042B1A6,
//   CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex @ 0x009AD9E0 (hidden result) /
//   GetDef @ 0x0042AF3C, CTCObjectAugmentations::GetDamageMultiplier @ 0x00766140, IsMeleeWeaponClass @ 0x005DA510,
//   ConstructWeaponDescription @ 0x005C6B26, CTCInventoryBase::GetItemDescription @ 0x005BC6B3.
// Shape: the def pointers are hand-released (`dec [reg+4] ; jne`, no null test once dereferenced); the GetPDefFromGlobalIndex
//   temporary is copied through an inline AddRef(const&) and released with the nulling forced-inline dtor; GetSubDef and GetDef
//   get same-TU noinline stand-ins (they only write the out slot) so the ability def is read once for its release; the
//   final def lookups sit in their own block so they reuse the ranged block's frame words.
// Layout: retail PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141 (packed, as the landed AugmentWeapon 0x005C44FA);
//   CThing PDef +0x70; ability def damage-def index +0xA0, damage def value +0x54, weapon def class +0x2C (retail offsets).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CWideString;
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
};
class CWeaponDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c - 0x08];
    long WeaponClass;                           // +0x2c
};
class CAbilityDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xa0 - 0x08];
    long DamageDefIndex;                        // +0xa0
};
struct CDamageDef : CDefPointeeBase {
    unsigned char _pad08[0x54 - 0x08];
    float Damage;                               // +0x54
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAbilityDef>* out);   // 0x0042B1A6
};
struct CDefBasePointer {                        // CDefPointer<CDefClassBase>: GetPDefFromGlobalIndex's by-value result
    CDefPointeeBase* Object;
    __forceinline ~CDefBasePointer()
    {
        if (Object) {
            CDefPointeeBase* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
};
__forceinline CDefPointeeBase* AddRef(const CDefBasePointer& pointer)
{
    CDefPointeeBase* object = pointer.Object;
    if (object)
        ++object->m_refCount;
    return object;
}
class CDefinitionManager { public:
    CDefBasePointer GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};

void* FindSubDef(const CDefClassBase* def);     // stand-in callee of the stand-in bodies below
void* FindDef(long index);
// Stand-in bodies (out of line in retail): they write only the out slot, so VC7.1 knows the def pointers' addresses do
// not escape the calls.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAbilityDef>* out)
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    out->Object = (CAbilityDef*)def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef(index);
    if (!def)
        return false;
    out->Object = (CDefClassBase*)def;
    return true;
}
#pragma auto_inline(on)

struct CPresenceBits { bool test(long kind) const; };      // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;                   // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;                        // +0x70
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
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };
#pragma pack(pop)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340

bool __fastcall IsWeaponItem(long defIndex);                // 0x005D97F0
bool __fastcall IsMeleeWeaponClass(long weaponClass);       // 0x005DA510
class CTCWeapon { public: float GetDamage() const; };              // 0x005DAAA0
class CTCObjectAugmentations { public: float GetDamageMultiplier() const; };   // 0x00766140

namespace NInventory {
class CTCInventoryBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CDefinitionManager* GetDefinitionManager() const;       // 0x006869D0 (CTCBase)
    void GetItemDescription(CWideString& description, long defIndex, bool flag) const;   // 0x005BC6B3 (virtual; called qualified)
};
}
class CTCInventoryWeapons : public NInventory::CTCInventoryBase { public:
    CIntelligentPointer<CThing> PActiveMeleeWeapon;         // +0x134
    long          ActiveMeleeWeaponDefIndex;                // +0x13C
    bool          MeleeWeaponCarriedOnLoad;                 // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;        // +0x141 (packed in retail)
    void ConstructWeaponDescription(CWideString& description, long defIndex, float damage, bool flag) const;   // 0x005C6B26
    void GetItemDescription(CWideString& description, long defIndex, bool flag) const;
};

void CTCInventoryWeapons::GetItemDescription(CWideString& description, long defIndex, bool flag) const
{
    if (IsWeaponItem(defIndex)) {
        float meleeDamage = 0.0f;
        float rangedDamage = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon))
            meleeDamage = PActiveMeleeWeapon.GetPItem()->GetTC<CTCWeapon>(0x48)->GetDamage();
        const CIntelligentPointer<CThing>& ranged = PActiveRangedWeapon;
        if (IS_THING_ALIVE(ranged)) {
            CDefPointer<CAbilityDef> ability;
            ability.Object = 0;
            CDefClassBase* thingDef = ranged.GetPItem()->PDef.Object;
            if (thingDef)
                ++thingDef->m_refCount;
            thingDef->GetSubDef(&ability);
            if (thingDef && --thingDef->m_refCount == 0)
                thingDef->Destroy();
            CDefPointeeBase* damageDef = AddRef(GetDefinitionManager()->GetPDefFromGlobalIndex(ability.Object->DamageDefIndex));
            rangedDamage = ((CDamageDef*)damageDef)->Damage;
            CTCObjectAugmentations* augmentations;
            if (ranged.GetPItem()->GetInterface(&augmentations, 0xbf))
                rangedDamage *= augmentations->GetDamageMultiplier();
            if (--damageDef->m_refCount == 0)
                damageDef->Destroy();
            ability.Object->Release();
        }
        {
        CDefPointer<CDefClassBase> def;
        def.Object = 0;
        GetDefinitionManager()->GetDef(defIndex, &def);
        CDefClassBase* thingDef = def.Object;
        CDefPointer<CAbilityDef> weapon;
        weapon.Object = 0;
        thingDef->GetSubDef(&weapon);
        CWeaponDef* weaponDef = (CWeaponDef*)weapon.Object;
        ConstructWeaponDescription(description, defIndex, IsMeleeWeaponClass(weaponDef->WeaponClass) ? meleeDamage : rangedDamage, flag);
        if (--weaponDef->m_refCount == 0)
            weaponDef->Destroy();
        if (thingDef && --thingDef->m_refCount == 0)
            thingDef->Destroy();
        }
    } else {
        NInventory::CTCInventoryBase::GetItemDescription(description, defIndex, false);
    }
}