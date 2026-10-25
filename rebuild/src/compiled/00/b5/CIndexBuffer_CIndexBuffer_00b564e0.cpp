// CIndexBuffer::~CIndexBuffer() -- destroy an engine index buffer: own vtable, Uninitialise() (release the Direct3D index buffer), then the CMovableResource base's vtable
// Leaf: ??1CIndexBuffer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B564E0, 22 bytes, __fastcall, `ret`.
// Identity: FableWin.pdb ??1CIndexBuffer@@UAE@XZ. The first vtable store 0x0129EF2C is RTTI CIndexBuffer, the final one
//   0x0129B860 RTTI CMovableResource (rtti-vtables.tsv). Its only caller is the scalar deleting destructor 0x00BF34B0
//   (vtable 0x012A81B8, CEngineDynamic3DResource<CIndexBuffer,3>). Label correction: the catalog's
//   "~CActionParamCCountedPointer<NUISystem::CComponent>" is propagated.
// Callees: CIndexBuffer::Uninitialise() @ 0x00AC1EE0 (FableWin.pdb ?Uninitialise@CIndexBuffer@@QAEXXZ: release
//   PD3DIndexBuffer through its vtable slot 2 and clear it; the catalog's "CVolume::ReleaseVolume" is propagated).
// Layout: engine/CIndexBuffer.h (Ego_r, 0x14 bytes: vptr, PD3DIndexBuffer +0x04, ByteLength +0x08, PrimitiveType +0x0c,
//   PAllocatedBuffer +0x10) with the vptr modelled as the polymorphic CMovableResource base (engine/CMovableResource.h
//   is just that vptr), so the classes are written out here.
// Shape: a user-declared out-of-line virtual destructor calling the non-virtual Uninitialise on `this` (ecx is still
//   `this` at the call), then the base's inline user destructor stores its vtable.
struct IDirect3DIndexBuffer9;

class CMovableResource {                                 // vtable 0x0129B860
public:
    virtual ~CMovableResource() { }
    virtual void RelocateData(long offset, void* oldBase, void* newBase, long size) = 0;
};

class CIndexBuffer : public CMovableResource {           // vtable 0x0129EF2C
public:
    IDirect3DIndexBuffer9* PD3DIndexBuffer;              // +0x04
    unsigned long          ByteLength;                   // +0x08
    long                   PrimitiveType;                // +0x0c EPrimitiveType
    void*                  PAllocatedBuffer;             // +0x10
    virtual ~CIndexBuffer();
    virtual void RelocateData(long offset, void* oldBase, void* newBase, long size);
    void Uninitialise();                                 // 0x00AC1EE0
};

CIndexBuffer::~CIndexBuffer()
{
    Uninitialise();
}