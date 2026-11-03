// Behaviour test for 0x00BC14E0 CEnginePrimitiveManagerParticleSpriteGroup ctor: three 5-entry point-light shader tables (entry 0
// the plain dirlight shader, 1..3 the 3-point-light shader, 4 the 5-point-light shader), three displacement vertex shaders and three
// pixel shaders fetched by name with reference counting (each fetched shader keeps exactly the member's reference once the
// by-value temporary is gone), then BuildBuffers runs once.
#include <cstdio>
#include <cstring>
#include <vector>
static int gBuilt = 0, gDeleted = 0;
struct CData { unsigned long ReferenceCount; const char* Name; virtual ~CData() { ++gDeleted; } };
struct CShader {
    CData* Data;
    CShader() : Data(0) { }
    CShader(const CShader& o) : Data(o.Data) { if (Data) ++Data->ReferenceCount; }
    const CShader& operator=(const CShader& o) {
        CData* d = Data; if (d) { if (--d->ReferenceCount == 0) delete d; }
        Data = o.Data; ++Data->ReferenceCount; return *this;
    }
    ~CShader() { if (Data) { --Data->ReferenceCount; if (Data->ReferenceCount == 0) delete Data; } }
};
static std::vector<CData*> gAll;
class CShaderResource { public:
    CShader Get(const char* name) const {
        CData* d = new CData; d->ReferenceCount = 0; d->Name = name; gAll.push_back(d);
        CShader s; s.Data = d; ++d->ReferenceCount; return s;
    }
};
static CShaderResource gV, gP;
struct CGroup {
    std::vector<CShader> Billboard, Crossed, FaceMe;
    CShader BillboardDisp, CrossedDisp, FaceMeDisp, PS, PSStipple, PSDisp;
    void BuildBuffers() { ++gBuilt; }
    CGroup() {
        FaceMe.resize(5);
        FaceMe[0] = gV.Get("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_FOG");
        for (int i = 1; i < 4; ++i) FaceMe[i] = gV.Get("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_3POINTLIGHTS_FOG");
        FaceMe[4] = gV.Get("VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_5POINTLIGHTS_FOG");
        Crossed.resize(5);
        Crossed[0] = gV.Get("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_FOG");
        for (int i = 1; i < 4; ++i) Crossed[i] = gV.Get("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_3POINTLIGHTS_FOG");
        Crossed[4] = gV.Get("VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_5POINTLIGHTS_FOG");
        Billboard.resize(5);
        Billboard[0] = gV.Get("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_FOG");
        for (int i = 1; i < 4; ++i) Billboard[i] = gV.Get("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_3POINTLIGHTS_FOG");
        Billboard[4] = gV.Get("VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_5POINTLIGHTS_FOG");
        BillboardDisp = gV.Get("VSHADER_SPRITE_GROUP_BILLBOARD_DISPLACEMENT");
        CrossedDisp = gV.Get("VSHADER_SPRITE_GROUP_CROSSED_DISPLACEMENT");
        FaceMeDisp = gV.Get("VSHADER_SPRITE_GROUP_3D_FACE_ME_DISPLACEMENT");
        PS = gP.Get("PSHADER_SPRITE_GROUP");
        PSStipple = gP.Get("PSHADER_SPRITE_GROUP_STIPPLE");
        PSDisp = gP.Get("PSHADER_SPRITE_GROUP_DISPLACEMENT");
        BuildBuffers();
    }
};
int main()
{
    CGroup* g = new CGroup;
    bool ok = gBuilt == 1 && gAll.size() == 21 && gDeleted == 0;
    for (size_t i = 0; i < gAll.size(); ++i) ok = ok && gAll[i]->ReferenceCount == 1;
    ok = ok && std::strstr(g->Crossed[2].Data->Name, "CROSSED_DIRLIGHT_3POINTLIGHTS") != 0
            && std::strstr(g->Billboard[4].Data->Name, "BILLBOARD_DIRLIGHT_5POINTLIGHTS") != 0
            && std::strcmp(g->PSStipple.Data->Name, "PSHADER_SPRITE_GROUP_STIPPLE") == 0;
    delete g;
    ok = ok && gDeleted == 21;
    std::printf(ok ? "00bc14e0_TEST PASS\n" : "00bc14e0_TEST FAIL\n");
    return ok ? 0 : 1;
}