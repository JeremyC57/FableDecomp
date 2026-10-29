// CGameScriptInterface::GetHeroWillEnergy() const -- the hero's current will energy; 0 when there is no live hero
// Leaf: GetHeroWillEnergy@CGameScriptInterface
// Retail: Fable.exe @ 0x0089A150, 86 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
// then CThing::PeekTC<CTCHeroStats> (Ego_r.pdb bool/out-pointer form; const Components lower_bound 0x004365B0).
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCHeroStats retail = Ego_r.pdb minus 8 through +0x118 (12-byte STLport map and vector), minus 0x18 after the four vectors
// (Age +0x30, Stamina +0x58, RenownLevel +0x70, Attractiveness +0x9c, BestTimePairs +0x178, Marriages +0x1d8); engine/CTCHeroStats.h
// carries the PDB offsets.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const ETCInterfaceType& type);          // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (const T*)it->second;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;      // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                                       // 0x00449970
};
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
struct CMarriageNode { CMarriageNode* _M_next; CMarriageNode* _M_prev; };
class CMarriageList {                                    // std::list<CHeroMarriageInfo> (STLport: one node pointer)
public:
    CMarriageNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
enum EMarriageState { MARRIAGE_STATE_1 = 1, MARRIAGE_STATE_2 = 2, MARRIAGE_STATE_HAS_CHILDREN = 4, MARRIAGE_STATE_DIVORCED = 5,
                      MARRIAGE_STATE_WIFE_MURDERED = 7, MARRIAGE_STATE_FORCE_DWORD = 0x7fffffff };
class CTCHeroStats {
public:
    enum { INTERFACE_TYPE = 0x4 };
    unsigned char _pad_0x000[0x30];
    float Age;                                  // +0x030
    float SunTan;                               // +0x034
    float Fatness;                              // +0x038
    unsigned char _pad_0x03c[0x1c];
    long Stamina;                               // +0x058
    long MaxStamina;                            // +0x05c
    unsigned char _pad_0x060[0x10];
    long RenownLevel;                           // +0x070
    long RenownMaxLevel;                        // +0x074
    unsigned char _pad_0x078[0x24];
    float Attractiveness;                       // +0x09c
    float Scariness;                            // +0x0a0
    unsigned char _pad_0x0a4[0xcc];
    CCharString DeathRecoveryMarkerName;        // +0x170
    unsigned char _pad_0x174[0x4];
    float BestTimePairs;                        // +0x178
    float BestTimeSorting;                      // +0x17c
    unsigned char _pad_0x180[0x10];
    float BestTimeGuessTheAddition;             // +0x190
    unsigned char _pad_0x194[0x44];
    CMarriageList Marriages;                    // +0x1d8
    float GetAge() const { return Age; }
    float GetFatness() const { return Fatness; }
    long  GetStamina() const { return Stamina; }
    long  GetMaxStamina() const { return MaxStamina; }
    float GetStaminaPercentage() const { return (float)Stamina / MaxStamina; }
    long  GetRenownLevel() const { return RenownLevel; }
    float GetAttractiveness() const { return Attractiveness; }
    float GetScariness() const { return Scariness; }
    float GetBestTimePairs() const { return BestTimePairs; }
    float GetBestTimeSorting() const { return BestTimeSorting; }
    float GetBestTimeGuessTheAddition() const { return BestTimeGuessTheAddition; }
    const CCharString& GetDeathRecoveryMarkerName() const { return DeathRecoveryMarkerName; }
    bool  HasMarriageInState(EMarriageState state) const;          // 0x0057AC29
    bool  HasCurrentMarriage() const;                              // 0x0057AC4B
    float GetPercentageThroughRenownLevel() const;                 // 0x0057AAD9
};
class CGameScriptInterface {
public:
    virtual long GetHeroWillEnergy() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

long CGameScriptInterface::GetHeroWillEnergy() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    const CTCHeroStats* stats;
    if (hero && !hero->IsDead() && hero->PeekTC(&stats))
        return stats->GetStamina();
    return 0;
}