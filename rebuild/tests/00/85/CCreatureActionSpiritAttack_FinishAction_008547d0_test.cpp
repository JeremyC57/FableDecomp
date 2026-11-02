// Behaviour test for 0x008547D0 CCreatureAction_SpiritAttack::FinishAction(): a live particle emitter is killed (immediately),
// a dead or missing one is left alone; the base FinishAction runs in every case.
#include <cstdio>

static int g_baseFinish = 0;

class CThing {
public:
    CThing() : Dead(0), Kills(0), LastImmediate(false) { }
    unsigned char Dead : 1;
    int Kills;
    bool LastImmediate;
    bool IsDead() const { return Dead; }
    void Kill(bool immediately) { ++Kills; LastImmediate = immediately; }
};

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : Item(0) { }
    virtual ~CBaseIntelligentPointer() { }
    void* GetPItem() const { return Item; }
    void SetPItem(void* item) { Item = item; }
private:
    void* Item;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};

template <class T> inline bool IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase() { }
    virtual void FinishAction() { ++g_baseFinish; }
};

class CCreatureAction_SpiritAttack : public CCreatureActionBase {
    virtual void FinishAction();
public:
    void CallFinish() { FinishAction(); }
    CIntelligentPointer<CThing> PTarget;
    CIntelligentPointer<CThing> PParticleEmitter;
};

void CCreatureAction_SpiritAttack::FinishAction()
{
    if (IS_THING_ALIVE(PParticleEmitter))
        PParticleEmitter->Kill(true);
    CCreatureActionBase::FinishAction();
}

int main()
{
    CThing emitter;
    CCreatureAction_SpiritAttack a;
    a.CallFinish();                                           // no emitter
    if (g_baseFinish != 1) { std::printf("008547d0_TEST FAIL\n"); return 1; }
    a.PParticleEmitter.SetPItem(&emitter);
    a.CallFinish();                                           // live emitter
    if (emitter.Kills != 1 || !emitter.LastImmediate || g_baseFinish != 2) { std::printf("008547d0_TEST FAIL\n"); return 1; }
    emitter.Dead = 1;
    a.CallFinish();                                           // dead emitter
    if (emitter.Kills != 1 || g_baseFinish != 3) { std::printf("008547d0_TEST FAIL\n"); return 1; }
    std::printf("008547d0_TEST PASS\n");
    return 0;
}