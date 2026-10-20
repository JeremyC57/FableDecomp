// Behaviour test for 00633cb0: the model below is a copy of src/00633cb0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
typedef long EPlayerMode;
typedef long EProcessedEventType;
typedef long EGameEventPriority;

struct CGameEvent {                                     // 0x28 bytes
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent() : Player(-1), Valid(false) { }
};

struct CProcessedInput {                                // 0xb0 bytes
    long                Player;                         // +0x00
    EProcessedEventType Type;                           // +0x04
    CGameEvent          GameEvents[4];                  // +0x08
    unsigned char       NoGameEvents;                   // +0xa8
    EGameEventPriority  Priority;                       // +0xac
    CProcessedInput() { Clear(); }
    void Clear();                                       // 0x00A0D300
    void SetPlayer(long player);                        // 0x00A0D3A0
};

struct CInputProcessManager {
    void AddQueuedProcessedInput(CProcessedInput* input);   // 0x00A0D460
};
struct CGamePlayerInterface : CInputProcessManager { };

#pragma pointers_to_members(full_generality, multiple_inheritance)
struct CPlayer;
typedef void (CPlayer::*TModeFunc)(EPlayerMode, CProcessedInput*);   // {function, this delta}
struct CPlayerModeInfo {                                // 0x20 bytes
    TModeFunc     InitFunc;                             // +0x00
    unsigned char _pad08[8];
    TModeFunc     UninitFunc;                           // +0x10
    unsigned char _pad18[8];
};

struct SModeListNode {                                  // list<EPlayerMode> node
    SModeListNode* Next;
    SModeListNode* Prev;
    EPlayerMode    Value;
};

struct CPlayer {
    void*                 __vftable;
    unsigned char         _pad04[0x0c - 0x04];
    CGamePlayerInterface* PlayerInterface;              // +0x0c
    unsigned char         _pad10[0x28 - 0x10];
    long                  Number;                       // +0x28
    unsigned char         _pad2c[0x210 - 0x2c];
    SModeListNode*        PlayerModes;                  // +0x210 list<EPlayerMode> (sentinel)
    bool                  SuppressFullModeRemoval;      // +0x214
    bool                  DisallowModeChanges;          // +0x215
    static CPlayerModeInfo PlayerModeInfo[];            // 0x013B8C88
    EPlayerMode GetCurrentMode() const { return PlayerModes->Prev->Value; }
    void InitActiveMode(EPlayerMode mode);
};

void CPlayer::InitActiveMode(EPlayerMode mode)
{
    DisallowModeChanges = true;
    EPlayerMode current = GetCurrentMode();
    if (PlayerModeInfo[current].InitFunc) {
        CProcessedInput input;
        input.SetPlayer(Number);
        (this->*PlayerModeInfo[current].InitFunc)(mode, &input);
        if (input.Type != 0)
            PlayerInterface->AddQueuedProcessedInput(&input);
    }
    DisallowModeChanges = false;
}

// ---- test doubles and driver ----
static int g_clears = 0; static int g_queued = 0; static int g_calls = 0; static CProcessedInput* g_lastQueued = 0;
static EPlayerMode g_seenMode = -1; static long g_seenPlayer = -1; static int g_lockSeen = -1; static int g_otherCalls = 0;
void CProcessedInput::Clear() { ++g_clears; Player = -1; Type = 0; NoGameEvents = 0; Priority = 2; }
void CProcessedInput::SetPlayer(long p) { Player = p; }
void CInputProcessManager::AddQueuedProcessedInput(CProcessedInput* in) { ++g_queued; g_lastQueued = in; }
struct CTestPlayer : CPlayer {
    long Marker;
    void Handler(EPlayerMode m, CProcessedInput* in)
    {
        ++g_calls; g_seenMode = m; g_seenPlayer = in->Player;
#if 1
        g_lockSeen = DisallowModeChanges ? 1 : 0;
#endif
        if (in->GameEvents[0].Player != -1 || in->GameEvents[3].Valid) g_seenPlayer = -99;
        if (m == 5) in->Type = 3;     // produce an event
    }
    void Other(EPlayerMode, CProcessedInput*) { ++g_otherCalls; }
};
CPlayerModeInfo CPlayer::PlayerModeInfo[4];
int main()
{
    static CGamePlayerInterface gpi;
    static CTestPlayer p; p.PlayerInterface = &gpi; p.Number = 1;
    static SModeListNode sentinel, node;
    sentinel.Next = &node; sentinel.Prev = &node; node.Next = &sentinel; node.Prev = &sentinel; node.Value = 2;
    p.PlayerModes = &sentinel;
    CPlayer::PlayerModeInfo[2].InitFunc = static_cast<TModeFunc>(&CTestPlayer::Handler);
    CPlayer::PlayerModeInfo[2].UninitFunc = static_cast<TModeFunc>(&CTestPlayer::Other);
    bool ok = true;
    p.InitActiveMode(4);                                   // handler runs, no event -> nothing queued
    if (g_calls != 1 || g_seenMode != 4 || g_seenPlayer != 1 || g_clears != 1 || g_queued != 0 || g_otherCalls) ok = false;
    p.InitActiveMode(5);                                   // handler produces an event -> queued
    if (g_calls != 2 || g_queued != 1 || g_lastQueued == 0) ok = false;
#if 1
    if (g_lockSeen != 1 || p.DisallowModeChanges) ok = false;
#endif
    node.Value = 1;                             // no handler for mode 1: no input built
    p.InitActiveMode(5);
    if (g_calls != 2 || g_clears != 2 || g_queued != 1) ok = false;
#if 1
    if (p.DisallowModeChanges) ok = false;
#endif
    if (!ok) { std::printf("00633cb0_TEST FAIL\n"); return 1; }
    std::printf("00633cb0_TEST PASS\n");
    return 0;
}