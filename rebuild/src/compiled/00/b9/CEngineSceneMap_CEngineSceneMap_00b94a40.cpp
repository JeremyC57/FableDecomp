// CEngineSceneMap::CEngineSceneMap(CEngineScene*, unsigned long, const C2DBoxF&) -- base CBase(), own vtable, empty GridLevels array, then Initialise(scene, maxLevels, area)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B94A40, length 51 bytes.
// Callees: CBase::CBase @ 0x0099A2F0; CEngineSceneMap::Initialise(CEngineScene*, unsigned long,
// const C2DBoxF&) @ 0x00B949A0 (the manifest labels that body as the constructor: it stores the
// scene at +0x24 and MaxLevels at +0x14, copies the box into Area at +4 and builds the grid levels).
// Vtable 0x012A3E60 = CEngineSceneMap. Retail layout = Ego_r.pdb minus 4 after the CArray: Area
// +4, MaxLevels +0x14, GridLevels (CArray<CCountedPointer<CEngineSceneGrid>>, 12 bytes) +0x18,
// Scene +0x24; size 0x28. The manifest's "CColourConverter" label is propagated.
class CEngineScene;
class CEngineSceneGrid;

struct CBase {
    virtual void v00();
    CBase();                                                // 0x0099A2F0
};

class C2DBoxF { public: float MinX, MinY, MaxX, MaxY; };

struct CCountedGrid { void* m_pObject; void* m_pCount; };

template<class T> struct CRetailArray {                     // retail CArray: {first, last, end}
    T* first; T* last; T* end_of_storage;
    CRetailArray() : first(0), last(0), end_of_storage(0) { }
};

class CEngineSceneMap : public CBase { public:
    C2DBoxF       Area;                                     // +0x04
    unsigned long MaxLevels;                                // +0x14
    CRetailArray<CCountedGrid> GridLevels;                  // +0x18
    CEngineScene* Scene;                                    // +0x24
    virtual void v00();                                     // own vtable 0x012A3E60
    CEngineSceneMap(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area);
    void Initialise(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area);   // 0x00B949A0
};

CEngineSceneMap::CEngineSceneMap(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area)
    : CBase()
    , GridLevels()
{
    Initialise(scene, maxLevels, area);
}