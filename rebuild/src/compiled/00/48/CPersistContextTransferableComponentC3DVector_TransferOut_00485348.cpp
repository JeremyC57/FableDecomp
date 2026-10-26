// CPersistContext_TransferableComponent<C3DVector>::TransferOut(CPersistContext& context, CCharString& out) -- formats the vector into the output string through a CPersistTraits<C3DVector> whose DefaultVal is the zero vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485348, length 42 bytes.
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent
// Callee: 0x009E7F90 CPersistTraits<C3DVector>::TransferOut(CPersistContext&, CCharString&, const C3DVector*) (thiscall,
// `ret 0Ch`; manifest-named).
// Label correction: the manifest calls this "CPersistContext_TransferableComponent<float>::TransferIn". It is slot 0
// (the persist WRITE hook) of the component vtable 0x01236750, whose slot 1 0x0048531E is the matching TransferIn
// calling CPersistTraits<C3DVector>::TransferIn 0x009E84D0; the traits default is three zero floats.
// Shape: size-optimised module with frame pointers (`#pragma optimize("s", on)` plus `optimize("y", off)`: `push ebp ; mov ebp,esp`, `push
// dword ptr` argument pass-through, `leave`); the traits' 12-byte DefaultVal is built by the C3DVector(x, y, z)
// constructor as three `fldz ; fstp` stores interleaved with the argument pushes (the model of the landed
// CPersistContext_TransferableComponent<CRGBColour> rows 0x006421C0 / 0x006421F0).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CPersistContext;
class CStringParser;
class CCharString;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<C3DVector> {
    C3DVector DefaultVal;
    CPersistTraits() : DefaultVal(0.0f, 0.0f, 0.0f) {}
    void TransferOut(CPersistContext& context, CCharString& out, const C3DVector* value);   // 0x009E7F90
    void TransferIn(CPersistContext& context, CStringParser& parser, C3DVector* value);     // 0x009E84D0
};

class CPersistContext_TransferableComponentGenericType { public:
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

template void CPersistContext_TransferableComponent<C3DVector>::TransferOut(CPersistContext& context, CCharString& out);