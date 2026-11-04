// Behaviour test for CSequenceTrack::Play 0x00C36220: both arguments go unchanged to the embedded cube's UpdateDistances.
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CAmbientCube { const C3DVector* Last; float Height; int Calls; CAmbientCube() : Last(0), Height(0), Calls(0) {}
    void UpdateDistances(const C3DVector& listener, float height) { Last = &listener; Height = height; ++Calls; } };
struct CSequenceTrack { char pad[0x4c]; CAmbientCube Cube;
    void Play(const C3DVector& position, float duration) { Cube.UpdateDistances(position, duration); } };
int main()
{
    CSequenceTrack t; C3DVector p = { 1, 2, 3 };
    t.Play(p, 2.5f);
    if (t.Cube.Calls != 1 || t.Cube.Last != &p || t.Cube.Height != 2.5f) { std::printf("00c36220_TEST FAIL\n"); return 1; }
    std::printf("00c36220_TEST PASS\n");
    return 0;
}