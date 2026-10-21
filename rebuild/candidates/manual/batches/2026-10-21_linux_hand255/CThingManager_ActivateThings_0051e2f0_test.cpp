#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingManager::ActivateThings(vector<CThing*>& things) -- bring freshly created things through their three initialisation stages (stage 0: virtual slot 8, stage 1: OnCreate slot 9, stage 2: Activate slot 10) with a progress line before each thing; the hero is skipped while the game component's flag at +0x101 is set, and the stage-2 hero is activated last (when alive)
// Leaf: ?ActivateThings@CThingManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E2F0, length 395 bytes (+5 int3), `ret 4`.
//   Name and argument from the catalog / engine_api (`CThingManager::ActivateThings(vector<CThing*>*)`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0, DisplayProgress(const CCharString&, float, bool, bool)
//   @ 0x009E9F40 (`__fastcall`), ~CCharString @ 0x0099EAE0; the things' virtual slots 8/9/10 (+0x20/+0x24/+0x28).
//   Strings "Activate Things" @ 0x01244E24, "Activate Things: OnCreate" @ 0x01244E08, "Activate Things: Initial
//   Activate" @ 0x01244DE4, "Activate Things End" @ 0x01244DD0.
// Shape: each progress line is an inline helper with a named CCharString local (retail builds the string before the
//   float/bool pushes and re-leas its address for the call -- the 0x0051E2F0 "named local in an inline" idiom).
// Layout (retail): CThing ThingType byte +0x10, Dead bit 0 of +0x91, InitialisationStage bits 0..2 of +0x94 (Ego_r.pdb
//   +0xa1/+0xa4, CThing 0x10 lower); CThingManager +0x24 the main game component (Ego_r.pdb Component +0x28 -- the
//   base part is 4 smaller in retail; unproven), whose bool at +0x101 holds the hero back.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
__forceinline void ShowProgress(const char* message)
{
    CCharString text(message);
    DisplayProgress(text, -1.0f, false, false);
}

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Initialise();                          // slot 8
    virtual void OnCreate();                            // slot 9
    virtual void Activate();                            // slot 10
    unsigned char  _pad04[0x10 - 4];
    unsigned char  ThingType;                           // +0x10
    unsigned char  _pad11[0x91 - 0x11];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    unsigned char  _bits91 : 7;
    unsigned char  _pad92[2];
    unsigned char  InitialisationStage : 3;             // +0x94 bits 0..2
    unsigned char  _bits94 : 5;
};

template<class T> struct vector {
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    T* begin() { return _Myfirst; }
    T* end() { return _Mylast; }
};

struct CMainGameComponent {
    unsigned char _pad00[0x101];
    bool          HoldHero;                             // +0x101
};

struct CThingManager {
    unsigned char       _pad00[0x24];
    CMainGameComponent* Component;                      // +0x24
    void ActivateThings(vector<CThing*>& things);
};

void CThingManager::ActivateThings(vector<CThing*>& things)
{
    CThing** it;
    for (it = things.begin(); it != things.end(); ++it) {
        ShowProgress("Activate Things");
        CThing* thing = *it;
        if (thing->InitialisationStage == 0 && (thing->ThingType != 2 || !Component->HoldHero))
            thing->Initialise();
    }
    for (it = things.begin(); it != things.end(); ++it) {
        ShowProgress("Activate Things: OnCreate");
        CThing* thing = *it;
        if (thing->InitialisationStage == 1 && (thing->ThingType != 2 || !Component->HoldHero))
            thing->OnCreate();
    }
    CThing* hero = 0;
    for (it = things.begin(); it != things.end(); ++it) {
        ShowProgress("Activate Things: Initial Activate");
        CThing* thing = *it;
        if (thing->InitialisationStage == 2) {
            if (thing->ThingType == 2)
                hero = thing;
            else
                thing->Activate();
        }
    }
    if (hero && !hero->Dead && !Component->HoldHero)
        hero->Activate();
    ShowProgress("Activate Things End");
}

void CThing::v00() {}
void CThing::v01() {}
void CThing::v02() {}
void CThing::v03() {}
void CThing::v04() {}
void CThing::v05() {}
void CThing::v06() {}
void CThing::v07() {}
static int g_lines = 0, g_strings = 0;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
static const char* g_last = 0;
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{ ++g_lines; g_last = (const char*)text.PStringData; if (progress != -1.0f || a || b) g_lines = -1000; }
static char g_log[64]; static int g_n = 0;
struct CTestThing : CThing { char id; };
void CThing::Initialise() { g_log[g_n++] = 'i'; g_log[g_n++] = ((CTestThing*)this)->id; InitialisationStage = 1; }
void CThing::OnCreate() { g_log[g_n++] = 'c'; g_log[g_n++] = ((CTestThing*)this)->id; InitialisationStage = 2; }
void CThing::Activate() { g_log[g_n++] = 'a'; g_log[g_n++] = ((CTestThing*)this)->id; }
#define CHECK(c) if (!(c)) { std::printf("0051e2f0_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static CTestThing a, b, hero, deadHero; a.id = 'A'; b.id = 'B'; hero.id = 'H'; deadHero.id = 'D';
    a.ThingType = 1; b.ThingType = 3; hero.ThingType = 2; b.InitialisationStage = 1;
    static CThing* list[3] = { &a, &b, &hero };
    vector<CThing*> v; v._Myfirst = list; v._Mylast = list + 3; v._Myend = list + 3;
    static CMainGameComponent comp; static CThingManager mgr; mgr.Component = &comp;
    mgr.ActivateThings(v);
    CHECK(std::strcmp(g_log, "iAiHcAcBcHaAaBaH") == 0);
    CHECK(g_lines == 10 && g_strings == 0 && std::strcmp(g_last, "Activate Things End") == 0);
    g_n = 0; std::memset(g_log, 0, sizeof g_log); g_lines = 0;
    a.InitialisationStage = 0; b.InitialisationStage = 2; hero.InitialisationStage = 0; comp.HoldHero = true;
    mgr.ActivateThings(v);                                   // hero held back at every stage
    CHECK(std::strcmp(g_log, "iAcAaAaB") == 0 && g_lines == 10);
    g_n = 0; std::memset(g_log, 0, sizeof g_log);
    comp.HoldHero = false; hero.InitialisationStage = 2; hero.Dead = 1; a.InitialisationStage = 3; b.InitialisationStage = 3;
    mgr.ActivateThings(v);                                   // dead stage-2 hero is not activated
    CHECK(g_n == 0);
    hero.Dead = 0; v._Mylast = list;
    mgr.ActivateThings(v);                                   // empty list: only the end line
    CHECK(g_n == 0 && std::strcmp(g_last, "Activate Things End") == 0);
    std::printf("0051e2f0_TEST PASS\n");
    return 0;
}
