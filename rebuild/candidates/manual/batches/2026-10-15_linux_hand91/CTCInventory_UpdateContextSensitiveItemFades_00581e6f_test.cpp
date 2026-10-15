#include <cstdio>
// Behaviour test for CTCInventory::UpdateContextSensitiveItemFades (retail 0x00581E6F).
struct CFadeInFadeOutBase_Calls {
    float OpenTimer;
    int   updates;
    void Update(float elapsed) { OpenTimer += elapsed; ++updates; }
};
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
struct CEnginePrimitivePrepareInfo { CInterpolationInfoSet Current; };
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
struct CFadeVector {
    CFadeInFadeOutBase_Calls* first; CFadeInFadeOutBase_Calls* last; CFadeInFadeOutBase_Calls* end_of_storage;
    CFadeInFadeOutBase_Calls& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventory {
    CLongVector ContextSensitiveItems;
    CFadeVector ContextSensitiveFadeTimes;
    void UpdateContextSensitiveItemFades(const CEnginePrimitivePrepareInfo& info);
};
void CTCInventory::UpdateContextSensitiveItemFades(const CEnginePrimitivePrepareInfo& info)
{
    for (unsigned long i = 0; i < ContextSensitiveItems.size(); ++i)
        ContextSensitiveFadeTimes[i].Update(info.Current.GTPredictedTimeSinceLastRenderFrame);
}
int main()
{
    long items[2] = { 3, 0 };
    CFadeInFadeOutBase_Calls fades[3];
    for (int k = 0; k < 3; ++k) { fades[k].OpenTimer = 0.0f; fades[k].updates = 0; }
    CTCInventory inv;
    inv.ContextSensitiveItems.first = items; inv.ContextSensitiveItems.last = items + 2; inv.ContextSensitiveItems.end_of_storage = items + 2;
    inv.ContextSensitiveFadeTimes.first = fades; inv.ContextSensitiveFadeTimes.last = fades + 3; inv.ContextSensitiveFadeTimes.end_of_storage = fades + 3;
    CEnginePrimitivePrepareInfo info; info.Current.GTPredictedTimeSinceLastRenderFrame = 0.25f; info.Current.WFInterpolate = 9.0f;
    inv.UpdateContextSensitiveItemFades(info);
    inv.UpdateContextSensitiveItemFades(info);
    if (fades[0].updates != 2 || fades[1].updates != 2 || fades[2].updates != 0 || fades[0].OpenTimer != 0.5f) { std::printf("00581e6f_TEST FAIL\n"); return 1; }
    std::printf("00581e6f_TEST PASS\n");
    return 0;
}
