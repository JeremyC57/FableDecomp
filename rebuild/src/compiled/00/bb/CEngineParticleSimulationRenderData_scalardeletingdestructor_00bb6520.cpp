// CEngineParticleSimulationRenderData::`scalar deleting destructor'(unsigned int) -- the implicit destructor (the two Win32 vertex buffers destroyed through the vector destructor iterator, then ~CBaseClassNonCopyable), then free the object on flag bit 0
// Leaf: ??_GCEngineParticleSimulationRenderData@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB6520, 50 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of vtable 0x012A56BC (RTTI .?AVCEngineParticleSimulationRenderData@@ :
//   CParticleSimulationRenderData : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb ??_GCEngineParticleSimulationRenderData@@UAEPAXI@Z.
//   The catalog label "vfunc_0" is a placeholder.
// Callees: `vector destructor iterator' 0x00401400 (??_I, __stdcall: array, 0x34, 2, dtor), CVertexBufferWin32::~CVertexBufferWin32
//   0x00B564C0 (stores vtable 0x0129BA20 .?AVCVertexBufferWin32@@ first; its other caller is the CEngineDynamic3DResource
//   ??_G 0x00BF3490 -- the catalog's "CEngineDynamic3DResource<...>::~" label is propagated), CBaseClassNonCopyable::~CBaseClassNonCopyable
//   0x0099A300 (protected virtual, Ego_r.pdb MAE), operator delete 0x00BFE9BC.
// Layout (Ego_r.pdb): VertexBuffers CVertexBufferWin32[2] at +4 (0x34 each), CurrentVB +0x6C (0x70 bytes).
// Shape: no user destructor (no vptr store); the vtable and this ??_G are emitted by the class's constructor, written as
//   the landed byte-exact 0x00BB63A0 (inline CVertexBufferWin32 default constructor, CurrentVB = 0).
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

class CBaseClass {
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual ~CBaseClass();                           // 0x0099A2E0
};
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

class CParticleSimulationRenderData : public CBaseClassNonCopyable {
public:
    virtual bool IsBusy() const = 0;
};

class CEngineParticleSimulationRenderData : public CParticleSimulationRenderData {
public:
    CEngineParticleSimulationRenderData();
    virtual bool IsBusy() const;
    CVertexBufferWin32 VertexBuffers[2];             // +0x04
    long CurrentVB;                                  // +0x6C
};

CEngineParticleSimulationRenderData::CEngineParticleSimulationRenderData()
    : CurrentVB(0)
{
}