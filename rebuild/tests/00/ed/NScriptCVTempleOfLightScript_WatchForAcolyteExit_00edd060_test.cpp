// Behaviour test for 00edd060: src/00edd060.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CV_TempleOfLightScript::WatchForAcolyteExit()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
inline void* operator new(unsigned int, void* where) { return where; }
// ---- source (verbatim) ----
// CV_TempleOfLightScript::WatchForAcolyteExit() -- script thread: once the hero has attacked, collect the TOL_AcolyteGood things, wait until all are gone and set AllAcolytesEscaped
// Leaf: ?WatchForAcolyteExit@CV_TempleOfLightScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDD060, length 270 bytes, plain `ret`.
// Label: catalog "CQ_BountyHuntScript::WatchForAllGruntsKilled" is propagated; the body is started as the thread
// "WatchForAcolyteExit" (0x012F3E28) by CV_TempleOfLightScript::Main (0x00EDB190, functor store at 0x00EDB3AD).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, AreAllThingsInVectorDead (__fastcall, 0x00CBED00), vector<CScriptThing>::~vector
// (out-of-line COMDAT copy 0x008AC970, propagated label "~vector<CConversation>"), CScriptThing's virtual destructor
// (slot 0) and free 0x00BFEA14 in the inlined copy; through the game script interface (+0x40): NewScriptFrame
// (slot 7, +0x1c), GetAllThingsWithScriptName (slot 75, +0x12c). Literal "TOL_AcolyteGood" 0x012F3E54.
// Layout: Ego_r.pdb CV_TempleOfLightScript - 0x14: AttackedByHero +0x5b, AllAcolytesEscaped +0x5d.
// Shape: the inline-once destructor band: retail expands the local vector's destructor at the wait loop's terminating
// return and calls the out-of-line copy at the normal end (the two closing terminating tests jump to that call). The
// lane calls the out-of-line copy at every site, so the vector's destruction is written per exit: a __forceinline copy
// of the STLport ~vector body at the wait-loop return and the out-of-line ~vector (Destroy) once at the merged end.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

struct CCPPointerInfo;

struct CBaseClass {                                    // polymorphic root of the script handles
    CBaseClass();                                      // 0x0099A2D0
    virtual ~CBaseClass();                             // 0x0099A2E0
};

struct CScriptThing : CBaseClass {                     // 12 bytes, vtable 0x01238C8C (slot 0: virtual dtor)
    CScriptThing* PImp_Data;                           // +0x04
    CCPPointerInfo* PImp_Info;                         // +0x08
};

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct vector {                     // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void Destroy();                                    // ~vector, out of line: 0x008AC970
    __forceinline void DestroyInline()                 // the same body, expanded once (VC's inline-once band)
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
bool __fastcall AreAllThingsInVectorDead(vector<CScriptThing>* things);   // 0x00CBED00

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();                                       // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things); // slot 75 (+0x12c)
};

struct CScriptBase {
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                // 0x00CB7940
};

struct CV_TempleOfLightScript : CScriptBase {
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    long DonationTotal;                              // +0x48
    long DonationThisVisit;                          // +0x4c
    long DonationCounter;                            // +0x50
    bool DonationInvalid;                            // +0x54
    bool DonationActive;                             // +0x55
    bool DisplayQuestInfo;                           // +0x56
    bool WeaponWarningComplete;                      // +0x57
    bool WeaponUnsheathed;                           // +0x58
    bool TempleIntroComplete;                        // +0x59
    bool InsideTemple;                               // +0x5a
    bool AttackedByHero;                             // +0x5b
    bool AcolyteAttackMessageComplete;               // +0x5c
    bool AllAcolytesEscaped;                         // +0x5d
    void WatchForAcolyteExit();
};

void CV_TempleOfLightScript::WatchForAcolyteExit()
{
    while (!AttackedByHero) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    vector<CScriptThing> acolytes;
    m_pInterface->GetAllThingsWithScriptName("TOL_AcolyteGood", acolytes);
    for (;;) {
        if (IsActiveThreadTerminating()) {
            acolytes.DestroyInline();
            return;
        }
        if (AreAllThingsInVectorDead(&acolytes))
            break;
        m_pInterface->NewScriptFrame();
    }
    if (!IsActiveThreadTerminating() && !IsActiveThreadTerminating())
        AllAcolytesEscaped = true;
    acolytes.Destroy();
}

