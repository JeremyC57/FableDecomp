// CCreatureAction_TrollTunnel::PerformAction() -- whack the ground, put the troll into mode 0x1e (tunnelling) through its mode manager (interface type 0x31), then refresh the player GUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C63C0, length 97 bytes (+ int3 padding).
// Callees: CCreatureAction_TrollWhackGroundBase::WhackGround @ 0x008C61C0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10 (bool result dropped), CPlayerGui::Refresh(long) @ 0x004378C0 (landed naming of
// 0x0089EEC0; the manifest "_Cons_val<...>" label is propagated) on the global g_pPlayerGui @ 0x013B8790.
// Shape: the mode manager is an UNINITIALISED out-parameter used unconditionally; here the two paths JOIN (`jmp` to the shared AddMode push) because
// more code follows the call; the miss path reads the never-written frame word into eax. The GUI pointer is loaded straight into ecx after AddMode.
// The PDB header (CCreatureAction_TrollTunnel.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
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
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
class CPlayerGui { public:
    void Refresh(long flag);                // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;            // 0x013B8790

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};
class CCreatureAction_TrollWhackGroundBase : public CCombatActionBase { public:
    void WhackGround();                     // 0x008C61C0
};
class CCreatureAction_TrollTunnel : public CCreatureAction_TrollWhackGroundBase { public:
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