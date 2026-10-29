// CTCBattleCharge::FinalCleanup() -- when the parent is alive: release its special-abilities lock if ability 12 (battle charge) is running, and unless the exit modes already ran resume an AI creature parent, drop creature mode 0x23 and end a player parent's special-move mode; finally kill the parent when it carries a CTCDestroyThingOnSpecialAbilityFinish (interface 0x109)
// Leaf: ?FinalCleanup@CTCBattleCharge@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A75F0, 243 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?FinalCleanup@CTCBattleCharge@@AAEXXZ (private void()); its only references are the two
//   CTCBattleCharge vtable thunks (vtable 0x0126CBFC) 0x007A7BD0 (slot 14, OnKill) and 0x007A7BE0 (slot 2, OnRemove) that
//   tail-jump here -- the ability-TC pattern of the landed CTCWillLightning::FinalCleanup 0x007A0950 and
//   CTCAssassinRush::FinalCleanup 0x007AA4D0. Label correction: the landed 0x007A7BE0 is catalogued as
//   CTCBattleCharge::FinalCleanup (it is OnRemove) and 0x007A7BD0 as vfunc_14 (OnKill); both are free-function forwarders.
// Callees: CVectorMap LowerBound 0x0040F020 (inline GetInterface: 0xA7 CTCSpecialAbilities, 0x31 CTCCreatureModeManager),
//   CThingAICreature::SuspendAI(bool, void*) 0x00833A10 (Ego_r.pdb), CTCCreatureModeManager::RemoveMode(unsigned long)
//   0x0073ABA0, CThing::GetPlayer() 0x004C7A30 (Ego_r.pdb, CPlayer&), CPlayer::EndSpecialMoveMode() 0x00487150 (bound
//   name; it removes player mode 0x12), CThing::Kill(bool) 0x004C9B80.
// Layout: CTCBase ParentThing +4; CTCBattleCharge CompletedExitModes +0x90 (Ego_r +0x94 less the narrower PeopleHit
//   vector); CThing State byte +0x10 (1 AI creature, 2 player creature), UsingComponents bitset +0x20 (bit 0x109 in the
//   word at +0x40), interface map +0x44, dead bit +0x91; CTCSpecialAbilities AbilityMutexLock +0x0C, CurrentAbility +0x10.
// Interface 0x109 = CTCDestroyThingOnSpecialAbilityFinish (its GetInterfaceType 0x004D63D2 returns 0x109).
// Shape: the parent is a named local (esi, never reloaded); the AI-creature resume sits inside the special-abilities
//   test (retail jumps from a missing 0xA7 interface straight to the second CompletedExitModes test); the closing
//   interface-0x109 test runs on the local outside the alive test.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer {
public:
    void EndSpecialMoveMode();                          // 0x00487150
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char State;                                // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
    bool IsDead() const { return (m_stateFlags & 1) != 0; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    void Kill(bool immediately);                        // 0x004C9B80
    CPlayer& GetPlayer();                               // 0x004C7A30
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
class CThingAICreature : public CThing {
public:
    void SuspendAI(bool suspend, void* key);            // 0x00833A10
};
class CTCSpecialAbilities { public:                     // interface 0xA7
    unsigned char _pad00[0xc];
    bool          AbilityMutexLock;                     // +0x0c
    long          CurrentAbility;                       // +0x10 (12 = battle charge)
};
class CTCCreatureModeManager { public:                  // interface 0x31
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};

class CTCBattleCharge {
public:
    virtual ~CTCBattleCharge();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x90 - 0x08];
    bool          CompletedExitModes;                   // +0x90
private:
    void FinalCleanup();
};

void CTCBattleCharge::FinalCleanup()
{
    CThing* parent = ParentThing;
    if (parent && !parent->IsDead()) {
        CTCSpecialAbilities* abilities;
        if (parent->GetInterface(&abilities, 0xa7)) {
            if (abilities->CurrentAbility == 12) {
                abilities->AbilityMutexLock = false;
                abilities->CurrentAbility = 0;
            }
            if (!CompletedExitModes && parent->State == 1)
                ((CThingAICreature*)parent)->SuspendAI(false, 0);
        }
        if (!CompletedExitModes) {
            CTCCreatureModeManager* modes;
            if (parent->GetInterface(&modes, 0x31))
                modes->RemoveMode(0x23);
            if (!parent->IsDead() && parent->State == 2)
                parent->GetPlayer().EndSpecialMoveMode();
        }
    }
    if (parent->HasTC(0x109))
        parent->Kill(false);
}