// Behaviour test for 00c43de0: CAudioObject::AcquireVoice -- create a voice for the current or queued buffer through the virtual
#include <cstdio>

struct CSoundBuffer { int id; };
struct CVoice { int id; };
static CVoice g_voice = { 9 };
struct CircularQueue {
    CSoundBuffer** m_items; unsigned int m_capacity, m_tail;
    CSoundBuffer* Peek() const { if (m_capacity == 0) return 0; return m_items[m_tail]; }
};
struct CAudioObject {
    CVoice* m_pVoice; CircularQueue m_queue; CSoundBuffer* m_pCurrentBuffer; CSoundBuffer* m_seen;
    virtual int CreateVoice(CSoundBuffer* buffer, CVoice** ppVoice) { m_seen = buffer; if (!buffer) return -3; *ppVoice = &g_voice; return 0; }
    virtual ~CAudioObject() { }
    int AcquireVoice()
    {
        CSoundBuffer* buffer = m_pCurrentBuffer;
        if (!buffer)
            buffer = m_queue.m_capacity != 0 ? m_queue.Peek() : 0;
        return CreateVoice(buffer, &m_pVoice);
    }
};

int main()
{
    CSoundBuffer a = { 1 }, b = { 2 };
    CSoundBuffer* items[2] = { &a, &b };
    CAudioObject o; o.m_pVoice = 0; o.m_queue.m_items = items; o.m_queue.m_capacity = 0; o.m_queue.m_tail = 1; o.m_pCurrentBuffer = 0; o.m_seen = &a;
    int ok = 1;
    ok &= o.AcquireVoice() == -3 && o.m_seen == 0 && o.m_pVoice == 0;
    o.m_queue.m_capacity = 2;
    ok &= o.AcquireVoice() == 0 && o.m_seen == &b && o.m_pVoice == &g_voice;
    o.m_pCurrentBuffer = &a;
    ok &= o.AcquireVoice() == 0 && o.m_seen == &a;
    if (!ok) { std::printf("00c43de0_TEST FAIL\n"); return 1; }
    std::printf("00c43de0_TEST PASS\n");
    return 0;
}