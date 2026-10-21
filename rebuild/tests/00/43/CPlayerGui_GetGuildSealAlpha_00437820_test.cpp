#include <stdio.h>
static const void* g_this;
namespace NPlayerGui { struct CDrawGuildSeal { float a; float GetAlpha() const { g_this = this; return a; } }; }
struct CCPPointerInfo;
template <class T> struct CCountedPointer { T* Data; CCPPointerInfo* Info; T* operator->() const { return Data; } };
struct CPlayerGui {
    unsigned char _pad00[0x1d0];
    CCountedPointer<NPlayerGui::CDrawGuildSeal> PDrawGuildSeal;
    float GetGuildSealAlpha() const;
};

float CPlayerGui::GetGuildSealAlpha() const
{
    float alpha = PDrawGuildSeal->GetAlpha();
    return alpha;
}

int main()
{
    static CPlayerGui g; NPlayerGui::CDrawGuildSeal s; s.a = 0.5f;
    g.PDrawGuildSeal.Data = &s;
    bool ok = g.GetGuildSealAlpha() == 0.5f && g_this == &s && (char*)&g.PDrawGuildSeal - (char*)&g == 0x1d0;
    s.a = 1.0f; ok = ok && g.GetGuildSealAlpha() == 1.0f;
    printf(ok ? "00437820_TEST PASS\n" : "00437820_TEST FAIL\n");
    return ok ? 0 : 1;
}