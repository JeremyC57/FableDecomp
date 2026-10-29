// CTCTurncoatSpell::SetPTarget(CThingCreatureBase* target) -- stores the spell target; when the target is alive and turncoat-able, sets its turncoat duration from the spell def's TurncoatTime entry for the caster's turncoat ability level
// Leaf: SetPTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C6940, 243 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer() 0x00A01B10 (then the CIntelligentPointer vtable 0x0123E7D8); CBaseIntelligentPointer::
//          SetPItem 0x00A01B90 (x2); GetPItem 0x00A01B50 (x4); ~CBaseIntelligentPointer 0x00A01C10;
//          CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x9F, CTCTurncoat) and its const
//          instantiation 0x004365B0 (interface 0x6F, CTCInventoryAbilities, through a const CThing*);
//          CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) const 0x005EDE93 (ability 5); CTCTurncoat::
//          SetTurncoatTimeLast(float) 0x007C7810.
// Layout: engine/CTCTurncoatSpell.h carries Ego_r.pdb offsets (PDef +0x44, PTarget +0x48); retail members after the
//          timer sit 4 bytes lower (as the landed StopTurncoatingEffects 0x007C6DA0 / FinalCleanup 0x007C75E0 models):
//          PDef +0x40, PTarget +0x44 -- so a local model. CSpecialAbilitiesTurncoatSpellDef::TurncoatTime (vector<float>)
//          +0x54 (PDB +0x60, 12-byte vectors after a 0x3c base). CThing bitset +0x20 (0x9F = sign bit of word +0x30,
//          0x6F = bit 15 of word +0x2c), interface map +0x44, dead bit +0x91.
// Shape: `PTarget = CIntelligentPointer<CThingCreatureBase>(target)` through the inline operator= (GetPItem of the
//        temporary, SetPItem on the member, the temporary destroyed); IS_THING_ALIVE re-fetches the item; the target's
//        component is fetched with the bare one-ternary GetTC after its presence test (no zero preload), the caster's
//        through the const GetTC; the def is a named local read just before the level call (kept in esi across it).
class CThing;
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
};
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();                         // +0x00 vptr
    void*   PData;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CTCTurncoat { public:
    void SetTurncoatTimeLast(float time);       // 0x007C7810
};
class CTCInventoryAbilities { public:
    long GetAbilityLevel(long ability) const;   // 0x005EDE93 (EHeroAbility)
};
struct CFloatVector {                            // STLport vector<float> (12 bytes in retail)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    const float& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CSpecialAbilitiesTurncoatSpellDef { public:
    unsigned char _pad_0x00[0x54];
    CFloatVector  TurncoatTime;                 // +0x54
};

class CTCTurncoatSpell { public:
    void*                     __vftable;        // +0x00
    CThing*                   ParentThing;      // +0x04 (CTCBase)
    unsigned char             _pad_0x08[0x38];
    CSpecialAbilitiesTurncoatSpellDef* PDef;    // +0x40 (CDefPointer<const CSpecialAbilitiesTurncoatSpellDef>)
    CIntelligentPointer<CThingCreatureBase> PTarget;   // +0x44
    void SetPTarget(CThingCreatureBase* target);
};

void CTCTurncoatSpell::SetPTarget(CThingCreatureBase* target)
{
    PTarget = CIntelligentPointer<CThingCreatureBase>(target);
    if (IS_THING_ALIVE(PTarget)) {
        CThing* targetThing = PTarget.GetPItem();
        if (targetThing->HasTC(0x9f)) {
            CTCTurncoat* turncoat = (CTCTurncoat*)targetThing->GetTC(0x9f);
            const CThing* caster = ParentThing;
            if (caster->HasTC(0x6f)) {
                const CTCInventoryAbilities* abilities = (const CTCInventoryAbilities*)caster->GetTC(0x6f);
                CSpecialAbilitiesTurncoatSpellDef* def = PDef;
                turncoat->SetTurncoatTimeLast(def->TurncoatTime[abilities->GetAbilityLevel(5)]);
            }
        }
    }
}