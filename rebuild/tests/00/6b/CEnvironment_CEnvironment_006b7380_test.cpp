// Behaviour test for 0x006B7380 CEnvironment::~CEnvironment (model copy): members are destroyed in reverse declaration
// order -- particle pointer, interpolator (null-tested delete), interface, pallette, draw params, the two tick states
// (each blended theme's texture sets, last element first), priority stacks -- and the CBase part runs last.
#include <cstdio>

static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CBaseClassNonCopyable {
public:  virtual ~CBaseClassNonCopyable() { Log('B'); } };
struct Particle { ~Particle() { Log('P'); } };
struct Interpolator { ~Interpolator() { Log('I'); } };
struct Interface { int x; };
struct Vec { char tag; ~Vec() { Log(tag); } };
struct TextureSet { ~TextureSet() { Log('t'); } };
struct Blended { TextureSet TextureSets[3]; };
struct TickState { Blended BlendedTheme[6]; };
template <class T> struct scoped_ptr { T* ptr; ~scoped_ptr() { delete ptr; } };

struct Env : CBaseClassNonCopyable {
    Vec PriorityStackChannels;
    TickState GameTickState;
    TickState PrevGameTickState;
    Vec ThemeChannels;
    Vec DaySets;
    scoped_ptr<Interface> SpellEffectInterface;
    scoped_ptr<Interpolator> SpellEffectInterpolator;
    Particle PRainSplashParticle;
    Env() { PriorityStackChannels.tag = 'S'; ThemeChannels.tag = 'C'; DaySets.tag = 'D'; }
    ~Env() { }
};

int main()
{
    {
        Env* e = new Env;
        e->SpellEffectInterface.ptr = new Interface;
        e->SpellEffectInterpolator.ptr = new Interpolator;
        delete e;
    }
    const char* want = "PIDCttttttttttttttttttttttttttttttttttttSB";
    int ok = 1;
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { ok = 0; break; }
    g_n = 0; g_log[0] = 0;
    {
        Env* e = new Env;                                             // null scoped pointers: no delete side effects
        e->SpellEffectInterface.ptr = 0;
        e->SpellEffectInterpolator.ptr = 0;
        delete e;
    }
    ok = ok && g_log[0] == 'P' && g_log[1] == 'D' && g_n == 41;
    if (!ok) { std::printf("006b7380_TEST FAIL\n"); return 1; }
    std::printf("006b7380_TEST PASS\n");
    return 0;
}