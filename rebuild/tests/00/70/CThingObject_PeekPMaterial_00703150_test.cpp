// Behaviour test for 00703150: src/00703150.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingObject::PeekPMaterial() const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingObject::PeekPMaterial() const -- returns the object's material def (looked up by the object def's material index), or a null def pointer when the def has none
// Leaf: ?PeekPMaterial@CThingObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703150, length 104 bytes (`ret 4`, + int3 padding). CThingObject vtable 0x01264DCC
// slot 58 (the catalog's "vfunc_58"; Ego_r.pdb has ?PeekPMaterial@CThingObject@@MBE?AV?$CDefPointer...).
// Callees: CThing::GetDefinitionManager() @ 0x004C7990, CDefinitionManager::GetDef<CMaterialDef> @ 0x0066C340 (manifest label names a
// propagated CThingPhysicalSwitchDef instantiation), the def's slot 1 (Destroy) on release.
// Layout (retail, local model): CThingObject PDef at retail +0xd0 (PDB +0xe8); the object def's material index at +0x84.
// Shape: returned by value through the hidden pointer; the hit arm builds a zeroed CDefPointer local, fills it through GetDef
// and returns it by copy (VC7.1 has no NRVO: the copy ctor's add-ref and the local's release merge into
// `inc ; mov eax,edx ; dec eax`), the miss arm returns a default-constructed (null) pointer.
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;               // slot 1
    mutable long m_refCount;                    // +0x04
};
struct CMaterialDef : CDefPointeeBase {};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<const CMaterialDef>* out);   // 0x0066C340
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<const CMaterialDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingObjectDef {
    unsigned char _pad00[0x84];
    long          MaterialIndex;                // +0x84
};
struct CThingObject {
    virtual void v00();
    unsigned char    _pad04[0xd0 - 0x04];
    CThingObjectDef* PDef;                      // +0xd0 (CDefPointer<const CThingObjectDef>)
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    CDefPointer<const CMaterialDef> PeekPMaterial() const;
};

CDefPointer<const CMaterialDef> CThingObject::PeekPMaterial() const
{
    if (PDef->MaterialIndex) {
        CDefPointer<const CMaterialDef> material;
        CThingObjectDef* def = PDef;
        GetDefinitionManager()->GetDef(def->MaterialIndex, &material);
        return material;
    }
    return CDefPointer<const CMaterialDef>();
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0;
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() const { ++g_destroyed; }
static CDefinitionManager g_mgr;
CDefinitionManager* CThingObject::GetDefinitionManager() const { return &g_mgr; }
void CThingObject::v00() {}
int main()
{
    CMaterialDef mat; mat.m_refCount = 1; g_def = &mat;
    CThingObjectDef od; memset(&od, 0, sizeof(od));
    CThingObject o; memset(o._pad04, 0, sizeof(o._pad04)); o.PDef = &od;
    { CDefPointer<const CMaterialDef> p = o.PeekPMaterial(); CHECK(p.Object == 0); }
    od.MaterialIndex = 4;
    { CDefPointer<const CMaterialDef> p = o.PeekPMaterial(); CHECK(p.Object == &mat); CHECK(mat.m_refCount == 1); }
    CHECK(mat.m_refCount == 0); CHECK(g_destroyed == 1);
    g_def = 0; { CDefPointer<const CMaterialDef> p = o.PeekPMaterial(); CHECK(p.Object == 0); }
    if (fails == 0) printf("00703150_TEST PASS\n"); else printf("00703150_TEST FAIL\n");
    return fails;
}