// CGlobalMemoryHolder::`scalar_deleting_destructor'(unsigned int) -- GlobalFree the handle at +0x20, then the object if asked
// Leaf: ??_GCGlobalMemoryHolder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098F500, length 39 bytes.
// Callees: KERNEL32!GlobalFree through the import slot 0x0143FDF8, operator delete @ 0x00BFE9BC.
// Identity: the manifest label "CSystemRegistry::~CSystemRegistry" is propagated and wrong (the body
// frees a global-memory handle, not a registry key). It sits after the CThreadedFileManager constructor
// (0x0098F3A0) and has no direct caller or table reference in the image, so the owning class is not
// established and is modelled descriptively: a 0x24-byte record whose last word is an HGLOBAL.
//
// Shape: no vtable store -> a NON-polymorphic class; VC7.1 emits its `??_G` in a TU that `delete`s it
// (the `Destroy` helper), inlining the destructor `if (m_hMemory) GlobalFree(m_hMemory);` (`__stdcall`
// import: no stack cleanup after it). `add esp,4` after operator delete = a speed-optimised module.
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* hMem);   // [0x0143FDF8]

struct CGlobalMemoryHolder {
    unsigned char _pad00[0x20];                  // +0x00 .. +0x1f (not touched here)
    void*         m_hMemory;                     // +0x20 HGLOBAL
    ~CGlobalMemoryHolder() { if (m_hMemory) GlobalFree(m_hMemory); }
};

void DestroyGlobalMemoryHolder(CGlobalMemoryHolder* holder)
{
    delete holder;
}