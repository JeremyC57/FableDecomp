#include <cstdio>
// Behaviour of CTCHeroMorph::ApplyChanges (0x0071E130): nothing happens unless Changed; otherwise a morph-changed
// message is sent to the parent thing (and destroyed right after), texture morphs rebuilt, the graphic appearance
// flagged with 8 and Changed cleared.
static int g_order = 0; static int g_sent = 0, g_destroyed = 0, g_update = 0, g_flagged = 0; static unsigned long g_flags = 0; static bool g_ok = true;
namespace N_0099a2d0 { struct CBase {
    virtual long Kind() const { return 0; }
    CBase() {}
    ~CBase() { g_destroyed = ++g_order; }
}; } using N_0099a2d0::CBase;
struct CMessageOnMorphChanged : CBase {
    virtual long Kind() const { return 42; }
    CMessageOnMorphChanged() {}
};
struct CTCGraphicAppearance {
    void SetAsChanged(unsigned long flags) { g_flagged = ++g_order; g_flags = flags; }
};
class CThing { public:
    CTCGraphicAppearance* m_pGraphicAppearance;
    void SendMessage(CBase* message) { g_sent = ++g_order; if (message->Kind() != 42) g_ok = false; }
};
struct CTCHeroMorph_Retail {
    CThing* ParentThing;
    bool Changed;
    void UpdateTextureMorphs() { g_update = ++g_order; }
    void ApplyChanges()
    {
        if (Changed) {
            {
                CMessageOnMorphChanged message;
                ParentThing->SendMessage(&message);
            }
            UpdateTextureMorphs();
            ParentThing->m_pGraphicAppearance->SetAsChanged(8);
            Changed = false;
        }
    }
};
int main()
{
    CTCGraphicAppearance appearance; CThing thing; thing.m_pGraphicAppearance = &appearance;
    CTCHeroMorph_Retail m; m.ParentThing = &thing; m.Changed = false;
    m.ApplyChanges();
    if (g_order != 0) { std::printf("0071e130_TEST FAIL\n"); return 1; }
    m.Changed = true;
    m.ApplyChanges();
    if (!g_ok || g_sent != 1 || g_destroyed != 2 || g_update != 3 || g_flagged != 4 || g_flags != 8 || m.Changed) {
        std::printf("0071e130_TEST FAIL\n"); return 1;
    }
    std::printf("0071e130_TEST PASS\n");
    return 0;
}