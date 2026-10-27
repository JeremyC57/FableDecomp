// CEngineLandscapeRenderer::Clear() -- clear the landscape of every loaded engine map: walk the world map's MapList and call CEngineLandscapeMap::Clear on each map that has a landscape
// Leaf: ?Clear@CEngineLandscapeRenderer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B676F0, 69 bytes, thiscall (this unused), plain `ret`.
// Identity (probable): Ego_r.pdb lays the class out as OnReleaseDefaultPoolResources (0x009A89B0), Clear (0x009A89F0), then the
//   console commands SetMeshDetail / SetProceduralTextureDetail / SetForegroundFade; retail has the same run -- the landed
//   OnReleaseDefaultPoolResources 0x00B676A0 (this body's twin, calling ReleaseDefaultPoolSurfaces), this function, then the
//   three static fastcall console commands at 0x00B67740 / 0x00B67750 / 0x00B67760, each storing its CGenericVar argument
//   into the renderer and tail-jumping here (0x00B6774B, 0x00B6775B, 0x00B6777B). Clear is the only no-argument method that
//   fits; its callee 0x00BDC450 frees the map's landscape buffers, i.e. CEngineLandscapeMap::Clear() (Ego_r.pdb name).
// Callees: CEngineLandscapeMap::Clear() 0x00BDC450 (descriptive binding in the catalog: cl_h_00bdc4f0).
// Layout: Ego_r.pdb CEngineWorldMap.MapList (CArray<CEngineMap*>) +0x20 (retail STLport vector: _M_start +0x20, _M_finish
//   +0x24), CEngineMap.LandscapeMap +0x00; g_pEngineWorldMap 0x01436E8C, re-read after each call. Local model --
//   engine/CEngineWorldMap.h carries MapList as opaque bytes.
class CEngineLandscapeMap {
public:
    void Clear();                                        // 0x00BDC450
};
class CEngineMap {
public:
    CEngineLandscapeMap* LandscapeMap;                   // +0x00
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _Tp* _M_end_of_storage;                              // +0x08
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };
class CEngineWorldMap {
public:
    unsigned char      _pad_0x00[0x20];                  // vptr, Map, MapArrayWidth/Height, MapOffsetX/Y
    CArray<CEngineMap*> MapList;                         // +0x20
};
extern CEngineWorldMap* g_pEngineWorldMap;               // 0x01436E8C

class CEngineLandscapeRenderer {
public:
    void Clear();
};

void CEngineLandscapeRenderer::Clear()
{
    for (long i = 0; i < (long)g_pEngineWorldMap->MapList.size(); ++i)
    {
        CEngineMap* map = g_pEngineWorldMap->MapList[i];
        if (map != 0 && map->LandscapeMap != 0)
            map->LandscapeMap->Clear();
    }
}