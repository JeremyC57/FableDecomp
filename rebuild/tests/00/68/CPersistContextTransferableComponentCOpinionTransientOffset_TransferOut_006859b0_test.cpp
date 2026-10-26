// Behaviour test for 0x006859B0 CPersistContext_TransferableComponent<COpinionTransientOffset>::TransferOut:
// the hook builds a default (all-zero) traits object and hands it the context, the output string and the
// component's target pointer.
#include <cstdio>

class CPersistContext { public: int id; };
class CStringParser { public: int id; };
class CCharString { public: int id; };

typedef long EOpinion;
class COpinionTransientOffset {
public:
    EOpinion Opinion;
    float    OffsetPerFrameRunIn;
    long     FramesToRunIn;
    long     FramesOfCappedPeak;
    float    OffsetPerFrameRunOut;
    long     FramesToRunOut;
    COpinionTransientOffset()
    {
        Opinion = 0;
        OffsetPerFrameRunIn = 0;
        FramesToRunIn = 0;
        OffsetPerFrameRunOut = 0;
        FramesToRunOut = 0;
        FramesOfCappedPeak = 0;
    }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, T* value);
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value);
};

class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, TransferableValue);
}

template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext&, CStringParser&) { }

static int g_calls = 0, g_defaultsZero = 0;
static CPersistContext* g_ctx = 0; static CCharString* g_out = 0; static COpinionTransientOffset* g_value = 0;

template <>
void CPersistTraits<COpinionTransientOffset>::TransferOut(CPersistContext& context, CCharString& out, COpinionTransientOffset* value)
{
    ++g_calls; g_ctx = &context; g_out = &out; g_value = value;
    g_defaultsZero = DefaultVal.Opinion == 0 && DefaultVal.OffsetPerFrameRunIn == 0.0f && DefaultVal.FramesToRunIn == 0 &&
                     DefaultVal.FramesOfCappedPeak == 0 && DefaultVal.OffsetPerFrameRunOut == 0.0f && DefaultVal.FramesToRunOut == 0;
}

int main()
{
    COpinionTransientOffset target;
    target.Opinion = 3; target.FramesToRunIn = 9;
    CPersistContext_TransferableComponent<COpinionTransientOffset> component;
    component.TransferableValue = &target;
    CPersistContext_TransferableComponentGenericType* generic = &component;
    CPersistContext context; CCharString out;
    generic->TransferOut(context, out);
    if (g_calls != 1 || g_ctx != &context || g_out != &out || g_value != &target || !g_defaultsZero || target.Opinion != 3) {
        std::printf("006859b0_TEST FAIL\n"); return 1;
    }
    std::printf("006859b0_TEST PASS\n");
    return 0;
}