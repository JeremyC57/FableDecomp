// Behaviour test for CWorld::HandleLeaveWorldPacket @ 0x006294A0: the packet is copied into a fresh zero-initialised
// 0x2c-byte packet buffer, one byte (the auto-load flag) is read from it, and the world either auto-loads the region
// being left (flag 1) or resets to the front end (flag 0) -- exactly one of the two, on the same world object.
// Checks both arms, the buffer's initial state, the read size and that the packet's own buffer is the one read.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct SPacketBuffer {
    unsigned char m_payload[0x20];                 // +0x00
    long          m_length;                        // +0x20
    long          m_a;                             // +0x24
    long          m_b;                             // +0x28
    SPacketBuffer() : m_length(0), m_a(0), m_b(0) {}
    void ReadBytes(long count, void* destination); // 0x009F18F0
};

struct CEventPacket {
    long CopyToBuffer(SPacketBuffer* buffer);      // 0x009F17A0
};

class CWorld { public:
    bool AutoLoadRegionLeaving();                  // 0x004A5E90
    void ResetToFrontEnd();                        // 0x0049DCC0
    void HandleLeaveWorldPacket(CEventPacket* packet);
};

void CWorld::HandleLeaveWorldPacket(CEventPacket* packet)
{
    SPacketBuffer buffer;
    packet->CopyToBuffer(&buffer);
    bool autoLoad;
    buffer.ReadBytes(1, &autoLoad);
    if (autoLoad == true)
        AutoLoadRegionLeaving();
    else
        ResetToFrontEnd();
}

// ---- instrumented stubs ----
struct TestPacket : CEventPacket { unsigned char flag; };
static int g_copies = 0, g_reads = 0, g_autoLoads = 0, g_resets = 0;
static bool g_initOk = false, g_readOk = false;
static SPacketBuffer* g_copiedInto = 0;
static CEventPacket* g_expectPacket = 0;
static CWorld* g_expectWorld = 0;

long CEventPacket::CopyToBuffer(SPacketBuffer* buffer)
{
    ++g_copies;
    if (this != g_expectPacket) g_fail = 1;
    g_initOk = buffer->m_length == 0 && buffer->m_a == 0 && buffer->m_b == 0;
    g_copiedInto = buffer;
    buffer->m_payload[0] = static_cast<TestPacket*>(this)->flag;
    buffer->m_length = 1;
    return 1;
}
void SPacketBuffer::ReadBytes(long count, void* destination)
{
    ++g_reads;
    g_readOk = count == 1 && this == g_copiedInto && m_a + count <= m_length;
    std::memcpy(destination, m_payload + m_a, count); m_a += count;
}
bool CWorld::AutoLoadRegionLeaving() { ++g_autoLoads; if (this != g_expectWorld) g_fail = 1; return true; }
void CWorld::ResetToFrontEnd() { ++g_resets; if (this != g_expectWorld) g_fail = 1; }

static void reset() { g_copies = g_reads = g_autoLoads = g_resets = 0; g_initOk = g_readOk = false; g_copiedInto = 0; }

int main()
{
    CWorld world; g_expectWorld = &world;
    TestPacket packet; g_expectPacket = &packet;

    // Flag 1: auto-load the region being left.
    reset(); packet.flag = 1;
    world.HandleLeaveWorldPacket(&packet);
    CHECK(g_copies == 1 && g_initOk && g_reads == 1 && g_readOk);
    CHECK(g_autoLoads == 1 && g_resets == 0);

    // Flag 0: reset to the front end.
    reset(); packet.flag = 0;
    world.HandleLeaveWorldPacket(&packet);
    CHECK(g_copies == 1 && g_initOk && g_reads == 1 && g_readOk);
    CHECK(g_autoLoads == 0 && g_resets == 1);

    // Flag 1 again (no state carried over between calls).
    reset(); packet.flag = 1;
    world.HandleLeaveWorldPacket(&packet);
    CHECK(g_autoLoads == 1 && g_resets == 0 && g_initOk);

    if (g_fail) { std::printf("006294a0_TEST FAIL\n"); return 1; }
    std::printf("006294a0_TEST PASS\n");
    return 0;
}