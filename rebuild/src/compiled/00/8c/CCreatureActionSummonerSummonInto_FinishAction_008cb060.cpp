// CCreatureAction_SummonerSummonInto::FinishAction() -- base FinishAction, then when the summoner has a mode manager and the target is present and alive, add a new summoning mode (owner, target)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB060, length 186 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager), operator new @ 0x00BFEA1A (0x90 bytes), CModeSummonerSummon::CModeSummonerSummon(CThing*, CThing*)
// @ 0x008CC0B0 (vtable 0x01282BA0), CCountedPointer<CModeBase>::CCountedPointer(CModeBase*) @ 0x00718A00 (out of
// line, built in the argument area), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0.
// Shape: the landed CCreatureAction_BriarRoseIntoInvulnerable::Initialise (0x008C1E80) with the base FinishAction in place of the
// animation prelude (`this` kept in edi until the out pointer spill reuses the frame word): the owner argument goes through the intelligent pointer's inline `operator CThing*()` (fetched
// before the target), the `new` null test selects 0 for the counted pointer.
// Layout: owner CIntelligentPointer +8, target +0xa8.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};

struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;                            // +0
    void* m_pCount;                             // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeSummonerSummon : CModeBase {
    unsigned char _pad[0x90];
    CModeSummonerSummon(CThing* owner, CThing* target);         // 0x008CC0B0
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    protected: virtual void FinishAction(); public:                // 0x00859A20
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
};

class CCreatureAction_SummonerSummonInto : public CCombatActionBase {
public:
    virtual void FinishAction();
};

void CCreatureAction_SummonerSummonInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCCreatureModeManager* modeManager;
    if (((CThing*)PCreature.GetPItem())->GetInterface(&modeManager, 0x31) && PTarget.GetPItem() && !(((CThing*)PTarget.GetPItem())->m_flags91 & 1))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeSummonerSummon(PCreature, ((CThing*)PTarget.GetPItem()))));
}