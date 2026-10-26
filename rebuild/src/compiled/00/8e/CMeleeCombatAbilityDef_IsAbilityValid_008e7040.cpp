// CMeleeCombatAbilityDef::IsAbilityValid(CThingCreatureBase* creature) -- the hero's combo multiplier is inside the
//   def's bounds (-1 = unbounded), the hero meets the stats package, the creature is flagged (bit 3 at +0x6c), carries
//   and fights (TCs 0x46 / 0x30, and for a flourish is in melee and the second combat test holds), its primary-slot
//   weapon class (0 without a live weapon) is one of the valid classes, and every attacker has the flourish animation
// Leaf: IsAbilityValid
// Retail: Fable.exe @ 0x008E7040, 592 bytes, `ret 4`, no frame pointer.
// Callees: CVectorMap LowerBound const 0x004365B0, the hero TC's combo multiplier 0x006D6570, CHeroAbilityStatPackage
//   IsMetBy 0x00851B50, CTCCombat 0x00733190 / 0x00733180, CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//   IS_THING_ALIVE<CThing*> 0x0040DCE0, CTCWeapon::GetWeaponClass 0x005DA7F0, std::find<long*> 0x008E6FB0,
//   CDefString::operator CCharString 0x00415D70 / const 0x00415DD0, ~CCharString 0x0099EAE0, GetCombatManager
//   0x00661EC0, CCombatManager::GetCombatantInfo 0x006E9360, the attacker iterator ++ 0x006EF790,
//   GetCombatAnimationSet 0x00663260, CAnimationSet::HasAnimation 0x005DC2E0; CAnimFilterNoGroup vtable 0x0122FFDC
//   (base 0x0122FFCC).
// Layout: Ego_r.pdb CMeleeCombatAbilityDef less 4 from Flourish on: FlourishAnimName +0x40, Flourish +0x48,
//   ValidWeaponClasses +0x50 (12-byte vector), StatsNeeded +0x68, ComboMultiplierLowerBound +0x74, UpperBound +0x78.
#pragma optimize("t", on)
#pragma optimize("y", on)
void StandInSink(void* p, unsigned long n);
class CTCBase;
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0
};
struct CTCBitset {
    unsigned long _M_w[9];
    __forceinline bool test(long bit) const { return (_M_w[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
class CThing { public:
    unsigned char _pad00[0x20];
    CTCBitset     UsingComponents;                       // +0x20
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x6c - 0x50];
    unsigned long Flags6c;                               // +0x6c
    template<class T> __forceinline bool GetInterface(T** out, long type) const
    {
        if (UsingComponents.test(type)) {
            unsigned long key = type;
            const CTCMap* map = &Components;
            CTCEntry* it = map->LowerBound(key);
            if (it == map->_M_finish || type < it->Type)
                it = map->_M_finish;
            *out = (T*)it->TC;
            return true;
        }
        return false;
    }
};
bool IsThingAliveImpl(const CThing* thing);
#pragma auto_inline(off)
// Same-TU read-only stand-ins for out-of-line retail callees that receive a local's address (IS_THING_ALIVE<CThing*>
// 0x0040DCE0, CVectorMap LowerBound const 0x004365B0, std::find<long*> 0x008E6FB0): the address does not escape, so
// VC7.1 can reuse the dead argument slot as retail does.
bool __fastcall IS_THING_ALIVE(CThing* const& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
CTCEntry* CTCMap::LowerBound(const unsigned long& type) const
{
    CTCEntry* it = _M_start;
    long count = _M_finish - _M_start;
    while (count > 0) {
        long half = count / 2;
        if ((unsigned long)it[half].Type < type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CCharStringData { public: char* PData; unsigned long StringLength; };
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                      // 0x0099EAE0
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
};
namespace N_00415dd0 { struct CDefString {
    long Index;
    operator CCharString();                              // 0x00415D70
    operator CCharString() const;                        // 0x00415DD0
}; } using N_00415dd0::CDefString;
class CAnimFilterBase { public:                                 // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
};
struct CAnimFilterNoGroup : CAnimFilterBase {            // vtable 0x0122FFDC
    virtual void v01();
};
class CAnimationSet { public:
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x005DC2E0
};
class CCombatManager;
class CThingCreatureBase : public CThing { public:
    CCombatManager* GetCombatManager() const;            // 0x00661EC0
    const CAnimationSet& GetCombatAnimationSet() const;  // 0x00663260
};
struct SAttackerNode {                                   // STLport node of map<unsigned __int64, CThingCreatureBase*>
    long colour; SAttackerNode* parent; SAttackerNode* left; SAttackerNode* right;
    unsigned __int64 first;                              // +0x10
    CThingCreatureBase* second;                          // +0x18
};
struct CAttackerIterator {
    SAttackerNode* _M_node;
    CAttackerIterator(SAttackerNode* node) : _M_node(node) {}
    bool operator!=(const CAttackerIterator& o) const { return _M_node != o._M_node; }
    CAttackerIterator& operator++();                     // 0x006EF790
    SAttackerNode* operator->() const { return _M_node; }
};
struct CAttackerMap {
    SAttackerNode* _M_header;
    unsigned long  _M_node_count;
    unsigned long  _M_compare;
    CAttackerIterator begin() const { return CAttackerIterator(_M_header->left); }
    CAttackerIterator end() const { return CAttackerIterator(_M_header); }
};
struct CCombatantInfo {
    unsigned char _pad00[0x34];
    CAttackerMap  Attackers;                             // +0x34
};
class CCombatManager { public:
    CCombatantInfo* GetCombatantInfo(const CThingCreatureBase* thing);   // 0x006E9360
};
class CTCHeroStats { public: long GetComboMultiplier() const; };   // 0x006D6570
class CTCCarrying { public: CThing* GetThingInPrimarySlot() const; };   // 0x006A1440
class CTCCombat { public:
    bool IsInMelee() const;                              // 0x00733190
    bool CanFlourish() const;                            // 0x00733180
};
class CTCWeapon { public: long GetWeaponClass() const; };       // 0x005DA7F0
class CHeroAbilityStatPackage { public:
    unsigned char _body[0xc];
    bool IsMetBy(const CTCHeroStats* stats) const;       // 0x00851B50
};
struct CIteratorTag { CIteratorTag() { } };
__declspec(noinline) long* __fastcall find(long* first, long* last, const long& value, const CIteratorTag&) { StandInSink((void*)first, (unsigned long)value); return last; }   // 0x008E6FB0

class CMeleeCombatAbilityDef { public:
    unsigned char           _pad00[0x40];
    CDefString              FlourishAnimName;            // +0x40
    CDefString              FlourishResponseAnimName;    // +0x44
    bool                    Flourish;                    // +0x48
    unsigned char           _pad49[0x50 - 0x49];
    long*                   ValidWeaponClasses_start;    // +0x50
    long*                   ValidWeaponClasses_finish;   // +0x54
    long*                   ValidWeaponClasses_end;      // +0x58
    unsigned char           _pad5c[0x68 - 0x5c];
    CHeroAbilityStatPackage StatsNeeded;                 // +0x68
    long                    ComboMultiplierLowerBound;   // +0x74
    long                    ComboMultiplierUpperBound;   // +0x78
    bool IsAbilityValid(CThingCreatureBase* creature);
};

bool CMeleeCombatAbilityDef::IsAbilityValid(CThingCreatureBase* creature)
{
    CTCHeroStats* stats;
    if (creature->GetInterface(&stats, 0x68)) {
        long multiplier = stats->GetComboMultiplier();
        if (ComboMultiplierLowerBound != -1 && multiplier <= ComboMultiplierLowerBound)
            goto fail;
        if (ComboMultiplierUpperBound != -1 && multiplier > ComboMultiplierUpperBound)
            goto fail;
        if (!StatsNeeded.IsMetBy(stats))
            goto fail;
        if (!(creature->Flags6c & 8))
            goto fail;
        CTCCarrying* carrying;
        if (!creature->GetInterface(&carrying, 0x46))
            goto fail;
        CTCCombat* combat;
        if (!creature->GetInterface(&combat, 0x30))
            goto fail;
        if (Flourish) {
            if (!combat->IsInMelee())
                goto fail;
            if (!combat->CanFlourish())
                goto fail;
        }
        CThing* weapon = carrying->GetThingInPrimarySlot();
        long weaponClass = 0;
        if (IS_THING_ALIVE(weapon)) {
            CTCWeapon* tc;
            if (weapon->GetInterface(&tc, 0x48))
                weaponClass = tc->GetWeaponClass();
        }
        long* last = ValidWeaponClasses_finish;
        if (find(ValidWeaponClasses_start, last, weaponClass, CIteratorTag()) == last)
            goto fail;
        if (FlourishAnimName.operator CCharString().GetLength() != 0) {
            CCombatantInfo* info = creature->GetCombatManager()->GetCombatantInfo(creature);
            for (CAttackerIterator it = info->Attackers.begin(); it != info->Attackers.end(); ++it) {
                CThingCreatureBase* attacker = it->second;
                if (!attacker->GetCombatAnimationSet().HasAnimation(((const CDefString&)FlourishAnimName), CAnimFilterNoGroup()))
                    goto fail;
            }
        }
        return true;
    }
fail:
    return false;
}