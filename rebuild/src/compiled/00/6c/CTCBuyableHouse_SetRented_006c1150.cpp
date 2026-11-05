// CTCBuyableHouse::SetRented(bool rented) -- mark the house as rented out (or not): keep the living hero's rented-building count in step, evict the tenant when an owned house stops being rented, rebuild the main sign post and refresh the player GUI's mini-map markers
// Leaf: ?SetRented@CTCBuyableHouse@@QAEX_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C1150, 243 bytes, `ret` through the tail jump to CPlayerGui::UpdateMiniMapMarkers
//   with the argument slot reused (`mov [esp+4],0`).
// Identity: Ego_r.pdb has `?SetRented@CTCBuyableHouse@@QAEX_N@Z`; the catalog label is right. The sibling
//   CTCBuyableHouse::SetOwnedByPlayer 0x006C17B0 (landed) ends with the same sign-post and GUI tail.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (Ego_r `QAEAAVCPlayer@@XZ`), CPlayer::GetPControlledCreature 0x00487DC0
//   (settled), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (settled, inside the inline
//   CThing::GetTC<CTCHeroStats>), CTCBuyableHouse::Evict 0x006C0FD0, NeedsMainSignPost 0x006BFB90 and CreateMainSignPost
//   0x006C0F00 (all private in Ego_r: `AAEXXZ` / `AAE_NXZ` / `AAEXXZ`), CBaseIntelligentPointer::GetPItem 0x00A01B50 (inside
//   CIntelligentPointer<CThing>::Get), CThing::Kill 0x004C9B80 (`?Kill@CThing@@QAEX_N@Z`), CPlayerGui::UpdateMiniMapMarkers
//   0x004378C0 (Ego_r `?UpdateMiniMapMarkers@CPlayerGui@@QAEX_N@Z`: the body forwards to the mini-map drawer +0x160, whose
//   0x00643E9A forwards to its CMiniMapDisplay +0x28 at 0x0082B7C0 -- CDrawMiniMap::UpdateMarkers(bool) and
//   CMiniMapDisplay::UpdateMarkers(bool) in the PDB; the landed spelling `Refresh(long)` is not a PDB name).
// Globals: CMainGameComponent::Get() 0x013B86A0 and (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790 (bound spellings; no PDB data symbols).
// Layout: engine/CTCBuyableHouse.h (OwnedByPlayer +0x11, Rented +0x12, MainSignPost +0x24), engine/CTCHeroStats.h
//   (NumberOfBuildingsRented +0xE8), engine/CPlayer.h, engine/CPlayerManager.h and engine/CPlayerGui.h through the rename trick (callee classes,
//   methods only); CMainGameComponent::PPlayerManager +0x1C (Ego_r, scoped_ptr); CThing: UsingComponents +0x20, Components
//   +0x44, Dead +0x91 bit 0 (Ego_r minus 0x10 past Components). CTCHeroStats is interface type 4 (bit 4 of the bitset byte +0x20).
// Shape: the hero stats pointer starts null (`xor ecx,ecx`) and GetTC leaves it so when the bitset bit is clear; the key of
//   the inlined LowerBound is homed in the dead `rented` argument slot (bl keeps the argument). GetTC must take the map's
//   address into a local (`lea esi,[eax+0x44]`, end read as [esi+4]) as agent561's 0x004A2F10 model; reading
//   Components.Data.end() straight off the hero pointer keeps the hero in esi instead (DIFFER 246v243).
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

#define CTCHeroStats CTCHeroStatsLayout
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCHeroStats : public CTCHeroStatsLayout { public:
};

class CThingPlayerCreature : public CThing { };

#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManagerLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager
class CPlayer : public CPlayerLayout { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager : public CPlayerManagerLayout { public:
    CPlayer& GetMainPlayer();                               // 0x00449970
};
template <class T> class scoped_ptr { public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:                          // Ego_r.pdb offsets up to +0x28 hold in retail
    unsigned char              _pad00[0x1C];
    scoped_ptr<CPlayerManager> PPlayerManager;              // +0x1C
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
#define CPlayerGui CPlayerGuiLayout
#include "engine/CPlayerGui.h"
#undef CPlayerGui
class CPlayerGui : public CPlayerGuiLayout { public:
    void UpdateMiniMapMarkers(bool force);                  // 0x004378C0
};

class CBaseObject;
class CBaseIntelligentPointer { public:                     // vptr + PData, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};

#define CTCBuyableHouse CTCBuyableHouseLayout
#include "engine/CTCBuyableHouse.h"
#undef CTCBuyableHouse
class CTCBuyableHouse : public CTCBuyableHouseLayout { public:
    void SetRented(bool rented);
private:
    bool NeedsMainSignPost();                               // 0x006BFB90
    void CreateMainSignPost();                              // 0x006C0F00
    void Evict();                                           // 0x006C0FD0
};

void CTCBuyableHouse::SetRented(bool rented)
{
    CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead()) {
        CTCHeroStats* stats = 0;
        hero->GetTC(TCI_HERO_STATS, &stats);
        if (rented == true) {
            if (!Rented)
                stats->NumberOfBuildingsRented++;
        } else if (rented == false) {
            if (Rented == true)
                stats->NumberOfBuildingsRented--;
        }
    }
    Rented = rented;
    if (!rented && OwnedByPlayer)
        Evict();
    if (reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost).Get()
        && !reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost).Get()->IsDead())
        reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost).Get()->Kill(true);
    if (NeedsMainSignPost())
        CreateMainSignPost();
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
}