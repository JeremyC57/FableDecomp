// CCreatureAction_CastStrengthMultiStrike::Initialise() -- make sure the creature has a CTCHaste, register this action's 1.75x multiplier, set the MULTI_STRIKE_FRONT animation, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_CastStrengthMultiStrike
// Grade: DIFFER(254v244) -- VC rotates the HasTC loop (test duplicated at the bottom, alignment nops) where retail keeps test-at-top + jmp; lower_bound first pointer in esi vs eax
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
// Shape: a read-ahead `creature` local: `while (!creature->HasTC(0x8C)) { AddTC("CTCHaste", true, 0); creature =
//   PCreature.GetPItem(); }` (the bit mask hoisted into edi, no loop rotation; the lookup reuses the loop's creature), then the component lookup, insert(pair(this, 1.75f)) with the pair and the hidden result in the
//   frame, the recalculation and the animation name (implicit conversions of the literals, so the strings are
//   re-addressed with `lea`).
struct CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct SInterfaceEntry { long type; CTCBase* component; };
__forceinline SInterfaceEntry* LowerBound(SInterfaceEntry* first, SInterfaceEntry* last, long key)
{
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SInterfaceEntry* middle = first + half;
        if (middle->type < key) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20 std::bitset of interface types
    SInterfaceEntry* m_interfacesFirst;               // +0x44
    SInterfaceEntry* m_interfacesLast;                // +0x48
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        SInterfaceEntry* it = LowerBound(m_interfacesFirst, m_interfacesLast, type);
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
struct CTCHaste {
    unsigned char _pad00[0x18];
    CMultiplierMap Multipliers;                       // +0x18
    void RecalculateMultiplier();                     // 0x00800020
};

struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
struct CCreatureAction_CastStrengthMultiStrike : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_CastStrengthMultiStrike::Initialise()
{
    CThing* creature = PCreature.GetPItem();
    while (!creature->HasTC(0x8C)) {
        PCreature.GetPItem()->AddTC("CTCHaste", true, 0);
        creature = PCreature.GetPItem();
    }
    CTCHaste* haste = (CTCHaste*)creature->GetTC(0x8C);
    haste->Multipliers.insert(CMultiplierEntry(this, 1.75f));
    haste->RecalculateMultiplier();
    SetAnimationName("MULTI_STRIKE_FRONT");
    CCombatActionBase::Initialise();
}
