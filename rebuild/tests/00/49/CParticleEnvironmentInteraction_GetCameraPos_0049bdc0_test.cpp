#include <cstdio>
// Behaviour of CParticleEnvironmentInteraction::GetCameraPos (0x0049BDC0): the world's LOD centre when a world is
// attached, else the stored (byte-packed) CameraPos.
struct C3DVector { float X, Y, Z; };
struct CWorld {
    C3DVector m_centre;
    virtual C3DVector GetGameLODCentre() const { return m_centre; }
    virtual ~CWorld() {}
};
#pragma pack(push, 1)
struct CParticleEnvironmentInteraction {
    void* m_vtable; CWorld* PWorld; C3DVector WindVelocity; bool WindActive; C3DVector CameraPos;
    void GetCameraPos(C3DVector& pos) const
    {
        if (PWorld)
            pos = PWorld->GetGameLODCentre();
        else
            pos = CameraPos;
    }
};
#pragma pack(pop)
int main()
{
    CWorld w; w.m_centre.X = 1.0f; w.m_centre.Y = 2.0f; w.m_centre.Z = 3.0f;
    CParticleEnvironmentInteraction e; e.WindActive = false;
    C3DVector stored; stored.X = 7.0f; stored.Y = 8.0f; stored.Z = 9.0f; e.CameraPos = stored;
    C3DVector p;
    e.PWorld = &w; e.GetCameraPos(p);
    if (p.X != 1.0f || p.Y != 2.0f || p.Z != 3.0f) { std::printf("0049bdc0_TEST FAIL\n"); return 1; }
    e.PWorld = 0; e.GetCameraPos(p);
    if (p.X != 7.0f || p.Y != 8.0f || p.Z != 9.0f) { std::printf("0049bdc0_TEST FAIL\n"); return 1; }
    std::printf("0049bdc0_TEST PASS\n");
    return 0;
}