// CGameScriptInterface::UpdateOnlineScore_ChickenKick(float score) const -- report the score for online-scoreboard activity 0x11 through the main player's creature's CTCHeroOnlineScoreboard
// Leaf: ?UpdateOnlineScore_ChickenKick@CGameScriptInterface@@UBEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008a10c0, 113 bytes, thiscall, `ret 4`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 726; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?UpdateOnlineScore_ChickenKick@CGameScriptInterface@@UBEXM@Z; activity 0x11; CTCHeroOnlineScoreboard::GetInterfaceType() is 269 (0x10D, landed).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970 (on PlayerManager +0x14; the non-const overload, it calls the non-const
//   GetPlayer 0x004498C0 -- 0x00449960 is the const one), CPlayer::GetPControlledCreature() const 0x00487DC0 (ControlledCreature +0x2C), CVectorMap<ETCInterfaceType,
//   CTCBase*, ...>::LowerBound 0x0040F020 (inlined GetInterface), CTCHeroOnlineScoreboard::UpdateScoreForActivity(
//   EOnlineScoreboardActivity, float, bool) 0x0055F3B0.
// Body: GetInterface's result is ignored -- on a miss the call goes through the uninitialised scoreboard local (retail's miss arm
//   passes the argument slot that local shares), exactly as written.
// Header debt: engine/CGameScriptInterface.h, CPlayer, CThing and engine/CTCHeroOnlineScoreboard.h are flat layouts; modelled locally.
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
    value_type* First;                                     // +0x00 (CArray: STLport vector)
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                  // 0x0040F020 for the interface map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20 (the bitset of present interface types)
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;  // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                              // 0x00449970
    const CPlayer& GetMainPlayer() const;                  // 0x00449960
};
class CTCHeroOnlineScoreboard {
public:
    enum EOnlineScoreboardActivity { EOnlineScoreboardActivity_FIRST };
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool flag);   // 0x0055F3B0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::UpdateOnlineScore_ChickenKick(float score) const
{
    CTCHeroOnlineScoreboard* scoreboard;
    PlayerManager->GetMainPlayer().GetPControlledCreature()->GetInterface(&scoreboard, (ETCInterfaceType)0x10D);
    scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0x11, score, false);
}