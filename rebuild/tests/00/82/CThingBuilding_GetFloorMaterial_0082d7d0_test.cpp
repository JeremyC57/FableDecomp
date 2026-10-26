// Behaviour test for 0082d7d0: src/0082d7d0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingBuilding::GetFloorMaterial()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingBuilding::GetFloorMaterial() -- the building def's floor material (virtual slot 40)
// Leaf: ?GetFloorMaterial@CThingBuilding
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D7D0, 64 bytes.
// Callees: CThing::GetDefinitionManager @ 0x004C7990 (propagated label "CViewBase::GetDisplayEngine"),
//          CDefinitionManager::GetDef<CThingBuildingDef> @ 0x004AB840 (propagated "<CThingPhysicalSwitchDef>"), the def's
//          Destroy (vtable slot 1) on the last release.
// Layout: CThing::DefGlobalIndex (unsigned short) at retail +0x8c; PDB CThingBuildingDef has WallMaterial +0x8c,
//   FloorMaterial +0x90, ReverbEnvironmentType +0x94, ReverbEnvironmentLevel +0x98, the flag bits at +0xa4: retail
//   reads all of them 4 bytes lower (one STL container narrower in a base).
// Shape: an inline GetBuildingDef() builds a CDefPointer local (zeroed after the argument pushes), fills it through
//   the manager and returns the RAW object pointer, so the local's release (`dec [esi+4] ; jne ; call [eax+4]`, the
//   counted release living on the def object) runs BEFORE the field read (the landed wave-24m rule).
// Identity: CThingBuilding vtable 0x0124509C slot 40 (rtti-vtables.tsv), returns CThingBuildingDef::FloorMaterial.
class CDefPointeeBase { public:                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

class CThingBuildingDef : public CDefPointeeBase { public:    // retail offsets = PDB - 4
    unsigned char _pad08[0x80];
    long WallMaterial;                  // +0x88 (PDB +0x8c)
    long FloorMaterial;                 // +0x8c (PDB +0x90)
    long ReverbEnvironmentType;         // +0x90 EReverbEnvironmentType (PDB +0x94)
    long ReverbEnvironmentLevel;        // +0x94 float, moved as a dword (PDB +0x98)
    long OutsideOcclusionLevel;         // +0x98 float
    long CameraManagerSetIndex;         // +0x9c
    unsigned char UseHighDetailQuadTree : 1;    // +0xa0 bit 0 (PDB +0xa4)
    unsigned char IsSelfIlluminating : 1;
    unsigned char IsPartOfHeroGuild : 1;
    unsigned char HasWeatherMask : 1;
};

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template<class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};

class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out);   // 0x004AB840 (GetDef<CThingBuildingDef>)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingBuilding { public:
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;      // +0x8c (CThing; PDB +0x9c)
    CDefinitionManager* GetDefinitionManager();     // 0x004C7990 (CThing)
    const CThingBuildingDef* GetBuildingDef()
    {
        CDefPointer<CThingBuildingDef> out;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &out);
        CDefHolder<CThingBuildingDef> def(out.Object);
        return def.Object;
    }
    long GetFloorMaterial();
};

long CThingBuilding::GetFloorMaterial()
{
    return GetBuildingDef()->FloorMaterial;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0, g_get = 0;
void* FindDef_CDefinitionManager_GetDef_0() { ++g_get; return g_def; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static CDefinitionManager g_mgr;
CDefinitionManager* CThingBuilding::GetDefinitionManager() { return &g_mgr; }
int main()
{
    static CThingBuildingDef def; def.m_refCount = 2; def.WallMaterial = 11; def.FloorMaterial = 22; g_def = &def;
    CThingBuilding b; memset(&b, 0, sizeof(b)); b.DefGlobalIndex = 0x99;
    long expect = def.FloorMaterial;
    CHECK(b.GetFloorMaterial() == expect); CHECK(g_get == 1); CHECK(def.m_refCount == 1); CHECK(g_destroyed == 0);
    def.FloorMaterial = 33; CHECK(b.GetFloorMaterial() == 33); CHECK(def.m_refCount == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("0082d7d0_TEST PASS\n"); else printf("0082d7d0_TEST FAIL\n");
    return fails;
}