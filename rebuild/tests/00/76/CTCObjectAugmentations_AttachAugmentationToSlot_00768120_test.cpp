// Behaviour test for 00768120: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCObjectAugmentations::AttachAugmentationToSlot(long defIndex, long slot) -- fetch the thing def for the global index; when it carries an augmentation sub-def, store it in the slot; then re-attach the augmentation particles and sounds
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
class CAugmentationDef : public CDefClassBase { public: enum { Slot = 1 }; unsigned long Name; float DamageMultiplier; };
class CObjectAugmentationsDef : public CDefClassBase { public: enum { Slot = 2 }; };
template <class T> class CSlots { public:
    T Data[4]; unsigned int Count;
    unsigned int size() const { return Count; }
    T& operator[](unsigned int i) { return Data[i]; }
    const T& operator[](unsigned int i) const { return Data[i]; }
};
class CTCObjectAugmentations { public:
    CSlots<CDefPointer<const CThingBaseDef> > Augmentations; int Particles, Sounds;
    const CGameDefinitionManager& GetDefinitionManager() const { return g_manager; }
    void AttachAugmentationParticles() { ++Particles; }
    void SetupAugmentationSounds() { ++Sounds; }
    void AttachAugmentationToSlot(long augmentation_def_index, long slot);
};
// ---- row body (verbatim) ----
void CTCObjectAugmentations::AttachAugmentationToSlot(long augmentation_def_index, long slot)
{
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(augmentation_def_index, &def);
    CDefPointer<const CAugmentationDef> augmentation;
    if (def->GetSubDef(&augmentation))
        Augmentations[slot] = def;
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef plain, augmented; CAugmentationDef aug; augmented.Sub[1] = &aug;
    g_manager.Table[3] = &plain; g_manager.Table[5] = &augmented;
    CTCObjectAugmentations c; c.Augmentations.Count = 4; c.Particles = c.Sounds = 0;
    c.AttachAugmentationToSlot(3, 1);                          // no augmentation sub-def: slot left empty
    CHECK(c.Augmentations[1].IsNull() && c.Particles == 1 && c.Sounds == 1 && plain.Refs == 1 && g_manager.LastIndex == 3);
    c.AttachAugmentationToSlot(5, 2);                          // stored: the slot holds one more reference
    CHECK(c.Augmentations[2].Object == &augmented && augmented.Refs == 2 && aug.Refs == 1 && c.Particles == 2 && c.Sounds == 2);
    c.AttachAugmentationToSlot(5, 2);                          // same def again: unchanged
    CHECK(augmented.Refs == 2 && aug.Deleted == 0);
    if (fails == 0) printf("00768120_TEST PASS\n");
    return fails;
}
