// behaviour test for 0x00AECF90 CParticleEmitter::ShouldSpawn: continuous emitters always spawn, otherwise within MaxSpawnDistance of the camera
#include <cstdio>
struct C3DVector { float X, Y, Z; };
static C3DVector g_camera = { 10.0f, 0.0f, 0.0f };
struct CParticleEmitter {
    unsigned short ContinuousEmitter : 1; float MaxSpawnDistance;
    int ShouldSpawn(const C3DVector& pos) const
    {
        C3DVector camera = g_camera;
        if (!ContinuousEmitter) {
            float dx = pos.X - camera.X, dy = pos.Y - camera.Y, dz = pos.Z - camera.Z;
            if (!(dx * dx + dy * dy + dz * dz <= MaxSpawnDistance * MaxSpawnDistance)) return 0;
        }
        return 1;
    }
};
int main()
{
    CParticleEmitter e; e.ContinuousEmitter = 0; e.MaxSpawnDistance = 5.0f;
    C3DVector near_ = { 12.0f, 3.0f, 0.0f }, far_ = { 20.0f, 0.0f, 0.0f }, edge = { 15.0f, 0.0f, 0.0f };
    int a = e.ShouldSpawn(near_), b = e.ShouldSpawn(far_), c = e.ShouldSpawn(edge);
    e.ContinuousEmitter = 1; int d = e.ShouldSpawn(far_);
    if (a != 1 || b != 0 || c != 1 || d != 1) { std::printf("00aecf90_TEST FAIL\n"); return 1; }
    std::printf("00aecf90_TEST PASS\n");
    return 0;
}
