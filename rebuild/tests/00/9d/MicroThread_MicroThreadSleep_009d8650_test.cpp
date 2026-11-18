// Behaviour test for row 0x009D8650 MicroThread::MicroThreadSleep() (links the row's object; plain C++ + MSVC structured exception
// handling, no assembler). Two micro-threads are driven by a scheduler exactly as the retail module drives them:
//   scenario 1: each thread on its own stack region, the scheduler keeps each thread's parked token;
//   scenario 2: the retail design -- one shared stack (SetStack 0x009D86B0: guard word 0xCDCDCDCD at the top), each thread's stack
//               image copied under StackTop before the switch and back after it (SwitchToMicroThread 0x009D87F0's RestoreStack /
//               PreserveStack), first image = CreateMicroThread 0x009D8710's frame { &DummyExceptionBlock, 0, 0, 0, 0, entry,
//               MicroThreadReturn, argument }.
// Checked: entry reached with its argument; the interleaving of 24 trace points (each yield returns to the other side right after
// its call); locals of both sides survive the switches (ebp-framed /Od code: the saved ebp is restored); a structured exception
// raised after a yield inside a __try is caught by THAT side's handler, on the thread and on the scheduler (the fs:[0] chain head
// is parked and restored per side); the parked frame { chain, edi, esi, ebp, ebx, return } lies on the parked thread's stack with
// its chain head = the thread's own SEH registration, linked to the first frame's &DummyExceptionBlock; the shared stack's guard
// word is intact. A mutant without the chain-head swap, the ebp save or the global exchange fails or crashes.
#include <stdio.h>
#include <string.h>

extern "C" __declspec(dllimport) void __stdcall RaiseException(unsigned long code, unsigned long flags, unsigned long count,
                                                               const unsigned long* arguments);

namespace MicroThread
{
    unsigned char* PGlobalStackPointer;                       // 0x013CA898 (the row references it)
    void __fastcall MicroThreadSleep();                       // the row under test

    struct CDummyExceptionBlock { void* Next; void* Handler; };
    CDummyExceptionBlock DummyExceptionBlock = { (void*)-1, 0 };   // 0x0139650C: the first frame's chain head

    struct SInitialStackFrame                                 // CreateMicroThread's first image (the frame MicroThreadSleep pops)
    {
        CDummyExceptionBlock* ExceptionChain;
        unsigned long Edi, Esi, Ebp, Ebx;
        void (__cdecl *Entry)(void*);
        void (__cdecl *Return)();
        void* Argument;
    };
}

enum { kRegion = 0x8000, kThreads = 2, kImageMax = 0x6000 };

static int g_trace[64];
static int g_count;
static int g_done[kThreads];
static unsigned long g_sum[kThreads];
static unsigned char* g_lo[kThreads];                         // the stack range each thread runs on
static unsigned char* g_hi[kThreads];

static void Trace(int v)
{
    if (g_count < 64)
        g_trace[g_count++] = v;
}

static void __cdecl ThreadReturnTrap()                        // MicroThreadReturn's slot: never reached (the body sleeps forever)
{
    Trace(-1);
    for (;;)
        MicroThread::MicroThreadSleep();
}

static void __cdecl ThreadBody(void* argument)
{
    int id = (int)(unsigned long)argument;
    unsigned long sum = 17 + id;
    Trace(100 + id);
    for (int k = 0; k < 3; ++k) {
        Trace(200 + id * 10 + k);
        __try {
            MicroThread::MicroThreadSleep();                  // yield with this side's registration on the chain
            sum = sum * 31 + k;
            if ((k + id) & 1)
                RaiseException(0xE0000000 | (id * 16 + k), 0, 0, 0);
            Trace(300 + id * 10 + k);
        }
        __except (1) {
            Trace(400 + id * 10 + k);
        }
    }
    g_sum[id] = sum;
    g_done[id] = 1;
    Trace(500 + id);
    for (;;)
        MicroThread::MicroThreadSleep();
}

static void FirstFrame(MicroThread::SInitialStackFrame* frame, int id)
{
    frame->ExceptionChain = &MicroThread::DummyExceptionBlock;
    frame->Edi = 0;
    frame->Esi = 0;
    frame->Ebp = 0;
    frame->Ebx = 0;
    frame->Entry = ThreadBody;
    frame->Return = ThreadReturnTrap;
    frame->Argument = (void*)(unsigned long)id;
}

static int InRange(unsigned long p, int t)
{
    return p >= (unsigned long)g_lo[t] && p < (unsigned long)g_hi[t];
}

static int g_badFrames;

