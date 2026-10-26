#include <cstdio>
// Behaviour test for 006d4d00: the typed intelligent-pointer accessor returns GetPItem() cast to CThing*.
class CThing { public: int id; };
struct CBaseIntelligentPointer { void* PData; void* GetPItem() const { return PData; } };
struct CIntelligentPointer_CThing : CBaseIntelligentPointer { CThing* Get() const { return (CThing*)GetPItem(); } };
int main()
{
    CThing t; t.id = 7;
    CIntelligentPointer_CThing p; p.PData = &t;
    int ok = (p.Get() == &t && p.Get()->id == 7);
    p.PData = 0; ok = ok && (p.Get() == 0);
    if (!ok) { std::printf("006d4d00_TEST FAIL\n"); return 1; }
    std::printf("006d4d00_TEST PASS\n");
    return 0;
}