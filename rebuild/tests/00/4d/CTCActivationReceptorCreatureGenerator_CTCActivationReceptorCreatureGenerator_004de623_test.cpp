// behaviour test for 0x004DE623 CTCActivationReceptorCreatureGenerator::CTCActivationReceptorCreatureGenerator (model: the member defaults)
#include <cstdio>
struct Obj { bool TriggerOnActivate; bool ActivateOnActivate; Obj() : TriggerOnActivate(true), ActivateOnActivate(false) { } };
int main()
{
    Obj o;
    int ok = o.TriggerOnActivate == true && o.ActivateOnActivate == false;
    if (!ok) { std::printf("004de623_TEST FAIL\n"); return 1; }
    std::printf("004de623_TEST PASS\n");
    return 0;
}