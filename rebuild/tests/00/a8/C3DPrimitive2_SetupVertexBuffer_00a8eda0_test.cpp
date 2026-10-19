// C3DPrimitive2::SetupVertexBuffer(long,long,unsigned long,unsigned long) -- replace the vertex buffer and choose the vertex layout/pool.
// Retail 0x00A8EDA0, 165 bytes. operator new 0x00BFEA1A; CBase ctor 0x0099A2F0;
// CVertexBufferWin32 Initialise 0x00A63150, vtable 0x0129BA20. Model reuses landed ctor 0x00A8E160.
// Flags bit 0 selects 24-byte vertices/pool 2, otherwise 8-byte vertices/pool 1; count is at least one.
struct CBase {
    virtual void VSlot0(unsigned long flags);
    ~CBase(); // 0x0099A300
    CBase();                                    // 0x0099A2F0

};
#include "engine/CVertexBufferWin32.h"   // PDB header: an opaque 0x30-byte flattened base, so the retail
                                            // members (CVertexBufferBase in the PDB) are modelled locally
struct IDirect3DVertexBuffer9;
struct CVertexCompressionParams {               // PDB: PosScale XYZW then PosBias XYZW
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBase {              // 0x34 bytes, PDB member names
    IDirect3DVertexBuffer9*  PVertexBuffer;     // +0x04
    unsigned long            Length;            // +0x08
    long                     Type;              // +0x0c (EVertexType)
    unsigned char            Stride;            // +0x10
    CVertexCompressionParams CompressionParams; // +0x14
    CVertexBufferBase() : Length(0), Type(0), CompressionParams()
    {

    }
};
struct CVertexBufferWin32_Retail : CVertexBufferBase {

    virtual void VSlot1();
    __forceinline CVertexBufferWin32_Retail() : CVertexBufferBase() { PVertexBuffer = 0; Stride = 0; }
    CVertexBufferWin32_Retail(long type, long a, long b, unsigned long c, long pool);
    bool Initialise(long type, long a, long b, unsigned long c, long pool);   // 0x00A63150
};
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32_Retail) == sizeof(CVertexBufferWin32));


struct IScalarDeleteView { virtual void Destroy(unsigned long); };
struct CVertexBufferOwner {
    CVertexBufferWin32_Retail* Pointer;
    void reset(CVertexBufferWin32_Retail* p) {
        CVertexBufferWin32_Retail* old = Pointer;
        Pointer = p;
        if (old) ((IScalarDeleteView*)old)->Destroy(1);
    }
};
struct C3DPrimitive2 {
    unsigned char _beforeVertexBuffer[0x30];
    CVertexBufferOwner VertexBuffer;
    bool SetupVertexBuffer(long first, long second, unsigned long count, unsigned long flags);
};
bool C3DPrimitive2::SetupVertexBuffer(long first, long second, unsigned long count, unsigned long flags)
{
    VertexBuffer.reset(new CVertexBufferWin32_Retail());
    CVertexBufferWin32_Retail* buffer = VertexBuffer.Pointer;
    unsigned long stride = 8;
    if (count < 1) count = 1;
    long pool = 1;
    if (flags & 1) { stride = 24; pool = 2; }
    return buffer->Initialise(first, count, second, stride, pool);
}


#include <cstdio>
static long args[5];static int frees;static bool initResult=true;static bool defaultsCorrect=true;
CBase::CBase() {}
CBase::~CBase() {}
void CBase::VSlot0(unsigned long flags) { ++frees;if(flags&1)delete (CVertexBufferWin32_Retail*)this; }
void CVertexBufferWin32_Retail::VSlot1() {}
bool CVertexBufferWin32_Retail::Initialise(long first,long count,long second,unsigned long stride,long pool) {
 defaultsCorrect=defaultsCorrect && !PVertexBuffer && Length==0 && Type==0 && Stride==0 &&
  CompressionParams.PosScaleX==1 && CompressionParams.PosScaleY==1 && CompressionParams.PosScaleZ==1 && CompressionParams.PosScaleW==1 &&
  CompressionParams.PosBiasX==0 && CompressionParams.PosBiasY==0 && CompressionParams.PosBiasZ==0 && CompressionParams.PosBiasW==0;
 args[0]=first;args[1]=count;args[2]=second;args[3]=stride;args[4]=pool;return initResult;
}
int main() {
 C3DPrimitive2 primitive;primitive.VertexBuffer.Pointer=0;
 if(!primitive.SetupVertexBuffer(7,11,0,0) || args[0]!=7 || args[1]!=1 || args[2]!=11 || args[3]!=8 || args[4]!=1 || frees) return 1;
 if(!primitive.SetupVertexBuffer(13,17,3,1) || args[1]!=3 || args[3]!=24 || args[4]!=2 || frees!=1) return 2;
 initResult=false;
 if(primitive.SetupVertexBuffer(19,23,1,2) || args[0]!=19 || args[2]!=23 || args[3]!=8 || args[4]!=1 || frees!=2) return 3;
 if(!defaultsCorrect)return 4;
 primitive.VertexBuffer.reset(0);if(frees!=3)return 5;
 std::printf("00a8eda0_TEST PASS\n");return 0;
}