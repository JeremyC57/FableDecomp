// CEngineScene::GetMapAt(const C2DVector& position) -- the scene map covering a world XY position: the cell (floor of position * cell scale) is looked up in the scene's map table and a new CEngineSceneMap covering that cell is created and stored when the slot is empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B94BF0, length 309 bytes (frame pointer, `ret 4`).
// Callees: the nested map's operator[] pair @ 0x00B988E0 (outer, keyed by the row y) and @ 0x00B97C10 (inner, keyed by x;
// the two `const long&` arguments are the cell's members, pushed outer-call-first), operator new @ 0x00BFEA1A
// (0x28 bytes), CBase::CBase @ 0x0099A2F0, CEngineSceneMap::Initialise @ 0x00B949A0 (landed), CCountedPointer<CEngineSceneMap>::Reset
// @ 0x00B95AA0. Vtable 0x012A3E60 = CEngineSceneMap. 0.5f / 1.0f immediates (0x3F000000 / 0x3F800000).
// Layout (descriptive): the scene's map table +0x04 (map<int, map<int, CCountedPointer<CEngineSceneMap>>>), cell size +0x10,
// inverse cell size +0x14, map level limit +0x18. The area is built from a min corner and a (CellSize, CellSize) extent.
#pragma optimize("y", off)
// Retail-asm: 0x00B94C13..0x00B94C2D (`fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fadd [ebp-4] ; fstp [ebp-10h] ; mov eax,[ebp-0Ch] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp+8]`, the result homed in the dead `position` slot) and 0x00B94C4D..0x00B94C67 -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
};
struct C2DCoordI { long X, Y; };
struct C2DBoxF {
    C2DVector Min, Max;
    C2DBoxF() { }
    C2DBoxF(float x0, float y0, float x1, float y1) : Min(x0, y0), Max(x1, y1) { }
};
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
struct CEngineScene;
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};
struct CCountedGrid { void* m_pObject; void* m_pCount; };
template<class T> struct CRetailArray {
    T* first; T* last; T* end_of_storage;
    CRetailArray() : first(0), last(0), end_of_storage(0) { }
};
struct CEngineSceneMap : CBase {                   // vtable 0x012A3E60, 0x28 bytes
    C2DBoxF       Area;                            // +0x04
    unsigned long MaxLevels;                       // +0x14
    CRetailArray<CCountedGrid> GridLevels;         // +0x18
    CEngineScene* Scene;                           // +0x24
    void Initialise(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area);   // 0x00B949A0
    CEngineSceneMap(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area) { Initialise(scene, maxLevels, area); }
    virtual void v00();
};
struct CMapPointer {                               // CCountedPointer<CEngineSceneMap>
    CEngineSceneMap* m_pObject;
    long*            m_pCount;
    void Reset(CEngineSceneMap* object);           // 0x00B95AA0
};
struct CMapRow {                                   // STLport map<long, CCountedPointer<CEngineSceneMap>>: the inner (x) map
    CMapPointer& operator[](const long& x);        // 0x00B97C10 (map::operator[]: insert(value_type(key, T())).first->second)
};
struct CMapTable {                                 // STLport map<long, map<long, ...>>: the outer (y) map
    CMapRow& operator[](const long& y);            // 0x00B988E0
};
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
struct CEngineScene {
    unsigned char  _pad00[4];
    CMapTable      Maps;                           // +0x04
    unsigned char  _pad08[8];
    float          CellSize;                       // +0x10
    float          InverseCellSize;                // +0x14
    unsigned long  MaxLevels;                      // +0x18
    CEngineSceneMap* GetMapAt(const C2DVector& position);
};
CEngineSceneMap* CEngineScene::GetMapAt(const C2DVector& position)
{
    C2DCoordI cell;
    cell.X = FloorToLong(position.X * InverseCellSize);
    cell.Y = FloorToLong(position.Y * InverseCellSize);
    CMapPointer& slot = Maps[cell.Y][cell.X];
    if (slot.m_pObject == 0) {
        float minX = cell.X * CellSize;
        float minY = cell.Y * CellSize;
        C2DBoxF area(minX, minY, minX + CellSize, minY + CellSize);
        slot.Reset(new CEngineSceneMap(this, MaxLevels, area));
    }
    return slot.m_pObject;
}
