#include <cstdio>
#include <new>
// behaviour copy of CEngineLandscapePatch::CEngineLandscapePatch() @ 0x00BF3A10
struct CCountedPointerModel {
    void* m_pObject;
    void* m_pCount;
    CCountedPointerModel() : m_pObject(0), m_pCount(0) { }
};
struct CEngineLandscapePatchModel {
    void*           Map;
    void*           LayerMeshList;
    unsigned short* FastAccessTextureIDs;
    unsigned short  FastAccessTextureIDCount;
    void*           WaterPatch;
    long            FilePos;
    long            FileDataSize;
    float           BoundingBox[6];
    unsigned short  MapX, MapY;
    bool            TexturesLoaded : 1;
    bool            HasForegroundVB : 1;
    bool            ForegroundCanDraw : 1;
    bool            InForegroundRange : 1;
    bool            ForegroundNeeded : 1;
    CCountedPointerModel LoadingPatch;
    CEngineLandscapePatchModel()
    {
        LayerMeshList = 0;
        TexturesLoaded = false;
        HasForegroundVB = false;
        ForegroundCanDraw = false;
        InForegroundRange = false;
        ForegroundNeeded = true;
        WaterPatch = 0;
        FilePos = 0;
        FileDataSize = 0;
        FastAccessTextureIDs = 0;
        FastAccessTextureIDCount = 0;
    }
};

int main()
{
    static unsigned char storage[sizeof(CEngineLandscapePatchModel)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xAB;
    CEngineLandscapePatchModel* p = new (storage) CEngineLandscapePatchModel;
    int ok = p->LayerMeshList == 0 && p->WaterPatch == 0 && p->FilePos == 0 && p->FileDataSize == 0
          && p->FastAccessTextureIDs == 0 && p->FastAccessTextureIDCount == 0
          && !p->TexturesLoaded && !p->HasForegroundVB && !p->ForegroundCanDraw && !p->InForegroundRange
          && p->ForegroundNeeded && p->LoadingPatch.m_pObject == 0 && p->LoadingPatch.m_pCount == 0;
    if (!ok) { std::printf("00bf3a10_TEST FAIL\n"); return 1; }
    std::printf("00bf3a10_TEST PASS\n");
    return 0;
}