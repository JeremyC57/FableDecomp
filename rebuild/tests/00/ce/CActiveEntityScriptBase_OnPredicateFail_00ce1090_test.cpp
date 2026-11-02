// Behaviour test for 0x00CE1090 CActiveEntityScriptBase::OnPredicateFail: forwards to the bound entity script only when one is bound.
#include <cstdio>
static int g_calls = 0;
struct CEntityScriptBaseModel { virtual ~CEntityScriptBaseModel() { } virtual void OnPredicateFail() { ++g_calls; } };
template <class T> struct CCountedPointerModel { T* Data; void* Info; T* Get() const { return Data; } T* operator->() const { return Data; } };
struct CActiveEntityScriptBaseModel {
    CCountedPointerModel<CEntityScriptBaseModel> PEntityScript;
    void OnPredicateFail()
    {
        if (PEntityScript.Get())
            PEntityScript->OnPredicateFail();
    }
};
int main()
{
    CEntityScriptBaseModel script;
    CActiveEntityScriptBaseModel active;
    active.PEntityScript.Data = 0; active.PEntityScript.Info = 0;
    active.OnPredicateFail();
    if (g_calls != 0) { std::printf("00ce1090_TEST FAIL\n"); return 1; }
    active.PEntityScript.Data = &script;
    active.OnPredicateFail();
    active.OnPredicateFail();
    if (g_calls != 2) { std::printf("00ce1090_TEST FAIL\n"); return 1; }
    std::printf("00ce1090_TEST PASS\n");
    return 0;
}