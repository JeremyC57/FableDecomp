// CCreatureAction_ForcePushOutOf::PerformAction() -- add a CTCForcePushPower component to the caster (ignoring friendlies when OnlyHitEnemies), then leave creature mode 0x5b
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BA0B0, length 143 bytes, plain `ret`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString
//          @ 0x0099EAE0, CVectorMap::LowerBound @ 0x0040F020, CTCCreatureModeManager::RemoveMode(ulong) @ 0x0073ABA0.
// Shape: the component name is a named CCharString in its own block, built before the argument pushes and
//   destroyed right after AddTC; the returned component rides in ebx (declared outside the block) for the
//   IgnoreFriendlies byte store (+0x24, Ego_r.pdb CTCForcePushPower); the mode-manager lookup (bool out-parameter
//   GetInterface, bit 0x31) reuses the string's dead frame word for its key.
// Layout: Ego_r.pdb puts OnlyHitEnemies at +0x12c (PDB CCombatActionBase 0x12c bytes, retail 0x114): +0x114 here;
//   engine/CCreatureAction_ForcePushOutOf.h is the PDB build and is not used.
class CTCBase;
class CTCParamsBase;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
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

class CTCForcePushPower { public:                  // Ego_r.pdb, 0x28 bytes
    unsigned char _pad00[0x24];
    bool IgnoreFriendlies;                  // +0x24
};

class CTCCreatureModeManager { public:             // interface type 0x31 component
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_ForcePushOutOf : public CCombatActionBase { public:
    bool OnlyHitEnemies;                    // +0x114 (PDB +0x12c)
    virtual void PerformAction();
};

void CCreatureAction_ForcePushOutOf::PerformAction()
{
    CTCForcePushPower* power;
    {
        CCharString name("CTCForcePushPower", -1);
        power = (CTCForcePushPower*)PCreature.GetPItem()->AddTC(name, true, 0);
    }
    if (OnlyHitEnemies)
        power->IgnoreFriendlies = true;
    CTCCreatureModeManager* modes;
    if (PCreature.GetPItem()->GetInterface(&modes, 0x31))
        modes->RemoveMode(0x5b);
}