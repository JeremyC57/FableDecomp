// behaviour copy of NCreatureMode::CModeOutOfActionBase::OnActivate: recovery countdown in frames, suspend in state 1, head emitter
// when the mode has an emitter type and the creature allows effects, then the derived anim hook and the stand-mode activation
#include <cstdio>
static long g_framesPerSecond = 30;
static int g_suspend = -1, g_created = 0, g_hook = 0, g_base = 0; static long g_type = -9;
struct CThing { unsigned char m_state; unsigned char m_flags6c; void SetSuspendAnimation(bool s, long) { g_suspend = s; } };
struct C3DVector { float X, Y, Z; };
static C3DVector GetHeadPosWithOffset(CThing*) { C3DVector v = { 1, 2, 3 }; return v; }
static CThing g_effect;
static CThing* CreateEffectThing(long type, const C3DVector* p, bool) { ++g_created; g_type = type; return p->Z == 3 ? &g_effect : 0; }
struct CIP { CThing* PItem; CIP() : PItem(0) {} CIP(CThing* t) : PItem(0) { SetPItem(t); } CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* t) { PItem = t; } CIP& operator=(const CIP& o) { SetPItem(o.GetPItem()); return *this; } };
struct CModeOutOfActionBase {
    CThing* ParentThing; long RecoveryCount; bool Recoverable; CIP PEmitter; long emitterType;
    float GetRecoveryTime() { return 1.5f; }
    long GetEmitterType() { return emitterType; }
    void StartOutOfActionAnim() { ++g_hook; }
    void BaseOnActivate() { ++g_base; }
    void OnActivate()
    {
        long framesPerSecond = g_framesPerSecond;
        RecoveryCount = (long)(framesPerSecond * GetRecoveryTime());
        if (ParentThing->m_state == 1)
            ParentThing->SetSuspendAnimation(true, 0);
        long type = GetEmitterType();
        if (type >= 0 && (ParentThing->m_flags6c & 8)) {
            C3DVector position = GetHeadPosWithOffset(ParentThing);
            PEmitter = CIP(CreateEffectThing(type, &position, false));
        }
        StartOutOfActionAnim();
        BaseOnActivate();
    }
};
int main()
{
    CThing c; c.m_state = 1; c.m_flags6c = 8;
    CModeOutOfActionBase m; m.ParentThing = &c; m.emitterType = 4;
    m.OnActivate();
    if (m.RecoveryCount != 45 || g_suspend != 1 || g_created != 1 || g_type != 4 || m.PEmitter.PItem != &g_effect || g_hook != 1 || g_base != 1)
    { std::printf("008d7270_TEST FAIL\n"); return 1; }
    m.emitterType = -1; m.PEmitter.PItem = 0; m.OnActivate();
    if (g_created != 1 || m.PEmitter.PItem != 0 || g_hook != 2) { std::printf("008d7270_TEST FAIL\n"); return 1; }
    std::printf("008d7270_TEST PASS\n");
    return 0;
}
