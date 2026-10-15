// CSurface::LoadFromBitmapFileToPreallocatedSurface(const CCharString& filename, unsigned long filter) -- expand the file name to a full wide path and have D3DX load the image file into the already allocated Direct3D surface with the given filter; true when D3DX returns D3D_OK
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2DD0, length 79 bytes.
// Callees: ConvertPathToFullPath @ 0x00999110 (`__fastcall`, hidden CWideString result in ecx, the path in edx),
// CWideString::operator const wchar_t* @ 0x0099AF00, D3DXLoadSurfaceFromFileW @ 0x00BFEFBC (`__stdcall`, 8 args),
// CWideString::~CWideString @ 0x0099B510.
// Shape: the wide-string temporary is built in the DEAD slot of the by-reference filename argument
// (`lea ecx,[esp+8]` right after `push esi`), the D3DX arguments pushed right to left with the conversion
// call between the filter and the remaining nulls, `test eax,eax` (== D3D_OK), and the destructor duplicated
// into both return paths (`xor al,al` / `mov al,1`, `ret 8`). No EH frame (the module builds without /EHsc).
// Model: engine/CSurface.h (+4 PD3DSurface) with a methods struct.
#include "engine/CSurface.h"

struct CCharString;

class CWideString {
public:
    unsigned short* m_pData;                       // +0x00 (wchar_t is unsigned short under the era defaults)
    ~CWideString();                                // 0x0099B510
    operator const unsigned short*() const;        // 0x0099AF00
};

CWideString __fastcall ConvertPathToFullPath(const CCharString& path);   // 0x00999110

extern "C" long __stdcall D3DXLoadSurfaceFromFileW(IDirect3DSurface9* destSurface, const void* destPalette,
    const void* destRect, const unsigned short* srcFile, const void* srcRect, unsigned long filter,
    unsigned long colourKey, void* srcInfo);       // 0x00BFEFBC

struct CSurface_Methods : CSurface {
    bool LoadFromBitmapFileToPreallocatedSurface(const CCharString& filename, unsigned long filter);
};

bool CSurface_Methods::LoadFromBitmapFileToPreallocatedSurface(const CCharString& filename, unsigned long filter)
{
    CWideString path = ConvertPathToFullPath(filename);
    if (D3DXLoadSurfaceFromFileW(PD3DSurface, 0, 0, path, 0, filter, 0, 0) != 0)
        return false;
    return true;
}