// CCreatureAction_NymphTeleportIntoLimbo::PerformAction() -- if the creature has the nymph component (interface type 0xf5), start it flying around for the remaining time (the words at +0x10 and +0x1c), then do the limbo effect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA110, length 92 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0xf5 =
// bit 21 of the bitset dword at +0x3c, landed shape of 0x00849320), CTCNymph::StartFlyingAround(long) @ 0x0080F5E0, CCreatureAction_NymphTeleportLimboBase::DoEffect
// @ 0x008C9FE0 (non-virtual). Shape: one `push ecx` frame for the lookup key; the action rides in edi, the map in esi.
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
class CTCNymph { public:
    void StartFlyingAround(long duration);  // 0x0080F5E0
};

class CCreatureAction_NymphTeleportLimboBase { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08
    long m_duration;                        // +0x10
    unsigned char _pad14[8];
    long m_elapsed;                         // +0x1c
    void DoEffect();                        // 0x008C9FE0
};
class CCreatureAction_NymphTeleportIntoLimbo : public CCreatureAction_NymphTeleportLimboBase { public:
    virtual void PerformAction();
};

void CCreatureAction_NymphTeleportIntoLimbo::PerformAction()
{
    CTCNymph* pNymph;
    if (PCreature.GetPItem()->GetInterface(&pNymph, 0xf5))
        pNymph->StartFlyingAround(m_duration - m_elapsed);
    DoEffect();
}