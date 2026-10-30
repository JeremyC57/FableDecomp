// NPlayerGui::CDrawStatUpdates::Update() -- when the hero is alive, compare its three stat-category experience totals and its spendable experience with the previous frame's values and, once initialised, post each positive change to the player GUI's stat-update bar; then remember the new values
// Grade: RELOCATION_MATCH (retry wave 2 sec2): the retail const CVectorMap::LowerBound body (0x004365B0) defined in the TU under __declspec(noinline) -- VC7.1 then knows the GetTC key does not escape and packs it into the hero/experience word [ebp-4] as retail (was DIFFER(335v334), a separate key word); CPlayerManager / CCountedPointer from the shared engine headers. Header debt: local CPlayer / CPlayerGui / CTCBase / CTCHeroExperience views (their engine headers are flattened Ego_r layouts without these members).
// Leaf: ?Update@CDrawStatUpdates@NPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A060, 334 bytes, thiscall, ebp frame (`leave`), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Update@CDrawStatUpdates@NPlayerGui@@QAEXXZ (non-virtual void()); the body reads
//   CDrawStatUpdates::Initialised (+0x30) and PreviousStats (+0x34, vector<long>), Ego_r.pdb members 4 below the debug-STL donor,
//   and the same-TU neighbour 0x0064A1AE is the landed CDrawPerceivers::Update.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature const 0x00487DD0,
//   IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0, std::bitset<274>::test const 0x00410DE0 (out of line in this /Os TU),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (interface 0x68, CTCHeroExperience),
//   CTCHeroExperience::GetStatExperience(EHeroExperienceStatCategory) const 0x006D6510, CMultiStatBar::AddStatUpdate
//   (EStatUpdateType, long) 0x00647457 (CPlayerGui::PStatUpdates, retail +0x2F4).
// Names: 0x00647457 is Ego_r.pdb ?AddStatUpdate@CMultiStatBar@NPlayerGui@@QAEXW4EStatUpdateType@@J@Z -- the only (enum, long)
//   member of the class CPlayerGui::PStatUpdates points to, and its body searches a list of updates by type; probable.
// Label correction: catalogued as "CDrawStatUpdates::Update" without the NPlayerGui namespace.
// Layout: local view (no engine/ header for CDrawStatUpdates, CDrawBase, CPlayerGui or CTCHeroExperience): CDrawBase PlayerGui +4 /
//   PlayerManager +8 (Ego_r.pdb); CTCHeroExperience::ExperiencePointsAvailableToSpend +0x14 (Ego_r.pdb, read directly);
//   CThing UsingComponents +0x20 / Components +0x44 (retail).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
    const _Tp* end() const { return _M_finish; }
};
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                    // 0x00410DE0 (bitset<274>)
};
}
#include "engine/CPlayerManager.h"
#include "engine/CCountedPointer.h"
class CThing;
enum ETCInterfaceType { };
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    const value_type* LowerBound(const K& key) const;       // 0x004365B0
    std::vector<value_type> Data;
    Cmp  Compare;
    bool Dirty;
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> UsingComponents;                       // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char _pad_0x54[0x91 - 0x54];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    __forceinline const CTCBase* GetTC(ETCInterfaceType type) const
    {
        const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        const ETCInterfaceType key = type;
        const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        const std::pair<ETCInterfaceType, CTCBase*>* end = map->Data.end();
        return (it == end || type < it->first ? end : it)->second;
    }
};
template <class K, class V, class Cmp>
__declspec(noinline) const std::pair<K, V>* CVectorMap<K, V, Cmp>::LowerBound(const K& key) const   // 0x004365B0: the retail body, out of line
{
    long count = Data._M_finish - Data._M_start;
    const value_type* first = Data._M_start;
    while (count > 0) {
        long half = count / 2;
        const value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count = count - half - 1;
        } else
            count = half;
    }
    return first;
}
class CThingPlayerCreature : public CThing { };
// IS_THING_ALIVE 0x0043DBF0 (T = const CThingPlayerCreature*): the header template, visible here and called out of line.
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->Dead;
}
#pragma auto_inline(on)
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
enum EHeroExperienceStatCategory { };
class CTCHeroExperience : public CTCBase {
public:
    long GetStatExperience(EHeroExperienceStatCategory category) const;   // 0x006D6510
    long FishingLevel;                                      // +0x0C
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
};
enum EStatUpdateType { };
namespace NPlayerGui {
class CMultiStatBar {
public:
    void AddStatUpdate(EStatUpdateType type, long change);  // 0x00647457
};
}
class CPlayerGui {
public:
    unsigned char _pad_0x00[0x2f4];
    CCountedPointer<NPlayerGui::CMultiStatBar> PStatUpdates;   // +0x2F4
};
class CEnginePrimitiveHandle;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;
namespace NPlayerGui {
class CDrawBase {
public:
    virtual ~CDrawBase();
    CPlayerGui*         PlayerGui;                          // +0x04
    CPlayerManager*     PlayerManager;                      // +0x08
    CWorld*             World;                              // +0x0C
    CMainGameComponent* Component;                          // +0x10
    CGraphicDataBank*   Bank;                               // +0x14
    bool                Drawing;                            // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;   // +0x1C
};
class CDrawStatUpdates : public CDrawBase {
public:
    void Update();
    long LastRenownChange;                                  // +0x28
    long LastMoralityChange;                                // +0x2C
    bool Initialised;                                       // +0x30
    std::vector<long> PreviousStats;                        // +0x34
};

void CDrawStatUpdates::Update()
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        const CTCHeroExperience* experience;
        if (hero->HasTC((ETCInterfaceType)0x68))
            experience = (const CTCHeroExperience*)hero->GetTC((ETCInterfaceType)0x68);
        long value = experience->GetStatExperience((EHeroExperienceStatCategory)0);
        if (Initialised) {
            long previous = PreviousStats[0];
            if (previous != -1) {
                long change = value - previous;
                if (change > 0)
                    PlayerGui->PStatUpdates->AddStatUpdate((EStatUpdateType)0, change);
            }
        }
        PreviousStats[0] = value;
        value = experience->GetStatExperience((EHeroExperienceStatCategory)1);
        if (Initialised) {
            long previous = PreviousStats[1];
            if (previous != -1) {
                long change = value - previous;
                if (change > 0)
                    PlayerGui->PStatUpdates->AddStatUpdate((EStatUpdateType)1, change);
            }
        }
        PreviousStats[1] = value;
        value = experience->GetStatExperience((EHeroExperienceStatCategory)2);
        if (Initialised) {
            long previous = PreviousStats[2];
            if (previous != -1) {
                long change = value - previous;
                if (change > 0)
                    PlayerGui->PStatUpdates->AddStatUpdate((EStatUpdateType)2, change);
            }
        }
        PreviousStats[2] = value;
        value = experience->ExperiencePointsAvailableToSpend;
        if (Initialised) {
            long previous = PreviousStats[3];
            if (previous != -1) {
                long change = value - previous;
                if (change > 0)
                    PlayerGui->PStatUpdates->AddStatUpdate((EStatUpdateType)3, change);
            }
        }
        PreviousStats[3] = value;
        Initialised = true;
    }
}
}   // namespace NPlayerGui