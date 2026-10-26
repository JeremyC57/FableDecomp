// CCreatureAction_CastWillHarvestSoulSpell::PerformAction() -- if the target is alive, kill it and tell the caster's special-abilities component (interface 0xa7) that a soul was harvested
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BA970, length 138 bytes, plain `ret`. Vtable slot of
//   CCreatureAction_CastWillHarvestSoulSpell (referenced from 0x0127D2C0).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CVectorMap::LowerBound
//          @ 0x0040F020, CTCSpecialAbilities::OnSoulHarvested @ 0x0072A840 (an empty out-of-line method in retail;
//          the manifest label "$E2" is propagated, the name is descriptive).
// Shape: PTarget's address is taken once (esi); IS_THING_ALIVE inlined (null test, dead bit +0x91); Kill(true) on
//   the target; then the out-parameter GetInterface template on PCreature (+8): bit 0xa7 is bit 7 of the presence
//   dword +0x34 (`mov cl,[eax+34h] ; test cl,cl ; jns`), the out pointer is NOT initialised (the absent path reads
//   the dead frame word, landed 0x007504E0 shape) and the component call is tail-duplicated into both paths.
// Layout: CCreatureActionBase (retail 0xa8 bytes, PCreature at +8) followed by the target pointer at +0xa8; the class
//   is not in Ego_r.pdb and has no engine header.
class CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool removeNow);              // 0x004C9B80
    template <class T> __forceinline void GetInterface(T** ppOut, long interfaceType)   // leaves *ppOut alone when absent
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                      // +0x00
    CThing* m_pItem;                        // +0x04
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

class CTCSpecialAbilities { public:                // interface type 0xa7 component
    void OnSoulHarvested();                 // 0x0072A840
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

class CCreatureAction_CastWillHarvestSoulSpell : public CCreatureActionBase { public:
    CBaseIntelligentPointer PTarget;        // +0xa8 CIntelligentPointer<CThing>
    virtual void PerformAction();
};

void CCreatureAction_CastWillHarvestSoulSpell::PerformAction()
{
    if (IS_THING_ALIVE(PTarget)) {
        PTarget.GetPItem()->Kill(true);
        CTCSpecialAbilities* abilities;
        PCreature.GetPItem()->GetInterface(&abilities, 0xa7);
        abilities->OnSoulHarvested();
    }
}