// ---- stubs ----
void CGameScriptInterface::v000() { }
void CGameScriptInterface::v001() { }
void CGameScriptInterface::v002() { }
void CGameScriptInterface::v003() { }
void CGameScriptInterface::v004() { }
void CGameScriptInterface::v005() { }
void CGameScriptInterface::v006() { }
void CGameScriptInterface::v008() { }
void CGameScriptInterface::v009() { }
void CGameScriptInterface::v010() { }
void CGameScriptInterface::v011() { }
void CGameScriptInterface::v012() { }
void CGameScriptInterface::v013() { }
void CGameScriptInterface::v014() { }
void CGameScriptInterface::v015() { }
void CGameScriptInterface::v016() { }
void CGameScriptInterface::v017() { }
void CGameScriptInterface::v018() { }
void CGameScriptInterface::v019() { }
void CGameScriptInterface::v020() { }
void CGameScriptInterface::v021() { }
void CGameScriptInterface::v022() { }
void CGameScriptInterface::v023() { }
void CGameScriptInterface::v024() { }
void CGameScriptInterface::v025() { }
void CGameScriptInterface::v026() { }
void CGameScriptInterface::v027() { }
void CGameScriptInterface::v028() { }
void CGameScriptInterface::v029() { }
void CGameScriptInterface::v030() { }
void CGameScriptInterface::v031() { }
void CGameScriptInterface::v032() { }
void CGameScriptInterface::v033() { }
void CGameScriptInterface::v034() { }
void CGameScriptInterface::v035() { }
void CGameScriptInterface::v036() { }
void CGameScriptInterface::v037() { }
void CGameScriptInterface::v038() { }
void CGameScriptInterface::v039() { }
void CGameScriptInterface::v040() { }
void CGameScriptInterface::v041() { }
void CGameScriptInterface::v042() { }
void CGameScriptInterface::v043() { }
void CGameScriptInterface::v044() { }
void CGameScriptInterface::v045() { }
void CGameScriptInterface::v046() { }
void CGameScriptInterface::v047() { }
void CGameScriptInterface::v048() { }
void CGameScriptInterface::v049() { }
void CGameScriptInterface::v050() { }
void CGameScriptInterface::v051() { }
void CGameScriptInterface::v052() { }
void CGameScriptInterface::v053() { }
void CGameScriptInterface::v054() { }
void CGameScriptInterface::v055() { }
void CGameScriptInterface::v056() { }
void CGameScriptInterface::v057() { }
void CGameScriptInterface::v058() { }
void CGameScriptInterface::v059() { }
void CGameScriptInterface::v060() { }
void CGameScriptInterface::v061() { }
void CGameScriptInterface::v062() { }
void CGameScriptInterface::v063() { }
void CGameScriptInterface::v064() { }
void CGameScriptInterface::v065() { }
void CGameScriptInterface::v066() { }
void CGameScriptInterface::v067() { }
void CGameScriptInterface::v068() { }
void CGameScriptInterface::v069() { }
void CGameScriptInterface::v070() { }
void CGameScriptInterface::v071() { }
void CGameScriptInterface::v072() { }
void CGameScriptInterface::v073() { }
void CGameScriptInterface::v074() { }
static int g_term[16]; static int g_termN = 0, g_termCalls = 0;
bool CScriptBase::IsActiveThreadTerminating() { int i = g_termCalls++; return i < g_termN ? g_term[i] != 0 : false; }
static CV_TempleOfLightScript* g_script; static int g_frames = 0, g_attackAfter = 0;
void CGameScriptInterface::NewScriptFrame() { ++g_frames; if (g_frames >= g_attackAfter) g_script->AttackedByHero = true; }
static int g_getCalls = 0; static char g_name[32];
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; (void)length; }
CCharString::~CCharString() { }
long CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things)
{
    ++g_getCalls; strncpy(g_name, (const char*)name.PStringData, 31);
    CScriptThing* p = (CScriptThing*)malloc(2 * sizeof(CScriptThing));
    new (&p[0]) CScriptThing; new (&p[1]) CScriptThing;
    things._M_start = p; things._M_finish = p + 2; things._M_end_of_storage = p + 2;
    return 2;
}
static int g_deadCalls = 0, g_deadAfter = 0; static vector<CScriptThing>* g_deadArg;
bool __fastcall AreAllThingsInVectorDead(vector<CScriptThing>* things) { g_deadArg = things; return ++g_deadCalls >= g_deadAfter; }
static int g_ctors = 0, g_dtors = 0;
CBaseClass::CBaseClass() { ++g_ctors; }
CBaseClass::~CBaseClass() { ++g_dtors; }
static int g_destroyCalls = 0; static CScriptThing* g_destroyStart;
template <> void vector<CScriptThing>::Destroy() { ++g_destroyCalls; g_destroyStart = _M_start; }
void CScriptBase::v00() { }

