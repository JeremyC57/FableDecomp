// CCreatureAction_CastStrengthMultiStrike::Initialise() -- make sure the creature has a CTCHaste, register this action's 1.75x multiplier, set the MULTI_STRIKE_FRONT animation, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_CastStrengthMultiStrike
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E6780, 244 bytes (256 with padding), virtual Initialise.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature at +8), CCharString(const char*, int = -1)
//   0x0099EBF0 ("CTCHaste" 0x0123C2A0 / "MULTI_STRIKE_FRONT" 0x01287368), CThing::AddTC(const CCharString&, bool,
//   CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0, map<void*, float>::insert(const value_type&) 0x00580D21
//   (pair<iterator,bool> by hidden pointer; byte twin of the landed set<ulong> insert), CTCHaste::RecalculateMultiplier
//   0x00800020, CCombatActionBase::SetAnimationName 0x008574D0, CCombatActionBase::Initialise 0x00857520.
// Layout: CTCHaste is interface type 0x8C (bit 12 of the presence word at +0x30); its multiplier map at +0x18 (landed
//   CTCHaste::RemoveMultiplier 0x0058148D). The creature's component vector map at +0x44 is read with an INLINE
//   STLport lower_bound here (signed `len/2` halving, `first = middle + 1; len = len - half - 1`), not the
//   out-of-line LowerBound 0x0040F020.
// Shape: a read-ahead `creature` local: `while (creature->LacksTC(0x8C)) { AddTC("CTCHaste", true, 0); creature =
//   PCreature.GetPItem(); }` -- the __forceinline early-return LacksTC condition keeps the loop unrotated (bit mask hoisted
//   into edi); then GetTC runs the lower_bound loop directly on its result iterator `it` (initialised from the first
//   pointer after the length, so the first pointer is copied eax->esi and `it = last` reuses esi), then insert(pair(this, 1.75f)) with the pair and the hidden result in the
//   frame, the recalculation and the animation name (implicit conversions of the literals, so the strings are
//   re-addressed with `lea`).
class CTCBase;
class CTCParamsBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct SInterfaceEntry { long type; CTCBase* component; };
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20 std::bitset of interface types
    SInterfaceEntry* m_interfacesFirst;               // +0x44
    SInterfaceEntry* m_interfacesLast;                // +0x48
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline bool LacksTC(long type) const
    {
        if (HasTC(type)) return false;
        return true;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        int len = int(m_interfacesLast - m_interfacesFirst);
        SInterfaceEntry* it = m_interfacesFirst;
        while (len > 0) {
            int half = len / 2;
            SInterfaceEntry* middle = it + half;
            if (middle->type < type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        if (it == m_interfacesLast || type < it->type)
            it = m_interfacesLast;
        return it->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};

struct CMultiplierEntry {                             // map<void*, float>::value_type
    void* first;
    float second;
    CMultiplierEntry(void* key, float value) : first(key), second(value) { }
};
struct CMultiplierMap {
    struct node;
    struct iterator { node* current; };
    struct insert_result { iterator first; bool second; };
    void* m_head;
    insert_result insert(const CMultiplierEntry& value);   // 0x00580D21
};
class CTCHaste { public:
    unsigned char _pad00[0x18];
    CMultiplierMap Multipliers;                       // +0x18
    void RecalculateMultiplier();                     // 0x00800020
};

class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
class CCreatureAction_CastStrengthMultiStrike : public CCombatActionBase { public:
    void Initialise();
};

void CCreatureAction_CastStrengthMultiStrike::Initialise()
{
    CThing* creature = PCreature.GetPItem();
    while (creature->LacksTC(0x8C)) {
        PCreature.GetPItem()->AddTC("CTCHaste", true, 0);
        creature = PCreature.GetPItem();
    }
    CTCHaste* haste = (CTCHaste*)creature->GetTC(0x8C);
    haste->Multipliers.insert(CMultiplierEntry(this, 1.75f));
    haste->RecalculateMultiplier();
    SetAnimationName("MULTI_STRIKE_FRONT");
    CCombatActionBase::Initialise();
}