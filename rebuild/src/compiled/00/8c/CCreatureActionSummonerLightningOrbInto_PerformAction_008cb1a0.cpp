// CCreatureAction_SummonerLightningOrbInto::PerformAction() -- when the target is present and alive and the summoner has a mode manager, add a new CModeSummonerLightningOrb(owner, target)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB1A0, length 175 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager), operator new @ 0x00BFEA1A (0x90 bytes), CModeSummonerLightningOrb::CModeSummonerLightningOrb(CThing*,
// CThing*) @ 0x008CC180 (vtable 0x01282BDC), CCountedPointer<CModeBase>::CCountedPointer(CModeBase*) @ 0x00718A00 (out of
// line, built in the argument area), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0.
// Shape: the landed CCreatureAction_BriarRoseIntoInvulnerable::Initialise (0x008C1E80) without the animation prelude and with
// the target tests first: the owner argument goes through the intelligent pointer's inline `operator CThing*()` (fetched
// before the target), the `new` null test selects 0 for the counted pointer.
// Layout: owner CIntelligentPointer +8, target +0xa8.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
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

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};

struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;                            // +0
    void* m_pCount;                             // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeSummonerLightningOrb : CModeBase {
    unsigned char _pad[0x90];
    CModeSummonerLightningOrb(CThing* owner, CThing* target);   // 0x008CC180
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
};

class CCreatureAction_SummonerLightningOrbInto : public CCombatActionBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_SummonerLightningOrbInto::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1) && PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeSummonerLightningOrb(PCreature, PTarget.GetPItem())));
}