// CCreatureAction_CastWillDrainLifeSpell::PerformAction() -- when the target is alive and the caster has its spell-casting component (interface 8), add a CTCDrainLife component, point the caster's spell at the target, give an AI-state caster the action's cast parameters, and hand a live target thing to the drain
// Leaf: ?PerformAction@CCreatureAction_CastWillDrainLifeSpell@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BA7F0, 375 bytes (384 with padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, ctor 0x00A01B10, SetPItem 0x00A01B90, dtor 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); CVectorMap::LowerBound 0x0040F020 (inlined GetInterface,
//          type 8 = bit 8 of the bitset dword +0x20); CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
//          0x004C9D60 with "CTCDrainLife" 0x0123D18C (CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0); the spell
//          component's virtual slot 34 (+0x88); CTCDrainLife::SetAICast(long, long) 0x007C3040.
//
// Shape: the landed Cast*Spell family (0x008B9970): the target's live test reads PTarget twice; `this` is advanced
// to PCreature in place and re-read from its frame copy (every register is taken); the component name is an
// implicit literal temporary; the drain rides in ebp; the target thing is a named copy assigned through the
// inline by-value operator= when alive.
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PCreature +8, PItemRemoved +0x2c, PTarget +0xa8);
// the action's cast parameters at +0x114 / +0x118 (descriptive names); the drain's target pointer +0x14.
// Frame: the same-TU CVectorMap::LowerBound stand-in (auto_inline(off), above the caller) is what lets the lookup key
// share its frame word with the name temporary and the by-value operator= parameter, as retail (LTCG) does.
class CTCParamsBase;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CThing;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointerThing& operator=(CIntelligentPointerThing other) { SetPItem(other.GetPItem()); return *this; }
};
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): compiled above the caller so VC7.1 knows the key's address does not escape, which lets
// the key share its frame word with later locals (retail's LTCG knowledge).
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
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    CBaseObject* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08 (the owning creature)
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointerThing PItemRemoved;      // +0x2c (CCreatureActionBase, Ego_r.pdb name; the spell's target thing)
    unsigned char _pad34[0xa8 - 0x34];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    virtual void Initialise();                  // 0x00857520
};

struct CTCSpellCasting {                        // interface type 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void SetSpellTarget(CThing* target);    // slot 34 (+0x88)
};
class CTCDrainLife { public:
    unsigned char _pad00[0x14];
    CIntelligentPointerThing PTarget;           // +0x14
    void SetAICast(long level, long power);     // 0x007C3040
};

class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase { public:
    long SpellLevel;                            // +0x114
    long SpellPower;                            // +0x118
    virtual void PerformAction();
};

void CCreatureAction_CastWillDrainLifeSpell::PerformAction()
{
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
        CTCSpellCasting* casting;
        if (PCreature.GetPItem()->GetInterface(&casting, 8)) {
            CTCDrainLife* drain = (CTCDrainLife*)PCreature.GetPItem()->AddTC("CTCDrainLife", true, 0);
            casting->SetSpellTarget(PTarget.GetPItem());
            if (PCreature.GetPItem()->m_state == 1)
                drain->SetAICast(SpellLevel, SpellPower);
            CIntelligentPointerThing target = PItemRemoved;
            if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1))
                drain->PTarget = target;
        }
    }
}