// CTCBerserk::Pause() -- pause a pausable, running berserk: mark it paused and take the berserk mode (0x24) off the creature's mode manager
// Leaf: ?Pause@CTCBerserk@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078C780, 98 bytes, plain `ret`.
// Identity: Ego_r.pdb `?Pause@CTCBerserk@@QAEXXZ`. The Ego_r.pdb donor lays the CTCBerserk TU out as CreateStartingEffect,
//   InitAbility, CancelMove, Pause, UnPause, OnKill, OnRemove; retail has the landed CreateStartingEffect 0x0078C4C0 and
//   CancelMove 0x0078C610, then 0x0078C780 / 0x0078C7F0, then the landed OnKill 0x0078C860 / OnRemove 0x0078C8B0. This body
//   sets Paused and removes a mode, its twin 0x0078C7F0 clears Paused and adds it back. Callers 0x0072C094 / 0x0072C161 (with
//   UnPause at 0x0072C09D / 0x0072C221). Catalog row was the placeholder `_gapscan::sub_0078c780`.
// Layout: Ego_r.pdb CTCBerserk with the retail 12-byte DependentThings vector (+0x4c): Paused +0x70, Pausable +0x71 (PDB
//   +0x74/+0x75), as the landed CancelMove 0x0078C610 / CreateStartingEffect 0x0078C4C0 local model; CThing component
//   bitset +0x20 (interface 0x31 = bit 17 of +0x24) and the {type, component} vector map at +0x44.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCCreatureModeManager::RemoveMode(unsigned long)
//   @ 0x0073ABA0 (Ego_r.pdb `?RemoveMode@CTCCreatureModeManager@@QAEXK@Z`).
// Shape: two byte-flag guards to one shared exit; the mode manager comes from an inline accessor returning an unset local
//   when the 0x31 bit is clear (retail calls RemoveMode on the dead `push ecx` frame word, which also held the lookup key),
//   so the call is duplicated on both paths.
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> at CThing +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 component-presence bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CTCCreatureModeManager { public:                  // interface 0x31
    bool IsModeActive(unsigned long type) const;        // 0x0073A8A0
    bool AddMode(unsigned long type);                   // 0x0073AE10
    void RemoveMode(unsigned long type);                // 0x0073ABA0
};
// The creature's mode manager as the berserk code fetches it: an unset local when the thing has no 0x31 component
// (retail then reads the dead frame word; never reached in practice).
__forceinline CTCCreatureModeManager* GetModeManager(CThing* thing)
{
    CTCCreatureModeManager* modes;
    if (thing->HasTC(0x31))
        modes = thing->GetTC<CTCCreatureModeManager>(0x31);
    return modes;
}

class CTCBerserk {
public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    long          Timer;                                // +0x0c
    unsigned char _pad10[0x70 - 0x10];                  // timers, multipliers, rumble, sound, level, flags,
                                                        // DependentThings +0x4c, effects +0x58/+0x60, PDef +0x68,
                                                        // RadialBlurFadeKey +0x6c
    bool          Paused;                               // +0x70
    bool          Pausable;                             // +0x71
    bool          IsInCutscene;                         // +0x72
    void Pause();
};

void CTCBerserk::Pause()
{
    if (Pausable && !Paused) {
        Paused = true;
        GetModeManager(ParentThing)->RemoveMode(0x24);
    }
}