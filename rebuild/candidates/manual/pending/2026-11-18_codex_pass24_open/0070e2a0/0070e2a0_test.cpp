#include <cassert>
#include <cstdio>
#include <list>

struct C3DSkeletalMorph { int id; };

class MorphPointer
{
    const C3DSkeletalMorph* value;
public:
    explicit MorphPointer(const C3DSkeletalMorph* v) : value(v) { }
    bool IsNull() const { return value==0; }
    const C3DSkeletalMorph* Get() const { return value; }
};

struct CTCSkeletalMorph
{
    const C3DSkeletalMorph* morph;
    mutable unsigned calls;
    MorphPointer GetPSkeletalMorph() const { ++calls; return MorphPointer(morph); }
};

struct CThing
{
    const CTCSkeletalMorph* appearance;
    bool PeekTC(const CTCSkeletalMorph** out) const
    {
        *out = appearance;
        return appearance != 0;
    }
};

struct C3DAnimationBlendState
{
    float blend;
    const C3DSkeletalMorph* morph;
    void SetSkeletalMorph(const MorphPointer& value) { morph = value.Get(); }
};

class CAnimateThing
{
public:
    int CurrentAnim;
    std::list<int> DelayedAnimQueue;
    CThing ParentThing;
    mutable C3DAnimationBlendState StartAnimState, EndAnimState;
    mutable unsigned interpolation_calls;

    C3DAnimationBlendState CalcInterpolatedMeshAnim(int current, int delayed, float amount) const
    {
        ++interpolation_calls;
        assert(current == 15 && delayed == 27);
        C3DAnimationBlendState state = { amount, 0 };
        return state;
    }

    const CThing& GetParentThing() const { return ParentThing; }

    void CalcCachedAnims() const;

};

// The selected implementation is reproduced verbatim for this standalone fixture.
void CAnimateThing::CalcCachedAnims() const
{
    const CTCSkeletalMorph* ptc_appearance_morph;
    StartAnimState = CalcInterpolatedMeshAnim(CurrentAnim, DelayedAnimQueue.front(), 0.0f);
    EndAnimState = CalcInterpolatedMeshAnim(CurrentAnim, DelayedAnimQueue.front(), 1.0f);
    if (GetParentThing().PeekTC(&ptc_appearance_morph)) {
        if (!ptc_appearance_morph->GetPSkeletalMorph().IsNull()) {
            StartAnimState.SetSkeletalMorph(ptc_appearance_morph->GetPSkeletalMorph());
            EndAnimState.SetSkeletalMorph(ptc_appearance_morph->GetPSkeletalMorph());
        }
    }
}

int main()
{
    C3DSkeletalMorph morph = { 42 };
    CTCSkeletalMorph component = { 0, 0 };
    CAnimateThing animated;
    animated.CurrentAnim = 15;
    animated.DelayedAnimQueue.push_back(27);
    animated.DelayedAnimQueue.push_back(90);
    for (unsigned i = 0; i < 3; ++i) {
        component.morph = i == 2 ? &morph : 0;
        component.calls = 0;
        animated.ParentThing.appearance = i == 0 ? 0 : &component;
        animated.interpolation_calls = 0;
        animated.StartAnimState.morph = &morph;
        animated.EndAnimState.morph = &morph;
        animated.CalcCachedAnims();
        assert(animated.interpolation_calls == 2);
        assert(animated.StartAnimState.blend == 0.0f && animated.EndAnimState.blend == 1.0f);
        assert(animated.StartAnimState.morph == component.morph &&
               animated.EndAnimState.morph == component.morph);
        assert(component.calls == (i == 0 ? 0U : i == 1 ? 1U : 3U));
    }
    std::puts("0070e2a0_TEST PASS");
    return 0;
}
