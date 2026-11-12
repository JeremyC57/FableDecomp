// CMemoryAllocatorBase::operator new -- class allocation function: commits fresh pages for an allocator object.
// Leaf: ??2CMemoryAllocatorBase@@SIPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66730.
// Name: Ego_r.pdb 0x00C085A0 and FableWin.pdb 0x033BEC90 ??2CMemoryAllocatorBase@@SIPAXI@Z (public static __fastcall);
// row 0x009F20B0 (CMemoryAllocator ctor) binds this address under that symbol through the shared header opt-in.
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATORBASE_OPERATOR_NEW
#include "engine/CMemoryAllocatorBase.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*, unsigned long, unsigned long, unsigned long);   // IAT 0x0143FD44 = KERNEL32.dll!VirtualAlloc

void* FABLE_FASTCALL CMemoryAllocatorBase::operator new(unsigned int size){ return VirtualAlloc(0, size, 0x3000, 4); }
