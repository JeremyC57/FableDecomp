// CVertexBufferBase::CVertexBufferBase() -- CBaseClassNonCopyable(), no D3D buffer, zero length/type/stride, own vtable, identity compression params (scale 1, bias 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E27E0, length 61 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (manifest label "CBase").
// Vtable 0x0129BA08 = CVertexBufferBase (rtti-vtables.tsv); the manifest's "CCompressedMemory
// DataInputStream" label is propagated. Layout = Ego_r.pdb CVertexBufferBase (52 bytes):
// PVertexBuffer +4, Length +8, Type +0xc, Stride +0x10 (uchar), CompressionParams +0x14
// (CVertexCompressionParams: PosScale XYZW +0x14..+0x20, PosBias XYZW +0x24..+0x30). The four
// init-list scalars are stored before the vtable; the params member has an inline constructor
// (bias zeroes share the zero register, scales are the 1.0f immediate loaded once), so its stores
// sink below the vtable store.
struct IDirect3DVertexBuffer9;

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;           // +0x00..+0x0c
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;               // +0x10..+0x1c
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f)
        , PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f)
    {
    }
};

struct CVertexBufferBase : CBaseClassNonCopyable {
    IDirect3DVertexBuffer9*  PVertexBuffer;                     // +0x04
    unsigned long            Length;                            // +0x08
    long                     Type;                              // +0x0c (EVertexType)
    unsigned char            Stride;                            // +0x10
    CVertexCompressionParams CompressionParams;                 // +0x14
    virtual void v00();                                         // own vtable 0x0129BA08
    CVertexBufferBase();
};

CVertexBufferBase::CVertexBufferBase()
    : CBaseClassNonCopyable()
    , PVertexBuffer(0)
    , Length(0)
    , Type(0)
    , Stride(0)
    , CompressionParams()
{
}