static void CheckParkedFrame(int t)                           // the 24-byte frame the thread pushed before switching back
{
    unsigned long* frame = (unsigned long*)MicroThread::PGlobalStackPointer;
    if (!InRange((unsigned long)frame, t) || !InRange(frame[3], t) || frame[5] == 0)
        ++g_badFrames;
    // chain head = ThreadBody's own SEH registration (on the thread's stack), whose Next link is the first frame's
    // &DummyExceptionBlock: the chain head popped on the first entry became fs:[0] and came back with this side.
    if (!InRange(frame[0], t) || *(unsigned long*)frame[0] != (unsigned long)&MicroThread::DummyExceptionBlock)
        ++g_badFrames;
}

static unsigned char* g_stackTop;                             // scenario 2: SetStack's top (guard word address)
static unsigned char g_image[kThreads][kImageMax];
static unsigned long g_imageSize[kThreads];

static void SwitchTo(int t, int shared, unsigned char** token)
{
    if (shared) {
        MicroThread::PGlobalStackPointer = g_stackTop - g_imageSize[t];                       // RestoreStack
        memmove(MicroThread::PGlobalStackPointer, g_image[t], g_imageSize[t]);
    }
    else
        MicroThread::PGlobalStackPointer = token[t];
    MicroThread::MicroThreadSleep();
    CheckParkedFrame(t);
    if (shared) {
        g_imageSize[t] = (unsigned long)(g_stackTop - MicroThread::PGlobalStackPointer);      // PreserveStack
        if (g_imageSize[t] > kImageMax)
            g_imageSize[t] = kImageMax, ++g_badFrames;
        memmove(g_image[t], MicroThread::PGlobalStackPointer, g_imageSize[t]);
    }
    else
        token[t] = MicroThread::PGlobalStackPointer;
}

static int RunScenario(unsigned char* stack, int shared)
{
    static const int kExpected[24] = {
        100, 200, 600, 101, 210, 601, 300, 201, 710, 410, 211, 711,
        401, 202, 620, 311, 212, 621, 302, 500, 730, 412, 501, 731 };
    unsigned char* token[kThreads];
    unsigned long schedulerSum = 5;
    g_count = 0;
    g_badFrames = 0;
    for (int t = 0; t < kThreads; ++t) {
        MicroThread::SInitialStackFrame* frame;
        g_done[t] = 0;
        g_sum[t] = 0;
        if (shared) {
            g_lo[t] = stack;
            g_hi[t] = g_stackTop;
            frame = (MicroThread::SInitialStackFrame*)g_image[t];
            g_imageSize[t] = sizeof(MicroThread::SInitialStackFrame);
        }
        else {
            g_lo[t] = stack + t * kRegion;
            g_hi[t] = g_lo[t] + kRegion;
            frame = (MicroThread::SInitialStackFrame*)(g_hi[t] - 16) - 1;
        }
        FirstFrame(frame, t);
        token[t] = (unsigned char*)frame;
    }
    for (int round = 0; round < 8 && !(g_done[0] && g_done[1]); ++round) {
        for (int t = 0; t < kThreads; ++t) {
            if (g_done[t])
                continue;
            __try {
                SwitchTo(t, shared, token);
                schedulerSum = schedulerSum * 7 + round * 2 + t;
                if (round & 1)
                    RaiseException(0xE0001000 | round, 0, 0, 0);
                Trace(600 + round * 10 + t);
            }
            __except (1) {
                Trace(700 + round * 10 + t);
            }
        }
    }
    int bad = g_badFrames;
    if (g_count != 24)
        ++bad;
    for (int i = 0; i < 24 && i < g_count; ++i)
        if (g_trace[i] != kExpected[i])
            ++bad;
    for (int t = 0; t < kThreads; ++t)
        if (g_sum[t] != (((17ul + t) * 31 + 0) * 31 + 1) * 31 + 2)
            ++bad;
    unsigned long want = 5;
    for (int r = 0; r < 4; ++r)
        for (int t = 0; t < kThreads; ++t)
            want = want * 7 + r * 2 + t;
    if (schedulerSum != want)
        ++bad;
    return bad;
}

int main()
{
    unsigned char stack[kThreads * kRegion];                  // on the real stack: SEH accepts registrations only within its bounds
    memset(stack, 0, sizeof(stack));
    int bad1 = RunScenario(stack, 0);
    g_stackTop = stack + sizeof(stack) - 4;                   // SetStack(stack end, size): guard word below the end
    *(unsigned long*)g_stackTop = 0xCDCDCDCD;
    int bad2 = RunScenario(stack, 1);
    if (*(unsigned long*)g_stackTop != 0xCDCDCDCD)
        ++bad2;
    printf("separate stacks: %d bad, shared stack + image copy: %d bad\n", bad1, bad2);
    if (bad1 == 0 && bad2 == 0)
        printf("009d8650_TEST PASS\n");
    else
        printf("009d8650_TEST FAIL\n");
    return 0;
}