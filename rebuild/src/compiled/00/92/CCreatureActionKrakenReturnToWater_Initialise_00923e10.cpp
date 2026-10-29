// CCreatureAction_KrakenReturnToWater::Initialise() -- base Initialise, add creature mode 0x54 through the owner's mode manager, clear the flag byte +0x10 of the owner's interface-9 component and set the owner's ImmuneToAttack bit
// Leaf: ?Initialise@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923E10, length 175 bytes (+ int3 padding), vtable slot 12 of 0x012906F4
// (RTTI `?A0x29922355::CCreatureAction_KrakenReturnToWater`; FableWin.pdb ?Initialise@CCreatureAction_KrakenReturnToWater@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// address kept in edi), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of
// +0x24; then 9, bit 9 of +0x20), CTCCreatureModeManager::AddMode @ 0x0073AE10.
// Shape: both lookups are UNCHECKED accessors (absent arms read the key word); `mov cl ; or cl,40h ; mov` sets the bool
// bit-field CThingAICreature::ImmuneToAttack (bit 6 of +0x1c8; Ego_r.pdb +0x1f8) through a NAMED local (the temporary form adds `add eax,1c8h`).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // a thing's sorted {type, component} vector map at +0x44
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
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
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
class CTCCreatureModeManager { public:                 // interface type 0x31 component
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCInterface9 {                          // the owner's interface-9 component (descriptive, as landed 0x008CF610)
    unsigned char _pad[0x10];
    bool          m_flag10;                     // +0x10
};
class CThingAICreature : public CThing { public:
    unsigned char _padb8[0x1c8 - 0xb8];
    bool PerformingVillageTaskFlag : 1;         // +0x1c8 bit 0 (Ego_r.pdb +0x1f8)
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;                    // bit 6
    bool EnableCreatureAutoPlacing : 1;         // bit 7
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520 (slot 12)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCCreatureModeManager* GetModeManager() const
    {
        CTCCreatureModeManager* modeManager;
        PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
        return modeManager;
    }
    __forceinline CTCInterface9* GetInterface9() const
    {
        CTCInterface9* component;
        PCreature.GetPItem()->GetInterface(&component, 9);
        return component;
    }
    __forceinline CThingAICreature* GetAICreature() const { return static_cast<CThingAICreature*>(PCreature.GetPItem()); }
};
struct CCreatureAction_KrakenReturnToWater : CCombatActionBase {   // vtable 0x012906F4, sizeof 0x114
    virtual void Initialise();
};

void CCreatureAction_KrakenReturnToWater::Initialise()
{
    CCombatActionBase::Initialise();
    GetModeManager()->AddMode(0x54);
    GetInterface9()->m_flag10 = false;
    CThingAICreature* creature = GetAICreature();   // named local: the bit-field update addresses [eax+1c8h] directly
    creature->ImmuneToAttack = true;
}