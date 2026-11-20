// 00a8e160 test: the shared header's inline CVertexBufferWin32 constructor (the COMDAT the row emits): the base constructor, the empty-buffer fields.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/CVertexBufferWin32.h"
#include <stdio.h>
// The out-of-line members the CVertexBufferWin32 vtable and destructor reach (0x0099A2D0..0x0099A300, 0x00A63200, the Do* slots) are not in
// this test: recording / inert definitions.
static int g_baseCtor = 0, g_uninit = 0;
CBaseClass::CBaseClass() { ++g_baseCtor; }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
void CVertexBufferWin32::Uninitialise() { ++g_uninit; }
unsigned char* CVertexBufferWin32::DoLock(unsigned long, unsigned long, unsigned long) { return 0; }
void CVertexBufferWin32::DoUnlock() { }
bool CVertexBufferWin32::DoIsBusy() const { return false; }
unsigned long CVertexBufferWin32::DoSizeof() const { return 0; }
inline void* operator new(unsigned int, void* where) { return where; }
static bool Shell(const CVertexBufferWin32& v)
{
    const CVertexCompressionParams& p = v.CompressionParams;
    return v.PVertexBuffer == 0 && v.Length == 0 && v.Type == 0 && v.Stride == 0 && p.PosScaleX == 1.0f && p.PosScaleY == 1.0f && p.PosScaleZ == 1.0f
        && p.PosScaleW == 1.0f && p.PosBiasX == 0.0f && p.PosBiasY == 0.0f && p.PosBiasZ == 0.0f && p.PosBiasW == 0.0f;
}

int main()
{
    unsigned char raw[sizeof(CVertexBufferWin32)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xee;
    CVertexBufferWin32* v = new (raw) CVertexBufferWin32();
    if (g_baseCtor != 1 || !Shell(*v) || g_uninit != 0) { printf("00a8e160_TEST FAIL\n"); return 1; }
    v->~CVertexBufferWin32();
    if (g_uninit != 1) { printf("00a8e160_TEST FAIL\n"); return 1; }
    printf("00a8e160_TEST PASS\n"); return 0;
}
