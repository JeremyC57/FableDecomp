// Behaviour test for CWorld::EASetFreeCameraMode @ 0x00629390: the event payload is copied into a fresh zeroed
// buffer, one bool is read, and the player named by the event's (signed) Player byte gets free-camera mode 0x13
// added (with false) when the flag is true, or removed otherwise. Checks both arms, the player-number sign extension,
// the buffer's initial state and the 1-byte read.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline bool ReadBool() { bool value; Read(1, &value); return value; }
};
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

enum EPlayerMode { PLAYER_MODE_FREE_CAMERA = 0x13 };
class CPlayer { public:
    void AddMode(EPlayerMode mode, bool b);        // 0x006345C0
    void RemoveMode(EPlayerMode mode);             // 0x00634190
};

class CWorld { public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EASetFreeCameraMode(const CGameEvent* event);
};

void CWorld::EASetFreeCameraMode(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    bool on = buffer.ReadBool();
    CPlayer* player = GetPlayer(event->Player);
    if (on == true)
        player->AddMode(PLAYER_MODE_FREE_CAMERA, false);
    else
        player->RemoveMode(PLAYER_MODE_FREE_CAMERA);
}

// ---- instrumented stubs: the event payload (Data[0..EndPos)) is copied into the buffer, Read consumes it ----
static int g_getData = 0, g_reads = 0;
static bool g_bufInitOk = false, g_readsInBuffer = true;
static unsigned long g_readSizes[8];
static const CGameEvent* g_expectEvent = 0;
static CMemoryBuffer* g_filled = 0;
long CGameEvent::GetData(CMemoryBuffer& buffer) const
{
    ++g_getData; if (this != g_expectEvent) g_fail = 1;
    g_bufInitOk = buffer.Length == 0 && buffer.ReadPos == 0 && buffer.WritePos == 0;
    std::memcpy(buffer.Data, Data, EndPos); buffer.Length = EndPos; g_filled = &buffer;
    return EndPos;
}
void CMemoryBuffer::Read(unsigned long length, void* data)
{
    if (g_reads < 8) g_readSizes[g_reads] = length;
    ++g_reads;
    if (this != g_filled || ReadPos + (long)length > Length) { g_readsInBuffer = false; return; }
    std::memcpy(data, Data + ReadPos, length); ReadPos += length;
}
static void resetEv() { g_getData = g_reads = 0; g_bufInitOk = false; g_readsInBuffer = true; g_filled = 0; std::memset(g_readSizes, 0, sizeof(g_readSizes)); }
static void makeEvent(CGameEvent& e, signed char player, const void* payload, unsigned char n)
{ std::memset(&e, 0, sizeof(e)); e.Type = 0x77; e.Player = player; std::memcpy(e.Data, payload, n); e.EndPos = n; e.Valid = true; }
static int g_getPlayer = 0, g_adds = 0, g_removes = 0;
static long g_playerArg = 0, g_modeArg = 0;
static bool g_addB = true;
static CPlayer g_player;
static CWorld* g_expectWorld = 0;
CPlayer* CWorld::GetPlayer(long number) { ++g_getPlayer; g_playerArg = number; if (this != g_expectWorld) g_fail = 1; return &g_player; }
void CPlayer::AddMode(EPlayerMode mode, bool b) { ++g_adds; g_modeArg = mode; g_addB = b; if (this != &g_player) g_fail = 1; }
void CPlayer::RemoveMode(EPlayerMode mode) { ++g_removes; g_modeArg = mode; if (this != &g_player) g_fail = 1; }
static void reset() { resetEv(); g_getPlayer = g_adds = g_removes = 0; g_playerArg = g_modeArg = 0; g_addB = true; }

int main()
{
    CWorld world; g_expectWorld = &world;
    CGameEvent ev; g_expectEvent = &ev;
    unsigned char on = 1, off = 0;

    reset(); makeEvent(ev, 2, &on, 1);
    world.EASetFreeCameraMode(&ev);
    CHECK(g_getData == 1 && g_bufInitOk && g_reads == 1 && g_readSizes[0] == 1 && g_readsInBuffer);
    CHECK(g_getPlayer == 1 && g_playerArg == 2 && g_adds == 1 && g_removes == 0 && g_modeArg == 0x13 && !g_addB);

    reset(); makeEvent(ev, 0, &off, 1);
    world.EASetFreeCameraMode(&ev);
    CHECK(g_getData == 1 && g_bufInitOk && g_reads == 1 && g_readsInBuffer);
    CHECK(g_getPlayer == 1 && g_playerArg == 0 && g_adds == 0 && g_removes == 1 && g_modeArg == 0x13);

    reset(); makeEvent(ev, -1, &on, 1);
    world.EASetFreeCameraMode(&ev);
    CHECK(g_playerArg == -1 && g_adds == 1 && g_removes == 0);

    if (g_fail) { std::printf("00629390_TEST FAIL\n"); return 1; }
    std::printf("00629390_TEST PASS\n");
    return 0;
}