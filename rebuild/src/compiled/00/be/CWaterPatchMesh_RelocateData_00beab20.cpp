// CWaterPatchMesh::RelocateData(long handle, void* oldData, void* newData, long size) -- the landscape memory manager moved a block: rebase the vertex buffer pointer when it referred to the old address
// Leaf: ?RelocateData@CWaterPatchMesh@@UAEXJPAX0J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEAB20, 27 bytes (+5 int3 padding), thiscall, `ret 10h`.
// Identity: slot 1 of the CWaterPatchMesh vtable 0x012A8134 (rtti-vtables.tsv; slot 2 is the landed CanRelocateData
//   0x00BEAA50, slot 0 the deleting destructor 0x00BEB4D0). The (long, void*, void*, long) signature is the one every
//   RelocateData override in the Ego_r.pdb name set carries (`...@@UAEXJPAX0J@Z`).
// Layout: engine/CWaterPatchMesh.h (Map +4, Offset +8, VertexBuffer +0x10, VertexHeightData +0x14, WaterBodySpan +0x18,
//   WaterType +0x1c) models the vptr as a data word, so a class declaring the virtual cannot be built on it: the same
//   offsets and names are modelled locally behind a real vptr.
// Shape: the landed CEngineLandscapePatch::RelocateData 0x00BF3980 rebase helper: `if (oldData == p)` then the
//   null-checked `p = p - oldData + newData` (retail's `sub ; add` order).
class CEngineLandscapeMap;
class CVertexBufferWin32;
class CVertexHeightData;
class C2DCoordI { public: long X, Y; };
enum EWaterType { WATER_TYPE_NONE };

template <class T> inline void RelocatePointer(T*& p, void* oldData, void* newData)
{
    if (p)
        p = (T*)((char*)p - (char*)oldData + (char*)newData);
}

class CWaterPatchMesh {                                  // vtable 0x012A8134, 0x20 bytes
public:
    virtual ~CWaterPatchMesh();
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
    virtual bool CanRelocateData(long handle) const;
    CEngineLandscapeMap* Map;                            // +0x04
    C2DCoordI            Offset;                         // +0x08
    CVertexBufferWin32*  VertexBuffer;                   // +0x10
    CVertexHeightData*   VertexHeightData;               // +0x14
    float                WaterBodySpan;                  // +0x18
    EWaterType           WaterType;                      // +0x1c
};

void CWaterPatchMesh::RelocateData(long handle, void* oldData, void* newData, long size)
{
    if (oldData == VertexBuffer)
        RelocatePointer(VertexBuffer, oldData, newData);
}