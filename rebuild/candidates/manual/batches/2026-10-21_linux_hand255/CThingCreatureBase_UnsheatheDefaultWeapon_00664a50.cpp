// CThingCreatureBase::UnsheatheDefaultWeapon() -- draw the creature's default weapon (the melee weapon when its def prefers melee, else -- unless already busy or already carrying a ranged weapon -- sheathe everything and draw the def's ranged weapon), then, when the def asks for it, reset the combat component's combat type to the def's one if it differs
// Leaf: ?UnsheatheDefaultWeapon@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664A50, length 336 bytes, plain `ret`.
//   Name from Ego_r.pdb `?UnsheatheDefaultWeapon@CThingCreatureBase@@QAEXXZ`.
// Callees: CThingCreatureBase::UnsheatheMeleeWeapon @ 0x006649C0, the creature's virtual slot 77 (+0x134, a busy test),
//   IsThingCarryingRangedWeapon(const CThing&) @ 0x006A1620 (`__fastcall`), CThingCreatureBase::SheatheAllWeapons
//   @ 0x006647F0, CThingCreatureBase::UnsheatheWeapon(long, bool) @ 0x006648D0, LowerBound @ 0x0040F020,
//   CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0 (CDefString
//   by value), the combat type def's virtual slot 10 (+0x28, its def name by value), CDefinitionManager::GetDef
//   <CCombatTypeDef>(long, CDefPointer*) @ 0x00669640, CTCCombat::SetCombatTypeDef @ 0x007355F0 (the landed
//   "SetAttackStyleDef"), the defs' virtual slot 1 on the last release.
// Layout (retail): CThing bitset +0x20 (0x30 CTCCombat = bit 16 of +0x24), map +0x44, PBaseDef +0x70;
//   CThingCreatureBase::PDef +0xe0; CThingCreatureDef RangedWeaponDef +0xe0, CombatTypeDef +0x124,
//   ResetCombatTypeOnUnsheatheDefaultWeapons +0x128, UseMeleeWeaponAsDefault bit 4 of +0x148 (PDB 8 higher);
//   CTCCombat::PCombatTypeDef +0x28.
// Shape: the combat type is a local counted copy (its release has no null test: VC drops it after the vcall
//   dereference); the creature def is peeked through an inline copy that is released before its fields are read; the
//   name compare is `!(a == b)` with the manager call evaluated first (member operator== argument).
struct CTCBase;

struct CDefString {
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
    bool operator==(const CDefString& other) const { return m_index == other.m_index; }
};

struct CDefPointeeBase {
    virtual void v000();
    virtual void DestroyDef() const;                    // slot 1
    mutable long m_refCount;                            // +4
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};

struct CCombatTypeDef : CDefPointeeBase {
    virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void v007(); virtual void v008(); virtual void v009();
    virtual CDefString GetDefName() const;              // slot 10 (+0x28)
};

struct CThingCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0xe0 - 8];
    long          RangedWeaponDef;                      // +0xe0
    unsigned char _pade4[0x124 - 0xe4];
    long          CombatTypeDef;                        // +0x124
    bool          ResetCombatTypeOnUnsheatheDefaultWeapons;   // +0x128
    unsigned char _pad129[0x148 - 0x129];
    unsigned char _bits148 : 4;
    unsigned char UseMeleeWeaponAsDefault : 1;          // +0x148 bit 4
};

struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
    void GetDef(long index, CDefPointer<const CCombatTypeDef>* out);   // 0x00669640
};

struct CTCCombat {
    unsigned char _pad00[0x28];
    CDefPointer<const CCombatTypeDef> PCombatTypeDef;   // +0x28
    void SetCombatTypeDef(const CDefPointer<const CCombatTypeDef>* def);   // 0x007355F0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual bool IsBusy() const;                        // slot 77 (+0x134)
    unsigned char  _pad04[0x20 - 4];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefPointer<const CThingCreatureDef> PBaseDef;      // +0x70
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
bool __fastcall IsThingCarryingRangedWeapon(const CThing& thing);   // 0x006A1620

struct CThingCreatureBase : CThing {
    unsigned char _pad74[0xe0 - 0x74];
    const CThingCreatureDef* PDef;                      // +0xe0 CDefPointer<const CThingCreatureDef>::Object
    const CThingCreatureDef* GetDef() const { return PDef; }
    __forceinline const CThingCreatureDef& PeekDefinition() const
    {
        CDefPointer<const CThingCreatureDef> def(PBaseDef);
        return *def.Object;
    }
    void SheatheAllWeapons();                           // 0x006647F0
    void UnsheatheWeapon(long weaponDef, bool primary); // 0x006648D0
    void UnsheatheMeleeWeapon();                        // 0x006649C0
    void UnsheatheDefaultWeapon();
};

void CThingCreatureBase::UnsheatheDefaultWeapon()
{
    if (GetDef()->UseMeleeWeaponAsDefault) {
        UnsheatheMeleeWeapon();
    } else if (!IsBusy() && !IsThingCarryingRangedWeapon(*this)) {
        SheatheAllWeapons();
        UnsheatheWeapon(GetDef()->RangedWeaponDef, true);
    }
    if (HasTC(0x30)) {
        CTCCombat* combat = (CTCCombat*)GetTC(0x30);
        CDefPointer<const CCombatTypeDef> combatType = combat->PCombatTypeDef;
        const CThingCreatureDef& def = PeekDefinition();
        if (!(combatType->GetDefName() == GetDefinitionManager()->GetDefNameFromGlobalIndex(def.CombatTypeDef))
            && def.ResetCombatTypeOnUnsheatheDefaultWeapons) {
            CDefPointer<const CCombatTypeDef> newType;
            GetDefinitionManager()->GetDef(def.CombatTypeDef, &newType);
            combat->SetCombatTypeDef(&newType);
        }
    }
}
