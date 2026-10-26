// CPersistContext_TransferableComponent<CRGBColour>::TransferOut(CPersistContext& context, CCharString& out) -- formats the colour into the output string through a CPersistTraits<CRGBColour> whose DefaultVal is opaque black
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006421F0, length 46 bytes.
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent
// Callee: 0x009E8110 CPersistTraits<CRGBColour>::TransferOut(CPersistContext&, CCharString&, const CRGBColour*)
// (thiscall, `ret 0Ch`: appends the R, G, B, A bytes with separators to the string).
// Label correction: the manifest calls this "CPersistContext_TransferableComponent<float>::TransferIn". It is slot 0
// (the persist WRITE hook) of the component vtable 0x0125A188 whose slot 1 is the landed
// CPersistContext_TransferableComponent<CRGBColour>::TransferIn 0x006421C0 (PDB-named); the bodies are byte twins
// apart from the callee, and 0x009E8110 is the manifest's CPersistTraits<CRGBColour>::TransferOut.
// Shape: the landed TransferIn's -- the traits object in the `push ecx` frame slot, its inline default constructor
// assigning A first and then R, G, B (`xor al,al` ; +2, +1, +0 stores, the 0xFF alpha store scheduled after the
// argument pushes).
class CPersistContext;
class CStringParser;
class CCharString;

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() { A = 255; R = 0; G = 0; B = 0; }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T* value);   // 0x009E8110 for CRGBColour
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

template void CPersistContext_TransferableComponent<CRGBColour>::TransferOut(CPersistContext& context, CCharString& out);