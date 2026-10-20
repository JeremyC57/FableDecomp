#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#endif
struct CDefString {
    long TablePos;
    bool operator==(const char* text) const;    // 0x005B3440
};
struct CAIBrainDef {
    virtual void v00(); virtual void Destroy();  // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetInstantiationName() const;   // slot 10 (+0x28)
    long m_refCount;                             // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CAIBrainDef>* out);   // 0x0079BD80
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CAIBrainDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

struct SSuspenderNode;
struct SSuspenderIterator {
    SSuspenderNode* m_node;
    SSuspenderIterator() { }
};
struct SSuspenderInsertResult {                 // pair<iterator, bool>
    SSuspenderIterator first;
    bool               second;
    SSuspenderInsertResult() { }
};
struct CSuspenderSet {                          // STLport set<void*>: {header, size}
    SSuspenderNode* m_header;                   // +0
    unsigned long   m_size;                     // +4
    SSuspenderInsertResult InsertUnique(void* const& value);   // 0x00835560
    bool empty() const { return m_size == 0; }
};
struct CThingCreatureDef {
    unsigned char _pad00[0xe8];
    long BrainDef;                              // +0xe8
};
struct CThingCreatureBase {
    void*         __vftable;
    unsigned char _pad04[0xe0 - 0x04];
    CThingCreatureDef* PDef;                    // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    const CThingCreatureDef* GetCreatureDef() const { return PDef; }
    unsigned char _pad0e4[0x15e - 0xe4];
    bool IsShowingDebugInfo : 1;                // +0x15e bit 0
    CDefinitionManager* GetDefinitionManager();  // 0x004C7990 (CThing)
    bool ValidateInitialisation();              // 0x006A4D60 (CThingPhysical)
    void CreateDefaultWeapons();                // 0x00664680
    bool OnCreate();                            // 0x00666310
};
struct CThingAICreature : CThingCreatureBase {
    unsigned char _pad160[0x1c8 - 0x160];
    bool          PerformingVillageTaskFlag : 1;    // +0x1c8 bit 0
    bool          AISuspended : 1;                  // +0x1c8 bit 1
    CSuspenderSet Suspenders;                       // +0x1cc
    __forceinline void AddSuspender(void* suspender)
    {
        Suspenders.InsertUnique(suspender);
        AISuspended = !Suspenders.empty();
    }
    void InitialiseFromDef();                   // 0x008338D0
    bool OnCreate();
};

bool CThingAICreature::OnCreate()
{
    if (CThingCreatureBase::OnCreate()) {
        InitialiseFromDef();
        IsShowingDebugInfo = false;
        if (ValidateInitialisation()) {
            CreateDefaultWeapons();
            if (PDef->BrainDef > 0) {
                CDefPointer<CAIBrainDef> brainDef;
                GetDefinitionManager()->GetDef(GetCreatureDef()->BrainDef, &brainDef);
                if (brainDef->GetInstantiationName() == "BRAIN_NULL")
                    AddSuspender(0);
            } else {
                AddSuspender(0);
            }
            return true;
        }
    }
    return false;
}
// ---- test doubles ----
static const char* g_brainName = "BRAIN_NULL";
static long g_namePos = 0;
bool CDefString::operator==(const char* text) const { return TablePos == 1 && std::strcmp(g_brainName, text) == 0; }
void CAIBrainDef::v00() {} void CAIBrainDef::Destroy() {} void CAIBrainDef::v02() {} void CAIBrainDef::v03() {}
void CAIBrainDef::v04() {} void CAIBrainDef::v05() {} void CAIBrainDef::v06() {} void CAIBrainDef::v07() {}
void CAIBrainDef::v08() {} void CAIBrainDef::v09() {}
CDefString CAIBrainDef::GetInstantiationName() const { CDefString s; s.TablePos = 1; return s; }
struct CTestBrainDef : CAIBrainDef { int destroyed; virtual void Destroy() { ++destroyed; } };
static CTestBrainDef* g_brain;
void* FindDef_CDefinitionManager_GetDef_0() { ++g_brain->m_refCount; return g_brain; }
static int g_inserts = 0; static void* g_lastKey = (void*)1;
SSuspenderInsertResult CSuspenderSet::InsertUnique(void* const& v) { ++g_inserts; g_lastKey = v; ++m_size; return SSuspenderInsertResult(); }
static bool g_baseOk = true, g_validOk = true; static int g_initDef = 0, g_weapons = 0;
static CDefinitionManager g_dm;
CDefinitionManager* CThingCreatureBase::GetDefinitionManager() { return &g_dm; }
bool CThingCreatureBase::ValidateInitialisation() { return g_validOk; }
void CThingCreatureBase::CreateDefaultWeapons() { ++g_weapons; }
bool CThingCreatureBase::OnCreate() { return g_baseOk; }
void CThingAICreature::InitialiseFromDef() { ++g_initDef; }
int main()
{
    CTestBrainDef brain; brain.m_refCount = 1; brain.destroyed = 0; g_brain = &brain;
    CThingCreatureDef cdef; cdef.BrainDef = 5;
    CThingAICreature c; c.PDef = &cdef; c.IsShowingDebugInfo = 1; c.AISuspended = 0; c.PerformingVillageTaskFlag = 1;
    c.Suspenders.m_header = 0; c.Suspenders.m_size = 0;
    bool ok = c.OnCreate() && g_initDef == 1 && !c.IsShowingDebugInfo && g_weapons == 1 && g_inserts == 1 && g_lastKey == 0
        && c.AISuspended && c.PerformingVillageTaskFlag && brain.m_refCount == 1 && brain.destroyed == 0;
    g_brainName = "BRAIN_VILLAGER"; c.Suspenders.m_size = 0; c.AISuspended = 0;
    ok = ok && c.OnCreate() && g_inserts == 1 && !c.AISuspended && brain.m_refCount == 1;
    cdef.BrainDef = 0;                                      // no brain def: suspended
    ok = ok && c.OnCreate() && g_inserts == 2 && c.AISuspended;
    g_validOk = false;
    ok = ok && !c.OnCreate() && g_weapons == 3 && g_initDef == 4;
    g_baseOk = false;
    ok = ok && !c.OnCreate() && g_initDef == 4;
    if (!ok) { std::printf("00833a70_TEST FAIL\n"); return 1; }
    std::printf("00833a70_TEST PASS\n");
    return 0;
}