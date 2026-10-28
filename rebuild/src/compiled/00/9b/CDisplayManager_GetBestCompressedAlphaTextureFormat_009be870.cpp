// CDisplayManager::GetBestCompressedAlphaTextureFormat(unsigned long flags,CPixelFormat& result) -- Select the supported compressed texture format.
// Leaf: ?GetBestCompressedAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z
// Identity: Both PDB sets original GetBestCompressedAlphaTextureFormat(unsigned long,CPixelFormat&); caller texture-selector branch and own D3DFMT_DXT3 constant, IDirect3D9 CheckDeviceFormat slot10, Initialise(_D3DFORMAT)009e3830.
#define CPixelFormat CPixelFormat_HeaderLayout
#include "engine/CPixelFormat.h"
#undef CPixelFormat
#ifndef _MSC_VER
#define __stdcall
#endif
enum _D3DFORMAT{D3DFMT_UNKNOWN=0,D3DFMT_DXT1=0x31545844,D3DFMT_DXT3=0x33545844};
enum _D3DDEVTYPE{D3DDEVTYPE_HAL=1};enum _D3DRESOURCETYPE{D3DRTYPE_TEXTURE=3};
struct _GUID;struct _D3DADAPTER_IDENTIFIER9;struct _D3DDISPLAYMODE{unsigned int Width,Height,RefreshRate;_D3DFORMAT Format;};
struct IDirect3D9 {
virtual long __stdcall QueryInterface(const _GUID&,void**)=0;
virtual unsigned long __stdcall AddRef()=0;
virtual unsigned long __stdcall Release()=0;
virtual long __stdcall RegisterSoftwareDevice(void*)=0;
virtual unsigned int __stdcall GetAdapterCount()=0;
virtual long __stdcall GetAdapterIdentifier(unsigned int,unsigned long,_D3DADAPTER_IDENTIFIER9*)=0;
virtual unsigned int __stdcall GetAdapterModeCount(unsigned int,_D3DFORMAT)=0;
virtual long __stdcall EnumAdapterModes(unsigned int,_D3DFORMAT,unsigned int,_D3DDISPLAYMODE*)=0;
virtual long __stdcall GetAdapterDisplayMode(unsigned int,_D3DDISPLAYMODE*)=0;
virtual long __stdcall CheckDeviceType(unsigned int,_D3DDEVTYPE,_D3DFORMAT,_D3DFORMAT,int)=0;
virtual long __stdcall CheckDeviceFormat(unsigned int,_D3DDEVTYPE,_D3DFORMAT,unsigned long,_D3DRESOURCETYPE,_D3DFORMAT)=0;
};
class CPixelFormat:public CPixelFormat_HeaderLayout{public:void Initialise(_D3DFORMAT);};
struct _D3DCAPS9{_D3DDEVTYPE DeviceType;unsigned int AdapterOrdinal;unsigned char RemainingCaps[296];};
// Layout: no active display-manager header. D3D+84, device+88, SDK caps+92(size304), CurrentMode+440; only caps DeviceType/AdapterOrdinal and mode Format+452 read.
class CDisplayManager{public:unsigned char Prefix[84];IDirect3D9* D3D;void* D3DDevice;_D3DCAPS9 D3DDeviceCaps;unsigned char DisplayState[44];_D3DDISPLAYMODE CurrentMode;bool GetBestCompressedAlphaTextureFormat(unsigned long,CPixelFormat&);};
bool CDisplayManager::GetBestCompressedAlphaTextureFormat(unsigned long,CPixelFormat& result){if(D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal,D3DDeviceCaps.DeviceType,CurrentMode.Format,0,D3DRTYPE_TEXTURE,D3DFMT_DXT3)>=0){result.Initialise(D3DFMT_DXT3);return true;}return false;}