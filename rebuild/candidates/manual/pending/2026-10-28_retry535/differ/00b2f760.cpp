// CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager() -- engine component base, occlude delay 4 frames, camera direction +X, camera position origin, step limits 1/6 and 4
// Grade: DIFFER(101v95) -- RTM regrade 2026-10-28: LastCameraDirection = C3DVector(1,0,0) through the implicit operator= keeps the 12-byte frame temporary, but VC7.1 reloads temp.X and temp.Z from the frame (copy through ecx/edx, member addressed via `lea eax`); retail reloads only temp.X, forwards the zero Y/Z through edx (`mov edx,eax`), addresses the member with `lea ecx,[esi+14h]`, sinks the dead temp Y/Z stores to the end and stores LastCameraPosition Z,Y,X. User operator= / copy-ctor / named-local / Set / reference-parameter forms fold the temporary away (0x44 body)
// Leaf: ??0CEnginePrimitiveOcclusionManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2F760, 95 bytes, thiscall, plain `ret`, returns this.
// Note: the parked source lacked C3DVector() (C2512 under RTM); restored here (Ego_r.pdb ??0C3DVector@@QAE@XZ).
// Identity (proven): stores vtable 0x012A124C (RTTI .?AVCEnginePrimitiveOcclusionManager@@ : CEngineComponent);
//   Ego_r.pdb ??0CEnginePrimitiveOcclusionManager@@QAE@XZ. The catalog label "CCompressedMemoryDataInputStream" is
//   propagated (wrong class).
// Callees: CEngineComponent::CEngineComponent 0x00B59710.
// Layout (Ego_r.pdb names): CEngineComponent 0x0C, PrimitiveOccludeDelay +0x0C, ShowVisibilityInfo +0x10 (left
//   uninitialised), LastCameraDirection +0x14, LastCameraPosition +0x20, MaxCameraAngleStep +0x2C,
//   MaxCameraPositionStep +0x30, InvalidateOcclusion +0x34, NewSceneFlag +0x35.
// Layout note: engine/CEnginePrimitiveOcclusionManager.h flattens the CEngineComponent base and the two C3DVector
//   members, so the constructor (which must run the base ctor and C3DVector's constructors) uses a local model.
// Shape: LastCameraDirection is built from a C3DVector(1,0,0) temporary through C3DVector's user copy constructor --
//   retail's frame temporary (1.0f stored and reloaded, the dead Y/Z stores) -- and LastCameraPosition by C3DVector(0,0,0).
#pragma pack(push, 1)
class C3DVector {
public:
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    float X, Y, Z;
};
#pragma pack(pop)

class CEngineComponent {
public:
    CEngineComponent();                              // 0x00B59710
    virtual ~CEngineComponent();
    void** GlobalPointerAddress;                     // +0x04
    bool RenderEnabled;                              // +0x08
    unsigned char KeepAlive;                         // +0x09 (Ego_r.pdb uchar)
};

class CEnginePrimitiveOcclusionManager : public CEngineComponent {
public:
    CEnginePrimitiveOcclusionManager();
    virtual ~CEnginePrimitiveOcclusionManager();
    unsigned long PrimitiveOccludeDelay;             // +0x0C
    long ShowVisibilityInfo;                         // +0x10
    C3DVector LastCameraDirection;                   // +0x14
    C3DVector LastCameraPosition;                    // +0x20
    float MaxCameraAngleStep;                        // +0x2C
    float MaxCameraPositionStep;                     // +0x30
    bool InvalidateOcclusion;                        // +0x34
    bool NewSceneFlag;                               // +0x35
};

CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager()
    : PrimitiveOccludeDelay(4),
      LastCameraPosition(0.0f, 0.0f, 0.0f),
      MaxCameraAngleStep(1.0f / 6.0f),
      MaxCameraPositionStep(4.0f),
      InvalidateOcclusion(false),
      NewSceneFlag(false)
{
    LastCameraDirection = C3DVector(1.0f, 0.0f, 0.0f);
}
