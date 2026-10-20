// Behaviour test for 007868a0: body copied from src/007868a0.cpp (CTCSkeletalMorph::vfunc_4 / OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strDtors = 0;
struct CCharString {
    void* PStringData;
    ~CCharString() { ++g_strDtors; }
};
static int g_getString = 0;
struct CDefStringTable {
    CCharString GetString(long id) { ++g_getString; CCharString s; s.PStringData = (void*)(id * 16); return s; }
};
CDefStringTable g_DefStringTable;
static int g_deleted = 0;
struct CDefPointeeBase {
    virtual void Destroy() {}
    virtual void Delete() { ++g_deleted; }
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CDefStringArray {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    long operator[](unsigned long n) const { return _M_start[n]; }
};
struct CSkeletalMorphDef : CDefPointeeBase {
    unsigned char   _pad08[0x28 - 0x08];
    CDefStringArray SkeletalMorphs;
};
static CSkeletalMorphDef* g_morphDef = 0; static int g_getSubDef = 0;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CSkeletalMorphDef>* out)
    {
        ++g_getSubDef;
        if (!g_morphDef) return false;
        ++g_morphDef->m_refCount; out->Object = g_morphDef; return true;
    }
};
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;
};
static int g_addMorph = 0; static long g_tags[4]; static void* g_names[4]; static float g_weights[4]; static int g_calculate = 0;
struct CTCSkeletalMorph {
    virtual void v00() {}
    CThing* ParentThing;
    void AddMorph(long tag, const CCharString& name, float weight) { if (g_addMorph < 4) { g_tags[g_addMorph] = tag; g_names[g_addMorph] = name.PStringData; g_weights[g_addMorph] = weight; } ++g_addMorph; }
    void CalculateSkeletalMorph() { ++g_calculate; CHECK(g_addMorph >= 0); }
    void vfunc_4();
};

void CTCSkeletalMorph::vfunc_4()
{
    CDefClassBase* thingDef = ParentThing->PDef;
    if (thingDef) {
        CDefPointer<CSkeletalMorphDef> out;
        bool found = thingDef->GetSubDef(&out);
        CSkeletalMorphDef* def = out.Object;
        if (found) {
            for (unsigned long i = 0; i < def->SkeletalMorphs.size(); ++i) {
                {
                    CCharString name = g_DefStringTable.GetString(def->SkeletalMorphs[i]);
                    AddMorph(0x534b454c, name, 1.0f);
                }
            }
            CalculateSkeletalMorph();
        }
        if (def && --def->m_refCount == 0)
            def->Delete();
    }
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CSkeletalMorphDef morph; morph.m_refCount = 1;
    long ids[2] = { 3, 5 }; morph.SkeletalMorphs._M_start = ids; morph.SkeletalMorphs._M_finish = ids + 2;
    CTCSkeletalMorph m; m.ParentThing = &thing;
    // no thing def: nothing happens
    thing.PDef = 0; m.vfunc_4(); CHECK(g_getSubDef == 0 && g_calculate == 0);
    // no skeletal morph sub-def: no morphs, no recalculation
    thing.PDef = &classDef; g_morphDef = 0; m.vfunc_4(); CHECK(g_getSubDef == 1 && g_addMorph == 0 && g_calculate == 0 && g_deleted == 0);
    // two morphs listed: both added under the SKEL tag at full weight, then recalculated; the def released
    g_morphDef = &morph; m.vfunc_4();
    CHECK(g_addMorph == 2 && g_getString == 2 && g_strDtors >= 2); CHECK(g_tags[0] == 0x534b454c && g_tags[1] == 0x534b454c); CHECK(g_names[0] == (void*)48 && g_names[1] == (void*)80);
    CHECK(g_weights[0] == 1.0f && g_weights[1] == 1.0f && g_calculate == 1); CHECK(morph.m_refCount == 1 && g_deleted == 0);
    // an empty list still recalculates; a def held only by the lookup is deleted
    morph.SkeletalMorphs._M_finish = ids; morph.m_refCount = 0; m.vfunc_4(); CHECK(g_addMorph == 2 && g_calculate == 2 && g_deleted == 1);
    if (fails == 0) printf("007868a0_TEST PASS\n"); else printf("007868a0_TEST FAIL\n");
    return fails;
}