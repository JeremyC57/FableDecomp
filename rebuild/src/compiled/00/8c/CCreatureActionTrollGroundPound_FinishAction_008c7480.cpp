// CCreatureAction_TrollGroundPound::FinishAction() -- if the creature has the haste component (interface type 0x8c), remove this action from its set of speed sources and let it recompute the speed; then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7480, length 99 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for
// type 0x8c = bit 12 of the bitset dword at +0x30: `mov ecx,[eax+30h] ; test ch,10h`), the set erase-by-key @ 0x0058119B (std::set<CCreatureActionBase*>
// at +0x18 of the component: equal_range + erase; the manifest "CBankFile::CompareDataOffset::operator()" label is propagated), the component's
// speed recomputation @ 0x00800020 (between the CTCHaste bodies 0x007FFFB0/0x00800000; manifest "CAnimation_RecalculateScale" is propagated),
// CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, last).
// Shape: the out-parameter GetInterface as the if (component in esi, pushed only inside); `this` is passed to the erase by const reference, so it is
// copied into the frame word the lookup key used (`mov [esp+0Ch],edi` after the `lea ; push`).
// No engine header for this class; modelled locally (CCombatActionBase is 0x114 bytes in retail).
class CTCBase;
class CCreatureActionBase;
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
struct CActionSet {                         // std::set<CCreatureActionBase*>
    unsigned long erase(CCreatureActionBase* const& key);   // 0x0058119B
};
class CTCHaste { public:                           // interface type 0x8c component
    unsigned char _pad0[0x18];
    CActionSet SpeedSources;                // +0x18
    void RecalculateSpeed();                // 0x00800020
};

class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    virtual void FinishAction();            // 0x00859A20
};
class CCreatureAction_TrollGroundPound : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_TrollGroundPound::FinishAction()
{
    CTCHaste* pHaste;
    if (PCreature.GetPItem()->GetInterface(&pHaste, 0x8c)) {
        pHaste->SpeedSources.erase(this);
        pHaste->RecalculateSpeed();
    }
    CCombatActionBase::FinishAction();
}