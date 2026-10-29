// CAIStateGroup_WhisperPotionCombat::Cleanup() -- when PTargetThing is alive and has a stealth component, mark it as in danger from this group's creature (DANGER_STATUS_0)
// Leaf: ?Cleanup@CAIStateGroup_WhisperPotionCombat@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096A730, 112 bytes (body 0x6F + int3), thiscall, `ret`.
// Identity: vtable 0x0128D5EC slot 9 (RTTI .?AVCAIStateGroup_WhisperPotionCombat@@; bases CAIStateGroup_WhisperPotionCombat <- CAIStateGroup_EnemyCreatureBase <- CAIStateGroupBase <- CBaseClass); slot 2 is OnActivate and slot 9 Cleanup in the
//   landed CAIStateGroup_* models (e.g. CAIStateGroup_MinionSitOnSeatBase::Cleanup 0x0090FA60). PTargetThing is the
//   CIntelligentPointer at +0x14 (Ego_r CAIStateGroup_EnemyCreatureBase::PTargetThing, CIntelligentPointer<CThingCreatureBase>).
//   Family G003 (12 bodies that differ only in the member offset and the danger status pushed).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3: the inlined IS_THING_ALIVE pair, then the item for the lookup),
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x36 = CTCStealth, bit 22 of the bitset dword at +0x24),
//   CTCStealth::SetAsInDanger(const CThing&, NStealth::EDangerStatus) 0x00747B80 (Ego_r signature) with PBrain->PCreature.
// Layout: local model of the shared AI state-group shapes (the generated engine headers flatten the vptr and the base, so a
//   virtual override cannot be declared on them). EDangerStatus enumerator names are descriptive (values only).
// Shape: nested ifs as in the landed MinionSitOnSeatBase::Cleanup; the creature argument loads after the component.
class CTCBase;
class CThing;

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
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
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

#pragma pack(push, 1)   // CIntelligentPointer is 1-aligned: Ego_r puts CAIStateGroup_Thunder::PTargetThing at +0x1D
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
#pragma pack(pop)

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

namespace NStealth { enum EDangerStatus { DANGER_STATUS_0 = 0, DANGER_STATUS_2 = 2, DANGER_STATUS_3 = 3 }; }

class CTCStealth { public:                             // interface type 0x36
    void SetAsInDanger(const CThing& source, NStealth::EDangerStatus status);   // 0x00747B80
};

class CAIStateGroupBase { public:
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

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;       // +0x14 CIntelligentPointer<CThingCreatureBase>
};

struct CAIStateGroup_WhisperPotionCombat : CAIStateGroup_EnemyCreatureBase {
    void* TrapThings[3];                       // +0x1c std::vector<CIntelligentPointer<CThing> > (12-byte STLport vector)
    long FrameLastRun;                         // +0x28
    virtual void Cleanup();
};

void CAIStateGroup_WhisperPotionCombat::Cleanup()
{
    if (IS_THING_ALIVE(PTargetThing)) {
        CTCStealth* pStealth;
        if (PTargetThing.GetPItem()->GetInterface(&pStealth, 0x36))
            pStealth->SetAsInDanger(*PBrain->PCreature, NStealth::DANGER_STATUS_0);
    }
}