// Behaviour test for 00705b00 (CTCCarryable::GetCarriedPositionInfo): the pose query fills both start values (with forceUpdate false and the time), the end values copy them, and its result is returned.
#include <cstdio>

struct C3DVector { float X; float Y; float Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct C3DPositionInterpolator {
    C3DVector StartPosition;
    C3DVector EndPosition;
};

struct C3DOrientationInterpolator {
    CRightHandedSet StartOrientation;
    CRightHandedSet EndOrientation;
};

struct CTCCarryable {
    bool  m_result;
    bool  m_lastForce;
    float m_lastTime;
    bool GetCarriedPositionInfo(C3DVector* position, CRightHandedSet* orientation, bool forceUpdate, float time)
    {
        m_lastForce = forceUpdate; m_lastTime = time;
        position->X = 1.0f; position->Y = 2.0f; position->Z = 3.0f;
        orientation->Up.X = 0.0f; orientation->Up.Y = 0.0f; orientation->Up.Z = 1.0f;
        orientation->Forward.X = 0.0f; orientation->Forward.Y = 1.0f; orientation->Forward.Z = 0.0f;
        return m_result;
    }
    bool GetCarriedPositionInfo(C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation, float time)
    {
        bool result = GetCarriedPositionInfo(&position->StartPosition, &orientation->StartOrientation, false, time);
        position->EndPosition = position->StartPosition;
        orientation->EndOrientation = orientation->StartOrientation;
        return result;
    }
};

int main()
{
    CTCCarryable c; c.m_result = true; c.m_lastForce = true; c.m_lastTime = 0.0f;
    C3DPositionInterpolator pos = { { 0, 0, 0 }, { 9, 9, 9 } };
    C3DOrientationInterpolator ori = { { { 0, 0, 0 }, { 0, 0, 0 } }, { { 9, 9, 9 }, { 9, 9, 9 } } };
    bool r = c.GetCarriedPositionInfo(&pos, &ori, 0.5f);
    bool ok = r && !c.m_lastForce && c.m_lastTime == 0.5f && pos.EndPosition.X == 1.0f && pos.EndPosition.Y == 2.0f && pos.EndPosition.Z == 3.0f
        && ori.EndOrientation.Up.Z == 1.0f && ori.EndOrientation.Forward.Y == 1.0f && ori.EndOrientation.Forward.X == 0.0f;
    c.m_result = false;
    ok = ok && !c.GetCarriedPositionInfo(&pos, &ori, 0.25f);
    if (!ok) { std::printf("00705b00_TEST FAIL\n"); return 1; }
    std::printf("00705b00_TEST PASS\n");
    return 0;
}
