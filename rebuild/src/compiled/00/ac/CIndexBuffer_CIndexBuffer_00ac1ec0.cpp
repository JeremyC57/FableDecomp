// CIndexBuffer::CIndexBuffer() -- store the vtable and clear PAllocatedBuffer (+0x10) then PD3DIndexBuffer (+0x04)
// Leaf: ??0CIndexBuffer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ac1ec0, 17 bytes, thiscall, plain `ret`.
// Identity (proven): it stores vtable 0x0129EF2C (RTTI .?AVCIndexBuffer@@) and clears Ego_r.pdb CIndexBuffer's PAllocatedBuffer and PD3DIndexBuffer; bound ??0CIndexBuffer@@QAE@XZ (and the stand-in ??0CBase_00ac1ec0@@QAE@XZ); callers C3DPrimitive2::UploadToIndexBuffer 0x00A8F9E0, ::CreateDegenerateTriangleIndexBuffer 0x00A90260, ::LoadBinary 0x00A92900, CEngineSkyRenderer::CEngineSkyRenderer 0x00B625E0 and 0x00B52250.
// Shape (family L114): `mov eax,ecx ; xor ecx,ecx`, the vtable 0x0129EF2C, two zero stores, `this` returned.
struct IDirect3DIndexBuffer9;
enum EPrimitiveType { };
class CIndexBuffer {
public:
    CIndexBuffer();
    virtual ~CIndexBuffer();
    IDirect3DIndexBuffer9* PD3DIndexBuffer;                // +0x04
    unsigned long          ByteLength;                     // +0x08
    EPrimitiveType         PrimitiveType;                  // +0x0C
    void*                  PAllocatedBuffer;               // +0x10
};
CIndexBuffer::CIndexBuffer()
{
    PAllocatedBuffer = 0;
    PD3DIndexBuffer = 0;
}