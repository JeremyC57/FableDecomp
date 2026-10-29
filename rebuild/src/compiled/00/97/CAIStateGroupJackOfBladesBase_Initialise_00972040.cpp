// CAIStateGroup_JackOfBladesBase::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) -- base initialisation, then reset the boss component to phase 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972040, length 120 bytes (+ int3 padding). Vtable slot 1 of
// CAIStateGroup_JackOfBladesBase (0x0128D384).
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (the five arguments forwarded straight from their
// slots), CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0xad = CTCBoss, bit 13 of the
// presence dword at +0x34: `test ch,20h`), CTCBoss::SetNextBossPhase(long, bool) @ 0x007AEDA0.
//
// Shape: the out-parameter GetInterface template with its result ignored (the landed 0x0095AA00 shape):
// the key temporary and the never-written boss local of the miss path share the dead fifth argument
// slot (`[esp+18h]`), both arms join in esi; then SetNextBossPhase(0, false) and the boss dword at +0x10
// cleared. The engine header CAIStateGroup_JackOfBladesBase.h is flattened data-only: local model.
class CTCBase;
class CAIBrain;
class CCharString;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_DEFAULT };

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

class CTCBoss { public:                                // interface type 0xad
    unsigned char _pad[0x10];
    long m_phaseCounter;                        // +0x10 (descriptive)
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
};

struct CAIBrainModel {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long scriptPriority,
        EScriptAIPriority priority, const CCharString& name);   // slot 1, base @ 0x008FCEC0
    CAIBrainModel* PBrain;                      // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
};

struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long scriptPriority,
        EScriptAIPriority priority, const CCharString& name);
};

void CAIStateGroup_JackOfBladesBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long scriptPriority,
    EScriptAIPriority priority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, scriptPriority, priority, name);
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 0xad);
    boss->SetNextBossPhase(0, false);
    boss->m_phaseCounter = 0;
}