// Behaviour test for CAudioSource::SetSourceProperty (0x00C72A40): translate the item to a buffer id (fail -> return the negative code), clear the AL error, alSourcei(source, AL_BUFFER, id), return the checked error.
#include <stdio.h>
static int g_errClears = 0, g_param = 0, g_value = 0, g_source = 0;
struct Ctx {
    long GetError() { ++g_errClears; return 0; }
    void Sourcei(unsigned long s, long p, long v) { g_source = (int)s; g_param = (int)p; g_value = (int)v; }
    long CheckError() { return 5; }
};
static Ctx g_AudioContext;
struct Src {
    unsigned long SourceId; long fail;
    long PrepareBuffer(long item, unsigned long* id) { if (fail) return -3; *id = (unsigned long)(item * 10); return 0; }
    long SetSourceProperty(long item)
    {
        Ctx& context = g_AudioContext;
        long result = PrepareBuffer(item, (unsigned long*)&item);
        if (result < 0) return result;
        context.GetError();
        context.Sourcei(SourceId, 0x1009, item);
        return context.CheckError();
    }
};
int main()
{
    Src s; s.SourceId = 9; s.fail = 1;
    if (s.SetSourceProperty(4) != -3 || g_errClears != 0) { printf("00c72a40_TEST FAIL\n"); return 1; }
    s.fail = 0;
    if (s.SetSourceProperty(4) != 5 || g_errClears != 1 || g_source != 9 || g_param != 0x1009 || g_value != 40) { printf("00c72a40_TEST FAIL\n"); return 1; }
    printf("00c72a40_TEST PASS\n"); return 0;
}