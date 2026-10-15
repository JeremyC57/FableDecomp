// Behaviour test for 0x00851D30 (NCameraModes::CScriptModuleBase::SetCoordAxis): the whole 24-byte axis
// set is copied into CoordAxis and nothing else changes.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CScriptModuleBase {
    long            m_before;
    CRightHandedSet CoordAxis;
    long            m_after;
    void SetCoordAxis(const CRightHandedSet& axis);
};

void CScriptModuleBase::SetCoordAxis(const CRightHandedSet& axis)
{
    CoordAxis = axis;
}

int main()
{
    CScriptModuleBase m;
    m.m_before = 11; m.m_after = 22;
    CRightHandedSet axis = { { 0.0f, 0.0f, 1.0f }, { 1.0f, 2.0f, 3.0f } };
    m.SetCoordAxis(axis);
    if (m.CoordAxis.Up.Z != 1.0f || m.CoordAxis.Forward.X != 1.0f || m.CoordAxis.Forward.Z != 3.0f || m.m_before != 11 || m.m_after != 22) {
        std::printf("00851d30_TEST FAIL\n");
        return 1;
    }
    std::printf("00851d30_TEST PASS\n");
    return 0;
}