static void Run(CV_TempleOfLightScript* s, int attackAfter, int deadAfter, const int* term, int termN)
{
    g_script = s; s->AttackedByHero = attackAfter == 0; s->AllAcolytesEscaped = false;
    g_frames = 0; g_attackAfter = attackAfter; g_deadCalls = 0; g_deadAfter = deadAfter;
    memcpy(g_term, term, termN * sizeof(int)); g_termN = termN; g_termCalls = 0;
    g_getCalls = 0; g_ctors = 0; g_dtors = 0; g_destroyCalls = 0; g_name[0] = 0;
    s->WatchForAcolyteExit();
}
int main()
{
    static unsigned char raw[0x60];
    memset(raw, 0, sizeof(raw));
    CV_TempleOfLightScript* s = (CV_TempleOfLightScript*)raw;
    CGameScriptInterface game;
    s->m_pInterface = &game;
    // A: terminating while waiting for the attack
    { int t[] = { 0, 1 }; Run(s, 5, 1, t, 2); }
    CHECK(g_frames == 2 && g_getCalls == 0 && g_termCalls == 2 && !s->AllAcolytesEscaped);
    // B: attacked after 3 frames, acolytes all gone after 2 checks, no termination: flag set, out-of-line ~vector once
    { int t[] = { 0 }; Run(s, 3, 2, t, 1); }
    CHECK(g_getCalls == 1 && strcmp(g_name, "TOL_AcolyteGood") == 0 && g_deadArg != 0);
    CHECK(g_deadCalls == 2 && g_frames == 4 && s->AllAcolytesEscaped);
    CHECK(g_destroyCalls == 1 && g_dtors == 0);
    // C: terminating inside the wait loop: the inline destructor runs both element dtors, no out-of-line call
    { int t[] = { 0, 0, 0, 1 }; Run(s, 0, 100, t, 4); }
    CHECK(g_getCalls == 1 && g_deadCalls == 2 && g_dtors == 2 && g_destroyCalls == 0 && !s->AllAcolytesEscaped);
    // D: terminating at the first closing test: out-of-line destructor, flag untouched
    { int t[] = { 0, 0, 1 }; Run(s, 0, 1, t, 3); }
    CHECK(g_destroyCalls == 1 && g_dtors == 0 && !s->AllAcolytesEscaped && g_termCalls == 3);
    // E: terminating at the second closing test
    { int t[] = { 0, 0, 0, 1 }; Run(s, 0, 1, t, 4); }
    CHECK(g_destroyCalls == 1 && !s->AllAcolytesEscaped && g_termCalls == 4);
    // F: attacked already but terminating before the search
    { int t[] = { 1 }; Run(s, 0, 1, t, 1); }
    CHECK(g_getCalls == 0 && g_destroyCalls == 0 && g_frames == 0);
    char out[64];
    _snprintf(out, sizeof(out), "00edd060_TEST %s", fails ? "FAIL" : "PASS");
    printf("%s\n", out);
    return fails ? 1 : 0;
}