// Behaviour test for 0x00686200 CPersistContext_TransferableComponent<COpinionTransientOffset>::TransferIn:
// the hook builds a default (all-zero) traits object and hands it the context, the parser and the
// component's target pointer.
#include <cstdio>

struct CPersistContext { int id; };
struct CStringParser { int id; };
struct CCharString { int id; };

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

struct CPersistContext_TransferableComponentGenericType {
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, TransferableValue);
}

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext&, CCharString&) { }

static int g_calls = 0, g_defaultsZero = 0;
static CPersistContext* g_ctx = 0; static CStringParser* g_out = 0; static COpinionTransientOffset* g_value = 0;

template <>
void CPersistTraits<COpinionTransientOffset>::TransferIn(CPersistContext& context, CStringParser& out, COpinionTransientOffset* value)
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
    CPersistContext context; CStringParser out;
    generic->TransferIn(context, out);
    if (g_calls != 1 || g_ctx != &context || g_out != &out || g_value != &target || !g_defaultsZero || target.Opinion != 3) {
        std::printf("00686200_TEST FAIL\n"); return 1;
    }
    std::printf("00686200_TEST PASS\n");
    return 0;
}