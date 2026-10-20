// C3DMeshFileMaterialChunk::GetMap(EMaterialMapType) -- find the requested MMAP child by its map type (non-const overload)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC1020, 310 bytes (`ret 8`: hidden CCountedPointer result + type).
// Ego_r.pdb: ?GetMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@W4EMaterialMapType@@@Z.
// Callees: C3DMeshFileMaterialChunk::FindFirstMap() 0x00AC0CE0 and FindNextMap(const CCountedPointer<const
// C3DMeshFileMapChunk>&) 0x00AC0D70 (manifest "C3DMeshFileSubMeshChunk::FindFirstBone/FindNextBone" are propagated),
// operator delete 0x00BFE9BC (count blocks). The two finders are the previous functions of this TU: stand-in bodies
// in the shape of their retail code (FindFirstSubChunkOfType 0x00AA80E0 / FindNextSubChunk 0x00AA8190) sit above under
// auto_inline(off), as in the landed const twin 0x00AC1160.
// Shape: the landed const GetMap loop (`while (map && map->MapType != type) map = FindNextMap(map);`), here with the
// argument converted to a CCountedPointer<const ...> temporary (add-ref copy) and the counted pointer's zeroing dtor.
// Layout: C3DMeshFileMapChunk MapType +0x2c (retail compare).
// Leaf: ?GetMap@C3DMeshFileMaterialChunk@@QAE
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> struct CCountedPointer {
    T* Object; SCountBlock* Count;
    CCountedPointer() : Object(0), Count(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->references; }
    template<class U> CCountedPointer(const CCountedPointer<U>& o) : Object((T*)o.Object), Count(o.Count) { if (Count) ++Count->references; }
    ~CCountedPointer() { if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } } Object = 0; Count = 0; }
    struct CastTag {};
    template<class U> CCountedPointer(const CCountedPointer<U>& o, CastTag) : Object(0), Count(0) { if (o.Count) { Object = (T*)o.Object; Count = o.Count; ++Count->references; } }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        SCountBlock* c = o.Count; T* p = o.Object;
        if (Count != c) {
            if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
            Object = p; Count = c; if (c) ++c->references;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
struct CChunkedFileChunk;
enum EMaterialMapType { MAP_DIFFUSE = 0, MAP_BUMP = 1, MAP_REFLECTION = 2, MAP_ILLUMINATION = 3 };
struct C3DMeshFileMapChunk { unsigned char _pad[0x2c]; EMaterialMapType MapType; };

struct C3DMeshFileMaterialChunk {
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type);   // 0x00AA80E0
    CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& current);   // 0x00AA8190
    CCountedPointer<C3DMeshFileMapChunk> FindFirstMap();                                // 0x00AC0CE0
    CCountedPointer<C3DMeshFileMapChunk> FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>& current);   // 0x00AC0D70
    CCountedPointer<C3DMeshFileMapChunk> GetMap(EMaterialMapType type);
};

#pragma auto_inline(off)
CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::FindFirstMap()
{
    return CCountedPointer<C3DMeshFileMapChunk>(FindFirstSubChunkOfType("MMAP"), CCountedPointer<C3DMeshFileMapChunk>::CastTag());
}
CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>& current)
{
    CCountedPointer<CChunkedFileChunk> prev(current);
    return CCountedPointer<C3DMeshFileMapChunk>(FindNextSubChunk(prev), CCountedPointer<C3DMeshFileMapChunk>::CastTag());
}
#pragma auto_inline(on)

CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::GetMap(EMaterialMapType type)
{
    CCountedPointer<C3DMeshFileMapChunk> map = FindFirstMap();
    while (map.Object && map->MapType != type)
        map = FindNextMap(map);
    if (map.Object)
        return map;
    return CCountedPointer<C3DMeshFileMapChunk>();
}
