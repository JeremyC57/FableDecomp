// CTCShapeManager::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo*) -- when the world has a current game camera, fetches the camera position and draws the shapes for it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00761B10, length 52 bytes (+12 int3 padding); vtable CTCShapeManager 0x0126A2A4 slot 11.
// Callees: CTCBase::GetWorld() const @ 0x00686A80 (the manifest label "CViewBase::GetDisplayEngine" is propagated), the
// camera's virtual slot 11 (+0x2c, a C3DVector returned through the hidden pointer), CTCShapeManager::DrawShapes
// (CEnginePrimitivePrepareInfo*, const C3DVector*) @ 0x00761980.
// Layout: engine/CWorld.h PCurrentGameCamera at +0x34.
// Shape: the 12-byte position local is the hidden result slot of the virtual getter (a C3DVector with a user
// constructor); DrawShapes takes its address; the prepare-info argument is re-read from its slot after the virtual call.
// The world (not the camera) is the named local and the camera member is read twice (test, then as `this`): VC CSEs it
// straight into ecx; a camera pointer local is loaded into eax and copied (`mov ecx,eax`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};

struct CAIGameCameraBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector GetPosition() const;          // slot 11 (+0x2c)
};

struct CWorld {
    unsigned char      _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;          // +0x34
};

struct CEnginePrimitivePrepareInfo;

struct CTCShapeManager {
    const CWorld* GetWorld() const;                 // 0x00686A80 (CTCBase)
    void DrawShapes(CEnginePrimitivePrepareInfo* info, const C3DVector* position);   // 0x00761980
    void DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info);
};

void CTCShapeManager::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info)
{
    const CWorld* world = GetWorld();
    if (world->PCurrentGameCamera) {
        C3DVector position = world->PCurrentGameCamera->GetPosition();
        DrawShapes(info, &position);
    }
}
