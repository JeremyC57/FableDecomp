// CVolume::InitialiseFromPreallocatedSurface(IDirect3DVolume9* volume) -- release the current Direct3D volume (ReleaseVolume, inlined), adopt and AddRef the given one, then add the volume's memory footprint (Sizeof: 8 bytes for the object, plus the voxel bytes and one word when a volume is held) to two global memory counters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67770, length 96 bytes.
// Callees: IDirect3DVolume9::Release (slot 2) / AddRef (slot 1), CVolume::GetByteLength @ 0x00A674E0; the
// counters 0x013D28DC and 0x013CAAB0 (`add dword ptr [g],eax`).
// Shape: the inlined ReleaseVolume (0x00A67420: null test, Release, one zero store), the AddRef through the
// argument with no null test, then the inline Sizeof evaluated twice (the global store may alias the member):
// `mov ecx,[esi+4] ; test ecx,ecx ; mov eax,8 ; je ; call ; add eax,0Ch` = a size local initialised to
// sizeof(CVolume) with `size += GetByteLength() + 4` under the test (a ternary gives `jmp`/`mov eax,8` arms).
// The manifest's "CSurface" label is propagated; ReleaseVolume / GetByteLength around it are CVolume's.
struct IDirect3DVolume9 {                          // IUnknown slots of the COM volume
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

extern unsigned long g_VolumeMemoryBytes;          // 0x013D28DC
extern unsigned long g_GraphicsMemoryBytes;        // 0x013CAAB0

// CVolume (PDB, 8 bytes): +0 the vptr, +4 the Direct3D volume. No engine header exists for it; the body
// never touches the vptr, so it is kept as an opaque word.
struct CVolume {
    void*             __vftable;                   // +0x00
    IDirect3DVolume9* PD3DVolume;                  // +0x04

    void ReleaseVolume()                           // 0x00A67420
    {
        if (PD3DVolume) {
            PD3DVolume->Release();
            PD3DVolume = 0;
        }
    }
    unsigned long GetByteLength() const;           // 0x00A674E0
    unsigned long Sizeof() const
    {
        unsigned long size = sizeof(CVolume);
        if (PD3DVolume)
            size += GetByteLength() + 4;
        return size;
    }
    void InitialiseFromPreallocatedSurface(IDirect3DVolume9* volume);
};

void CVolume::InitialiseFromPreallocatedSurface(IDirect3DVolume9* volume)
{
    ReleaseVolume();
    PD3DVolume = volume;
    PD3DVolume->AddRef();
    g_VolumeMemoryBytes += Sizeof();
    g_GraphicsMemoryBytes += Sizeof();
}