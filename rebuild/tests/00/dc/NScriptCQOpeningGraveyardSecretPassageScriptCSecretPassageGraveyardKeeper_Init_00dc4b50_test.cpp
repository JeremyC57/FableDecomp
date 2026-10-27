// behaviour test for 0x00DC4B50 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Init() (model: the listed members are cleared, the others untouched)
#include <cstdio>
#include <cstring>
struct S {
    bool already_talked_to;
    bool Helmfound;
    bool Swordfound;
    bool Shieldfound;
    bool Armourfound;
    void Init()
    {
        already_talked_to = false;
        Helmfound = false;
        Swordfound = false;
        Shieldfound = false;
        Armourfound = false;
    }
};
int main()
{
    S s; std::memset(&s, 0x5A, sizeof(s));
    s.Init();
    int ok = 1;
    if (s.already_talked_to != 0) ok = 0;
    if (s.Helmfound != 0) ok = 0;
    if (s.Swordfound != 0) ok = 0;
    if (s.Shieldfound != 0) ok = 0;
    if (s.Armourfound != 0) ok = 0;
    if (!ok) { std::printf("00dc4b50_TEST FAIL\n"); return 1; }
    std::printf("00dc4b50_TEST PASS\n");
    return 0;
}