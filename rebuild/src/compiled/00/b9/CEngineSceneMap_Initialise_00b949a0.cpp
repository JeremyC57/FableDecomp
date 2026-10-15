// CEngineSceneMap::Initialise(CEngineScene*, unsigned long, const C2DBoxF&) -- store the scene, the level limit and the covered area, size the grid-level array to one empty counted pointer and give it a fresh top-level CEngineSceneGrid
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B949A0, length 135 bytes (+ int3 padding). Catalogued as the
// CEngineSceneMap constructor; the landed constructor @ 0x00B94A40 calls this body as Initialise after building the
// vtable and the empty array (see rebuild/src/compiled/00/b9/CEngineSceneMap_CEngineSceneMap_00b94a40.cpp).
// Callees: CArray<CCountedPointer<CEngineSceneGrid>>::resize(count, const value&) @ 0x00B98170 (on +0x18), operator new
// @ 0x00BFEA1A (0x20 bytes), CEngineSceneGrid::CEngineSceneGrid(CEngineSceneGrid* levelUp) @ 0x00B93EB0 (with 0),
// CCountedPointer<CEngineSceneGrid>::Reset(CEngineSceneGrid*) @ 0x00B95A30 (on element 0).
// Layout: as the landed constructor (retail = Ego_r.pdb minus 4 after the 12-byte CArray): Area +0x04, MaxLevels +0x14,
// GridLevels +0x18, Scene +0x24; no engine header exists for this class.
// Shape: Scene then MaxLevels are stored from the argument registers, the box is a struct assignment (`lea eax,[ecx+4]`
// then four dwords through esi); the resize value is a default-constructed CCountedPointer temporary whose two zero
// words are immediate stores after the pushes; `new` keeps its null test and both arms call Reset on `GridLevels[0]`
// (the array's first pointer re-read in each arm).
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineScene;

struct CEngineSceneGrid {
    unsigned char _body[0x20];
    CEngineSceneGrid(CEngineSceneGrid* levelUp);   // 0x00B93EB0
};

struct C2DBoxF { float MinX, MinY, MaxX, MaxY; };

template <class T> struct CCountedPointer {
    T*    m_pObject;
    long* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    void Reset(T* object);                         // 0x00B95A30
};

template <class T> struct CRetailArray {           // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    void resize(unsigned int count, const T& value);   // 0x00B98170
    T& operator[](unsigned int n) { return first[n]; }
};

struct CEngineSceneMap {
    void*                                          _vfptr;       // +0x00 (CBase vtable)
    C2DBoxF                                        Area;         // +0x04
    unsigned long                                  MaxLevels;    // +0x14
    CRetailArray<CCountedPointer<CEngineSceneGrid> > GridLevels; // +0x18
    CEngineScene*                                  Scene;        // +0x24
    void Initialise(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area);
};

void CEngineSceneMap::Initialise(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area)
{
    Scene = scene;
    MaxLevels = maxLevels;
    Area = area;
    GridLevels.resize(1, CCountedPointer<CEngineSceneGrid>());
    GridLevels[0].Reset(new CEngineSceneGrid(0));
}