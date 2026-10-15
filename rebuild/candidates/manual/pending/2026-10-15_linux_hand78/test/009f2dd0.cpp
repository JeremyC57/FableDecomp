#include <cstdio>
#include <cstring>
struct IDirect3DSurface9 { int id; };
struct CCharString { const char* m_text; };
static int g_wideAlive = 0, g_loads = 0;
static long g_result = 0;
static unsigned long g_lastFilter = 0;
static IDirect3DSurface9* g_lastSurface = 0;
static unsigned short g_lastFirst = 0;
class CWideString {
public:
    unsigned short m_data[32];
    CWideString(const char* text) { int i = 0; for (; text[i] && i < 31; ++i) m_data[i] = (unsigned short)text[i]; m_data[i] = 0; ++g_wideAlive; }
    CWideString(const CWideString& o) { std::memcpy(m_data, o.m_data, sizeof(m_data)); ++g_wideAlive; }
    ~CWideString() { --g_wideAlive; }
    operator const unsigned short*() const { return m_data; }
};
static CWideString ConvertPathToFullPath(const CCharString& path) { return CWideString(path.m_text); }
static long D3DXLoadSurfaceFromFileW(IDirect3DSurface9* dest, const void*, const void*, const unsigned short* file, const void*, unsigned long filter, unsigned long, void*)
{ ++g_loads; g_lastSurface = dest; g_lastFilter = filter; g_lastFirst = file[0]; return g_result; }
struct CSurface {
    void* __vftable; IDirect3DSurface9* PD3DSurface; unsigned long AllocationSource; void* PAllocatedMemory;
    bool LoadFromBitmapFileToPreallocatedSurface(const CCharString& filename, unsigned long filter)
    {
        CWideString path = ConvertPathToFullPath(filename);
        if (D3DXLoadSurfaceFromFileW(PD3DSurface, 0, 0, path, 0, filter, 0, 0) != 0)
            return false;
        return true;
    }
};
int main()
{
    IDirect3DSurface9 d3d; d3d.id = 7;
    CSurface s; s.PD3DSurface = &d3d;
    CCharString name; name.m_text = "Data\\pic.bmp";
    bool ok = s.LoadFromBitmapFileToPreallocatedSurface(name, 3);
    if (!ok || g_loads != 1 || g_lastSurface != &d3d || g_lastFilter != 3 || g_lastFirst != 'D' || g_wideAlive != 0) { std::printf("009f2dd0_TEST FAIL\n"); return 1; }
    g_result = -2005530516;
    ok = s.LoadFromBitmapFileToPreallocatedSurface(name, 1);
    if (ok || g_loads != 2 || g_wideAlive != 0) { std::printf("009f2dd0_TEST FAIL\n"); return 1; }
    std::printf("009f2dd0_TEST PASS\n");
    return 0;
}
