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