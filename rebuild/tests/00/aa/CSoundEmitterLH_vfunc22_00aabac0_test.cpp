#include <cstdio>
struct CActiveSound { long m_references; long m_handle; };
struct SSoundNode { SSoundNode* next; SSoundNode* prev; CActiveSound* sound; long extra; };
struct CSoundPlayer {
    int updates; long lastUpdate; long pairA0, pairA1, pairB0, pairB1; long state; int stateQueries;
    int enabledArg; long played; long activeCount;
    CSoundPlayer() : updates(0), lastUpdate(-1), pairA0(0), pairA1(0), pairB0(0), pairB1(0), state(0), stateQueries(0),
                     enabledArg(-1), played(-1), activeCount(0) {}
    void Update(long h) { ++updates; lastUpdate = h; }
    void SetPairA(long a, long b) { pairA0 = a; pairA1 = b; }
    void SetPairB(long a, long b) { pairB0 = a; pairB1 = b; }
    long GetState(long) { ++stateQueries; return state; }
    void SetEnabled(int e) { enabledArg = e; }
    void Play(long s) { played = s; }
    long GetActiveCount() { return activeCount; }
};
struct COutput { long value; COutput() : value(0) {} void SetValue(long v) { value = v; } };
struct CSoundSystem {
    CSoundPlayer player; COutput output;
    CSoundPlayer* GetPlayer() { return &player; }
    COutput* GetOutput() { return &output; }
};
struct CSoundManagerLH { CSoundSystem* m_pSystem; };
struct C3DVector { float X, Y, Z; };
struct Fixture {
    CSoundSystem system; CSoundManagerLH manager; SSoundNode head, n1, n2; CActiveSound s1, s2;
    Fixture() {
        manager.m_pSystem = &system;
        s1.m_handle = 11; s2.m_handle = 22;
        head.next = &n1; n1.next = &n2; n2.next = &head;
        n1.sound = &s1; n2.sound = &s2;
    }
};
struct CSoundEmitterLH {
    CSoundManagerLH* m_pManager; C3DVector m_position; SSoundNode* m_pSounds; bool m_active; bool m_paused; bool m_finished;
    int calls15, calls10;
    CSoundEmitterLH() : m_pManager(0), m_pSounds(0), m_active(false), m_paused(false), m_finished(false), calls15(0), calls10(0) {}
    virtual ~CSoundEmitterLH() {}
    virtual void vfunc_15() { ++calls15; }
    virtual void vfunc_10() { ++calls10; }
    bool vfunc_22(long handle) {
        if (m_pManager->m_pSystem)
            for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next)
                if (node->sound->m_handle == handle) return true;
        return false; }
};

int main()
{
    bool ok = false;
    {
    Fixture f; CSoundEmitterLH e; e.m_pManager = &f.manager; e.m_pSounds = &f.head;
    ok = e.vfunc_22(22) && e.vfunc_22(11) && !e.vfunc_22(33); f.manager.m_pSystem = 0; ok = ok && !e.vfunc_22(22);
    }
    if (!ok) { std::printf("00aabac0_TEST FAIL\n"); return 1; }
    std::printf("00aabac0_TEST PASS\n");
    return 0;
}