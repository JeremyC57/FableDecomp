// CGameScriptInterface::GetHeroTitle() const -- script call: the main player's hero's title (CTCHero::GetTitle), 0 without a live hero holding the hero component
// Leaf: ?GetHeroTitle@CGameScriptInterface@@UBE?AW4EHeroTitle@@XZ
// Retail: Fable.exe @ 0x00897880, 88 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 205 holds this body; slot 205 is
//   ?GetHeroTitle@CGameScriptInterface@@UBE?AW4EHeroTitle@@XZ in ghidra_out/cgsi_slots_aligned.tsv.
//   Getter: FableWin.pdb ?GetTitle@CTCHero@@QBE?AW4EHeroTitle@@XZ, inline (+0x10). Interface 0x29 = CTCHero (landed
//   CTCHero::GetInterfaceType 0x006CA6F0 returns 0x29).
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0); live hero holding
//   interface 0x29 (UsingComponents word 1 bit 9, `test ch,2`): GetTC (non-const LowerBound 0x0040F020), return +0x10.
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
    template <class T> __forceinline bool GetTC(T** out)
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
    template <class T> __forceinline bool PeekTC(const T** out) const
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
    CThingPlayerCreature* GetPControlledCreature() const;    // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
enum EHeroTitle { EHERO_TITLE_NONE_PLACEHOLDER = 0 };
class CTCHero {                                         // interface 0x29
public:
    enum { INTERFACE_TYPE = 0x29 };
    unsigned char _pad00[0x10];
    EHeroTitle Title;                                    // +0x10
    EHeroTitle GetTitle() const { return Title; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
EHeroTitle CGameScriptInterface::GetHeroTitle() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCHero* tc;
        if (hero->GetTC(&tc))
            return tc->GetTitle();
    }
    return (EHeroTitle)0;
}