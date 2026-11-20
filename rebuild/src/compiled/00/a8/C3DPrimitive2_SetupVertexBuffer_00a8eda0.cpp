// C3DPrimitive2::SetupVertexBuffer(EVertexType,unsigned long,unsigned long,unsigned long) -- replace the vertex buffer and choose the vertex layout/pool.
// Retail 0x00A8EDA0, 165 bytes. operator new 0x00BFEA1A; CBase ctor 0x0099A2F0;
// CVertexBufferWin32 Initialise 0x00A63150, vtable 0x0129BA20. Model reuses landed ctor 0x00A8E160.
// Flags bit 0 selects 24-byte vertices/pool 2, otherwise 8-byte vertices/pool 1; count is at least one.
// Leaf: ?SetupVertexBuffer@C3DPrimitive2@@AAE_NW4EVertexType@@KKK@Z
// NF192 (q6242): SetupVertexBuffer is private with an EVertexType first parameter (Ego_r.pdb 0x00BD15B0 / FableWin.pdb 0x032F8CD0
//   ?SetupVertexBuffer@C3DPrimitive2@@AAE_NW4EVertexType@@KKK@Z; engine/C3DPrimitive2.h _LOADER declares it so): respelled in place in the
//   row-local C3DPrimitive2 model (was the public `long` form ?SetupVertexBuffer@C3DPrimitive2@@QAE_NJKKK@Z).
#include "fable_compiler.h"
// NF149 (q4552 / q6203 family): the buffer is the shared retail CVertexBufferWin32 (engine/CVertexBufferWin32.h: the inline default
// constructor, the virtual destructor whose deleting call `push 1 ; call [vtbl]` the reset makes, Initialise 0x00A63150 with the PDB
// parameter types); was the stand-in CVertexBufferWin32_Retail on local CBase / CVertexCompressionParams / CVertexBufferBase models
// (binding ?Initialise@CVertexBufferWin32_Retail@@QAE_NJJJKJ@Z and ??_7CVertexBufferWin32_Retail@@6B@) and an IScalarDeleteView cast.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CVertexBufferWin32.h"
struct CVertexBufferOwner {
    CVertexBufferWin32* Pointer;
    void reset(CVertexBufferWin32* p) {
        CVertexBufferWin32* old = Pointer;
        Pointer = p;
        if (old) delete old;
    }
};
class C3DPrimitive2 { public:
    unsigned char _beforeVertexBuffer[0x30];
    CVertexBufferOwner VertexBuffer;
private:
    bool SetupVertexBuffer(EVertexType type, unsigned long size, unsigned long count, unsigned long flags);   // 0x00A8EDA0
};
bool C3DPrimitive2::SetupVertexBuffer(EVertexType type, unsigned long size, unsigned long count, unsigned long flags)
{
    VertexBuffer.reset(new CVertexBufferWin32());
    CVertexBufferWin32* buffer = VertexBuffer.Pointer;
    unsigned long stride = 8;
    if (count < 1) count = 1;
    long pool = 1;
    if (flags & 1) { stride = 24; pool = 2; }
    return buffer->Initialise(type, count, size, stride, (EVertexBufferPool)pool);
}