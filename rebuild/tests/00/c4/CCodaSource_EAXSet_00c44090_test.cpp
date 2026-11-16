// Behaviour test for CCodaSource::EAXSet (0x00C44090): self-contained doubles. A source with a voice forwards the four arguments
// to the voice's EAXSet; one without first allocates a voice for its set buffer, else the queue's play buffer, else null. An
// allocation error is latched and the voice is not called; a voice error is latched; success latches nothing.
#include <stdio.h>
struct _GUID { unsigned long Data1; };
struct CSalBuffer { int Id; };
static int g_error, g_setErrorCalls;
struct CCodaContext { void SetError(int error) { ++g_setErrorCalls; if (g_error == 0) g_error = error; } };
static CCodaContext CODA;
struct CSalVoice {
    int SetCalls, GetCalls, Result; const _GUID* Guid; int Property; void* Data; int Size;
    CSalVoice() : SetCalls(0), GetCalls(0), Result(0), Guid(0), Property(0), Data(0), Size(0) {}
    virtual int EAXSet(const _GUID* g, int p, void* d, int s) { ++SetCalls; Guid = g; Property = p; Data = d; Size = s; return Result; }
    virtual int EAXGet(const _GUID* g, int p, void* d, int s) { ++GetCalls; Guid = g; Property = p; Data = d; Size = s; return Result; }
};
struct CCodaBufferQueue {
    CSalBuffer** m_ppBuffer; unsigned int m_nBuffers, m_nAdd, m_nPlay;
    unsigned int GetSize() const { return m_nBuffers; }
    bool IsCreated() const { return GetSize() != 0; }
    CSalBuffer* GetPlayBuffer() const { if (GetSize()) return m_ppBuffer[m_nPlay]; return 0; }
};
struct CCodaSource {
    CSalVoice* m_pVoice; CCodaBufferQueue m_Queue; CSalBuffer* m_pBuffer;
    CSalVoice* Supply; int AllocateResult, AllocateCalls; CSalBuffer* AllocateBuffer;
    virtual int AllocateVoice(CSalBuffer* buffer, CSalVoice** voice) { ++AllocateCalls; AllocateBuffer = buffer; if (AllocateResult == 0) *voice = Supply; return AllocateResult; }
    void EAXSet(const _GUID* guid, int property, void* data, int size)
    {
        int error;
        if (!m_pVoice) {
            error = AllocateVoice(m_pBuffer ? m_pBuffer : m_Queue.IsCreated() ? m_Queue.GetPlayBuffer() : 0, &m_pVoice);
            if (error) {
                CODA.SetError(error);
                return;
            }
        }
        error = m_pVoice->EAXSet(guid, property, data, size);
        if (error)
            CODA.SetError(error);
    }
};
int main()
{
    int ok = 1, cases = 0;
    CSalBuffer set = { 1 }, q0 = { 2 }, q1 = { 3 }; CSalBuffer* queued[2] = { &q0, &q1 };
    _GUID guid = { 0x1234 }; char payload[8];
    // has voice: 0/1; buffer source: 0 set buffer, 1 queue (play index 1), 2 nothing; allocation result 0 / -5; voice result 0 / -7
    for (int hasVoice = 0; hasVoice < 2; ++hasVoice) for (int source = 0; source < 3; ++source) for (int allocFails = 0; allocFails < 2; ++allocFails) for (int voiceFails = 0; voiceFails < 2; ++voiceFails) {
        CSalVoice voice; voice.Result = voiceFails ? -7 : 0;
        CCodaSource s; s.m_pVoice = hasVoice ? &voice : 0; s.Supply = &voice; s.AllocateResult = allocFails ? -5 : 0; s.AllocateCalls = 0; s.AllocateBuffer = (CSalBuffer*)1;
        s.m_pBuffer = source == 0 ? &set : 0; s.m_Queue.m_ppBuffer = queued; s.m_Queue.m_nBuffers = source == 2 ? 0 : 2; s.m_Queue.m_nAdd = 0; s.m_Queue.m_nPlay = 1;
        g_error = 0; g_setErrorCalls = 0;
        s.EAXSet(&guid, 40 + cases, payload, 8);
        bool allocated = !hasVoice, reached = hasVoice || !allocFails;
        ok = ok && s.AllocateCalls == (allocated ? 1 : 0);
        if (allocated) ok = ok && s.AllocateBuffer == (source == 0 ? &set : source == 1 ? &q1 : (CSalBuffer*)0);
        ok = ok && voice.SetCalls == (reached ? 1 : 0) && voice.GetCalls == 0;
        if (reached) ok = ok && voice.Guid == &guid && voice.Property == 40 + cases && voice.Data == payload && voice.Size == 8 && s.m_pVoice == &voice;
        else ok = ok && s.m_pVoice == 0;
        int expected = !reached ? -5 : voiceFails ? -7 : 0;
        ok = ok && g_error == expected && g_setErrorCalls == (expected ? 1 : 0);
        ++cases;
    }
    ok = ok && cases == 24;
    printf(ok ? "00c44090_TEST PASS\n" : "00c44090_TEST FAIL\n");
    return ok ? 0 : 1;
}