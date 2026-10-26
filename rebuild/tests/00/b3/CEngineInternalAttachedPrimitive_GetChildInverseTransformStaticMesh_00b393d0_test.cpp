// 00b393d0 behaviour test: CEngineInternalAttachedPrimitive::GetChildInverseTransformStaticMesh (match.cpp body, host-modelled)
#include <stdio.h>
#include <math.h>
#include <string.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CCharStringData { public: char* m_pData; unsigned long m_length; };
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
static unsigned long Calc_004014a0(unsigned long seed, const char* data, unsigned long length)
{ unsigned long h = seed; for (unsigned long i = 0; i < length; ++i) h = h * 31 + (unsigned char)data[i]; return h; }
static int g_destroyed = 0;
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template <class T> class CCountedPointer { public:
    T* m_pObject; SCountBlock* m_pCount;
    T* operator->() const { return m_pObject; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
        }
        m_pObject = 0; m_pCount = 0;
    }
};
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void SetIdentity()
    {
        E43 = 0.0f; E42 = 0.0f; E41 = 0.0f;
        E32 = 0.0f; E31 = 0.0f; E23 = 0.0f; E21 = 0.0f; E13 = 0.0f; E12 = 0.0f;
        E33 = 1.0f; E22 = 1.0f; E11 = 1.0f;
    }
    void SetInverse(const CMatrix3x4& m)
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
class C3DDummyObject { public: CMatrix3x4 Transform; };
class C3DMeshStats { public:
    const C3DDummyObject* dummy; unsigned long wantKey, lastKey, lastIndex;
    const C3DDummyObject* GetDummyObject(unsigned long key, unsigned long index) const
    { const_cast<C3DMeshStats*>(this)->lastKey = key; const_cast<C3DMeshStats*>(this)->lastIndex = index; return key == wantKey ? dummy : 0; }
};
static void destroyStats(void*) { ++g_destroyed; }
class C3DMeshInfo { public:
    bool loaded; int begins; bool lastHigh; C3DMeshStats* stats;
    bool IsLoadedStats() const { return loaded; }
    void BeginLoadingStats(bool high) { ++begins; lastHigh = high; }
    CCountedPointer<const C3DMeshStats> GetMeshStats() const
    { CCountedPointer<const C3DMeshStats> p; p.m_pObject = stats; p.m_pCount = new SCountBlock; p.m_pCount->m_references = 1; p.m_pCount->m_pfnDestroy = destroyStats; p.m_pCount->m_pObject = stats; return p; }
};
class CEngineInternalPrimitiveMeshBase { public: C3DMeshInfo* MeshInfo; };
class CEnginePrimitiveAttachmentInfo { public: unsigned long Flags; CCharString ParentDummyName; long ParentDummyIndex; CCharString ChildDummyName; long ChildDummyIndex; };
class CEngineInternalAttachedPrimitive { public:
    CEngineInternalPrimitiveMeshBase* OutputPrimitive;
    CEnginePrimitiveAttachmentInfo AttachmentInfo;
    bool GetChildInverseTransformStaticMesh(CMatrix3x4* inverse)
    {
        float u0 = 0.0f; float u1 = 0.0f; float u2 = 0.0f; float u3 = 0.0f; float u4 = 0.0f; float u5 = 0.0f; float u6 = 0.0f;
        CEngineInternalPrimitiveMeshBase* child = OutputPrimitive;
        if (child->MeshInfo->IsLoadedStats()) {
            CCountedPointer<const C3DMeshStats> stats = child->MeshInfo->GetMeshStats();
            const CCharString& name = AttachmentInfo.ChildDummyName;
            unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
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
};
int main()
{
    char nm[] = "bone"; CCharStringData sd = { nm, 4 };
    C3DDummyObject dummy; float v[12] = { 2,0,1, 1,3,0, 0,1,4, 5,6,7 }; memcpy(&dummy.Transform, v, sizeof v);
    C3DMeshStats stats = { &dummy, Calc_004014a0(0, "bone", 4), 0, 0 };
    C3DMeshInfo info = { false, 0, true, &stats };
    CEngineInternalPrimitiveMeshBase mesh = { &info };
    CEngineInternalAttachedPrimitive p; p.OutputPrimitive = &mesh; p.AttachmentInfo.ChildDummyName.m_pStringData = &sd; p.AttachmentInfo.ChildDummyIndex = 3;
    CMatrix3x4 out; memset(&out, 0, sizeof out); out.E11 = 42.0f;
    CHECK(!p.GetChildInverseTransformStaticMesh(&out));
    CHECK(info.begins == 1 && !info.lastHigh && out.E11 == 42.0f && g_destroyed == 0);
    info.loaded = true;
    CHECK(p.GetChildInverseTransformStaticMesh(&out));
    CHECK(g_destroyed == 1 && stats.lastIndex == 3 && stats.lastKey == stats.wantKey);
    const CMatrix3x4& A = dummy.Transform; const CMatrix3x4& B = out;
    float a[4][3] = { {A.E11,A.E12,A.E13},{A.E21,A.E22,A.E23},{A.E31,A.E32,A.E33},{A.E41,A.E42,A.E43} };
    float b[4][3] = { {B.E11,B.E12,B.E13},{B.E21,B.E22,B.E23},{B.E31,B.E32,B.E33},{B.E41,B.E42,B.E43} };
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 3; ++j) {
        float s = (i == 3 ? b[3][j] : 0.0f); for (int k = 0; k < 3; ++k) s += a[i][k] * b[k][j];
        CHECK(fabs(s - (i == j ? 1.0f : 0.0f)) < 1e-4f);
    }
    p.AttachmentInfo.ChildDummyName.m_pStringData = 0;   // "" -> no dummy -> identity
    CHECK(p.GetChildInverseTransformStaticMesh(&out));
    CHECK(out.E11 == 1 && out.E22 == 1 && out.E33 == 1 && out.E12 == 0 && out.E41 == 0 && out.E43 == 0 && g_destroyed == 2 && stats.lastKey == 0);
    if (!g_fail) printf("00b393d0_TEST PASS\n");
    return g_fail;
}