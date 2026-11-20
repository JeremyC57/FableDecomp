#include <cstdio>
#include <cstring>
// Behaviour test for 0x009F2DD0 CSurface::LoadFromBitmapFileToPreallocatedSurface(const CWideString&, unsigned long).
// NF146 (2026-11-20, q4283): rewritten on the shared headers the row now uses (fable_string.h, engine/CSurface.h BITMAP,
//   engine/CAFile.h). The test defines the out-of-line callees the row binds (CAFile::ConvertPathToFullPath, the CWideString default
//   constructor / destructor / operator const wchar_t*, D3DXLoadSurfaceFromFileW) as recording fakes and checks: the input reaches
//   ConvertPathToFullPath, the converted text and the filter reach D3DX with the surface's Direct3D pointer, D3D_OK gives true and a
//   failure code false, and the path temporary is destroyed on both return paths.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CAFILE
#include "fable_string.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurface.h"
#include "engine/CAFile.h"

static int g_alive = 0, g_converts = 0, g_loads = 0;
static long g_result = 0;
static unsigned long g_lastFilter = 0;
static IDirect3DSurface9* g_lastSurface = 0;
static const CWideString* g_lastInput = 0;
static unsigned short g_lastFirst = 0;
static const unsigned short g_full[] = { 'D', ':', '\\', 0 };

CWideString::CWideString() { ++g_alive; }
CWideString::~CWideString() { --g_alive; }
CWideString::operator const wchar_t*() const { return (const wchar_t*)g_full; }
CWideString __fastcall CAFile::ConvertPathToFullPath(const CWideString& path) { ++g_converts; g_lastInput = &path; return CWideString(); }
extern "C" long __stdcall D3DXLoadSurfaceFromFileW(IDirect3DSurface9* dest, const void*, const void*, const unsigned short* file,
    const void*, unsigned long filter, unsigned long, void*)
{ ++g_loads; g_lastSurface = dest; g_lastFilter = filter; g_lastFirst = file[0]; return g_result; }

int main()
{
    static void* storage[4];                       // CSurface: vftable, PD3DSurface (+4), AllocationSource, PAllocatedMemory
    IDirect3DSurface9* d3d = (IDirect3DSurface9*)&g_result;
    storage[1] = d3d;
    CSurface* s = (CSurface*)storage;
    bool ok;
    {
        CWideString name;
        ok = s->LoadFromBitmapFileToPreallocatedSurface(name, 3);
        if (!ok || g_converts != 1 || g_lastInput != &name || g_loads != 1 || g_lastSurface != d3d || g_lastFilter != 3
            || g_lastFirst != 'D' || g_alive != 1) { std::printf("009f2dd0_TEST FAIL\n"); return 1; }
        g_result = -2005530516;
        ok = s->LoadFromBitmapFileToPreallocatedSurface(name, 1);
        if (ok || g_loads != 2 || g_lastFilter != 1 || g_alive != 1) { std::printf("009f2dd0_TEST FAIL\n"); return 1; }
    }
    if (g_alive != 0) { std::printf("009f2dd0_TEST FAIL\n"); return 1; }
    std::printf("009f2dd0_TEST PASS\n");
    return 0;
}
