// Behaviour test for 00768010: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCObjectAugmentations::OnInitialActivate() -- on a fresh (not save-loaded) object, put each of the def's initial augmentations into a free slot; then attach the augmentation particles and set up the sounds
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
#ifndef _MSC_VER
#define __fastcall
#endif
class CAugmentationDef : public CDefClassBase { public: enum { Slot = 1 }; unsigned long Name; float DamageMultiplier; };
template <class T> class CSlots { public:
    T Data[4]; unsigned int Count;
    unsigned int size() const { return Count; }
    T& operator[](unsigned int i) { return Data[i]; }
    const T& operator[](unsigned int i) const { return Data[i]; }
};
namespace std { template <class T> class vector : public CSlots<T> { }; }
class CObjectAugmentationsDef : public CDefClassBase { public: enum { Slot = 2 }; std::vector<long> InitialAugmentations; };
class CTCObjectAugmentations { public:
    CDefPointer<const CObjectAugmentationsDef> PDef; bool LoadedFromSaveGame;
    CSlots<CDefPointer<const CThingBaseDef> > Augmentations; int Particles, Sounds;
    void AttachAugmentationParticles() { ++Particles; }
    void SetupAugmentationSounds() { ++Sounds; }
    long GetFreeSlot() const;
    void OnInitialActivate();
    static float __fastcall GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations);
};
// ---- row body (verbatim) ----
long CTCObjectAugmentations::GetFreeSlot() const
{
    for (unsigned int i = 0; i < Augmentations.size(); ++i) {
        if (Augmentations[i].IsNull())
            return i;
    }
    return -1;
}

void CTCObjectAugmentations::OnInitialActivate()
{
    if (!LoadedFromSaveGame) {
        for (unsigned int i = 0; i < PDef->InitialAugmentations.size(); ++i) {
            long slot = GetFreeSlot();
            if (slot >= 0) {
                if (PDef->InitialAugmentations[i] > 0)
                    CGameDefinitionManager::Get().GetDef(PDef->InitialAugmentations[i], &Augmentations[slot]);
            }
        }
    }
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef a, b; g_manager.Table[3] = &a; g_manager.Table[4] = &b;
    CObjectAugmentationsDef def; def.InitialAugmentations.Count = 3;
    def.InitialAugmentations[0] = 3; def.InitialAugmentations[1] = 0; def.InitialAugmentations[2] = 4;
    CTCObjectAugmentations c; c.PDef.Set(&def); c.LoadedFromSaveGame = false; c.Augmentations.Count = 2; c.Particles = c.Sounds = 0;
    c.OnInitialActivate();
    CHECK(c.Augmentations[0].Object == &a && c.Augmentations[1].Object == &b && a.Refs == 2 && b.Refs == 2);
    CHECK(c.Particles == 1 && c.Sounds == 1 && g_manager.Calls == 2);
    c.OnInitialActivate();                                     // no free slot left: nothing placed
    CHECK(g_manager.Calls == 2 && c.Particles == 2 && c.Sounds == 2);
    CTCObjectAugmentations d; d.PDef.Set(&def); d.LoadedFromSaveGame = true; d.Augmentations.Count = 2; d.Particles = d.Sounds = 0;
    d.OnInitialActivate();
    CHECK(d.Augmentations[0].IsNull() && d.Particles == 1 && d.Sounds == 1 && g_manager.Calls == 2);
    if (fails == 0) printf("00768010_TEST PASS\n");
    return fails;
}
