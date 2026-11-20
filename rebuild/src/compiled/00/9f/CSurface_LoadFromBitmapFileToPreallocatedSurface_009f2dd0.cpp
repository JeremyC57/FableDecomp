// CSurface::LoadFromBitmapFileToPreallocatedSurface(const CWideString& filename, unsigned long filter) -- expand the file name to a full wide path and have D3DX load the image file into the already allocated Direct3D surface with the given filter; true when D3DX returns D3D_OK
// Leaf: ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2DD0, length 79 bytes.
// Callees: CAFile::ConvertPathToFullPath @ 0x00999110 (`__fastcall`, hidden CWideString result in ecx, the path in edx),
// CWideString::operator const wchar_t* @ 0x0099AF00, D3DXLoadSurfaceFromFileW @ 0x00BFEFBC (`__stdcall`, 8 args),
// CWideString::~CWideString @ 0x0099B510.
// Shape: the wide-string temporary is built in the DEAD slot of the by-reference filename argument
// (`lea ecx,[esp+8]` right after `push esi`), the D3DX arguments pushed right to left with the conversion
// call between the filter and the remaining nulls, `test eax,eax` (== D3D_OK), and the destructor duplicated
// into both return paths (`xor al,al` / `mov al,1`, `ret 8`). No EH frame (the module builds without /EHsc).
// Naming (NF146, 2026-11-20, q4283): own leaf moved from the CSurface_Methods stand-in (const CCharString&) to the PDB
//   ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z (Ego_r.pdb 0x00B46AA0 + FableWin.pdb 0x030C9340; the body
//   passes the parameter unchanged in edx to 0x00999110, whose parameter both PDBs type const CWideString&), declared by engine/CSurface.h
//   (FABLE_RETAIL_CSURFACE_BITMAP); every landed caller binds it. The callees move off the namespaced N_0099af00::CWideString model and the
//   free ConvertPathToFullPath(const CCharString&) stand-in onto fable_string.h and engine/CAFile.h:
//   ?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z (Ego_r.pdb 0x00B00D80 + FableWin.pdb 0x02FEF590), ??BCWideString@@QBEPBGXZ and
//   ??1CWideString@@QAE@XZ (the spellings other landed rows bind).
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CAFILE
#include "fable_string.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurface.h"
#include "engine/CAFile.h"

extern "C" long __stdcall D3DXLoadSurfaceFromFileW(IDirect3DSurface9* destSurface, const void* destPalette,
    const void* destRect, const unsigned short* srcFile, const void* srcRect, unsigned long filter,
    unsigned long colourKey, void* srcInfo);       // 0x00BFEFBC

bool CSurface::LoadFromBitmapFileToPreallocatedSurface(const CWideString& filename, unsigned long filter)
{
    CWideString path = CAFile::ConvertPathToFullPath(filename);
    if (D3DXLoadSurfaceFromFileW(PD3DSurface, 0, 0, path, 0, filter, 0, 0) != 0)
        return false;
    return true;
}
