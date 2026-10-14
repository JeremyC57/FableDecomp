#include <cstdio>
struct C3DVector { float X; float Y; float Z; };
struct CTrackedVector {
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};
struct CTrackedTransform_00c40450 {
    C3DVector m_stored; long m_flags;
    void SetPosition(const CTrackedVector& v) { m_stored = v.Value; m_flags |= 0x8000; }
};
struct CScriptRenderBinding_00c338c0 {
    void SetTargetPosition(CTrackedTransform_00c40450* target, const C3DVector& position);
};
void CScriptRenderBinding_00c338c0::SetTargetPosition(CTrackedTransform_00c40450* target, const C3DVector& position)
{
    target->SetPosition(CTrackedVector(position));
}
int main()
{
    CTrackedTransform_00c40450 t = { { 0, 0, 0 }, 0 };
    CScriptRenderBinding_00c338c0 b;
    C3DVector p = { 1.0f, 2.0f, 3.0f };
    b.SetTargetPosition(&t, p);
    if (t.m_stored.X != 1.0f || t.m_stored.Y != 2.0f || t.m_stored.Z != 3.0f || t.m_flags != 0x8000) { std::printf("00c338c0_TEST FAIL\n"); return 1; }
    std::printf("00c338c0_TEST PASS\n");
    return 0;
}