// Grade: IMPROVED(297v311) retry1 -- retail_asm FloorToLong in place; residue: helper slot packing + the area box x87 chain
// CEngineScene::GetMapAt(const C2DVector& position) -- the scene map covering a world XY position: the cell (floor of position * cell scale) is looked up in the scene's map table and a new CEngineSceneMap covering that cell is created and stored when the slot is empty
// Retail-asm: 0x00B94C16 `fsub [ebp-8]` + 0x00B94C19 `fistp dword ptr [ebp+8]` -- the engine inline-assembly FloorToLong (landed 0x006A68F0), expanded twice; VC7.1 C++ (long)x calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B94BF0, length 309 bytes (frame pointer, `ret 4`).
// Callees: the map-table lower-bound/insert @ 0x00B988E0 and value accessor @ 0x00B97C10 (on +0x04), operator new @ 0x00BFEA1A
// (0x28 bytes), CBase::CBase @ 0x0099A2F0, CEngineSceneMap::Initialise @ 0x00B949A0 (landed), CCountedPointer<CEngineSceneMap>::Reset
// @ 0x00B95AA0. Vtable 0x012A3E60 = CEngineSceneMap. 0.5f / 1.0f immediates (0x3F000000 / 0x3F800000).
// Layout (descriptive): the scene's map table +0x04, cell size +0x10, inverse cell size +0x14, map level limit +0x18.
struct C2DVector { float X, Y; };
struct C2DCoordI { long X, Y; };
struct C2DBoxF { float MinX, MinY, MaxX, MaxY; };
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
struct SMapIterator {
    void* m_pNode;
    CMapPointer* Value();                          // 0x00B97C10
};
struct CMapTable {
    SMapIterator* LowerBoundOrInsert(SMapIterator* result, const C2DCoordI& key);   // 0x00B988E0
};
// the engine's inline-assembly floor helper (the landed CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong, verbatim)
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
    SMapIterator it;
    CMapPointer* slot = Maps.LowerBoundOrInsert(&it, cell)->Value();
    if (slot->m_pObject == 0) {
        C2DBoxF area;
        area.MinX = cell.X * CellSize;
        area.MinY = cell.Y * CellSize;
        area.MaxX = area.MinX + CellSize;
        area.MaxY = area.MinY + CellSize;
        slot->Reset(new CEngineSceneMap(this, MaxLevels, area));
    }
    return slot->m_pObject;
}
