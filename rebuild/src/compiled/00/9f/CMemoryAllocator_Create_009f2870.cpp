// CMemoryAllocator::Create() -- build the global memory allocator in its own committed pages (VirtualAlloc of sizeof(CMemoryAllocator) = 0x49c) over the process heap, publish it (0 on failure), and record the physical memory already in use (TotalPhys - AvailPhys) in two globals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2870, length 96 bytes.
// Leaf: ?Create@CMemoryAllocator@@KIXXZ
// PDB: ?Create@CMemoryAllocator@@KIXXZ (protected static __fastcall, no arguments); the class operator new is
//   ??2CMemoryAllocator@@KIPAXI@Z (protected static __fastcall), inlined here as VirtualAlloc(0, size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE).
// Callees: KERNEL32 VirtualAlloc (IAT 0x0143FD44), GetProcessHeap (IAT 0x0143FD3C), GlobalMemoryStatus (IAT 0x0143FE20),
//   CMemoryAllocator::CMemoryAllocator(void* heap) @ 0x009F20B0.
// Globals: the allocator @ 0x013CAA90; two copies of the start-up physical memory use @ 0x013CAA88 and 0x013CAAB0.
// Shape: a new-expression through the inline class operator new: the allocation runs first, its null test guards the
//   GetProcessHeap argument and the constructor call (the 0 store is the new-expression's null arm); the 32-byte
//   MEMORYSTATUS is the only frame local (`sub esp,20h`), its TotalPhys/AvailPhys words at +8/+0xc.
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void* address, unsigned long size, unsigned long allocationType, unsigned long protect);   // IAT 0x0143FD44
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();   // IAT 0x0143FD3C
struct SMemoryStatus {               // MEMORYSTATUS
    unsigned long dwLength;
    unsigned long dwMemoryLoad;
    unsigned long dwTotalPhys;       // +0x08
    unsigned long dwAvailPhys;       // +0x0c
    unsigned long dwTotalPageFile;
    unsigned long dwAvailPageFile;
    unsigned long dwTotalVirtual;
    unsigned long dwAvailVirtual;
};
extern "C" __declspec(dllimport) void __stdcall GlobalMemoryStatus(SMemoryStatus* status);   // IAT 0x0143FE20

extern unsigned long g_physicalMemoryInUseAtStart;      // 0x013CAA88
extern unsigned long g_physicalMemoryInUseAtStartCopy;  // 0x013CAAB0

class CMemoryAllocator {
protected:
    unsigned char _body[0x49c];                      // PDB CMemoryAllocator is 0x49c bytes, no vtable
    CMemoryAllocator(void* heap);                    // 0x009F20B0
    static void* __fastcall operator new(unsigned int size)
    {
        return VirtualAlloc(0, size, 0x3000, 4);     // MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE
    }
    static void __fastcall operator delete(void* p); // 0x009F1F90
    static CMemoryAllocator* Allocator;              // 0x013CAA90
    static void __fastcall Create();
};

void __fastcall CMemoryAllocator::Create()
{
    Allocator = new CMemoryAllocator(GetProcessHeap());
    SMemoryStatus status;
    GlobalMemoryStatus(&status);
    unsigned long inUse = status.dwTotalPhys - status.dwAvailPhys;
    g_physicalMemoryInUseAtStart = inUse;
    g_physicalMemoryInUseAtStartCopy = inUse;
}