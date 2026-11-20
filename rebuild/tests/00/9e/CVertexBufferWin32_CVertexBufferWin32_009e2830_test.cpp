// 009e2830 test: the shared header's inline CVertexBufferWin32(EVertexType, long, long, unsigned long, EVertexBufferPool) constructor (the COMDAT the row emits): the empty buffer, then Initialise with the five arguments.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXBUFFERWIN32_CTOR_INITIALISE
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
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
static long g_init[5] = { -1, -1, -1, -1, -1 }; static bool g_shellAtInit = false;
bool CVertexBufferWin32::Initialise(EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool)
{
    g_shellAtInit = Shell(*this); g_init[0] = type; g_init[1] = count; g_init[2] = stride; g_init[3] = (long)usage; g_init[4] = pool; return true;
}

int main()
{
    unsigned char raw[sizeof(CVertexBufferWin32)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xee;
    CVertexBufferWin32* v = new (raw) CVertexBufferWin32(VERTEX_NORM_TEX1, 30, 40, 50, VB_POOL_MANAGED);
    if (g_baseCtor != 1 || !g_shellAtInit || g_init[0] != VERTEX_NORM_TEX1 || g_init[1] != 30 || g_init[2] != 40 || g_init[3] != 50 || g_init[4] != VB_POOL_MANAGED) { printf("009e2830_TEST FAIL\n"); return 1; }
    printf("009e2830_TEST PASS\n"); return 0;
}
