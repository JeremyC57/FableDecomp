// CEnginePrimitiveManagerAnimatedMeshesBase::RenderSubPrimitive(CEngineSubPrimitiveBase&, ERenderPassType) -- a cloth-simulated sub-primitive is rendered by its simulation (found by the first cloth primitive's index) with the animation blend states and the bullet-time or interpolation-set time; any other goes to RenderMeshPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD7110, length 146 bytes (+ int3 padding). Vtable 0x012A78DC slot 5.
// Callees: 0x00B812C0 CEnginePrimitiveSimulationInfo::FindSimulation(unsigned long) (bounds-checked lookup in its
// 8-byte entry array), the simulation's virtual slot 9 (+0x24, render the simulated mesh), 0x00BD3070
// CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh&, ERenderPassType).
// Globals: 0x0143B918 the bullet-time clock value, 0x0143B934 the 12-byte per-interpolation-set time records.
// Layout: CEngineSubPrimitiveAnimatedMesh = CEngineSubPrimitiveBase (PDB 0x18: Parent +0xc) + Primitive (C3DPrimitive2*)
// +0x18, render mode +0x1c, the simulated flag +0x20 (PDB constructor (parent, C3DPrimitive2*, ERenderMode, bool)).
// CEngineInternalPrimitiveAnimatedMesh (PDB): InterpolationSet +0x2c, BulletTimeAffectsCloth +0x60, SimulationInfo
// +0x64 (CIVCountedPointer: the object), Anim +0x70 (StartState +0x74, EndState +0x7c), RenderInfo +0xe4 (its first
// word and the block at +0xec are passed). C3DPrimitive2 (PDB): ClothPrimitives (CArray of counted pointers) at
// +0x20; C3DClothPrimitive's simulation index at +0xc.
// Shape: the time is a named float local homed in the dead `subPrimitive` argument slot (the bullet-time global is
// copied with integer moves, the table entry through the x87); the simulation's vtable is fetched into ebx before
// the eight argument pushes; the cloth primitive is reached through the counted pointer's `operator->` (a direct
// `.Data->` read loads the index into edx instead of eax).

struct C3DAnimationBlendState { unsigned long m_a, m_b; };     // 8 bytes (PDB)
struct C3DClothPrimitive {
    unsigned char _pad00[0xc];
    unsigned long SimulationIndex;                       // +0x0c
};
struct C3DClothPrimitivePointer {                         // CCountedPointer<C3DClothPrimitive>
    C3DClothPrimitive* Data;
    void*              Info;
    C3DClothPrimitive* operator->() const { return Data; }
};
struct C3DPrimitive2 {
    unsigned char             _pad00[0x20];
    C3DClothPrimitivePointer* ClothPrimitives;           // +0x20 CArray data
};
class CEngineSubPrimitiveBase;
struct CEngineRenderInfoBlock;

class CEngineSimulationBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void RenderSimulatedMesh(C3DPrimitive2* primitive, CEngineSubPrimitiveBase& subPrimitive,
                                     unsigned long renderInfo, const C3DAnimationBlendState& startState,
                                     const C3DAnimationBlendState& endState, float time,
                                     CEngineRenderInfoBlock& renderBlock, long passType);   // slot 9 (+0x24)
};
class CEnginePrimitiveSimulationInfo {
public:
    CEngineSimulationBase* FindSimulation(unsigned long index);   // 0x00B812C0
};

struct CEngineInternalPrimitiveAnimatedMesh {
    unsigned char                   _pad00[0x2c];
    long                            InterpolationSet;    // +0x2c
    unsigned char                   _pad30[0x30];
    bool                            BulletTimeAffectsCloth;   // +0x60
    CEnginePrimitiveSimulationInfo* SimulationInfo;      // +0x64
    unsigned char                   _pad68[0xc];
    C3DAnimationBlendState          StartState;          // +0x74
    C3DAnimationBlendState          EndState;            // +0x7c
    unsigned char                   _pad84[0x60];
    unsigned long                   RenderInfo;          // +0xe4
    unsigned char                   _pade8[4];
    CEngineRenderInfoBlock*         _renderBlockAnchor;  // +0xec (address passed)
};

class CEngineSubPrimitiveBase {                           // PDB, 0x18 bytes
public:
    virtual ~CEngineSubPrimitiveBase();
    unsigned long                         RenderFlags;   // +0x04
    unsigned char                         Alpha;         // +0x08
    CEngineInternalPrimitiveAnimatedMesh* Parent;        // +0x0c
    CEngineSubPrimitiveBase*              NextSubPrimitive;   // +0x10
    void*                                 Manager;       // +0x14
};
class CEngineSubPrimitiveAnimatedMesh : public CEngineSubPrimitiveBase {
public:
    C3DPrimitive2* Primitive;                            // +0x18
    long           RenderMode;                           // +0x1c
    bool           Simulated;                            // +0x20
};

struct SInterpolationTime { float Time; float m_4; float m_8; };   // 12-byte records
extern float              g_bulletTimeClock;             // 0x0143B918
extern SInterpolationTime g_interpolationTimes[];        // 0x0143B934

enum ERenderPassType { RENDER_PASS_DEFAULT = 0 };

class CEnginePrimitiveManagerAnimatedMeshesBase {
public:
    void RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh& subPrimitive, ERenderPassType passType);   // 0x00BD3070
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType passType);
};

void CEnginePrimitiveManagerAnimatedMeshesBase::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType passType)
{
    CEngineSubPrimitiveAnimatedMesh& mesh = static_cast<CEngineSubPrimitiveAnimatedMesh&>(subPrimitive);
    if (mesh.Simulated) {
        if (mesh.Parent->SimulationInfo) {
            C3DPrimitive2* primitive = mesh.Primitive;
            CEngineSimulationBase* simulation =
                mesh.Parent->SimulationInfo->FindSimulation(primitive->ClothPrimitives[0]->SimulationIndex);
            if (simulation) {
                CEngineInternalPrimitiveAnimatedMesh* parent = mesh.Parent;
                float time;
                if (parent->BulletTimeAffectsCloth)
                    time = g_bulletTimeClock;
                else
                    time = g_interpolationTimes[parent->InterpolationSet].Time;
                simulation->RenderSimulatedMesh(primitive, mesh, parent->RenderInfo, parent->StartState,
                                                parent->EndState, time,
                                                *(CEngineRenderInfoBlock*)&parent->_renderBlockAnchor, passType);
            }
        }
    }
    else
        RenderMeshPrimitive(mesh, passType);
}
