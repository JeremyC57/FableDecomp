// CGameScriptInterface::GetBestScoreCoinGolfSnowSpire() const -- the hero's best CoinGolfSnowSpire score (CTCHeroStats::BestScoreCoinGolfSnowSpire truncated to long), 0 when the main player's creature is missing or dead
// Leaf: ?GetBestScoreCoinGolfSnowSpire@CGameScriptInterface@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008a1710, 94 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 734; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?GetBestScoreCoinGolfSnowSpire@CGameScriptInterface@@UBEJXZ; it reads Ego_r.pdb CTCHeroStats::BestScoreCoinGolfSnowSpire (PDB +416, retail +0x188) through interface 4 (CTCHeroStats in the landed GetInterface(&stats, 4) callers).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970 (non-const: it calls the non-const GetPlayer 0x004498C0), CPlayer::PeekPControlledCreature() const 0x00487DD0 (Ego_r.pdb, a const
//   CThingPlayerCreature*), the const CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x004365B0 (the inlined const
//   GetInterface), __ftol2 0x00BFEA70.
// Body: alive test (non-null, Dead bit 0 of +0x91 clear), presence bit 4 of the interface bitset (`test byte ptr [eax+20h],10h`).
// Header debt: engine/CGameScriptInterface.h, CPlayer, CThing and engine/CTCHeroStats.h are flat layouts (PDB offsets); modelled locally.
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                     // +0x00
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    const value_type* end() const { return Last; }
    const value_type* LowerBound(const K& key) const;      // 0x004365B0 for the interface map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20
    CInterfaceMap Interfaces;                              // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Flags91;                                 // +0x91 bit 0: dead
    template <class T> __forceinline bool GetInterface(const T** out, ETCInterfaceType type) const
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            const CInterfaceMap* map = &Interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                              // 0x00449970
    const CPlayer& GetMainPlayer() const;                  // 0x00449960
};
class CTCHeroStats {
public:
    unsigned char _pad00[0x188];
    float         BestScoreCoinGolfSnowSpire;                                   // +0x188
};
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    unsigned char   _pad04[0x10];
    CPlayerManager* PlayerManager;                         // +0x14
    CPlayerManager& GetPlayerManager() const { return *PlayerManager; }   // inline accessor (spelling unproven: emits no symbol); selects the non-const GetMainPlayer
    virtual long GetBestScoreCoinGolfSnowSpire() const;
};
long CGameScriptInterface::GetBestScoreCoinGolfSnowSpire() const
{
    const CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const CTCHeroStats* stats;
    if (hero && !(hero->Flags91 & 1) && hero->GetInterface(&stats, (ETCInterfaceType)4))
        return (long)stats->BestScoreCoinGolfSnowSpire;
    return 0;
}