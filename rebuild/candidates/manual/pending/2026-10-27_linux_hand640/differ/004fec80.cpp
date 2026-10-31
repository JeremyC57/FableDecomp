// CWorldMap::UpdateDestroyedThemeAreas() -- grow one box over the queued destroyed-theme positions; when a position would stretch it beyond 5 cells, report the box (bordered by one cell) as a changed height area and restart it there; report the last box and empty the queue
// Grade: DIFFER(337v337) -- register colouring tie: count/index swapped (retail count=ebx, index=ebp; ours count=ebp, index=ebx, with the matching push order) | this session: PDB respell (FableWin C2DBoxI copy ctor, GetWidth/GetHeight), separate first/pos locals (321), pos declared before count (same), extended copied before pos (308)
// Leaf: ?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FEC80, 337 bytes, `ret`. Ego_r.pdb ?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ
//   (protected; caller CWorldMap::Update 0x004FF920, reloc binding).
// Callees: C2DBoxI::ExtendToContain(const C2DCoordI&) 0x00A39E20 (Ego_r ?ExtendToContain@C2DBoxI@@QAEXABVC2DCoordI@@@Z),
//   C2DBoxI::ExtendBordersBy(long) 0x00A39E10 (Ego_r ?ExtendBordersBy@C2DBoxI@@QAEXJ@Z), CWorldMap::HeightAreaChanged(const
//   C2DBoxI*) virtual slot 28 (+0x70; Ego_r ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z, vtable 0x01244AEC -> 0x004FEB70).
// Layout: DestroyedThemePositions (STLport vector<C2DCoordI>) at retail +0xB0 (Ego_r.pdb +0xD0: Dinkumware sizes);
//   engine/CWorldMap.h flattens the vptr, so CWorldMap is the local model of the landed 0x004FF920 (slot names from there).
//   C2DBoxI / C2DCoordI: FableWin.pdb inline ctors (??0C2DBoxI@@QAE@ABVC2DCoordI@@0@Z, copy ctors) and GetWidth/GetHeight.
// Shape: the box is held in four registers and spilled each pass; the clear() is STLport erase(begin, end) (copy loop of
//   the empty tail, then the finish store).
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(const C2DCoordI& o) : X(o.X), Y(o.Y) { }
};
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& tl, const C2DCoordI& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) { }
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) { }
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
    void ExtendToContain(const C2DCoordI& pos);    // 0x00A39E20
    void ExtendBordersBy(long border);             // 0x00A39E10
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* result = first;
        for (long n = _M_finish - last; n > 0; --n) {
            *result = *last;
            ++last;
            ++result;
        }
        _M_finish = result;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
}
class CWorldMap {
public:
    virtual ~CWorldMap();
    virtual void DeleteAllLevels();
    virtual void DeleteAllRegions();
    virtual void LoadFromFile();
    virtual void SaveToFile();
    virtual void RemoveLevel(long map);
    virtual void LoadTopologyFromFile();
    virtual void PostLoadTopology(long map);
    virtual void ForceLoad(long map);
    virtual void ForceUnload(long map);
    virtual bool IsMapLoaded(long map) const;
    virtual void _slot11(); virtual void _slot12(); virtual void _slot13(); virtual void _slot14();
    virtual void _slot15(); virtual void _slot16(); virtual void _slot17(); virtual void _slot18();
    virtual void _slot19(); virtual void _slot20(); virtual void _slot21(); virtual void _slot22();
    virtual void _slot23(); virtual void _slot24(); virtual void _slot25(); virtual void _slot26();
    virtual void Update();                                 // slot 27
    virtual void HeightAreaChanged(const C2DBoxI* area);   // slot 28 (+0x70) 0x004FEB70
    unsigned char                _pad04[0xb0 - 4];
    std::vector<C2DCoordI>       DestroyedThemePositions;  // +0xB0
protected:
    void UpdateDestroyedThemeAreas();
};
void CWorldMap::UpdateDestroyedThemeAreas()
{
    long count = DestroyedThemePositions.size();
    if (count == 0)
        return;
    C2DCoordI pos = DestroyedThemePositions[0];
    C2DBoxI area(pos, pos);
    for (long i = 0; i < count; ++i) {
        pos = DestroyedThemePositions[i];
        C2DBoxI extended(area);
        extended.ExtendToContain(pos);
        if (extended.GetWidth() > 5 || extended.GetHeight() > 5) {
            area.ExtendBordersBy(1);
            HeightAreaChanged(&area);
            area = C2DBoxI(pos, pos);
        }
        else
            area = extended;
    }
    area.ExtendBordersBy(1);
    HeightAreaChanged(&area);
    DestroyedThemePositions.clear();
}
