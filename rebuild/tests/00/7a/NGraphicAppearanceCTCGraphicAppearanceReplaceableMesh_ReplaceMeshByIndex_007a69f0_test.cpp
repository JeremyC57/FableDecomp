#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#endif
#include <cstdio>
#include <cstring>
class CTCBase;
class CThing;

class CBaseClass {                                  // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                   // 0x0099A2D0
    ~CBaseClass();                                  // 0x0099A2E0
};
namespace NTCMessages {
class CMessageBase : public CBaseClass {
public:
    virtual void v000();
};
class CMessageOnMeshReplaced : public CMessageBase {   // vtable 0x0126CB54
public:
    virtual void v000();
};
}

#pragma pack(push, 2)
class CEngineGraphic {                              // 14 bytes
public:
    unsigned long BankIndex;                        // +0x00
    float AnimStep;                                 // +0x04
    float RenderSizeX;                              // +0x08
    bool AdditiveAlpha;                             // +0x0C
    unsigned char Type;                             // +0x0D
};
#pragma pack(pop)

class CReplaceableMeshDef {
public:
    unsigned char _pad00[0x28];
    CEngineGraphic* Meshes;                         // +0x28 CReplaceableMeshes (first word: the graphic array)
};

class CTCPhysicsBase {                              // interface 2
public:
    void CreatePPhysicsMeshInfo(const CEngineGraphic& graphic);   // 0x006B08E0
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x44];
    CEngineGraphic MainGraphic;                     // +0x44
    void SetAsChanged(unsigned long reasons);       // 0x004BF9E0
};
}

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    void SendMessageW(const NTCMessages::CMessageBase& message);       // 0x004C7820
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCPhysicsBase* GetPhysicsTC()
    {
        CTCPhysicsBase* physics;
        if (HasTC(2))
            physics = (CTCPhysicsBase*)GetTC(2);
        return physics;
    }
};

namespace NGraphicAppearance {
class CTCGraphicAppearanceReplaceableMesh {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool Replaced;                                  // +0x10
    unsigned long MeshIndex;                        // +0x14
    void* PrimHandle;                               // +0x18 CPrimitive*
    const CReplaceableMeshDef* PDef;                // +0x1C CDefPointer<const CReplaceableMeshDef>::Object
    void ReplaceMeshByIndex(unsigned long index);
};

void CTCGraphicAppearanceReplaceableMesh::ReplaceMeshByIndex(unsigned long index)
{
    MeshIndex = index;
    Replaced = true;
    const CEngineGraphic& mesh = PDef->Meshes[index];
    ParentThing->PTCGraphicAppearance->MainGraphic = mesh;
    ParentThing->PTCGraphicAppearance->SetAsChanged(4);
    ParentThing->GetPhysicsTC()->CreatePPhysicsMeshInfo(mesh);
    ParentThing->SendMessageW(NTCMessages::CMessageOnMeshReplaced());
}
}

// ---- test stubs for the retail callees ----
static unsigned long g_changed; static const CEngineGraphic* g_physicsMesh; static int g_messages, g_ctors, g_dtors;
CBaseClass::CBaseClass() { ++g_ctors; }
CBaseClass::~CBaseClass() { ++g_dtors; }
void CBaseClass::v000() {}
void NTCMessages::CMessageBase::v000() {}
void NTCMessages::CMessageOnMeshReplaced::v000() {}
void CTCPhysicsBase::CreatePPhysicsMeshInfo(const CEngineGraphic& g) { g_physicsMesh = &g; }
void NGraphicAppearance::CTCGraphicAppearance::SetAsChanged(unsigned long r) { g_changed = r; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CThing::SendMessageW(const NTCMessages::CMessageBase&) { ++g_messages; }
void NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::v00() {}

int main()
{
    int ok = 1;
    CEngineGraphic meshes[3];
    for (int i = 0; i < 3; ++i) { meshes[i].BankIndex = 100 + i; meshes[i].AnimStep = 0.5f * i; meshes[i].RenderSizeX = 2.0f;
                                  meshes[i].AdditiveAlpha = i == 2; meshes[i].Type = (unsigned char)(7 + i); }
    CReplaceableMeshDef def; def.Meshes = meshes;
    NGraphicAppearance::CTCGraphicAppearance appearance; std::memset(&appearance, 0, sizeof(appearance));
    CTCPhysicsBase physics;
    CInterfaceMap::SEntry e; e.type = 2; e.component = (CTCBase*)&physics;
    CThing owner; std::memset(&owner, 0, sizeof(owner));
    owner.m_interfaces.first = &e; owner.m_interfaces.last = &e + 1; owner.m_presentBits[0] = 1u << 2;
    owner.PTCGraphicAppearance = &appearance;
    NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh rm; rm.ParentThing = &owner; rm.PDef = &def; rm.Replaced = false; rm.MeshIndex = 0;
    rm.ReplaceMeshByIndex(2);
    if (!rm.Replaced || rm.MeshIndex != 2 || appearance.MainGraphic.BankIndex != 102 || appearance.MainGraphic.AnimStep != 1.0f) ok = 0;
    if (!appearance.MainGraphic.AdditiveAlpha || appearance.MainGraphic.Type != 9 || appearance.MainGraphic.RenderSizeX != 2.0f) ok = 0;
    if (g_changed != 4 || g_physicsMesh != &meshes[2] || g_messages != 1 || g_ctors != 1 || g_dtors != 1) ok = 0;
    if (!ok) { std::printf("007a69f0_TEST FAIL\n"); return 1; }
    std::printf("007a69f0_TEST PASS\n");
    return 0;
}