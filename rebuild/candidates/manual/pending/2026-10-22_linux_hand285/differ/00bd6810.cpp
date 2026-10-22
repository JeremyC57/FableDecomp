// Grade: DIFFER(868v894) -- start/end world points: retail stages the end point through a full 12-byte temporary (0x108) and keeps the parent/child matrix copies swapped in the frame (0x134 frame v ours smaller); line built from default ctor + Start/End assignment
// CEnginePrimitiveManagerAnimatedMeshesBase::RenderDebugBoneHierarchy(CEngineInternalPrimitiveAnimatedMesh* mesh) -- debug view of a mesh's skeleton: inside a render-state capture block, blend the current bone transforms into a scratch matrix buffer, then draw a thin white line from each bone's parent to the bone (world space), once a bone with more than one child has been seen
// Leaf: RenderDebugBoneHierarchy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD6810, length 894 bytes (`ret 4`; `this` unused).
// Callees: vector<CMatrix3x4>(size_type) @ 0x00BD8730, CEngineInternalPrimitiveAnimatedMesh::GetBoneScalings @ 0x00B83750,
//   C3DAnimationManager::CalcBlendedMeshTransformations @ 0x00AA09F0, CEnginePrimitiveRenderer::AddPrimitive @ 0x00B32E90
//   (global @ 0x01436E80), CRenderStateManager::RestoreCaptureBlock @ 0x00A05840, free @ 0x00BFEA14.
// Globals: the render-state manager @ 0x013CB508 (capture push inlined as the landed BeginOutlineRendering 0x00B89A20),
//   the LOD table @ 0x0143B934 (12-byte entries, first float passed), 0.02f line width.
// Layout: retail mesh offsets (Anim +0x74, WorldPos +0x7c, MeshInfo +0xe4, world transform +0xec, LOD index +0x2c, owner
//   +0x50 -> +4 -> animation manager +0x3c0); C3DMeshInfo BoneCount +0x98, Bones +0x9c (0x3c-byte records, parent +4,
//   child count +8); CEnginePrimitiveLine as Ego_r.pdb (Type 6, Start +0x0c, End +0x18, Colour +0x24, Width +0x28,
//   BankIndex +0x2c, UseZBuffer +0x38; the blend mode/op words are left unwritten).
#include <stdlib.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CMatrix3x4 {
    float m[12];
};
inline C3DVector operator*(const C3DVector& v, const CMatrix3x4& t)
{
    C3DVector r;
    r.X = v.X * t.m[0] + v.Y * t.m[3] + v.Z * t.m[6] + t.m[9];
    r.Y = v.X * t.m[1] + v.Y * t.m[4] + v.Z * t.m[7] + t.m[10];
    r.Z = v.X * t.m[2] + v.Y * t.m[5] + v.Z * t.m[8] + t.m[11];
    return r;
}
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& c) { R = c.R; G = c.G; B = c.B; A = c.A; }
};
struct CEnginePrimitiveThemeChannelDesc {
    bool Channel0, Channel1, Channel2;
    CEnginePrimitiveThemeChannelDesc() : Channel0(false), Channel1(false), Channel2(false) { }
};
struct CEnginePrimitive {
    long                             Type;              // +0x00
    long                             InterpolationSet;  // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;  // +0x08
    CEnginePrimitive(long type, long interpolation) : Type(type), InterpolationSet(interpolation) { }
};
struct CEnginePrimitiveLine : CEnginePrimitive {
    C3DVector     Start;                                // +0x0c
    C3DVector     End;                                  // +0x18
    CRGBColour    Colour;                               // +0x24
    float         Width;                                // +0x28
    unsigned long BankIndex;                            // +0x2c
    long          BlendMode;                            // +0x30
    long          BlendOp;                              // +0x34
    bool          UseZBuffer;                           // +0x38
    CEnginePrimitiveLine() : CEnginePrimitive(6, 0), Colour(255, 255, 255, 255), Width(0.02f), BankIndex(0), UseZBuffer(false) { }
};
struct CEnginePrimitiveRenderer {
    void AddPrimitive(CEnginePrimitive* primitive);    // 0x00B32E90
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

struct SCaptureEntry { void* Block; unsigned long State; };
struct CRenderStateManager {
    unsigned char _pad000[8];
    SCaptureEntry CaptureStack[0x400];             // +0x008
    unsigned char _pad2008[0x2808 - 0x2008];
    unsigned long CaptureStackCount;               // +0x2808
    unsigned char _pad280c[8];
    unsigned long CaptureMask;                     // +0x2814
    unsigned char _pad2818[0x3a24 - 0x2818];
    unsigned char CaptureBlock[4];                 // +0x3a24
    void OpenCaptureBlock()
    {
        CaptureStack[CaptureStackCount++].Block = CaptureBlock;
        CaptureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

struct CMatrixVector {                             // STLport vector<CMatrix3x4>, 12 bytes
    CMatrix3x4* _M_start;
    CMatrix3x4* _M_finish;
    CMatrix3x4* _M_end_of_storage;
    explicit CMatrixVector(unsigned long count);   // 0x00BD8730
    ~CMatrixVector() { if (_M_start) free(_M_start); }
    long size() const { return _M_finish - _M_start; }
    CMatrix3x4& operator[](unsigned long i) { return _M_start[i]; }
    CMatrix3x4* GetData() { return size() ? _M_start : (CMatrix3x4*)this; }
};

struct CBoneRecord {                               // 0x3c bytes
    unsigned long Name;                            // +0x00
    long          Parent;                          // +0x04
    long          NoChildren;                      // +0x08
    unsigned char _pad0c[0x30];
};
struct C3DMeshInfo {
    unsigned char _pad00[0x98];
    unsigned long BoneCount;                       // +0x98
    CBoneRecord*  Bones;                           // +0x9c
};
struct C3DAnimationInterpolator;
struct C3DPositionInterpolator;
struct C3DAnimationManager {
    void CalcBlendedMeshTransformations(C3DAnimationInterpolator* anim, C3DPositionInterpolator* pos, float lod,
                                        C3DVector* scalings, C3DMeshInfo* info, CMatrix3x4* out, unsigned long count);   // 0x00AA09F0
};
struct CEngineAnimationOwner {
    unsigned char        _pad000[0x3c0];
    C3DAnimationManager* AnimationManager;         // +0x3c0
};
struct CEngineOwnerLink {
    unsigned char          _pad0[4];
    CEngineAnimationOwner* Owner;                  // +0x04
};
struct SLODEntry { float Distance; float _pad[2]; };
extern SLODEntry g_LODTable_0143b934[];            // 0x0143B934

struct CEngineInternalPrimitiveAnimatedMesh {
    unsigned char      _pad000[0x2c];
    long               LODIndex;                   // +0x2c
    unsigned char      _pad030[0x20];
    CEngineOwnerLink*  Link;                       // +0x50
    unsigned char      _pad054[0x20];
    unsigned char      Anim[8];                    // +0x74 C3DAnimationInterpolator
    unsigned char      WorldPos[0x68];             // +0x7c C3DPositionInterpolator
    C3DMeshInfo*       MeshInfo;                   // +0xe4
    unsigned char      _pad0e8[4];
    CMatrix3x4         WorldTransform;             // +0xec
    C3DVector* GetBoneScalings(C3DMeshInfo* info); // 0x00B83750
};

struct CEnginePrimitiveManagerAnimatedMeshesBase {
    void RenderDebugBoneHierarchy(CEngineInternalPrimitiveAnimatedMesh* mesh);
};

void CEnginePrimitiveManagerAnimatedMeshesBase::RenderDebugBoneHierarchy(CEngineInternalPrimitiveAnimatedMesh* mesh)
{
    g_pRenderStateManager->OpenCaptureBlock();
    C3DMeshInfo* info = mesh->MeshInfo;
    {
    unsigned long boneCount = info->BoneCount;
    CMatrixVector matrices(boneCount);
    float lod = g_LODTable_0143b934[mesh->LODIndex].Distance;
    C3DVector* scalings = mesh->GetBoneScalings(info);
    mesh->Link->Owner->AnimationManager->CalcBlendedMeshTransformations(
        (C3DAnimationInterpolator*)mesh->Anim, (C3DPositionInterpolator*)mesh->WorldPos, lod, scalings, info,
        matrices.GetData(), boneCount);
    bool draw = false;
    for (unsigned long i = 0; i < boneCount; ++i) {
        CBoneRecord& bone = info->Bones[i];
        if (bone.Parent != -1 && draw) {
            CMatrix3x4 parent = matrices[bone.Parent];
            CMatrix3x4 child = matrices[i];
            C3DVector start = *(C3DVector*)&parent.m[9] * mesh->WorldTransform;
            C3DVector end = *(C3DVector*)&child.m[9] * mesh->WorldTransform;
            CEnginePrimitiveLine line;
            line.Start = start;
            line.End = end;
            g_pPrimitiveRenderer_01436e80->AddPrimitive(&line);
        }
        if (bone.NoChildren > 1)
            draw = true;
    }
    }
    g_pRenderStateManager->RestoreCaptureBlock();
}
