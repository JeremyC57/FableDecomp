// Behaviour test for CTCPhysicsStandard::SetPhysicsMeshBankIndex (0x00724630): a model of the two branches -- index 0 frees the
// mesh info, recomputes the radius, zeroes the offset and clears the navigation-layer bit; any other index (re)fills the info from
// the bank mesh (full scale, box centre rotated by the RH-set matrix and scaled by scale*0.01, sphere / circle radii scaled) and
// copies the centre into the offset; afterwards a thing that is in its mapwho is removed and re-placed.
#include <cstdio>
#include <cmath>
#include <cstdlib>
struct V3 { float X, Y, Z; };
struct Mesh { V3 Min, Max; float SphereRadius, CircleRadius; int navCount; int refs; };
struct Info { unsigned short BankIndex; float RenderSizeX; V3 ToCentre; float BoundingRadius, BoundingCircleRadius; };
struct Mapwho { bool In; int removes, places; };
struct Phys {
    Info* PPhysicsMeshInfo; float Radius; V3 Offset; float M[3][3]; bool NavigationLayers; float scale; Mesh* bank[4]; Mapwho mw;
    int radiusCalls;
    float CalculateRadius() { ++radiusCalls; return 5.0f; }
    void SetPhysicsMeshBankIndex(long index)
    {
        if (index == 0) {
            operator delete(PPhysicsMeshInfo);
            PPhysicsMeshInfo = 0;
            Radius = CalculateRadius();
            Offset.X = 0.0f; Offset.Y = 0.0f; Offset.Z = 0.0f;
            NavigationLayers = false;
        } else {
            if (!PPhysicsMeshInfo)
                PPhysicsMeshInfo = static_cast<Info*>(operator new(sizeof(Info)));
            PPhysicsMeshInfo->BankIndex = (unsigned short)index;
            PPhysicsMeshInfo->RenderSizeX = scale;
            const Mesh* mesh = bank[index];
            V3 c = { (mesh->Max.X + mesh->Min.X) * 0.5f, (mesh->Max.Y + mesh->Min.Y) * 0.5f, (mesh->Max.Z + mesh->Min.Z) * 0.5f };
            const float xx = c.X, yy = c.Y, zz = c.Z;
            c.X = xx * M[0][0] + yy * M[1][0] + zz * M[2][0];
            c.Y = xx * M[0][1] + yy * M[1][1] + zz * M[2][1];
            c.Z = xx * M[0][2] + yy * M[1][2] + zz * M[2][2];
            const float s = PPhysicsMeshInfo->RenderSizeX * 0.01f;
            c.X *= s; c.Y *= s; c.Z *= s;
            PPhysicsMeshInfo->ToCentre = c;
            PPhysicsMeshInfo->BoundingRadius = mesh->SphereRadius * PPhysicsMeshInfo->RenderSizeX * 0.01f;
            PPhysicsMeshInfo->BoundingCircleRadius = PPhysicsMeshInfo->RenderSizeX * mesh->CircleRadius * 0.01f;
            Offset = PPhysicsMeshInfo->ToCentre;
            Radius = CalculateRadius();
            NavigationLayers = mesh->navCount != 0;
        }
        if (mw.In) { ++mw.removes; ++mw.places; }
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Mesh m = { {-2.0f, 0.0f, 2.0f}, {4.0f, 6.0f, 8.0f}, 10.0f, 7.0f, 3, 1 };
    Phys p; p.PPhysicsMeshInfo = 0; p.Radius = 0; p.Offset.X = p.Offset.Y = p.Offset.Z = 9.0f; p.NavigationLayers = false;
    p.scale = 200.0f; p.bank[2] = &m; p.mw.In = true; p.mw.removes = p.mw.places = 0; p.radiusCalls = 0;
    // rotation: x -> y, y -> -x, z -> z
    p.M[0][0] = 0; p.M[0][1] = 1; p.M[0][2] = 0; p.M[1][0] = -1; p.M[1][1] = 0; p.M[1][2] = 0; p.M[2][0] = 0; p.M[2][1] = 0; p.M[2][2] = 1;
    p.SetPhysicsMeshBankIndex(2);
    // centre (1,3,5) -> rotated (-3,1,5) -> *2
    if (!p.PPhysicsMeshInfo || p.PPhysicsMeshInfo->BankIndex != 2 || !near(p.Offset.X, -6.0f) || !near(p.Offset.Y, 2.0f) ||
        !near(p.Offset.Z, 10.0f) || !near(p.PPhysicsMeshInfo->BoundingRadius, 20.0f) || !near(p.PPhysicsMeshInfo->BoundingCircleRadius, 14.0f) ||
        !p.NavigationLayers || p.Radius != 5.0f || p.mw.removes != 1 || p.mw.places != 1)
        { std::printf("00724630_TEST FAIL\n"); return 1; }
    p.mw.In = false;
    p.SetPhysicsMeshBankIndex(0);
    if (p.PPhysicsMeshInfo || p.Offset.X != 0.0f || p.Offset.Y != 0.0f || p.Offset.Z != 0.0f || p.NavigationLayers ||
        p.radiusCalls != 2 || p.mw.removes != 1)
        { std::printf("00724630_TEST FAIL\n"); return 1; }
    std::printf("00724630_TEST PASS\n");
    return 0;
}