// Behaviour test for 006631c0: src/006631c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingCreatureBase::PreloadAnimation(long animation)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::PreloadAnimation(long animation) -- fetch the creature's appearance sub-def and touch (preload) every variant of one animation type in its animation set (no group filter)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006631C0, 160 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (byte-twin label GetSubDef<CAbilityDef>),
//   CThing::GetWorld @ 0x004C7AB0, CAnimationSet::GetNumberOfAnimations(long, const CAnimFilter&) @ 0x005DC290
//   (auto label "AnimationDef_CountMatchingPreloads"), CAnimationSet::GetAnimation(long, long, const CAnimFilter&)
//   @ 0x005DC340 (auto label "Animation_FindNthValid"; returns the entry, animation index first), the mesh bank's
//   GetAnimation(long) @ 0x00A242C0, C3DAnimationInfo::TouchAnimation @ 0x00A26A50.
// Identity: the frame object whose only word is 0x0122FFDC is a CAnimFilterNoGroup temporary (RTTI locator at
//   vtable-4: .?AVCAnimFilterNoGroup@@; slot 0 0x0041CD13 the accept test, slot 1 0x00429A2F Clone), built anew for
//   each call (the vptr store is repeated in the loop).
// Layout: retail CThing PBaseDef +0x70; retail CWorld +0x3c = main mesh bank; CAppearanceDef Animation at +0x34
//   (see 0x004717C7); def ref count +4, virtual Destroy slot 1.
// Shape: GetSubDef's out CDefPointer (zero store) released by hand at the end (one read of the slot); the def pointer is re-read from its slot for the
//   set address; count in ebx, index in esi, bank in ebp.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
class C3DAnimationInfo { public:
    void TouchAnimation();                      // 0x00A26A50
};
struct CMeshBank {
    C3DAnimationInfo* GetAnimation(long index); // 0x00A242C0
};
class CWorld { public:
    unsigned char _pad[0x3c];
    CMeshBank* PMainMeshBank;                   // +0x3c
};
class CAnimationEntry { public:
    long AnimIndex;                             // +0x00
};
struct CAnimFilter {
    virtual bool Accept(const CAnimationEntry* entry) const = 0;
    virtual CAnimFilter* Clone() const = 0;
};
struct CAnimFilterNoGroup : CAnimFilter {      // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilter* Clone() const;                         // 0x00429A2F
};
class CAnimationSet { public:
    long GetNumberOfAnimations(long animation, const CAnimFilter& filter) const;                    // 0x005DC290
    const CAnimationEntry* GetAnimation(long animation, long index, const CAnimFilter& filter) const; // 0x005DC340
};
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
class CAppearanceDef : public CDefClassBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
class CThingBaseDef { public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;      // 0x0042B0A2
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

class CThingCreatureBase { public:
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
    CWorld* GetWorld();                         // 0x004C7AB0
    void PreloadAnimation(long animation);
};

void CThingCreatureBase::PreloadAnimation(long animation)
{
    CDefPointer<const CAppearanceDef> def;
    if (PBaseDef->GetSubDef(&def)) {
        CMeshBank* bank = GetWorld()->PMainMeshBank;
        const CAnimationSet* set = &def->Animation;
        long count = set->GetNumberOfAnimations(animation, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->GetAnimation(set->GetAnimation(animation, i, CAnimFilterNoGroup())->AnimIndex)->TouchAnimation();
    }
    const CDefClassBase* object = def.Object;
    if (object && --const_cast<CDefClassBase*>(object)->RefCount == 0)
        const_cast<CDefClassBase*>(object)->Destroy();
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_find = 0, g_release = 0, g_destroy = 0, g_world = 0, g_count = 0, g_getAnim = 0; static int g_touch[8];
static long g_animArg = -1; static long g_countResult = 0;
static C3DAnimationInfo g_infos[8]; static CMeshBank g_bank; static CWorld g_worldObj; static CAnimationEntry g_entries[4];
void* FindDef_s7r_0() { ++g_find; return g_def; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefClassBase::v00() {}
void CDefClassBase::Destroy() { ++g_destroy; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilter* CAnimFilterNoGroup::Clone() const { return 0; }
long CAnimationSet::GetNumberOfAnimations(long animation, const CAnimFilter& filter) const { ++g_count; g_animArg = animation; CHECK(filter.Accept(0)); return g_countResult; }
const CAnimationEntry* CAnimationSet::GetAnimation(long animation, long index, const CAnimFilter& filter) const
{ ++g_getAnim; CHECK(animation == g_animArg); CHECK(filter.Accept(0)); return &g_entries[index & 3]; }
C3DAnimationInfo* CMeshBank::GetAnimation(long index) { CHECK(this == &g_bank); return &g_infos[index & 7]; }
void C3DAnimationInfo::TouchAnimation() { ++g_touch[this - g_infos]; }
CWorld* CThingCreatureBase::GetWorld() { ++g_world; return &g_worldObj; }
int main()
{
    g_worldObj.PMainMeshBank = &g_bank;
    g_entries[0].AnimIndex = 2; g_entries[1].AnimIndex = 4; g_entries[2].AnimIndex = 6;
    static CAppearanceDef def; def.RefCount = 5;
    static CThingBaseDef base; CThingCreatureBase c; c.PBaseDef = &base;
    g_def = 0; c.PreloadAnimation(3); CHECK(g_find == 1 && g_world == 0 && g_count == 0 && g_destroy == 0);
    g_def = &def; g_countResult = 3; c.PreloadAnimation(3);
    CHECK(g_world == 1 && g_count == 1 && g_animArg == 3 && g_getAnim == 3); CHECK(g_touch[2] == 1 && g_touch[4] == 1 && g_touch[6] == 1);
    CHECK(def.RefCount == 4 && g_destroy == 0);
    g_countResult = 0; def.RefCount = 1; c.PreloadAnimation(8); CHECK(g_count == 2 && g_getAnim == 3 && def.RefCount == 0 && g_destroy == 1);
    CHECK(g_release == 0);
    if (fails == 0) printf("006631c0_TEST PASS\n"); else printf("006631c0_TEST FAIL\n");
    return fails;
}