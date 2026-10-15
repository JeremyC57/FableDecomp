// CCreatureAction_TrollTunnel::PerformAction() -- whack the ground, put the troll into mode 0x1e (tunnelling) through its mode manager (interface type 0x31), then refresh the player GUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C63C0, length 97 bytes (+ int3 padding).
// Callees: CCreatureAction_TrollWhackGroundBase::WhackGround @ 0x008C61C0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10 (bool result dropped), CPlayerGui::Refresh(long) @ 0x004378C0 (landed naming of
// 0x0089EEC0; the manifest "_Cons_val<...>" label is propagated) on the global g_pPlayerGui @ 0x013B8790.
// Shape: the mode manager is an UNINITIALISED out-parameter used unconditionally; here the two paths JOIN (`jmp` to the shared AddMode push) because
// more code follows the call; the miss path reads the never-written frame word into eax. The GUI pointer is loaded straight into ecx after AddMode.
// The PDB header (CCreatureAction_TrollTunnel.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCCreatureModeManager {             // interface type 0x31 component
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
struct CPlayerGui {
    void Refresh(long flag);                // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;            // 0x013B8790

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};
struct CCreatureAction_TrollWhackGroundBase : CCombatActionBase {
    void WhackGround();                     // 0x008C61C0
};
struct CCreatureAction_TrollTunnel : CCreatureAction_TrollWhackGroundBase {
    virtual void PerformAction();
};

void CCreatureAction_TrollTunnel::PerformAction()
{
    WhackGround();
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->AddMode(0x1e);
    g_pPlayerGui->Refresh(0);
}
