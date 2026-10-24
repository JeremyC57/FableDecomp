// CEngineLandscapePatch::CEngineLandscapePatch() -- default constructor: vtable, empty LoadingPatch counted pointer, the foreground flag bits (only ForegroundNeeded set), and null layer meshes / water patch / file span / fast-access texture ids
// Leaf: ??0CEngineLandscapePatch@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF3A10, 48 bytes. Ego_r.pdb: ??0CEngineLandscapePatch@@QAE@XZ (the manifest label
// "NHeroInformationScreens_Initialize" is propagated). Vtables: 0x012A8200 = CEngineLandscapePatch (rtti-vtables.tsv; its slots
// 0x00BDD3E0 / 0x00BF3980 RelocateData / 0x00BF3840 CanRelocateData), the CMovableResource base store is dead and dropped.
// Layout (Ego_r.pdb CEngineLandscapePatch, 0x48 bytes): Map +4 (left uninitialised), LayerMeshList +8, FastAccessTextureIDs +0xc,
// FastAccessTextureIDCount +0x10 (word), WaterPatch +0x14, FilePos +0x18, FileDataSize +0x1c, BoundingBox +0x20 (no constructor
// code), MapX/MapY +0x38/+0x3a, the five `bool:1` flags in the byte at +0x3c, LoadingPatch CCountedPointer<CBankFileAsyncData> +0x40.
// Shape: the member counted pointer's inline constructor zeroes both words right after the vtable store; the body then writes
// LayerMeshList, the flag byte as one read-modify-write (`and dl,0F0h ; or dl,10h`: four flags cleared, ForegroundNeeded set;
// the LayerMeshList store is scheduled between its load and store) and the remaining members in the body's order (WaterPatch,
// FilePos, FileDataSize, FastAccessTextureIDs, the count) from one zero register. Grades: 2 (flags-first body put +8 after the byte store).
struct CBankFileAsyncData;
struct CEngineLandscapeMap;
struct CLandscapeLayerMesh;
struct CWaterPatchMesh;

struct SCountBlock_00bf3a10 {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};

template <class T> struct CCountedPointer {
    T*                    m_pObject;               // +0
    SCountBlock_00bf3a10* m_pCount;                // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    ~CCountedPointer();
};

struct C3DBoundingBox {
    float m_min[3];
    float m_max[3];
};

class CMovableResource {
public:
    CMovableResource() { }
    virtual ~CMovableResource() { }
    virtual void RelocateData();
    virtual bool CanRelocateData();
};

class CEngineLandscapePatch : public CMovableResource {
public:
    CEngineLandscapeMap*  Map;                     // +0x04
    CLandscapeLayerMesh*  LayerMeshList;           // +0x08
    unsigned short*       FastAccessTextureIDs;    // +0x0c
    unsigned short        FastAccessTextureIDCount;// +0x10
    CWaterPatchMesh*      WaterPatch;              // +0x14
    long                  FilePos;                 // +0x18
    long                  FileDataSize;            // +0x1c
    C3DBoundingBox        BoundingBox;             // +0x20
    unsigned short        MapX;                    // +0x38
    unsigned short        MapY;                    // +0x3a
    bool                  TexturesLoaded : 1;      // +0x3c
    bool                  HasForegroundVB : 1;
    bool                  ForegroundCanDraw : 1;
    bool                  InForegroundRange : 1;
    bool                  ForegroundNeeded : 1;
    CCountedPointer<CBankFileAsyncData> LoadingPatch;   // +0x40
    CEngineLandscapePatch();
    virtual ~CEngineLandscapePatch();
    virtual void RelocateData();
    virtual bool CanRelocateData();
};

CEngineLandscapePatch::CEngineLandscapePatch()
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