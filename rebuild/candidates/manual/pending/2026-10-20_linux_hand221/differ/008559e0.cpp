// CCreatureAction_BlockBroken::Initialise() -- pick the left/right block-broken response animation (random, or the attacker's side), notify the owner's combat component, then the base Initialise
// Leaf: Initialise
// Grade: DIFFER(269v265) -- ours holds the constant 1 in ebx (and eax,ebx / test [eax+0x91],bl; extra push ebx) and swaps this/side between edi/ebp; rest of the body matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x008559E0, 265 bytes (272 with padding), virtual Initialise.
// Callees: CCreatureActionBase::GetWorld 0x00692940 (manifest "_Dest_val" label is STL noise), CWorld random-seed
//   accessor 0x0049D810 (returns &seed), GFROR13 0x00497890 (__fastcall), CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CCharString(const char*, int = -1) 0x0099EBF0 ("BLOCK_BROKEN_RESPONSE_RIGHT" 0x01275F1C /
//   "BLOCK_BROKEN_RESPONSE_LEFT" 0x01275F00), CCombatActionBase::SetAnimationName 0x008574D0, ~CCharString 0x0099EAE0,
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x30 = bit 16 of the word at +0x24: the combat
//   component), combat-component call 0x00733F70 (takes the target thing; name unconfirmed), CCombatActionBase::
//   Initialise 0x00857520 (direct base call).
// Layout: retail CCreatureActionBase is 0xa8 bytes (PCreature +0x08); the target pointer is the combat action's +0xa8;
//   the target's flag byte +0x6c (bit 3) and side word +0x138.
// Shape: the world LCG step (`seed = seed*0x24A1 + 0x24DF; seed = GFROR13(seed)`) in a forceinline helper with
//   `% 2` (`and eax,1`); the animation name is an implicit CCharString conversion of a ternary (`lea eax` for the
//   push); IS_THING_ALIVE is the landed inline (two GetPItem calls) and GetInterface the landed out-parameter form.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                // +0x6c bit 3
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    unsigned char _pad92[0x138 - 0x92];
    long m_side;                            // +0x138 (non-zero: the left response)
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
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCharString {
    void* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

long __fastcall GFROR13(unsigned long value);         // 0x00497890
struct CWorld {
    unsigned long* GetRandomSeed();                   // 0x0049D810
};
__forceinline unsigned long GetRandomBelow(CWorld* world, unsigned long n)
{
    unsigned long* seed = world->GetRandomSeed();
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed % n;
}

struct CTCCombat {
    void OnBlockBroken(CThing* attacker);             // 0x00733F70 (name unconfirmed)
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CWorld* GetWorld() const;               // 0x00692940
};
struct CCombatActionBase : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;        // +0xa8
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                      // 0x00857520
};
struct CCreatureAction_BlockBroken : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_BlockBroken::Initialise()
{
    long side = GetRandomBelow(GetWorld(), 2);
    if (IS_THING_ALIVE(PTarget) && (PTarget.GetPItem()->m_flags6c & 8))
        side = PTarget.GetPItem()->m_side;
    SetAnimationName(side == 0 ? "BLOCK_BROKEN_RESPONSE_RIGHT" : "BLOCK_BROKEN_RESPONSE_LEFT");
    if (IS_THING_ALIVE(PTarget)) {
        CTCCombat* combat;
        if (PCreature.GetPItem()->GetInterface(&combat, 0x30))
            combat->OnBlockBroken(PTarget.GetPItem());
    }
    CCombatActionBase::Initialise();
}
