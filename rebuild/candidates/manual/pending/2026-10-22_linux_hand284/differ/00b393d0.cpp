// CEngineInternalAttachedPrimitive::GetChildInverseTransformStaticMesh(CMatrix3x4* inverse) -- for a static-mesh child: start loading the mesh stats and fail if they are not loaded yet, else invert the child dummy object's transform (identity when the child has no such dummy) into *inverse
// Grade: DIFFER(529v521) -- the inline 3x3 inverse: VC7.1 picks its own fld/fmul operand order for the cofactor products (source order and named-member forms do not move it), sums the determinant terms in another order and spills the first output element to an extra frame word (sub esp,0xc vs 8); the stats/dummy lookup and release are exact
// Leaf: GetChildInverseTransformStaticMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B393D0, length 519 bytes (`ret 4`).
// Callees: C3DMeshInfo::IsLoadedStats 0x00A23E80, BeginLoadingStats(bool) 0x00A24A80, GetMeshStats() const 0x00A25530
// (counted pointer by hidden pointer), Calc 0x004014A0 (`__fastcall(0, chars, length)` CRC, "" 0x0129AAF4 for a null
// string), C3DMeshStats::GetDummyObject(unsigned long, unsigned long) 0x00A2E920, operator delete 0x00BFE9BC (count block).
// Layout (Ego_r.pdb CEngineInternalAttachedPrimitive, 220 bytes): OutputPrimitive (CEnginePrimitiveHandle) +0x8c, its
// counted Data pointer +0x90; AttachmentInfo +0x94 {Flags, ParentDummyName +0x98, ParentDummyIndex +0x9c, ChildDummyName
// +0xa0, ChildDummyIndex +0xa4}. The child primitive is a CEngineInternalPrimitiveMeshBase (MeshInfo +0x50).
// Shape: the stats are a counted pointer released inline at the end (the landed HasDummyObject 0x004BE2C0 model); the
// inverse is an inline cofactor/determinant 3x3 inverse plus the negated rotated translation, written in retail's
// operand order.
#include "engine/_quarantine/CEngineInternalPrimitiveMeshBase.h"
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

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
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CMatrix3x4 {
    float m[12];
    __forceinline void SetIdentity()
    {
        m[11] = 0.0f; m[10] = 0.0f; m[9] = 0.0f;
        m[7] = 0.0f; m[6] = 0.0f; m[5] = 0.0f; m[3] = 0.0f; m[2] = 0.0f; m[1] = 0.0f;
        m[8] = 1.0f; m[4] = 1.0f; m[0] = 1.0f;
    }
    __forceinline void GetInverse(CMatrix3x4* out) const
    {
        float c0 = m[4] * m[8] - m[5] * m[7];
        float det = c0 * m[0] + ((m[7] * m[2] - m[1] * m[8]) * m[3] + (m[5] * m[1] - m[4] * m[2]) * m[6]);
        float inv = 1.0f / det;
        out->m[0] = c0 * inv;
        out->m[3] = (m[5] * m[6] - m[3] * m[8]) * inv;
        out->m[6] = (m[3] * m[7] - m[4] * m[6]) * inv;
        out->m[1] = (m[7] * m[2] - m[1] * m[8]) * inv;
        out->m[4] = (m[8] * m[0] - m[6] * m[2]) * inv;
        out->m[7] = (m[1] * m[6] - m[7] * m[0]) * inv;
        out->m[2] = (m[5] * m[1] - m[4] * m[2]) * inv;
        out->m[5] = (m[3] * m[2] - m[5] * m[0]) * inv;
        out->m[8] = (m[4] * m[0] - m[3] * m[1]) * inv;
        out->m[9] = -(m[11] * out->m[6] + m[10] * out->m[3] + m[9] * out->m[0]);
        out->m[10] = -(m[11] * out->m[7] + m[9] * out->m[1] + out->m[4] * m[10]);
        out->m[11] = -(out->m[2] * m[9] + out->m[5] * m[10] + m[11] * out->m[8]);
    }
};
struct C3DDummyObject {
    CMatrix3x4 Transform;                                   // +0x00
};
struct C3DMeshStats {
    const C3DDummyObject* GetDummyObject(unsigned long key, unsigned long index) const;   // 0x00A2E920
};
struct C3DMeshInfo {
    bool IsLoadedStats() const;                             // 0x00A23E80
    void BeginLoadingStats(bool high);                      // 0x00A24A80
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};

struct CEnginePrimitiveAttachmentInfo {                     // 20 bytes
    unsigned long Flags;
    CCharString   ParentDummyName;
    long          ParentDummyIndex;
    CCharString   ChildDummyName;
    long          ChildDummyIndex;
};
struct CEngineInternalAttachedPrimitive {
    unsigned char                     _pad00[0x8c];
    unsigned long                     OutputPrimitiveHandle;   // +0x8c
    CEngineInternalPrimitiveMeshBase* OutputPrimitive;      // +0x90 CIVCountedPointer Data
    CEnginePrimitiveAttachmentInfo    AttachmentInfo;       // +0x94
    bool GetChildInverseTransformStaticMesh(CMatrix3x4* inverse);
};

bool CEngineInternalAttachedPrimitive::GetChildInverseTransformStaticMesh(CMatrix3x4* inverse)
{
    CEngineInternalPrimitiveMeshBase* child = OutputPrimitive;
    if (child->MeshInfo->IsLoadedStats()) {
        CCountedPointer<const C3DMeshStats> stats = child->MeshInfo->GetMeshStats();
        const CCharString& name = AttachmentInfo.ChildDummyName;
        unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
        const C3DDummyObject* dummy = stats->GetDummyObject(key, AttachmentInfo.ChildDummyIndex);
        if (dummy)
            dummy->Transform.GetInverse(inverse);
        else
            inverse->SetIdentity();
        return true;
    }
    child->MeshInfo->BeginLoadingStats(false);
    return false;
}
