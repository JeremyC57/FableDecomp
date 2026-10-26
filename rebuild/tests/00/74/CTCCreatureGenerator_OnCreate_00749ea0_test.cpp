#include <cstdio>
class CCreatureGeneratorDef { public: long SecondsBetweenEffects; long SecondsBetweenSoundEffects; };
struct CDefPointer_CreatureGeneratorDef { const CCreatureGeneratorDef* Object; };
class CDefClassBase { public:
    const CCreatureGeneratorDef* m_def;
    bool GetSubDef(CDefPointer_CreatureGeneratorDef& out) { out.Object = m_def; return m_def != 0; }
};
class CThing { public:
    CDefClassBase* DefClass;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
};
static long g_FramesPerSecond = 15;
class CTCCreatureGenerator { public:
    CThing* ParentThing; long SelfTriggerResetInterval; long ActivationFrame;
    CDefPointer_CreatureGeneratorDef PDef; long FramesUntilRandomEffect; long FramesUntilRandomSoundEffect;
    void OnCreate()
    {
        ParentThing->DefClass->GetSubDef(PDef);
        if (PDef.Object) {
            FramesUntilRandomEffect = PDef.Object->SecondsBetweenEffects * g_FramesPerSecond;
            FramesUntilRandomSoundEffect = PDef.Object->SecondsBetweenSoundEffects * g_FramesPerSecond;
        }
        ActivationFrame = -(SelfTriggerResetInterval * g_FramesPerSecond);
        ParentThing->LevelPersistent = 1;
    }
};
int main()
{
    CCreatureGeneratorDef def = { 4, 10 };
    CDefClassBase dc = { &def };
    CThing thing; thing.DefClass = &dc;
    thing.DeletedOnOwnerLevelOutOfScope = 1; thing.Drawable = 0; thing.Dying = 0; thing.ForceFrameUpdate = 0;
    thing.LevelPersistent = 0; thing.GameStatePersisted = 0; thing.PersistOnUnloadWhenInLimbo = 0; thing.PausedDuringCutscenes = 0;
    CTCCreatureGenerator g; g.ParentThing = &thing; g.SelfTriggerResetInterval = 20; g.ActivationFrame = 0;
    g.PDef.Object = 0; g.FramesUntilRandomEffect = -1; g.FramesUntilRandomSoundEffect = -1;
    g.OnCreate();
    if (g.PDef.Object != &def || g.FramesUntilRandomEffect != 60 || g.FramesUntilRandomSoundEffect != 150
        || g.ActivationFrame != -300 || !thing.LevelPersistent || !thing.DeletedOnOwnerLevelOutOfScope || thing.Drawable)
    { std::printf("00749ea0_TEST FAIL\n"); return 1; }
    dc.m_def = 0; g.FramesUntilRandomEffect = -1; g.FramesUntilRandomSoundEffect = -1; g.SelfTriggerResetInterval = 2;
    g.OnCreate();
    if (g.PDef.Object != 0 || g.FramesUntilRandomEffect != -1 || g.FramesUntilRandomSoundEffect != -1 || g.ActivationFrame != -30)
    { std::printf("00749ea0_TEST FAIL\n"); return 1; }
    std::printf("00749ea0_TEST PASS\n");
    return 0;
}