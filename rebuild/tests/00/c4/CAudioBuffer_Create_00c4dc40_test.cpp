// Behaviour test for CAudioBuffer::Create (0x00C4DC40): a copy of the model's logic over a stub OpenAL context.
#include <stdio.h>
enum { AL_FORMAT_MONO8 = 0x1100, AL_FORMAT_MONO16 = 0x1101, AL_FORMAT_STEREO8 = 0x1102, AL_FORMAT_STEREO16 = 0x1103 };
struct SAudioBufferDesc { long SampleFormat; unsigned long Channels; long Frequency; long Size; long _10, _14; const void* Data; };
struct CAudioContext {
    bool Suspended; long RefCount; int errorAt; int checks; int suspends; int processes; long lastFormat; int deleted; unsigned long nextBuffer;
    void GetError() { }
    void Suspend() { ++suspends; }
    void Process() { ++processes; }
    void GenBuffers(long, unsigned long* b) { *b = nextBuffer++; }
    void DeleteBuffers(long, const unsigned long*) { ++deleted; }
    void BufferData(unsigned long, long format, const void*, long, long) { lastFormat = format; }
    long CheckError() { ++checks; return checks == errorAt ? -1 : 0; }
};
static CAudioContext g_AudioContext;
static long GetALFormat(long sampleFormat, unsigned long channels)
{
    switch (sampleFormat) {
    case 1: return channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    case 2: return channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
    }
    return 0;
}
struct CAudioBuffer {
    unsigned long Buffer; long _4; unsigned long Flags; const SAudioBufferDesc* Desc;
    long Create(const SAudioBufferDesc* desc)
    {
        if (desc->Channels > 2) return -14;
        g_AudioContext.GetError();
        bool wasSuspended = g_AudioContext.Suspended;
        g_AudioContext.Suspend();
        g_AudioContext.GenBuffers(1, &Buffer);
        if (!wasSuspended) g_AudioContext.Process();
        if (g_AudioContext.CheckError() < 0) return -5;
        g_AudioContext.BufferData(Buffer, GetALFormat(desc->SampleFormat, desc->Channels), desc->Data, desc->Size, desc->Frequency);
        if (g_AudioContext.CheckError() < 0) {
            bool suspended = g_AudioContext.Suspended;
            g_AudioContext.Suspend();
            g_AudioContext.DeleteBuffers(1, &Buffer);
            if (!suspended) g_AudioContext.Process();
            return -14;
        }
        Flags |= 1;
        Desc = desc;
        ++g_AudioContext.RefCount;
        return 0;
    }
};
static void reset(int errorAt, bool suspended)
{
    g_AudioContext.Suspended = suspended; g_AudioContext.errorAt = errorAt; g_AudioContext.checks = 0;
    g_AudioContext.suspends = 0; g_AudioContext.processes = 0; g_AudioContext.deleted = 0; g_AudioContext.lastFormat = 0;
}
int main()
{
    int ok = 1;
    SAudioBufferDesc d = { 1, 2, 22050, 100, 0, 0, 0 };
    CAudioBuffer b = { 0, 0, 0, 0 };
    g_AudioContext.RefCount = 0; g_AudioContext.nextBuffer = 7;
    reset(0, false);
    ok &= b.Create(&d) == 0 && b.Buffer == 7 && b.Flags == 1 && b.Desc == &d && g_AudioContext.RefCount == 1;
    ok &= g_AudioContext.lastFormat == AL_FORMAT_STEREO16 && g_AudioContext.processes == 1;
    d.SampleFormat = 2; d.Channels = 1; reset(0, true);
    ok &= b.Create(&d) == 0 && g_AudioContext.lastFormat == AL_FORMAT_MONO8 && g_AudioContext.processes == 0;
    d.Channels = 3; reset(0, false);
    ok &= b.Create(&d) == -14 && g_AudioContext.suspends == 0;
    d.Channels = 2; reset(1, false);
    ok &= b.Create(&d) == -5 && g_AudioContext.RefCount == 2;
    reset(2, false);
    ok &= b.Create(&d) == -14 && g_AudioContext.deleted == 1 && g_AudioContext.processes == 2 && g_AudioContext.RefCount == 2;
    printf(ok ? "00c4dc40_TEST PASS\n" : "00c4dc40_TEST FAIL\n");
    return ok ? 0 : 1;
}