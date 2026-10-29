// NCreatureMode::CModeBlock::AnimUpdate(CAnimateThing&) -- while the creature is idle (no action), keep its main animation on the combat component's blocking animation: unless the block animation (type 0x1a) is already playing, start it with the entry's blend timings
// Grade: RELOCATION_MATCH -- the CAnimInit is a temporary built in the SetMainAnim call (not a named local): VC then takes AnimIndex into edx and re-forms &init in eax after the two time pushes, like retail
// Leaf: AnimUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8DD0, 194 bytes (`ret 4`; vtable 0x01267C3C slot 2).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (the const instantiation: inlined const GetInterface,
// CTCCombat = type 0x30, bit 16 of the dword at +0x24); CThingCreatureBase::IsPerformingAnyAction 0x006622A0;
// CTCCombat::GetBlockingAnimationEntry 0x00733690; CAnimationEntry::GetAnimateThingFlags 0x005DC390,
// GetAnimationTransitionInTime 0x005DC1F0 (manifest class "CCombatActionBase" is propagated), GetTimeToBlendIn(CThing*) 0x005DC230;
// CAnimInit::Set 0x0070C050 (manifest "CInitRumble"; it fills the CAnimInit record); CAnimateThing::SetMainAnim(const CAnimInit&,
// long, long) 0x0070D580.
// Layout: CModeBlock = CModeBase 0x1c (ParentThing +0x04) + PCreature +0x1c. CAnimateThing: CurrentAnim's anim vector begin/end
// +0x04/+0x08, current anim type byte +0x50, its flags byte +0x51 (bit 1: finishing). CAnimInit: 0x2c-byte frame record whose
// +0x18 pair is zeroed by its member ctor before Set fills everything.
// Shape: the record's ctor is inline (member zeroing, then Set); SetMainAnim's arguments are evaluated right to left (transition
// time, then the blend-in time for the parent thing), the parent thing pointer is loaded once up front.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;  // 0x004365B0 (the const instantiation)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (const T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { public:
    bool IsPerformingAnyAction();          // 0x006622A0
};

struct CBlendPair {                        // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};

class CAnimInit { public:                         // 0x2c bytes
    long          Mode;                    // +0x00
    unsigned char AnimType;                // +0x04
    bool          Flag05;                  // +0x05
    long          AnimIndex;               // +0x08
    float         Scales[3];               // +0x0c
    CBlendPair    Blend;                   // +0x18
    unsigned long Flags;                   // +0x20
    long          Arg24;                   // +0x24
    long          Percent;                 // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

class CAnimationEntry { public:
    long AnimIndex;                        // +0x00
    unsigned long GetAnimateThingFlags();  // 0x005DC390
    long GetAnimationTransitionInTime();   // 0x005DC1F0
    long GetTimeToBlendIn(CThing* thing);  // 0x005DC230
};

class CTCCombat { public:                         // interface type 0x30
    CAnimationEntry* GetBlockingAnimationEntry() const;   // 0x00733690
};

class CAnimateThing { public:
    long          UpdateMode;              // +0x00
    void*         AnimsBegin;              // +0x04
    void*         AnimsEnd;                // +0x08
    unsigned char _pad0c[0x44];
    unsigned char CurrentAnimType;         // +0x50
    unsigned char CurrentAnimFlags;        // +0x51
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition);   // 0x0070D580
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {

struct CModeBlock : CModeBase {            // vtable 0x01267C3C
    CThingCreatureBase* PCreature;         // +0x1c
    void AnimUpdate(CAnimateThing& animate);
};

void CModeBlock::AnimUpdate(CAnimateThing& animate)
{
    const CTCCombat* combat;
    if (ParentThing->GetInterface(&combat, 0x30)) {
        if (!PCreature->IsPerformingAnyAction()) {
            if (animate.AnimsBegin != animate.AnimsEnd && !(animate.CurrentAnimFlags & 2) && animate.CurrentAnimType == 0x1a)
                return;
            CAnimationEntry* entry = combat->GetBlockingAnimationEntry();
            CThing* thing = ParentThing;
            animate.SetMainAnim(CAnimInit(0x1a, 0, entry->AnimIndex, 1, false, entry->GetAnimateThingFlags(), 100), entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
        }
    }
}

}