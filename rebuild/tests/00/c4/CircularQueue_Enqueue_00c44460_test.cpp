// Behaviour test for 0x00C44460 CircularQueue::Enqueue(CSoundBuffer* buffer, unsigned __int64 startTime)
#include <stdio.h>

struct Format { unsigned int W0, W1, W2; };
struct Buffer { Format Fmt; unsigned int Rest[4]; unsigned int RefCount; bool FailAddRef;
    bool AddRef() { if (FailAddRef) return false; ++RefCount; return true; } };
static int g_lastError = 0;
struct Queue {
    Buffer** Items; unsigned int* TimesLo; unsigned int* TimesHi; unsigned int Capacity, Head, Tail; Format Fmt;
    bool Enqueue(Buffer* buffer, unsigned int lo, unsigned int hi)  // the row's body
    {
        Buffer format = *buffer;
        bool matches = true;
        if (Capacity != 0 && Fmt.W0 == 0)
            Fmt = format.Fmt;
        else {
            if (Fmt.W0 != format.Fmt.W0) matches = false;
            if (Fmt.W1 != format.Fmt.W1) matches = false;
            if (!matches) { g_lastError = -14; return false; }
        }
        if (Items[Head] != 0) { g_lastError = -15; return false; }
        if (!buffer->AddRef()) return false;
        Items[Head] = buffer; TimesLo[Head] = lo; TimesHi[Head] = hi;
        if (Capacity == ++Head) Head = 0;
        return true;
    }
};

int main()
{
    Buffer* slots[2] = { 0, 0 }; unsigned int lo[2], hi[2];
    Queue q = { slots, lo, hi, 2, 0, 0, { 0, 0, 0 } };
    Buffer a = { { 44100, 2, 16 }, { 0 }, 0, false }, b = a, c = a, wrong = a;
    wrong.Fmt.W1 = 1;
    int ok = 1;
    ok &= q.Enqueue(&a, 1, 2) && q.Fmt.W0 == 44100 && q.Head == 1 && a.RefCount == 1 && hi[0] == 2;
    ok &= !q.Enqueue(&wrong, 0, 0) && g_lastError == -14;
    ok &= q.Enqueue(&b, 3, 4) && q.Head == 0 && slots[1] == &b;
    ok &= !q.Enqueue(&c, 5, 6) && g_lastError == -15;
    slots[0] = 0; c.FailAddRef = true;
    ok &= !q.Enqueue(&c, 5, 6) && slots[0] == 0;
    printf(ok ? "00c44460_TEST PASS\n" : "00c44460_TEST FAIL\n");
    return ok ? 0 : 1;
}