// CAIStateGroup_BriarRoseWillUse::Cleanup() -- when PEnemy is alive and has a stealth component, mark it as in danger from this group's creature (DANGER_STATUS_2)
// Leaf: ?Cleanup@CAIStateGroup_BriarRoseWillUse@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956E00, 112 bytes (body 0x6F + int3), thiscall, `ret`.
// Identity: vtable 0x0128C3C8 slot 9 (RTTI .?AVCAIStateGroup_BriarRoseWillUse@@; bases CAIStateGroup_BriarRoseWillUse <- CAIStateGroupBase <- CBaseClass); slot 2 is OnActivate and slot 9 Cleanup in the
//   landed CAIStateGroup_* models (e.g. CAIStateGroup_MinionSitOnSeatBase::Cleanup 0x0090FA60). PEnemy is the
//   CIntelligentPointer at +0x14 (Ego_r CAIStateGroup_BriarRoseWillUse::PEnemy, CIntelligentPointer<CThing>).
//   Family G003 (12 bodies that differ only in the member offset and the danger status pushed).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3: the inlined IS_THING_ALIVE pair, then the item for the lookup),
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x36 = CTCStealth, bit 22 of the bitset dword at +0x24),
//   CTCStealth::SetAsInDanger(const CThing&, NStealth::EDangerStatus) 0x00747B80 (Ego_r signature) with PBrain->PCreature.
// Layout: local model of the shared AI state-group shapes (the generated engine headers flatten the vptr and the base, so a
//   virtual override cannot be declared on them). EDangerStatus enumerator names are descriptive (values only).
// Shape: nested ifs as in the landed MinionSitOnSeatBase::Cleanup; the creature argument loads after the component.
struct CTCBase;
struct CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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

#pragma pack(push, 1)   // CIntelligentPointer is 1-aligned: Ego_r puts CAIStateGroup_Thunder::PTargetThing at +0x1D
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(pop)

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

namespace NStealth { enum EDangerStatus { DANGER_STATUS_0 = 0, DANGER_STATUS_2 = 2, DANGER_STATUS_3 = 3 }; }

struct CTCStealth {                             // interface type 0x36
    void SetAsInDanger(const CThing& source, NStealth::EDangerStatus status);   // 0x00747B80
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01();
    virtual void OnActivate();                  // slot 2
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_BriarRoseWillUse : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThing>
    long NextWillUseFrame;                     // +0x1c
    virtual void Cleanup();
};

void CAIStateGroup_BriarRoseWillUse::Cleanup()
{
    if (IS_THING_ALIVE(PEnemy)) {
        CTCStealth* pStealth;
        if (PEnemy.GetPItem()->GetInterface(&pStealth, 0x36))
            pStealth->SetAsInDanger(*PBrain->PCreature, NStealth::DANGER_STATUS_2);
    }
}