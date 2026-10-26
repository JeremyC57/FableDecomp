#include <cstdio>
// CModeSummonerLightningOrb::OnDeactivate model.
class CThing { public:
    unsigned char type; bool dead; int kills; int resumes; long removedTC;
    void Kill(bool) { ++kills; dead = true; }
    void SetSuspendAnimation(bool suspend, void*) { if (!suspend) ++resumes; }
    void RemoveTC(long t) { removedTC = t; }
};
struct CIP { CThing* item; CThing* GetPItem() const { return item; } };
struct CModeSummonerLightningOrb {
    int baseCalls; CThing* m_pThing; CIP PChargeEffect; CIP POrbChargeEffect;
    void BaseOnDeactivate() { ++baseCalls; }
    void OnDeactivate()
    {
        BaseOnDeactivate();
        if (m_pThing && !m_pThing->dead) {
            if (m_pThing->type == 1)
                m_pThing->SetSuspendAnimation(false, this);
            m_pThing->RemoveTC(0xee);
        }
        if (POrbChargeEffect.GetPItem() && !POrbChargeEffect.GetPItem()->dead)
            POrbChargeEffect.GetPItem()->Kill(true);
        if (PChargeEffect.GetPItem() && !PChargeEffect.GetPItem()->dead)
            PChargeEffect.GetPItem()->Kill(true);
    }
};
int main()
{
    CThing parent = { 1, false, 0, 0, 0 }, orb = { 0, false, 0, 0, 0 }, charge = { 0, false, 0, 0, 0 };
    CModeSummonerLightningOrb m; m.baseCalls = 0; m.m_pThing = &parent; m.PChargeEffect.item = &charge; m.POrbChargeEffect.item = &orb;
    m.OnDeactivate();
    if (!(m.baseCalls == 1 && parent.resumes == 1 && parent.removedTC == 0xee && orb.kills == 1 && charge.kills == 1)) { std::printf("008d79f0_TEST FAIL\n"); return 1; }
    std::printf("008d79f0_TEST PASS\n");
    return 0;
}