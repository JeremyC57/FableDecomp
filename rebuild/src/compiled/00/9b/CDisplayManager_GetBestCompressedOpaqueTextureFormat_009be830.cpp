// CDisplayManager::GetBestCompressedOpaqueTextureFormat(unsigned long flags,CPixelFormat& result) -- Select the supported compressed texture format.
// Leaf: ?GetBestCompressedOpaqueTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z
// Identity: Both PDB sets original GetBestCompressedOpaqueTextureFormat(unsigned long,CPixelFormat&); caller texture-selector branch and own D3DFMT_DXT1 constant, IDirect3D9 CheckDeviceFormat slot10, Initialise(_D3DFORMAT)009e3830.
#define CPixelFormat CPixelFormat_HeaderLayout
#include "engine/CPixelFormat.h"
#undef CPixelFormat
#ifndef _MSC_VER
#define __stdcall
#endif
#include "engine/IDirect3D9.h"   // IDirect3D9; engine/Direct3D9Types.h: _D3DFORMAT, _D3DDEVTYPE, _D3DDISPLAYMODE, _D3DCAPS9
enum _D3DRESOURCETYPE{D3DRTYPE_TEXTURE=3};














class CPixelFormat:public CPixelFormat_HeaderLayout{public:void Initialise(_D3DFORMAT);};

// Layout: no active display-manager header. D3D+84, device+88, SDK caps+92(size304), CurrentMode+440; only caps DeviceType/AdapterOrdinal and mode Format+452 read.
class CDisplayManager{public:unsigned char Prefix[84];IDirect3D9* D3D;void* D3DDevice;_D3DCAPS9 D3DDeviceCaps;unsigned char DisplayState[44];_D3DDISPLAYMODE CurrentMode;bool GetBestCompressedOpaqueTextureFormat(unsigned long,CPixelFormat&);};
bool CDisplayManager::GetBestCompressedOpaqueTextureFormat(unsigned long,CPixelFormat& result){if(D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal,D3DDeviceCaps.DeviceType,CurrentMode.Format,0,D3DRTYPE_TEXTURE,D3DFMT_DXT1)>=0){result.Initialise(D3DFMT_DXT1);return true;}return false;}