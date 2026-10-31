// Behaviour test for 0x00C11D00 CSoundEventManager::CSoundEventManager(CAudioOwner*, float, float, float, float, float,
// CSteeringWorldEntry*): the same member initialisation (listener frame 0/front/up, event set from the three floats, fader
// step 1000 / owner rate, cleared state block, flags, 500 default) and the four heap components wired to the manager,
// with stub components that record what they were built with.
#include <cstdio>
#include <cstring>

struct CVec3 {
    float X, Y, Z;
    CVec3(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CSteeringWorldEntry { int Id; };
struct CAudioOwner { float Rate; };
struct CSoundEventSet {
    float A, B, C;
    CSoundEventSet(float a, float b, float c) : A(a), B(b), C(c) { }
};
struct CFader {
    float Step;
    CFader(float timeStep) : Step(timeStep) { }
};
struct CHelper { void* Owner; CHelper(void* owner) : Owner(owner) { } };
struct CAudioResources {
    void* Owner;
    void* Stream;
    CAudioResources(void* owner, void* stream) : Owner(owner), Stream(stream) { }
};
struct SThreadParams { unsigned long m_stackSize; long m_priority; };
struct CTimerHost {
    float Rate;
    long Arg;
    void* Target;
    SThreadParams Params;
    CTimerHost(float rate, long arg, void* target, const SThreadParams* params)
        : Rate(rate), Arg(arg), Target(target), Params(*params) { }
};

struct CSoundEventManager {
    CAudioOwner*  Owner;
    CVec3         Position;
    CVec3         Front;
    CVec3         Up;
    CSoundEventSet Event;
    CHelper*      HelperA;
    CHelper*      HelperB;
    CAudioResources* Resources;
    float         Gain;
    CTimerHost*   Timer;
    long          ValueC8;
    long          ValueCC;
    float         Width;
    float         Height;
    CFader        Timing;
    bool          Flag828;
    long          State[21];
    long          Value880;
    bool          Flag884;
    bool          Flag885;
    long          Value888;
    long          Value88C;
    CSoundEventManager(CAudioOwner* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry)
        : Owner(g), Position(0.0f, 0.0f, 0.0f), Front(0.0f, 1.0f, 0.0f), Up(0.0f, 0.0f, 1.0f), Event(a4, a0, a8),
          HelperA(0), HelperB(0), Gain(1.0f), Timer(0), ValueC8(0), ValueCC(0), Width(w), Height(h), Timing(1000.0f / g->Rate)
    {
        Flag828 = false;
        std::memset(State, 0, sizeof(State));
        Value880 = 0;
        Flag884 = false;
        Flag885 = true;
        Value888 = 500;
        Value88C = 0;
        HelperA = new CHelper(this);
        HelperB = new CHelper(this);
        Resources = new CAudioResources(this, entry);
        SThreadParams params;
        params.m_stackSize = 0x3000;
        params.m_priority = 0xf;
        Timer = new CTimerHost(50.0f, 0, Resources, &params);
    }
};

int main()
{
    CAudioOwner owner;
    owner.Rate = 50.0f;
    CSteeringWorldEntry entry;
    entry.Id = 7;
    CSoundEventManager m(&owner, 1.5f, 2.5f, 3.5f, 640.0f, 480.0f, &entry);
    bool ok = m.Owner == &owner;
    ok = ok && m.Position.X == 0.0f && m.Position.Y == 0.0f && m.Position.Z == 0.0f;
    ok = ok && m.Front.X == 0.0f && m.Front.Y == 1.0f && m.Front.Z == 0.0f;
    ok = ok && m.Up.X == 0.0f && m.Up.Y == 0.0f && m.Up.Z == 1.0f;
    ok = ok && m.Event.A == 1.5f && m.Event.B == 2.5f && m.Event.C == 3.5f;
    ok = ok && m.Gain == 1.0f && m.Width == 640.0f && m.Height == 480.0f && m.Timing.Step == 20.0f;
    ok = ok && m.ValueC8 == 0 && m.ValueCC == 0 && !m.Flag828 && m.Value880 == 0 && !m.Flag884 && m.Flag885;
    ok = ok && m.Value888 == 500 && m.Value88C == 0;
    for (int i = 0; i < 21; ++i)
        ok = ok && m.State[i] == 0;
    ok = ok && m.HelperA && m.HelperA->Owner == &m && m.HelperB && m.HelperB->Owner == &m && m.HelperA != m.HelperB;
    ok = ok && m.Resources && m.Resources->Owner == &m && m.Resources->Stream == &entry;
    ok = ok && m.Timer && m.Timer->Rate == 50.0f && m.Timer->Arg == 0 && m.Timer->Target == m.Resources;
    ok = ok && m.Timer->Params.m_stackSize == 0x3000 && m.Timer->Params.m_priority == 0xf;
    delete m.Timer;
    delete m.Resources;
    delete m.HelperB;
    delete m.HelperA;
    if (!ok) {
        std::printf("00c11d00_TEST FAIL\n");
        return 1;
    }
    std::printf("00c11d00_TEST PASS\n");
    return 0;
}