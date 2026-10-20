// Behaviour test for 006c5630: src/006c5630.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. COpinionDeedDefTable::GetDeedDef(long deedType)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// COpinionDeedDefTable::GetDeedDef(long deedType) -- the def registered for a deed type, fetched from the definition manager through the owner thing and returned as a counted pointer
// Leaf: GetDeedDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5630, 78 bytes (+ int3 padding).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0 (`mov ecx,[ecx+4] ; jmp CThing::GetDefinitionManager`: the owner
//          thing at +4), CDefinitionManager::GetDef<...> @ 0x006C76B0, the def's Destroy (vtable slot 1).
// Identity: a component (owner thing at +4) holding a table of unsigned-short def indices at +0x2e, reached through
//   interface type 0x51 by CTCHeroStats::UpdateOutstandingCrimes (0x0057DA68) and called from CTCVillage,
//   CTCCreatureOpinionOfHero and CTCHeroOpinionDeedLog deed code (44 call sites); it sits just before
//   CTCHeroOpinionDeedLog's methods. The class is not resolved: the name is descriptive. The manifest label
//   "CThingBuilding::GetFloorMaterial" is propagated.
// Shape: as 0x0083C1E0 -- a CDefPointer local filled by the manager, returned by value through the hidden pointer
//   (`ret 8`: hidden result + the deed type).
struct CDefPointeeBase {                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
};

struct COpinionDeedTypeDef : CDefPointeeBase { };

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};

struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<COpinionDeedTypeDef>* out);   // 0x006C76B0
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<COpinionDeedTypeDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct COpinionDeedDefTable {
    unsigned char _pad00[0x2e];
    unsigned short DeedDefIndices[1];   // +0x2e, indexed by deed type
    CDefinitionManager* GetDefinitionManager();     // 0x006869D0 (CTCBase: through the owner thing)
    CDefPointer<COpinionDeedTypeDef> GetDeedDef(long deedType);
};

CDefPointer<COpinionDeedTypeDef> COpinionDeedDefTable::GetDeedDef(long deedType)
{
    CDefPointer<COpinionDeedTypeDef> def;
    GetDefinitionManager()->GetDef(DeedDefIndices[deedType], &def);
    return def;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0;
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static CDefinitionManager g_mgr;
CDefinitionManager* COpinionDeedDefTable::GetDefinitionManager() { return &g_mgr; }
int main()
{
    COpinionDeedTypeDef d; d.m_refCount = 1;
    unsigned char raw[0x40]; memset(raw, 0, sizeof(raw));
    COpinionDeedDefTable* t = (COpinionDeedDefTable*)raw;
    g_def = &d;
    {
        CDefPointer<COpinionDeedTypeDef> r = t->GetDeedDef(3);
        CHECK(r.Object == &d); CHECK(d.m_refCount == 1); CHECK(g_destroyed == 0);
    }
    CHECK(d.m_refCount == 0); CHECK(g_destroyed == 1);
    g_def = 0;
    {
        CDefPointer<COpinionDeedTypeDef> r = t->GetDeedDef(1);
        CHECK(r.Object == 0);
    }
    CHECK(g_destroyed == 1);
    if (fails == 0) printf("006c5630_TEST PASS\n"); else printf("006c5630_TEST FAIL\n");
    return fails;
}