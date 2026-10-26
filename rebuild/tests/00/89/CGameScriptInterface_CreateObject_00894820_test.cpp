// Behaviour test for 00894820: body copied from src/00894820.cpp (CGameScriptInterface::CreateObject), callees stubbed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCharString { public:
    void* PStringData;
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class CBaseClass { public:
    CBaseClass() {}
    virtual ~CBaseClass() {}
};
static int g_impDeleted = 0, g_infoDeleted = 0;
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ++g_infoDeleted;
            }
        }
        Data = 0;
        Info = 0;
    }
};
static int g_assigns = 0;
class CScriptThing : public CBaseClass { public:
    CCountedScriptImp PImp;
    CScriptThing() {}
    virtual ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other) { ++g_assigns; CCountedScriptImp copy(other.PImp); PImp.~CCountedScriptImp(); PImp.Data = copy.Data; PImp.Info = copy.Info; if (PImp.Info) ++PImp.Info->RefCount; return *this; }
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
static int g_defDestroyed = 0;
class CThingObjectDef { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_defDestroyed; }
    long RefCount;
};
struct CDefPointerThingObjectDef {
    CThingObjectDef* Object;
    CDefPointerThingObjectDef() : Object(0) {}
    ~CDefPointerThingObjectDef() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
};
static CThingObjectDef* g_def = 0; static int g_getDef = 0; static long g_getDefIndex = -1; static int g_nameLookups = 0; static const char* g_lookedUp = 0;
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) { ++g_nameLookups; g_lookedUp = (const char*)name.PStringData; return 321; }
    bool GetDef(long globalIndex, CDefPointerThingObjectDef* out) { ++g_getDef; g_getDefIndex = globalIndex; if (!g_def) return false; ++g_def->RefCount; out->Object = g_def; return true; }
};
static void __fastcall DeleteImp(void*) { ++g_impDeleted; }
static CCPPointerInfo g_info = { 1, DeleteImp, (void*)0x5555 };
static int g_getScript = 0;
class CThing { public:
    CScriptThing GetScriptThing() { ++g_getScript; CScriptThing s; s.PImp.Data = (void*)0x5555; s.PImp.Info = &g_info; ++g_info.RefCount; return s; }
};
static CThing* g_object = 0; static int g_creates = 0; static long g_createDef = -1; static const C3DVector* g_createPos = 0; static long g_createPlayer = -1; static const char* g_createName = 0;
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* orientation, unsigned long uid, const CCharString& scriptName)
    { ++g_creates; g_createDef = defIndex; g_createPos = &position; g_createPlayer = player; g_createName = (const char*)scriptName.PStringData; CHECK(orientation == 0 && uid == 0); return (CThingObject*)g_object; }
};
class CGameScriptInterface { public:
    void*                   _vptr;
    void*                   World;
    void*                   Component;
    void*                   DisplayEngine;
    CGameDefinitionManager* DefinitionManager;
    CScriptThing CreateObject(const CCharString& defName, const C3DVector& position, const CCharString& scriptName) const;
};

CScriptThing CGameScriptInterface::CreateObject(const CCharString& defName, const C3DVector& position,
                                                const CCharString& scriptName) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CDefPointerThingObjectDef def;
    DefinitionManager->GetDef(index, &def);
    CThingObject* object = CThingObject::Create(index, position, 4, 0, 0, scriptName);
    CScriptThing result;
    if (object)
        result = object->GetScriptThing();
    return result;
}
int main()
{
    static CGameDefinitionManager manager; static CThingObjectDef def; def.RefCount = 1; g_def = &def; static CThingObject object;
    CGameScriptInterface gsi; memset(&gsi, 0, sizeof(gsi)); gsi.DefinitionManager = &manager;
    CCharString defName = { (void*)"OBJECT_BARREL" }; CCharString scriptName = { (void*)"Barrel1" }; C3DVector pos = { 4, 5, 6 };
    g_object = &object;
    {
        CScriptThing r = gsi.CreateObject(defName, pos, scriptName);
        CHECK(g_nameLookups == 1 && g_lookedUp == (const char*)defName.PStringData); CHECK(g_getDef == 1 && g_getDefIndex == 321);
        CHECK(g_creates == 1 && g_createDef == 321 && g_createPos == &pos && g_createPlayer == 4 && g_createName == (const char*)scriptName.PStringData);
        CHECK(g_getScript == 1 && g_assigns == 1 && r.PImp.Data == (void*)0x5555 && r.PImp.Info == &g_info);
        CHECK(def.RefCount == 1 && g_defDestroyed == 0);
    }
    CHECK(g_info.RefCount == 1 && g_impDeleted == 0);
    // creation fails: an empty script thing; a def held only by the lookup is destroyed
    g_object = 0; def.RefCount = 0;
    { CScriptThing r = gsi.CreateObject(defName, pos, scriptName); CHECK(r.PImp.Data == 0 && r.PImp.Info == 0 && g_getScript == 1 && g_assigns == 1); }
    CHECK(g_defDestroyed == 1 && g_creates == 2);
    if (fails == 0) printf("00894820_TEST PASS\n"); else printf("00894820_TEST FAIL\n");
    return fails;
}