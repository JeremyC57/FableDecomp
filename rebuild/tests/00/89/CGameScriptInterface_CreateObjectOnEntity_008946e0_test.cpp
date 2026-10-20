// Behaviour test for 008946e0: body copied from src/008946e0.cpp (CGameScriptInterface::CreateObjectOnEntity), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CCharString {
    void* PStringData;
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct CBaseClass {
    CBaseClass() {}
    virtual ~CBaseClass() {}
};
static int g_impDeleted = 0;
struct CCPPointerInfo {
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
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CThing;
static bool g_alive = false; static C3DVector g_entityPos = { 1, 2, 3 }; static CThing* g_entityThing = 0; static int g_assigns = 0;
struct CScriptThing : CBaseClass {
    CCountedScriptImp PImp;
    CScriptThing() {}
    virtual ~CScriptThing() {}
    virtual const C3DVector& GetPosition() const { return g_entityPos; }
    virtual CThing* GetThing() const { return g_entityThing; }
    virtual bool IsAlive() const { return g_alive; }
    CScriptThing& operator=(const CScriptThing& other) { ++g_assigns; CCountedScriptImp copy(other.PImp); PImp.~CCountedScriptImp(); PImp.Data = copy.Data; PImp.Info = copy.Info; if (PImp.Info) ++PImp.Info->RefCount; return *this; }
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
static int g_defDestroyed = 0;
struct CThingObjectDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_defDestroyed; }
    long RefCount;
};
struct CDefPointerThingObjectDef {
    CThingObjectDef* Object;
    CDefPointerThingObjectDef() : Object(0) {}
    ~CDefPointerThingObjectDef() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
};
static CThingObjectDef* g_def = 0; static int g_getDef = 0; static long g_getDefIndex = -1; static int g_nameLookups = 0;
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) { ++g_nameLookups; CHECK(strcmp((const char*)name.PStringData, "OBJECT_X") == 0); return 456; }
    bool GetDef(long globalIndex, CDefPointerThingObjectDef* out) { ++g_getDef; g_getDefIndex = globalIndex; if (!g_def) return false; ++g_def->RefCount; out->Object = g_def; return true; }
};
static int g_attached = 0; static void* g_attachedWith = 0;
struct CAttachmentHolder {
    virtual void Attach(void* attachment) { ++g_attached; g_attachedWith = attachment; }
};
static void* g_attachment = (void*)0x7777;
struct CThingVirtuals {
    virtual void* GetAttachment() { return g_attachment; }
};
static void __fastcall DeleteImp(void*) { ++g_impDeleted; }
static CCPPointerInfo g_info = { 1, DeleteImp, (void*)0x5555 };
static int g_getScript = 0;
struct CThing {
    unsigned char _pad00[0x60];
    CAttachmentHolder* Attachments;
    CThingVirtuals* GetTargetAttachments() const { return (CThingVirtuals*)Attachments; }
    CScriptThing GetScriptThing() { ++g_getScript; CScriptThing s; s.PImp.Data = (void*)0x5555; s.PImp.Info = &g_info; ++g_info.RefCount; return s; }
};
static CThing* g_object = 0; static int g_creates = 0; static long g_createDef = -1; static const C3DVector* g_createPos = 0; static long g_createPlayer = -1; static const char* g_createName = 0;
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* orientation, unsigned long uid, const CCharString& scriptName)
    { ++g_creates; g_createDef = defIndex; g_createPos = &position; g_createPlayer = player; g_createName = (const char*)scriptName.PStringData; CHECK(orientation == 0 && uid == 0); return (CThingObject*)g_object; }
};
struct CGameScriptInterface {
    void*                   _vptr;
    void*                   World;
    void*                   Component;
    void*                   DisplayEngine;
    CGameDefinitionManager* DefinitionManager;
    CScriptThing CreateObjectOnEntity(const CCharString& defName, const CScriptThing& entity, const CCharString& scriptName) const;
};
CScriptThing CGameScriptInterface::CreateObjectOnEntity(const CCharString& defName, const CScriptThing& entity,
                                                        const CCharString& scriptName) const
{
    CScriptThing result;
    if (entity.IsAlive()) {
        long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
        CDefPointerThingObjectDef def;
        DefinitionManager->GetDef(index, &def);
        CThingObject* object = CThingObject::Create(index, entity.GetPosition(), 4, 0, 0, scriptName);
        if (object) {
            CAttachmentHolder* holder = object->Attachments;
            if (holder) {
                CThingVirtuals* target = entity.GetThing()->GetTargetAttachments();
                if (target)
                    holder->Attach(target->GetAttachment());
            }
            result = object->GetScriptThing();
        }
    }
    return result;
}
int main()
{
    static CGameDefinitionManager manager; static CThingObjectDef def; def.RefCount = 1; g_def = &def;
    static CThingObject object; static CThing entityThing; static CAttachmentHolder holder; static CThingVirtuals target;
    memset(&object, 0, sizeof(object)); memset(&entityThing, 0, sizeof(entityThing)); g_entityThing = &entityThing;
    CGameScriptInterface gsi; memset(&gsi, 0, sizeof(gsi)); gsi.DefinitionManager = &manager;
    CCharString defName = { (void*)"OBJECT_X" }; CCharString scriptName = { (void*)"Script1" }; CScriptThing entity;
    // dead entity: the null script thing, no lookups
    g_alive = false;
    { CScriptThing r = gsi.CreateObjectOnEntity(defName, entity, scriptName); CHECK(r.PImp.Data == 0 && r.PImp.Info == 0 && g_nameLookups == 0 && g_creates == 0); }
    // alive, creation fails: still null
    g_alive = true; g_object = 0;
    { CScriptThing r = gsi.CreateObjectOnEntity(defName, entity, scriptName); CHECK(g_nameLookups == 1 && g_getDef == 1 && g_getDefIndex == 456); CHECK(g_creates == 1 && g_createDef == 456 && g_createPos == &g_entityPos && g_createPlayer == 4 && g_createName == (const char*)scriptName.PStringData); CHECK(r.PImp.Info == 0 && g_assigns == 0); }
    // created without an attachment holder: the script thing is returned, nothing attached
    g_object = &object;
    { CScriptThing r = gsi.CreateObjectOnEntity(defName, entity, scriptName); CHECK(g_getScript == 1 && g_assigns == 1 && r.PImp.Info == &g_info && g_attached == 0); }
    CHECK(g_info.RefCount == 1);
    // holder present, entity has no attachments: nothing attached
    object.Attachments = &holder; entityThing.Attachments = 0;
    { CScriptThing r = gsi.CreateObjectOnEntity(defName, entity, scriptName); CHECK(g_attached == 0 && g_getScript == 2); }
    // both present: attached with the entity's attachment
    entityThing.Attachments = (CAttachmentHolder*)&target;
    { CScriptThing r = gsi.CreateObjectOnEntity(defName, entity, scriptName); CHECK(g_attached == 1 && g_attachedWith == g_attachment && g_getScript == 3); }
    CHECK(def.RefCount == 1 && g_defDestroyed == 0);
    // a def held only by the lookup is destroyed
    def.RefCount = 0; { CScriptThing r = gsi.CreateObjectOnEntity(defName, entity, scriptName); } CHECK(g_defDestroyed == 1);
    if (fails == 0) printf("008946e0_TEST PASS\n"); else printf("008946e0_TEST FAIL\n");
    return fails;
}