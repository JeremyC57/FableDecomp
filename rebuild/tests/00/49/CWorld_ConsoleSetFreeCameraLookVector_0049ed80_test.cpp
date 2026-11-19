// Behaviour test for CWorld::ConsoleSetFreeCameraLookVector (0x0049ED80): a fixture models the world's player manager, the main player's
// free camera (SetView records forward and up) and the console argument vector, and runs the row's verbatim body over 14 look vectors --
// general, vertical (the (1,0,0) side fallback), near-vertical on both sides of DELTA_NOTIONAL_ZERO, negative and axis-aligned -- against
// an independent double-precision reference of side = (Y, -X, 0) / fallback, up = normalise(side x look).
#include <stdio.h>
#include <math.h>
static int failures;
static void check(bool ok, const char* what, int c) { if (!ok) { ++failures; printf("FAIL case %d: %s\n", c, what); } }
const float DELTA_NOTIONAL_ZERO = 1.0e-4f;
class C3DVector { public: float X, Y, Z;
    C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Initialise(float x, float y, float z) { X = x; Y = y; Z = z; }
    const C3DVector operator^(const C3DVector& vector) const { return C3DVector((Y*vector.Z-Z*vector.Y),(Z*vector.X-X*vector.Z),(X*vector.Y-Y*vector.X)); }
    C3DVector& Normalise() { float m = (float)sqrt(X * X + Y * Y + Z * Z); X /= m; Y /= m; Z /= m; return *this; } };
class CCamera { public: C3DVector Forward, Up; int calls;
    void SetView(const C3DVector& forward, const C3DVector& up) { Forward = forward; Up = up; ++calls; } };
class CPlayer { public: CCamera CurrentFreeCamera; CCamera& GetCurrentFreeCamera() { return CurrentFreeCamera; } };
class CPlayerManager { public: CPlayer Main; int calls; CPlayer& GetMainPlayer() { ++calls; return Main; } };
class CGenericVar { public: float ValFloat; float GetFloat() const { return ValFloat; } };
namespace std { template<class T> class vector { public: T items[3]; const T& operator[](unsigned int n) const { return items[n]; } }; }
class CWorld { public: CPlayerManager* PlayerManager;
    static void ConsoleSetFreeCameraLookVector(CWorld& owner, const std::vector<CGenericVar>& params); };
void CWorld::ConsoleSetFreeCameraLookVector(CWorld& owner, const std::vector<CGenericVar>& params)
{
    CCamera& camera = owner.PlayerManager->GetMainPlayer().GetCurrentFreeCamera();
    C3DVector look_vect(params[0].GetFloat(), params[1].GetFloat(), params[2].GetFloat());
    C3DVector side_vector(look_vect.Y, -look_vect.X, 0.0f);
    if (side_vector.GetMagnitude() < DELTA_NOTIONAL_ZERO)
        side_vector.Initialise(1.0f, 0.0f, 0.0f);
    C3DVector up_vect = side_vector ^ look_vect;
    up_vect.Normalise();
    camera.SetView(look_vect, up_vect);
}
static bool near(double a, double b) { return fabs(a - b) <= 1e-4; }
int main()
{
    static const float looks[14][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, -1}, {3, 4, 5}, {-2, 7, 0.5f}, {0.6f, -0.8f, 0},
        {0.00005f, 0.00005f, 1}, {0.0002f, 0, 1}, {0, -0.0002f, -1}, {-1, -1, -1}, {10, -20, 30}, {0.25f, 0.5f, -2}, {-0.00007f, 0, 2} };
    for (int c = 0; c < 14; ++c) {
        CPlayerManager pm; pm.calls = 0; pm.Main.CurrentFreeCamera.calls = 0;
        CWorld world; world.PlayerManager = &pm;
        std::vector<CGenericVar> params; for (int i = 0; i < 3; ++i) params.items[i].ValFloat = looks[c][i];
        CWorld::ConsoleSetFreeCameraLookVector(world, params);
        double lx = looks[c][0], ly = looks[c][1], lz = looks[c][2];
        double sx = ly, sy = -lx, sz = 0.0;
        if (sqrt(sx * sx + sy * sy) < 1.0e-4) { sx = 1; sy = 0; }
        double ux = sy * lz - sz * ly, uy = sz * lx - sx * lz, uz = sx * ly - sy * lx;
        double m = sqrt(ux * ux + uy * uy + uz * uz); ux /= m; uy /= m; uz /= m;
        const CCamera& cam = pm.Main.CurrentFreeCamera;
        check(pm.calls == 1 && cam.calls == 1, "calls", c);
        check(cam.Forward.X == looks[c][0] && cam.Forward.Y == looks[c][1] && cam.Forward.Z == looks[c][2], "forward = look", c);
        check(near(cam.Up.X, ux) && near(cam.Up.Y, uy) && near(cam.Up.Z, uz), "up", c);
    }
    if (failures) { printf("%d failures\n", failures); return 1; }
    puts("0049ed80_TEST PASS");
    return 0;
}