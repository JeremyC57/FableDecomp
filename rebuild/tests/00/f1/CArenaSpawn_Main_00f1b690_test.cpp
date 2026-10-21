// Behaviour test for 00f1b690: src/00f1b690.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CArenaSpawn::Main()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CArenaSpawn::Main() -- arena spawn point: its data string is the spawn-slot index; while the script runs, wait for the parent script to flag that slot, play the SUMMON_ARENA effect at the spawn (unless the arena is in state 3), clear the flag and wait a frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B690, length 361 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract).
// Callees: CScriptThing slot 3 GetDataString (CCharString by hidden pointer) and slot 6 GetPos, GFCharStringToInt
//          @ 0x0099E7F0 (__fastcall), ~CCharString @ 0x0099EAE0, CCharString(const char*, long = -1) @ 0x0099EBF0,
//          ~CScriptThing @ 0x004AA840, CEntityScriptBase::IsDone @ 0x00F35B30, the game interface's slots 7 NewScriptFrame,
//          87 RegisterTimer, 88 DeregisterTimer, 100 CreateEffect (cgsi_slots_aligned.tsv); data g_pGameScriptInterface
//          @ 0x0143E8F8; literals "SUMMON_ARENA" 0x012F9D00 and "" 0x0122D70E.
// Shape: the timer handle is registered on the global interface; the inner wait loop's IsDone exit and the ArenaState
//          branch's IsDone exit both `goto stop` (one shared DeregisterTimer tail, retail copy 2 with eax), while the
//          after-wait IsDone check deregisters and returns inline and is tail-merged with the loop fallthrough (copy 1,
//          edx). Explicit returns at all three sites (or break at the middle one) put the inner exit into copy 1 or swap
//          the copies' registers; this goto/return mix is the only one of 36 exit-form combinations that matches. The slot
//          index stays in ebp, the timer in ebx with a frame spill across CreateEffect (which loads its vtable into ebx);
//          the CScriptThing result is a named local (destroyed after the two string temporaries).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; the parent script's arena state +0xa8
//          and per-slot spawn flags (bytes) at +0xe2 (descriptive names). engine/CScriptThing.h and
//          engine/CGameScriptInterface.h flatten the vptr into a data member, but this row calls their virtual slots
//          (GetDataString/GetPos, NewScriptFrame/RegisterTimer/DeregisterTimer/CreateEffect), so polymorphic local
//          declarations are used.
struct CCharString {                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
struct C3DVector { float x, y, z; };

struct CCPPointerInfo;                                   // CCountedPointer's shared block
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
};
struct CBaseClass {                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing();                                     // 0x004AA840
    virtual void v01() const; virtual void v02() const;
    virtual CCharString GetDataString() const;           // slot 3 (+0x0c)
    virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
};
long __fastcall GFCharStringToInt(const CCharString& text);                                                   // 0x0099E7F0
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099();
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& pos, const CCharString& param, float scale, bool a, bool b) const;   // slot 100 (+0x190)
};

extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

struct CQ_ArenaScript {
    unsigned char _pad[0xa8];
    long ArenaState;                                     // +0xa8
    unsigned char _pad2[0xe2 - 0xac];
    bool SpawnFlags[1];                                  // +0xe2 [slot]
};

struct CArenaSpawn {
    virtual ~CArenaSpawn();
    virtual void Main();                                 // slot 1
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CQ_ArenaScript* ParentClass;                         // +0x14
    bool IsDone();                                       // 0x00F35B30
};

void CArenaSpawn::Main()
{
    long slot = GFCharStringToInt(Me.GetDataString());
    long timer = g_pGameScriptInterface->RegisterTimer();
    while (!IsDone()) {
        while (!ParentClass->SpawnFlags[slot]) {
            Game->NewScriptFrame();
            if (IsDone()) goto stop;
        }
        if (IsDone()) {
            g_pGameScriptInterface->DeregisterTimer(timer);
            return;
        }
        if (ParentClass->ArenaState != 3) {
            if (IsDone()) goto stop;
            CScriptThing effect = Game->CreateEffect("SUMMON_ARENA", Me.GetPos(), "", 0.0f, false, false);
        }
        ParentClass->SpawnFlags[slot] = false;
        Game->NewScriptFrame();
    }
    g_pGameScriptInterface->DeregisterTimer(timer);
    return;
stop:
    g_pGameScriptInterface->DeregisterTimer(timer);
}

