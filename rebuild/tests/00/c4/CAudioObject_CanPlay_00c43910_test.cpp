// Behaviour test for 00c43910: CAudioObject::CanPlay -- true with a voice, else ask CanAcquireVoice for the current or queued buffer
#include <cstdio>

struct CSoundBuffer { int id; };
struct SVoiceCaps { unsigned int m_id, m_available, m_objectCaps, m_formatCaps, m_words[2]; };
static SVoiceCaps g_voiceCaps[4];
static unsigned int g_audioFlags = 0x18;
static CSoundBuffer* g_seenBuffer;
static unsigned int g_seenFlags;
static const SVoiceCaps* g_seenHardware;
static const SVoiceCaps* g_seenSoftware;

struct CircularQueue {
    CSoundBuffer** m_items; unsigned int m_capacity, m_tail;
    CSoundBuffer* Peek() const { if (m_capacity == 0) return 0; return m_items[m_tail]; }
};
struct CAudioObject {
    bool m_3d;
    void* m_pVoice;
    CircularQueue m_queue;
    CSoundBuffer* m_pCurrentBuffer;
    bool Is3D() const { return m_3d; }
    bool CanAcquireVoice(CSoundBuffer* buffer, unsigned int flags, const SVoiceCaps* hardware, const SVoiceCaps* software)
    {
        g_seenBuffer = buffer; g_seenFlags = flags; g_seenHardware = hardware; g_seenSoftware = software;
        return buffer != 0;
    }
    bool CanPlay()
    {
        if (m_pVoice)
            return true;
        CSoundBuffer* buffer = m_pCurrentBuffer;
        if (!buffer)
            buffer = m_queue.m_capacity != 0 ? m_queue.Peek() : 0;
        return CanAcquireVoice(buffer, g_audioFlags, Is3D() ? &g_voiceCaps[1] : &g_voiceCaps[0],
                               Is3D() ? &g_voiceCaps[3] : &g_voiceCaps[2]);
    }
};

int main()
{
    CSoundBuffer a = { 1 }, b = { 2 };
    CSoundBuffer* items[2] = { &a, &b };
    CAudioObject o; o.m_3d = false; o.m_pVoice = 0; o.m_queue.m_items = items; o.m_queue.m_capacity = 0; o.m_queue.m_tail = 1; o.m_pCurrentBuffer = 0;
    int ok = 1;
    ok &= o.CanPlay() == false && g_seenBuffer == 0 && g_seenHardware == &g_voiceCaps[0] && g_seenSoftware == &g_voiceCaps[2] && g_seenFlags == 0x18;
    o.m_queue.m_capacity = 2; o.m_3d = true;
    ok &= o.CanPlay() == true && g_seenBuffer == &b && g_seenHardware == &g_voiceCaps[1] && g_seenSoftware == &g_voiceCaps[3];
    o.m_pCurrentBuffer = &a;
    ok &= o.CanPlay() == true && g_seenBuffer == &a;
    g_seenBuffer = 0; o.m_pVoice = &o;
    ok &= o.CanPlay() == true && g_seenBuffer == 0;
    if (!ok) { std::printf("00c43910_TEST FAIL\n"); return 1; }
    std::printf("00c43910_TEST PASS\n");
    return 0;
}