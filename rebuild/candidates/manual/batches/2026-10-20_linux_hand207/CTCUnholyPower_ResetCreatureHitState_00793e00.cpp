// CTCUnholyPower::ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> creature) -- for a live creature drop its mode 0x5F and re-arm the hit state of its interface-0x8B component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00793E00, length 190 bytes (+ int3 padding). __thiscall, `ret 8`
//         (the 8-byte intelligent pointer by value; `this` is never read and the callee destroys the parameter).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inlined IS_THING_ALIVE, then one fetch per block),
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x31 = CTCCreatureModeManager,
//          word 1 bit 17; interface 0x8B, word 4 bit 11), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0,
//          ~CBaseIntelligentPointer @ 0x00A01C10.
// Label: the catalog says CTCDivineWrath; all six callers sit in the CTCUnholyPower TU (0x00794DF8 .. 0x00796F8F) and the
//        CTCDivineWrath copy of this body is 0x0078E900.
// Shape: the out-parameter GetInterface template (component left in eax, `mov ecx,eax` after the push); both lookup keys
//        share the `push ecx` frame word.
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
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
    CThing* m_pItem;                                // +0x04
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCCreatureModeManager {                     // interface 0x31
    void RemoveMode(unsigned long mode);            // 0x0073ABA0
};

struct CTCHitState_8b {                             // interface 0x8B component (class not identified)
    unsigned char _pad00[0x10];
    long CurrentHitFrame;                           // +0x10
    unsigned char _pad14[4];
    bool Reset;                                     // +0x18
    unsigned char _pad19[3];
    long ResetHitFrame;                             // +0x1C
};

struct CTCUnholyPower {
    virtual void v00();
    void ResetCreatureHitState(CIntelligentPointerCreature creature);
};

void CTCUnholyPower::ResetCreatureHitState(CIntelligentPointerCreature creature)
{
    if (IS_THING_ALIVE(creature)) {
        CTCCreatureModeManager* modes;
        if (creature.GetPItem()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x5f);
        CTCHitState_8b* hitState;
        if (creature.GetPItem()->GetInterface(&hitState, 0x8b)) {
            hitState->Reset = true;
            hitState->ResetHitFrame = hitState->CurrentHitFrame;
        }
    }
}
