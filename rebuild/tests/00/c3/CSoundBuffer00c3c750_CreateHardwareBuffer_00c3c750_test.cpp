// Behaviour test for 00c3c750: CSoundBuffer::CreateHardwareBuffer -- creates the device buffer only when the audio device and its interface exist and the buffer is not a software one; more than two channels fails with -14
#include <cstdio>

struct SBufferDesc {
    unsigned long m_mode;
    unsigned long m_channels;
    unsigned long m_words[5];
};
struct CAudioDevice {
    void* m_pInterface;
};
static CAudioDevice* g_pAudioDevice = 0;
static int g_created = 0;

struct CDeviceBuffer {
    long Create(const SBufferDesc* desc) { ++g_created; return desc->m_mode == 7 ? -3 : 1; }
};

struct CSoundBuffer {
    SBufferDesc m_desc;
    unsigned long m_state;
    CDeviceBuffer m_buffer;
    unsigned char m_flags;
    const SBufferDesc* GetDesc() const { return &m_desc; }
    long CreateHardwareBuffer()
    {
        long result = 0;
        if (g_pAudioDevice && g_pAudioDevice->m_pInterface && !(m_flags & 1)) {
            if (GetDesc()->m_channels > 2)
                return -14;
            result = m_buffer.Create(GetDesc());
        }
        return result;
    }
};

static int fail() { std::printf("00c3c750_TEST FAIL\n"); return 1; }

int main()
{
    int iface;
    CAudioDevice device = { 0 };
    CSoundBuffer b;
    b.m_desc.m_mode = 0; b.m_desc.m_channels = 2; b.m_flags = 0;
    if (b.CreateHardwareBuffer() != 0 || g_created != 0) return fail();        // no device
    g_pAudioDevice = &device;
    if (b.CreateHardwareBuffer() != 0 || g_created != 0) return fail();        // no interface
    device.m_pInterface = &iface;
    if (b.CreateHardwareBuffer() != 1 || g_created != 1) return fail();
    b.m_desc.m_mode = 7;
    if (b.CreateHardwareBuffer() != -3 || g_created != 2) return fail();
    b.m_desc.m_channels = 6;
    if (b.CreateHardwareBuffer() != -14 || g_created != 2) return fail();
    b.m_flags = 1;                                                             // software buffer
    if (b.CreateHardwareBuffer() != 0 || g_created != 2) return fail();
    std::printf("00c3c750_TEST PASS\n");
    return 0;
}