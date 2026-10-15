// Behaviour test for 00878a10 CTCCameraPointDefinitionBase::GetActiveCameraPosition: the thing's position raised by 1.6 on Z.
#include <cstdio>

struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

struct CThing
{
    C3DVector m_pos;
    CThing() : m_pos(1.0f, 2.0f, 3.0f) {}
    const C3DVector& GetPosition() const { return m_pos; }
};

static C3DVector GetActiveCameraPosition(const CThing* thing)
{
    const C3DVector& position = thing->GetPosition();
    C3DVector camera(position.X, position.Y, position.Z + 1.6f);
    return camera;
}

int main()
{
    CThing thing;
    C3DVector c = GetActiveCameraPosition(&thing);
    float dz = c.Z - 4.6f;
    if (c.X != 1.0f || c.Y != 2.0f || dz > 0.0001f || dz < -0.0001f) { std::printf("00878a10_TEST FAIL\n"); return 1; }
    std::printf("00878a10_TEST PASS\n");
    return 0;
}