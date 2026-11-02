// Behaviour test for 0x00ed39f0 NScript::CV_SingingStonesScript::~CV_SingingStonesScript() (implicit destructor): ListText[3..0],
// then each EffectList element's virtual destructor in order and the block freed, then ~CScriptBase.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static char g_log[64];
static int g_n = 0;
static int g_freed = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString {
public:
    CCharString() : Id('?') { }
    ~CCharString() { Log(Id); }
    char Id;
};
class CScriptThing {                                        // 12 bytes in retail: vptr + counted PImp
public:
    explicit CScriptThing(char id) : Id(id) { }
    virtual ~CScriptThing() { Log(Id); }
    char Id;
};
struct ThingVector {                                        // STLport vector<CScriptThing>
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    ThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~ThingVector()
    {
        for (CScriptThing* p = _M_start; p != _M_finish; ++p)
            p->~CScriptThing();                             // virtual: `push 0 ; call [eax]`
        if (_M_start != 0) { std::free(_M_start); ++g_freed; }
    }
};
class CScriptBase { public: virtual ~CScriptBase() { Log('B'); } };
class CLevelScriptBase : public CScriptBase { public: void* Game; void* MasterScriptData; };
class CV_SingingStonesScript : public CLevelScriptBase {
public:
    CV_SingingStonesScript() { for (int i = 0; i < 4; ++i) ListText[i].Id = (char)('0' + i); }
    long        CurrentPlayListIndex;
    long        PlayList[4];
    long        RudePlayList[4];
    long        YourPersonalPlayList[4];
    ThingVector EffectList;
    CCharString ListText[4];
    bool        DoorManFlags[4];
};

int main()
{
    CV_SingingStonesScript* s = new CV_SingingStonesScript;
    s->EffectList._M_start = (CScriptThing*)std::malloc(3 * sizeof(CScriptThing));
    new (&s->EffectList._M_start[0]) CScriptThing('a');
    new (&s->EffectList._M_start[1]) CScriptThing('b');
    s->EffectList._M_finish = s->EffectList._M_start + 2;
    s->EffectList._M_end_of_storage = s->EffectList._M_start + 3;
    delete (CScriptBase*)s;
    if (std::strcmp(g_log, "3210abB") != 0 || g_freed != 1) {
        std::printf("00ed39f0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00ed39f0_TEST PASS\n");
    return 0;
}