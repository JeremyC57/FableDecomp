// Behaviour test for UpdateDirectionOverride (007a7000): stick past 0.05 becomes the unit override direction (Y flipped), rotated.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; };
struct Rush {
    bool DirectionOverride; V3 Dir; bool eventQueued; float stickX, stickY; float angle;
    void Update()
    {
        if (eventQueued) {
            float sx = stickX, sy = stickY;
            if ((float)std::sqrt(sy * sy + sx * sx) > 0.05f) {
                Dir.X = sx; Dir.Y = -sy; DirectionOverride = true; Dir.Z = 0.0f;
                float r = 1.0f / (float)std::sqrt(Dir.X * Dir.X + Dir.Y * Dir.Y + Dir.Z * Dir.Z);
                Dir.X *= r; Dir.Y *= r; Dir.Z *= r;
                float c = (float)std::cos(angle), s = (float)std::sin(angle);   // stand-in for the rotate-about-Z helper
                float x = Dir.X * c - Dir.Y * s, y = Dir.X * s + Dir.Y * c;
                Dir.X = x; Dir.Y = y;
            }
        }
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 0.001f; }
int main()
{
    Rush r = { false, { 0, 0, 0 }, true, 0.03f, 0.03f, 0.0f };
    r.Update();
    if (r.DirectionOverride) { std::printf("007a7000_TEST FAIL\n"); return 1; }    // length 0.042 < 0.05
    r.stickX = 0.6f; r.stickY = 0.8f; r.Update();
    if (!r.DirectionOverride || !near(r.Dir.X, 0.6f) || !near(r.Dir.Y, -0.8f) || r.Dir.Z != 0.0f) { std::printf("007a7000_TEST FAIL\n"); return 1; }
    Rush q = { false, { 0, 0, 0 }, false, 1.0f, 0.0f, 0.0f }; q.Update();
    if (q.DirectionOverride) { std::printf("007a7000_TEST FAIL\n"); return 1; }
    std::printf("007a7000_TEST PASS\n");
    return 0;
}