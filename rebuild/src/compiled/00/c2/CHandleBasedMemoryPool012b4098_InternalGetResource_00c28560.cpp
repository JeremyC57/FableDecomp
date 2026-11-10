// CHandleBasedMemoryPool_012b4098::InternalGetResource(long handle) -- the owner's resource pointer stored in the header of the area behind a handle
// Leaf: ?InternalGetResource@CHandleBasedMemoryPool_012b4098@@IAEPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28560, 21 bytes, thiscall `ret 4`. Not called or referenced in retail (an
//   inline-candidate member kept out of line).
// Identity: the libpk build of the engine pool; the Ego_r.pdb name set lists the protected pair
//   `InternalSetResource(long, void*)` / `InternalGetResource(long)` (?InternalSetResource@CHandleBasedMemoryPool@@IAEXJPAX@Z,
//   ?InternalGetResource@CHandleBasedMemoryPool@@IAEPAXJ@Z) between Defragment and MoveArea, the same order as retail here
//   (Defragment 0x00C28400, InternalSetResource 0x00C28540, this row, MoveArea 0x00C28580), and the arities fit (`ret 8` / `ret 4`).
// Layout: the shared libpk model (libpk/CHandleBasedMemoryPool_012b4098.h): a VC7.1 Dinkumware vector<CHeader*> at +0x43C
//   (_Myfirst +0x440); engine/CHandleBasedMemoryPool.h has the engine layout.
// Shape: the dead `lea eax,[eax+ecx*4]` after the slot load is Dinkumware's `operator[]` (`*(begin() + _Pos)` over _Ptrit).
#include "libpk/CHandleBasedMemoryPool_012b4098.h"

void* CHandleBasedMemoryPool_012b4098::InternalGetResource(long handle)
{
    return Handles[handle]->Resource;
}