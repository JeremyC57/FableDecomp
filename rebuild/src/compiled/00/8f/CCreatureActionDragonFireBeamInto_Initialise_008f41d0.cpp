// CCreatureAction_DragonFireBeamInto::Initialise() -- animation "FIRE_BEAM_INTO", base Initialise, then (mode manager present) push a new CModeDragonFireBeam(owner, target, Timer) onto the owner's mode manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F41D0, length 199 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("FIRE_BEAM_INTO" @ 0x012884CC), SetCombatAnimation 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager), operator new 0x00BFEA1A (0x94 bytes),
// CModeDragonFireBeam::CModeDragonFireBeam(CThing*, CThing*, float) 0x008D86E0, CCountedPointer<CModeBase>::CCountedPointer(CModeBase*)
// 0x00718A00, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: as CCreatureAction_DragonBurninateInto::Initialise 0x008F4320 (in-chunk): both pointer arguments go through the inline
// `operator CThing*()`, so both GetPItem calls run (right to left: target, owner) before any argument is pushed; the manager
// out-parameter is then spilled to the frame across the new (plain `.GetPItem()` arguments interleave with the pushes).
// Layout: Timer at retail +0x114 (PDB +0x12c); the combat target pointer at +0xa8.
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
struct CModeDragonFireBeam : CModeBase {
    unsigned char _pad[0x94];
    CModeDragonFireBeam(CThing* owner, CThing* target, float timer);   // 0x008D86E0
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

class CCreatureAction_DragonFireBeamInto : public CCombatActionBase {
public:
    float Timer;                                // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_DragonFireBeamInto::Initialise()
{
    {
        CCharString name("FIRE_BEAM_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeDragonFireBeam(PCreature, PTarget, Timer)));
}