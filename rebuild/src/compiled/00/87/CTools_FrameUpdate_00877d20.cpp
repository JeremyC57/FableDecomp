// CTools::FrameUpdate() -- rescans the things around the camera: clears ScanThings and refills it with every game object in the 20x20 world-square box centred on the camera's (integer) world position
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00877D20, length 106 bytes, `ret`. Called from
// CGameCameraManager::Update (0x0069B7E9) on CamTools at +0x78, so the catalog class is right.
// Callees: __ftol2 @ 0x00BFEA70 (x2), the ScanThings vector's erase(first, last) @ 0x00738D70 (inline clear()),
// CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject const, vector<CIntelligentPointer<CThing const> >,
// CThingFilter_None> @ 0x008786E0.
// Layout (Ego_r.pdb, retail agrees): CTools: CameraManager +0x00, ScanThings +0x04; CGameCameraManager: Camera
// (CCamera, WorldPosition first) +0x04, World +0x74; CWorld::PThingSearchTools +0x20.
// Shape: the search tools pointer is read first (a named local), the box is a C2DBoxI temporary built from the
// two truncated coordinates, the filter an empty object in the frame.
class C3DVector { public: float x, y, z; };
struct C2DVectorI {
    long x, y;
    C2DVectorI(long ax, long ay) : x(ax), y(ay) { }
};
class C2DBoxI { public:
    C2DVectorI TopLeft;                            // +0x00
    C2DVectorI BottomRight;                        // +0x08
    C2DBoxI(long x0, long y0, long x1, long y1) : TopLeft(x0, y0), BottomRight(x1, y1) { }
};
class CThing;
class CThingGameObject;
class CThingFilter_None { public: };
template <class T> class CIntelligentPointer { public: void* m_vtable; T* m_pItem; };
struct CThingConstPtrVector {                      // STLport vector<CIntelligentPointer<CThing const> >
    CIntelligentPointer<const CThing>* _M_start;
    CIntelligentPointer<const CThing>* _M_finish;
    CIntelligentPointer<const CThing>* _M_end_of_storage;
    CIntelligentPointer<const CThing>* begin() { return _M_start; }
    CIntelligentPointer<const CThing>* end() { return _M_finish; }
    CIntelligentPointer<const CThing>* erase(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last);   // 0x00738D70
    void clear() { erase(begin(), end()); }
};
class CThingSearchTools { public:
    template <class T, class C, class F> void GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& out);   // 0x008786E0 (<CThingGameObject const, ..., CThingFilter_None>)
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* get() const { return ptr; }
    T* operator->() const { return ptr; }
};
class CWorld { public:
    unsigned char _pad00[0x20];
    scoped_ptr<CThingSearchTools> PThingSearchTools;   // +0x20
};
class CCamera { public:
    C3DVector WorldPosition;                       // +0x00
    unsigned char _rest[0x38 - 12];
};
class CGameCameraManager { public:
    void*   m_vtable;                              // +0x00
    CCamera Camera;                                // +0x04
    CCamera OldCamera;                             // +0x3c
    CWorld* World;                                 // +0x74
};
struct CTools {
    CGameCameraManager*  CameraManager;            // +0x00
    CThingConstPtrVector ScanThings;               // +0x04
    float CameraMeshOffsetDistance;                // +0x10
    float CameraCreatureOffsetDistance;            // +0x14
    float CameraLandscapeOffsetDistance;           // +0x18
    float CameraThemeOffsetDistance;               // +0x1c
    void FrameUpdate();
};
void CTools::FrameUpdate()
{
    CThingSearchTools* search = CameraManager->World->PThingSearchTools.get();
    long x = (long)CameraManager->Camera.WorldPosition.x;
    long y = (long)CameraManager->Camera.WorldPosition.y;
    C2DBoxI box(x - 10, y - 10, x + 10, y + 10);
    ScanThings.clear();
    CThingFilter_None filter;
    search->GetAllThingsInRoughArea<const CThingGameObject>(box, filter, ScanThings);
}