// Behaviour test for 0x00B949A0 CEngineSceneMap::Initialise: fields stored, one grid level holding a new top-level grid.
#include <cstdio>

class CEngineScene { public: int id; };
class CEngineSceneGrid { public: CEngineSceneGrid* LevelUp; CEngineSceneGrid(CEngineSceneGrid* up) : LevelUp(up) {} };
class C2DBoxF { public: float MinX, MinY, MaxX, MaxY; };
template <class T> class CCountedPointer { public:
    T* m_pObject; long* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    void Reset(T* o) { m_pObject = o; }
};
template <class T> struct CRetailArray {
    T* first; T* last; T* end_of_storage; T storage[4];
    void resize(unsigned int count, const T& value) { first = storage; for (unsigned int i = 0; i < count; ++i) storage[i] = value; last = first + count; end_of_storage = storage + 4; }
    T& operator[](unsigned int n) { return first[n]; }
};
class CEngineSceneMap { public:
    C2DBoxF Area; unsigned long MaxLevels; CRetailArray<CCountedPointer<CEngineSceneGrid> > GridLevels; CEngineScene* Scene;
    void Initialise(CEngineScene* scene, unsigned long maxLevels, const C2DBoxF& area)
    {
        Scene = scene;
        MaxLevels = maxLevels;
        Area = area;
        GridLevels.resize(1, CCountedPointer<CEngineSceneGrid>());
        GridLevels[0].Reset(new CEngineSceneGrid(0));
    }
};

int main()
{
    CEngineScene scene; C2DBoxF box = { 1.0f, 2.0f, 3.0f, 4.0f };
    CEngineSceneMap m;
    m.Initialise(&scene, 6, box);
    if (m.Scene != &scene || m.MaxLevels != 6 || m.Area.MaxX != 3.0f || m.Area.MaxY != 4.0f || m.GridLevels.last - m.GridLevels.first != 1
        || !m.GridLevels.first[0].m_pObject || m.GridLevels.first[0].m_pObject->LevelUp != 0) { std::printf("00b949a0_TEST FAIL\n"); return 1; }
    delete m.GridLevels.first[0].m_pObject;
    std::printf("00b949a0_TEST PASS\n");
    return 0;
}