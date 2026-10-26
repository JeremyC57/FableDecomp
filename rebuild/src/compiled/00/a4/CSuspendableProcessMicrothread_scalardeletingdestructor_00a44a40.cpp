// CSuspendableProcessMicrothread::`scalar deleting destructor'(unsigned int flags) -- the destructor (0x00A44620) then free for flag bit 0; an unoptimised TU (`this` spilled to [ebp-4])
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44A40, length 48 bytes.
// Leaf: ??_GCSuspendableProcessMicrothread@@UAEPAXI@Z
// Shape: the EBP frame with `this` reloaded from its spill for every use, `and eax,1`, `mov esp,ebp`.
#pragma optimize("", off)
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CSuspendableProcessMicrothread { public:
    virtual ~CSuspendableProcessMicrothread();     // 0x00A44620 (out of line)
};

void* MakeOne_00a44a40()                            // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CSuspendableProcessMicrothread;
}