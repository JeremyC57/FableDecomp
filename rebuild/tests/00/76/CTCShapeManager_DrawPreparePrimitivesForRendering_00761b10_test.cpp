#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
class CAIGameCameraBase { public: C3DVector m_pos; virtual ~CAIGameCameraBase() { } virtual C3DVector GetPosition() const { return m_pos; } };
class CWorld { public: CAIGameCameraBase* PCurrentGameCamera; };
class CEnginePrimitivePrepareInfo { public: int m_draws; float m_lastZ; };
static CWorld g_world;
class CTCShapeManager { public:
    const CWorld* GetWorld() const { return &g_world; }
    void DrawShapes(CEnginePrimitivePrepareInfo* info, const C3DVector* position) { ++info->m_draws; info->m_lastZ = position->Z; }
    void DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info)
    {
        const CWorld* world = GetWorld();
        if (world->PCurrentGameCamera) {
            C3DVector position = world->PCurrentGameCamera->GetPosition();
            DrawShapes(info, &position);
        }
    }
};
int main()
{
    CEnginePrimitivePrepareInfo info; info.m_draws = 0; info.m_lastZ = 0.0f;
    CTCShapeManager m; g_world.PCurrentGameCamera = 0;
    m.DrawPreparePrimitivesForRendering(&info);
    CAIGameCameraBase cam; cam.m_pos = C3DVector(1.0f, 2.0f, 3.0f); g_world.PCurrentGameCamera = &cam;
    m.DrawPreparePrimitivesForRendering(&info);
    if (info.m_draws != 1 || info.m_lastZ != 3.0f) { std::printf("00761b10_TEST FAIL\n"); return 1; }
    std::printf("00761b10_TEST PASS\n");
    return 0;
}