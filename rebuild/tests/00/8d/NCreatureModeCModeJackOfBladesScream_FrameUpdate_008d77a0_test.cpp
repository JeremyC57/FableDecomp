#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V operator-(const V& o) const { return V(X - o.X, Y - o.Y, Z - o.Z); }
    V operator+(const V& o) const { return V(X + o.X, Y + o.Y, Z + o.Z); }
    V operator*(float f) const { return V(X * f, Y * f, Z * f); } };
struct Scream {
    bool alive; bool falling; long timer; V fallPos; V initPos; V lastPos; float lastT; int updates;
    void FrameUpdate()
    {
        if (alive && falling) {
            if (timer <= 30) {
                float t = timer * (1.0f / 30.0f);
                lastT = t;
                V pos = (fallPos - initPos) * t + initPos;
                lastPos = pos; ++updates;
                ++timer;
            }
        }
    }
};
int main()
{
    Scream s; s.alive = true; s.falling = true; s.timer = 15; s.fallPos = V(30, 0, -30); s.initPos = V(0, 0, 0); s.updates = 0;
    s.FrameUpdate();
    bool ok = s.updates == 1 && s.timer == 16 && std::fabs(s.lastPos.X - 15.0f) < 1e-3f && std::fabs(s.lastPos.Z + 15.0f) < 1e-3f;
    s.timer = 31; s.FrameUpdate(); ok = ok && s.updates == 1 && s.timer == 31;
    s.timer = 0; s.falling = false; s.FrameUpdate(); ok = ok && s.updates == 1;
    if (!ok) { std::printf("008d77a0_TEST FAIL\n"); return 1; }
    std::printf("008d77a0_TEST PASS\n");
    return 0;
}