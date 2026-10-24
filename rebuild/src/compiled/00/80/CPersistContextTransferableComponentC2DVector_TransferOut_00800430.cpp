// CPersistContext_TransferableComponent<C2DVector>::TransferOut(CPersistContext& context, CCharString& out) -- writes a 2D vector as text through a CPersistTraits<C2DVector> whose DefaultVal is (0, 0)
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800430, 48 bytes, `ret 8`.
// Callee: 0x009E7F00 CPersistTraits<C2DVector>::TransferOut(CPersistContext&, CCharString&, C2DVector*) (thiscall,
// `ret 0Ch`; writes "(x,y)").
// Persist WRITE hook (vtable 0x01272208 slot 0; slot 1 is TransferIn 0x00800400) of the component wrapper
// { vptr; T* TransferableValue; } (PDB CPersistContext_TransferableComponent<C2DVector>, 8 bytes). The manifest
// label `CPersistContext_TransferableComponent<float>` is propagated: the 8-byte traits frame and the callee name
// the type. The traits object (PDB: { C2DVector DefaultVal; }) lives in the `sub esp,8` frame; C2DVector's inline
// constructor stores X = Y = 0.0f as immediates, which VC schedules after the argument pushes.
struct CPersistContext;
struct CStringParser;
struct CCharString;

class C2DVector {
public:
    float X;                                           // +0x0
    float Y;                                           // +0x4
    C2DVector() : X(0.0f), Y(0.0f) {}
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, T* value);       // 0x009E7F00 for C2DVector
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value);   // 0x009E81E0 for C2DVector
};

struct CPersistContext_TransferableComponentGenericType {
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;                              // +0x4
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, TransferableValue);
}

template void CPersistContext_TransferableComponent<C2DVector>::TransferOut(CPersistContext& context, CCharString& out);