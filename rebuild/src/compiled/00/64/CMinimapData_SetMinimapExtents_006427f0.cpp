// CMinimapData::SetMinimapExtents(const C2DExtentsI& extents) -- store the minimap extents
// Leaf: ?SetMinimapExtents@CMinimapData@@QAEXABVC2DExtentsI@@@Z
// Retail: Fable.exe @ 0x006427F0, 17 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb ?SetMinimapExtents@CMinimapData@@QAEXABVC2DExtentsI@@@Z; an 8-byte copy into +0x00, no return value.
// Callees: none.
// Layout: retail CMinimapData (Ego_r.pdb names, STLport 12-byte vectors): MinimapExtents +0x00, MinimapThemeList +0x08,
// MinimapSurface +0x14 (CSurface vtable 0x0122F84C), MinimapTexture +0x24, MaxMinimapThemeColours +0x2C, ColourMinimapTheme
// +0x30; 0x3C bytes, which the constructor 0x00642A30 initialises member by member. No engine header for CMinimapData;
// engine/CSurface.h flattens the vptr into a data word, so CSurface is a local polymorphic view (as the landed CSurface rows).
// Identity of the class: CMiniMapDisplay's minimap loader 0x0082BAF0 builds one in its frame with 0x00642A30, sets its extents
// with 0x006427F0 and loads it with 0x00642820; Ego_r.pdb orders CMinimapData GetMinimapExtents, SetMinimapExtents,
// PeekDefinitionManager, Get/SetMinimapThemeValue, BuildMinimapTGA, LoadMinimapTGAFile, InitialiseMinimapTexture,
// ResizeMinimapThemeList, CMinimapData(), ~CMinimapData() -- the retail order of these three rows.
class CWideString;
class C2DExtentsI {                                         // 8 bytes
public:
    long Width;                                             // +0x00
    long Height;                                            // +0x04
};
class CPixelFormat {                                        // engine/CPixelFormat.h: FormatIndex +0x00
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
    void Initialise(long rBits, long gBits, long bBits, long aBits, bool linear);   // 0x009E3790
};
struct IDirect3DSurface9;
struct IDirect3DTexture9;
class CSurface {                                            // vtable 0x0122F84C (RTTI CSurface), 0x10 bytes
public:
    IDirect3DSurface9* PD3DSurface;                         // +0x04
    unsigned long      AllocationSource;                    // +0x08
    void*              PAllocatedMemory;                    // +0x0C
    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    virtual ~CSurface();                                    // slot 0 (scalar deleting 0x0041C9C0)
    bool IsInitialised() const { return PD3DSurface != 0; }
    void Uninitialise();                                    // 0x009F2E20
    bool InitialiseAsImage(const C2DExtentsI& size, const CPixelFormat& format, bool dynamic);   // 0x009F3230
    bool LoadFromBitmapFileToPreallocatedSurface(const CWideString& filename, unsigned long filter);   // 0x009F2DD0
};
class CTexture {                                            // 8 bytes (Ego_r.pdb)
public:
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
    CTexture() : PD3DTexture(0), ByteLength(0), AllocationSource(0) { }
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc&, _Value __p) : _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
class CRGBColour { public: unsigned char B, G, R, A; };
class CMinimapData {                                        // retail 0x3C bytes (Ego_r.pdb 0x44, Dinkumware vectors)
public:
    enum MinimapTheme { MINIMAP_THEME_FORCE_DWORD = 0x7fffffff };
    C2DExtentsI                      MinimapExtents;        // +0x00
    std::vector<MinimapTheme>        MinimapThemeList;      // +0x08 (STLport, 12 bytes)
    CSurface                         MinimapSurface;        // +0x14
    CTexture                         MinimapTexture;        // +0x24
    unsigned long                    MaxMinimapThemeColours; // +0x2C
    std::vector<CRGBColour>          ColourMinimapTheme;    // +0x30 (STLport, 12 bytes)
    CMinimapData();
    void SetMinimapExtents(const C2DExtentsI& extents);
    void LoadMinimapTGAFile(const CWideString& filename);
};

void CMinimapData::SetMinimapExtents(const C2DExtentsI& extents)
{
    MinimapExtents = extents;
}