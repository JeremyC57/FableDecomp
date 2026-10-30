// CGameScriptInterface::GiveHeroMorality(float fraction) const -- add fraction of the maximum morality to the hero's morality
// Grade: DIFFER(154v150) -- frame only: retail `push ecx` (the map key temp, the fild temp and the ROUND result share [esp+4]); VC7.1 here `sub esp,8`. Uses the retail_asm ROUND (fld/fistp) helper, so it would land as retail_asm if it matched. ~4 grades.
// Leaf: ?GiveHeroMorality@CGameScriptInterface@@UBEXM@Z
// Retail: Fable.exe 0x00899EF0, 150 bytes, `ret 4`; CGameScriptInterface vtable 0x01260F0C slot 156 (engine header).
// Names (Ego_r.pdb / FableWin.pdb): ?GiveHeroMorality@CGameScriptInterface@@UBEXM@Z; ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ
//   0x00449970 (called twice); ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ 0x00487DC0; the thing's component
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound 0x0040F020 (interface 4 = CTCHeroStats, the
//   EXTRA_RULES interface table); ?GetMoralityMax@CTCHeroStats@@QBEJXZ 0x0057A67E; ?SetMorality@CTCHeroStats@@QAEXJ@Z 0x0057C2E6.
// Body: if the main player's creature exists and is not Dead (retail CThing +0x91 bit 0, Ego_r +0xA1), look up its hero stats
//   (an unchecked lookup: the component word stays uninitialised when the interface bit is clear) and SetMorality(Morality +
//   ROUND(GetMoralityMax() * fraction)).
// Retail-asm: 0x00899F66..0x00899F6E `fstp dword ptr [esp+0Ch] ; fld dword ptr [esp+0Ch] ; fistp dword ptr [esp+4]` -- the engine's
//   inline-assembly ROUND helper (`fld value ; fistp result`, __forceinline, the product homed in the dead `fraction` slot and the
//   result in the `push ecx` slot). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives
//   `fistp qword` without the reload: debakeA 148v150; GOTCHAS rasm1 evidence); same helper as the landed retail_asm rows
//   0x006C3BD0 / 0x00AF9000 / 0x00B102C0.
// Layout: retail CThing (no usable header: engine/_quarantine/CThing.h flattens the +0x91 flag byte) -- UsingComponents bitset
//   +0x20, Components CVectorMap +0x44 (STLport vector 0xC + compare), the Dead bit at +0x91.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#define CPlayerManager CPlayerManagerPdb
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#define CTCHeroStats CTCHeroStatsPdb
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats

__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

class CTCBase;
enum ETCInterfaceType { TCI_HERO_STATS = 4 };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >
{
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
    Entry* LowerBound(const ETCInterfaceType& key);                       // 0x0040F020
    Entry* Start;
    Entry* Finish;
    Entry* End;
    unsigned long Compare;
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing
{
public:
    unsigned char  _pad00[0x20];
    unsigned long  UsingComponents[9];                                   // +0x20 bitset<274>
    CInterfaceMap  Components;                                           // +0x44
    unsigned char  _pad54[0x91 - 0x54];
    unsigned char  Dead : 1;                                             // +0x91 bit 0
};
class CThingPlayerCreature : public CThing { };

class CTCHeroStats : public CTCHeroStatsPdb
{
public:
    long GetMoralityMax() const;                                         // 0x0057A67E
    void SetMorality(long morality);                                     // 0x0057C2E6
};
class CPlayer : public CPlayerPdb
{
public:
    CThingPlayerCreature* GetPControlledCreature() const;                // 0x00487DC0
};
class CPlayerManager : public CPlayerManagerPdb
{
public:
    CPlayer& GetMainPlayer();                                            // 0x00449970
};

template <ETCInterfaceType Type, class T> __forceinline T* GetTC(CThing* thing)
{
    T* tc;
    if (thing->UsingComponents[Type >> 5] & (1u << (Type & 31))) {
        CInterfaceMap* map = &thing->Components;
        CInterfaceMap::Entry* it = map->LowerBound(Type);
        CInterfaceMap::Entry* end = map->Finish;
        tc = (T*)((it == end || it->first > Type) ? end : it)->second;
    }
    return tc;
}

void CGameScriptInterface::GiveHeroMorality(float fraction) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead) {
        CTCHeroStats* stats = GetTC<TCI_HERO_STATS, CTCHeroStats>(PlayerManager->GetMainPlayer().GetPControlledCreature());
        fraction = stats->GetMoralityMax() * fraction;
        stats->SetMorality(stats->Morality + ROUND(fraction));
    }
}
