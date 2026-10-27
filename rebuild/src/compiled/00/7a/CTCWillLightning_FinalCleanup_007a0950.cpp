// CTCWillLightning::FinalCleanup() -- release the caster's special-ability lock when the lightning is still the current ability, then stop the looping sound
// Leaf: ?FinalCleanup@CTCWillLightning@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A0950, 83 bytes, plain `ret`.
// Identity: Ego_r.pdb `?FinalCleanup@CTCWillLightning@@AAEXXZ` (private void()). Both CTCWillLightning OnKill (vtable 0x0126C724
//   slot 14, 0x007A0D70) and OnRemove (slot 2, 0x007A0D80) are bare tail jumps to this body, the ability-TC pattern (the
//   donor TU lists FinalCleanup right before CTCWillLightning's OnKill / OnRemove); it ends in the landed StopLoopingSound
//   0x007A0490. The landed 0x007A0D80 source calls it `ForwardTarget_007a0950` and is itself catalogued as FinalCleanup:
//   that row is OnRemove. Catalog row was the placeholder `_gapscan::sub_007a0950`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inline GetInterface, interface 0xA7 =
//   CTCSpecialAbilities: bit 7 of the bitset dword +0x34, tested as the sign of its low byte),
//   CTCWillLightning::StopLoopingSound() @ 0x007A0490 (landed).
// Layout: as the landed CancelMove 0x007A03B0: CTCSpecialAbilities AbilityMutexLock +0x0c, CurrentAbility +0x10 (0xB = the
//   lightning ability).
// Shape: `GetInterface(&abilities, 0xa7) && CurrentAbility == 0xb` then both members cleared from one zero register.
class CTCBase;

struct CInterfaceMap {                                   // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                          // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCSpecialAbilities { public:                             // interface 0xA7
    unsigned char _pad00[0x0c];
    bool          AbilityMutexLock;                      // +0x0c
    long          CurrentAbility;                        // +0x10 EHeroAbility
};

class CTCWillLightning {
public:
    virtual ~CTCWillLightning();
    CThing* ParentThing;                                 // +0x04 (CTCBase)

    void StopLoopingSound();                             // 0x007A0490
private:
    void FinalCleanup();
};

void CTCWillLightning::FinalCleanup()
{
    CTCSpecialAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0xa7) && abilities->CurrentAbility == 0xb) {
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    StopLoopingSound();
}