// Behaviour test for 0x008E5FB0 (catalog CThing::AddToRenderList): velocity scaled by (1 - air resistance) only while moving.
#include <cstdio>
struct C3DVector { float X, Y, Z; void operator*=(float f) { X *= f; Y *= f; Z *= f; } };
struct CThing {
    bool moving; float resistance; C3DVector Velocity;
    virtual bool IsMoving() { return moving; }
    float GetAirResistance() { return resistance; }
    void AddToRenderList()
    {
        if (IsMoving())
            Velocity *= 1.0f - GetAirResistance();
    }
};
int main()
{
    CThing t; t.moving = false; t.resistance = 0.25f; t.Velocity.X = 4.0f; t.Velocity.Y = -8.0f; t.Velocity.Z = 2.0f;
    t.AddToRenderList();
    if (t.Velocity.X != 4.0f) { std::printf("008e5fb0_TEST FAIL\n"); return 1; }
    t.moving = true; t.AddToRenderList();
    if (t.Velocity.X != 3.0f || t.Velocity.Y != -6.0f || t.Velocity.Z != 1.5f) { std::printf("008e5fb0_TEST FAIL\n"); return 1; }
    std::printf("008e5fb0_TEST PASS\n");
    return 0;
}
