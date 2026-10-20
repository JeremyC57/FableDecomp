#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#endif
#include <cstdio>
#include <cstring>
static int g_deletes; void __cdecl operator delete(void*) throw() { ++g_deletes; }   // test spy (never frees)

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct C3DMeshInfo;

struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

struct CCountedPointer {
    void* m_pObject;                                        // +0
    SCountBlock* m_pCount;                                  // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CBaseClass {                                         // vtable 0x01231710
    virtual ~CBaseClass();                                  // 0x0099A2E0
};

struct C3DMeshObject : CBaseClass {                         // vtable 0x01239860, 0x78 bytes
    const C3DMeshInfo* MeshInfo;                            // +0x4
    CCountedPointer    Mesh;                                // +0x8 CCountedPointer<const C3DMeshStats>
    unsigned char      _rest[0x78 - 0x10];                  // +0x10 PositionMatrix .. Scale
    C3DMeshObject(const C3DMeshInfo* info, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A36E70
    C3DMeshObject& operator=(const C3DMeshObject& other);   // 0x004C3380
    virtual ~C3DMeshObject();
};

C3DMeshObject::~C3DMeshObject()
{
}

struct CMeshDataBank {
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                           // +0x3C
};

#pragma pack(push, 2)
struct CEngineGraphic {                                     // 14 bytes
    unsigned long BankIndex;                                // +0x00
    float AnimStep;                                         // +0x04
    float RenderSizeX;                                      // +0x08
    bool AdditiveAlpha;                                     // +0x0C
    unsigned char Type;                                     // +0x0D
};
#pragma pack(pop)

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    unsigned char  _pad00[0x34];
    float          Scale;                                   // +0x34
    unsigned char  _pad38[0x44 - 0x38];
    CEngineGraphic MainGraphic;                             // +0x44
    CWorld* GetWorld() const;                               // 0x00686A80 (CTCBase)
    bool GetRenderMeshObject(C3DMeshObject& out, const C3DVector& position, const CRightHandedSet& orientation) const;
};

bool CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject& out, const C3DVector& position, const CRightHandedSet& orientation) const
{
    if (MainGraphic.Type != 5 && MainGraphic.Type != 4)
        return false;
    CMeshDataBank* bank = GetWorld()->PMainMeshBank;
    const C3DMeshInfo* meshInfo = bank->GetMeshInfo(MainGraphic.BankIndex);
    out = C3DMeshObject(meshInfo, position, orientation, MainGraphic.RenderSizeX * Scale * 0.01f);
    return true;
}

}

// ---- test stubs for the retail callees ----
struct C3DMeshInfo { int Id; };
static C3DMeshInfo g_infos[4]; static long g_lastIndex; static float g_lastScale; static const C3DVector* g_lastPos;
static const CRightHandedSet* g_lastOrient; static int g_baseDtors;
CBaseClass::~CBaseClass() { ++g_baseDtors; }
C3DMeshObject::C3DMeshObject(const C3DMeshInfo* info, const C3DVector& position, const CRightHandedSet& orientation, float scale)
{ MeshInfo = info; Mesh.m_pObject = 0; Mesh.m_pCount = 0; g_lastScale = scale; g_lastPos = &position; g_lastOrient = &orientation; }
C3DMeshObject& C3DMeshObject::operator=(const C3DMeshObject& o) { MeshInfo = o.MeshInfo; return *this; }
const C3DMeshInfo* CMeshDataBank::GetMeshInfo(long index) { g_lastIndex = index; return &g_infos[index]; }
static CMeshDataBank g_bank; static CWorld g_world;
CWorld* NGraphicAppearance::CTCGraphicAppearance::GetWorld() const { return &g_world; }

int main()
{
    int ok = 1;
    g_world.PMainMeshBank = &g_bank;
    NGraphicAppearance::CTCGraphicAppearance ga; ga.Scale = 2.0f;
    ga.MainGraphic.BankIndex = 3; ga.MainGraphic.RenderSizeX = 50.0f; ga.MainGraphic.Type = 5;
    C3DVector pos; pos.X = pos.Y = pos.Z = 0.0f;
    const CRightHandedSet& orient = *(const CRightHandedSet*)&pos;
    C3DMeshObject out(0, pos, orient, 0.0f);
    g_baseDtors = 0;
    if (!ga.GetRenderMeshObject(out, pos, orient)) ok = 0;
    if (out.MeshInfo != &g_infos[3] || g_lastIndex != 3 || g_lastScale != 50.0f * 2.0f * 0.01f) ok = 0;
    if (g_lastPos != &pos || g_lastOrient != &orient || g_baseDtors != 1 || g_deletes != 0) ok = 0;
    ga.MainGraphic.Type = 4; ga.MainGraphic.BankIndex = 1;
    if (!ga.GetRenderMeshObject(out, pos, orient) || out.MeshInfo != &g_infos[1]) ok = 0;
    ga.MainGraphic.Type = 3;
    out.MeshInfo = 0;
    if (ga.GetRenderMeshObject(out, pos, orient) || out.MeshInfo != 0) ok = 0;
    if (!ok) { std::printf("004bc750_TEST FAIL\n"); return 1; }
    std::printf("004bc750_TEST PASS\n");
    return 0;
}