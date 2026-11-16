// CHandleBasedMemoryPool_012b4098::GetPointerFromHandle(long handle) -- the start of the area behind a handle (just past its 32-byte header)
// Leaf: ?GetPointerFromHandle@CHandleBasedMemoryPool_012b4098@@QAEPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28380, 21 bytes, thiscall, `ret 4`.
// Identity: libpk's own build of the engine pool (owner decision 2026-10-31: the engine name plus the vtable address 0x012B4098,
//   libpk has no PDB / RTTI; libpk/CHandleBasedMemoryPool_012b4098.h). The member is Ego_r.pdb / FableWin.pdb
//   ?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z (Ego_r 0x00B6D5E0, directly before GetSizeFromHandle 0x00B6D600, as retail
//   0x00C28380 precedes the landed 0x00C283A0); the engine build's copy is the landed 0x00A026B0. The caller 0x00C26090 already binds
//   this spelling; the binding "?Compute@SubObjectAt360@@QAEHH@Z" from caller 0x00C26240 is an invented placeholder (NAME_FIXES).
// Shape: Dinkumware `operator[]` = `*(begin() + _Pos)` over the handle table (+0x440 _Myfirst), then past the 32-byte CHeader.
#define FABLE_RETAIL_LIBPK_STREAM_BUFFER_PASS22
#include "libpk/CHandleBasedMemoryPool_012b4098.h"

void* CHandleBasedMemoryPool_012b4098::GetPointerFromHandle(long handle)
{
    return Handles[handle] + 1;
}