// CAIStateGroup_BanditPlayDice::GetDiceGame() -- the dice-game component (interface 0xf0) of the village this creature's villager component belongs to, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929220, length 135 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined GetInterface lookups:
// the creature's villager component, interface 0x23 = bitset word 1 bit 3 -> `test byte ptr [eax+24h],8`, and the
// village thing's dice game, interface 0xf0 = word 7 -> `test dword ptr [eax+3Ch],10000h`),
// GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager component, twice; the manifest's IS_THING_ALIVE
// label is propagated -- see the landed 0x0093EDF0).
// Shape: the landed GetInterface out-parameter template (`__forceinline`: two uses) inside nested ifs with one shared `return 0` tail
// (`xor eax,eax`); the villager component stays in esi for the second village call; the village's owner thing
// (+4) is read from the second call's result; the final lookup returns the component straight from the entry.
// Model: local (engine/CAIStateGroup_BanditPlayDice.h flattens the base, and the base layout is needed); the base fields keep their PDB names.
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

class CTCVillage { public:
    void*   __vftable;                          // +0x00
    CThing* Owner;                              // +0x04 (CTCBase)
};
struct CTCVillager;
class CTCDiceGame;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_BanditPlayDice : CAIStateGroupBase {
    CTCDiceGame* GetDiceGame();
};

CTCDiceGame* CAIStateGroup_BanditPlayDice::GetDiceGame()
{
    CTCVillager* villager;
    if (PBrain->Creature->GetInterface(&villager, 0x23)) {
        if (GFGetPVillageInterface(villager) != 0) {
            CThing* villageThing = GFGetPVillageInterface(villager)->Owner;
            CTCDiceGame* game;
            if (villageThing->GetInterface(&game, 0xf0))
                return game;
        }
    }
    return 0;
}