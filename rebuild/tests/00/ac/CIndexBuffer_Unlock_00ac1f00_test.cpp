// 00ac1f00 test: CIndexBuffer::Unlock on the shared header, linked against the row object (a recording IDirect3DIndexBuffer9 double).
#define FABLE_RETAIL_CINDEXBUFFER
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
#include "engine/CIndexBuffer.h"

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

int main()
{
    CFakeIndexBuffer fake; CIndexBuffer* b = (CIndexBuffer*)g_raw; b->PD3DIndexBuffer = &fake;
    b->Unlock();
    if (strcmp(g_seq, "U") != 0 || b->PD3DIndexBuffer != &fake) { printf("00ac1f00_TEST FAIL\n"); return 1; }
    printf("00ac1f00_TEST PASS\n"); return 0;
}
