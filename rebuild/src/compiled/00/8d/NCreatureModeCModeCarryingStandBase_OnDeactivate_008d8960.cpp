// NCreatureMode::CModeCarryingStandBase::OnDeactivate() -- drop the carrying combination animations (type 0x18) from the creature's animation, then the stand-mode deactivation
// Leaf: ?OnDeactivate@CModeCarryingStandBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8960, 90 bytes (+ int3 padding), `ret`; vtable 0x01267400 slot 6.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x5a = bit 26 of the dword at +0x28 =
//   CTCAnimationComplex), CTCAnimationComplex::GetAnimation @ 0x0070B460 (manifest "CWorld::DrawGetEnvironment" is
//   propagated), CAnimateThing::RemoveCombinationAnimsOfType(long) @ 0x0070D8C0 (manifest "CSpellContainerList::AddChild"
//   is propagated), CModeStandBase::OnDeactivate @ 0x00861D00 (qualified).
// Name: FableWin.pdb `?OnDeactivate@CModeCarryingStandBase@NCreatureMode@@UAEXXZ` (slot 6).
// Shape: the landed sibling CModeCarryingStandBase::FrameUpdate @ 0x008D88A0: the GetInterface result is not tested, so the
//   absent path joins with the never-written out variable, which shares the lookup key's frame word (`mov eax,[esp+4]`).
//   The animation is a named reference: chained `GetAnimation().Remove(0x18)` schedules the `push 18h` before the getter call.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

struct CAnimateThing {
    void RemoveCombinationAnimsOfType(long type);   // 0x0070D8C0
};
struct CTCAnimationComplex {               // interface type 0x5a
    CAnimateThing& GetAnimation();         // 0x0070B460
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};
struct CModeStandBase : CModeBase {        // retail 0x68
    unsigned char _stand[0x4c];
    void OnDeactivate();                   // 0x00861D00
};

namespace NCreatureMode {
struct CModeCarryingStandBase : CModeStandBase {   // vtable 0x01267400
    void OnDeactivate();
};
}

void NCreatureMode::CModeCarryingStandBase::OnDeactivate()
{
    CTCAnimationComplex* animation;
    ParentThing->GetInterface(&animation, 0x5a);
    CAnimateThing& animate = animation->GetAnimation();
    animate.RemoveCombinationAnimsOfType(0x18);
    CModeStandBase::OnDeactivate();
}