// Behaviour test for CCreatureAction_ScreamerDie::FrameUpdate (0x008B8B80): the creature's appearance alpha is
// (unsigned char)((1 - percentage complete) * 255), then the combat-action base FrameUpdate runs.
#include <cstdio>
static int gOrder, gAlphaAt, gBaseAt, gAlphaCalls; static unsigned char gAlpha;
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha) { ++gAlphaCalls; gAlphaAt = ++gOrder; gAlpha = alpha; }
};
class CBaseObject { public: virtual ~CBaseObject() { } };
class CThing : public CBaseObject { public:
    CTCGraphicAppearance* PTCGraphicAppearance;
    CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }
};
class CThingCreatureBase : public CThing { };
class CBaseIntelligentPointer { public:
    CBaseObject* Item;
    CBaseObject* GetPItem() const { return Item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CCreatureActionBase : public CBaseObject { public:
    long FramesRun, TotalFrames;
    float GetPercentageComplete() const { return (float)FramesRun / (float)TotalFrames; }
};
class CCombatActionBase : public CCreatureActionBase { public:
    CIntelligentPointer<CThingCreatureBase> PCreature;
    virtual void FrameUpdate() { gBaseAt = ++gOrder; }
};
class CCreatureAction_ScreamerDie : public CCombatActionBase { public:
    virtual void FrameUpdate();
};
void CCreatureAction_ScreamerDie::FrameUpdate()
{
    PCreature.GetPItem()->GetTCGraphicAppearance()->SetAlpha((unsigned char)((1.0f - GetPercentageComplete()) * 255.0f));
    CCombatActionBase::FrameUpdate();
}
int main()
{
    CTCGraphicAppearance appearance; CThingCreatureBase screamer; screamer.PTCGraphicAppearance = &appearance;
    CCreatureAction_ScreamerDie die; die.PCreature.Item = &screamer; die.TotalFrames = 4;
    const unsigned char expect[5] = { 255, 191, 127, 63, 0 };
    for (long f = 0; f <= 4; ++f) {
        gOrder = 0; die.FramesRun = f; die.FrameUpdate();
        if (gAlpha != expect[f] || gAlphaAt != 1 || gBaseAt != 2) { std::printf("008b8b80_TEST FAIL\n"); return 1; }
    }
    if (gAlphaCalls != 5) { std::printf("008b8b80_TEST FAIL\n"); return 1; }
    std::printf("008b8b80_TEST PASS\n");
    return 0;
}