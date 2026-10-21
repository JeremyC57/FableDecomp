// Behaviour test for 00750050: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCChest::GetNumberOfKeysToUnlock(CCharString&) const
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCChest::GetNumberOfKeysToUnlock(CCharString& keyName) const -- how many opener objects the chest needs and the display name of that opener; 0 and "" when the chest def asks for none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750050, 186 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CChestDef>(CDefPointer<const CChestDef>*) @ 0x0074F7D0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", byte twin, as the landed GetOpenAnimationForCreature 0x00750240),
// CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDefNameFromGlobalIndex(long) @ 0x009ACCC0 (a CDefString
// by hidden pointer), CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable @ 0x013CA828, CCharString::operator=
// (const CCharString&) @ 0x0099EFB0 and (const char*) @ 0x0099EFE0, ~CCharString @ 0x0099EAE0, the def's virtual slot 1 on the
// last release. Literal "" @ 0x0122D70E.
// Layout: CTCBase ParentThing +0x04; CThing def +0x70; CChestDef OpenerObject +0x34, OpenersRequired +0x38 (GOTCHAS chest facts).
// Shape: the landed GetOpenAnimationForCreature 0x00750240 model verbatim (thing def local, out CDefPointer, `found`, holder
// released on both returns) with the same-TU GetSubDef stand-in (the out word no longer escapes: 8-byte frame, the GetString
// temporary and the CDefString share it with the out word). The condition reads OpenerObject/OpenersRequired through
// `out.Object` and the body through the holder `def`: two base variables, so VC7.1 re-reads [esi+34h] for the push (retail)
// instead of CSE-ing the test's load. The key name is a block-scoped named local (RVO into the frame slot, passed to
// operator= by its address) -- an unnamed temporary passes GetString's returned eax.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString { long Index; };
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index) const;   // 0x009ACCC0
};
struct CDefPointeeBase {
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
struct CChestDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    long OpenerObject;                                  // +0x34 (global def index)
    long OpenersRequired;                               // +0x38
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def taken out of the out pointer, released when the function returns
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CChestDef>* out);        // 0x0074F7D0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CChestDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CChestDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
struct CTCChest {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    const CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    __forceinline CChestDef* GetChestDef(bool& found) const
    {
        CDefClassBase* thingDef = ParentThing->PDef;
        CDefPointer<CChestDef> out;
        found = thingDef->GetSubDef(&out);
        return out.Object;
    }
    long GetNumberOfKeysToUnlock(CCharString& keyName) const;
};

long CTCChest::GetNumberOfKeysToUnlock(CCharString& keyName) const
{
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<CChestDef> out;
    bool found = thingDef->GetSubDef(&out);
    CDefHolder<CChestDef> def(out.Object);
    if (found && out.Object->OpenerObject > 0 && out.Object->OpenersRequired > 0) {
        {
            CCharString name = g_DefStringTable.GetString(GetDefinitionManager()->GetDefNameFromGlobalIndex(def->OpenerObject).Index);
            keyName = name;
        }
        return def->OpenersRequired;
    }
    keyName = "";
    return 0;
}

// ---- stubs ----
static char g_log[2048];
static void logf(const char* fmt, long a = 0, long b = 0) { char t[256]; _snprintf(t, sizeof t, fmt, a, b); strcat(g_log, t); strcat(g_log, ";"); }
struct CTestChestDef : CChestDef {
    virtual void Destroy() { logf("Destroy"); }
    virtual void Delete() { logf("Delete"); }
};
void CDefPointeeBase::Destroy() { logf("BaseDestroy"); }
void CDefPointeeBase::Delete() { logf("BaseDelete"); }
static CTestChestDef g_def;
static CTestChestDef* g_found;          // what the GetSubDef stand-in's extern returns
void* Find_CDefClassBase_GetSubDef(const void* owner) { logf("FindSubDef(%ld)", (long)(owner == (const void*)&g_def ? 1 : 0)); return g_found; }
static CDefinitionManager g_manager;
CDefStringTable g_DefStringTable;
CCharString& CCharString::operator=(const CCharString& other) { logf("assign(str %ld)", (long)(size_t)other.PStringData); PStringData = other.PStringData; return *this; }
CCharString& CCharString::operator=(const char* text) { logf("assign(\"%s\")", (long)(size_t)text); PStringData = (void*)text; return *this; }
CCharString::~CCharString() { logf("~CCharString(%ld)", (long)(size_t)PStringData); }
static CCharString MakeString(long v) { void* p = (void*)(size_t)v; return *(CCharString*)&p; }
CCharString CDefStringTable::GetString(long id) { logf("GetString(%ld,%ld)", id, (long)(this == &g_DefStringTable)); return MakeString(id * 100); }
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index) const { logf("GetDefName(%ld)", index); CDefString s; s.Index = index + 1; return s; }
const CDefinitionManager* CTCChest::GetDefinitionManager() const { logf("GetDefinitionManager"); return &g_manager; }

int main()
{
    CThing thing; memset(&thing, 0, sizeof thing);
    CTCChest* chest = (CTCChest*)calloc(1, sizeof(CTCChest));
    chest->ParentThing = &thing;
    CCharString keyName; keyName.PStringData = 0;

    // 1. def found, opener object and count set: name resolved, count returned, def released (count 2 -> 1, no Delete)
    thing.PDef = (CDefClassBase*)&g_def;
    g_found = &g_def; g_def.m_refCount = 2; g_def.OpenerObject = 7; g_def.OpenersRequired = 3; g_log[0] = 0;
    long n = chest->GetNumberOfKeysToUnlock(keyName);
    CHECK(n == 3);
    CHECK(strcmp(g_log, "FindSubDef(1);GetDefinitionManager;GetDefName(7);GetString(8,1);assign(str 800);~CCharString(800);") == 0);
    CHECK(keyName.PStringData == (void*)800);
    CHECK(g_def.m_refCount == 1);

    // 2. last reference: the holder's release deletes the def after the name is built
    g_def.m_refCount = 1; g_log[0] = 0;
    n = chest->GetNumberOfKeysToUnlock(keyName);
    CHECK(n == 3);
    CHECK(strstr(g_log, "~CCharString(800);Delete;") != 0);
    CHECK(g_def.m_refCount == 0);

    // 3. opener count zero: "" and 0, def still released
    g_def.m_refCount = 1; g_def.OpenersRequired = 0; g_log[0] = 0;
    n = chest->GetNumberOfKeysToUnlock(keyName);
    CHECK(n == 0);
    CHECK(strcmp(g_log, "FindSubDef(1);assign(\"\");Delete;") == 0);
    CHECK(strcmp((const char*)keyName.PStringData, "") == 0);

    // 4. opener object not set (count positive): "" and 0
    g_def.m_refCount = 2; g_def.OpenerObject = 0; g_def.OpenersRequired = 3; g_log[0] = 0;
    n = chest->GetNumberOfKeysToUnlock(keyName);
    CHECK(n == 0);
    CHECK(strcmp(g_log, "FindSubDef(1);assign(\"\");") == 0);
    CHECK(g_def.m_refCount == 1);

    // 5. no sub def: "" and 0, nothing released
    g_found = 0; g_log[0] = 0;
    n = chest->GetNumberOfKeysToUnlock(keyName);
    CHECK(n == 0);
    CHECK(strcmp(g_log, "FindSubDef(1);assign(\"\");") == 0);

    if (fails == 0) printf("00750050_TEST PASS\n"); else printf("00750050_TEST FAIL\n");
    return fails != 0;
}