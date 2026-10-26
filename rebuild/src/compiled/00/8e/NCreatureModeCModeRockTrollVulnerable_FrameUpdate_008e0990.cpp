// NCreatureMode::CModeRockTrollVulnerable::FrameUpdate() -- count the recovery frames down; once they are spent and the troll's animation list is empty or its current animation may be replaced, recover; then the base frame update
// Leaf: ?FrameUpdate@CModeRockTrollVulnerable
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E0990, 118 bytes (+ int3 padding), `ret`; vtable 0x01285774 slot 3.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookup, type 0x5a = bit 26 of the +0x28 dword = CTCAnimationComplex),
//   CTCAnimationComplex::GetAnimation @ 0x0070B460, CModeRockTrollVulnerable::Recover @ 0x008DE700, CModeBase::FrameUpdate
//   @ 0x0073AC50 (qualified).
// Name: FableWin.pdb `?FrameUpdate@CModeRockTrollVulnerable@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeRockTrollVulnerable (0x34, matches retail): RecoveryCount +0x1c. CAnimateThing (the landed
//   CModeConversation::AnimUpdate model): anims begin/end +0x04/+0x08, flags +0x51.
// Shape: the component comes from an inline accessor returning an unset local (`T* tc; if (HasTC) tc = GetTC; return tc;`):
//   the hit path loads it straight into ecx and the absent path reads the never-written local from the key's frame word;
//   RecoveryCount is decremented only when non-zero and re-read for the `<= 0` test.
class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* tc;
        if (HasTC(interfaceType))
            tc = GetTC<T>(interfaceType);
        return tc;
    }
};

class CAnimateThing { public:
    long          UpdateMode;              // +0x00
    void*         AnimsBegin;              // +0x04
    void*         AnimsEnd;                // +0x08
    unsigned char _pad0c[0x44];
    unsigned char CurrentAnimType;         // +0x50
    unsigned char CurrentAnimFlags;        // +0x51
};
class CTCAnimationComplex { public:               // interface type 0x5a
    CAnimateThing& GetAnimation();         // 0x0070B460
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};

namespace NCreatureMode {
struct CModeRockTrollVulnerable : CModeBase {   // vtable 0x01285774
    long          RecoveryCount;           // +0x1c
    unsigned char Filter[0x0c];            // +0x20 CAnimFilterOfGroup
    void*         PCreature;               // +0x2c
    bool          bRecovering;             // +0x30
    void Recover();                        // 0x008DE700
    void FrameUpdate();
};
}

void NCreatureMode::CModeRockTrollVulnerable::FrameUpdate()
{
    CAnimateThing& animate = ParentThing->GetTCIfPresent<CTCAnimationComplex>(0x5a)->GetAnimation();
    if (RecoveryCount)
        --RecoveryCount;
    if (RecoveryCount <= 0 && (animate.AnimsBegin == animate.AnimsEnd || (animate.CurrentAnimFlags & 2)))
        Recover();
    CModeBase::FrameUpdate();
}