// CCreatureAction_GetUpFromSitting::Initialise() -- run the base initialisation, then (unless the sitting mode is removed on PerformAction) take the creature out of its sitting mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7920, length 126 bytes, plain `ret`.
// Callees: CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CVectorMap::LowerBound @ 0x0040F020, CTCCreatureModeManager::RemoveMode(ulong) @ 0x0073ABA0.
// Shape: the base Initialise call first, SittingMode (+0x114) non-zero and the RemoveOnPerformAction byte (+0x118) tested through eax/al; the
//   out-parameter GetInterface template for the mode manager (bit 0x31 = dword +0x24 test, out pointer
//   uninitialised so the absent path reads the dead frame word) and RemoveMode(SittingMode), tail-duplicated; the
//   map pointer's esi is pushed only inside the lookup branch.
// Layout: Ego_r.pdb puts SittingMode/RemoveOnPerformAction at +0x12c/+0x130 (PDB CCombatActionBase 0x12c bytes,
//   retail 0x114): +0x114/+0x118 here; no engine header exists for this class.
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
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01();
    virtual void Initialise();              // 0x00857520
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_GetUpFromSitting : public CCombatActionBase { public:
    long SittingMode;                       // +0x114 (PDB +0x12c)
    bool RemoveOnPerformAction;             // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_GetUpFromSitting::Initialise()
{
    CCombatActionBase::Initialise();
    if (SittingMode != 0 && !RemoveOnPerformAction) {
        CTCCreatureModeManager* modes;
        PCreature.GetPItem()->GetInterface(&modes, 0x31);
        modes->RemoveMode(SittingMode);
    }
}