// behaviour test for 00bdd6b0 (standalone copy of the model)
#include <cstdio>

class C3DMeshInfo;
class CBaseClass { public: CBaseClass() {} virtual ~CBaseClass() {} };
class CBaseClassNonCopyable : public CBaseClass { protected: CBaseClassNonCopyable() {} virtual ~CBaseClassNonCopyable() {} };
class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable { public:
    CLocalDetailObjectCollectionType() : Mesh(0), ZSpriteMesh(0), ShadowMesh(0) {}
    C3DMeshInfo* Mesh; C3DMeshInfo* ZSpriteMesh; C3DMeshInfo* ShadowMesh; long MeshPolyCount; };

int main()
{
    CLocalDetailObjectCollectionType t;
    if (!(t.Mesh == 0 && t.ZSpriteMesh == 0 && t.ShadowMesh == 0)) { std::printf("00bdd6b0_TEST FAIL\n"); return 1; }
    std::printf("00bdd6b0_TEST PASS\n");
    return 0;
}