// NCreatureMode::CModeIntangible::FrameUpdate() -- follow the boss phase; when the phase changes, or a running timer reaches zero, remove creature mode 0x51
// Leaf: ?FrameUpdate@CModeIntangible
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7600, 177 bytes, `ret`; vtable 0x01268514 slot 3.
// Callees: CVectorMap::LowerBound (const) @ 0x004365B0 (inlined CTCBoss lookup, type 0xad = bit 13 of the +0x34 dword),
//   CTCBoss::GetBossPhase @ 0x007AED10, CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, type 0x31 =
//   the creature-mode manager), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0.
// Name: FableWin.pdb `?FrameUpdate@CModeIntangible@NCreatureMode@@UAEXXZ` (slot 3; no base call).
// Layout: Ego_r.pdb CModeIntangible (0x44, matches retail): Timer +0x30, BossPhase +0x34, WasDamageable +0x38.
// Shape: a `changed` flag in bl; the boss lookup is the one-ternary HasTC + GetTC on the const map (component straight into
//   ecx); the remove test is `(Timer > 0 && --Timer == 0) || changed`; the mode-manager lookup result is not tested, so the
//   absent path calls through the never-written out variable (VC7.1 duplicates that call tail).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);  // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the const instantiation)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->first) ? map->last : it;
        return (T*)sel->second;
    }
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

class CTCBoss { public:                           // interface type 0xad
    long GetBossPhase() const;                   // 0x007AED10
};
class CTCCreatureModeManager { public:            // interface 0x31
    void RemoveMode(unsigned long mode);   // 0x0073ABA0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {
struct CModeIntangible : CModeBase {       // vtable 0x01268514
    unsigned char _pad1c[0x14];
    long          Timer;                   // +0x30
    long          BossPhase;               // +0x34
    bool          WasDamageable;           // +0x38
    void FrameUpdate();
};
}

void NCreatureMode::CModeIntangible::FrameUpdate()
{
    bool changed = false;
    if (ParentThing->HasTC(0xad)) {
        long phase = ParentThing->GetTC<CTCBoss>(0xad)->GetBossPhase();
        if (phase != BossPhase) {
            changed = true;
            BossPhase = phase;
        }
    }
    if ((Timer > 0 && --Timer == 0) || changed) {
        CTCCreatureModeManager* modes;
        ParentThing->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x51);
    }
}