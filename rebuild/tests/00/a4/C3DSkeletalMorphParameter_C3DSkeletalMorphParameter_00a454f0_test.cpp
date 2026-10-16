// behaviour test for C3DSkeletalMorphParameter::C3DSkeletalMorphParameter (0x00A454F0)
#include <cstdio>
struct Target { int name; int bones; };
struct Shared { Target Positive, Negative; bool Valid; };
struct Param {
    float CurrentValue; void* SkeletalMorph; Shared* SharedData; Shared storage;
    Param(const Target& pos, const Target& neg)
    {
        SharedData = &storage; storage.Valid = false;
        SharedData->Positive = pos; SharedData->Negative = neg; SharedData->Valid = true;
        CurrentValue = 0; SkeletalMorph = 0;
    }
};
int main()
{
    Target p = { 1, 2 }, n = { 3, 4 };
    Param x(p, n);
    bool ok = x.SharedData->Positive.name == 1 && x.SharedData->Negative.bones == 4 && x.SharedData->Valid && x.CurrentValue == 0 && x.SkeletalMorph == 0;
    if (!ok) { std::printf("00a454f0_TEST FAIL\n"); return 1; }
    std::printf("00a454f0_TEST PASS\n");
    return 0;
}