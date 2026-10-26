// Behaviour test for 007ef600: src/007ef600.cpp copied verbatim; OnActionUse runs the base, fetches the activate-quest
// sub-def, and for a live creature user sets the activate-quest action (script name from the string table, the def's
// LoadResources flag, the parent thing as the object); the def is released (deleted at refcount zero); a null def returns.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCActionUseActivateQuest::OnActionUse(CThing& user) -- base OnActionUse; a live creature user gets the action of activating the def's quest script
// Leaf: ?OnActionUse@CTCActionUseActivateQuest@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EF600, length 187 bytes. Vtable slot 28 ("vfunc_28" in the catalog), `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified; manifest "$E2"), CDefClassBase::GetSubDef<CActivateQuestDef>
// @ 0x007B5680 (on the thing's def, +0x70; manifest instantiation name is a byte twin), CDefStringTable::GetString(long)
// @ 0x009D49B0 on g_DefStringTable 0x013CA828, CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest @ 0x00843F50,
// CThingCreatureBase::SetCurrentAction @ 0x006644F0 (result dropped), ~CBaseIntelligentPointer @ 0x00A01C10,
// ~CCharString @ 0x0099EAE0, ~CCreatureActionBase @ 0x00693EF0, the def's virtual Delete (slot 1).
// Layout: Ego_r.pdb CActivateQuestDef: ScriptName (CDefString index) +0x28, LoadResources +0x2C. CCreatureAction_ActivateQuest
// is PDB 0xC4 bytes with ScriptName +0xB4, LoadResources +0xB8, PObjectToUse +0xB9; retail CCreatureActionBase is 0xA8 (PDB
// 0xB4), so the members sit at +0xA8/+0xAC/+0xAD (the intelligent pointer is unaligned: declared packed).
// Shape: GetSubDef has a same-TU out-slot-only stand-in body, so the out word does not escape and is reused by the
// script-name string (both at esp+0xC); the def is held in ebx and released at the end with no null test (a null def
// returns first); ParentThing is read into a local after the string, so `this` dies there and its register (edi) takes it,
// which also puts the user in esi; the action temporary is passed by reference straight from the ctor's eax and destroyed
// inline with no vtable store (novtable leaf): PObjectToUse, ScriptName, then the base.
class CCharString { public:
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
class CThing {
public:
    unsigned char _pad00[0x70];
    class CDefClassBase* PDef;                     // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                          // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }

class CDefPointeeBase { public:
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void Release() { if (--m_refCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CActivateQuestDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long ScriptName;                                // +0x28 CDefString (string-table index)
    bool LoadResources;                             // +0x2c
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CActivateQuestDef>* out) const;   // 0x007B5680
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CActivateQuestDef>* out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CActivateQuestDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefStringTable {
    CCharString GetString(long index);              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                    // unaligned inside the action (+0xad)
    void* __vftable;
    void* m_pItem;
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};
#pragma pack(pop)
class CCreatureActionBase {                         // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                 // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_ActivateQuest : public CCreatureActionBase {
public:
    CCharString ScriptName;                         // +0xa8
    bool        LoadResources;                      // +0xac
    CBaseIntelligentPointer PObjectToUse;           // +0xad
    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing& objectToUse,
                                  const CCharString& scriptName, bool loadResources);   // 0x00843F50
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCActionUseBase : public CTCBase {
public:
    const void* PActionUseDef;                      // +0x0c
    bool Usable;                                    // +0x10
    virtual void OnActionUse(CThing& user);         // 0x007EAB90
};
class CTCActionUseActivateQuest : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};

void CTCActionUseActivateQuest::OnActionUse(CThing& user)
{
    CThingCreatureBase* creature = CastToCreature(&user);
    CTCActionUseBase::OnActionUse(user);
    CActivateQuestDef* def;
    {
        CDefPointer<CActivateQuestDef> out;
        ParentThing->PDef->GetSubDef(&out);
        def = out.Object;
    }
    if (!def)
        return;
    if (creature && !creature->IsDead()) {
        CCharString scriptName = g_DefStringTable.GetString(def->ScriptName);
        CThing* parent = ParentThing;
        creature->SetCurrentAction(CCreatureAction_ActivateQuest(*creature, *parent, scriptName, def->LoadResources));
    }
    if (--def->m_refCount == 0)
        def->Delete();
}

// ---- harness ----
static int g_base = 0, g_set = 0, g_ctor = 0, g_deleted = 0, g_ipDtor = 0, g_actDtor = 0, g_strDtor = 0;
static CThing* g_baseUser = 0;
static void* g_def = 0;
static const void* g_findOwner = 0;
static long g_strIndex = -1;
static const void* g_strDtorAddr[8];
static const void* g_ctorScript = 0;
static const void* g_ctorScriptData = 0;
static CThingCreatureBase* g_ctorCreature = 0;
static CThing* g_ctorObject = 0;
static bool g_ctorLoad = false;
static const CCreatureActionBase* g_setAction = 0;
static const void* g_actDtorAddr = 0;
static char g_strData[64];
CDefStringTable g_DefStringTable;
CCharString::~CCharString() { if (g_strDtor < 8) g_strDtorAddr[g_strDtor] = this; ++g_strDtor; }
CCharString CDefStringTable::GetString(long index) { g_strIndex = index; CCharString r; r.PStringData = g_strData + index; return r; }
void* Find_CDefClassBase_GetSubDef(const void* owner) { g_findOwner = owner; if (g_def) ++((CDefPointeeBase*)g_def)->m_refCount; return g_def; }
void CDefPointeeBase::Destroy() { }
void CDefPointeeBase::Delete() { ++g_deleted; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; }
CCreatureActionBase::~CCreatureActionBase() { ++g_actDtor; g_actDtorAddr = this; }
CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing& objectToUse,
                                                             const CCharString& scriptName, bool loadResources)
{
    ++g_ctor; g_ctorCreature = &creature; g_ctorObject = &objectToUse; g_ctorScript = &scriptName;
    g_ctorScriptData = scriptName.PStringData; g_ctorLoad = loadResources;
    ScriptName.PStringData = 0; LoadResources = loadResources; PObjectToUse.__vftable = 0; PObjectToUse.m_pItem = 0;
}
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) { ++g_set; g_setAction = &action; return true; }
CTCBase::~CTCBase() { }
void CTCActionUseBase::OnActionUse(CThing& user) { ++g_base; g_baseUser = &user; }

static void reset() { g_base = g_set = g_ctor = g_deleted = g_ipDtor = g_actDtor = g_strDtor = 0; g_baseUser = 0;
    g_findOwner = 0; g_strIndex = -1; g_ctorScript = 0; g_ctorScriptData = 0; g_ctorCreature = 0; g_ctorObject = 0;
    g_ctorLoad = false; g_setAction = 0; g_actDtorAddr = 0; memset(g_strDtorAddr, 0, sizeof g_strDtorAddr); }
static bool strDestroyed(const void* p) { for (int i = 0; i < g_strDtor && i < 8; ++i) if (g_strDtorAddr[i] == p) return true; return false; }

int main()
{
    CDefClassBase defClass;
    CThing parent; memset(&parent, 0, sizeof parent); parent.PDef = &defClass;
    CThingCreatureBase user; memset(&user, 0, sizeof user);
    CActivateQuestDef def; def.m_refCount = 1; def.ScriptName = 7; def.LoadResources = true;
    CTCActionUseActivateQuest tc; tc.ParentThing = &parent; tc.ToBeRemoved = false; tc.IsInGlobalUpdateTCs = false;
    tc.PActionUseDef = 0; tc.Usable = true;

    // no sub-def: base only
    reset(); g_def = 0;
    tc.OnActionUse(user);
    CHECK(g_base == 1 && g_baseUser == &user && g_findOwner == &defClass);
    CHECK(g_set == 0 && g_ctor == 0 && g_strDtor == 0 && g_deleted == 0);

    // live creature: action set, def released (refcount back to 1, not deleted)
    reset(); g_def = &def;
    tc.OnActionUse(user);
    CHECK(g_base == 1 && g_baseUser == &user);
    CHECK(g_strIndex == 7 && g_ctor == 1 && g_ctorCreature == &user && g_ctorObject == &parent);
    CHECK(g_ctorScriptData == g_strData + 7 && g_ctorLoad);
    CHECK(g_set == 1 && g_setAction != 0 && g_actDtor == 1 && g_actDtorAddr == g_setAction && g_ipDtor == 1);
    CHECK(strDestroyed(g_ctorScript));
    CHECK(def.m_refCount == 1 && g_deleted == 0);

    // LoadResources false passes through
    reset(); def.LoadResources = false;
    tc.OnActionUse(user);
    CHECK(g_set == 1 && !g_ctorLoad && def.m_refCount == 1);

    // dead creature: no action, def still released
    reset(); user.Flags91 = 1;
    tc.OnActionUse(user);
    CHECK(g_base == 1 && g_set == 0 && g_ctor == 0 && g_strIndex == -1 && def.m_refCount == 1 && g_deleted == 0);
    user.Flags91 = 0;

    // last reference: the def is deleted after the action
    reset(); def.m_refCount = 0;
    tc.OnActionUse(user);
    CHECK(g_set == 1 && def.m_refCount == 0 && g_deleted == 1);

    if (fails == 0) printf("007ef600_TEST PASS\n");
    return fails;
}