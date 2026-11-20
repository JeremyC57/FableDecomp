#ifndef FABLE_ENGINE_IDIRECT3D9_H
#define FABLE_ENGINE_IDIRECT3D9_H
// New header (hand3c2, row 0x009BF7E0 CDisplayManager::CreateDevice), the twin of engine/IDirect3DDevice9.h: IDirect3D9 (DirectX 9 SDK
// d3d9.h, DECLARE_INTERFACE_(IDirect3D9, IUnknown)), the COM object CDisplayManager::D3D (+0x54) points at. Methods in SDK order and
// with SDK names (slot N at +4N; the row calls CheckDeviceMultiSampleType 11 +0x2C at 0x009BF987, CreateDevice 16 +0x40 at
// 0x009BF9CC and GetAdapterDisplayMode 8 +0x20 at 0x009BF9E3). d3d9.h is not on the verifier include path: parameter types are the
// SDK typedefs of engine/Direct3D9Types.h where that header has them, else the underlying types; interfaces and the structures the
// engine does not fill are forward declarations. Calls go through the vtable, so no symbol is bound.
#include "engine/Direct3D9Types.h"
// winnt.h: `typedef LONG HRESULT;` (Ego_d.pdb types the display manager's `hr` locals T_HRESULT, 0x0008).
typedef long HRESULT;
struct _D3DADAPTER_IDENTIFIER9;
struct IDirect3DDevice9;
struct HMONITOR__;
struct _GUID;
// d3d9.h: the adapter ordinal of the primary display and the IDirect3D9::CreateDevice behaviour flags.
#define D3DADAPTER_DEFAULT                     0
// d3d9.h: FullScreen_RefreshRateInHz value that lets the runtime choose.
#define D3DPRESENT_RATE_DEFAULT                 0x00000000
#define D3DCREATE_FPU_PRESERVE                  0x00000002L
#define D3DCREATE_MULTITHREADED                 0x00000004L
#define D3DCREATE_PUREDEVICE                    0x00000010L
#define D3DCREATE_SOFTWARE_VERTEXPROCESSING     0x00000020L
#define D3DCREATE_HARDWARE_VERTEXPROCESSING     0x00000040L
#define D3DCREATE_MIXED_VERTEXPROCESSING        0x00000080L
// d3d9caps.h: the D3DCAPS9::DevCaps bit the row tests (0x009BF927 `and esi,100000h`).
#define D3DDEVCAPS_PUREDEVICE                   0x00100000L
struct IDirect3D9 {
    virtual long __stdcall QueryInterface(const _GUID& riid, void** ppvObj) = 0;   // slot 0 (+0x0)
    virtual unsigned long __stdcall AddRef() = 0;   // slot 1 (+0x4)
    virtual unsigned long __stdcall Release() = 0;   // slot 2 (+0x8)
    virtual long __stdcall RegisterSoftwareDevice(void* pInitializeFunction) = 0;   // slot 3 (+0xc)
    virtual UINT __stdcall GetAdapterCount() = 0;   // slot 4 (+0x10)
    virtual long __stdcall GetAdapterIdentifier(UINT Adapter, DWORD Flags, _D3DADAPTER_IDENTIFIER9* pIdentifier) = 0;   // slot 5 (+0x14)
    virtual UINT __stdcall GetAdapterModeCount(UINT Adapter, D3DFORMAT Format) = 0;   // slot 6 (+0x18)
    virtual long __stdcall EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) = 0;   // slot 7 (+0x1c)
    virtual long __stdcall GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) = 0;   // slot 8 (+0x20)
    virtual long __stdcall CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) = 0;   // slot 9 (+0x24)
    virtual long __stdcall CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, unsigned long RType, D3DFORMAT CheckFormat) = 0;   // slot 10 (+0x28)
    virtual long __stdcall CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) = 0;   // slot 11 (+0x2c)
    virtual long __stdcall CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) = 0;   // slot 12 (+0x30)
    virtual long __stdcall CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) = 0;   // slot 13 (+0x34)
    virtual long __stdcall GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) = 0;   // slot 14 (+0x38)
    virtual HMONITOR__* __stdcall GetAdapterMonitor(UINT Adapter) = 0;   // slot 15 (+0x3c)
    virtual long __stdcall CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) = 0;   // slot 16 (+0x40)
};
#endif // FABLE_ENGINE_IDIRECT3D9_H
