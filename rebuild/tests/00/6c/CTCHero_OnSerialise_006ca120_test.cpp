// 006ca120 behaviour test: CTCHero::OnSerialise (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __int64 long long
#endif
#include <stdio.h>
#include <string.h>

static int g_strings;   // live CCharString count
class CCharString {
public:
    char Buf[64];
    CCharString() { Buf[0] = 0; ++g_strings; }
    CCharString(const char* s, long = -1) { strcpy(Buf, s); ++g_strings; }
    CCharString(const CCharString& o) { strcpy(Buf, o.Buf); ++g_strings; }
    ~CCharString() { --g_strings; }
    CCharString& operator=(const CCharString& o) { strcpy(Buf, o.Buf); return *this; }
};
inline CCharString EmptyString()
{
    CCharString empty;
    return empty;
}
class CDefString {
public:
    long Index;
    CDefString() : Index(0) {}
};
static const char* g_names[4] = { "TITLE_NONE", "TITLE_HERO", "TITLE_ARCHMAGE", "TITLE_CHICKEN" };
class CDefStringTable {
public:
    CCharString GetString(long index) { return CCharString(g_names[index]); }
};
static CDefStringTable g_DefStringTable;

static int g_destroys;
class CDefClassBase {
public:
    virtual void v00() {}
    virtual void Destroy() const { ++g_destroys; }
    mutable long RefCount;
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CDefinition : public CDefClassBase {
public:
    long NameIndex;
    virtual CDefString GetDefName() const { CDefString s; s.Index = NameIndex; return s; }
};
class CHeroTitleDef : public CDefinition { };
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }   // test model: VC7.1 /Od copies the return
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
    CDefPointer& operator=(T* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
};
static CHeroTitleDef g_defs[4];
class CDefinitionManager {
public:
    CDefPointer<const CDefinition> GetPDefFromGlobalIndex(long index) const
    {
        CDefPointer<const CDefinition> p;
        if (index > 0 && index < 4) { p.Object = &g_defs[index]; ++g_defs[index].RefCount; }
        return p;
    }
    long GetDefGlobalIndexFromName(const CCharString& name) const
    {
        for (long i = 1; i < 4; ++i) if (!strcmp(g_names[i], name.Buf)) return i;
        return 0;
    }
};
static CDefinitionManager g_defMgr;

class CThing {
public:
    unsigned __int64 UID;
    short LevelIndex;
    unsigned char Flags91;
    bool LevelLess;
    bool IsLevelLess() const { return LevelLess; }
};
static int g_remapCalls;
static bool g_lastSaving, g_lastLevelLess; static long g_lastLevel;
class CThingManager {
public:
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid)
    {
        ++g_remapCalls; g_lastSaving = saving; g_lastLevelLess = levelLess; g_lastLevel = level;
        return saving ? uid + 0x100000000 : uid - 0x100000000;
    }
};
static CThingManager g_thingMgr;
class CBaseIntelligentPointer {
public:
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};

class CPersistContext {
public:
    long Mode;
    unsigned __int64 SavedID;
    CCharString SavedName;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue)
    {
        if (strcmp(name, "LastWeaponEquippedID")) return;
        if (IsSaving()) SavedID = value; else value = SavedID ? SavedID : defaultValue;
    }
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue)
    {
        if (strcmp(name, "hero_title_object_def_name")) return;
        if (IsSaving()) SavedName = value; else value = SavedName.Buf[0] ? SavedName : defaultValue;
    }
};

class CTCBase {
public:
    virtual void v00() {}
    CThing* Thing;
    const CDefinitionManager* GetDefinitions() const { return &g_defMgr; }
    CThingManager* GetThingManager() const { return &g_thingMgr; }
};
class CTCHero : public CTCBase {
public:
    long HeroTitleObjectDef;
    CDefPointer<const CHeroTitleDef> PHeroTitleSubDef;
    CBaseIntelligentPointer LastWeaponEquipped;
    unsigned __int64 LastWeaponEquippedID;
    void OnSerialise(CPersistContext* context, long version);
};

