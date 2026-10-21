// behaviour test for 0x00437950
#include <stdio.h>
namespace NPlayerGui {
class CDrawQuickAccessIcons {
public:
    int Hits;
    void OnHeroHit() { ++Hits; }
};
}
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
class CPlayerGui {
public:
    unsigned char                                  _pad00[0x1d8];
    CCountedPointer<NPlayerGui::CDrawQuickAccessIcons> PDrawQuickAccessIcons;   // +0x1d8
    void OnHeroHit(float damage);
};
void CPlayerGui::OnHeroHit(float damage)
{
    if (damage != 0.0001f)
        PDrawQuickAccessIcons->OnHeroHit();
}

int main()
{
    static CPlayerGui g; NPlayerGui::CDrawQuickAccessIcons icons; icons.Hits = 0; g.PDrawQuickAccessIcons.Data = &icons;
    g.OnHeroHit(5.0f); g.OnHeroHit(0.0001f); g.OnHeroHit(0.0f);
    int ok = icons.Hits == 2;
    printf(ok ? "00437950_TEST PASS\n" : "00437950_TEST FAIL\n");
    return ok ? 0 : 1;
}