#include <cstdio>
// Behaviour test for CTCEffectOnDie scalar deleting destructor (0x007E0DB0): the death-sound string dies
// before the CTCBase base, once per delete.
static int g_order[8]; static int g_n = 0;
class CTCBase { public: virtual ~CTCBase() { g_order[g_n++] = 1; } long ParentThing; bool ToBeRemoved; };
class CCharString { public: void* m_pData; ~CCharString() { g_order[g_n++] = 2; } };
class CTCEffectOnDie : public CTCBase { public:
    bool StartedNonRecoverableFadeout; long Effect, Timer, FadeOutTimer;
    CCharString DeathSoundCriteria; bool Pausable, Paused;
    virtual ~CTCEffectOnDie() { }
};
int main()
{
    CTCBase* p = new CTCEffectOnDie;
    delete p;
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("007e0db0_TEST FAIL\n"); return 1; }
    std::printf("007e0db0_TEST PASS\n");
    return 0;
}