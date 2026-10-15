#include <cstdio>
// CModeJackOfBladesFlash::OnDeactivate model.
struct CThing {
    bool dead; int kills; bool animSuspended; int resumes;
    void Kill(bool) { ++kills; dead = true; }
    void SetSuspendAnimation(bool suspend, void*) { if (!suspend) { ++resumes; animSuspended = false; } }
};
struct CIP { CThing* item; CThing* GetPItem() const { return item; } };
struct CModeJackOfBladesFlash {
    bool Active; int flamesOut;
    CThing* PCreature; CIP PEmitter; CIP PSpotLight;
    void ExtinguishFlames() { ++flamesOut; }
    void OnDeactivate()
    {
        if (PEmitter.GetPItem() && !PEmitter.GetPItem()->dead)
            PEmitter.GetPItem()->Kill(true);
        if (PSpotLight.GetPItem() && !PSpotLight.GetPItem()->dead)
            PSpotLight.GetPItem()->Kill(true);
        ExtinguishFlames();
        if (PCreature->animSuspended)
            PCreature->SetSuspendAnimation(false, 0);
        Active = false;
    }
};
int main()
{
    CThing creature = { false, 0, true, 0 }, emitter = { false, 0, false, 0 };
    CModeJackOfBladesFlash m; m.Active = true; m.flamesOut = 0; m.PCreature = &creature;
    m.PEmitter.item = &emitter; m.PSpotLight.item = 0;
    m.OnDeactivate();
    if (!(emitter.kills == 1 && m.flamesOut == 1 && creature.resumes == 1 && !m.Active)) { std::printf("008df310_TEST FAIL\n"); return 1; }
    std::printf("008df310_TEST PASS\n");
    return 0;
}
