#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
#include <string.h>
// Behaviour test for NCreatureMode::CModeBlock::AnimUpdate (0x008D8DD0).
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingCreatureBase : CThing {
    bool busy;
    bool IsPerformingAnyAction() { return busy; }
};

struct CBlendPair {                        // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};

struct CAnimInit {
    CAnimInit() {}
    long          Mode;                    // +0x00
    unsigned char AnimType;                // +0x04
    bool          Flag05;                  // +0x05
    long          AnimIndex;               // +0x08
    float         Scales[3];               // +0x0c
    CBlendPair    Blend;                   // +0x18
    unsigned long Flags;                   // +0x20
    long          Arg24;                   // +0x24
    long          Percent;                 // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    { AnimType = (unsigned char)animType; Arg24 = arg24; AnimIndex = animIndex; Mode = mode; Flag05 = flag05; Flags = flags; Percent = percent; }
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

struct CAnimationEntry {
    long AnimIndex;                        // +0x00
    CThing* lastThing;
    unsigned long GetAnimateThingFlags() { return 0x55; }
    long GetAnimationTransitionInTime() { return 7; }
    long GetTimeToBlendIn(CThing* thing) { lastThing = thing; return 9; }
};

struct CTCCombat {                         // interface type 0x30
    CAnimationEntry* entry;
    CAnimationEntry* GetBlockingAnimationEntry() const { return entry; }
};

struct CAnimateThing {
    long          UpdateMode;              // +0x00
    void*         AnimsBegin;              // +0x04
    void*         AnimsEnd;                // +0x08
    unsigned char _pad0c[0x44];
    unsigned char CurrentAnimType;         // +0x50
    unsigned char CurrentAnimFlags;        // +0x51
    int calls; CAnimInit last; long b, t;
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition) { ++calls; last = init; b = blendIn; t = transition; }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00() {}
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

using namespace NCreatureMode;
int main()
{
    int ok = 1;
    CThingCreatureBase creature; memset(&creature, 0, sizeof(creature)); creature.busy = false;
    CAnimationEntry entry; entry.AnimIndex = 33; entry.lastThing = 0;
    CTCCombat combat; combat.entry = &entry;
    CInterfaceMap::SEntry e[1] = { { 0x30, (CTCBase*)&combat } }; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 1;
    CModeBlock mode; mode.ParentThing = &creature; mode.PCreature = &creature;
    CAnimateThing anim; memset(&anim, 0, sizeof(anim)); anim.calls = 0;
    mode.AnimUpdate(anim);                                 // no combat TC
    if (anim.calls != 0) ok = 0;
    creature.m_presentBits[1] |= 1UL << (0x30 & 31);
    mode.AnimUpdate(anim);
    if (anim.calls != 1 || anim.last.AnimIndex != 33 || anim.last.AnimType != 0x1a || anim.last.Mode != 1 || anim.last.Flags != 0x55 || anim.last.Percent != 100 || anim.last.Flag05) ok = 0;
    if (anim.last.Blend.m_first != 0.0f || anim.b != 9 || anim.t != 7 || entry.lastThing != &creature) ok = 0;
    anim.AnimsEnd = (void*)4; anim.CurrentAnimType = 0x1a;  // already blocking
    mode.AnimUpdate(anim);
    if (anim.calls != 1) ok = 0;
    anim.CurrentAnimFlags = 2; mode.AnimUpdate(anim);       // blocking anim finishing: restart
    if (anim.calls != 2) ok = 0;
    creature.busy = true; mode.AnimUpdate(anim);
    if (anim.calls != 2) ok = 0;
    if (ok) { printf("008d8dd0_TEST PASS\n"); return 0; }
    printf("008d8dd0_TEST FAIL\n"); return 1;
}