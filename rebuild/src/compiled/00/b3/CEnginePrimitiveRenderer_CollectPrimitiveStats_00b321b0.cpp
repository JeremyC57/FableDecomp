// CEnginePrimitiveRenderer::CollectPrimitiveStats(map* destination, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long statFlags) -- build a primitive-stats enumerator on the stack and run it over every scene in SceneList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B321B0, length 120 bytes (+ int3 padding).
// Callees: CEngineScenePrimitiveStatsEnumerator::CEngineScenePrimitiveStatsEnumerator(map*, const C2DBoxF&, EEnginePrimitiveStatIndex, unsigned long)
// @ 0x00BACA20 (out of line; vtable 0x012A534C), CEngineScene::Enumerate(CBase& enumerator) @ 0x00B93290 (the same callee as the landed
// CEnginePrimitiveRenderer::ResetVisibilityRatios @ 0x00B320A0), CBase::~CBase() @ 0x0099A300 (the enumerator has no destructor of its own).
// Label correction: the manifest's "CEngine::CollectPrimitiveStats" belongs to CEnginePrimitiveRenderer: the vector at +0x338 is the PDB
// SceneList (CArray<CEngineScene*> at PDB +0x33c, retail 4 bytes lower as in ResetVisibilityRatios); CEngine is only 0x188 bytes.
// Shape: the 0x20-byte enumerator in the frame, its four constructor arguments forwarded in order; an index loop bounded by the
// re-computed unsigned size (`jbe` entry, `jb` back edge), each scene read through STLport `*(begin() + i)`; the base destructor last.
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
};

struct CPrimitiveStatMap;                    // map<unsigned long, unsigned long>

namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                // 0x0099A300
}; } using N_0099a300::CBase;

struct CEngineScenePrimitiveStatsEnumerator : CBase {   // 0x20 bytes, vtable 0x012A534C
    CPrimitiveStatMap* Destination;          // +0x04
    C2DBoxF            Area;                 // +0x08
    long               StatIndex;            // +0x18 (EEnginePrimitiveStatIndex)
    unsigned long      StatFlags;            // +0x1c
    CEngineScenePrimitiveStatsEnumerator(CPrimitiveStatMap* destination, const C2DBoxF& area, long statIndex, unsigned long statFlags);   // 0x00BACA20
};

class CEngineScene { public:
    void Enumerate(CBase& enumerator);       // 0x00B93290
};

template <class T>
struct CRetailVector {                       // STLport vector of pointers: 12 bytes
    T* m_start;
    T* m_finish;
    T* m_endOfStorage;
    T* begin() const { return m_start; }
    unsigned long size() const { return m_finish - m_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

class CEnginePrimitiveRenderer { public:
    unsigned char                _pad[0x338];
    CRetailVector<CEngineScene*> SceneList;  // +0x338 (PDB +0x33c)
    void CollectPrimitiveStats(CPrimitiveStatMap* destination, const C2DBoxF& area, long statIndex, unsigned long statFlags);
};

void CEnginePrimitiveRenderer::CollectPrimitiveStats(CPrimitiveStatMap* destination, const C2DBoxF& area, long statIndex, unsigned long statFlags)
{
    CEngineScenePrimitiveStatsEnumerator enumerator(destination, area, statIndex, statFlags);
    for (unsigned long i = 0; i < SceneList.size(); ++i)
        SceneList[i]->Enumerate(enumerator);
}