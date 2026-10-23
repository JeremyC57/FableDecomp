#include <cstdio>
// Behaviour test for CVtable_012B4F74::Close (retail 0x00C3C450).
struct CVtable_012B4F74;

static int g_unregisterCount = 0;
static CVtable_012B4F74* g_lastUnregistered = 0;
static int g_freeCount = 0;
static void* g_lastFreed = 0;

struct CNotificationManager {
    virtual void v00() { }
    virtual void Unregister(CVtable_012B4F74* client) { ++g_unregisterCount; g_lastUnregistered = client; }
};

static CNotificationManager g_manager;
CNotificationManager* g_pNotificationManager = &g_manager;

void FreeBuffer_00c3c370(void* buffer) { ++g_freeCount; g_lastFreed = buffer; }

struct CVtable_012B4F74 {
    virtual void v00() { }
    virtual void Close();
    unsigned char _pad_0x04[0x24];
    void*         m_pBuffer;
    unsigned long m_state;

    unsigned char IsRegisteredA() const { return (unsigned char)(m_state >> 2) & 1; }
    unsigned char IsRegisteredB() const { return (unsigned char)(m_state >> 3) & 1; }
};

void CVtable_012B4F74::Close()
{
    if (m_state != 0) {
        if (IsRegisteredA() && IsRegisteredB())
            g_pNotificationManager->Unregister(this);
        if ((m_state & 2) && m_pBuffer)
            FreeBuffer_00c3c370(m_pBuffer);
        m_state = 0;
    }
}

static int Fail() { std::printf("00c3c450_TEST FAIL\n"); return 1; }

int main()
{
    int buffer = 0;
    CVtable_012B4F74 o;
    // zero state: nothing happens
    o.m_pBuffer = &buffer; o.m_state = 0;
    o.Close();
    if (g_unregisterCount || g_freeCount || o.m_state != 0) return Fail();
    // all bits: unregister + free + clear
    o.m_state = 0x0e;
    o.Close();
    if (g_unregisterCount != 1 || g_lastUnregistered != &o || g_freeCount != 1 || g_lastFreed != &buffer || o.m_state != 0) return Fail();
    // only bit 2 (not bit 3): no unregister; bit 1 with null buffer: no free
    o.m_state = 0x06; o.m_pBuffer = 0;
    o.Close();
    if (g_unregisterCount != 1 || g_freeCount != 1 || o.m_state != 0) return Fail();
    // unrelated bit: just cleared
    o.m_state = 0x80; o.m_pBuffer = &buffer;
    o.Close();
    if (g_unregisterCount != 1 || g_freeCount != 1 || o.m_state != 0) return Fail();
    std::printf("00c3c450_TEST PASS\n");
    return 0;
}