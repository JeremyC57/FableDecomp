// Behaviour test for 006421f0 CPersistContext_TransferableComponent<CRGBColour>::TransferOut (copy of the model; the
// traits TransferOut is a stand-in that records its arguments and the default it was built with).
#include <cstdio>
class CPersistContext { public: int id; };
class CStringParser { public: int id; };
class CCharString { public: int len; };
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() { A = 255; R = 0; G = 0; B = 0; }
};
static const void* g_ctx = 0; static const void* g_out = 0; static const void* g_val = 0;
static CRGBColour g_default;
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T* value)
    { g_ctx = &context; g_out = &out; g_val = value; g_default = DefaultVal; }
};
class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual ~CPersistContext_TransferableComponentGenericType() {}
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;
    virtual void TransferOut(CPersistContext& context, CCharString& out);
};
template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, TransferableValue);
}
int main()
{
    CRGBColour c; c.R = 10; c.G = 20; c.B = 30; c.A = 40;
    CPersistContext_TransferableComponent<CRGBColour> comp; comp.TransferableValue = &c;
    CPersistContext ctx; CCharString out;
    CPersistContext_TransferableComponentGenericType* base = &comp;
    base->TransferOut(ctx, out);
    int ok = 1;
    if (g_ctx != &ctx || g_out != &out || g_val != &c) ok = 0;
    if (g_default.R != 0 || g_default.G != 0 || g_default.B != 0 || g_default.A != 255) ok = 0;
    if (!ok) { std::printf("006421f0_TEST FAIL\n"); return 1; }
    std::printf("006421f0_TEST PASS\n");
    return 0;
}