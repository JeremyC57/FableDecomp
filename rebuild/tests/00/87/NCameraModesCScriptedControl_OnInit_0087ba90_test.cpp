// Behaviour test for NCameraModes::CScriptedControl::OnInit (0x0087BA90), g++ model of the retail logic.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct Camera { V Pos; V Target; float Roll, Fov; int Id; };
struct Module { void* Owner; void* Manager; int Inits; };
struct Manager { Camera Cam, Old; bool Cut; };
struct Control { Camera Init; Module* M; Manager* Mgr; V LookAt, GoTo; float RollAngle, Fov, Height, MeshHeight; };
static void OnInit(Control& c, Camera& cam)
{
    c.Init = cam;
    c.M->Owner = &c; c.M->Manager = c.Mgr; ++c.M->Inits;
    float roll = c.RollAngle, fov = c.Fov;
    V view = { c.LookAt.X - c.GoTo.X, c.LookAt.Y - c.GoTo.Y, c.LookAt.Z - c.GoTo.Z };
    cam.Pos = c.GoTo;
    V dir = view;
    if (view.X * view.X + view.Y * view.Y + view.Z * view.Z > 1e-8f) {
        float l = std::sqrt(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z);
        dir.X /= l; dir.Y /= l; dir.Z /= l;
        V t = { dir.X + c.GoTo.X, dir.Y + c.GoTo.Y, dir.Z + c.GoTo.Z }; cam.Target = t;
        cam.Roll = roll;
    }
    cam.Fov = fov;
    c.Mgr->Old = c.Mgr->Cam; c.Mgr->Cut = true;
    c.Height = c.MeshHeight;
}
int main()
{
    Module m = { 0, 0, 0 }; Manager mg; mg.Cut = false; mg.Cam.Id = 3; mg.Old.Id = 0;
    Camera cam = { { 0, 0, 0 }, { 0, 0, 0 }, 0, 0, 9 };
    Control c; c.M = &m; c.Mgr = &mg; c.LookAt.X = 1; c.LookAt.Y = 2; c.LookAt.Z = 7; c.GoTo.X = 1; c.GoTo.Y = 2; c.GoTo.Z = 3;
    c.RollAngle = 0.5f; c.Fov = 60.0f; c.MeshHeight = 1.8f;
    OnInit(c, cam);
    int ok = c.Init.Id == 9 && m.Owner == &c && m.Manager == &mg && m.Inits == 1 && cam.Pos.Z == 3 && std::fabs(cam.Target.Z - 4.0f) < 1e-5f
          && cam.Roll == 0.5f && cam.Fov == 60.0f && mg.Cut && mg.Old.Id == 3 && c.Height == 1.8f;
    std::printf(ok ? "0087ba90_TEST PASS\n" : "0087ba90_TEST FAIL\n");
    return ok ? 0 : 1;
}