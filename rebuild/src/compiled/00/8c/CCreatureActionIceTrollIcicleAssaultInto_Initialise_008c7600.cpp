// CCreatureAction_IceTrollIcicleAssaultInto::Initialise() -- animation "ICICLE_ASSAULT_INTO", base Initialise, bump the boss phase when the creature is a boss, then (mode manager present, target alive) push a new CModeIceTrollGroundSmashIcicleAssault(owner, target)
// Leaf: ?Initialise@CCreatureAction_IceTrollIcicleAssaultInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C7600, 292 bytes, virtual Initialise.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("ICICLE_ASSAULT_INTO" 0x012817DC), SetCombatAnimation 0x008574D0,
//   CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound
//   0x0040F020 (inlined GetInterface: 0xAD = CTCBoss, bit 13 of +0x34; 0x31 = CTCCreatureModeManager, bit 17 of +0x24),
//   CTCBoss::IncrementBossPhase 0x007AED20, operator new 0x00BFEA1A (0x9c bytes),
//   NCreatureMode::CModeIceTrollGroundSmashIcicleAssault(CThing*, CThing*) 0x008C7C00 (landed),
//   CCountedPointer<CModeBase>(CModeBase*) 0x00718A00 (built in the argument area), CTCCreatureModeManager::AddMode 0x0073ACC0.
// Shape: the landed CCreatureAction_BriarRoseIntoInvulnerable::Initialise 0x008C1E80 model (named animation string in its
//   own block, the owner through the intelligent pointer's inline `operator CThing*()` evaluated before the target's
//   GetPItem) with the boss-phase lookup in front; the target is the combat action's +0xa8 pointer.
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
class CThingCreatureBase : public CThing { public: };

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;                            // +0
    void* m_pCount;                             // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeIceTrollGroundSmashIcicleAssault : CModeBase {
    unsigned char _pad[0x9c];
    CModeIceTrollGroundSmashIcicleAssault(CThing* thing, CThing* target);   // 0x008C7C00
};
class CTCBoss { public:                                // interface type 0xAD
    void IncrementBossPhase();                  // 0x007AED20
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_IceTrollIcicleAssaultInto : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCreatureAction_IceTrollIcicleAssaultInto::Initialise()
{
    {
        CCharString name("ICICLE_ASSAULT_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCBoss* boss;
    if (PCreature.GetPItem()->GetInterface(&boss, 0xAD))
        boss->IncrementBossPhase();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31) && PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeIceTrollGroundSmashIcicleAssault(PCreature, PTarget.GetPItem())));
}