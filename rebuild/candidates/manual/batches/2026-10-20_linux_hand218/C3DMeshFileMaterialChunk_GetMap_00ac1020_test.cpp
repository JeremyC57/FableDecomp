// Behaviour test for 0x00AC1020 C3DMeshFileMaterialChunk::GetMap (non-const), model copy over a stub chunk list.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
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
    C3DMeshFileMapChunk* Maps; SCountBlock* Blocks; int Count;
    CCountedPointer<CChunkedFileChunk> Make(int i)
    {
        CCountedPointer<CChunkedFileChunk> p;
        if (i < Count) { p.Object = (CChunkedFileChunk*)&Maps[i]; p.Count = &Blocks[i]; ++Blocks[i].references; }
        return p;
    }
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char*) { return Make(0); }
    CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& current)
    {
        return Make((int)((C3DMeshFileMapChunk*)current.Object - Maps) + 1);
    }
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

static int g_destroyed = 0;
static void __fastcall Destroy(void*) { ++g_destroyed; }

int main()
{
    C3DMeshFileMapChunk maps[3]; SCountBlock blocks[3];
    for (int i = 0; i < 3; ++i) { blocks[i].references = 1; blocks[i].destroy = Destroy; blocks[i].object = &maps[i]; }
    maps[0].MapType = MAP_DIFFUSE; maps[1].MapType = MAP_REFLECTION; maps[2].MapType = MAP_ILLUMINATION;
    C3DMeshFileMaterialChunk chunk; chunk.Maps = maps; chunk.Blocks = blocks; chunk.Count = 3;
    {
        CCountedPointer<C3DMeshFileMapChunk> m = chunk.GetMap(MAP_ILLUMINATION);
        if (m.Object != &maps[2] || m.Count != &blocks[2] || blocks[2].references != 2) { std::printf("00ac1020_TEST FAIL\n"); return 1; }
        if (blocks[0].references != 1 || blocks[1].references != 1) { std::printf("00ac1020_TEST FAIL\n"); return 1; }
    }
    {
        CCountedPointer<C3DMeshFileMapChunk> m = chunk.GetMap(MAP_BUMP);
        if (m.Object != 0 || m.Count != 0) { std::printf("00ac1020_TEST FAIL\n"); return 1; }
    }
    if (blocks[0].references != 1 || blocks[1].references != 1 || blocks[2].references != 1 || g_destroyed != 0) {
        std::printf("00ac1020_TEST FAIL\n"); return 1;
    }
    std::printf("00ac1020_TEST PASS\n");
    return 0;
}
