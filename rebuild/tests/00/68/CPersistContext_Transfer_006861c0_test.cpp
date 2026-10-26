// Behaviour test for 006861c0: CPersistContext::Transfer(name, COpinionTransientOffset&) forwards name and value to the three-argument Transfer with an all-zero default
#include <cstdio>

enum EOpinion { OPINION_NULL = 0, OPINION_FEAR = 3 };

class COpinionTransientOffset { public:
    EOpinion Opinion;
    float    OffsetPerFrameRunIn;
    long     FramesToRunIn;
    long     FramesOfCappedPeak;
    float    OffsetPerFrameRunOut;
    long     FramesToRunOut;
    COpinionTransientOffset()
        : Opinion(OPINION_NULL), OffsetPerFrameRunIn(0.0f), FramesToRunIn(0), FramesOfCappedPeak(0),
          OffsetPerFrameRunOut(0.0f), FramesToRunOut(0) {}
};

static const char* g_name = 0;
static COpinionTransientOffset* g_value = 0;
static bool g_defaultZero = false;
static int g_calls = 0;

class CPersistContext { public:
    long Mode;
    void Transfer(const char* name, COpinionTransientOffset& value, const COpinionTransientOffset& defaultValue)   // retail 0x00685F80 (stub: record)
    {
        ++g_calls;
        g_name = name;
        g_value = &value;
        g_defaultZero = defaultValue.Opinion == OPINION_NULL && defaultValue.OffsetPerFrameRunIn == 0.0f
                     && defaultValue.FramesToRunIn == 0 && defaultValue.FramesOfCappedPeak == 0
                     && defaultValue.OffsetPerFrameRunOut == 0.0f && defaultValue.FramesToRunOut == 0
                     && &defaultValue != &value;
        if (Mode == 4)
            value = defaultValue;
    }
    void Transfer(const char* name, COpinionTransientOffset& value)
    {
        Transfer(name, value, COpinionTransientOffset());
    }
};

int main()
{
    CPersistContext ctx;
    ctx.Mode = 1;
    COpinionTransientOffset v;
    v.Opinion = OPINION_FEAR; v.FramesToRunIn = 12; v.OffsetPerFrameRunOut = 2.5f;
    ctx.Transfer("TransientOffset", v);
    bool ok = g_calls == 1 && g_name != 0 && g_name[0] == 'T' && g_value == &v && g_defaultZero && v.FramesToRunIn == 12;
    ctx.Mode = 4;                                        // reset mode copies the zero default into the value
    ctx.Transfer("TransientOffset", v);
    ok = ok && g_calls == 2 && v.Opinion == OPINION_NULL && v.FramesToRunIn == 0 && v.OffsetPerFrameRunOut == 0.0f;
    if (!ok) { std::printf("006861c0_TEST FAIL\n"); return 1; }
    std::printf("006861c0_TEST PASS\n");
    return 0;
}