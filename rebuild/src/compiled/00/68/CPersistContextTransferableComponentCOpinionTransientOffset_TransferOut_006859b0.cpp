// CPersistContext_TransferableComponent<COpinionTransientOffset>::TransferOut(CPersistContext& context, CCharString& out) -- writes the transient opinion offset as text through a default CPersistTraits<COpinionTransientOffset>
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006859B0, 57 bytes (+7 int3 padding), `ret 8`.
// Callee: 0x006859F0 CPersistTraits<COpinionTransientOffset>::TransferOut(CPersistContext&, CCharString&,
// COpinionTransientOffset*) (thiscall, `ret 0Ch`, landed).
// Persist WRITE hook (vtable 0x0125BAB0 slot 0; slot 1 is TransferIn 0x00686200) of the component wrapper
// { vptr; T* TransferableValue; } (PDB CPersistContext_TransferableComponent<COpinionTransientOffset>, 8 bytes).
// The manifest label `CPersistContext_TransferableComponent<float>::TransferIn` is propagated: the 24-byte
// traits frame and the callee name the type. The traits object (PDB: { COpinionTransientOffset DefaultVal; },
// 24 bytes) lives in the `sub esp,18h` frame; COpinionTransientOffset's inline default constructor zeroes its
// members in the order Opinion, OffsetPerFrameRunIn, FramesToRunIn, OffsetPerFrameRunOut, FramesToRunOut,
// FramesOfCappedPeak (retail stores +0,+4,+8,+10,+14,+0C through one `xor eax,eax`).
class CPersistContext;
class CStringParser;
class CCharString;

typedef long EOpinion;
class COpinionTransientOffset {
public:
    EOpinion Opinion;                              // +0x00
    float    OffsetPerFrameRunIn;                  // +0x04
    long     FramesToRunIn;                        // +0x08
    long     FramesOfCappedPeak;                   // +0x0c
    float    OffsetPerFrameRunOut;                 // +0x10
    long     FramesToRunOut;                       // +0x14
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
    void TransferOut(CPersistContext& context, CCharString& out, T* value);       // 0x006859F0 for COpinionTransientOffset
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value);   // 0x00686240 for COpinionTransientOffset
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

template void CPersistContext_TransferableComponent<COpinionTransientOffset>::TransferOut(CPersistContext& context, CCharString& out);