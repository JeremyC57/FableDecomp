// CParticleSimulation::ApplyDragging(const CParticleContext& context) -- when dragging is enabled, take the context's 3x4 transform as a 4x4 matrix, blend it towards the reference matrix by (1 - DraggingStrength) and move the current and the last particle positions with it
// Leaf: ApplyDragging
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF300, 256 bytes, `ret 4`. PDB: ?ApplyDragging@CParticleSimulation@@AAEXABVCParticleContext@@@Z.
//   Called from 0x00AE2CC7 and 0x00AE2D37.
// Callees: CParticleContext virtual slot 3 (+0x0C, fills a CMatrix3x4; descriptive name), CMatrix4x4::AlignedSetInterpolate
//   (const CMatrix4x4&, const CMatrix4x4&, float) 0x00A5CEE0 and AlignedTransformTransposedAssumeW(const C4DVector*,
//   C4DVector*, unsigned long, float) const 0x00A5CB90 (PDB names). The reference matrix is the global 0x013D2890
//   (descriptive name). Constant 0x0122DED8 = 1.0f.
// Layout: CParticleSimulation (Ego_r.pdb, as the landed 0x00ADF2A0/0x00ADF610): Size +0x18, Positions +0x1C,
//   LastPositions +0x20, DraggingEnable +0x3C, DraggingStrength +0x40.
// Shape: the 16-aligned CMatrix4x4 (`and esp,-16` frame) is built from the 3x4 by an inline converting constructor
//   (`__forceinline`: transposed, last row 0,0,0,1); the in-place blend goes through an inline wrapper passing `*this` as the
//   source (`push eax ; mov ecx,eax` with one `lea`); the blend factor is evaluated early by the scheduler.
struct C4DVector { float X, Y, Z, W; };
struct CMatrix3x4 { float m[4][3]; };                  // rows: right, up, forward, position

class __declspec(align(16)) CMatrix4x4 {
public:
    float m[4][4];
    __forceinline CMatrix4x4(const CMatrix3x4& o)
    {
        m[0][0] = o.m[0][0]; m[0][1] = o.m[1][0]; m[0][2] = o.m[2][0]; m[0][3] = o.m[3][0];
        m[1][0] = o.m[0][1]; m[1][1] = o.m[1][1]; m[1][2] = o.m[2][1]; m[1][3] = o.m[3][1];
        m[2][0] = o.m[0][2]; m[2][1] = o.m[1][2]; m[2][2] = o.m[2][2]; m[2][3] = o.m[3][2];
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }
    void AlignedSetInterpolate(const CMatrix4x4& from, const CMatrix4x4& to, float t);   // 0x00A5CEE0
    void InterpolateTo(const CMatrix4x4& to, float t) { AlignedSetInterpolate(*this, to, t); }
    void AlignedTransformTransposedAssumeW(const C4DVector* in, C4DVector* out, unsigned long count, float w) const;   // 0x00A5CB90
};

extern const CMatrix4x4 g_ParticleDragReference;       // 0x013D2890

class CParticleContext {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void GetTransform(CMatrix3x4& transform) const;   // slot 3 (+0x0c)
};

struct CMovableResource { virtual ~CMovableResource(); };
struct CMovableResourceMemoryPool;

class CParticleSimulation : public CMovableResource {
public:
    unsigned char               RenderData[8];          // +0x04
    float                       Timestep;               // +0x0c
    bool                        TimestepChanged;        // +0x10
    float                       TimestepMultiplier;     // +0x14
    unsigned long               Size;                   // +0x18
    C4DVector*                  Positions;              // +0x1c
    C4DVector*                  LastPositions;          // +0x20
    C4DVector*                  SavedPositions;         // +0x24
    C4DVector*                  Accelerations;          // +0x28
    float*                      SimulationAlphas;       // +0x2c
    float                       MaximumAdjustment;      // +0x30
    float                       GravityStrength;        // +0x34
    float                       WindStrength;           // +0x38
    bool                        DraggingEnable;         // +0x3c
    bool                        DraggingRotational;     // +0x3d
    float                       DraggingStrength;       // +0x40
    void ApplyDragging(const CParticleContext& context);
};

void CParticleSimulation::ApplyDragging(const CParticleContext& context)
{
    if (!DraggingEnable)
        return;
    CMatrix3x4 transform;
    context.GetTransform(transform);
    CMatrix4x4 drag(transform);
    drag.InterpolateTo(g_ParticleDragReference, 1.0f - DraggingStrength);
    drag.AlignedTransformTransposedAssumeW(Positions, Positions, Size, 1.0f);
    drag.AlignedTransformTransposedAssumeW(LastPositions, LastPositions, Size, 1.0f);
}