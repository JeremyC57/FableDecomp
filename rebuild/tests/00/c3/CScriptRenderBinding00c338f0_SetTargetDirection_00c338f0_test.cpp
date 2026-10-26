#include <cstdio>
class C3DVector { public: float X; float Y; float Z; };
struct CTrackedVector {
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};
struct CTrackedTransform_00c404a0 {
    C3DVector m_stored; long m_flags;
    void SetDirection(const CTrackedVector& v) { m_stored = v.Value; m_flags |= 0x8000; }
};
struct CScriptRenderBinding_00c338f0 {
    void SetTargetDirection(CTrackedTransform_00c404a0* target, const C3DVector& position);
};
void CScriptRenderBinding_00c338f0::SetTargetDirection(CTrackedTransform_00c404a0* target, const C3DVector& position)
{
    target->SetDirection(CTrackedVector(position));
}
int main()
{
    CTrackedTransform_00c404a0 t = { { 0, 0, 0 }, 0 };
    CScriptRenderBinding_00c338f0 b;
    C3DVector p = { 1.0f, 2.0f, 3.0f };
    b.SetTargetDirection(&t, p);
    if (t.m_stored.X != 1.0f || t.m_stored.Y != 2.0f || t.m_stored.Z != 3.0f || t.m_flags != 0x8000) { std::printf("00c338f0_TEST FAIL\n"); return 1; }
    std::printf("00c338f0_TEST PASS\n");
    return 0;
}