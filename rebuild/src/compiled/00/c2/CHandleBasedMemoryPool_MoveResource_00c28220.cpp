// CHandleBasedMemoryPool_012b4098::MoveResource(void* resource, long handle, void* source, void* destination, unsigned long size) -- move an area's bytes when the libpk pool relocates it (memmove)
// Leaf: ?MoveResource@CHandleBasedMemoryPool_012b4098@@MAEXPAXJ00K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28220, 27 bytes, thiscall, `ret 14h`.
// Identity: slot 0 of the libpk pool's vtable 0x012B4098 (no RTTI); Ego_r.pdb names the virtual
//   ?MoveResource@CHandleBasedMemoryPool_012b4098@@MAEXPAXJ00K@Z (protected virtual, five arguments), and the engine's own
//   build at 0x00A02120 has the same body (`memmove(destination, source, size)`) calling the CRT directly where this
//   libpk copy calls through the __imp__memmove import (0x01440144).
// Layout: the libpk build of the pool differs from engine/CHandleBasedMemoryPool.h (free lists at +0x3C, a Dinkumware
//   handle vector at +0x43C; landed 0x00C28080 / 0x00C283C0), so the class is modelled locally; only the virtual is needed.
// Shape: three pushes, the import call and `add esp,0Ch`.
extern "C" __declspec(dllimport) void* __cdecl memmove(void* destination, const void* source, unsigned int size);   // 0x01440144

class CHandleBasedMemoryPool_012b4098 {                  // the libpk build (vtable 0x012B4098)
protected:
    virtual void MoveResource(void* resource, long handle, void* source, void* destination, unsigned long size);
};

void CHandleBasedMemoryPool_012b4098::MoveResource(void* resource, long handle, void* source, void* destination, unsigned long size)
{
    memmove(destination, source, size);
}