// CMap::Update() -- per-frame map update: update the height map, forget this frame's changed height / game areas, then advance the scripted map brushes
// Leaf: ?Update@CMap@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00820B50, 36 bytes, thiscall, tail jump to UpdateScriptedMapBrushes.
// Identity (proven): RTTI vtable 0x0127252C (.?AVCMap@@; bases CIDrawMap at +0, CBaseClassNonCopyable at +4) slot 13, the last
//   slot, after the three protected virtual ScriptedMapBrush{Add,Move,Remove}Things (slots 10-12, landed 0x00820790 / 0x00820840 /
//   0x008209C0); Ego_r.pdb ?Update@CMap@@MAEXXZ (protected virtual) sits right after UpdateScriptedMapBrushes in donor order, as
//   retail 0x00820B50 follows UpdateScriptedMapBrushes 0x00820AB0. The catalog label is right.
// Callees: CHeightMap::Update() 0x008F5F00 (Ego_r ?Update@CHeightMap@@AAEXXZ: private, so CMap is a friend of CHeightMap),
//   CMap::UpdateScriptedMapBrushes() 0x00820AB0 (Ego_r ?UpdateScriptedMapBrushes@CMap@@IAEXXZ, protected).
// Layout: retail CMap (the landed ctor 0x00820CC0): HeightMap (boost::scoped_ptr<CHeightMap>) +0x1A7C, NoChangedHeightAreas
//   +0x1D00, NoChangedGameAreas +0x1D04 (Ego_r +0x1A8C / +0x1D10 / +0x1D14 less the 12-byte STLport ThemePaletteEntryList shrink
//   and the 4-byte STLport brush list). There is no engine/CMap.h.
// Shape: `X = 0; Y = 0;` share one `xor eax,eax` after the height-map call.
// Layout: engine/CIDrawMap.h and engine/CBaseClass.h are generated flat headers (vptr as a data member, no virtual functions); CMap
//   needs the real polymorphic bases (CIDrawMap at +0, CBaseClassNonCopyable : CBaseClass at +4, RTTI of vtable 0x0127252C), so both
//   are restated as polymorphic classes here.
class CHeightMap {
    friend class CMap;
private:
    void Update();                                          // 0x008F5F00
};

namespace boost {
template <class T> class scoped_ptr {
public:
    T* operator->() const { return ptr; }
private:
    T* ptr;
};
}

class CIDrawMap {
public:
    virtual ~CIDrawMap();
};
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
public:
};

class CMap : public CIDrawMap, public CBaseClassNonCopyable {
protected:
    void UpdateScriptedMapBrushes();                        // 0x00820AB0
    virtual void Update();
public:
    unsigned char                  _pad08[0x1a7c - 0x08];
    boost::scoped_ptr<CHeightMap>  HeightMap;               // +0x1A7C
    unsigned char                  _pad1a80[0x1d00 - 0x1a80];   // ChangedHeightAreas / ChangedGameAreas (C2DBoxI[20] each)
    long                           NoChangedHeightAreas;    // +0x1D00
    long                           NoChangedGameAreas;      // +0x1D04
};

void CMap::Update()
{
    HeightMap->Update();
    NoChangedHeightAreas = 0;
    NoChangedGameAreas = 0;
    UpdateScriptedMapBrushes();
}