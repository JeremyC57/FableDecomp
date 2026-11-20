// CDisplayManager::ReadDeviceRefreshRate() -- the default adapter's current refresh rate (IDirect3D9::GetAdapterDisplayMode); throws CGenericException when the query fails
// Leaf: ?ReadDeviceRefreshRate@CDisplayManager@@AAEJXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BE340, 54 bytes, `ret`, `sub esp, 14h` (the 16-byte D3DDISPLAYMODE + the 1-byte exception
//   temporary). Expanded by CDisplayManager::CreateDevice (0x009BF9D7..0x009BFA01, whose row carries this text as a same-TU copy).
// Identity (re-port, audit 2026-11-20; landed 2026-10-22 as "CDisplayManager::GetRefreshRate" on row-local models): Ego_r.pdb /
//   FableWin.pdb ?ReadDeviceRefreshRate@CDisplayManager@@AAEJXZ; Ego_d.pdb procedure of the same name in lib_display_manager.obj
//   (debug length 270, file id 600 = the display manager's .cpp, lines 847..858; locals hr (type 0x0008 T_HRESULT), mode), whose
//   body is this one: line 850 the GetAdapterDisplayMode call stored to hr (29 bytes = our /Od 27 + the VC10 COM reload), 851 the
//   test, 853 a debug-only log (138 bytes), 854 the throw, 857 `return mode.RefreshRate` (3 = ours). Ego_d's
//   ?GetRefreshRate@CDisplayManager@@QBEJXZ is the public const accessor in main.obj (20 bytes), a different function.
// Inferred: `throw CGenericException();`. Ego_d line 854 is 32 bytes, the size of the five `throw CWaveFileException("...")` lines
//   of CWaveLoader::LoadWaveFile in the same PDB (our /Od: 27 without an argument, 32 with a string), so the original passed a
//   message. Its text is in no source (release CExceptionBase discards it: retail copies a 1-byte temporary with no constructor call
//   and no string, 0x009BE357), so the row throws through the class's other constructor (Ego_d method list 0xCA77: `(const char*)`
//   and `()`); the retail bytes are the same.
// Callees: IDirect3D9 slot 8 GetAdapterDisplayMode (+20h, adapter D3DADAPTER_DEFAULT); 0x00BFEB84 _CxxThrowException with ThrowInfo
//   0x013692F8 (__TI2?AVCGenericException@@).
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTEXTURE
#include "engine/CDisplayManager.h"
#include "engine/IDirect3D9.h"
#include "engine/CGenericException.h"

long CDisplayManager::ReadDeviceRefreshRate()
{
    D3DDISPLAYMODE mode;
    HRESULT hr = D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
    if (hr < 0)
    {
        throw CGenericException();
    }

    return mode.RefreshRate;
}