#include <cassert>
#include <cstdio>
#include <vector>

struct C3DAnimationStateAlpha
{
    unsigned AnimationIndex;
    float AnimationTime;
    float AlphaWeight;
};

static float GFGetInterpolatedValue(float old_value, float new_value, float weight)
{
    const float delta = (new_value - old_value) * weight;
    return old_value + delta;
}

struct MorphPointer
{
    const int* value;
    MorphPointer() : value(0) { }
    bool IsNull() const { return value==0; }
};

struct BlendData
{
    float BaseAnimation;
    std::vector<C3DAnimationStateAlpha> CombinationBlends;
    MorphPointer SkeletalMorph;
};

class C3DAnimationBlendState
{
    BlendData state;
public:
    BlendData* Data;
    unsigned copies;
    C3DAnimationBlendState() : Data(&state), copies(0) { state.BaseAnimation=0.0f; }
    void MakeOwnCopyOfData() { ++copies; }
    float PeekBaseAnimation() const { return Data->BaseAnimation; }
    std::size_t GetCombinationBlendCount() const { return Data->CombinationBlends.size(); }
    const C3DAnimationStateAlpha& PeekCombinationBlend(unsigned long index) const
    { return Data->CombinationBlends[index]; }
    static float CreateInterpolate(float x,float y,float ratio) { return GFGetInterpolatedValue(x,y,ratio); }
    void SetInterpolate(const C3DAnimationBlendState&,const C3DAnimationBlendState&,float);
};

// The selected implementation is reproduced verbatim for this standalone fixture.
void C3DAnimationBlendState::SetInterpolate(const C3DAnimationBlendState& anim1, const C3DAnimationBlendState& anim2, float weight2)
{
    MakeOwnCopyOfData();
    Data->BaseAnimation = CreateInterpolate(anim1.PeekBaseAnimation(), anim2.PeekBaseAnimation(), weight2);
    unsigned long no_blends = anim1.GetCombinationBlendCount();
    Data->CombinationBlends.resize(no_blends, C3DAnimationStateAlpha());
    for (unsigned long c_blend = 0; c_blend < no_blends; c_blend++) {
        C3DAnimationStateAlpha& dest = Data->CombinationBlends[c_blend];
        const C3DAnimationStateAlpha& blend1 = anim1.PeekCombinationBlend(c_blend);
        const C3DAnimationStateAlpha& blend2 = anim2.PeekCombinationBlend(c_blend);
        dest.AnimationIndex = blend1.AnimationIndex;
        dest.AnimationTime = GFGetInterpolatedValue(blend1.AnimationTime, blend2.AnimationTime, weight2);
        dest.AlphaWeight = (GFGetInterpolatedValue(blend1.AlphaWeight, blend2.AlphaWeight, weight2));
    }
    if (!anim1.Data->SkeletalMorph.IsNull())
        Data->SkeletalMorph = anim1.Data->SkeletalMorph;
}

int main()
{
    int first_morph = 12, old_morph = 8, second_morph = 16;
    for (unsigned count = 0; count < 13; ++count) {
        for (unsigned scenario = 0; scenario < 6; ++scenario) {
            C3DAnimationBlendState first, second, result;
            first.Data->BaseAnimation = 1.0f;
            second.Data->BaseAnimation = 5.0f;
            result.Data->SkeletalMorph.value = &old_morph;
            first.Data->SkeletalMorph.value = scenario < 3 ? 0 : &first_morph;
            second.Data->SkeletalMorph.value = &second_morph;
            result.Data->CombinationBlends.resize((scenario & 1) ? 16 : 0);
            for (unsigned i = 0; i < count; ++i) {
                C3DAnimationStateAlpha a = { i + 1, float(i) - 1.0f, 0.125f * float(i) };
                C3DAnimationStateAlpha b = { i + 21, float(i) + 7.0f, 0.125f * float(i) + 1.0f };
                first.Data->CombinationBlends.push_back(a);
                second.Data->CombinationBlends.push_back(b);
            }
            const float weight = 0.5f * (scenario % 3);
            result.SetInterpolate(first, second, weight);
            assert(result.copies == 1 && result.Data->BaseAnimation == 1.0f + 4.0f * weight);
            assert(result.Data->CombinationBlends.size() == count);
            assert(result.Data->SkeletalMorph.value == (scenario < 3 ? &old_morph : &first_morph));
            for (unsigned i = 0; i < count; ++i) {
                assert(result.Data->CombinationBlends[i].AnimationIndex == i + 1);
                assert(result.Data->CombinationBlends[i].AnimationTime == float(i) - 1.0f + 8.0f * weight);
                assert(result.Data->CombinationBlends[i].AlphaWeight == 0.125f * float(i) + weight);
                assert(first.Data->CombinationBlends[i].AnimationIndex == i + 1);
                assert(second.Data->CombinationBlends[i].AnimationIndex == i + 21);
            }
        }
    }
    std::puts("00a3c3b0_TEST PASS");
    return 0;
}
