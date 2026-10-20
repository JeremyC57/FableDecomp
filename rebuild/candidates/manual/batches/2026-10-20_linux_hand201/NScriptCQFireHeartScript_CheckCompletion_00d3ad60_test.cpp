// Behaviour test for 00d3ad60: the model below is a copy of src/00d3ad60.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __thiscall
#define __forceinline inline
#define __int64 long long
#endif
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};

#ifdef _MSC_VER
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
#endif

struct CSpawnedFuncBase {                               // retail 0x34 bytes (CScriptProcessBase + thread state)
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};

template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};

struct CQ_FireHeartScript : CScriptBase {
    void* GSI;                                          // +0x40
    void* PMasterData;                                  // +0x44
    bool  QuestStartScreenShown;                        // +0x48
    bool  MissionSucceeded;                             // +0x49
    bool  MissionFailed;                                // +0x4a
    unsigned char _maps04c[0x18];                       // +0x4c .. +0x64 (CutsceneFlags, csargs: 12-byte maps)
    long  FreedProphets;                                // +0x64
    long  KilledProphets;                               // +0x68
    long  StartNumberOfSuns;                            // +0x6c
    long  NumberOfRounds;                               // +0x70
    long  NumberOfPatterns;                             // +0x74
    long  NumberOfProphets;                             // +0x78
    void CheckCompletion();
    void PlayCutscene(CCharString name);                // 0x00D39330
    void StartRound();                                  // 0x00D39960
};

void CQ_FireHeartScript::CheckCompletion()
{
    if (FreedProphets + KilledProphets == NumberOfProphets) {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene(CCharString("CS_FIREHEART_OUTRO", -1));
        MissionSucceeded = true;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc0<CQ_FireHeartScript>("StartRound", 0, this, &CQ_FireHeartScript::StartRound),
                       CCharString("", -1));
}
}

// ---- test doubles and driver ----
static int g_termCalls = 0, g_termAt = 1000, g_ctors = 0, g_dtors = 0, g_cutscenes = 0, g_rounds = 0, g_added = 0;
static int g_threadsAlive = 0, g_bad = 0;
static char g_cutscene[32], g_threadName[32];
CCharString::CCharString(const char* text, long length) { ++g_ctors; PStringData = (void*)text; if (length != -1) g_bad = 1; }
CCharString::CCharString(const CCharString& other) { ++g_ctors; PStringData = other.PStringData; }
CCharString::~CCharString() { ++g_dtors; }
CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id)
{
    ++g_threadsAlive; std::strcpy(g_threadName, (const char*)name.PStringData); if (id != 0) g_bad = 1;
}
CSpawnedFuncBase::~CSpawnedFuncBase() { --g_threadsAlive; }
void CSpawnedFuncBase::RunSuspendableProcess() {}
void CSpawnedFuncBase::SuspendSuspendableProcess(bool) {}
void CSpawnedFuncBase::Restart() {}
bool NScript::CScriptBase::IsActiveThreadTerminating() { ++g_termCalls; return g_termCalls > g_termAt; }
void NScript::CScriptBase::AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName)
{
    ++g_added;
    if (std::strcmp((const char*)regionName.PStringData, "") != 0) g_bad = 1;
    func->SuspendableProcess();     // runs the bound member: StartRound on the owning script
    delete func;
}
void NScript::CQ_FireHeartScript::PlayCutscene(CCharString name) { ++g_cutscenes; std::strcpy(g_cutscene, (const char*)name.PStringData); }
static NScript::CQ_FireHeartScript* g_owner = 0;
void NScript::CQ_FireHeartScript::StartRound() { if (this == g_owner) ++g_rounds; else g_bad = 1; }

static bool Run(long freed, long killed, long prophets, int termAt, int expectCut, int expectRounds, bool expectSucceeded)
{
    NScript::CQ_FireHeartScript s;
    s.GSI = 0; s.PMasterData = 0; s.QuestStartScreenShown = s.MissionSucceeded = s.MissionFailed = false;
    s.FreedProphets = freed; s.KilledProphets = killed; s.NumberOfProphets = prophets;
    g_owner = &s; g_termCalls = 0; g_termAt = termAt; g_ctors = g_dtors = 0; g_cutscenes = g_rounds = g_added = 0;
    g_cutscene[0] = g_threadName[0] = 0;
    s.CheckCompletion();
    if (g_cutscenes != expectCut || g_rounds != expectRounds || s.MissionSucceeded != expectSucceeded) return false;
    if (expectCut && std::strcmp(g_cutscene, "CS_FIREHEART_OUTRO") != 0) return false;
    if (expectRounds && (std::strcmp(g_threadName, "StartRound") != 0 || g_added != 1)) return false;
    return g_ctors == g_dtors && g_threadsAlive == 0 && !g_bad;
}

int main()
{
    bool ok = true;
    ok = ok && Run(3, 2, 5, 1000, 1, 0, true);     // all prophets dealt with: outro, mission succeeded
    ok = ok && Run(3, 1, 5, 1000, 0, 1, false);    // one left: next round thread spawned
    ok = ok && Run(5, 0, 5, 0, 0, 0, false);       // terminating: nothing
    ok = ok && Run(0, 0, 5, 0, 0, 0, false);       // terminating: no thread
    if (!ok) { std::printf("00d3ad60_TEST FAIL\n"); return 1; }
    std::printf("00d3ad60_TEST PASS\n");
    return 0;
}
