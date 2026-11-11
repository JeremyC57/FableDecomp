// Behaviour test for 007661c0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations) -- product of the DamageMultiplier of every augmentation def named by a positive def index (1.0 with none)
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
float __fastcall CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations)
{
    float multiplier = 1.0f;
    for (unsigned int i = 0; i < augmentations.size(); ++i) {
        if (augmentations[i] > 0) {
            CDefPointer<const CThingBaseDef> def;
            CGameDefinitionManager::Get().GetDef(augmentations[i], &def);
            CDefPointer<const CAugmentationDef> augmentation;
            def->GetSubDef(&augmentation);
            multiplier *= augmentation->DamageMultiplier;
        }
    }
    return multiplier;
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef a, b; CAugmentationDef aa, ab; a.Sub[1] = &aa; b.Sub[1] = &ab; aa.DamageMultiplier = 2.0f; ab.DamageMultiplier = 1.5f;
    g_manager.Table[3] = &a; g_manager.Table[4] = &b;
    std::vector<long> set; set.Count = 0;
    CHECK(CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(set) == 1.0f);
    set.Count = 4; set[0] = 3; set[1] = 0; set[2] = 4; set[3] = -1;
    float m = CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(set);
    CHECK(m > 2.999f && m < 3.001f && g_manager.Calls == 2);
    CHECK(a.Refs == 1 && b.Refs == 1 && aa.Refs == 1 && ab.Refs == 1 && aa.Deleted == 0);
    if (fails == 0) printf("007661c0_TEST PASS\n");
    return fails;
}
