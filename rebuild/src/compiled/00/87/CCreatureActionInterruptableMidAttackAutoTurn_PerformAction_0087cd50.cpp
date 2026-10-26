// CCreatureAction_InterruptableMidAttackAutoTurn::PerformAction() -- if the animation entry's parameters allow it and the target is alive, put the target's mode manager into mode 0x1F
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087CD50, length 145 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_InterruptableMidAttackAutoTurn.h is the flattened PDB layout and sits high against the bytes (CCreatureActionBase is 0xb4 bytes in the PDB, 0xa8 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: the animation entry's parameter getter @ 0x00738A00 (thiscall on PAnimEntry.Data at +0x74, `void (T** out)`: a type-6
// lookup in the entry's own map with a global default map as fallback; manifest "CTCInterface_GetAnimation"),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the target pointer at +0xa8, three times), CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface for type 0x31), CTCCreatureModeManager::AddMode @ 0x0073AE10.
// Shape: the parameters pointer (fetched in an inline accessor, so its out pointer is an inner-scope local) and the GetInterface key share the single `push ecx` frame word (the absent GetInterface arm
// reads it back as the out pointer); IS_THING_ALIVE inlined; AddMode tail-duplicated into both GetInterface arms.
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
    unsigned char _pad4c[0x45];             // +0x4c
    unsigned char m_flags91;                // +0x91 bit 0: dead
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCCreatureModeManager { public:             // interface type 0x31
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
struct CAnimationParameters {
    unsigned char _pad00[8];
    bool m_autoTurn;                        // +0x08 flag tested before turning the target
};
class CAnimationEntry { public:
    void GetParameters(CAnimationParameters** out);   // 0x00738A00
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0x74 - 4];         // +0x04
    CAnimationEntry* PAnimEntry_Data;       // +0x74 CCountedPointer<CAnimationEntry>::Data
    unsigned char _pad78[0xa8 - 0x78];      // +0x78
};
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PTarget;        // +0xa8 the creature being attacked
    void PerformAction();
    // inline accessor: its out pointer lives in an inner scope, so it shares the frame word with the lookup key
    bool GetAutoTurn() const
    {
        CAnimationParameters* params;
        PAnimEntry_Data->GetParameters(&params);
        return params->m_autoTurn;
    }
};

void CCreatureAction_InterruptableMidAttackAutoTurn::PerformAction()
{
    if (GetAutoTurn() && IS_THING_ALIVE(PTarget)) {
        CTCCreatureModeManager* modes;
        PTarget.GetPItem()->GetInterface(&modes, 0x31);
        modes->AddMode(0x1F);
    }
}