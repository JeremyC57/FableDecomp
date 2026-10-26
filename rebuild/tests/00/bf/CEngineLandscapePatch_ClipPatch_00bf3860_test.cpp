// Behaviour test for CEngineLandscapePatch::ClipPatch @ 0x00BF3860.
#include <cstdio>

class C3DVector { public: float x, y, z; };
struct CFrustumPlane { float nx, ny, nz, d; };
class CEngineCamera { public: unsigned char m_unknown[0x1c0]; CFrustumPlane m_planes[6]; };
static CEngineCamera g_camera;
static CEngineCamera* g_pEngineCamera = &g_camera;

class CEngineLandscapePatch { public:
    unsigned char _pad00[0x20];
    C3DVector m_boxMin, m_boxMax;
    bool ClipPatch()
    {
        for (long i = 0; i < 4; ++i) {
            const CFrustumPlane& plane = g_pEngineCamera->m_planes[i];
            float nx = plane.nx, ny = plane.ny, nz = plane.nz;
            float px = nx > 0.0f ? m_boxMin.x : m_boxMax.x;
            float py = ny > 0.0f ? m_boxMin.y : m_boxMax.y;
            float pz = nz > 0.0f ? m_boxMin.z : m_boxMax.z;
            if (pz * nz + py * ny + px * nx > plane.d)
                return false;
        }
        return true;
    }
};

static void SetPlane(int i, float x, float y, float z, float d)
{ g_camera.m_planes[i].nx = x; g_camera.m_planes[i].ny = y; g_camera.m_planes[i].nz = z; g_camera.m_planes[i].d = d; }

int main()
{
    CEngineLandscapePatch p;
    p.m_boxMin.x = 0; p.m_boxMin.y = 0; p.m_boxMin.z = 0; p.m_boxMax.x = 10; p.m_boxMax.y = 10; p.m_boxMax.z = 10;
    // four planes, all with d large: box inside -> true
    for (int i = 0; i < 4; ++i) SetPlane(i, 1, 0, 0, 100);
    SetPlane(4, 1, 0, 0, -1000); SetPlane(5, 1, 0, 0, -1000);   // planes 4/5 are never consulted
    int ok = p.ClipPatch() == true;
    // plane 2: x > 5 is "outside"; the box's min corner along +x is 0, so 0 > 5 is false -> still true
    SetPlane(2, 1, 0, 0, 5);  ok = ok && p.ClipPatch() == true;
    // plane 2 with d = -1: min.x = 0 > -1 -> whole box outside -> false
    SetPlane(2, 1, 0, 0, -1); ok = ok && p.ClipPatch() == false;
    // negative normal picks the max corner: -x * 10 = -10 > -11 -> false; > -9 -> fails, so true
    SetPlane(2, -1, 0, 0, -9); ok = ok && p.ClipPatch() == true;
    SetPlane(2, -1, 0, 0, -11); ok = ok && p.ClipPatch() == false;
    // zero normal component picks max (not greater than 0): 0*10 + 1*0 (ny>0 -> min.y) = 0 > -0.5 -> false
    SetPlane(2, 0, 1, 0, -0.5f); ok = ok && p.ClipPatch() == false;
    SetPlane(2, 0, 0, 1, 0.0f); ok = ok && p.ClipPatch() == true;   // equal is not outside
    if (!ok) { std::printf("00bf3860_TEST FAIL\n"); return 1; }
    std::printf("00bf3860_TEST PASS\n");
    return 0;
}