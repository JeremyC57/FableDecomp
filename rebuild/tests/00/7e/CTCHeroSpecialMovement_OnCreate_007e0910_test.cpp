// Behaviour test for 007e0910: body copied from src/007e0910.cpp (CTCHeroSpecialMovement::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
long g_framesPerSecond_01375550 = 30;
static int g_deleted = 0;
struct CDefPointeeBase {
    virtual void Destroy() {}
    virtual void Delete() { ++g_deleted; }
    long m_refCount;
};
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};
struct CHeroSpecialMovementDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    float SprintDurationSeconds;
};
static CHeroSpecialMovementDef* g_subDef = 0; static int g_getSubDef = 0;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CHeroSpecialMovementDef>* out);
};
bool CDefClassBase::GetSubDef(CDefPointer<const CHeroSpecialMovementDef>* out)
{
    ++g_getSubDef;
    if (!g_subDef) return false;
    ++g_subDef->m_refCount; out->Object = g_subDef; return true;
}
static CDefClassBase* g_classDef = 0; static long g_askedIndex = -1; static int g_getDef = 0;
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CDefClassBase>* out);
};
bool CDefinitionManager::GetDef(long index, CDefPointer<const CDefClassBase>* out)
{
    ++g_getDef; g_askedIndex = index;
    if (!g_classDef) return false;
    ++g_classDef->m_refCount; out->Object = g_classDef; return true;
}
static CDefinitionManager g_manager;
class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;
};
class CTCBase {
public:
    virtual ~CTCBase() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CDefinitionManager* GetDefinitions() { return &g_manager; }
};
class CTCHeroSpecialMovement : public CTCBase {
public:
    bool  Sprinting;
    bool  Running;
    float SprintFramesRemaining;
    long  RunTimer;
    CDefPointer<const CHeroSpecialMovementDef> PHeroSpecialMovementDef;
    virtual void OnCreate();
};
void CTCHeroSpecialMovement::OnCreate()
{
    unsigned short defIndex = ParentThing->DefIndex;
    if (defIndex) {
        CDefPointer<const CDefClassBase> out;
        GetDefinitions()->GetDef(defIndex, &out);
        CDefClassBase* def = (CDefClassBase*)out.Object;
        def->GetSubDef(&PHeroSpecialMovementDef);
        if (def && --def->m_refCount == 0)
            def->Delete();
    }
    Sprinting = false;
    SprintFramesRemaining = (float)g_framesPerSecond_01375550 * PHeroSpecialMovementDef->SprintDurationSeconds;
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CHeroSpecialMovementDef subDef;
    memset(&thing, 0, sizeof(thing)); thing.DefIndex = 12; classDef.m_refCount = 1; subDef.m_refCount = 1; subDef.SprintDurationSeconds = 2.5f;
    g_classDef = &classDef; g_subDef = &subDef;
    CTCHeroSpecialMovement tc; tc.ParentThing = &thing; tc.Sprinting = true; tc.SprintFramesRemaining = -1.0f;
    tc.OnCreate();
    CHECK(g_getDef == 1 && g_askedIndex == 12 && g_getSubDef == 1); CHECK(tc.PHeroSpecialMovementDef.Object == &subDef && subDef.m_refCount == 2);
    CHECK(!tc.Sprinting); CHECK(tc.SprintFramesRemaining == 75.0f); CHECK(classDef.m_refCount == 1 && g_deleted == 0);
    // a class def held only by the lookup is deleted after the sub-def is taken
    classDef.m_refCount = 0; tc.OnCreate(); CHECK(g_deleted == 1 && subDef.m_refCount == 3);
    // no def index: only the sprint reset from the def already held
    thing.DefIndex = 0; tc.Sprinting = true; subDef.SprintDurationSeconds = 1.0f; tc.OnCreate();
    CHECK(g_getDef == 2 && !tc.Sprinting && tc.SprintFramesRemaining == 30.0f);
    if (fails == 0) printf("007e0910_TEST PASS\n"); else printf("007e0910_TEST FAIL\n");
    return fails;
}