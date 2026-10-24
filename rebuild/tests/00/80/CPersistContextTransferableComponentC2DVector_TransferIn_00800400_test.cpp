// Behaviour test for 0x00800400 CPersistContext_TransferableComponent<C2DVector>::TransferIn:
// the hook builds a default traits object (DefaultVal = (0, 0)) and hands it the context, the parser and the
// component's target pointer.
#include <cstdio>

struct CPersistContext { int id; };
struct CStringParser { int id; };
struct CCharString { int id; };

class C2DVector {
public:
    float X;
    float Y;
    C2DVector() : X(0.0f), Y(0.0f) {}
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
static CPersistContext* g_ctx = 0; static CStringParser* g_out = 0; static C2DVector* g_value = 0;

template <>
void CPersistTraits<C2DVector>::TransferIn(CPersistContext& context, CStringParser& out, C2DVector* value)
{
    ++g_calls; g_ctx = &context; g_out = &out; g_value = value;
    g_defaultsZero = DefaultVal.X == 0.0f && DefaultVal.Y == 0.0f;
}

int main()
{
    C2DVector target;
    target.X = 3.0f; target.Y = 9.0f;
    CPersistContext_TransferableComponent<C2DVector> component;
    component.TransferableValue = &target;
    CPersistContext_TransferableComponentGenericType* generic = &component;
    CPersistContext context; CStringParser out;
    generic->TransferIn(context, out);
    if (g_calls != 1 || g_ctx != &context || g_out != &out || g_value != &target || !g_defaultsZero || target.X != 3.0f) {
        std::printf("00800400_TEST FAIL\n"); return 1;
    }
    std::printf("00800400_TEST PASS\n");
    return 0;
}