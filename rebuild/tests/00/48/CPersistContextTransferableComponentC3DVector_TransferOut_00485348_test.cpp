// Behaviour test for 00485348 CPersistContext_TransferableComponent<C3DVector>::TransferOut (copy of the model; the traits
// TransferOut is a stand-in that records its arguments and the default it was built with).
#include <cstdio>
struct CPersistContext { int id; };
struct CStringParser { int id; };
struct CCharString { int len; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
static const void* g_ctx = 0; static const void* g_arg = 0; static const void* g_val = 0;
static float g_def[3] = { 1, 1, 1 };
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<C3DVector> {
    C3DVector DefaultVal;
    CPersistTraits() : DefaultVal(0.0f, 0.0f, 0.0f) {}
    void TransferOut(CPersistContext& context, CCharString& out, C3DVector* value)
    { g_ctx = &context; g_arg = &out; g_val = value; g_def[0] = DefaultVal.X; g_def[1] = DefaultVal.Y; g_def[2] = DefaultVal.Z; }
};
template <class T> struct CPersistContext_TransferableComponent {
    virtual ~CPersistContext_TransferableComponent() {}
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
    C3DVector v(1.5f, 2.5f, 3.5f);
    CPersistContext_TransferableComponent<C3DVector> comp; comp.TransferableValue = &v;
    CPersistContext ctx; CCharString arg;
    comp.TransferOut(ctx, arg);
    int ok = 1;
    if (g_ctx != &ctx || g_arg != &arg || g_val != &v) ok = 0;
    if (g_def[0] != 0.0f || g_def[1] != 0.0f || g_def[2] != 0.0f) ok = 0;
    if (!ok) { std::printf("00485348_TEST FAIL\n"); return 1; }
    std::printf("00485348_TEST PASS\n");
    return 0;
}