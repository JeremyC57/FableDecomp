// CWaterPatchMesh::CWaterPatchMesh(CEngineLandscapeMap* map, CDataInputStream* stream, const C2DCoordI* offset) -- a water patch mesh at a landscape offset: map and offset kept, no vertex data yet, then loaded from the stream
// Leaf: ??0CWaterPatchMesh@@QAE@PAVCEngineLandscapeMap@@PAVCDataInputStream@@PBVC2DCoordI@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEB490, 57 bytes (+7 int3 padding), thiscall, `ret 0Ch`.
// Identity: stores the CWaterPatchMesh vtable 0x012A8134; called from CEngineLandscapePatch::LoadForegroundPatch
//   (0x00BF409E) and the landed CWaterPatchMesh::CreateFromStream 0x00BF5A30 (0x00BF5B04), whose header names this
//   constructor's (map, stream, const C2DCoordI*) arguments. Catalog label CWaterPatchMesh::CWaterPatchMesh.
// Callees: CWaterPatchMesh::Load(CDataInputStream*) 0x00BEAE50 (bool result dropped).
// Layout: engine/CWaterPatchMesh.h (Map +4, Offset +8, VertexBuffer +0x10, VertexHeightData +0x14) models the vptr as a
//   data word, so the constructor of the polymorphic class needs a local model with a real vptr (same offsets and names).
// Shape: init-list members (Map, the two Offset words, the xor-zeroed VertexBuffer/VertexHeightData) around the vtable
//   store, then the Load call on `this` with the stream argument. C2DCoordI needs a memberwise user copy constructor:
//   the implicit bitwise copy sinks the Offset.Y store below the Load argument push.
class CEngineLandscapeMap;
class CDataInputStream;
class CVertexBufferWin32;
class CVertexHeightData;
class C2DCoordI {                           // class in Ego_r.pdb (V mangling)
public:
    long X, Y;
    C2DCoordI(const C2DCoordI& other) : X(other.X), Y(other.Y) {}
};
enum EWaterType { WATER_TYPE_NONE };

class CWaterPatchMesh {                                  // vtable 0x012A8134, 0x20 bytes
public:
    CWaterPatchMesh(CEngineLandscapeMap* map, CDataInputStream* stream, const C2DCoordI* offset);
    virtual ~CWaterPatchMesh();
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
    virtual bool CanRelocateData(long handle) const;
    bool Load(CDataInputStream* stream);                 // 0x00BEAE50
    CEngineLandscapeMap* Map;                            // +0x04
    C2DCoordI            Offset;                         // +0x08
    CVertexBufferWin32*  VertexBuffer;                   // +0x10
    CVertexHeightData*   VertexHeightData;               // +0x14
    float                WaterBodySpan;                  // +0x18
    EWaterType           WaterType;                      // +0x1c
};

CWaterPatchMesh::CWaterPatchMesh(CEngineLandscapeMap* map, CDataInputStream* stream, const C2DCoordI* offset)
    : Map(map), Offset(*offset), VertexBuffer(0), VertexHeightData(0)
{
    Load(stream);
}