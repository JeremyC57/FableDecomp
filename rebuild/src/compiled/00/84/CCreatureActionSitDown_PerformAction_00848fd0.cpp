// CCreatureAction_SitDown::PerformAction() -- push the action's sitting mode onto the creature's mode manager (interface type 0x31)
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model into an UNINITIALISED local (the miss arm reads the key slot): both arms load the mode manager straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848FD0, length 105 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface
// lookup for type 0x31 = bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>)
// @ 0x0073ACC0 (bool result dropped).
// Shape: the mode manager is an UNINITIALISED out-parameter used unconditionally (the miss path loads the never-written frame word into ecx and
// joins the hit path). The counted pointer argument is passed BY VALUE: its inline copy constructor builds it in the argument area
// (`sub esp,8 ; mov edx,esp`), copies the object word, then the count block word with the null test around the store and the add-ref
// `inc dword ptr [eax]` on the block's reference count.
// Layout: PMode (CCountedPointer<CModeSitBase>) at +0xa8/+0xac; the PDB header (CCreatureAction_SitDown.h, 0xbc bytes) sits 0x0c high
// against retail's 0xa8-byte CCreatureActionBase, so `this` is modelled locally.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CModeBase;
struct SCountBlock {                        // the shared count block of a CCountedPointer
    long m_references;                      // +0x00
    void (__fastcall* m_pfnDestroy)(CModeBase*);   // +0x04
    CModeBase* m_pObject;                   // +0x08
};
struct CCountedPointer_CModeBase {          // {object, count block}
    CModeBase*   m_pObject;                 // +0x00
    SCountBlock* m_pCount;                  // +0x04
    CCountedPointer_CModeBase(const CCountedPointer_CModeBase& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer_CModeBase();           // the callee destroys its by-value parameter
};
struct CTCCreatureModeManager {             // interface type 0x31 component
    bool AddMode(CCountedPointer_CModeBase mode);   // 0x0073ACC0
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    CCountedPointer_CModeBase PMode;        // +0xa8 CCountedPointer<CModeSitBase>
    virtual void PerformAction();
};

void CCreatureAction_SitDown::PerformAction()
{
    CTCCreatureModeManager* pModes;
    CThing* creature = PCreature.GetPItem();
    if (creature->HasTC(0x31))
        pModes = creature->GetTC<CTCCreatureModeManager>(0x31);
    pModes->AddMode(PMode);
}