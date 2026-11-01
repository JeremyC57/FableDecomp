// Behaviour test for 0x01219F00 `dynamic initializer for 'GGame'`: before main the global CGame is built by
// CInitBaseClass's constructor (Valid cleared, instance count incremented) and gets CGame's dynamic type; its destructor
// (CGame's implicit one = ~CInitBaseClass) is registered to run at exit.
#include <cstdio>
#include <cstdlib>
static long g_initBaseClassCount = 0;
class CBaseClass { public: virtual ~CBaseClass() { } };
class CInitBaseClass : public CBaseClass { public:
    CInitBaseClass() : Valid(false) { ++g_initBaseClassCount; }
    virtual ~CInitBaseClass() { if (Valid) Valid = false; }
    virtual int Kind() const { return 1; }
    bool Valid;
};
class CGameComponent;
class CGame : public CInitBaseClass { public:
    CGame() { }
    virtual int Kind() const { return 2; }
    CGameComponent* CurrentGameComponent;
    unsigned char ParameterBuffer[512];
    bool Quit;
};
CGame GGame;
int main()
{
    if (g_initBaseClassCount != 1 || GGame.Valid || GGame.Kind() != 2) { std::printf("01219f00_TEST FAIL\n"); return 1; }
    std::printf("01219f00_TEST PASS\n");
    return 0;
}