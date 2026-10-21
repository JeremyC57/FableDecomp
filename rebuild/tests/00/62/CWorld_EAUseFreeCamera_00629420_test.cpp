// Behaviour test for 00629420: src/00629420.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CWorld::EAUseFreeCamera(const CGameEvent*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CWorld::EAUseFreeCamera(const CGameEvent*) -- game-event action: set the event player's UsingFreeCamera flag from the event payload
// Leaf: EAUseFreeCamera
// Flags: /GS
// Retail: Fable.exe @ 0x00629420, 111 bytes (/GS cookie 0x0139C8A8). PDB ?EAUseFreeCamera@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CWorld::GetPlayer(long) 0x0049D850,
// __security_check_cookie 0x00BFE9F9. CPlayer UsingFreeCamera +0x220 (PDB +0x228).
// Shape: the player number is read before the payload is copied; the flag is read through an inline bool reader
// (`bool value; Read(1, &value); return value == true;`, descriptive name ReadBool) whose byte VC7.1 homes in the dead
// `event` argument slot (frame 0x30 under /GS); the normalised result (sete) is stored back there across GetPlayer.
// Reading into a caller-side `bool use` and normalising with `use = (use == true)` gives it its own frame byte (0x34).
// CGameEvent (40 bytes, Ego_r.pdb = retail): +0x00 Type, +0x04 signed char Player, +0x05 Data[32], +0x25 EndPos,
// +0x26 Valid, +0x27 Replacement. Its payload is read through a 44-byte CMemoryBuffer local: Data[0x20], then the
// length +0x20 and two cursor words +0x24/+0x28 that the inline constructor zeroes; CGameEvent::GetData(CMemoryBuffer&)
// 0x009F17A0 (label "CEventPacket::CopyToBuffer") fills it and CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0
// (label "CircularBuffer_Read") consumes it.
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    bool ReadBool() { bool value; Read(1, &value); return value == true; }   // inline (descriptive name)
};
struct CGameEvent {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

struct CPlayer {
    unsigned char _pad[0x220];
    bool          UsingFreeCamera;                 // +0x220 (PDB +0x228)
};

struct CWorld {
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseFreeCamera(const CGameEvent* event);
};

void CWorld::EAUseFreeCamera(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    signed char player = event->Player;
    event->GetData(buffer);
    bool use = buffer.ReadBool();
    GetPlayer(player)->UsingFreeCamera = use;
}

// ---- stubs and driver ----
static int g_getDataCalls = 0; static const CGameEvent* g_getDataEvent;
long CGameEvent::GetData(CMemoryBuffer& buffer) const
{
    ++g_getDataCalls; g_getDataEvent = this;
    for (int i = 0; i < 32; ++i) buffer.Data[i] = Data[i];
    buffer.Length = 32; buffer.ReadPos = 0;
    return 32;
}
static int g_reads = 0; static unsigned long g_readLen = 0;
void CMemoryBuffer::Read(unsigned long length, void* data)
{
    ++g_reads; g_readLen = length;
    unsigned char* out = (unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i) out[i] = Data[ReadPos + i];
    ReadPos += length;
}
static CPlayer g_players[4]; static long g_playerArg = -99; static CWorld* g_worldThis;
CPlayer* CWorld::GetPlayer(long number) { g_playerArg = number; g_worldThis = this; return &g_players[number & 3]; }
int main()
{
    CWorld world;
    CGameEvent e; e.Type = 5; e.Player = 2; for (int i = 0; i < 32; ++i) e.Data[i] = 0; e.EndPos = 0; e.Valid = true; e.Replacement = false;
    // 1) payload byte 1 -> flag set on player 2
    e.Data[0] = 1; g_players[2].UsingFreeCamera = false;
    world.EAUseFreeCamera(&e);
    CHECK(g_getDataCalls == 1 && g_getDataEvent == &e && g_reads == 1 && g_readLen == 1);
    CHECK(g_playerArg == 2 && g_worldThis == &world && g_players[2].UsingFreeCamera == true);
    // 2) payload byte 0 -> flag cleared
    e.Data[0] = 0;
    world.EAUseFreeCamera(&e);
    CHECK(g_players[2].UsingFreeCamera == false);
    // 3) a non-canonical byte (2) is not `true`: normalised to false
    e.Data[0] = 2; g_players[2].UsingFreeCamera = true;
    world.EAUseFreeCamera(&e);
    CHECK(*(unsigned char*)&g_players[2].UsingFreeCamera == 0);
    // 4) negative player numbers are sign-extended
    e.Player = -1; e.Data[0] = 1;
    world.EAUseFreeCamera(&e);
    CHECK(g_playerArg == -1 && g_players[3].UsingFreeCamera == true);
    if (fails == 0) printf("00629420_TEST PASS\n");
    return fails;
}
