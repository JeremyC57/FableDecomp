// Behaviour test for CCmdQueue::GetCommandDueFor (0x00CAFA10, DirectShow ctlutil.cpp): a presentation command already due by
// the clock wins (AddRef'd); else a stream command due by rtStream (ctlutil AddRefs the presentation head there, as retail);
// else, while running, a presentation command due by rtStream + offset (no AddRef); else VFW_E_NOT_FOUND.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct Cmd { i64 time; int refs; };
struct Queue {
    Cmd* presentHead; Cmd* streamHead; bool hasClock; i64 now; bool running; i64 offset;
    long GetCommandDueFor(i64 rtStream, Cmd** ppCmd)
    {
        i64 tStream = rtStream;
        Cmd* pStream = streamHead;
        Cmd* pPresent = presentHead;
        if (pPresent && hasClock && pPresent->time <= now) { pPresent->refs++; *ppCmd = pPresent; return 0; }
        if (pStream && pStream->time <= tStream) { pPresent->refs++; *ppCmd = pStream; return 0; }
        if (running && pPresent) {
            tStream += offset;
            if (pPresent->time <= tStream) { *ppCmd = pPresent; return 0; }
        }
        return (long)0x80040216L;
    }
};
int main()
{
    Cmd present = { 100, 0 }; Cmd stream = { 50, 0 }; Cmd* out = 0;
    Queue q = { &present, &stream, true, 150, false, 0 };
    if (q.GetCommandDueFor(0, &out) != 0 || out != &present || present.refs != 1) { std::printf("00cafa10_TEST FAIL\n"); return 1; }
    q.now = 10; out = 0;
    if (q.GetCommandDueFor(60, &out) != 0 || out != &stream || present.refs != 2 || stream.refs != 0) { std::printf("00cafa10_TEST FAIL\n"); return 1; }
    q.running = true; q.offset = 70; out = 0;
    if (q.GetCommandDueFor(40, &out) != 0 || out != &present || present.refs != 2) { std::printf("00cafa10_TEST FAIL\n"); return 1; }
    q.running = false; out = 0;
    if (q.GetCommandDueFor(40, &out) != (long)0x80040216L || out != 0) { std::printf("00cafa10_TEST FAIL\n"); return 1; }
    std::printf("00cafa10_TEST PASS\n");
    return 0;
}