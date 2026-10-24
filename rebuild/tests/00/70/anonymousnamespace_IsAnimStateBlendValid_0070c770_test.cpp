// Behaviour test for 0x0070C770 IsAnimStateBlendValid (base animation + every combination blend must be valid).
#include <cstdio>

struct CState { int AnimationIndex; };
struct CBank { bool loaded[8]; };
static int g_checks = 0;
static bool IsAnimationStateBaseValid(const CBank& bank, const CState& state) { ++g_checks; return bank.loaded[state.AnimationIndex]; }

struct CSharedData { CState* BaseAnimation; CState* blendsStart; CState* blendsFinish; };
struct CBlend {
    CSharedData* Data;
    unsigned int GetCombinationBlendCount() const { return Data == 0 ? 0 : (unsigned int)(Data->blendsFinish - Data->blendsStart); }
};

static bool IsAnimStateBlendValid(const CBank& bank, const CBlend& blend)
{
    if (!blend.Data)
        return false;
    if (!IsAnimationStateBaseValid(bank, *blend.Data->BaseAnimation))
        return false;
    for (unsigned int i = 0; i != blend.GetCombinationBlendCount(); ++i)
        if (!IsAnimationStateBaseValid(bank, blend.Data->blendsStart[i]))
            return false;
    return true;
}

int main()
{
    CBank bank = { { true, true, false, true, true, true, true, true } };
    CState base = { 0 }, badBase = { 2 };
    CState blends[3] = { { 1 }, { 3 }, { 4 } };
    CState badBlends[3] = { { 1 }, { 2 }, { 4 } };
    CSharedData good = { &base, blends, blends + 3 }, bad1 = { &badBase, blends, blends + 3 }, bad2 = { &base, badBlends, badBlends + 3 }, none = { &base, 0, 0 };
    CBlend b0 = { 0 }, b1 = { &good }, b2 = { &bad1 }, b3 = { &bad2 }, b4 = { &none };
    bool ok = !IsAnimStateBlendValid(bank, b0) && g_checks == 0;
    ok = ok && IsAnimStateBlendValid(bank, b1) && g_checks == 4;
    ok = ok && !IsAnimStateBlendValid(bank, b2) && g_checks == 5;
    ok = ok && !IsAnimStateBlendValid(bank, b3) && g_checks == 8;
    ok = ok && IsAnimStateBlendValid(bank, b4) && g_checks == 9;
    if (!ok) { std::printf("0070c770_TEST FAIL\n"); return 1; }
    std::printf("0070c770_TEST PASS\n");
    return 0;
}