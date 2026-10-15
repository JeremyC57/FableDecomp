// Grade: DIFFER(ours v retail) -- 136v136 register allocation only: retail keeps `this` in edi and &PTargetThing / the map pointer in esi; the lane swaps them (same gap as 0x0095A420)
// CAIStateGroup_MazeShieldedChase::IsStillRunnable() -- still runnable while the target lives, the group runs and the creature has interface 0x93; otherwise reset its will-power indicator and stop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954740, length 136 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2, PTargetThing +0x14), CAIStateGroupBase::IsStopping
// @ 0x008FD010, CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0xce =
// CTCAICreatureWillPowerIndicator, `test ch,40h` on the presence dword at +0x38),
// CTCAICreatureWillPowerIndicator::Reset(float) @ 0x007E0E70.
//
// Shape: `if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) return false;` (the inlined alive test with its
// double GetPItem, `&PTargetThing` kept in esi), then a raw presence test of interface 0x93 (bit 19 of the
// dword at +0x30) returning true, then the out-parameter GetInterface guarding `Reset(-1.0f)` (pushed as the
// immediate 0BF800000h) and `return false`; the false exits share the `xor al,al` tail placed before the
// `mov al,1` one. The creature is a named local. The engine header CAIStateGroup_MazeShieldedChase.h is
// flattened data-only: local model; interface 0x93 is not identified.
// Tried: the flat `if (HasTC) return true;` (true exit inlined), this nested form (only the register swap left),
// a template IS_THING_ALIVE with split tests, a base-class IsStillValid() helper -- `this` always lands in esi.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];                 // +0x4c..+0x90
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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

struct CTCAICreatureWillPowerIndicator {        // interface type 0xce
    void Reset(float value);                    // 0x007E0E70
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    bool IsStopping();                          // 0x008FD010
};

struct CAIStateGroup_MazeShieldedChase : CAIStateGroupBase {
    CBaseIntelligentPointer PTargetThing;       // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool IsStillRunnable();
};

bool CAIStateGroup_MazeShieldedChase::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping())
        return false;
    CThing* creature = PBrain->PCreature;
    if (!creature->HasTC(0x93)) {
        CTCAICreatureWillPowerIndicator* indicator;
        if (creature->GetInterface(&indicator, 0xce))
            indicator->Reset(-1.0f);
        return false;
    }
    return true;
}
