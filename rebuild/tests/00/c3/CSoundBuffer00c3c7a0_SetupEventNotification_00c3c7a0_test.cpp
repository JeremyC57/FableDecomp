// Behaviour test for 00c3c7a0: CSoundBuffer::SetupEventNotification -- base setup first; with a live device, a multichannel software buffer drops its device buffer, otherwise the device buffer is rebuilt and a failed rebuild closes the buffer
#include <cstdio>

struct SBufferDesc {
    unsigned long m_mode;
    unsigned long m_channels;
};
struct CAudioDevice { void* m_pInterface; };
static CAudioDevice* g_pAudioDevice = 0;
static int g_destroyed = 0, g_rebuilt = 0, g_closed = 0;
static long g_baseResult = 0, g_rebuildResult = 0;

struct CDeviceBuffer {
    void Destroy() { ++g_destroyed; }
    long Rebuild(const SBufferDesc*) { ++g_rebuilt; return g_rebuildResult; }
};

class CSoundBufferBase {
public:
    SBufferDesc m_desc;
    virtual ~CSoundBufferBase() { }
    virtual void Close() { ++g_closed; }
    virtual long SetupEventNotification(const SBufferDesc* desc) { m_desc = *desc; return g_baseResult; }
};

class CSoundBuffer : public CSoundBufferBase {
public:
    CDeviceBuffer m_buffer;
    unsigned char m_flags;
    virtual void Close() { }                                // the override is bypassed (qualified call)
    virtual long SetupEventNotification(const SBufferDesc* desc)
    {
        long result = CSoundBufferBase::SetupEventNotification(desc);
        if (result < 0)
            return result;
        if (!g_pAudioDevice || !g_pAudioDevice->m_pInterface)
            return 0;
        if ((m_flags & 1) && desc->m_channels > 2) {
            m_buffer.Destroy();
            return result;
        }
        result = m_buffer.Rebuild(&m_desc);
        if (result >= 0)
            return 0;
        CSoundBufferBase::Close();
        return result;
    }
};

static int fail() { std::printf("00c3c7a0_TEST FAIL\n"); return 1; }

int main()
{
    int iface;
    CAudioDevice device = { &iface };
    CSoundBuffer b; b.m_flags = 0;
    SBufferDesc d = { 1, 2 };
    g_baseResult = -5;
    if (b.SetupEventNotification(&d) != -5) return fail();
    g_baseResult = 3;
    if (b.SetupEventNotification(&d) != 0 || g_rebuilt != 0) return fail();       // no device
    g_pAudioDevice = &device;
    g_rebuildResult = 8;
    if (b.SetupEventNotification(&d) != 0 || g_rebuilt != 1 || g_closed != 0) return fail();
    g_rebuildResult = -2;
    if (b.SetupEventNotification(&d) != -2 || g_closed != 1) return fail();
    b.m_flags = 1; d.m_channels = 4;
    if (b.SetupEventNotification(&d) != 3 || g_destroyed != 1 || g_rebuilt != 2) return fail();
    std::printf("00c3c7a0_TEST PASS\n");
    return 0;
}