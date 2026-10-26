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
    CTCBase* AddTC(const CCharString& name, bool initialise, CTCParamsBase* params);   // 0x004C9D60
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                      // +0x00
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
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
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
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