// ---- stubs and driver ----
#include <new>
#include <cstdlib>
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; }
static int g_strDtors = 0;
CCharString::~CCharString() { ++g_strDtors; }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
static int g_thingCopies = 0, g_thingDtors = 0;
CScriptThing::CScriptThing(const CScriptThing&) { ++g_thingCopies; PImp.Data = 0; PImp.Info = 0; }
CScriptThing::~CScriptThing() { ++g_thingDtors; }
void CScriptThing::v01() const { }
void CScriptThing::v02() const { }
void CScriptThing::v04() const { }
void CScriptThing::v05() const { }
static const char* g_dataString = "2";
CCharString CScriptThing::GetDataString() const { return CCharString(g_dataString); }
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
static int g_posCalls = 0;
const C3DVector& CScriptThing::GetPos() const { ++g_posCalls; return g_pos; }
static int g_toIntCalls = 0;
long __fastcall GFCharStringToInt(const CCharString& text) { ++g_toIntCalls; return atol((const char*)text.PStringData); }
#define D(n) void CGameScriptInterface::v##n() { }
D(000) D(001) D(002) D(003) D(004) D(005) D(006) D(008) D(009) D(010) D(011) D(012) D(013) D(014) D(015) D(016) D(017) D(018) D(019) D(020) D(021) D(022) D(023) D(024) D(025) D(026) D(027) D(028) D(029) D(030) D(031) D(032) D(033) D(034) D(035) D(036) D(037) D(038) D(039) D(040) D(041) D(042) D(043) D(044) D(045) D(046) D(047) D(048) D(049) D(050) D(051) D(052) D(053) D(054) D(055) D(056) D(057) D(058) D(059) D(060) D(061) D(062) D(063) D(064) D(065) D(066) D(067) D(068) D(069) D(070) D(071) D(072) D(073) D(074) D(075) D(076) D(077) D(078) D(079) D(080) D(081) D(082) D(083) D(084) D(085) D(086) D(089) D(090) D(091) D(092) D(093) D(094) D(095) D(096) D(097) D(098) D(099)
static CGameScriptInterface g_global, g_game;
CGameScriptInterface* g_pGameScriptInterface;
static CQ_ArenaScript* g_parent; static long g_slot = -1;
static int g_frames = 0, g_flagAtFrame = -1;
void CGameScriptInterface::NewScriptFrame() { CHECK(this == &g_game); ++g_frames; if (g_frames == g_flagAtFrame) g_parent->SpawnFlags[g_slot] = true; }
static int g_registers = 0, g_deregisters = 0; static long g_deregTimer = 0;
long CGameScriptInterface::RegisterTimer() { CHECK(this == &g_global); ++g_registers; return 4711; }
void CGameScriptInterface::DeregisterTimer(long timer) { CHECK(this == &g_global); ++g_deregisters; g_deregTimer = timer; }
static unsigned char g_protoRaw[sizeof(CScriptThing)];
static int g_effects = 0; static const char* g_effName; static const char* g_effParam; static const C3DVector* g_effPos;
static float g_effScale = -1.0f; static bool g_effA = true, g_effB = true;
CScriptThing CGameScriptInterface::CreateEffect(const CCharString& name, const C3DVector& pos, const CCharString& param, float scale, bool a, bool b) const
{
    CHECK(this == &g_game); ++g_effects; g_effName = (const char*)name.PStringData; g_effPos = &pos;
    g_effParam = (const char*)param.PStringData; g_effScale = scale; g_effA = a; g_effB = b;
    return CScriptThing(*(const CScriptThing*)g_protoRaw);
}
CArenaSpawn::~CArenaSpawn() { }
static CArenaSpawn* g_spawn;
static const char* g_doneSeq; static int g_doneIdx = 0;
bool CArenaSpawn::IsDone() { CHECK(this == g_spawn); char c = g_doneSeq[g_doneIdx]; if (c == 0) return true; ++g_doneIdx; return c == 'T'; }
static unsigned char g_parentMem[0x100];
static long g_spawnRaw[8];
static unsigned char g_spawnBuf[sizeof(CArenaSpawn)];
static void Reset(const char* seq, long state, int flagAtFrame)
{
    for (int i = 0; i < 0x100; ++i) g_parentMem[i] = 0;
    g_parent = (CQ_ArenaScript*)g_parentMem; g_parent->ArenaState = state; g_slot = 2;
    g_frames = 0; g_flagAtFrame = flagAtFrame; g_registers = g_deregisters = 0; g_deregTimer = 0; g_effects = 0;
    g_effName = g_effParam = 0; g_effPos = 0; g_effScale = -1.0f; g_effA = g_effB = true;
    g_doneSeq = seq; g_doneIdx = 0; g_thingCopies = g_thingDtors = 0; g_strDtors = 0; g_posCalls = 0; g_toIntCalls = 0;
}
static void Run()
{
    g_spawn->Main();
}
int main()
{
    g_pGameScriptInterface = &g_global;
    g_spawnRaw[1] = (long)&g_game; g_spawnRaw[5] = (long)g_parentMem;
    g_spawn = new (g_spawnBuf) CArenaSpawn(*(const CArenaSpawn*)g_spawnRaw);
    CHECK(g_spawn->Game == &g_game && g_spawn->ParentClass == (CQ_ArenaScript*)g_parentMem);
    // 1) full pass: wait two frames for the flag, play the effect, clear the flag, one more frame, then done
    Reset("FFFFF" "T", 1, 2); Run();
    CHECK(g_toIntCalls == 1 && g_registers == 1 && g_deregisters == 1 && g_deregTimer == 4711);
    CHECK(g_frames == 3 && g_doneIdx == 6 && g_parent->SpawnFlags[2] == false);
    CHECK(g_effects == 1 && g_effName && strcmp(g_effName, "SUMMON_ARENA") == 0 && g_effParam && strcmp(g_effParam, "") == 0);
    CHECK(g_effPos == &g_pos && g_posCalls == 1 && g_effScale == 0.0f && !g_effA && !g_effB);
    CHECK(g_thingCopies == 1 && g_thingDtors == 1 && g_strDtors == 3);
    // 2) done while waiting for the flag: deregister, no effect
    Reset("FFT", 1, 99); Run();
    CHECK(g_registers == 1 && g_deregisters == 1 && g_deregTimer == 4711 && g_frames == 2 && g_effects == 0 && g_doneIdx == 3);
    // 3) done right after the wait
    Reset("FFT", 1, 1); Run();
    CHECK(g_deregisters == 1 && g_frames == 1 && g_effects == 0 && g_doneIdx == 3 && g_parent->SpawnFlags[2] == true);
    // 4) done inside the effect branch
    Reset("FFFT", 1, 1); Run();
    CHECK(g_deregisters == 1 && g_frames == 1 && g_effects == 0 && g_doneIdx == 4 && g_parent->SpawnFlags[2] == true);
    // 5) arena state 3: no effect and no extra IsDone; flag cleared, two rounds
    Reset("FFFFT", 3, 1); g_parentMem[0xe2 + 2] = 1; Run();
    CHECK(g_deregisters == 1 && g_effects == 0 && g_posCalls == 0 && g_frames == 2 && g_doneIdx == 5 && g_parent->SpawnFlags[2] == false);
    // 6) done at the top: register then deregister immediately
    Reset("T", 1, 1); Run();
    CHECK(g_registers == 1 && g_deregisters == 1 && g_frames == 0 && g_doneIdx == 1 && g_toIntCalls == 1 && g_strDtors == 1);
    if (fails == 0) printf("00f1b690_TEST PASS\n");
    return fails;
}
