// CTCSpecialAbilities::IsAbleToUseAnyAbility(EHeroAbility ability) const -- the hero may use an ability unless the abilities are locked, he is in bed or knocked down, or his interface-0x29 component disallows it; a carried weapon must also be usable by him (ability 8 exempt)
// Grade: DIFFER(256v256) -- scheduling only: the IsUsableBy argument (ParentThing) is loaded and pushed before the weapon component is moved into ecx; retail loads the component first and pushes the thing through eax (ternary / FindEntry / named-entry / getter / out-parameter forms tried)
// Leaf: IsAbleToUseAnyAbility
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072AF30, 254 bytes, `ret 4`.
// Callees: IsKnockedDown(CThing*) @ 0x0085F010 (`__fastcall`), the const interface LowerBound @ 0x004365B0 (interfaces 0x29,
//   0x46 = CTCCarrying, 0x48 = CTCWeapon), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsUsableBy(const
//   CThing&) const @ 0x005DAB10.
// Layout: Ego_r.pdb CTCSpecialAbilities AbilityMutexLock +0x0C, UsingBed +0x0D; retail CThing interface bitset +0x20
//   (0x29 = word +0x24 bit 9, 0x46 = byte +0x28 bit 6, 0x48 = word +0x28 bit 8), map +0x44; the 0x29 component's
//   abilities-allowed flag +0x0D (named descriptively).
// Shape: the 0x29 component is a zero-preloaded pointer filled under HasTC (`xor eax,eax`) and dereferenced unconditionally;
//   the later lookups are HasTC + end/hit ternary (const LowerBound); the weapon key homes in the dead `ability` slot.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    __forceinline const CInterfaceMap::SEntry* FindEntry(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= type) sel = it;
        else sel = map->last;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (HasTC(type)) {
            *out = GetTC<T>(type);
            return true;
        }
        return false;
    }
};
bool __fastcall IsKnockedDown(CThing* thing);           // 0x0085F010
struct CTCAbilityPermissions {                          // interface 0x29 (named descriptively)
    unsigned char _pad00[0x0d];
    bool          AbilitiesAllowed;                     // +0x0d
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                    // 0x006A1440
};
struct CTCWeapon {
    bool IsUsableBy(const CThing& thing) const;         // 0x005DAB10
};
enum EHeroAbility { HERO_ABILITY_8 = 8 };
struct CTCSpecialAbilities {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                              // +0x0c
    bool UsingBed;                                      // +0x0d
    long CurrentAbility;                                // +0x10
    bool IsAbleToUseAnyAbility(EHeroAbility ability) const;
};

bool CTCSpecialAbilities::IsAbleToUseAnyAbility(EHeroAbility ability) const
{
    if (!AbilityMutexLock && !UsingBed && !IsKnockedDown(ParentThing)) {
        const CThing* thing = ParentThing;
        CTCAbilityPermissions* permissions = 0;
        if (thing->HasTC(0x29))
            permissions = thing->GetTC<CTCAbilityPermissions>(0x29);
        if (permissions->AbilitiesAllowed) {
            if (!thing->HasTC(0x46))
                return true;
            CThing* carried = thing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
            if (!carried || ability == HERO_ABILITY_8 || !carried->HasTC(0x48)
                || ((CTCWeapon*)carried->FindEntry(0x48)->component)->IsUsableBy(*ParentThing))
                return true;
        }
        return false;
    }
    return false;
}
