// CTCCombat::UnsetBlocking() -- take the owning creature out of creature mode 12 and clear Blocking / StartingToBlock.
// Leaf: ?UnsetBlocking@CTCCombat@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007338A0, length 102 bytes (+ int3 padding).
// Identity (FINDINGS chunk 391 callee; was the propagated CAIStateGroup_MinionReactToNoise::Cleanup label of the
//   byte-similar 0x0091CFA0, modelled as a local creature-mode class): Ego_r.pdb `?UnsetBlocking@CTCCombat@@QAEXXZ`
//   (beside SetBlocking); +4 is the TC's ParentThing (the interface bitset is tested on it directly) and the two
//   cleared bools are CTCCombat::Blocking (+0x63) and StartingToBlock (+0x62) -- struct_layouts_egor puts them at
//   +0x6F / +0x6E, 12 bytes later because AbilityList, UsableAbilityList and AlwaysBlockThings are 16-byte Dinkumware
//   vectors there and 12-byte STLport vectors in retail (the layout of the landed CTCCombat::ClearCombatVariables
//   0x007334F0). Callers: CTCCombat::UpdateCombatTimers 0x007339E0 and SetBlockBroken 0x00733F70 (chunk 391),
//   CCreatureAction_BlockCounter::Initialise 0x00855920, CThingGameObject::OnHit 0x00839940,
//   CGameScriptInterface::SetCutsceneMode 0x0089D600 and seven more.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x31 = CTCCreatureModeManager, bit
//   17 of the bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0 with 12.
// Shape: the mode manager is an uninitialised local filled by the inlined lookup (the miss path reads the slot
//   and duplicates the tail); the two bools are cleared from one `xor al,al`, +0x63 then +0x62.
// Model: the retail CTCCombat layout is local (engine/CTCCombat.h still has the PDB's 16-byte vectors: header debt).
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
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

class CTCCreatureModeManager { public:                 // interface type 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

class CCombatManager;
class CThingCreatureBase;
class CDefPointeeBase;

class CTCCombat {                               // retail layout (STLport containers), 0x98 bytes
public:
    virtual void v00();
    CThing*             ParentThing;                    // +0x04 (CTCBase)
    bool                ToBeRemoved;                    // +0x08
    bool                IsInGlobalUpdateTCs;            // +0x09
    long                ComboChainPos;                  // +0x0c
    long                ComboChainID;                   // +0x10
    long                ComboChainGracePeriodCountdown; // +0x14
    long                HandednessGracePeriodCountdown; // +0x18
    long                UnarmedCombatIdleCountdown;     // +0x1c
    CCombatManager*     CombatManager;                  // +0x20
    CThingCreatureBase* ParentCreature;                 // +0x24
    CDefPointeeBase*    PCombatTypeDef;                 // +0x28
    void*               AbilityList[3];                 // +0x2c STLport vector
    void*               UsableAbilityList[3];           // +0x38 STLport vector
    long                CurrentAbilityIndex;            // +0x44
    CDefPointeeBase*    PBlockCounterAbility;           // +0x48
    CDefPointeeBase*    PFlourish360Ability;            // +0x4c
    long                BlockCounterTimer;              // +0x50
    void*               AlwaysBlockThings[3];           // +0x54 STLport vector
    bool                PerformingAction;               // +0x60
    bool                PerformingStrikeAction;         // +0x61
    bool                StartingToBlock;                // +0x62
    bool                Blocking;                       // +0x63
    long                BlockHoldTimer;                 // +0x64
    long                BlockingNumFrames;              // +0x68
    long                UnsheathingWeaponType;          // +0x6c
    void*               CombatAbilities[3];             // +0x70 STLport multimap
    long                ShotOverrideType;               // +0x7c
    bool                ReadyForMelee;                  // +0x80
    long                CurFlourishTimer;               // +0x84
    void*               FlourishParticles[3];           // +0x88 STLport vector
    bool                IsInMiddleOfFlourishAction;     // +0x94
    void UnsetBlocking();
};

void CTCCombat::UnsetBlocking()
{
    CTCCreatureModeManager* pModes;
    ParentThing->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(12);
    Blocking = false;
    StartingToBlock = false;
}
