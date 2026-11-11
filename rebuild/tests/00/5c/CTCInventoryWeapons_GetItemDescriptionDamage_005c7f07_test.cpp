// Behaviour test for 005c7f07: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCInventoryWeapons::GetItemDescriptionDamage(CWideString& description, long defIndex) const -- for a weapon item: the live active melee weapon's damage and the live active ranged weapon's damage (its ability def's damage def value times the object-augmentations damage multiplier), then ConstructWeaponDescriptionDamage with the one matching the item's weapon class; other items go to the base description
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
#define FABLE_FASTCALL __fastcall
class CWeaponDef : public CDefClassBase { public: enum { Slot = 1 }; long Class; long Shot; };
class CThingShotDef : public CDefClassBase { public: float HitThingDamage; };
class CWideString { public: long Index; float Damage; int Variant; bool Pc; };
class CTCWeapon { public: float Damage; float GetDamage() const { return Damage; } static bool __fastcall IsMeleeWeaponClass(long c) { return c == 1; } };
class CTCObjectAugmentations { public: float Multiplier; float GetDamageMultiplier() const { return Multiplier; } };
class CBaseObject { public: virtual ~CBaseObject() { } };
class CThing : public CBaseObject { public:
    bool Dead; const CTCWeapon* Weapon; const CTCObjectAugmentations* Augmentations; CDefPointer<const CThingBaseDef> PBaseDef;
    bool IsDead() const { return Dead; }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool PeekTC(const CTCWeapon** out) const { if (!Weapon) return false; *out = Weapon; return true; }
    bool PeekTC(const CTCObjectAugmentations** out) const { if (!Augmentations) return false; *out = Augmentations; return true; }
};
template <class T> class CIntelligentPointer { public:
    T* Item; CBaseObject* GetPItem() const { return Item; }
    bool operator!=(int) const { return Item != 0; }
    T* operator->() const { return Item; }
};
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) { return thing != 0 && !thing->IsDead(); }
class CTCInventoryItem { public: static bool __fastcall IsWeapon(long index) { return index < 10; } };
class CShotLookup { public:
    CThingShotDef* Shots[4];
    struct CHandle { CDefPointeeBase* P; template <class T> operator CDefPointer<T>() const { CDefPointer<T> p; p.Set(P); return p; } };
    CHandle GetPDefFromGlobalIndex(long i) const { CHandle h; h.P = Shots[i]; return h; }
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const { return g_manager.GetDef(index, out); }
};
static CShotLookup g_shots;
namespace NInventory { class CTCInventoryBase { public:
    void GetItemDescription(CWideString& text, long item_def_index, bool pc) const { text.Index = item_def_index; text.Variant = 9; text.Pc = pc; }
}; }
class CTCInventoryWeapons : public NInventory::CTCInventoryBase { public:
    CIntelligentPointer<CThing> PActiveMeleeWeapon, PActiveRangedWeapon;
    const CShotLookup& GetDefinitionManager() const { return g_shots; }
    void Fill(CWideString& t, long i, float d, int v, bool pc) const { t.Index = i; t.Damage = d; t.Variant = v; t.Pc = pc; }
    void ConstructWeaponDescription(CWideString& t, long i, float d, bool pc) const { Fill(t, i, d, 1, pc); }
    void ConstructWeaponDescriptionNoDamageNorWeight(CWideString& t, long i, float d, bool pc) const { Fill(t, i, d, 2, pc); }
    void ConstructWeaponDescriptionWeight(CWideString& t, long i, float d) const { Fill(t, i, d, 3, false); }
    void ConstructWeaponDescriptionDamage(CWideString& t, long i, float d) const { Fill(t, i, d, 4, false); }
    void GetItemDescription(CWideString& text, long item_def_index, bool pc) const;
    void GetItemDescriptionNoDamageNorWeight(CWideString& text, long item_def_index, bool pc) const;
    void GetItemDescriptionWeight(CWideString& text, long item_def_index) const;
    void GetItemDescriptionDamage(CWideString& text, long item_def_index) const;
};
// ---- row body (verbatim) ----
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340

void CTCInventoryWeapons::GetItemDescriptionDamage(CWideString& text, long item_def_index) const
{
    if (CTCInventoryItem::IsWeapon(item_def_index)) {
        float melee_damage = 0.0f;
        float ranged_damage = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
            const CTCWeapon* weapon;
            static_cast<CThing*>(PActiveMeleeWeapon.GetPItem())->PeekTC(&weapon);
            melee_damage = weapon->GetDamage();
        }
        if (IS_THING_ALIVE(PActiveRangedWeapon)) {
            CDefPointer<const CWeaponDef> weapon_def;
            static_cast<CThing*>(PActiveRangedWeapon.GetPItem())->GetPBaseDef()->GetSubDef(&weapon_def);
            CDefPointer<const CThingShotDef> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weapon_def->Shot));
            ranged_damage = shot->HitThingDamage;
            const CTCObjectAugmentations* augmentations;
            if (static_cast<CThing*>(PActiveRangedWeapon.GetPItem())->PeekTC(&augmentations))
                ranged_damage *= augmentations->GetDamageMultiplier();
        }
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(item_def_index, &def);
        CDefPointer<const CWeaponDef> weapon_def;
        def->GetSubDef(&weapon_def);
        float damage = CTCWeapon::IsMeleeWeaponClass(weapon_def->Class) ? melee_damage : ranged_damage;
        ConstructWeaponDescriptionDamage(text, item_def_index, damage);
    } else
        NInventory::CTCInventoryBase::GetItemDescription(text, item_def_index, false);
}
// ---- harness ----
int main()
{
    ResetManager();
    CWeaponDef sword, bow; sword.Class = 1; bow.Class = 2; bow.Shot = 2;
    CThingBaseDef swordThing, bowThing; swordThing.Sub[1] = &sword; bowThing.Sub[1] = &bow;
    g_manager.Table[3] = &swordThing; g_manager.Table[4] = &bowThing;
    CThingShotDef arrow; arrow.HitThingDamage = 6.0f; g_shots.Shots[2] = &arrow;
    CTCWeapon meleeTC; meleeTC.Damage = 11.0f; CTCObjectAugmentations aug; aug.Multiplier = 1.5f;
    CThing melee, ranged; melee.Dead = false; melee.Weapon = &meleeTC; melee.Augmentations = 0;
    ranged.Dead = false; ranged.Weapon = 0; ranged.Augmentations = &aug; ranged.PBaseDef.Set(&bowThing);
    CTCInventoryWeapons inv; inv.PActiveMeleeWeapon.Item = &melee; inv.PActiveRangedWeapon.Item = &ranged;
    CWideString t; memset(&t, 0, sizeof(t));
    inv.GetItemDescriptionDamage(t, 3);                                     // a melee weapon item: the melee damage
    CHECK(t.Variant == 4 && t.Index == 3 && t.Damage == 11.0f && t.Pc == false);
    inv.GetItemDescriptionDamage(t, 4);                                     // a ranged item: the shot damage times the augmentation multiplier
    CHECK(t.Variant == 4 && t.Index == 4 && t.Damage > 8.99f && t.Damage < 9.01f);
    inv.GetItemDescriptionDamage(t, 12);                                    // not a weapon: the base description
    CHECK(t.Variant == 9 && t.Index == 12 && !t.Pc);
    CHECK(sword.Refs == 1 && bow.Refs == 1 && swordThing.Refs == 1 && bowThing.Refs == 2 && arrow.Refs == 1 && arrow.Deleted == 0);
    if (fails == 0) printf("005c7f07_TEST PASS\n");
    return fails;
}
