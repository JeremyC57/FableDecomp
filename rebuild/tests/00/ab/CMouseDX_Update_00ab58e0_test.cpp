// Behaviour test for CMouseDX::Update 0x00AB58E0: movement accumulation by event type, the 0.001 threshold, the optional
// averaging filter against the previous update's raw movement, and the recentre position (half the back buffer).
#include <cstdio>
#include <cmath>

struct Vec3 { float X, Y, Z; };
struct MouseEvent { int Type; float Movement; };

struct Mouse {
    bool Filtering;
    Vec3 LastUpdateMovement;
    bool Posted;
    Vec3 PostedMovement;
    float PosX, PosY, OldX, OldY;
    long CursorX, CursorY;

    void Update(const MouseEvent* events, int count)
    {
        Vec3 movement = { 0.0f, 0.0f, 0.0f };
        Vec3 filtered = { 0.0f, 0.0f, 0.0f };
        Posted = false;
        for (int i = 0; i < count; ++i) {
            switch (events[i].Type) {
            case 7: case 12: movement.X += events[i].Movement; break;
            case 8: case 13: movement.Y += events[i].Movement; break;
            case 9: case 14: movement.Z += events[i].Movement; break;
            }
        }
        if (std::fabs(movement.X) > 0.001f || std::fabs(movement.Y) > 0.001f || std::fabs(movement.Z) > 0.001f) {
            if (Filtering) {
                filtered.X = (std::fabs(movement.X) > 0.0f && std::fabs(LastUpdateMovement.X) > 0.0f) ? (movement.X + LastUpdateMovement.X) * 0.5f : movement.X;
                filtered.Y = (std::fabs(movement.Y) > 0.0f && std::fabs(LastUpdateMovement.Y) > 0.0f) ? (movement.Y + LastUpdateMovement.Y) * 0.5f : movement.Y;
                filtered.Z = (std::fabs(movement.Z) > 0.0f && std::fabs(LastUpdateMovement.Z) > 0.0f) ? (movement.Z + LastUpdateMovement.Z) * 0.5f : movement.Z;
            } else {
                filtered = movement;
            }
            LastUpdateMovement = movement;
            PostedMovement = filtered;
            Posted = true;
        }
    }
    void Recentre(long left, long top, long width, long height)
    {
        CursorX = width / 2 + left;
        CursorY = height / 2 + top;
        PosX = (float)(width / 2);
        PosY = (float)(height / 2);
        OldX = PosX;
        OldY = PosY;
    }
};

int main()
{
    Mouse m;
    m.Filtering = true;
    m.LastUpdateMovement.X = 4.0f; m.LastUpdateMovement.Y = 0.0f; m.LastUpdateMovement.Z = 2.0f;
    MouseEvent ev[] = { { 7, 2.0f }, { 12, 4.0f }, { 8, 3.0f }, { 10, 100.0f }, { 14, -2.0f }, { 1, 50.0f } };
    m.Update(ev, 6);
    // raw (6, 3, -2); X averaged with 4 -> 5, Y kept (last Y zero), Z averaged with 2 -> 0
    if (!m.Posted || m.PostedMovement.X != 5.0f || m.PostedMovement.Y != 3.0f || m.PostedMovement.Z != 0.0f
        || m.LastUpdateMovement.X != 6.0f || m.LastUpdateMovement.Z != -2.0f) {
        std::printf("00ab58e0_TEST FAIL\n"); return 1;
    }
    MouseEvent tiny[] = { { 7, 0.0005f } };
    m.Update(tiny, 1);
    if (m.Posted) { std::printf("00ab58e0_TEST FAIL\n"); return 1; }
    m.Filtering = false;
    m.Update(ev, 2);
    if (!m.Posted || m.PostedMovement.X != 6.0f) { std::printf("00ab58e0_TEST FAIL\n"); return 1; }
    m.Recentre(100, 50, 1025, 768);
    if (m.CursorX != 612 || m.CursorY != 434 || m.PosX != 512.0f || m.OldY != 384.0f) {
        std::printf("00ab58e0_TEST FAIL\n"); return 1;
    }
    std::printf("00ab58e0_TEST PASS\n");
    return 0;
}