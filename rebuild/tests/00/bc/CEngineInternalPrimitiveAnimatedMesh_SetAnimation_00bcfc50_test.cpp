#include <cstdio>
static int g_anim = 0, g_orient = 0;
struct C3DAnimationInterpolator { unsigned char _body[0x18]; C3DAnimationInterpolator& operator=(const C3DAnimationInterpolator& o) { ++g_anim; _body[0] = o._body[0]; return *this; } };
struct C3DOrientationInterpolator { unsigned char _body[0x10]; C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& o) { ++g_orient; _body[0] = o._body[0]; return *this; } };
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct SAnimationState { C3DAnimationInterpolator m_animation; C3DVector m_position; C3DVector m_scale; C3DOrientationInterpolator m_orientation; };
struct CMesh { unsigned char _pad[0x70]; C3DAnimationInterpolator m_animation; C3DVector m_position; C3DVector m_scale; C3DOrientationInterpolator m_orientation;
    void SetAnimation(const SAnimationState& s) { m_animation = s.m_animation; m_position = s.m_position; m_scale = s.m_scale; m_orientation = s.m_orientation; } };
int main() { SAnimationState s; s.m_animation._body[0] = 1; s.m_position.X = 2; s.m_position.Z = 3; s.m_scale.Y = 4; s.m_orientation._body[0] = 5; CMesh m; m.SetAnimation(s);
    if (g_anim != 1 || g_orient != 1 || m.m_animation._body[0] != 1 || m.m_position.X != 2 || m.m_position.Z != 3 || m.m_scale.Y != 4 || m.m_orientation._body[0] != 5) { std::printf("00bcfc50_TEST FAIL\n"); return 1; } std::printf("00bcfc50_TEST PASS\n"); return 0; }