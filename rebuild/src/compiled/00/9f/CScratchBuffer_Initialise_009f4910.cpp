// CScratchBuffer::Initialise(long noBytes) -- clears Buffer, runs the base initialisation, allocates noBytes and records the size; false on failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4910, length 61 bytes.
// Leaf: ?Initialise@CScratchBuffer
// Retail bytes: push esi ; mov esi,ecx ; push edi ; mov dword ptr [esi+8],0 ; call CInitBaseClass::Init ; test al,al ; je F ; mov edi,[esp+0Ch] ; push edi ;
//               call operator new[] ; add esp,4 ; test eax,eax ; mov [esi+8],eax ; je F ; mov [esi+10h],edi ; pop edi ; mov byte ptr [esi+0Ch],0 ;
//               mov al,1 ; pop esi ; ret 4 ; F: pop edi ; xor al,al ; pop esi ; ret 4
// Callees: CInitBaseClass::Init @ 0x0099A350 (`Valid = true ; return true`; the manifest label "CXMVPlaybackInfo" is
// propagated) called on `this` directly, operator new[] @ 0x00BFEB22.
// Layout (PDB): CInitBaseClass base {vptr, bool Valid}, Buffer +8, Locked +0xc, NoBytes +0x10 (vtable 0x0129C790, the
// landed constructor 0x009F4A00). Shape: the nested-if form -- both failures share the `xor al,al` tail at the end
// (early returns would duplicate it and push edi late).
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)
class CInitBaseClass { public:
    virtual void v00();
    bool Valid;                                // +0x04
    bool Init();                               // 0x0099A350
};

class CScratchBuffer : public CInitBaseClass { public:
    unsigned char* Buffer;                     // +0x08
    bool           Locked;                     // +0x0c
    long           NoBytes;                    // +0x10
    bool Initialise(long noBytes);
};

bool CScratchBuffer::Initialise(long noBytes)
{
    Buffer = 0;
    if (Init()) {
        Buffer = new unsigned char[noBytes];
        if (Buffer) {
            NoBytes = noBytes;
            Locked = false;
            return true;
        }
    }
    return false;
}