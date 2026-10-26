// CIndexBuffer::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129EF2C), the buffer released (0x00AC1EE0), the CMovableResource base's inline destructor stores its vtable (0x0129B860), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2070, length 48 bytes.
// Leaf: ??_GCIndexBuffer@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x0129EF2C) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CMovableResource { public:                          // vtable 0x0129B860
    virtual void v00();
    ~CMovableResource() { }                        // inline: the vtable store
};
class CIndexBuffer : public CMovableResource { public:              // vtable 0x0129EF2C
    void Release();                                // 0x00AC1EE0 (labelled "CVolume::ReleaseVolume")
    __forceinline virtual ~CIndexBuffer()
    {
        Release();
    }
};

void* MakeOne_00ac2070()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CIndexBuffer;
}