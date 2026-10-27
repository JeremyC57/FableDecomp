// Behaviour test for 0x004BC670 NGraphicAppearance::CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject&, float): the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0c
};
class C3DMeshInfo;

class CBaseClass {
public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class C3DMeshObject : public CBaseClass {                   // vtable 0x01239860, 0x78 bytes
public:
    unsigned char _body[0x78 - 0x4];
    C3DMeshObject(const C3DMeshInfo* info, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A36E70
    C3DMeshObject& operator=(const C3DMeshObject& other);   // 0x004C3380
    virtual ~C3DMeshObject();                               // 0x004C3200
};

class CMeshDataBank {
public:
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
class CWorld {
public:
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                           // +0x3C
};

#define VSLOT(n) virtual void vslot##n();
#define VSLOT10(n) VSLOT(n##0) VSLOT(n##1) VSLOT(n##2) VSLOT(n##3) VSLOT(n##4) VSLOT(n##5) VSLOT(n##6) VSLOT(n##7) VSLOT(n##8) VSLOT(n##9)
class CTCPhysicsBase {
public:
    VSLOT10(0) VSLOT10(1) VSLOT10(2) VSLOT10(3) VSLOT10(4) VSLOT10(5) VSLOT10(6)
    VSLOT(70)
    virtual const CRightHandedSet& GetRHSet() const;              // slot 72 (+0x120)
    virtual CRightHandedSet GetRHSet(float interpolation) const;   // slot 71 (+0x11C)
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60 (retail; PDB +0x70)
};

#pragma pack(push, 2)
class CEngineGraphic {                                      // 14 bytes
public:
    unsigned long BankIndex;                                // +0x00
    float AnimStep;                                         // +0x04
    float RenderSizeX;                                      // +0x08
    bool AdditiveAlpha;                                     // +0x0C
    unsigned char Type;                                     // +0x0D
};
#pragma pack(pop)

class CTCBase {
public:
    void*   __vfptr;                                        // +0x00
    CThing* ParentThing;                                    // +0x04
protected:
    const CWorld& PeekWorld() const;                        // 0x00686A80
};

namespace NGraphicAppearance {
C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

class CTCGraphicAppearance : public CTCBase {
public:
    unsigned char  _pad08[0x34 - 0x08];
    float          Scale;                                   // +0x34
    unsigned char  _pad38[0x44 - 0x38];
    CEngineGraphic MainGraphic;                             // +0x44
    bool GetRenderMeshObject(C3DMeshObject& out, float interpolation) const;
};

bool CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject& out, float interpolation) const
{
    if (MainGraphic.Type != 0 && (MainGraphic.Type == 5 || MainGraphic.Type == 4)) {
        CMeshDataBank* bank = PeekWorld().PMainMeshBank;
        const CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        C3DVector position = GetRenderPositionForThing(*ParentThing, interpolation);
        const C3DMeshInfo* meshInfo = bank->GetMeshInfo(MainGraphic.BankIndex);
        if (interpolation == 1.0f)
            out = C3DMeshObject(meshInfo, position, physics->GetRHSet(), MainGraphic.RenderSizeX * Scale * 0.01f);
        else
            out = C3DMeshObject(meshInfo, position, physics->GetRHSet(interpolation), MainGraphic.RenderSizeX * Scale * 0.01f);
        return true;
    }
    return false;
}

}

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
#define DEFV(n) void CTCPhysicsBase::vslot##n() { }
#define DEFV10(n) DEFV(n##0) DEFV(n##1) DEFV(n##2) DEFV(n##3) DEFV(n##4) DEFV(n##5) DEFV(n##6) DEFV(n##7) DEFV(n##8) DEFV(n##9)
DEFV10(0) DEFV10(1) DEFV10(2) DEFV10(3) DEFV10(4) DEFV10(5) DEFV10(6) DEFV(70)
static CRightHandedSet g_current, g_interp;
const CRightHandedSet& CTCPhysicsBase::GetRHSet() const { Log("GetRHSet()"); return g_current; }
CRightHandedSet CTCPhysicsBase::GetRHSet(float t) const { char b[32]; sprintf(b, "GetRHSet(%g)", t); Log(b); return g_interp; }
CBaseClass::~CBaseClass() { }
struct MeshRecord { const C3DMeshInfo* info; float x; const CRightHandedSet* orient; float scale; };
static MeshRecord g_last;
C3DMeshObject::C3DMeshObject(const C3DMeshInfo* info, const C3DVector& position, const CRightHandedSet& orientation, float scale)
{
    MeshRecord r; r.info = info; r.x = position.X; r.orient = &orientation; r.scale = scale; memcpy(_body, &r, sizeof(r));
    r.orient = (&orientation == &g_current) ? &g_current : &g_interp; memcpy(_body, &r, sizeof(r));
}
C3DMeshObject& C3DMeshObject::operator=(const C3DMeshObject& other) { memcpy(_body, other._body, sizeof(_body)); Log("assign"); return *this; }
C3DMeshObject::~C3DMeshObject() { }
static C3DMeshInfo* const g_info = (C3DMeshInfo*)0x1234;
const C3DMeshInfo* CMeshDataBank::GetMeshInfo(long index) { char b[32]; sprintf(b, "GetMeshInfo(%ld)", index); Log(b); return g_info; }
static CWorld g_world;
const CWorld& CTCBase::PeekWorld() const { return g_world; }
C3DVector __fastcall NGraphicAppearance::GetRenderPositionForThing(const CThing&, float t)
{
    char b[32]; sprintf(b, "GetPos(%g)", t); Log(b); C3DVector v; v.X = 5.0f; v.Y = 6.0f; v.Z = 7.0f; return v;
}
int main()
{
    CMeshDataBank bank; g_world.PMainMeshBank = &bank;
    CTCPhysicsBase physics; CThing thing; thing.PhysicsTC = &physics;
    NGraphicAppearance::CTCGraphicAppearance* g = (NGraphicAppearance::CTCGraphicAppearance*)calloc(1, sizeof(NGraphicAppearance::CTCGraphicAppearance));
    g->ParentThing = &thing; g->Scale = 2.0f; g->MainGraphic.BankIndex = 42; g->MainGraphic.RenderSizeX = 50.0f;
    C3DMeshObject* out = (C3DMeshObject*)calloc(1, sizeof(C3DMeshObject));
    int ok = 1; MeshRecord r;
    g->MainGraphic.Type = 0; ok &= !g->GetRenderMeshObject(*out, 1.0f) && g_log[0] == 0;
    g->MainGraphic.Type = 3; ok &= !g->GetRenderMeshObject(*out, 1.0f) && g_log[0] == 0;
    g->MainGraphic.Type = 4; ok &= g->GetRenderMeshObject(*out, 1.0f);
    memcpy(&r, out->_body, sizeof(r));
    ok &= strcmp(g_log, "GetPos(1);GetMeshInfo(42);GetRHSet();assign;") == 0 && r.info == g_info && r.x == 5.0f && r.orient == &g_current && r.scale == 1.0f;
    g_log[0] = 0;
    g->MainGraphic.Type = 5; ok &= g->GetRenderMeshObject(*out, 0.5f);
    memcpy(&r, out->_body, sizeof(r));
    ok &= strcmp(g_log, "GetPos(0.5);GetMeshInfo(42);GetRHSet(0.5);assign;") == 0 && r.orient == &g_interp;
    if (!ok) printf("log %s\n", g_log);
    if (ok) printf("004bc670_TEST PASS\n");
    return ok ? 0 : 1;
}