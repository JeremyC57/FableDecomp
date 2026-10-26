// CCreatureActionBase::Initialise() -- mark initialised; for an action with an animation entry that is not a combination anim, disable four creature-control flags (0x24, 0x25, 0x1f, 0x23) on the creature's interface-0x5A controller; update the action animation, optionally start at once, clear Finished
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00694D10, 181 bytes, virtual `?Initialise@CCreatureActionBase@@UAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThingCreatureBase::IsAnimCombinationAnim 0x00662820,
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x5A = bit 26 of +0x28), the component's 0x0070B460
//   (returns the flag owner; manifest "CWorld::DrawGetEnvironment" is propagated), 0x0070D8C0 (thiscall, flag id; manifest
//   "CSpellContainerList::AddChild" is propagated), CCreatureActionBase::UpdateActionAnimation 0x00694890, virtual slots
//   57 (+0xe4) and 61 (+0xf4).
// Layout: engine/CCreatureActionBase.h offsets (PCreature +0x08, AddedActionAnim +0x60, Finished +0x61, Initialised +0x63,
//   PAnimEntry_Data +0x74); modelled locally because the header's vptr is a plain void* and this body makes virtual calls.
// Shape: the anim entry pointer is a named local read first (held in ebp); the creature is fetched into a named local
//   before the entry's dword is pushed; the entry's first dword is passed by value.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsAnimCombinationAnim(long animation); // 0x00662820 (CThingCreatureBase)
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
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CControlFlags {
    void DisableFlag(long flag);                // 0x0070D8C0
};

struct CTCInterface5A {                         // interface type 0x5A
    CControlFlags* GetControlFlags();           // 0x0070B460
};

class CAnimationEntry { public:
    long Animation;                             // +0x00
};

class CCreatureActionBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual bool ShouldStartImmediately();      // slot 57 (+0xe4)
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void StartImmediately();            // slot 61 (+0xf4)
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0x60 - 0x10];
    bool AddedActionAnim;                       // +0x60
    bool Finished;                              // +0x61
    bool PerformedAction;                       // +0x62
    bool Initialised;                           // +0x63
    unsigned char _pad64[0x74 - 0x64];
    CAnimationEntry* PAnimEntry_Data;           // +0x74 CCountedPointer<CAnimationEntry>::Data
    void UpdateActionAnimation();               // 0x00694890
    void Initialise();
};

void CCreatureActionBase::Initialise()
{
    CAnimationEntry* entry = PAnimEntry_Data;
    Initialised = true;
    if (entry) {
        CThing* creature = PCreature.GetPItem();
        if (!creature->IsAnimCombinationAnim(entry->Animation)) {
            CTCInterface5A* controller;
            if (PCreature.GetPItem()->GetInterface(&controller, 0x5a)) {
                CControlFlags* flags = controller->GetControlFlags();
                flags->DisableFlag(0x24);
                flags->DisableFlag(0x25);
                flags->DisableFlag(0x1f);
                flags->DisableFlag(0x23);
            }
        }
    }
    UpdateActionAnimation();
    if (ShouldStartImmediately())
        StartImmediately();
    Finished = false;
}