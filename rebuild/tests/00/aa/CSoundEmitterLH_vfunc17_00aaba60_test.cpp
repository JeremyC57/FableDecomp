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
    C3DVector vfunc_17() const { return m_position; }
};

int main()
{
    bool ok = false;
    {
    CSoundEmitterLH e; e.m_position.X = 1.5f; e.m_position.Y = -2.0f; e.m_position.Z = 3.25f; C3DVector v = e.vfunc_17();
    ok = v.X == 1.5f && v.Y == -2.0f && v.Z == 3.25f;
    }
    if (!ok) { std::printf("00aaba60_TEST FAIL\n"); return 1; }
    std::printf("00aaba60_TEST PASS\n");
    return 0;
}