#include <cstdio>
#include <cstdlib>

static long g_framesPerSecond = 30;
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static C3DVector g_pos(9, 9, 9), g_up(9, 9, 9), g_look(9, 9, 9); static float g_fov, g_zoom;
class CCamera { public:
    float WorldPosition[3]; float RHSet[6]; bool HeightLocked; bool ViewVecZLocked;
    unsigned long FOVFlags; float HorizontalFOV; float VerticalFOV; float Zoom;
    CCamera() : HeightLocked(false), ViewVecZLocked(false) {}
    CCamera(const C3DVector& position, const C3DVector& up, const C3DVector& lookAt, float fov, float zoom)
    { g_pos = position; g_up = up; g_look = lookAt; g_fov = fov; g_zoom = zoom; HeightLocked = true; ViewVecZLocked = true; }
};
template <class T> struct CList {
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* m_node;
    CList() : m_node(0)
    {
        Node* node = (Node*)std::malloc(sizeof(Node));
        node->Next = node; node->Prev = node; m_node = node;
    }
};
class CWorld { public: int id; };
class CAIGameCameraBase { public: virtual ~CAIGameCameraBase() {} };
struct CGameCameraBase : CAIGameCameraBase {
    CCamera InternalCam; CCamera LastInternalCam; float LastPointingAt[3]; float PointingAt[3]; CWorld* World;
    float Theta, Phi, Rad, ThetaFixed, RadFixed; bool ActAsFixedCam, GoingIntoBuilding;
    CList<C3DVector> DrawCoordList, LocalThingPosList; long TriggerFrame; unsigned long long BuildingUID; float PhiChanger;
    CGameCameraBase(CWorld& world)
        : InternalCam(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 1.0f, 1.0f), C3DVector(0.0f, 0.0f, 1.0f), 0.19444f, 0.0f),
          World(&world), Theta(0.2f), Phi(1.25f), Rad(10.0f), ThetaFixed(0.2f), RadFixed(10.0f),
          ActAsFixedCam(false), GoingIntoBuilding(false),
          TriggerFrame((long)(g_framesPerSecond * 1.5f)), BuildingUID(0), PhiChanger(0.1f) {}
};
int main()
{
    CWorld world;
    CGameCameraBase cam(world);
    if (g_pos.X != 0 || g_pos.Z != 0 || g_up.X != 1 || g_up.Y != 1 || g_up.Z != 1 || g_look.Z != 1 || g_look.X != 0 ||
        g_fov != 0.19444f || g_zoom != 0.0f || !cam.InternalCam.HeightLocked || cam.LastInternalCam.HeightLocked ||
        cam.LastInternalCam.ViewVecZLocked || cam.World != &world || cam.Theta != 0.2f || cam.Phi != 1.25f || cam.Rad != 10.0f ||
        cam.ThetaFixed != 0.2f || cam.RadFixed != 10.0f || cam.ActAsFixedCam || cam.GoingIntoBuilding ||
        cam.DrawCoordList.m_node == 0 || cam.DrawCoordList.m_node->Next != cam.DrawCoordList.m_node ||
        cam.LocalThingPosList.m_node->Prev != cam.LocalThingPosList.m_node || cam.DrawCoordList.m_node == cam.LocalThingPosList.m_node ||
        cam.TriggerFrame != 45 || cam.BuildingUID != 0 || cam.PhiChanger != 0.1f) {
        std::printf("006fd8c0_TEST FAIL\n"); return 1;
    }
    std::free(cam.DrawCoordList.m_node); std::free(cam.LocalThingPosList.m_node);
    std::printf("006fd8c0_TEST PASS\n");
    return 0;
}