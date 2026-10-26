// CPersistContext_TransferableComponent<CRGBColour>::TransferIn(CPersistContext& context, CStringParser& parser) -- parses the colour through a CPersistTraits<CRGBColour> whose DefaultVal is opaque black
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006421C0, 46 bytes, `ret 8`. PDB:
// ?TransferIn@?$CPersistContext_TransferableComponent@VCRGBColour@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z.
// Callee: 0x009E8C10 CPersistTraits<CRGBColour>::TransferIn(CPersistContext&, CStringParser&, CRGBColour*) (thiscall,
// `ret 0Ch`); its manifest label `CPersistTraits<NInventory::CItem>::TransferIn` is propagated.
// Persist READ hook (vtable slot 1) of the component wrapper { vptr; T* TransferableValue; }. The traits object
// (PDB: { CRGBColour DefaultVal; }, CRGBColour = { uchar B, G, R, A } at +0..+3) lives in the `push ecx` frame slot;
// its inline default constructor stores R, G, B = 0 (`xor al,al` ; +2, +1, +0) and A = 0xFF, which VC schedules
// after the argument pushes (`mov byte ptr [esp+0Fh],0FFh`).
// Shape: the constructor body assigns A first, then R, G, B: that source order gives retail's +2/+1/+0 byte stores
// and the late alpha store; the B, G, R, A (member order) and R, G, B, A forms store +0/+1/+2.
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
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value);   // 0x009E8C10 for CRGBColour
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
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, TransferableValue);
}

template void CPersistContext_TransferableComponent<CRGBColour>::TransferIn(CPersistContext& context, CStringParser& parser);