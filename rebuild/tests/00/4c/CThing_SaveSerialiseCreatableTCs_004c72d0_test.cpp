// Behaviour test for CThing::SaveSerialiseCreatableTCs (0x004C72D0): one `CreateTC "<name>";` line per live
// creatable component, in map order; non-creatable or to-be-removed components are skipped; adding components
// is forbidden during the walk and permitted again after.
#include <cstdio>
#include <cstring>
#include <string>
static int g_strings = 0;
class CCharString {
public:
    std::string* PStringData;
    CCharString() { ++g_strings; PStringData = new std::string(); }
    CCharString(const char* text) { ++g_strings; PStringData = new std::string(text); }
    CCharString(const CCharString& o) { ++g_strings; PStringData = new std::string(*o.PStringData); }
    ~CCharString() { --g_strings; delete PStringData; }
    CCharString& operator+=(const CCharString& other) { *PStringData += *other.PStringData; return *this; }
};
CCharString operator+(const char* left, const CCharString& right) { CCharString r(left); r += right; return r; }
CCharString operator+(const CCharString& left, const char* right) { CCharString r(left); CCharString t(right); r += t; return r; }
static bool g_addingPermittedDuringWalk = true;
struct CThing;
static CThing* g_thing;
static bool thingPermits(CThing* t);
struct CTCBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {}
    virtual bool IsCreatable_slot19() { if (thingPermits(g_thing)) g_addingPermittedDuringWalk = false; return creatable; }
    virtual void v20() {} virtual void v21() {}
    virtual CCharString GetName_slot22() { return CCharString(name); }
    void* ParentThing; bool ToBeRemoved; bool creatable; const char* name;
};
struct SComponentPair { long first; CTCBase* second; };
struct CThing {
    void* __vftable; unsigned char _pad04[0x44 - 0x04];
    SComponentPair* m_componentsFirst; SComponentPair* m_componentsLast;
    unsigned char _pad4c[0x93 - 0x4c];
    unsigned char NotPausedDuringCutscenes : 1; unsigned char DeathPersistent : 1; unsigned char KillOnLevelUnload : 1; unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1; unsigned char IsAffectedByBulletTimeOverride : 1; unsigned char AddingTCsPermitted : 1;
    void SaveSerialiseCreatableTCs(CCharString& out);
};
static bool thingPermits(CThing* t) { return t->AddingTCsPermitted != 0; }
void CThing::SaveSerialiseCreatableTCs(CCharString& out)
{
    AddingTCsPermitted = false;
    for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
        if (it->second->IsCreatable_slot19()) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved)
                out += "CreateTC \"" + tc->GetName_slot22() + "\";\r\n";
        }
    }
    AddingTCsPermitted = true;
}
static int fail(const char* why) { std::printf("004c72d0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CTCBase tcs[4];
    const char* names[4] = { "CTCPhysics", "CTCLight", "CTCSound", "CTCScript" };
    bool creatable[4] = { true, false, true, true }; bool removed[4] = { false, false, true, false };
    static SComponentPair pairs[4];
    for (int i = 0; i < 4; ++i) { tcs[i].name = names[i]; tcs[i].creatable = creatable[i]; tcs[i].ToBeRemoved = removed[i]; pairs[i].first = i * 10; pairs[i].second = &tcs[i]; }
    static CThing thing; g_thing = &thing; thing.m_componentsFirst = pairs; thing.m_componentsLast = pairs + 4; thing.AddingTCsPermitted = 1; thing.LevelIsUnloading = 1;
    {
        CCharString out("# header\n");
        thing.SaveSerialiseCreatableTCs(out);
        if (*out.PStringData != "# header\nCreateTC \"CTCPhysics\";\r\nCreateTC \"CTCScript\";\r\n") return fail("serialised text");
        if (!thing.AddingTCsPermitted || !thing.LevelIsUnloading || !g_addingPermittedDuringWalk) return fail("adding flag not restored / not cleared during the walk");
    }
    { // empty map: untouched output, flag restored
        thing.m_componentsLast = pairs; CCharString out("x"); thing.SaveSerialiseCreatableTCs(out);
        if (*out.PStringData != "x" || !thing.AddingTCsPermitted) return fail("empty map");
    }
    if (g_strings != 0) return fail("string leak");
    std::printf("004c72d0_TEST PASS\n");
    return 0;
}