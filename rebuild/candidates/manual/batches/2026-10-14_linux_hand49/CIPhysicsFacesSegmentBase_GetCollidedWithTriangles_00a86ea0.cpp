// CIPhysicsFacesSegmentBase::GetCollidedWithTriangles(CWorldMap* map, long a, long b, long c, long d) -- grow the global per-map collision stamp vector (0x013D2D48, resize @ 0x004428F0 filled with the current stamp) to the world map's map count (0x00A6C560), bump the stamp (0x013D2D44) and run this segment's virtual slot 4 with the four remaining arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A86EA0, length 91 bytes.
// Shape: `this` in esi, `sar ecx,2 ; cmp ecx,eax ; jae`, the stamp incremented between the argument loads, `ret 14h`.
struct CWorldMap {
    long GetNoMaps();                              // 0x00A6C560 (labelled "DrawGetNoMaps")
};
struct CRetailVector_long {                        // {first, last, end}
    long* first;
    long* last;
    long* end;
    void Resize(unsigned long n, const long& value);   // 0x004428F0
    unsigned long Size() { return last - first; }
};
extern long               g_collisionStamp_013d2d44;   // 0x013D2D44
extern CRetailVector_long g_mapStamps_013d2d48;        // 0x013D2D48 (one stamp per map)
struct CIPhysicsFacesSegmentBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void CollectCollidedTriangles(long a, long b, long c, long d);   // slot 4 (+0x10)
    void GetCollidedWithTriangles(CWorldMap* map, long a, long b, long c, long d);
};

void CIPhysicsFacesSegmentBase::GetCollidedWithTriangles(CWorldMap* map, long a, long b, long c, long d)
{
    long count = map->GetNoMaps();
    if (g_mapStamps_013d2d48.Size() < (unsigned long)count)
        g_mapStamps_013d2d48.Resize(count, g_collisionStamp_013d2d44);
    ++g_collisionStamp_013d2d44;
    CollectCollidedTriangles(a, b, c, d);
}
