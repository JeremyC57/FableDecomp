// NThingMap::CThingMap::CThingMap(CThingMapManager* manager, const C2DBoxI& area) -- empty level-info and cell vectors, the manager and area stored, NoThings 0, then InitMapwhoLevels() and Clear()
// Leaf: ??0CThingMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085D370, length 91 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (stores vtable 0x0129A7C4),
// CThingMap::InitMapwhoLevels @ 0x0085D180, CThingMap::Clear @ 0x0085CEC0. Own vtable 0x01276B00.
// Layout (PDB NThingMap::CThingMap, retail vectors 12 bytes): MapwhoLevelInfo +0x04, Manager
// +0x10, Cells +0x14, Area (C2DBoxI) +0x20, NoThings +0x30.
// Shape: the vtable store precedes the zero stores of the first vector = STLport vectors with
// their own inline constructors (`_M_start(0), _M_finish(0), _M_end_of_storage(0)`) sharing the
// ecx zero register; the area is copied dword by dword through edx -- C2DBoxI has a memberwise
// user copy constructor (the implicit POD copy goes through `lea edx,[esi+20h]` and edi); both
// calls in the body.
namespace NThingMap {
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(const C2DBoxI& other) : X0(other.X0), Y0(other.Y0), X1(other.X1), Y1(other.Y1) { }
};
struct CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                       // 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (protected)
public:
    virtual void v00();
};
template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
class CMapwhoLevelInfo;
class CThingMapCell;
class CThingMapManager;
class CThingMap : public CBaseClassNonCopyable { public:
    CRetailVector<CMapwhoLevelInfo>               MapwhoLevelInfo;   // +0x04
    CThingMapManager*                             Manager;           // +0x10
    CRetailVector< CRetailVector<CThingMapCell> > Cells;             // +0x14
    C2DBoxI                                       Area;              // +0x20
    long                                          NoThings;          // +0x30
    CThingMap(CThingMapManager* manager, const C2DBoxI& area);
    virtual void v00();
    void InitMapwhoLevels();                       // 0x0085D180
    void Clear();                                  // 0x0085CEC0
};

CThingMap::CThingMap(CThingMapManager* manager, const C2DBoxI& area)
    : MapwhoLevelInfo(), Manager(manager), Cells(), Area(area), NoThings(0)
{
    InitMapwhoLevels();
    Clear();
}
}