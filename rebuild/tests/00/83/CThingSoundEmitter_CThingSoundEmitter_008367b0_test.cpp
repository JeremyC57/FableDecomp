#include <cstdio>
struct CThing {
    virtual void v000() {}
    unsigned long m_type; bool m_killOnUnload;
    CThing(unsigned long thingType) : m_type(thingType), m_killOnUnload(true) {}
    void SetToKillOnLevelUnload(bool kill) { m_killOnUnload = kill; }
};
struct CEngine {
    bool m_persistent;
    virtual bool IsLevelPersistent() { return m_persistent; }
};
CEngine* g_pEngine;
struct CBaseIntelligentPointer {
    virtual void v000() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() {}
    virtual void v000() {}
};
struct CSoundPlayer;
struct C3DVector { float x, y, z; };
struct CThingSoundEmitter : CThing {
    CSoundPlayer*               m_pSoundPlayer;
    bool                        AttachedToThing;
    CIntelligentPointer<CThing> PAttachedThing;
    bool                        ContinuePlayingAfterAttachedThingDeath;
    C3DVector                   Pos;
    CThingSoundEmitter();
};
CThingSoundEmitter::CThingSoundEmitter()
    : CThing(6), m_pSoundPlayer(0), AttachedToThing(false), ContinuePlayingAfterAttachedThingDeath(false)
{
    if (!g_pEngine->IsLevelPersistent())
        SetToKillOnLevelUnload(false);
}
int main()
{
    int bad = 0;
    CEngine eng; eng.m_persistent = true; g_pEngine = &eng;
    CThingSoundEmitter a;
    if (a.m_type != 6 || a.m_pSoundPlayer != 0 || a.AttachedToThing || a.ContinuePlayingAfterAttachedThingDeath ||
        a.PAttachedThing.m_pItem != 0 || !a.m_killOnUnload) bad = 1;
    eng.m_persistent = false;
    CThingSoundEmitter b;
    if (b.m_killOnUnload) bad = 2;
    if (bad) { std::printf("008367b0_TEST FAIL\n"); return 1; }
    std::printf("008367b0_TEST PASS\n");
    return 0;
}