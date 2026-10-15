// Behaviour test for 0x00BF9AA0 (CLocalDetailPrimitiveBase::ClipPrimitive): fade-distance cull then the camera sphere clip's outside bit.
#include <cstdio>
struct V { float x, y, z; };
struct Flags { unsigned char Planes : 6; unsigned char Outside : 1; };
struct Type { float fade; float PeekFadeEnd() const { return fade; } };
struct Camera { V m_position; unsigned int result; int calls; bool ClipSphere(const V*, float, unsigned int* f) { ++calls; *f = result; return true; } };
struct Prim {
    V m_centre; float m_radius;
    bool Clip(Type* type, Camera* camera, bool skip, unsigned int clipFlags)
    {
        if (!skip) {
            V d; d.x = camera->m_position.x - m_centre.x; d.y = camera->m_position.y - m_centre.y; d.z = camera->m_position.z - m_centre.z;
            float range = type->PeekFadeEnd() + m_radius;
            if (range * range < d.z * d.z + d.y * d.y + d.x * d.x) return false;
        }
        camera->ClipSphere(&m_centre, m_radius, &clipFlags);
        return ((Flags*)&clipFlags)->Outside ? false : true;
    }
};
int main()
{
    Type t; t.fade = 10.0f;
    Camera c; c.m_position.x = 0; c.m_position.y = 0; c.m_position.z = 0; c.result = 0; c.calls = 0;
    Prim p; p.m_centre.x = 20; p.m_centre.y = 0; p.m_centre.z = 0; p.m_radius = 1.0f;
    int ok = !p.Clip(&t, &c, false, 0) && c.calls == 0;          // 20 > 11
    ok &= p.Clip(&t, &c, true, 0) && c.calls == 1;                // distance test skipped, inside
    c.result = 0x40; ok &= !p.Clip(&t, &c, true, 0);              // outside bit
    p.m_centre.x = 5; c.result = 0x3f; ok &= p.Clip(&t, &c, false, 0);
    if (!ok) { std::printf("00bf9aa0_TEST FAIL\n"); return 1; }
    std::printf("00bf9aa0_TEST PASS\n");
    return 0;
}
