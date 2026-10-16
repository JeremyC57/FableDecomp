// CThingCompare_BestCreatureToAttack::operator()<CThingCreatureBase>(CThingCreatureBase* a, CThingCreatureBase* b) -- pick the better attack target of two: none when neither is alive, the live one when only one is, else the one with the higher compare value
// Grade: DIFFER(104v110) -- block layout: with separate returns VC7.1 lays the `return 0` inline after the first alive test (112 bytes, `je` sense flipped); with a result variable (this form) the layout matches retail but the zero result is hoisted (`xor eax,eax` at entry, mask in cl instead of al). Tried 4 return arrangements and a goto.
// Leaf: ??R
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F6F20, 110 bytes (`ret 8`).
// Callees: CThingCompare_BestCreatureToAttack::GetCompareValueForThing<CThingCreatureBase>(CThingCreatureBase*) @ 0x007F6BF0 (float).
// Identity: a template instantiation of the comparator used by GetPBestThing searches; the manifest label operator()<CThingCreatureBase> is kept (caller 0x007F65B9).
// Layout: retail CThing Dead flag = +0x91 bit 0.
// Shape: nested form (`return 0` last, `return b` shared by the dead-a and lost-compare arms); IS_THING_ALIVE inline (`p && !(p->Dead)`, the bit mask kept in al); the second test re-checks a; b is only
//   null-tested before the compare; `valueA > valueB` with valueA spilled (`fcomp ; test ah,5 ; jp`).
struct CThing {
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                     // +0x91 bit 0
};
struct CThingCreatureBase : CThing { };
struct CThingAICreature : CThingCreatureBase { };
template<class T> __forceinline bool IS_THING_ALIVE(T* p) { return p && !p->Dead; }

struct CThingCompare_BestCreatureToAttack {
    template<class T> float GetCompareValueForThing(T* thing) const;    // 0x007F6BF0 for CThingCreatureBase
    template<class T> T* operator()(T* a, T* b) const
    {
        T* best = 0;
        if (IS_THING_ALIVE(a) || IS_THING_ALIVE(b)) {
            best = b;
            if (IS_THING_ALIVE(a)) {
                best = a;
                if (b) {
                    float valueA = GetCompareValueForThing(a);
                    if (!(valueA > GetCompareValueForThing(b)))
                        best = b;
                }
            }
        }
        return best;
    }
};

template CThingCreatureBase* CThingCompare_BestCreatureToAttack::operator()(CThingCreatureBase* a, CThingCreatureBase* b) const;
