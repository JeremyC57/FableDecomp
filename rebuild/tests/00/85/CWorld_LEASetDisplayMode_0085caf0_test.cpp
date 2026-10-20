// Behaviour test for CWorld::LEASetDisplayMode @ 0x0085CAF0: the event payload is copied into a fresh zeroed buffer
// and read as a 16-bit width, a 16-bit height and an 8-bit depth (in that order), which go zero-extended to the main
// game component's (+0x08) SetDisplayMode(width, height, depth). Checks a normal mode and one with high bits set
// (no sign extension), the read sizes/order and the buffer's initial state.
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
struct CGameEvent {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

struct CMainGameComponent { void SetDisplayMode(unsigned long width, unsigned long height, unsigned char depth); };   // 0x0041641F

struct CWorld {
    void*               __vftable;                 // +0x00
    unsigned char       _base_0x04[0x4];
    CMainGameComponent* Component;                 // +0x08
    void LEASetDisplayMode(const CGameEvent* event);
};

void CWorld::LEASetDisplayMode(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    unsigned short width;
    buffer.Read(2, &width);
    unsigned short height;
    buffer.Read(2, &height);
    unsigned char depth;
    buffer.Read(1, &depth);
    Component->SetDisplayMode(width, height, depth);
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
static unsigned long g_w = 0, g_h = 0;
static unsigned char g_d = 0;
static CMainGameComponent* g_expectMgc = 0;
void CMainGameComponent::SetDisplayMode(unsigned long width, unsigned long height, unsigned char depth)
{ ++g_sets; g_w = width; g_h = height; g_d = depth; if (this != g_expectMgc) g_fail = 1; }
static void reset() { resetEv(); g_sets = 0; g_w = g_h = 0; g_d = 0; }
static void payload(CGameEvent& e, unsigned short w, unsigned short h, unsigned char d)
{ unsigned char p[5]; std::memcpy(p, &w, 2); std::memcpy(p + 2, &h, 2); p[4] = d; makeEvent(e, 0, p, 5); }

int main()
{
    CMainGameComponent mgc; g_expectMgc = &mgc;
    CWorld world; world.__vftable = 0; world.Component = &mgc;
    CGameEvent ev; g_expectEvent = &ev;

    reset(); payload(ev, 1024, 768, 32);
    world.LEASetDisplayMode(&ev);
    CHECK(g_getData == 1 && g_bufInitOk && g_reads == 3 && g_readSizes[0] == 2 && g_readSizes[1] == 2 && g_readSizes[2] == 1 && g_readsInBuffer);
    CHECK(g_sets == 1 && g_w == 1024 && g_h == 768 && g_d == 32);

    reset(); payload(ev, 0xffff, 0x8000, 0xff);
    world.LEASetDisplayMode(&ev);
    CHECK(g_getData == 1 && g_bufInitOk && g_reads == 3 && g_readsInBuffer);
    CHECK(g_sets == 1 && g_w == 0xffffUL && g_h == 0x8000UL && g_d == 0xff);

    reset(); payload(ev, 640, 480, 16);
    world.LEASetDisplayMode(&ev);
    CHECK(g_sets == 1 && g_w == 640 && g_h == 480 && g_d == 16);

    if (g_fail) { std::printf("0085caf0_TEST FAIL\n"); return 1; }
    std::printf("0085caf0_TEST PASS\n");
    return 0;
}