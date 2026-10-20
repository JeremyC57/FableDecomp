// CCreatureAction_CastUnholyPowerSpell::Initialise() -- base Initialise, find or add the creature's CTCUnholyPower component, arm it and hand it the action's thing pointer when that thing is alive
// Leaf: ?Initialise@CCreatureAction_CastUnholyPowerSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BAF20, 271 bytes, virtual Initialise. Byte twin of
//   CCreatureAction_CastDivineWrathSpell::Initialise 0x008BAE10 (type 0x85 / "CTCDivineWrath").
// Callees: CCombatActionBase::Initialise 0x00857520 (direct base call, first), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x86 = bit 6 of the word at +0x30),
//   CCharString(const char*, int = -1) 0x0099EBF0 ("CTCUnholyPower" 0x0123C290), CThing::AddTC(const CCharString&, bool,
//   CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Layout: PCreature +0x08 and the base action's CIntelligentPointer<CThing> at +0x2c (PItemRemoved in the PDB layout of
//   CCreatureActionBase; retail's early base offsets agree with the PDB); the component's armed byte +0x18 and its
//   CIntelligentPointer<CThing> at +0x10.
// Shape: the find-or-add is the landed out-parameter GetInterface with the AddTC fallback in the else block (its
//   name string a block-scoped local); the thing pointer is copied into a named CIntelligentPointer (inline copy
//   ctor: base ctor, vtable store, SetPItem(other.GetPItem())), tested with IS_THING_ALIVE, and passed BY VALUE to an
//   inline setter on the component (the copy is the second frame object, sharing the string's slot). The component
//   name is an IMPLICIT CCharString conversion temporary: a named block local takes its own frame word instead of
//   sharing the inlined lookup key's.
struct CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): compiled above the caller so VC7.1 knows the key's address does not escape, which lets
// the key share its frame word with the AddTC name string and the by-value target copy.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCUnholyPower {
    unsigned char _pad00[0x10];
    CIntelligentPointer PTarget;                      // +0x10
    unsigned char m_bArmed;                           // +0x18
    void SetTarget(CIntelligentPointer target) { PTarget = target; }
};

struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer PItemRemoved;                 // +0x2c
    unsigned char _pad34[0x114 - 0x34];
    void Initialise();                                // 0x00857520
};
struct CCreatureAction_CastUnholyPowerSpell : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_CastUnholyPowerSpell::Initialise()
{
    CCombatActionBase::Initialise();
    CTCUnholyPower* power;
    if (!PCreature.GetPItem()->GetInterface(&power, 0x86))
        power = (CTCUnholyPower*)PCreature.GetPItem()->AddTC("CTCUnholyPower", true, 0);
    power->m_bArmed = 1;
    CIntelligentPointer thing(PItemRemoved);
    if (IS_THING_ALIVE(thing))
        power->SetTarget(thing);
}
