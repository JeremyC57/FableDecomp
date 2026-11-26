// Behaviour test for 0x00914EF0 CAIStateGroup_KidHurtSelfRunToMother::OnActivate(): clears AddedSadMode.
#include <cstdio>
struct CKidModel { bool AddedSadMode; void OnActivate() { AddedSadMode = false; } };
int main()
{
    CKidModel k; k.AddedSadMode = true; k.OnActivate();
    if (k.AddedSadMode) { std::printf("00914ef0_TEST FAIL\n"); return 1; }
    k.OnActivate(); if (k.AddedSadMode) { std::printf("00914ef0_TEST FAIL\n"); return 1; }
    std::printf("OK_00914ef0\n"); return 0;
}
