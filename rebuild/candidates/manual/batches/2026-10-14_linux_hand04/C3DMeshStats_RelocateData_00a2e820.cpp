// C3DMeshStats::RelocateData(long handle, void* oldBase, void* newBase, long) -- when the handle is ours, rebase PackedNames and the two pre-allocated map element arrays
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E820, oracle length 64 bytes.
// Callees: none. Virtual (manifest decorated name ?RelocateData@C3DMeshStats@@UAEXJPAX0J@Z).
//
// Retail bytes:  mov eax,[esp+4] ; cmp eax,[ecx+8] ; jne R ; mov eax,[ecx+74h] ; test eax,eax ; mov edx,[esp+0Ch] ; push esi ; mov esi,[esp+0Ch] ;
//                je A ; sub eax,esi ; add eax,edx ; mov [ecx+74h],eax ; A: mov eax,[ecx+3Ch] ; test eax,eax ; je B ; sub eax,esi ; add eax,edx ;
//                mov [ecx+3Ch],eax ; B: mov eax,[ecx+4Ch] ; test eax,eax ; je C ; sub eax,esi ; add eax,edx ; mov [ecx+4Ch],eax ; C: pop esi ; R: ret 10h
// Retail layout (see the constructor @ 0x00A2F210 for the full table): +8 StatsMemoryHandle, +0x3C
// HelperPoints.Elements, +0x4C DummyObjects.Elements, +0x74 PackedNames. Only the memory block whose
// handle matches StatsMemoryHandle holds these arrays; each non-null pointer is moved by the block's
// delta (p = p - oldBase + newBase). The size argument is unused.
struct CKeyPairCompareLess { };

template <class T> struct CPreAllocVectorMap {   // retail: 16 bytes
    CKeyPairCompareLess Compare;           // +0
    T*                  Elements;          // +4
    unsigned long       Size;              // +8
    bool                Dirty;             // +0xC
};

struct C3DHelperPointPair;                 // pair<ulong, C3DHelperPoint>
struct C3DDummyObjectPair;                 // pair<ulong, C3DDummyObject>

struct C3DMeshStats {
    virtual void v00();
    void*         MemoryPool;              // +0x04
    long          StatsMemoryHandle;       // +0x08
    unsigned char _pad_0x0c[0x38 - 0x0c];  // +0x0C Name, BoundingSphere, BoundingBox
    CPreAllocVectorMap<C3DHelperPointPair> HelperPoints;   // +0x38
    CPreAllocVectorMap<C3DDummyObjectPair> DummyObjects;   // +0x48
    unsigned char _pad_0x58[0x74 - 0x58];  // +0x58 MeshVolumes, MeshGenerators
    char*         PackedNames;             // +0x74
    virtual void RelocateData(long handle, void* oldBase, void* newBase, long size);
};

void C3DMeshStats::RelocateData(long handle, void* oldBase, void* newBase, long /*size*/)
{
    if (handle != StatsMemoryHandle)
        return;
    if (PackedNames)
        PackedNames = PackedNames - reinterpret_cast<char*>(oldBase) + reinterpret_cast<char*>(newBase);
    if (HelperPoints.Elements)
        HelperPoints.Elements = reinterpret_cast<C3DHelperPointPair*>(reinterpret_cast<char*>(HelperPoints.Elements) - reinterpret_cast<char*>(oldBase) + reinterpret_cast<char*>(newBase));
    if (DummyObjects.Elements)
        DummyObjects.Elements = reinterpret_cast<C3DDummyObjectPair*>(reinterpret_cast<char*>(DummyObjects.Elements) - reinterpret_cast<char*>(oldBase) + reinterpret_cast<char*>(newBase));
}
