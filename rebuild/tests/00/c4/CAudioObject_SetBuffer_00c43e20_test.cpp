// Behaviour test for 00c43e20: CAudioObject::SetBuffer -- drop the queue, reset, check, swap the referenced buffer, rebind or release the voice
#include <cstdio>

static int g_error = 0, g_queueReleased = 0, g_resets = 0, g_voiceReleased = 0;
struct CSoundBuffer {
    int m_create; bool m_addRef; int m_refs;
    int CreateHardwareBuffer() { return m_create; }
    bool AddRef() { if (!m_addRef) return false; ++m_refs; return true; }
    void Release() { --m_refs; }
};
struct CAudioObject;
struct CVoice { int m_rebind; int Rebind(CAudioObject*) { return m_rebind; } };
struct CircularQueue { unsigned int m_capacity; };
struct CAudioObject {
    CVoice* m_pVoice; CircularQueue m_queue; CSoundBuffer* m_pCurrentBuffer; bool m_usable;
    void ReleaseQueue() { ++g_queueReleased; m_queue.m_capacity = 0; }
    void Reset() { ++g_resets; }
    bool CanUseBuffer(CSoundBuffer*) { return m_usable; }
    bool SetBuffer(CSoundBuffer* buffer)
    {
        if (m_queue.m_capacity)
            ReleaseQueue();
        Reset();
        if (buffer && !CanUseBuffer(buffer))
            return false;
        if (m_pCurrentBuffer != buffer) {
            if (buffer) {
                int result = buffer->CreateHardwareBuffer();
                if (result < 0) {
                    g_error = result;
                    return false;
                }
                if (!buffer->AddRef())
                    return false;
            }
            if (m_pCurrentBuffer)
                m_pCurrentBuffer->Release();
            m_pCurrentBuffer = buffer;
            if (m_pVoice && m_pVoice->Rebind(this)) {
                ++g_voiceReleased;
                m_pVoice = 0;
            }
        }
        return true;
    }
};

int main()
{
    CSoundBuffer a = { 0, true, 0 }, b = { 0, true, 0 }, bad = { -5, true, 0 }, full = { 0, false, 0 };
    CVoice keep = { 0 }, drop = { 1 };
    CAudioObject o; o.m_pVoice = &keep; o.m_queue.m_capacity = 4; o.m_pCurrentBuffer = 0; o.m_usable = true;
    int ok = 1;
    ok &= o.SetBuffer(&a) && g_queueReleased == 1 && g_resets == 1 && o.m_pCurrentBuffer == &a && a.m_refs == 1 && o.m_pVoice == &keep;
    ok &= o.SetBuffer(&a) && a.m_refs == 1 && g_queueReleased == 1;           // unchanged
    ok &= !o.SetBuffer(&bad) && g_error == -5 && o.m_pCurrentBuffer == &a;
    ok &= !o.SetBuffer(&full) && o.m_pCurrentBuffer == &a;
    o.m_pVoice = &drop;
    ok &= o.SetBuffer(&b) && a.m_refs == 0 && b.m_refs == 1 && o.m_pVoice == 0 && g_voiceReleased == 1;
    ok &= o.SetBuffer(0) && b.m_refs == 0 && o.m_pCurrentBuffer == 0;
    o.m_usable = false;
    ok &= !o.SetBuffer(&a) && o.m_pCurrentBuffer == 0;
    if (!ok) { std::printf("00c43e20_TEST FAIL\n"); return 1; }
    std::printf("00c43e20_TEST PASS\n");
    return 0;
}