// CEngineWorldMap::CloseStaticMap() -- when a static map is open, close it on every engine map after the first, on the water renderer, and on the static-map bank file, then reset the open state and the build status
// Leaf: CloseStaticMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B40000, 108 bytes (+ int3 padding).
// Label correction: the manifest's "FrameStart(CEngineComponentParams*)" does not fit -- the body takes no argument (bare
//   `ret`) and calls CloseStaticMap on every map and on the water renderer; named after what it does.
// Callees: CEngineMap::CloseStaticMap @ 0x00B3EF40, CEngineWaterRenderer::CloseStaticMap @ 0x00B6DB80 on the global
//   water renderer @ 0x01436E54, StaticMapFile virtual slot 5 (`call [eax+14h]`, the bank file's Close).
// Layout (retail, the PDB's offsets less the narrower retail CArray / STLport list / CBankFileAsync): MapList (CArray of
//   CEngineMap*) begin/end +0x20/+0x24, StaticMapFile +0x34, StaticMapOpenState +0x1a8, StaticMapBuildStatus +0x1b0.
// Shape: the count is re-derived each pass (`(end - begin) >> 2`, unsigned compare), the loop starts at map 1; the global
//   is tested before its call; the two state words are cleared with immediates.
class CEngineMap { public:
    void CloseStaticMap();                       // 0x00B3EF40
};

class CEngineWaterRenderer { public:
    void CloseStaticMap();                       // 0x00B6DB80
};

extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54

class CStaticMapBankFile { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Close();                        // slot 5
};

template <class T> class CArray { public:               // retail CArray: begin, end, capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

enum STATIC_MAP_OPEN_STATE { STATIC_MAP_CLOSED = 0 };
enum STATIC_MAP_BUILD_STATUS { STATIC_MAP_BUILD_NONE = 0 };

class CEngineWorldMap { public:
    unsigned char           _pad0[0x20];
    CArray<CEngineMap*>     MapList;             // +0x20
    unsigned char           _pad2c[0x34 - 0x2c];
    CStaticMapBankFile      StaticMapFile;       // +0x34
    unsigned char           _pad38[0x1a8 - 0x38];
    STATIC_MAP_OPEN_STATE   StaticMapOpenState;  // +0x1a8
    unsigned char           _pad1ac[0x1b0 - 0x1ac];
    STATIC_MAP_BUILD_STATUS StaticMapBuildStatus;// +0x1b0
    void CloseStaticMap();
};

void CEngineWorldMap::CloseStaticMap()
{
    if (StaticMapOpenState == STATIC_MAP_CLOSED)
        return;
    for (unsigned long i = 1; i < MapList.Size(); ++i) {
        CEngineMap* map = MapList[i];
        if (map)
            map->CloseStaticMap();
    }
    if (g_pEngineWaterRenderer)
        g_pEngineWaterRenderer->CloseStaticMap();
    StaticMapFile.Close();
    StaticMapOpenState = STATIC_MAP_CLOSED;
    StaticMapBuildStatus = STATIC_MAP_BUILD_NONE;
}