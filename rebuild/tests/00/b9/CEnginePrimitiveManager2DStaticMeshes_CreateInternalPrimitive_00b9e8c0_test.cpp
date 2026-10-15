// Behaviour test for CEnginePrimitiveManager2DStaticMeshes::CreateInternalPrimitive @ 0x00B9E8C0.
#include <cstdio>
struct CEngineInternalPrimitiveBase {
    long m_type; unsigned int m_flagsBase;
    CEngineInternalPrimitiveBase(long t, unsigned int f) : m_type(t), m_flagsBase(f) { }
    virtual int Kind() { return 0; }
    virtual ~CEngineInternalPrimitiveBase() { }
};
struct CBaseClass { virtual int Kind() { return 0; } virtual ~CBaseClass() { } };
struct C3DOrientationInterpolator : CBaseClass { virtual int Kind() { return 5; } };
struct SFourWords { long m_words[4]; SFourWords() { m_words[0] = m_words[1] = m_words[2] = m_words[3] = 0; } };
struct CEngineInternalPrimitiveStaticMeshBase : CEngineInternalPrimitiveBase {
    long m_mesh, m_material, m_animation, m_frame, m_flags; bool m_visible, m_lit, m_dirty;
    CEngineInternalPrimitiveStaticMeshBase(long t, unsigned int f) : CEngineInternalPrimitiveBase(t, f), m_mesh(0), m_material(0),
        m_animation(0), m_frame(0), m_flags(0), m_visible(false), m_lit(false), m_dirty(false) { }
};
struct CEngineInternalPrimitive2DStaticMesh : CEngineInternalPrimitiveStaticMeshBase {
    SFourWords m_bounds; C3DOrientationInterpolator m_orientation;
    virtual int Kind() { return 3; }
    CEngineInternalPrimitive2DStaticMesh() : CEngineInternalPrimitiveStaticMeshBase(0x24, 0x80000000u) { }
};
struct CEnginePrimitiveManager2DStaticMeshes {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type)
    {
        if (type != 0x24) return 0;
        return new CEngineInternalPrimitive2DStaticMesh;
    }
};
int main()
{
    CEnginePrimitiveManager2DStaticMeshes m;
    CEngineInternalPrimitive2DStaticMesh* p = (CEngineInternalPrimitive2DStaticMesh*)m.CreateInternalPrimitive(0x24);
    int ok = p && p->Kind() == 3 && p->m_type == 0x24 && p->m_flagsBase == 0x80000000u && p->m_mesh == 0 && p->m_frame == 0
          && !p->m_visible && !p->m_dirty && p->m_bounds.m_words[3] == 0 && p->m_orientation.Kind() == 5
          && m.CreateInternalPrimitive(0x23) == 0 && m.CreateInternalPrimitive(0) == 0;
    delete p;
    if (!ok) { std::printf("00b9e8c0_TEST FAIL\n"); return 1; }
    std::printf("00b9e8c0_TEST PASS\n");
    return 0;
}