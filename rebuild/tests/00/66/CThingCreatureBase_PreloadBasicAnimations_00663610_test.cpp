// Behaviour test for 0x00663610: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingCreatureBase::PreloadBasicAnimations() -- start loading every walk and run animation of the creature's appearance animation set, and every animation whose name starts with "TURNING_"
// Leaf: PreloadBasicAnimations
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (same-TU stand-in below), CThing::GetWorld @ 0x004C7AB0,
// CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CAnimationSet::GetNumberOfAnimations(const CCharString&,
// const CAnimFilterBase&) @ 0x005DC290, CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&) @ 0x005DC340,
// CAnimationSet::GetAnimsWithNameStartingWith @ 0x005DCB00, CMeshBank::GetAnimation(long, bool) @ 0x00A242C0,
// C3DAnimationInfo::BeginLoading @ 0x00A26730, free @ 0x00BFEA14.
// Strings: 0x0125A990 "ST_WALK", 0x0125A998 "ST_RUN", 0x0125AB08 "TURNING_". CAnimFilterNoGroup vtable 0x0122FFDC.
// Layout (retail, landed 0x006631C0 / 0x00662C10 models): PBaseDef +0x70; CWorld main mesh bank +0x3c; CAppearanceDef
// animation set +0x34; def count +4, virtual Destroy slot 1.
// Shape: GetSubDef's same-TU stand-in only writes the out slot (the def slot does not escape: the release is `dec dword ptr
// [ecx+4] ; jne`), the def released by hand through a const local; a CAnimFilterNoGroup temporary per call; the three names are
// named locals of the success block; the prefix vector is indexed through a reference-returning `*(begin() + i)` operator[]
// (the value-returning one swaps ecx/edx for eax/ecx in the loop and the release).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct C3DAnimationInfo {
    void BeginLoading();                        // 0x00A26730
};
struct CMeshBank {
    C3DAnimationInfo* GetAnimation(long index, bool immediately);   // 0x00A242C0
};
struct CWorld {
    unsigned char _pad[0x3c];
    CMeshBank* PMainMeshBank;                   // +0x3c
};
struct CAnimationEntry {
    long AnimIndex;                             // +0x00
};
struct __declspec(novtable) CAnimFilterBase {
    virtual void v000();
    virtual void v001();
    virtual void v002();
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
    virtual void v000();
    virtual void v001();
    virtual void v002();
};
struct CAnimationEntryVector {                  // std::vector<const CAnimationEntry*>
    const CAnimationEntry** first;
    const CAnimationEntry** last;
    const CAnimationEntry** end;
    CAnimationEntryVector() : first(0), last(0), end(0) { }
    ~CAnimationEntryVector() { if (first) free(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    const CAnimationEntry** begin() const { return first; }
    const CAnimationEntry* const& operator[](unsigned long i) const { return *(begin() + i); }
};
struct CAnimationSet {
    long GetNumberOfAnimations(const CCharString& name, const CAnimFilterBase& filter) const;                       // 0x005DC290
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims, const CAnimFilterBase& filter) const;   // 0x005DCB00
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CAppearanceDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<const CAppearanceDef>* out) const;     // 0x0042B0A2
};
void* FindDef_3610();
 bool CThingBaseDef::GetSubDef(CDefPointer<const CAppearanceDef>* out) const
{
    void* def = FindDef_3610();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

struct CThingCreatureBase {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
    CWorld* GetWorld();                         // 0x004C7AB0
    void PreloadBasicAnimations();
};

void CThingCreatureBase::PreloadBasicAnimations()
{
    CDefPointer<const CAppearanceDef> def;
    if (PBaseDef->GetSubDef(&def)) {
        CMeshBank* bank = GetWorld()->PMainMeshBank;
        const CAnimationSet* set = &def->Animation;
        CCharString walk("ST_WALK", -1);
        long count = set->GetNumberOfAnimations(walk, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->GetAnimation(set->GetAnimation(walk, i, CAnimFilterNoGroup())->AnimIndex, false)->BeginLoading();
        CCharString run("ST_RUN", -1);
        count = set->GetNumberOfAnimations(run, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->GetAnimation(set->GetAnimation(run, i, CAnimFilterNoGroup())->AnimIndex, false)->BeginLoading();
        CCharString turning("TURNING_", -1);
        CAnimationEntryVector anims;
        set->GetAnimsWithNameStartingWith(turning, anims, CAnimFilterNoGroup());
        for (unsigned long i = 0; i < anims.size(); ++i)
            bank->GetAnimation(anims[i]->AnimIndex, false)->BeginLoading();
    }
    const CDefClassBase* object = def.Object;
    if (object && --const_cast<CDefClassBase*>(object)->RefCount == 0)
        const_cast<CDefClassBase*>(object)->Destroy();
}

// ---- recording stubs ----
void CAnimFilterBase::v000() { }
void CAnimFilterBase::v001() { }
void CAnimFilterBase::v002() { }
void CAnimFilterNoGroup::v000() { }
void CAnimFilterNoGroup::v001() { }
void CAnimFilterNoGroup::v002() { }
static int g_strings = 0, g_destroyed = 0, g_loads = 0;
static long g_loaded[32];
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
static C3DAnimationInfo g_infos[64];
void C3DAnimationInfo::BeginLoading() { if (g_loads < 32) g_loaded[g_loads] = (long)(this - g_infos); ++g_loads; }
C3DAnimationInfo* CMeshBank::GetAnimation(long index, bool immediately) { return immediately ? 0 : &g_infos[index]; }
static CMeshBank g_bank;
static CWorld g_world;
CWorld* CThingCreatureBase::GetWorld() { return &g_world; }
static CAnimationEntry g_walk[3] = { {10}, {11}, {12} };
static CAnimationEntry g_run[2] = { {20}, {21} };
static CAnimationEntry g_turn[2] = { {30}, {31} };
long CAnimationSet::GetNumberOfAnimations(const CCharString& name, const CAnimFilterBase& filter) const
{
    if (strcmp((const char*)name.m_pData, "ST_WALK") == 0) return 3;
    if (strcmp((const char*)name.m_pData, "ST_RUN") == 0) return 2;
    return 0;
}
const CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const
{
    if (strcmp((const char*)name.m_pData, "ST_WALK") == 0) return &g_walk[index];
    return &g_run[index];
}
void CAnimationSet::GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims, const CAnimFilterBase& filter) const
{
    if (strcmp((const char*)prefix.m_pData, "TURNING_") != 0) return;
    anims.first = (const CAnimationEntry**)malloc(2 * sizeof(CAnimationEntry*));
    anims.first[0] = &g_turn[0];
    anims.first[1] = &g_turn[1];
    anims.last = anims.end = anims.first + 2;
}
void CDefClassBase::v00() { }
void CDefClassBase::Destroy() { ++g_destroyed; }
static CAppearanceDef g_def;
static bool g_found = false;
void* FindDef_3610() { return g_found ? &g_def : 0; }

int main()
{
    g_world.PMainMeshBank = &g_bank;
    static CThingBaseDef baseDef;
    static CThingCreatureBase creature;
    creature.PBaseDef = &baseDef;
    bool ok = true;

    g_found = true; g_def.RefCount = 1;
    creature.PreloadBasicAnimations();
    ok = ok && g_loads == 7 && g_loaded[0] == 10 && g_loaded[1] == 11 && g_loaded[2] == 12
        && g_loaded[3] == 20 && g_loaded[4] == 21 && g_loaded[5] == 30 && g_loaded[6] == 31
        && g_destroyed == 1 && g_def.RefCount == 0 && g_strings == 0;

    g_found = false; g_loads = 0;
    creature.PreloadBasicAnimations();
    ok = ok && g_loads == 0 && g_destroyed == 1;

    g_found = true; g_def.RefCount = 3; g_loads = 0;
    creature.PreloadBasicAnimations();
    ok = ok && g_loads == 7 && g_def.RefCount == 2 && g_destroyed == 1;

    if (ok) printf("00663610_TEST PASS\n");
    else printf("00663610_TEST FAIL loads=%d destroyed=%d\n", g_loads, g_destroyed);
    return ok ? 0 : 1;
}