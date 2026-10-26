// CEngineInternalAttachedPrimitive::GetChildInverseTransformStaticMesh(CMatrix3x4* inverse) -- for a static-mesh child: start loading the mesh stats and fail if they are not loaded yet, else invert the child dummy object's transform (identity when the child has no such dummy) into *inverse
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

class CCharStringData { public:
    char*         m_pData;
    unsigned long m_length;
};
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer { public:
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

class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline void SetIdentity()
    {
        E43 = 0.0f; E42 = 0.0f; E41 = 0.0f;
        E32 = 0.0f; E31 = 0.0f; E23 = 0.0f; E21 = 0.0f; E13 = 0.0f; E12 = 0.0f;
        E33 = 1.0f; E22 = 1.0f; E11 = 1.0f;
    }
    __forceinline void SetInverse(const CMatrix3x4& m)
    {
        float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
        float inv = 1.0f / det;
        E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
        E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
    }
};
class C3DDummyObject { public:
    CMatrix3x4 Transform;                                   // +0x00
};
class C3DMeshStats { public:
    const C3DDummyObject* GetDummyObject(unsigned long key, unsigned long index) const;   // 0x00A2E920
};
class C3DMeshInfo { public:
    bool IsLoadedStats() const;                             // 0x00A23E80
    void BeginLoadingStats(bool high);                      // 0x00A24A80
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};

class CEnginePrimitiveAttachmentInfo { public:                     // 20 bytes
    unsigned long Flags;
    CCharString   ParentDummyName;
    long          ParentDummyIndex;
    CCharString   ChildDummyName;
    long          ChildDummyIndex;
};
class CEngineInternalAttachedPrimitive { public:
    unsigned char                     _pad00[0x8c];
    unsigned long                     OutputPrimitiveHandle;   // +0x8c
    CEngineInternalPrimitiveMeshBase* OutputPrimitive;      // +0x90 CIVCountedPointer Data
    CEnginePrimitiveAttachmentInfo    AttachmentInfo;       // +0x94
    bool GetChildInverseTransformStaticMesh(CMatrix3x4* inverse);
};

bool CEngineInternalAttachedPrimitive::GetChildInverseTransformStaticMesh(CMatrix3x4* inverse)
{
    float u0 = 0.0f;
    float u1 = 0.0f;
    float u2 = 0.0f;
    float u3 = 0.0f;
    float u4 = 0.0f;
    float u5 = 0.0f;
    float u6 = 0.0f;
    CEngineInternalPrimitiveMeshBase* child = OutputPrimitive;
    if (child->MeshInfo->IsLoadedStats()) {
        CCountedPointer<const C3DMeshStats> stats = child->MeshInfo->GetMeshStats();
        const CCharString& name = AttachmentInfo.ChildDummyName;
        unsigned long key = CCRC::Calc(0, name.GetPString(), name.GetLength());
        const C3DDummyObject* dummy = stats->GetDummyObject(key, AttachmentInfo.ChildDummyIndex);
        if (dummy)
            inverse->SetInverse(dummy->Transform);
        else
            inverse->SetIdentity();
        return true;
    }
    child->MeshInfo->BeginLoadingStats(false);
    return false;
}