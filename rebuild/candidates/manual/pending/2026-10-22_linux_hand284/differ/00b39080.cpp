// CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh(CMatrix3x4& transform, float frame) -- the parent animated mesh's dummy-object transform at the given frame (the parent group child when a group index is set): false without a parent, while the parent's mesh stats are still loading (loading is started) or when the parent has no such dummy; else the dummy transform, post-multiplied by the parent's interpolated object transform with the parent's scale divided back out of the rotation rows
// Grade: DIFFER(846v846) -- one scheduling slot: the success path's `lea ecx,[esp+0x74]` for the out-of-line stats release lands one scale store earlier than retail (scale written as members, a forceinline ScaleRotation, operand swap, scoping and direct-init variants do not move it); everything else exact
// Leaf: GetParentTransformAnimatedMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B39080, length 846 bytes (`ret 8`).
// Callees: GetChildPrimitive (group vtable slot 12, +0x30), C3DMeshInfo::IsLoadedStats 0x00A23E80, BeginLoadingStats(bool)
// 0x00A24A80, GetMeshStats() const 0x00A25530 (counted pointer by hidden pointer, released out of line through
// 0x004C24C0), Calc 0x004014A0 (CRC of the parent dummy name), C3DMeshStats::GetDummyObject 0x00A2E920, the stats'
// virtual slot 8 (+0x20, the animated dummy transform), CalcInterpolatedObjectTransform (slot 33, +0x84). Constant 1.0f
// 0x0122DED8.
// Layout: Ego_r.pdb CEngineInternalAttachedPrimitive (ParentHandle primitive +0x84, ParentGroupIndex +0x88,
// AttachmentInfo +0x94 {ParentDummyName +0x98, ParentDummyIndex +0x9c}); the parent is a mesh primitive (MeshInfo +0x50,
// Scale +0x58) whose animation state at +0x74 / +0x7c is passed by address; the mesh info's +4 object holds the bone set at
// +0x3c0. The stats / dummy / CRC model is the sibling GetChildInverseTransformStaticMesh 0x00B393D0.
// Shape: the product is an inline Multiply(a, b) on the target over a named copy `a` (an operator*= with an internal
// copy loads the wrong operand of the row-2..4 products); the null-dummy exit is the fall-out of `if (dummy) {...}`.
#include "engine/_quarantine/CEngineInternalPrimitiveMeshBase.h"

struct CCharStringData {
    char*         m_pData;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    T* operator->() const { return m_pObject; }
    ~CCountedPointer();                                 // 0x004C24C0 (const C3DMeshStats)
};

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline void ScaleRotation(float s)
    {
        E11 *= s; E12 *= s; E13 *= s;
        E21 *= s; E22 *= s; E23 *= s;
        E31 *= s; E32 *= s; E33 *= s;
    }
    __forceinline void Multiply(const CMatrix3x4& a, const CMatrix3x4& b)
    {
        E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
        E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
        E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
        E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
        E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
        E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
        E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
        E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
        E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
        E41 = (a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31) + b.E41;
        E42 = (a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32) + b.E42;
        E43 = (a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33) + b.E43;
    }
};
struct C3DDummyObject;
struct C3DMeshStats {
    const C3DDummyObject* GetDummyObject(unsigned long key, unsigned long index) const;   // 0x00A2E920
    virtual void v0() const; virtual void v1() const; virtual void v2() const; virtual void v3() const;
    virtual void v4() const; virtual void v5() const; virtual void v6() const; virtual void v7() const;
    virtual void GetAnimatedDummyTransform(const C3DDummyObject* dummy, CMatrix3x4& out, const void* anim, const void* blend,
                                           float frame, const void* bones, bool flag) const;   // slot 8 (+0x20)
};
struct CBoneHolder { unsigned char _pad[0x3c0]; void* Bones; };   // +0x3c0
struct C3DMeshInfo {
    unsigned char _pad00[4];
    CBoneHolder*  Holder;                                   // +0x04
    bool IsLoadedStats() const;                             // 0x00A23E80
    void BeginLoadingStats(bool high);                      // 0x00A24A80
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};
class CParentMesh {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CParentMesh* GetChildPrimitive(unsigned long index);   // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool scale) const;   // slot 33 (+0x84)
    unsigned char _pad04[0x50 - 4];
    C3DMeshInfo*  MeshInfo;                                 // +0x50
    C3DMeshInfo*  ShadowMeshInfo;                           // +0x54
    float         Scale;                                    // +0x58
    unsigned char _pad5c[0x74 - 0x5c];
    unsigned char AnimState[8];                             // +0x74
    unsigned char BlendState[8];                            // +0x7c
};
struct CEnginePrimitiveAttachmentInfo {                     // 20 bytes
    unsigned long Flags;
    CCharString   ParentDummyName;
    long          ParentDummyIndex;
    CCharString   ChildDummyName;
    long          ChildDummyIndex;
};
struct CEngineInternalAttachedPrimitive {
    unsigned char                  _pad00[0x84];
    CParentMesh*                   ParentPrimitive;         // +0x84 (ParentHandle data)
    long                           ParentGroupIndex;        // +0x88
    unsigned char                  _pad8c[8];
    CEnginePrimitiveAttachmentInfo AttachmentInfo;          // +0x94
    bool GetParentTransformAnimatedMesh(CMatrix3x4& transform, float frame);
};

bool CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh(CMatrix3x4& transform, float frame)
{
    CParentMesh* parent = ParentGroupIndex != -1 ? ParentPrimitive->GetChildPrimitive(ParentGroupIndex) : ParentPrimitive;
    if (!parent)
        return false;
    if (parent->MeshInfo->IsLoadedStats()) {
        CCountedPointer<const C3DMeshStats> stats = parent->MeshInfo->GetMeshStats();
        const CCharString& name = AttachmentInfo.ParentDummyName;
        unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
        const C3DDummyObject* dummy = stats->GetDummyObject(key, AttachmentInfo.ParentDummyIndex);
        if (dummy) {
        stats->GetAnimatedDummyTransform(dummy, transform, parent->AnimState, parent->BlendState, frame,
                                         parent->MeshInfo->Holder->Bones, false);
        CMatrix3x4 objectTransform;
        parent->CalcInterpolatedObjectTransform(objectTransform, frame, true);
        {
            CMatrix3x4 a(transform);
            transform.Multiply(a, objectTransform);
        }
        transform.ScaleRotation(1.0f / parent->Scale);
        return true;
        }
        return false;
    }
    parent->MeshInfo->BeginLoadingStats(false);
    return false;
}