void CTCHero::OnSerialise(CPersistContext* context, long)
{
    if (context->IsSaving()) {
        LastWeaponEquippedID = 0;
        if (LastWeaponEquipped.GetPItem() && !(LastWeaponEquipped.GetPItem()->Flags91 & 1)) {
            CThing* weapon = LastWeaponEquipped.GetPItem();
            unsigned __int64 uid = weapon->UID;
            LastWeaponEquippedID = GetThingManager()->RemapUIDForSerialisation(true, Thing->IsLevelLess(), Thing->LevelIndex, uid);
        }
        context->Transfer("LastWeaponEquippedID", LastWeaponEquippedID, 0);
        CCharString defName("");
        CDefPointer<const CDefinition> def = GetDefinitions()->GetPDefFromGlobalIndex(HeroTitleObjectDef);
        if (def.Object)
        {
            CCharString name = g_DefStringTable.GetString(def->GetDefName().Index);
            defName = name;
        }
        context->Transfer("hero_title_object_def_name", defName, EmptyString());
    } else {
        context->Transfer("LastWeaponEquippedID", LastWeaponEquippedID, 0);
        if (LastWeaponEquippedID)
            LastWeaponEquippedID = GetThingManager()->RemapUIDForSerialisation(false, Thing->IsLevelLess(), Thing->LevelIndex, LastWeaponEquippedID);
        CCharString defName("");
        context->Transfer("hero_title_object_def_name", defName, EmptyString());
        HeroTitleObjectDef = GetDefinitions()->GetDefGlobalIndexFromName(defName);
        PHeroTitleSubDef = 0;
    }
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }
static bool RefsBalanced() { for (int i = 0; i < 4; ++i) if (g_defs[i].RefCount != 1) return false; return g_destroys == 0; }

int main()
{
    for (int i = 0; i < 4; ++i) { g_defs[i].RefCount = 1; g_defs[i].NameIndex = i; }
    CThing self; self.UID = 1; self.LevelIndex = 7; self.Flags91 = 0; self.LevelLess = true;
    CThing sword; sword.UID = 0x0000000500000042; sword.LevelIndex = 3; sword.Flags91 = 0; sword.LevelLess = false;
    CTCHero hero; hero.Thing = &self; hero.HeroTitleObjectDef = 2; hero.LastWeaponEquipped.Item = &sword; hero.LastWeaponEquippedID = 99;
    CPersistContext save; save.Mode = 1; save.SavedID = 0;

    // save: weapon UID remapped with the hero thing's level data, title def saved by name
    hero.OnSerialise(&save, 0);
    Check("save id", save.SavedID == 0x0000000600000042 && hero.LastWeaponEquippedID == 0x0000000600000042);
    Check("save args", g_remapCalls == 1 && g_lastSaving && g_lastLevelLess && g_lastLevel == 7);
    Check("save name", !strcmp(save.SavedName.Buf, "TITLE_ARCHMAGE") && RefsBalanced());

    // save mode 3 with a non-saved weapon and no title def: id 0, empty name
    sword.Flags91 = 1; hero.HeroTitleObjectDef = 0;
    CPersistContext save3; save3.Mode = 3; save3.SavedID = 5;
    hero.OnSerialise(&save3, 0);
    Check("unsaved weapon", save3.SavedID == 0 && hero.LastWeaponEquippedID == 0 && g_remapCalls == 1);
    Check("no title", save3.SavedName.Buf[0] == 0 && RefsBalanced());
    // no weapon at all
    hero.LastWeaponEquipped.Item = 0; hero.LastWeaponEquippedID = 77;
    hero.OnSerialise(&save3, 0);
    Check("no weapon", save3.SavedID == 0 && g_remapCalls == 1);

    // load: id remapped back, title index resolved from the name, sub-def cleared
    CPersistContext load; load.Mode = 2; load.SavedID = 0x0000000600000042; load.SavedName = CCharString("TITLE_CHICKEN");
    g_defs[1].RefCount = 2; hero.PHeroTitleSubDef.Object = &g_defs[1];
    self.LevelLess = false; self.LevelIndex = 4;
    hero.OnSerialise(&load, 0);
    Check("load id", hero.LastWeaponEquippedID == 0x0000000500000042 && g_remapCalls == 2 && !g_lastSaving && !g_lastLevelLess && g_lastLevel == 4);
    Check("load title", hero.HeroTitleObjectDef == 3 && hero.PHeroTitleSubDef.Object == 0 && RefsBalanced());
    // load with a zero id: no remap, unknown name -> index 0
    CPersistContext load0; load0.Mode = 0; load0.SavedID = 0; load0.SavedName = CCharString("NOPE");
    hero.LastWeaponEquippedID = 0;
    hero.OnSerialise(&load0, 0);
    Check("load zero", hero.LastWeaponEquippedID == 0 && g_remapCalls == 2 && hero.HeroTitleObjectDef == 0);
    Check("strings", g_strings == 4);   // the four contexts' SavedName members
    if (g_fail) return 1;
    printf("006ca120_TEST PASS\n");
    return 0;
}