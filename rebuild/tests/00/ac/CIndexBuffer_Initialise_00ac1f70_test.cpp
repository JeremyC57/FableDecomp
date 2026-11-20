// 00ac1f70 test: CIndexBuffer::Initialise on the shared header, linked against the row object (CreateIndexBuffer through the manager chain).
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CIndexBuffer.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"

#include "engine/Direct3DSurface9.h"
// IDirect3DIndexBuffer9 (DirectX 9 SDK d3d9.h, DECLARE_INTERFACE_(IDirect3DIndexBuffer9, IDirect3DResource9)): no shared header defines
// it (engine/CIndexBuffer.h and engine/IDirect3DDevice9.h forward-declare it); modelled as the landed sibling 0x00AC1EE0 does, on the
// shared IUnknown / IDirect3DResource9 (engine/Direct3DSurface9.h), with the SDK names: Lock slot 11 (+0x2C), Unlock slot 12 (+0x30),
// GetDesc slot 13 (+0x34). _D3DINDEXBUFFER_DESC is d3d9types.h's (Format, Type, Usage, Pool, Size; 0x14 bytes).
struct _D3DINDEXBUFFER_DESC { _D3DFORMAT Format; _D3DRESOURCETYPE Type; unsigned long Usage; _D3DPOOL Pool; unsigned int Size; };
struct IDirect3DIndexBuffer9 : public IDirect3DResource9 {
    virtual long FABLE_STDCALL Lock(unsigned int OffsetToLock, unsigned int SizeToLock, void** ppbData, unsigned long Flags) = 0;   // slot 11
    virtual long FABLE_STDCALL Unlock() = 0;                                                                                      // slot 12
    virtual long FABLE_STDCALL GetDesc(_D3DINDEXBUFFER_DESC* pDesc) = 0;                                                          // slot 13
};
#include <stdio.h>
#include <string.h>
// Test double: an index buffer that records its calls (all IUnknown / IDirect3DResource9 slots are inert).
static char g_seq[16]; static int g_seqN = 0;
static void Note(char c) { if (g_seqN < 15) { g_seq[g_seqN++] = c; g_seq[g_seqN] = 0; } }
static unsigned long g_lockArgs[3]; static unsigned int g_descSize = 0; static int g_mapped;
struct CFakeIndexBuffer : public IDirect3DIndexBuffer9 {
    long FABLE_STDCALL QueryInterface(const _GUID&, void**) { return 0; }
    unsigned long FABLE_STDCALL AddRef() { return 1; }
    unsigned long FABLE_STDCALL Release() { return 0; }
    long FABLE_STDCALL GetDevice(IDirect3DDevice9**) { return 0; }
    long FABLE_STDCALL SetPrivateData(const _GUID&, const void*, unsigned long, unsigned long) { return 0; }
    long FABLE_STDCALL GetPrivateData(const _GUID&, void*, unsigned long*) { return 0; }
    long FABLE_STDCALL FreePrivateData(const _GUID&) { return 0; }
    unsigned long FABLE_STDCALL SetPriority(unsigned long) { return 0; }
    unsigned long FABLE_STDCALL GetPriority() { return 0; }
    void FABLE_STDCALL PreLoad() { }
    _D3DRESOURCETYPE FABLE_STDCALL GetType() { return D3DRTYPE_SURFACE; }
    long FABLE_STDCALL Lock(unsigned int offset, unsigned int size, void** data, unsigned long flags) { Note('L'); g_lockArgs[0] = offset; g_lockArgs[1] = size; g_lockArgs[2] = flags; *data = &g_mapped; return 0; }
    long FABLE_STDCALL Unlock() { Note('U'); return 0; }
    long FABLE_STDCALL GetDesc(_D3DINDEXBUFFER_DESC* desc) { Note('D'); desc->Format = D3DFMT_UNKNOWN; desc->Type = D3DRTYPE_SURFACE; desc->Usage = 0; desc->Pool = D3DPOOL_DEFAULT; desc->Size = g_descSize; return 0; }
};
// CIndexBuffer has out-of-line virtuals, so the test works on raw storage and calls the non-virtual members only.
static unsigned char g_raw[sizeof(CIndexBuffer)];
// Test doubles for the manager chain: raw storage at the retail offsets (CSystemManager::PDisplayManager +0x60,
// CDisplayManager::PRenderManager +0x08, CDisplayManager::D3DDevice +0x58); only the pointers the row reads are set.
static unsigned char g_system[0x180], g_display[0x2cc], g_render[0x4000];
static void Put(unsigned char* base, unsigned int offset, void* value) { memcpy(base + offset, &value, sizeof(value)); }
CSystemManager* __fastcall GFGetSystemManager() { Note('S'); return (CSystemManager*)g_system; }
static unsigned long g_create[5]; static void* g_createThis = 0; static void** g_shared = (void**)1;
static long __stdcall FakeCreateIndexBuffer(void* self, unsigned int length, unsigned long usage, unsigned long format, unsigned long pool, IDirect3DIndexBuffer9** out, void** shared)
{
    Note('C'); g_createThis = self; g_create[0] = length; g_create[1] = usage; g_create[2] = format; g_create[3] = pool; g_shared = shared;
    static CFakeIndexBuffer created; *out = &created; g_create[4] = (unsigned long)(void*)&created; return 0;
}
static void* g_deviceVtbl[120]; static void* g_device[1] = { g_deviceVtbl };

int main()
{
    g_deviceVtbl[27] = (void*)&FakeCreateIndexBuffer;   // IDirect3DDevice9::CreateIndexBuffer (+0x6C)
    Put(g_system, 0x60, g_display); Put(g_display, 0x58, g_device);
    CIndexBuffer* b = (CIndexBuffer*)g_raw; b->PD3DIndexBuffer = 0;
    b->Initialise(100, 8, IB_FORMAT_16, IB_POOL_MANAGED, PT_TRIANGLE_STRIP);
    if (strcmp(g_seq, "SC") != 0 || g_createThis != (void*)g_device || g_create[0] != 100 || g_create[1] != 8 || g_create[2] != 101 || g_create[3] != 1
        || g_shared != 0 || (unsigned long)(void*)b->PD3DIndexBuffer != g_create[4] || b->ByteLength != 100 || *(EPrimitiveType*)b->PrimitiveType != PT_TRIANGLE_STRIP) { printf("00ac1f70_TEST FAIL %s\n", g_seq); return 1; }
    printf("00ac1f70_TEST PASS\n"); return 0;
}
