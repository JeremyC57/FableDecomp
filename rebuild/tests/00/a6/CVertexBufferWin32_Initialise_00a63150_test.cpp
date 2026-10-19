#include <stdio.h>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define FABLE_ENGINE_CVERTEXBUFFERWIN32_H
struct CVertexBufferWin32 { void* __vftable; unsigned char _base_0x04[0x30]; };
#endif
// CVertexBufferWin32::Initialise(long,long,long,unsigned long,long) -- create D3D buffer, record its format and reset vertex decoding
// Retail 0x00A63150,165 bytes. GetSystem009A4EC0; IDirect3DDevice9::CreateVertexBuffer slot26.
// Canonical header has only an opaque52-byte base. Landed Lock/Uninitialise confirm buffer+4,size+8;
// this body proves type+c,stride+10,32-byte scale/offset record+14, matching total size0x34.
#include "engine/CVertexBufferWin32.h"
struct D3DVertexBuffer;
struct D3DVertexBufferDevice;
struct D3DVertexBufferDeviceMethods {
    void* Slots0[26];
    long (__stdcall *CreateVertexBuffer)(D3DVertexBufferDevice*, unsigned long, unsigned long, unsigned long,
        long, D3DVertexBuffer**, void*);
};
struct D3DVertexBufferDevice { D3DVertexBufferDeviceMethods* Methods; };
struct CBufferDisplayView { unsigned char BeforeDevice[0x58]; D3DVertexBufferDevice* D3DDevice; };
struct CBufferSystemView { unsigned char BeforeDisplay[0x60]; CBufferDisplayView* PDisplayManager; };
CBufferSystemView* __fastcall GetBufferSystemManager(); // 0x009A4EC0
struct CVertexDecodeScaleOffset {
    float ScaleX, ScaleY, ScaleZ, ScaleW;
    float OffsetX, OffsetY, OffsetZ, OffsetW;
    CVertexDecodeScaleOffset() : ScaleX(1), ScaleY(1), ScaleZ(1), ScaleW(1),
        OffsetX(0), OffsetY(0), OffsetZ(0), OffsetW(0) {}
};
struct CVertexBufferInitialiseView {
    void* VTable;
    D3DVertexBuffer* Buffer;
    unsigned long Size;
    long VertexType;
    unsigned char VertexStride;
    unsigned char BeforeDecode[3];
    CVertexDecodeScaleOffset Decode;
    bool Initialise(long type, long vertexCount, long stride, unsigned long usage, long pool);
};
bool CVertexBufferInitialiseView::Initialise(long type, long vertexCount, long stride, unsigned long usage, long pool)
{
    D3DVertexBufferDevice* device = GetBufferSystemManager()->PDisplayManager->D3DDevice;
    long size = vertexCount * stride;
    long result = device->Methods->CreateVertexBuffer(device, size, usage, type, pool, &Buffer, 0);
    Size = size;
    VertexStride = (unsigned char)stride;
    VertexType = type;
    Decode = CVertexDecodeScaleOffset();
    return result >= 0;
}

struct D3DVertexBuffer { int token; };
static D3DVertexBufferDevice device;
static D3DVertexBufferDeviceMethods methods;
static CBufferDisplayView display;
static CBufferSystemView systemManager;
static D3DVertexBuffer outputBuffer, oldBuffer;
static CVertexBufferInitialiseView* active;
static long wantedType,wantedStride,wantedCount,wantedPool,wantedResult;
static unsigned long wantedUsage;
static int bad,systemCalls,createCalls;
CBufferSystemView* __fastcall GetBufferSystemManager()
{
    ++systemCalls;
    return &systemManager;
}
static long __stdcall createBuffer(D3DVertexBufferDevice* self,unsigned long size,unsigned long usage,
    unsigned long type,long pool,D3DVertexBuffer** output,void* shared)
{
    ++createCalls;
    if (self!=&device || systemCalls!=1 || size!=(unsigned long)(wantedCount*wantedStride) ||
        usage!=wantedUsage || type!=(unsigned long)wantedType || pool!=wantedPool ||
        output!=&active->Buffer || shared || active->Size!=99 || active->VertexType!=123 ||
        active->VertexStride!=7 || active->Decode.ScaleX!=17 || active->Decode.OffsetW!=23) bad=1;
    if (wantedResult>=0) *output=&outputBuffer;
    return wantedResult;
}
static int run(long count,long stride,long type,unsigned long usage,long pool,long result)
{
    wantedCount=count;wantedStride=stride;wantedType=type;wantedUsage=usage;wantedPool=pool;wantedResult=result;
    bad=systemCalls=createCalls=0;
    CVertexBufferInitialiseView buffer;
    buffer.VTable=&methods;
    buffer.Buffer=&oldBuffer;
    buffer.Size=99;buffer.VertexType=123;buffer.VertexStride=7;
    buffer.BeforeDecode[0]=3;buffer.BeforeDecode[1]=5;buffer.BeforeDecode[2]=9;
    buffer.Decode.ScaleX=17;buffer.Decode.OffsetW=23;
    active=&buffer;
    bool success=buffer.Initialise(type,count,stride,usage,pool);
    if (bad || systemCalls!=1 || createCalls!=1 || success!=(result>=0) ||
        buffer.VTable!=&methods || buffer.Buffer!=(result>=0 ? &outputBuffer : &oldBuffer) ||
        buffer.Size!=(unsigned long)(count*stride) || buffer.VertexType!=type ||
        buffer.VertexStride!=(unsigned char)stride || buffer.BeforeDecode[0]!=3 ||
        buffer.BeforeDecode[1]!=5 || buffer.BeforeDecode[2]!=9) return 0;
    return buffer.Decode.ScaleX==1 && buffer.Decode.ScaleY==1 && buffer.Decode.ScaleZ==1 && buffer.Decode.ScaleW==1 &&
        buffer.Decode.OffsetX==0 && buffer.Decode.OffsetY==0 && buffer.Decode.OffsetZ==0 && buffer.Decode.OffsetW==0;
}
int main()
{
    methods.CreateVertexBuffer=createBuffer;
    device.Methods=&methods;
    display.D3DDevice=&device;
    systemManager.PDisplayManager=&display;
    if (!run(5,32,0x142,0x200,1,0) || !run(4,300,0x104,0x80000000UL,2,1) ||
        !run(0,16,0,0,0,-1) || !run(3,12,0x42,7,3,(-2147483647L-1))) {
        printf("00a63150_TEST FAIL\n");return 1;
    }
    printf("00a63150_TEST PASS\n");return 0;
}