// Behaviour test for CWorld::LEASetExclusiveMode @ 0x0085CA80: the event payload is copied into a fresh zeroed
// buffer, one byte is read, and the world's main game component (+0x08) gets SetExclusiveMode(byte != 0).
// Checks 0 (false), 1 (true), 0x80 (any non-zero byte is true), the buffer's initial state and the 1-byte read.
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

class CMainGameComponent { public: void SetExclusiveMode(bool exclusive); };   // 0x004163B7

class CWorld { public:
    void*               __vftable;                 // +0x00
    unsigned char       _base_0x04[0x4];
    CMainGameComponent* Component;                 // +0x08
    void LEASetExclusiveMode(const CGameEvent* event);
};

void CWorld::LEASetExclusiveMode(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    unsigned char exclusive;
    buffer.Read(1, &exclusive);
    Component->SetExclusiveMode(exclusive != 0);
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
static int g_sets = 0;
static unsigned char g_setArgRaw = 0xcc;
static CMainGameComponent* g_expectMgc = 0;
void CMainGameComponent::SetExclusiveMode(bool exclusive)
{ ++g_sets; std::memcpy(&g_setArgRaw, &exclusive, 1); if (this != g_expectMgc) g_fail = 1; }
static void reset() { resetEv(); g_sets = 0; g_setArgRaw = 0xcc; }

int main()
{
    CMainGameComponent mgc; g_expectMgc = &mgc;
    CWorld world; world.__vftable = 0; world.Component = &mgc;
    CGameEvent ev; g_expectEvent = &ev;
    unsigned char v;

    reset(); v = 0; makeEvent(ev, 0, &v, 1);
    world.LEASetExclusiveMode(&ev);
    CHECK(g_getData == 1 && g_bufInitOk && g_reads == 1 && g_readSizes[0] == 1 && g_readsInBuffer && g_sets == 1 && g_setArgRaw == 0);

    reset(); v = 1; makeEvent(ev, 0, &v, 1);
    world.LEASetExclusiveMode(&ev);
    CHECK(g_getData == 1 && g_bufInitOk && g_reads == 1 && g_sets == 1 && g_setArgRaw == 1);

    reset(); v = 0x80; makeEvent(ev, 0, &v, 1);
    world.LEASetExclusiveMode(&ev);
    CHECK(g_sets == 1 && g_setArgRaw == 1);

    if (g_fail) { std::printf("0085ca80_TEST FAIL\n"); return 1; }
    std::printf("0085ca80_TEST PASS\n");
    return 0;
}