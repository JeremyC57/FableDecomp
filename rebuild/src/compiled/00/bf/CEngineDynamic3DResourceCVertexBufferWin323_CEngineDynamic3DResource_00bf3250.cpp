// CEngineDynamic3DResource<CVertexBufferWin32,3>::CEngineDynamic3DResource() -- an empty Win32 vertex buffer (no D3D buffer, zero length/type/stride, unit compression scale, zero bias) that is not yet linked into the resource pool
// Leaf: ??0?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF3250, 64 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x012A81A0, whose RTTI type descriptor is .?AV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@
//   (bases CVertexBufferWin32, CVertexBufferBase, CBaseClassNonCopyable, CBaseClass); FableWin.pdb
//   ??0?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@QAE@XZ. The catalog label "CDataInputStream" is propagated.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Layout (Ego_r.pdb): CVertexBufferWin32 0x34 {PVertexBuffer +4, Length +8, Type +0x0C, Stride +0x10, CompressionParams
//   +0x14 (scale XYZW = 1, bias XYZW = 0)}, Key unsigned long[3] +0x34 (left uninitialised), PoolNext +0x40 (0x44 bytes).
// Shape: the inline CVertexBufferWin32 / CVertexBufferBase / CVertexCompressionParams constructors (the landed
//   0x00BB63A0 element model) and PoolNext(0); every intermediate vptr store is dead, so the template's own vtable is
//   stored last.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};

struct IDirect3DVertexBuffer9;

class CVertexCompressionParams {
public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};

class CVertexBufferBase : public CBaseClassNonCopyable {
public:
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
    IDirect3DVertexBuffer9*  PVertexBuffer;          // +0x04
    unsigned long            Length;                 // +0x08
    long                     Type;                   // +0x0C (EVertexType)
    unsigned char            Stride;                 // +0x10
    CVertexCompressionParams CompressionParams;      // +0x14
};

class CVertexBufferWin32 : public CVertexBufferBase {
public:
    CVertexBufferWin32()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
    virtual ~CVertexBufferWin32();                   // 0x00B564C0
};


template <class T, int N> class CEngineDynamic3DResource : public T {
public:
    CEngineDynamic3DResource() : PoolNext(0) { }
    unsigned long Key[N];                            // +0x34
    CEngineDynamic3DResource* PoolNext;              // +0x40
};

template class CEngineDynamic3DResource<CVertexBufferWin32, 3>;