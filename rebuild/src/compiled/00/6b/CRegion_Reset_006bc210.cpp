// CRegion::Reset() -- empties the Maps list, blanks the three names and the mini-map graphic, re-enables the world-map/creature-generation/sound-theme flags and resets the mini-map scale and the offsets
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC210, length 124 bytes (padded to 128), `ret`.
// Catalog label "CV_SingingStonesScript::Init" is a wrong propagation: the body sits between CRegion::SetRegionDefName
// (0x006BC1D0) and CRegion::GetWorldMapNameGraphic (0x006BC380) and its stores fit the PDB CRegion layout (retail
// STLport vectors are 12 bytes, so everything after VisMaps sits 8 bytes below the Ego_r.pdb offsets). No direct
// caller or table reference was found; the PDB name is not known, "Reset" is descriptive.
// Callees: memmove @ 0x00BFEAE6 (the inline STLport `__copy_trivial` of vector<long>::clear), CCharString::operator=
// (const char*) @ 0x0099EFE0 (x4, the empty literal @ 0x0122D70E).
// Layout (retail): Maps +0x00, VisMaps +0x0c, Name +0x18, DisplayName +0x1c, RegionDefName +0x20, PDef +0x24,
// MiniMapGraphic +0x28, MiniMapScale +0x2c, MiniMapOffset +0x30, MiniMapRegionExitTextOffsets +0x38, WorldMapOffset
// +0x44, NameGraphicOffset +0x4c, AppearOnWorldMap +0x54, CreatureGenerationEnabled +0x55, SoundThemesEnabled +0x56.
// Shape: the two zero offsets are written through an inline C2DVector::Set(x, y) member (stores through the member's `this`):
//   that keeps `xor eax,eax` after the MiniMapScale 1.0f immediate store as retail; direct member stores let the lane hoist the xor.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const char* text);      // 0x0099EFE0
};
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}
struct CLongVector {                               // STLport vector<long> (12 bytes in retail)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    long* erase(long* first, long* last)
    {
        long* i = (long*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
class C2DVector { public:
    void Set(float x, float y) { X = x; Y = y; }
    float X; float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CRegion { public:
    CLongVector Maps;                              // +0x00
    CLongVector VisMaps;                           // +0x0c
    CCharString Name;                              // +0x18
    CCharString DisplayName;                       // +0x1c
    CCharString RegionDefName;                     // +0x20
    void*       PDef;                              // +0x24 (CDefPointer<CRegionDef const>)
    CCharString MiniMapGraphic;                    // +0x28
    float       MiniMapScale;                      // +0x2c
    C2DVector   MiniMapOffset;                     // +0x30
    void*       MiniMapRegionExitTextOffsets[3];   // +0x38 (map<CCharString,C2DVector>)
    C2DVector   WorldMapOffset;                    // +0x44
    C2DVector   NameGraphicOffset;                 // +0x4c
    bool        AppearOnWorldMap;                  // +0x54
    bool        CreatureGenerationEnabled;         // +0x55
    bool        SoundThemesEnabled;                // +0x56
    void Reset();
};
void CRegion::Reset()
{
    Maps.clear();
    Name = "";
    DisplayName = "";
    RegionDefName = "";
    AppearOnWorldMap = true;
    CreatureGenerationEnabled = true;
    SoundThemesEnabled = true;
    MiniMapGraphic = "";
    MiniMapScale = 1.0f;
    MiniMapOffset.Set(0.0f, 0.0f);
    WorldMapOffset.Set(0.0f, 0.0f);
}