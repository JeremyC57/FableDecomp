// Grade: DIFFER(337v337) -- only the count/index register pair is swapped (retail count=ebx, index=ebp; lane count=ebp, index=ebx) with the matching push/pop order
// CWorldMap::UpdateDestroyedThemeAreas() -- walk the queued destroyed-theme positions, growing one box around
// neighbouring positions; whenever a position would stretch the box beyond 5 cells it reports the current box
// (bordered by one cell) as a changed height area and starts a new box there; the last box is reported too and the
// queue is emptied
// Leaf: UpdateDestroyedThemeAreas
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FEC80, 337 bytes, `ret`. PDB ?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ.
// Callees: C2DBoxI::ExtendToContain(const C2DCoordI&) 0x00A39E20, C2DBoxI::ExtendBordersBy(long) 0x00A39E10,
//   CWorldMap::HeightAreaChanged(const C2DBoxI*) virtual slot 28 (+0x70, vtable 0x01244AEC -> 0x004FEB70).
// Layout: DestroyedThemePositions (vector<C2DCoordI>) at retail +0xb0 (PDB +0xd0; retail CWorldMap is smaller, see
//   the landed CWorldMap rows), so the class is modelled locally; the queue is emptied by the STLport
//   erase(begin(), end()) inline (copy of the empty tail, then the finish store).
struct C2DCoordI {
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(const C2DCoordI& o) : X(o.X), Y(o.Y) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& tl, const C2DCoordI& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) { }
    void ExtendToContain(const C2DCoordI& pos);    // 0x00A39E20
    void ExtendBordersBy(long border);             // 0x00A39E10
};
template <class T> struct vector {                 // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long n) { return _M_start[n]; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    static T* __copy(T* first, T* last, T* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    T* erase(T* first, T* last)
    {
        T* i = __copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
};
struct CWorldMap {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void HeightAreaChanged(const C2DBoxI* area);   // slot 28 (+0x70)
    unsigned char     _pad04[0xb0 - 4];
    vector<C2DCoordI> DestroyedThemePositions;     // +0xb0
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
        C2DBoxI extended = area;
        extended.ExtendToContain(pos);
        if (extended.BRX - extended.TLX > 5 || extended.BRY - extended.TLY > 5) {
            area.ExtendBordersBy(1);
            HeightAreaChanged(&area);
            area = C2DBoxI(pos, pos);
        }
        else
            area = extended;
    }
    area.ExtendBordersBy(1);
    HeightAreaChanged(&area);
    DestroyedThemePositions.erase(DestroyedThemePositions.begin(), DestroyedThemePositions.end());
}
