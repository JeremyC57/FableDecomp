// behaviour test for CAIStateGroupBehaviourBase::Initialise (0x00957940)
#include <cstdio>
struct CThing { void* tc34; void* tc4f; void* tc31; };
struct Group {
    CThing* creature; bool baseInit; int hooks; void* perception; void* opinion; void* mode;
    void Initialise()
    {
        baseInit = true;
        if (creature->tc34) perception = creature->tc34;
        if (creature->tc4f) opinion = creature->tc4f;
        if (creature->tc31) mode = creature->tc31;
        ++hooks;
    }
};
int main()
{
    int a, b; CThing c = { &a, 0, &b };
    Group g = { &c, false, 0, 0, (void*)1, 0 };
    g.Initialise();
    bool ok = g.baseInit && g.hooks == 1 && g.perception == &a && g.opinion == (void*)1 && g.mode == &b;
    if (!ok) { std::printf("00957940_TEST FAIL\n"); return 1; }
    std::printf("00957940_TEST PASS\n");
    return 0;
}
