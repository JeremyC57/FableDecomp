// Behaviour test for 00667b30: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CThingCreatureBase::GetRandomCombatAnimationFromName(const CCharString& name) const -- collect every combat-animation-set entry called `name` and return the one GetAppropriateCombatAnimationFromList picks
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
class CCharString { public: unsigned long Crc; CCharString(unsigned long crc) : Crc(crc) { } };
class CAnimationEntry { public: long BankIndex; };
class CAnimationSet { public:
    typedef std::vector<std::pair<unsigned long, CAnimationEntry> > TAnimMap;   // sorted by key (the name CRC)
    TAnimMap Anims;
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const
    {
        for (TAnimMap::const_iterator it = Anims.begin(); it != Anims.end(); ++it) if (it->first == name.Crc) return it;
        return Anims.end();
    }
};
class CAppearanceDef : public CDefClassBase { public: enum { Slot = 1 }; CAnimationSet Animation; };
static std::vector<long> g_seenBanks; static long g_pick = 0;
class CThingCreatureBase { public:
    CDefPointer<const CThingBaseDef> PBaseDef;
    const CAnimationSet& GetCombatAnimationSet() const
    {
        CDefPointer<const CAppearanceDef> appearance;
        PBaseDef->GetSubDef(&appearance);
        return appearance->Animation;
    }
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& anims) const
    {
        g_seenBanks.clear();
        for (size_t i = 0; i < anims.size(); ++i) g_seenBanks.push_back(anims[i]->BankIndex);
        return g_pick;
    }
    const CAnimationEntry& GetRandomCombatAnimationFromName(const CCharString& name) const;
};
// ---- row body (verbatim) ----
const CAnimationEntry& CThingCreatureBase::GetRandomCombatAnimationFromName(const CCharString& name) const
{
    const CAnimationSet& animations = GetCombatAnimationSet();
    std::vector<const CAnimationEntry*> candidates;
    CAnimationSet::TAnimMap::const_iterator first = animations.FindFirstAnimWithName(name);
    for (CAnimationSet::TAnimMap::const_iterator it = first; it != animations.Anims.end() && it->first == first->first; ++it)
        candidates.push_back(&it->second);
    return *candidates[GetAppropriateCombatAnimationFromList(candidates)];
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef thingDef; CAppearanceDef app; thingDef.Sub[1] = &app;
    unsigned long keys[6] = { 5, 9, 9, 9, 12, 12 };
    for (int i = 0; i < 6; ++i) { CAnimationEntry e = { 100 + i }; app.Animation.Anims.push_back(std::make_pair(keys[i], e)); }
    CThingCreatureBase creature; creature.PBaseDef.Object = &thingDef;
    g_pick = 2;
    const CAnimationEntry& e9 = creature.GetRandomCombatAnimationFromName(CCharString(9));
    CHECK(g_seenBanks.size() == 3 && g_seenBanks[0] == 101 && g_seenBanks[2] == 103);
    CHECK(&e9 == &app.Animation.Anims[3].second);
    g_pick = 1;
    const CAnimationEntry& e12 = creature.GetRandomCombatAnimationFromName(CCharString(12));
    CHECK(g_seenBanks.size() == 2 && &e12 == &app.Animation.Anims[5].second);
    g_pick = 0;
    const CAnimationEntry& e5 = creature.GetRandomCombatAnimationFromName(CCharString(5));
    CHECK(g_seenBanks.size() == 1 && &e5 == &app.Animation.Anims[0].second);
    CHECK(app.Refs == 1 && app.Deleted == 0 && thingDef.Refs == 1);
    creature.PBaseDef.Object = 0;
    if (fails == 0) printf("00667b30_TEST PASS\n");
    return fails;
}
