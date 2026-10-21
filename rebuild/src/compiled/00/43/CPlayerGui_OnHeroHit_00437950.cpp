// CPlayerGui::OnHeroHit(float damage) -- forwards a non-trivial hit to the quick-access icon drawer
// Leaf: OnHeroHit@CPlayerGui
// Retail: Fable.exe @ 0x00437950, 33 bytes, `ret 4`; Ego_r.pdb ?OnHeroHit@CPlayerGui@@QAEXM@Z. Body: unless damage equals
// 0.0001f (float pool 0x0129BA3C; `fucompp ; test ah,44h ; jnp` skips on equality), call
// NPlayerGui::CDrawQuickAccessIcons::OnHeroHit (0x0064DA5C) through PDrawQuickAccessIcons. Only caller 0x006AB2E0
// (CThingPlayerCreature).
// Layout: PDrawQuickAccessIcons (CCountedPointer, Data first) at retail +0x1d8 (Ego_r.pdb +0x1e0; retail CPlayerGui is 8
// below the PDB here, as in 0x00437820), so the class is modelled locally.
namespace NPlayerGui {
class CDrawQuickAccessIcons {
public:
    void OnHeroHit();                                    // 0x0064DA5C
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