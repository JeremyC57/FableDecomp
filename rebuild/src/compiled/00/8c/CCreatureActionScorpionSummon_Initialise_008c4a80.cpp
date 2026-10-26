// CCreatureAction_ScorpionSummon::Initialise() -- base Initialise, put the scorpion's mode manager into mode 0x38, and if the creature's +0x10 state byte is 1 suspend its animation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4A80, length 117 bytes (+ int3 padding).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// three times), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x31), CTCCreatureModeManager::AddMode @ 0x0073AE10,
// CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10.
// Shape: the mode-manager lookup's result is not tested, so the absent arm calls through the never-written local (it reads the dead
// key frame word straight into ecx, as retail); the call is spelled in BOTH arms so VC7.1 cross-jumps them with the component already
// in ecx (one call after a joined pointer copies it through eax, +2 bytes). PCreature is addressed once (`lea edi,[esi+8]`); the last
// creature is a named local because retail fetches it before pushing the two arguments.
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type) ? map->last : it;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
class CTCCreatureModeManager { public:             // interface type 0x31
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                  // +0x10 creature state byte (compared with 1; meaning not pinned down)
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};
struct CCreaturePointer : CBaseIntelligentPointer {
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)GetPItem(); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CCreaturePointer PCreature;             // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_ScorpionSummon : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_ScorpionSummon::Initialise()
{
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modes;
    CThing* creature = PCreature.GetPItem();
    if (creature->HasTC(0x31))
        ((CTCCreatureModeManager*)creature->FindEntry(0x31)->component)->AddMode(0x38);
    else
        modes->AddMode(0x38);
    if (PCreature.GetCreature()->m_state == 1) {
        CThingCreatureBase* self = PCreature.GetCreature();
        self->SetSuspendAnimation(true, 0);
    }
}