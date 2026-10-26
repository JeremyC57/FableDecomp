// CCreatureAction_ScreamerAdvance::FrameUpdate() -- past 30% of the animation, end the action (TotalTime = 1) once the creature's mode manager holds mode 0x44; then the base FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8CB0, length 113 bytes (+ int3 padding).
// Callees: CCreatureActionBase::GetAnimationFraction @ 0x006925E0 (float: elapsed share of the action animation; the manifest
// "CInputTypeMouseMovementEvent::operator==" label is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::IsModeActive(unsigned long) @ 0x0073A8A0, CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call).
// Float constant: 0x01238170 = 0.3f. `fcomp ; test ah,41h ; jne` skips on "<=", i.e. the body runs for `fraction > 0.3f`.
// Shape: the mode manager pointer is NULL-INITIALISED (`xor esi,esi` before the GetPItem call), so it lives in esi rather than in the
// frame word of the lookup key and the miss path falls through with esi = 0 into the single HasMode call (no tail duplication, unlike
// the uninitialised form of 0x008C4780). TotalTime is the CCreatureActionBase long at +0x10 (PDB name).
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCreatureModeManager { public:             // interface type 0x31 component
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void FrameUpdate();             // 0x00859D80
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    long TotalTime;                         // +0x10
    unsigned char _pad14[0x114 - 0x14];     // +0x14
    float GetAnimationFraction();           // 0x006925E0 (CCreatureActionBase)
};

class CCreatureAction_ScreamerAdvance : public CCombatActionBase { public:
    virtual void FrameUpdate();
};

void CCreatureAction_ScreamerAdvance::FrameUpdate()
{
    if (GetAnimationFraction() > 0.3f) {
        CTCCreatureModeManager* pModes = 0;
        PCreature.GetPItem()->GetInterface(&pModes, 0x31);
        if (pModes->IsModeActive(0x44))
            TotalTime = 1;
    }
    CCombatActionBase::FrameUpdate();
}