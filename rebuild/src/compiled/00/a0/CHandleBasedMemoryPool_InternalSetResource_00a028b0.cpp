// CHandleBasedMemoryPool::InternalSetResource(long, void*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A028B0, oracle length 22 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+438h] ; mov ecx,[esp+4] ; mov edx,[eax+ecx*4] ; mov eax,[esp+8] ; mov [edx],eax ; ret 8
// engine/CHandleBasedMemoryPool.h (PDB): +0x438 CArray<CHandle> Handles. The array's first dword
// is its element pointer; each element is a pointer to a handle record whose first dword is the
// resource the handle currently resolves to. Rebinds handle `handle` to `resource`.
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleRecord {
    void* Resource;          // +0x00 resolved resource pointer
};

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void InternalSetResource(long handle, void* resource);
};

void CHandleBasedMemoryPool_Methods::InternalSetResource(long handle, void* resource)
{
    CHandleRecord** records = *(CHandleRecord***)&this->Handles[0];   // CArray element pointer
    records[handle]->Resource = resource;
}