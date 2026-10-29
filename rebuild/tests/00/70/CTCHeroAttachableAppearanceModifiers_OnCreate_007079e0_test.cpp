// Behaviour test for 007079e0: body copied from src/007079e0.cpp (CTCHeroAttachableAppearanceModifiers::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCtor = 0, g_strDtor = 0; static const char* g_strText = 0;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length) : PStringData((void*)text) { ++g_strCtor; g_strText = text; CHECK(length == -1); }
    ~CCharString() { ++g_strDtor; }
};
static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const long& operator[](unsigned long n) const { return _M_start[n]; }
};
class CCreatureDef : public CDefBase { public:
    unsigned char _pad_0x08[0x4C];
    CLongVector   InitialAppearanceModifiers;
};
template <class T> class CDefPointer { public:
    T* Object;
};
template <class T> struct CDefHolder {
    T* m_p;
    CDefHolder(T* p) : m_p(p) {}
    ~CDefHolder() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
};
static CCreatureDef* g_creatureDef = 0; static int g_getSubDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CCreatureDef>* out)
    {
        ++g_getSubDef;
        if (!g_creatureDef) return false;
        ++g_creatureDef->m_count; out->Object = g_creatureDef; return true;
    }
};
static int g_nameLookups = 0;
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) { ++g_nameLookups; CHECK(strcmp((const char*)name.PStringData, "OBJECT_HERO_HORNS") == 0); return 999; }
};
static CDefinitionManager g_manager;
static int g_setChanged = 0; static unsigned long g_changedFlags = 0;
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAsChanged(unsigned long flags) { ++g_setChanged; g_changedFlags = flags; }
};
}
static NGraphicAppearance::CTCGraphicAppearance g_graphic;
class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    unsigned char _pad68[0x08];
    CDefClassBase* PDef;
};
static long g_frame = 1234;
class CWorld { public:
    static long __fastcall GetFrame() { return g_frame; }
};
static CWorld g_world;
class CTCBase { public:
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CWorld* GetWorld() const { return &g_world; }
    CDefinitionManager* GetDefinitionManager() const { return &g_manager; }
};
static int g_added = 0; static long g_addedIndex[8]; static int g_hidMods = 0, g_hidMats = 0, g_gfx = 0;
class CTCHeroAttachableAppearanceModifiers : public CTCBase { public:
    void*         __vftable_0c;
    unsigned long FrameLastChanged;
    bool Changed : 1;
    bool RecreatePrimitives : 1;
    bool AppearanceModifiersLoaded : 1;
    void AddAppearanceModifier(long defIndex) { if (g_added < 8) g_addedIndex[g_added] = defIndex; ++g_added; }
    void UpdateHiddenModifiers() { ++g_hidMods; CHECK(!Changed); }
    void UpdateHiddenMaterials() { ++g_hidMats; }
    void UpdateGraphics() { ++g_gfx; }
    void UploadChangesToGraphics()
    {
        if (Changed) {
            Changed = false;
            UpdateHiddenModifiers();
            UpdateHiddenMaterials();
            UpdateGraphics();
            ParentThing->PTCGraphicAppearance->SetAsChanged(15);
        }
    }
    void OnCreate();
};

void CTCHeroAttachableAppearanceModifiers::OnCreate()
{
    if (!AppearanceModifiersLoaded) {
        CDefClassBase* thingDef = ParentThing->PDef;
        CDefPointer<CCreatureDef> out;
        out.Object = 0;
        bool found = thingDef->GetSubDef(&out);
        CDefHolder<CCreatureDef> def(out.Object);
        if (found) {
            for (unsigned long i = 0; i < def->InitialAppearanceModifiers.size(); ++i)
                AddAppearanceModifier(def->InitialAppearanceModifiers[i]);
        }
    }
    {
        CCharString horns("OBJECT_HERO_HORNS", -1);
        AddAppearanceModifier(GetDefinitionManager()->GetDefGlobalIndexFromName(horns));
    }
    Changed = true;
    UploadChangesToGraphics();
    FrameLastChanged = GetWorld()->GetFrame();
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CCreatureDef creature; creature.m_count = 1; g_creatureDef = &creature;
    long mods[3] = { 11, 22, 33 }; creature.InitialAppearanceModifiers._M_start = mods; creature.InitialAppearanceModifiers._M_finish = mods + 3;
    thing.PTCGraphicAppearance = &g_graphic; thing.PDef = &classDef;
    CTCHeroAttachableAppearanceModifiers m; memset(&m, 0, sizeof(m)); m.ParentThing = &thing; m.RecreatePrimitives = true;
    // not yet loaded: the three def modifiers, then the horns; the change is uploaded and the frame stamped
    m.OnCreate();
    CHECK(g_getSubDef == 1 && g_added == 4); CHECK(g_addedIndex[0] == 11 && g_addedIndex[1] == 22 && g_addedIndex[2] == 33 && g_addedIndex[3] == 999);
    CHECK(g_strCtor == 1 && g_strDtor == 1 && g_nameLookups == 1); CHECK(!m.Changed && m.RecreatePrimitives && g_hidMods == 1 && g_hidMats == 1 && g_gfx == 1 && g_setChanged == 1 && g_changedFlags == 15);
    CHECK(m.FrameLastChanged == 1234); CHECK(creature.m_count == 1 && g_destroyed == 0);
    // already loaded: only the horns
    m.AppearanceModifiersLoaded = true; g_frame = 5; m.OnCreate(); CHECK(g_getSubDef == 1 && g_added == 5 && g_addedIndex[4] == 999 && m.FrameLastChanged == 5 && g_setChanged == 2);
    // no creature sub-def: only the horns, nothing released
    m.AppearanceModifiersLoaded = false; g_creatureDef = 0; m.OnCreate(); CHECK(g_getSubDef == 2 && g_added == 6 && g_destroyed == 0);
    // a def held only by the lookup is destroyed by the holder
    g_creatureDef = &creature; creature.m_count = 0; creature.InitialAppearanceModifiers._M_finish = mods; m.OnCreate(); CHECK(g_added == 7 && g_destroyed == 1);
    if (fails == 0) printf("007079e0_TEST PASS\n"); else printf("007079e0_TEST FAIL\n");
    return fails;
}