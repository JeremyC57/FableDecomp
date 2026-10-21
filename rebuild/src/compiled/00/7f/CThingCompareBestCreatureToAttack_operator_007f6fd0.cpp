// CThingCompare_BestCreatureToAttack::operator()(CThingAICreature* a, CThingAICreature* b) -- the <CThingAICreature> instantiation: pick the better attack target of two: none when neither is alive, the live one when only one is, else the one with the higher compare value
// Leaf: ??R
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F6FD0, 110 bytes (`ret 8`).
// Callees: CThingCompare_BestCreatureToAttack::GetCompareValueForThing<CThingAICreature>(CThingAICreature*) @ 0x007F6CB0 (float).
// Identity: a template instantiation of the comparator used by GetPBestThing searches; its caller 0x007F70A5 is GetPBestLeafThing<..., CThingCompare_BestCreatureToAttack, CThingAICreature>, so this is the <CThingAICreature> instantiation (manifest says <CThingCreatureBase>; both callees are labelled GetCompareValueForThing<CThingAICreature>).
// Layout: retail CThing Dead flag = +0x91 bit 0.
// Shape: nested form with separate returns (`return 0` last, `return b` shared by the dead-a and lost-compare arms);
//   IS_THING_ALIVE is a macro `((p) != 0 && !(p)->IsDead())` over an inline CThing::IsDead() (as an inline function VC
//   lays `return 0` inline after the first alive test); the bit mask is kept in al; the second test re-checks a; b is
//   only null-tested before the compare; `valueA > valueB` with valueA spilled (`fcomp ; test ah,5 ; jp`).
struct CThing {
    unsigned char _pad[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
struct CThingCreatureBase : CThing { };
struct CThingAICreature : CThingCreatureBase { };
#define IS_THING_ALIVE(p) ((p) != 0 && !(p)->IsDead())

struct CThingCompare_BestCreatureToAttack {
    template<class T> float GetCompareValueForThing(T* thing) const;    // 0x007F6CB0 for CThingAICreature
    template<class T> T* operator()(T* a, T* b) const
    {
        if (IS_THING_ALIVE(a) || IS_THING_ALIVE(b)) {
            if (IS_THING_ALIVE(a)) {
                if (b) {
                    float valueA = GetCompareValueForThing(a);
                    if (!(valueA > GetCompareValueForThing(b)))
                        return b;
                }
                return a;
            }
            return b;
        }
        return 0;
    }
};

template CThingAICreature* CThingCompare_BestCreatureToAttack::operator()(CThingAICreature* a, CThingAICreature* b) const;