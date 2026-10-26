// CCreatureAction_KrakenRiseUp::Initialise() -- base Initialise, remove creature mode 0x54 through the owner's mode manager, then start the kraken's head effects
// Leaf: ?Initialise@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923CF0, length 166 bytes (+ int3 padding), vtable slot 12 of 0x01290594
// (RTTI `?A0x29922355::CCreatureAction_KrakenRiseUp`; FableWin.pdb ?Initialise@CCreatureAction_KrakenRiseUp@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// address kept in edi), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of the
// bitset dword at +0x24; then 0xe2 = CTCKraken, bit 2 of the byte at +0x3c), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0,
// CTCKraken::StartHeadEffects @ 0x007FBC30 (landed).
// Shape: BOTH lookups are UNCHECKED out-parameter accessors (each absent arm reads the shared key word and still calls).
class CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
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
class CTCCreatureModeManager { public:                 // interface type 0x31 component
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CTCKraken { public:                              // interface type 0xe2 component
    void StartHeadEffects();                    // 0x007FBC30
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
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenRiseUp : CCombatActionBase {   // vtable 0x01290594, sizeof 0x114
    virtual void Initialise();
};

void CCreatureAction_KrakenRiseUp::Initialise()
{
    CCombatActionBase::Initialise();
    GetModeManager()->RemoveMode(0x54);
    GetKraken()->StartHeadEffects();
}