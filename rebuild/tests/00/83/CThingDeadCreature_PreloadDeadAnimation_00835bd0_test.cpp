// Behaviour test for 00835bd0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CThingDeadCreature::PreloadDeadAnimation() -- run the dead-creature animation preloader over every entry of the appearance definition's animation set, resolving the matching entries through the world's main mesh bank
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixture models of the shared-header interfaces: a counted def (Refs counts references, Delete counted), the counted CDefPointer
// handle (copy / assignment add-ref, release on destruction), the definition manager's GetDef<T> (fills the handle with an add-ref,
// returns whether found) and CDefClassBase::GetSubDef<T> (the sub-def table of the def, by a type slot of T).
class CDefPointeeBase { public:
    long Refs; int Deleted; CDefPointeeBase* Sub[8];
    CDefPointeeBase() : Refs(1), Deleted(0) { memset(Sub, 0, sizeof(Sub)); }
    void Delete() { ++Deleted; }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { Release(); }
    CDefPointer& operator=(const CDefPointer& o) { if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->Refs; } return *this; }
    void Release() { if (Object) { if (--Object->Refs == 0) Object->Delete(); Object = 0; } }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    T* Get() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Set(CDefPointeeBase* p) { if (p) ++p->Refs; Release(); Object = p; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const
    {
        CDefPointeeBase* p = Sub[T::Slot];
        if (!p) return false;
        out->Set(p); return true;
    }
};
class CGameDefinitionManager { public:
    CDefPointeeBase* Table[16]; mutable long Calls; mutable long LastIndex;
    static CGameDefinitionManager& Get();
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        ++Calls; LastIndex = index;
        if (index > 0 && index < 16 && Table[index]) { out->Set(Table[index]); return true; }
        return false;
    }
};
static CGameDefinitionManager g_manager;
CGameDefinitionManager& CGameDefinitionManager::Get() { return g_manager; }
static void ResetManager() { memset(&g_manager, 0, sizeof(g_manager)); }
class CThingBaseDef : public CDefClassBase { public: enum { Slot = 0 }; };
#include <vector>
#include <utility>
#include <algorithm>
class CAnimationEntry { public: long BankIndex; const char* Name; };
class CAnimationSet { public: typedef std::vector<std::pair<unsigned long, CAnimationEntry> > TAnimMap; TAnimMap Anims; };
class CAppearanceDef : public CDefClassBase { public: enum { Slot = 1 }; CAnimationSet Animation; };
class CMeshDataBank { public: std::vector<long> Loaded; };
class CWorld { public: CMeshDataBank* Bank; const CMeshDataBank* PeekMeshBank() const { return Bank; } };
static CWorld g_world;
class CDeadCreatureAnimPreloader { public:
    CDeadCreatureAnimPreloader(const CMeshDataBank& mesh_bank) : MeshBank(&mesh_bank) {}
    void operator()(const std::pair<unsigned long, CAnimationEntry>& entry) const
    {
        if (strcmp(entry.second.Name, "STANDARD_DEAD") == 0) const_cast<CMeshDataBank*>(MeshBank)->Loaded.push_back(entry.second.BankIndex);
    }
    const CMeshDataBank* MeshBank;
};
class CThing { public:
    CDefPointer<const CThingBaseDef> PBaseDef;
    const CWorld& PeekWorld() const { return g_world; }
};
class CThingDeadCreature : public CThing { public: void PreloadDeadAnimation(); };
// ---- row body (verbatim) ----
void CThingDeadCreature::PreloadDeadAnimation()
{
    const CMeshDataBank& mesh_bank = *PeekWorld().PeekMeshBank();
    CDefPointer<const CAppearanceDef> appearance;
    PBaseDef->GetSubDef(&appearance);
    const CAnimationSet& animations = appearance->Animation;
    std::for_each(animations.Anims.begin(), animations.Anims.end(), CDeadCreatureAnimPreloader(mesh_bank));
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef thingDef; CAppearanceDef app; thingDef.Sub[1] = &app;
    const char* names[4] = { "DEFAULT", "STANDARD_DEAD", "WALK", "STANDARD_DEAD" };
    for (int i = 0; i < 4; ++i) { CAnimationEntry e = { 40 + i, names[i] }; app.Animation.Anims.push_back(std::make_pair((unsigned long)i, e)); }
    CMeshDataBank bank; g_world.Bank = &bank;
    CThingDeadCreature corpse; corpse.PBaseDef.Object = &thingDef;
    corpse.PreloadDeadAnimation();
    CHECK(bank.Loaded.size() == 2 && bank.Loaded[0] == 41 && bank.Loaded[1] == 43);
    CHECK(app.Refs == 1 && app.Deleted == 0 && thingDef.Refs == 1);
    corpse.PBaseDef.Object = 0;
    if (fails == 0) printf("00835bd0_TEST PASS\n");
    return fails;
}
